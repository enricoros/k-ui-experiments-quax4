
ifeq ($(PREFIX),)
  PREFIX = /usr/local
endif

MAKEFILE =	Makefile
QMAKE    =	qmake
DEL_FILE =    rm -f
SUBTARGETS =	 \
		sub-src

all: $(SUBTARGETS)

src/$(MAKEFILE): 
	cd src && $(QMAKE)  -o $(MAKEFILE)
sub-src: src/$(MAKEFILE) FORCE
	cd src && $(MAKE) -f $(MAKEFILE)

all: $(SUBTARGETS)
qmake_all: src/$(MAKEFILE)
	 ( [ -d src ] && cd src ; grep "^qmake_all:" $(MAKEFILE) && $(MAKE) -f $(MAKEFILE) qmake_all; ) || true
clean uninstall install uiclean mocclean lexclean yaccclean: qmake_all FORCE
	 ( [ -d src ] && cd src ; $(MAKE) -f $(MAKEFILE) $@; ) || true
distclean: qmake_all FORCE
	 ( [ -d src ] && cd src ; $(MAKE) -f $(MAKEFILE) $@; $(DEL_FILE) $(MAKEFILE); ) || true

FORCE:

