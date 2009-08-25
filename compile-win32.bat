@echo off

rem ************************************************************
rem * $Id: compile-win32.bat,v 1.2 2003/06/12 22:44:08 claudiuc Exp $
rem *
rem * Description: Compile Quax for WIN32
rem *
rem ************************************************************

set QTDIR=C:\QT
set MVSPATH=C:\Program Files\Microsoft Visual Studio\VC98\Bin
set PATH=%PATH%;%MVSPATH%;

cd src
nmake /f "quax.mak" CFG="quax - Win32 Release"
cd ..


