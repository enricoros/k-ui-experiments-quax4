# Microsoft Developer Studio Generated NMAKE File, Based on quax.dsp
!IF "$(CFG)" == ""
CFG=quax - Win32 Debug
!MESSAGE No configuration specified. Defaulting to quax - Win32 Debug.
!ENDIF

!IF "$(CFG)" != "quax - Win32 Release" && "$(CFG)" != "quax - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "quax.mak" CFG="quax - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "quax - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "quax - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF



#####################################################################################


!IF  "$(CFG)" == "quax - Win32 Release"

ALL : quax.exe

CLEAN :
	-@erase "vc60.idb"
	-@erase "vc60.pdb"
	-@erase "quax.exe"
	-@erase "quax.ilk"
	-@erase "quax.pdb"
	-@erase "main.obj"
	-@erase "moc_quax.obj"
	-@erase "moc_quax.cpp"
	-@erase "quax.obj"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /O2 /I "$(QTDIR)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /D "NO_DEBUG" /Fo"./" /Fd"./" /FD /c

.c.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.c.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"quax.bsc"
BSC32_SBRS= 

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\quax.pdb" /machine:I386 /out:"quax.exe"
LINK32_OBJS= \
	"main.obj" \
	"moc_quax.obj" \
	"quax.obj"

quax.exe : $(DEF_FILE) $(LINK32_OBJS)
  $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<


!ELSEIF  "$(CFG)" == "quax - Win32 Debug"

ALL : quax.exe

CLEAN :
	-@erase "vc60.idb"
	-@erase "vc60.pdb"
	-@erase "quax.exe"
	-@erase "quax.ilk"
	-@erase "quax.pdb"
	-@erase "main.obj"
	-@erase "moc_quax.obj"
	-@erase "moc_quax.cpp"
	-@erase "quax.obj"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /ZI /I "$(QTDIR)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /Fo"./" /Fd"./" /FD /GZ /c

.c.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.c.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"quax.bsc"
BSC32_SBRS= 

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QTDIR)\lib\qt-mt230nc.lib $(QTDIR)\lib\qtmain.lib /nologo /subsystem:windows /incremental:yes /pdb:"quax.pdb" /debug /machine:I386 /nodefaultlib:"libc" /out:"quax.exe" /pdbtype:sept
LINK32_OBJS= \
	"main.obj" \
	"moc_quax.obj" \
	"quax.obj"

quax.exe : $(DEF_FILE) $(LINK32_OBJS)
  $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF



#####################################################################################

!IF "$(CFG)" == "quax - Win32 Release" || "$(CFG)" == "quax - Win32 Debug"


###########  main.cpp
SOURCE=main.cpp
!IF  "$(CFG)" == "quax - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /O2 /I "$(QTDIR)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /D "NO_DEBUG" /Fo"./" /Fd"./" /FD /c
main.obj : $(SOURCE)  quax.h
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<

!ELSEIF  "$(CFG)" == "quax - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /ZI /Od /I "$(QTDIR)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /Fo"./" /Fd"./" /FD /GZ /c
main.obj : $(SOURCE) quax.h
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<

!ENDIF



###########  moc_quax.cpp
SOURCE=moc_quax.cpp
!IF  "$(CFG)" == "quax - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /O2 /I "$(QTDIR)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /D "NO_DEBUG" /Fo"./" /Fd"./" /FD /c
moc_quax.obj : $(SOURCE) quax.h
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<

!ELSEIF  "$(CFG)" == "quax - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /ZI /Od /I "$(QTDIR)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /Fo"./" /Fd"./" /FD /GZ /c
moc_quax.obj : $(SOURCE) quax.h
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<

!ENDIF


###########  quax.cpp
SOURCE=quax.cpp
!IF  "$(CFG)" == "quax - Win32 Release"

quax.obj : $(SOURCE)

!ELSEIF  "$(CFG)" == "quax - Win32 Debug"

quax.obj : $(SOURCE) quax.h

!ENDIF


###########  quax.h
SOURCE=quax.h
!IF  "$(CFG)" == "quax - Win32 Release"

moc_quax.cpp : $(SOURCE)
	<<tempfile.bat
	@echo off
	%QTDIR%\bin\moc.exe quax.h -o moc_quax.cpp
<<

!ELSEIF  "$(CFG)" == "quax - Win32 Debug"

moc_quax.cpp : $(SOURCE)
	<<tempfile.bat
	@echo off
	%QTDIR%\bin\moc.exe quax.h -o moc_quax.cpp
<<

!ENDIF


!ENDIF

