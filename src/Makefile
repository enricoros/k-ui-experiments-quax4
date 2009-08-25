
ifeq ($(PREFIX),)
  PREFIX = /usr/local
endif


####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -O2 -D_REENTRANT  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -O2 -D_REENTRANT  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I$(QTDIR)/mkspecs/default -I. -I$(QTDIR)/include
LINK     = g++
LFLAGS   = 
LIBS     = $(SUBLIBS)  -Wl,-rpath,$(QTDIR)/lib -L$(QTDIR)/lib -L/usr/X11R6/lib -lqt-mt -lXext -lX11 -lm -lpthread
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = quax.h
SOURCES = quax.cpp \
		main.cpp
OBJECTS = quax.o \
		main.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = moc_quax.cpp
OBJMOC = moc_quax.o
DIST	   = src.pro
QMAKE_TARGET = quax
DESTDIR  = 
TARGET   = quax

####### Implicit rules

.SUFFIXES: .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: $(TARGET)

$(TARGET): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(LIBS)

mocables: $(SRCMOC)

$(MOC): 
	( cd $(QTDIR)/src/moc ; $(MAKE) )

dist: 
	@mkdir -p .tmp/quax && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/quax/ && ( cd `dirname .tmp/quax` && $(TAR) quax.tar quax && $(GZIP) quax.tar ) && $(MOVE) `dirname .tmp/quax`/quax.tar.gz . && $(DEL_FILE) -r .tmp/quax

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:

yaccclean:
lexclean:
clean: mocclean
	-$(DEL_FILE) $(OBJECTS) 
	-$(DEL_FILE) *~ core *.core
	-$(DEL_FILE) $(TARGET)


####### Sub-libraries

distclean: clean


FORCE:

####### Compile

quax.o: quax.cpp mag.xpm \
		mag-alpha.xpm \
		exit.xpm \
		viewmag.xpm \
		viewmagin.xpm \
		viewmagout.xpm \
		look.xpm \
		left.xpm \
		right.xpm \
		icon.xpm \
		help.xpm \
		quax.h \
		../config.h

main.o: main.cpp quax.h \
		../config.h

moc_quax.o: moc_quax.cpp quax.h ../config.h

moc_quax.cpp: $(MOC) quax.h
	$(MOC) quax.h -o moc_quax.cpp

####### Install

install_man: 
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/share/man/man1/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/share/man/man1/
	-$(COPY_FILE) "quax.1" "$(INSTALL_ROOT)$(PREFIX)/share/man/man1/"


uninstall_man: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)$(PREFIX)/share/man/man1/quax.1"
	-$(DEL_DIR) "$(INSTALL_ROOT)$(PREFIX)/share/man/man1/"


install_target: 
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)$(PREFIX)/bin/ || $(MKDIR) $(INSTALL_ROOT)$(PREFIX)/bin/
	$(DEL_FILE) "$(INSTALL_ROOT)$(PREFIX)/bin/$(QMAKE_TARGET)"
	-$(COPY) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)$(PREFIX)/bin/$(QMAKE_TARGET)"
	-strip "$(INSTALL_ROOT)$(PREFIX)/bin/$(QMAKE_TARGET)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)$(PREFIX)/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) "$(INSTALL_ROOT)$(PREFIX)/bin/"


install: all install_man install_target

uninstall: uninstall_man uninstall_target

