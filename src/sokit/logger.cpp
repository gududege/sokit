#include <QDate>
#include <QDir>
#include <QTextStream>
#include <QKeySequence>
#include <QApplication>
#include <QClipboard>
#include <QTreeWidget>
#include <QPlainTextEdit>
#include <QRegExp>
#include "toolkit.h"
#include "setting.h"
#include "logger.h"

#define SET_MAX_LOGITM  100
#define SET_MAX_LOGTRM  30
#define SET_KEY_LOGSIZE "/log/size"
#define SET_VAL_LOGSIZE "10MB"

Logger::Logger(QObject *parent)
: QObject(parent),m_chkWrite(0),m_treeOut(0),m_textOut(0)
{
}

Logger::~Logger()
{
	m_file.close();
}

void Logger::init(QTreeWidget* o, QCheckBox* w, QPlainTextEdit* d)
{
	Setting::set(SET_SEC_CFG, SET_KEY_LOGSIZE, Setting::get(SET_SEC_CFG, SET_KEY_LOGSIZE, SET_VAL_LOGSIZE).trimmed());
	m_maxSize = parseFileSize(Setting::get(SET_SEC_CFG, SET_KEY_LOGSIZE, SET_VAL_LOGSIZE).trimmed());

	m_cmlog.clear();
	m_cmtxt.clear();	

	if (m_treeOut)
		m_treeOut->disconnect(this);

	if (m_textOut)
		m_textOut->disconnect(this);

	if (m_chkWrite)
		m_chkWrite->disconnect(this);

    m_treeOut = o;
	m_textOut = d;
    m_chkWrite = w;

	if (m_treeOut && m_textOut && m_chkWrite)
	{
		QList<QKeySequence> ks;
		ks << QKeySequence(Qt::CTRL + Qt::Key_D);

		QAction* copy = new QAction(tr("Copy"), this);
		copy->setShortcuts(QKeySequence::Copy);
		connect(copy, SIGNAL(triggered()), this, SLOT(copy()));

		QAction* clear = new QAction(tr("Clear"), this);
		clear->setShortcuts(ks);
		connect(clear, SIGNAL(triggered()), this, SIGNAL(clearLog()));

		QAction* all = new QAction(tr("Select All"), this);
		all->setShortcuts(QKeySequence::SelectAll);
        connect(all, SIGNAL(triggered()), m_textOut, SLOT(selectAll()));

		m_cmlog.addAction(copy);
		m_cmlog.addSeparator();
		m_cmlog.addAction(clear);

		m_cmtxt.addAction(copy);
		m_cmtxt.addSeparator();
		m_cmtxt.addAction(all);

		QPalette pal = m_textOut->palette();
		pal.setBrush(QPalette::Base, m_treeOut->palette().brush(QPalette::Window));
		m_textOut->setPalette(pal);

		m_treeOut->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(m_treeOut, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ctxmenu(const QPoint&)));
		connect(m_treeOut, SIGNAL(itemSelectionChanged()), this, SLOT(syncOutput()));

		m_textOut->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(m_textOut, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ctxmenu(const QPoint&)));
	}
}

void Logger::syncOutput()
{
	QList<QTreeWidgetItem*> list = m_treeOut->selectedItems();
	if (!list.isEmpty())
		m_textOut->setPlainText(list.first()->text(0));
	else
		m_textOut->clear();
}

void Logger::ctxmenu(const QPoint& pos)
{
	if (sender() == (QObject*)m_treeOut)
		m_cmlog.exec(m_treeOut->mapToGlobal(pos));
	else
		m_cmtxt.exec(m_textOut->mapToGlobal(pos));
}

void Logger::copy()
{
	if (sender() == (QObject*)m_treeOut)
	{
		QList<QTreeWidgetItem*> list = m_treeOut->selectedItems();
		if (!list.isEmpty())
			QApplication::clipboard()->setText(list.first()->text(0));
	}
	else
	{
		m_textOut->copy();
	}
}

