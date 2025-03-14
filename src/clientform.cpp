#include "toolkit.h"
#include "setting.h"
#include "clientskt.h"
#include "clientform.h"

#define SET_SEC_CLT  "client"
#define SET_KEY_CLT  "/client"

#define SET_KEY_CMBIP  "/ip"
#define SET_KEY_CMBPT  "/port"

#define SET_VAL_LGCLT  "log_client"

ClientForm::ClientForm(QWidget* p, Qt::WFlags f)
:BaseForm(p, f),m_client(0)
{
	m_ui.setupUi(this);

    m_timer_0 = new QTimer(this);
    m_timer_1 = new QTimer(this);
    m_timer_2 = new QTimer(this);
    m_timer_3 = new QTimer(this);
}

ClientForm::~ClientForm()
{
	unplug();
	saveConfig();
}

bool ClientForm::initForm()
{
	initCounter(m_ui.labRecv, m_ui.labSend);
	initLogger(m_ui.chkLog, m_ui.btnClear, m_ui.treeOutput, m_ui.txtOutput);

	bindBuffer(0, m_ui.edtBuf0, m_ui.btnSend0, 0);
	bindBuffer(1, m_ui.edtBuf1, m_ui.btnSend1, 0);
	bindBuffer(2, m_ui.edtBuf2, m_ui.btnSend2, 0);
	bindBuffer(3, m_ui.edtBuf3, m_ui.btnSend3, 0);

    bindTimer(m_ui.edtTime0, m_ui.chkTimer0, m_timer_0, m_ui.btnSend0);
    bindTimer(m_ui.edtTime1, m_ui.chkTimer1, m_timer_1, m_ui.btnSend1);
    bindTimer(m_ui.edtTime2, m_ui.chkTimer2, m_timer_2, m_ui.btnSend2);
    bindTimer(m_ui.edtTime3, m_ui.chkTimer3, m_timer_3, m_ui.btnSend3);

	connect(m_ui.btnTcp, SIGNAL(clicked(bool)), this, SLOT(trigger(bool)));
	connect(m_ui.btnUdp, SIGNAL(clicked(bool)), this, SLOT(trigger(bool)));

	return true;
}

void ClientForm::initConfig()
{
	QString ssc(SET_SEC_CLT);
	Setting::lord(ssc+SET_KEY_CMBIP, SET_PFX_CMBITM, *m_ui.cmbAddr);
	Setting::lord(ssc+SET_KEY_CMBPT, SET_PFX_CMBITM, *m_ui.cmbPort);

	QString skl(SET_SEC_DIR); 
	skl += SET_KEY_LOG;
	skl = Setting::get(skl, SET_KEY_CLT, SET_VAL_LGCLT);
	setProperty(SET_SEC_DIR, skl);

	TK::initNetworkInterfaces(m_ui.cmbAddr);
}

void ClientForm::saveConfig()
{
	QString ssc(SET_SEC_CLT);
	Setting::save(ssc+SET_KEY_CMBIP, SET_PFX_CMBITM, *m_ui.cmbAddr);
	Setting::save(ssc+SET_KEY_CMBPT, SET_PFX_CMBITM, *m_ui.cmbPort);

	QString skl(SET_SEC_DIR); 
	skl += SET_KEY_LOG;
	Setting::set(skl, SET_KEY_CLT, property(SET_SEC_DIR).toString());
}

bool ClientForm::initHotkeys()
{
	bindFocus(m_ui.cmbAddr, Qt::Key_Escape);
	bindClick(m_ui.btnTcp,  Qt::CTRL + Qt::Key_T);
	bindClick(m_ui.btnUdp,  Qt::CTRL + Qt::Key_U);
	return true;
}

void ClientForm::unplug()
{
	ClientSkt* client = 0;

	if (lock())
	{
		client = m_client;
		m_client = NULL;
		unlock();
	}

	if (client)
		client->unplug();
}

void ClientForm::unpluged()
{
	ClientSkt* c = qobject_cast<ClientSkt*>(sender());
	if (!c) return;

	if (lock())
	{
		if (c==m_client)
			m_client = NULL;

		unlock();
	}

	if (c->name().contains(TK::socketTypeName(true)))
		TK::resetPushBox(m_ui.btnTcp);
	else
		TK::resetPushBox(m_ui.btnUdp);

	c->disconnect(this);
	c->deleteLater();
}

void ClientForm::trigger(bool checked)
{
	unplug();

	m_ui.cmbAddr->setDisabled(checked);
	m_ui.cmbPort->setDisabled(checked);

	QToolButton* btn = qobject_cast<QToolButton*>(sender());
	if (checked && !plug(btn==m_ui.btnTcp))
		btn->click();
}

bool ClientForm::plug(bool istcp)
{
	ClientSkt* skt = 0;

	lock();

	if (m_client)
	{
		m_client->disconnect(this);
		m_client->deleteLater();
		m_client = NULL;
	}

	IPAddr addr;
	bool res  = TK::popIPAddr(m_ui.cmbAddr, m_ui.cmbPort, addr);

	if (res)
	{
		if (istcp)
			m_client = new ClientSktTcp();
		else
			m_client = new ClientSktUdp();

		if (!m_client)
		{
			res = false;
		}
		else
		{
			connect(m_client, SIGNAL(unpluged()), this, SLOT(unpluged()));
			connect(m_client, SIGNAL(message(const QString&)), this, SIGNAL(output(const QString&)));
			connect(m_client, SIGNAL(dumpbin(const QString&,const char*,quint32)), this, SIGNAL(output(const QString&,const char*,quint32)));
			connect(m_client, SIGNAL(countRecv(qint32)), this, SLOT(countRecv(qint32)));
			connect(m_client, SIGNAL(countSend(qint32)), this, SLOT(countSend(qint32)));
		
			skt = m_client;
		}
	}

	unlock();

	if (res && skt)
	{
		res = skt->plug(addr.ip, addr.port);

		if (res)
			TK::pushIPAddr(m_ui.cmbAddr, m_ui.cmbPort);
	}

	return res;
}

void ClientForm::send(const QString& data, const QString&)
{
	if (m_client && lock(1000))
	{
		m_client->send(data);
		unlock();
	}
}

