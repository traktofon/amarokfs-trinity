 
#ifndef AMFS_XML_H
#define AMFS_XML_H

#include "cb.h"
#include "label-command.h"
#include "amarokfs_config.h"
#include "label-pixmap.h"
		
#include <tqdom.h>
#include <tqwidget.h>
#include <tqpushbutton.h>
#include <tqvariant.h>
#include <tqprogressbar.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>
#include <tqapplication.h>
#include <tqtimer.h>
#include <tqstring.h>
#include <tqimage.h>
#include <tqdialog.h>
#include <tqaction.h>
#include <tqpopupmenu.h>
#include <tdeaboutapplication.h>
				
#include <vector>
#include <dcopclient.h>		
#include <iostream>
		
	
class AmfsXML : public TQWidget
{
	TQ_OBJECT

public:
	AmfsXML( TQWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
	~AmfsXML();

 private:
	TQDomDocument domTree;
	AKControlButton *cb;
	TQLabel *b;
	TQPushButton* pushButton1;

private:
	TQString getDCOPDataText(TQCString func);
	TQStringList getDCOPDataSQL(TQString query);
	int getDCOPDataInt(TQCString func);
	void sendPlayerCommand(TQCString command);
	TQImage getCoverPath(TQString artist, TQString album, bool transDef=FALSE);
	void restoreScreenSaver();
	void restoreAmarokOSD();
	
	void updateTrackLabels();
	void updateInstantLabels();
	void updateCover();
	void updateGradient();
	
	void wheelEvent ( TQWheelEvent * e );
	void contextMenuEvent(TQContextMenuEvent *event);
	void mouseMoveEvent( TQMouseEvent *e );
	void closeEvent( TQCloseEvent* ce );
			
			
	int topOffset, leftOffset;
	bool isSetDisScrSvr;
	bool isSetDisAmaOSD;
	bool isSetDisThemeHint;
	
	//vector to store the buttons;
	std::vector<AKControlButton*> controlButtons;
	//vector to store labels that get updated every second
	std::vector<AKLabelCommand*> labelsInstant;
	//vector to store labels that get update upon track change
	std::vector<AKLabelCommand*> labelsTrack;
	//vector to store static labels
	std::vector<AKLabelCommand*> labelsStatic;
		//vector to store pixmap labels
	std::vector<AKLabelPixmap*> labelsPixmap;
	
	TQPopupMenu* contextMenu;
	DCOPClient* dcopClient;
	TQProgressBar* progressBar;
	TQLabel *cdcover, *gradient, *themeHint;
	int cdcoverPosX, cdcoverPosY;
	TQPixmap cdimagePixmap, reflectionPixmap, gradientPixmap;
	TQTimer* timer, *hintTimer, *mouseTimer;
	AKLabelCommand *lInstant, *lTrack, *lStatic;
	AKLabelPixmap *lPixmap;
	TQString nowPlaying;
	TQString nextSongIntroduction, nextSongFormat;
	TQLabel *nextSongs;
	int nextSongCount;
	
	AmarokFS_config *configWindow;


private slots:
	void updateTime();
	void updateNextSongs();
	void controlButtonClicked(TQCString command);
	void launchConfig();
	void launchAbout();
	void hideHint();
	void hideMouse(bool now = TRUE);

public:
	TQAction *reloadAct, *setupAct, *closeAct, *aboutAct;
	TDEAboutApplication* aboutWindow;
};

#endif
 
