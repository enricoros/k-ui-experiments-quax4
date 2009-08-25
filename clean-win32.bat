@echo off

rem ************************************************************
rem * $Id: clean-win32.bat,v 1.1 2003/06/12 22:44:08 claudiuc Exp $
rem *
rem * Description: Clean after compile Quax for WIN32
rem *
rem ************************************************************


set MVSPATH=C:\Program Files\Microsoft Visual Studio\VC98\Bin
set PATH=%PATH%;%MVSPATH%;

cd src
nmake /f "quax.mak" CFG="quax - Win32 Release" clean
cd ..


