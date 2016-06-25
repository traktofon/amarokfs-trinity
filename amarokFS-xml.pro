TEMPLATE	= app
TARGET		= amarokFS

CONFIG		+= qt warn_on release

INCLUDEPATH += /usr/include/tqt
INCLUDEPATH += /usr/share/tqt3/include
INCLUDEPATH += /opt/trinity/include
		
				
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

LIBS += -L/usr/share/tqt3/lib
LIBS += -L/opt/trinity/lib
LIBS += -ltdecore
LIBS += -ltdeio
LIBS += -lDCOP
LIBS += -ltdeui
		
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
		
