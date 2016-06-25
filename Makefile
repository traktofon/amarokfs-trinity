#############################################################################
# Makefile for building: amarokFS
# Generated by qmake (1.07a) (TQt 3.5.0) on: Sat Jun 25 17:13:43 2016
# Project:  amarokFS-xml.pro
# Template: app
# Command: $(QMAKE) -o Makefile amarokFS-xml.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -g -fvisibility=hidden -fvisibility-inlines-hidden -Wall -W -O2 -D_REENTRANT  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT
CXXFLAGS = -pipe -g -fvisibility=hidden -fvisibility-inlines-hidden -Wall -W -O2 -D_REENTRANT  -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/usr/share/tqt3/mkspecs/default -I. -I/usr/include/tqt -I/usr/include/tqt3 -I/opt/trinity/include -I/usr/include/tqt3
LINK     = g++
LFLAGS   = -luuid
LIBS     = $(SUBLIBS) -L/usr/share/tqt3/lib -L/usr/X11R6/lib -luuid -L/usr/share/tqt3/lib -L/opt/trinity/lib -ltdecore -ltdeio -lDCOP -ltdeui -ltqt-mt -lXext -lX11 -lm -lpthread
AR       = ar cqs
RANLIB   = 
MOC      = /usr/share/tqt3/bin/tqmoc
UIC      = /usr/share/tqt3/bin/tquic
QMAKE    = tqmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = ./

####### Files

HEADERS = amfs-xml.h \
		cb.h \
		label-command.h \
		amarokfs_config.h \
		label-pixmap.h
SOURCES = main.cpp \
		amfs-xml.cpp \
		cb.cpp \
		label-command.cpp \
		amarokfs_config.cpp \
		label-pixmap.cpp
OBJECTS = main.o \
		amfs-xml.o \
		cb.o \
		label-command.o \
		amarokfs_config.o \
		label-pixmap.o
FORMS = 
UICDECLS = 
UICIMPLS = 
SRCMOC   = moc_amfs-xml.cpp \
		moc_cb.cpp \
		moc_label-command.cpp \
		moc_amarokfs_config.cpp \
		moc_label-pixmap.cpp
OBJMOC = moc_amfs-xml.o \
		moc_cb.o \
		moc_label-command.o \
		moc_amarokfs_config.o \
		moc_label-pixmap.o
DIST	   = amarokFS-xml.pro
QMAKE_TARGET = amarokFS
DESTDIR  = 
TARGET   = amarokFS

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

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

$(OBJECTS): $(UICDECLS)

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: amarokFS-xml.pro  /usr/share/tqt3/mkspecs/default/qmake.conf /usr/share/tqt3/lib/libtqt-mt.prl
	$(QMAKE) -o Makefile amarokFS-xml.pro
qmake: 
	@$(QMAKE) -o Makefile amarokFS-xml.pro

dist: 
	@mkdir -p .tmp/amarokFS && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .tmp/amarokFS/ && ( cd `dirname .tmp/amarokFS` && $(TAR) amarokFS.tar amarokFS && $(GZIP) amarokFS.tar ) && $(MOVE) `dirname .tmp/amarokFS`/amarokFS.tar.gz . && $(DEL_FILE) -r .tmp/amarokFS

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:

yaccclean:
lexclean:
clean: mocclean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

main.o: main.cpp amfs-xml.h \
		amarokfs_config.h \
		cb.h \
		label-command.h \
		label-pixmap.h

amfs-xml.o: amfs-xml.cpp amfs-xml.h \
		amarokfs_config.h \
		cb.h \
		label-command.h \
		label-pixmap.h

cb.o: cb.cpp cb.h

label-command.o: label-command.cpp label-command.h

amarokfs_config.o: amarokfs_config.cpp amarokfs_config.h

label-pixmap.o: label-pixmap.cpp label-pixmap.h

moc_amfs-xml.o: moc_amfs-xml.cpp  amfs-xml.h cb.h \
		label-command.h \
		amarokfs_config.h \
		label-pixmap.h

moc_cb.o: moc_cb.cpp  cb.h 

moc_label-command.o: moc_label-command.cpp  label-command.h 

