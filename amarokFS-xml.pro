TEMPLATE	= app
TARGET		= amarokFS

CONFIG		+= qt warn_on release

INCLUDEPATH += /usr/include/kde
INCLUDEPATH += /usr/local/include/kde
INCLUDEPATH += /usr/include
INCLUDEPATH += /opt/kde3/include 
INCLUDEPATH += /opt/kde/include
INCLUDEPATH += /usr/kde/3.5/include
		
				
HEADERS	= amfs-xml.h \
	cb.h \
	label-command.h \
	amarokfs_config.h \
	label-pixmap.h
		
SOURCES	= main.cpp \
	amfs-xml.cpp \
	cb.cpp \
	label-command.cpp \
	amarokfs_config.cpp \
	label-pixmap.cpp

LIBS += -lkdecore
LIBS += -L/opt/kde3/lib
LIBS += -L/usr/local/kde3/lib
LIBS += -L/usr/lib/qt3
LIBS += -L/opt/kde/lib
LIBS += -L/usr/kde/3.5/lib
LIBS += -lkio
		
images.path = /usr/share/amarokFS/images
images.files = images/*

defaultTheme.path = /usr/share/amarokFS
defaultTheme.files = theme.xml
		
target.path = /usr/local/bin
		
icon.files = amarokFS.png
icon.path = /usr/share/icons/
		
desktop.files = amarokFS.desktop
desktop.path = /usr/share/applications/
		
docs.files = README
docs.files += theme-howto.txt
docs.path = /usr/share/doc/amarokFS


INSTALLS += target
INSTALLS += images
INSTALLS += desktop
INSTALLS += icon
INSTALLS += defaultTheme
INSTALLS += docs
		
