# Sokit

Sokit is a TCP & UDP package send / receive / transfer tool

## Usage

sokit-cn.lan is language file, if rename or delete it, then use default english UI

## Development

This project should be built within Qt4.8.7 environment.

### Windows

1. Download and install packages:
    - MinGW: [i686-4.8.2-release-posix-dwarf-rt_v3-rev3](http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/4.8.2/threads-posix/dwarf/i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z/download)
    - [qt-opensource-windows-x86-mingw482-4.8.7](https://download.qt.io/archive/qt/4.8/4.8.7/qt-opensource-windows-x86-mingw482-4.8.7)
    - [Qt Creator 2.8.1](https://download.qt.io/archive/qtcreator/2.8/2.8.1/qt-creator-windows-opensource-2.8.1.exe)

2. Configure Path
    - Add `C:\mingw32\bin` to `Path`
    - Add `C:\Qt\4.8.7\bin` to `Path`
    - Add `C:\Qt\4.8.7` to `QTDIR`

3. Configure Qt Static compilation
    - Reference: <https://www.cnblogs.com/Bonker/p/3456898.html>

### Linux

> To be done