moc_amarokfs_config.o: moc_amarokfs_config.cpp  amarokfs_config.h 

moc_label-pixmap.o: moc_label-pixmap.cpp  label-pixmap.h 

moc_amfs-xml.cpp: $(MOC) amfs-xml.h
	$(MOC) amfs-xml.h -o moc_amfs-xml.cpp

moc_cb.cpp: $(MOC) cb.h
	$(MOC) cb.h -o moc_cb.cpp

moc_label-command.cpp: $(MOC) label-command.h
	$(MOC) label-command.h -o moc_label-command.cpp

moc_amarokfs_config.cpp: $(MOC) amarokfs_config.h
	$(MOC) amarokfs_config.h -o moc_amarokfs_config.cpp

moc_label-pixmap.cpp: $(MOC) label-pixmap.h
	$(MOC) label-pixmap.h -o moc_label-pixmap.cpp

####### Install

install_target: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)/usr/local/bin/" || $(MKDIR) "$(INSTALL_ROOT)/usr/local/bin/"
	-$(INSTALL_FILE) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"

uninstall_target: 
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) "$(INSTALL_ROOT)/usr/local/bin/"


install_images: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)/usr/share/amarokFS/images/" || $(MKDIR) "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/gradient.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/logo.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/logoB.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_end.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_end2.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_pause.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_pause2.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_play.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_play2.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_start.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_start2.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_stop.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"
	-$(INSTALL_FILE) "images/player_stop2.png" "$(INSTALL_ROOT)/usr/share/amarokFS/images/"


uninstall_images: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/gradient.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/logo.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/logoB.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_end.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_end2.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_pause.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_pause2.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_play.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_play2.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_start.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_start2.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_stop.png"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/images/player_stop2.png"
	-$(DEL_DIR) "$(INSTALL_ROOT)/usr/share/amarokFS/images/"


install_desktop: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)/usr/share/applications/" || $(MKDIR) "$(INSTALL_ROOT)/usr/share/applications/"
	-$(INSTALL_FILE) "amarokFS.desktop" "$(INSTALL_ROOT)/usr/share/applications/"


uninstall_desktop: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/applications/amarokFS.desktop"
	-$(DEL_DIR) "$(INSTALL_ROOT)/usr/share/applications/"


install_icon: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)/usr/share/icons/" || $(MKDIR) "$(INSTALL_ROOT)/usr/share/icons/"
	-$(INSTALL_FILE) "amarokFS.png" "$(INSTALL_ROOT)/usr/share/icons/"


uninstall_icon: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/icons/amarokFS.png"
	-$(DEL_DIR) "$(INSTALL_ROOT)/usr/share/icons/"


install_defaultTheme: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)/usr/share/amarokFS/" || $(MKDIR) "$(INSTALL_ROOT)/usr/share/amarokFS/"
	-$(INSTALL_FILE) "theme.xml" "$(INSTALL_ROOT)/usr/share/amarokFS/"


uninstall_defaultTheme: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/amarokFS/theme.xml"
	-$(DEL_DIR) "$(INSTALL_ROOT)/usr/share/amarokFS/"


install_docs: all 
	@$(CHK_DIR_EXISTS) "$(INSTALL_ROOT)/usr/share/doc/amarokFS/" || $(MKDIR) "$(INSTALL_ROOT)/usr/share/doc/amarokFS/"
	-$(INSTALL_FILE) "README" "$(INSTALL_ROOT)/usr/share/doc/amarokFS/"
	-$(INSTALL_FILE) "theme-howto.txt" "$(INSTALL_ROOT)/usr/share/doc/amarokFS/"


uninstall_docs: 
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/doc/amarokFS/README"
	-$(DEL_FILE) -r "$(INSTALL_ROOT)/usr/share/doc/amarokFS/theme-howto.txt"
	-$(DEL_DIR) "$(INSTALL_ROOT)/usr/share/doc/amarokFS/"


install: install_target install_images install_desktop install_icon install_defaultTheme install_docs  

uninstall: uninstall_target uninstall_images uninstall_desktop uninstall_icon uninstall_defaultTheme uninstall_docs  