qint64 Logger::parseFileSize(const QString& sizeStr) {
    QRegExp regex("^(\\d+)([kKmMgG]?[bB]?)$");
    if (regex.indexIn(sizeStr) != -1) {
        QString numberStr = regex.cap(1);
        QString unitStr = regex.cap(2).toLower();

        qint64 number = numberStr.toLongLong();
        if (unitStr.isEmpty()) {
            return number;
        } else if (unitStr.startsWith('k')) {
            return number * 1024;
        } else if (unitStr.startsWith('m')) {
            return number * 1024 * 1024;
        } else if (unitStr.startsWith('g')) {
            return number * 1024 * 1024 * 1024;
        }
    }
    return parseFileSize(SET_VAL_LOGSIZE); // parse error
}

const QString Logger::getLogFileName()
{
    int i = 0;
    while (2 > i++)
    {
        if (!m_dir.isEmpty())
        {
            QDir d;
            if (d.exists(m_dir) || d.mkpath(m_dir)) {
                i = 0;
                break;
            }
        }

        m_dir = Setting::path() + "/" + property(SET_SEC_DIR).toString();
    }

    QString logFileName = m_dir + QDir::separator() + QDate::currentDate().toString("yyyy-MM-dd.log");
    QFile file(logFileName);
    if (file.exists()) {
        qint64 fileSize = file.size();
        if (fileSize >= m_maxSize) {
            QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz");
            QString newFileName = m_dir + QDir::separator() + timestamp + ".log";
            if (file.rename(newFileName)) {
                logFileName = m_dir + QDir::separator() + QDate::currentDate().toString("yyyy-MM-dd.log");
            }
        }
    }

    return (i==2) ? QString() : logFileName;
}

void Logger::writeLogFile(const QString& info)
{
	if (!m_chkWrite->isChecked())
		return;

	m_file.close();
	m_file.setFileName(getLogFileName());

	if (m_file.open(QIODevice::Append|
					QIODevice::WriteOnly|
					QIODevice::Text))
	{
		QByteArray a(info.toUtf8());

		const char* d = a.data();
		for (int n=a.size(); n>0;)
		{
			int w = m_file.write(d, n);

			d += w;
			n -= w;
		}

		m_file.close();
	}
}

void Logger::clear()
{
	m_treeOut->clear();
	m_textOut->clear();
}

void Logger::output(const QString& info)
{
	output("MSG", info);
}

void Logger::output(const char* buf, uint len)
{
	output("DAT", buf, len);
}

void Logger::pack()
{
	if (m_treeOut->topLevelItemCount() > SET_MAX_LOGITM)
		m_treeOut->model()->removeRows(0, SET_MAX_LOGTRM);

	m_treeOut->scrollToBottom();
}

QTreeWidgetItem* Logger::appendLogEntry(QTreeWidgetItem* p, const QString& t)
{
	QTreeWidgetItem* res = new QTreeWidgetItem(p);
	if (res)
	{
		res->setText(0, t);

		if (p)
		{
			p->addChild(res);
		}
		else
		{
			m_treeOut->addTopLevelItem(res);
			m_textOut->setPlainText(t);
		}
	}
	return res;
}

void Logger::output(const QString& title, const QString& info)
{
	QTreeWidgetItem* it = new QTreeWidgetItem(0);
	if (!it) return;

	QString lab(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz "));
	
	lab += title;
	lab += ' ';
	lab += info;

	appendLogEntry(0, lab);

	pack();

	lab += '\n';
	lab += '\n';

	writeLogFile(lab);
}

void Logger::output(const QString& title, const char* buf, quint32 len)
{
	QString lab(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss.zzz "));
	
	QTextStream out(&lab);

	out << title
		<< " <" << len << "> "
		<< TK::bin2ascii(buf, len);

	QString hex = TK::bin2hex(buf, len);

	QTreeWidgetItem* it = appendLogEntry(0, lab);
	if (it)
	{
		appendLogEntry(it, hex);

		pack();
	}

	out << '\n' << hex << '\n' << '\n';

	writeLogFile(lab);
}
