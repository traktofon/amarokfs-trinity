#include "amfs-xml.h"
#include "amarokfs_config.h"

#define COVERSIZE (500)

#include <kapp.h>
#include <tdeconfig.h>
#include <kiconloader.h>
#include <tqiconset.h>

#include <tqfile.h>
#include <tqdom.h>
#include <tqwidget.h>
#include <tqdialog.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <tqmessagebox.h>
#include <tqerrormessage.h>
#include <tqregexp.h>

using namespace std;

		
AmfsXML::AmfsXML( TQWidget* parent, const char* name, WFlags fl )
    : TQWidget( parent, name, fl ) {
	
	
	progressBar = (TQProgressBar *) 0;
	cdcover = (TQLabel *) 0;
	gradient = (TQLabel *) 0;
	
	//read the setting whether the user wants to disable the screensaver
	kapp->config()->setGroup ("Settings");
	isSetDisScrSvr =  (bool) ((kapp->config()->readEntry ("DisableScreenSaver", "0")).toInt());
	isSetDisAmaOSD =  (bool) ((kapp->config()->readEntry ("DisableAmarokOSD", "0")).toInt());
	isSetDisThemeHint =  (bool) ((kapp->config()->readEntry ("DisableThemeHint", "0")).toInt());
	
	//load the path from user theme
	kapp->config()->setGroup ("Theme");
	TQString themeCustomPath = kapp->config()->readEntry ("KUrl", "");
	//strip the filename to get the directory containing the file
	TQString themeCustomPrepath = themeCustomPath;
	themeCustomPrepath.replace(TQFileInfo(themeCustomPath).fileName(), "");
	
	//cout << themeCustomPath << "\n";
	//cout << themeCustomPrepath << "\n";
	
	//set the default path
	TQString themePath = "/usr/share/amarokFS/theme.xml";
	//default prepath for the images directory	
	TQString themeImagesPrePath = "/usr/share/amarokFS/";
		
	//if the custom theme file exists, use it, otherwise use the default theme
	if(TQFile(themeCustomPath).exists()) {
		themePath = themeCustomPath;
		themeImagesPrePath = themeCustomPrepath;
		
	}
	cout << "Using skin: " << themePath << "\n";
	
	TQFile opmlFile( themePath );
	if ( !opmlFile.open( IO_ReadOnly ) ) {
		cout << "Critical Error\n";
		cout << "Cannot open file \n";
		TQMessageBox::critical( 0, "amarokFS", "The theme file is not readable.\nPlease select a new theme \nand start the application again.");
		launchConfig();
		close();
	
	}

	if ( !domTree.setContent( &opmlFile ) ) {
		cout << "Critical Error\n";
		cout << "Cannot parse file \n";
		opmlFile.close();
		TQMessageBox::critical( 0, "amarokFS", "The theme file is broken.\nPlease select a new theme \nand start the application again.");
		launchConfig();
		close();
	}
	opmlFile.close();
	
	//parse the document
	TQDomElement root = domTree.documentElement();
	
	//get the theme resolution and background color
	int themeWidth = 0, themeHeight = 0, bgcolorR = 0, bgcolorG = 0, bgcolorB = 0;
	if( root.hasAttribute("themeWidth")) 
		themeWidth = root.attributeNode("themeWidth").nodeValue().toInt();
	if( root.hasAttribute("themeHeight")) 
		themeHeight = root.attributeNode("themeHeight").nodeValue().toInt();
	resize(themeWidth,themeHeight);
	
	if( root.hasAttribute("bgcolorR")) 
		bgcolorR = root.attributeNode("bgcolorR").nodeValue().toInt();
	if( root.hasAttribute("bgcolorG")) 
		bgcolorG = root.attributeNode("bgcolorG").nodeValue().toInt();
	if( root.hasAttribute("bgcolorB")) 
		bgcolorB = root.attributeNode("bgcolorB").nodeValue().toInt();
	setPaletteBackgroundColor(TQColor(bgcolorR, bgcolorG, bgcolorB));
	
	//get the screen resolution:
	int screenResX, screenResY;
	TQDesktopWidget desktop;
	screenResX = desktop.screenGeometry(this).width();
	screenResY = desktop.screenGeometry(this).height();
		
	//compute the offset to make the theme appear at the center off the screen;
	leftOffset = (int) (screenResX - themeWidth)/2;
	topOffset = (int) (screenResY - themeHeight)/2;
	
	contextMenu = new TQPopupMenu(this);
	reloadAct = new TQAction(tr("&Reload"), tr("F5"), this);
	reloadAct->setIconSet(SmallIconSet("reload"));
	reloadAct->setStatusTip(tr("Reload."));
	connect(reloadAct, SIGNAL(activated()), this, SLOT(updateNextSongs()));
	reloadAct->addTo(contextMenu);
	setupAct = new TQAction(tr("&Settings"), 0, this);
	setupAct->setIconSet(SmallIconSet("configure"));
	setupAct->setStatusTip(tr("Change settings."));
	connect(setupAct, SIGNAL(activated()), this, SLOT(launchConfig()));
	setupAct->addTo(contextMenu);

	contextMenu->insertSeparator();
	aboutAct = new TQAction(tr("&About"), 0, this);
	aboutAct->setIconSet(TQIconSet(TQPixmap("/usr/share/icons/amarokFS.png")));
	aboutAct->setStatusTip(tr("Show About Dialog."));
	connect(aboutAct, SIGNAL(activated()), this, SLOT(launchAbout()));
	aboutAct->addTo(contextMenu);
	closeAct = new TQAction(tr("&Quit"), tr("Esc"), this);
	closeAct->setIconSet(SmallIconSet("exit"));
	closeAct->setStatusTip(tr("Quit Amarok Full Screen."));
	connect(closeAct, SIGNAL(activated()), this, SLOT(close()));
	closeAct->addTo(contextMenu);
	
	if(!isSetDisThemeHint) {
	themeHint = new TQLabel(this, "themeHint", 0);
	themeHint->setText("To change settings do a rightclick.");
	themeHint->setGeometry(0, 0, 650, 25);
	themeHint->setPaletteForegroundColor(TQColor(255, 255, 255));
	themeHint->setFont(TQFont("System", 13, 50, FALSE));
	}
	
	TQDomNode node, subnode;
	TQDomElement akfswidget;
	node = root.firstChild();
	
	//parse the document
	while ( !node.isNull() ) {
		if ( node.isElement() ) {
			akfswidget = node.toElement();
		
			
			//parse a button and its settings
			if(akfswidget.nodeName() == "button") {
				int x = 0, y = 0, w = 0, h = 0;
				TQString clicked = "", path = "";
				TQCString command = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "path") path = themeImagesPrePath + elem.text();
						if(subnode.nodeName() == "clicked") clicked = themeImagesPrePath + elem.text();
						if(subnode.nodeName() == "command") command = elem.text();
					}
					subnode = subnode.nextSibling();
				}
				cb = new AKControlButton(this, "button", path, clicked, command);
				cb->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
				connect(cb, SIGNAL(clickedB(TQCString)), this, SLOT(controlButtonClicked(TQCString)));
				controlButtons.push_back(cb);
			}
			
			//parse a progress bar and its settings
			if(akfswidget.nodeName() == "progressbar") {
				int x = 0, y = 0, w = 0, h = 0;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				progressBar = new TQProgressBar(this, "progressbar");
				progressBar->setPercentageVisible(FALSE);
				progressBar->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
			}
			
			//parse a label that changes every track and its settings
			if(akfswidget.nodeName() == "labelTrack") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 12, fontWeight = 50, align = 0, valign = 0;
				TQString fontFace = "System";
				TQCString command = "", ignore = "", defaultText = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "fontR") r = elem.text().toInt();
						if(subnode.nodeName() == "fontG") g = elem.text().toInt();
						if(subnode.nodeName() == "fontB") b = elem.text().toInt();
						if(subnode.nodeName() == "command") command = elem.text();
						if(subnode.nodeName() == "ignore") ignore = elem.text();
						if(subnode.nodeName() == "defaultText") defaultText = elem.text();
						if(subnode.nodeName() == "fontSize") fontSize = elem.text().toInt();
						if(subnode.nodeName() == "fontWeight") fontWeight = elem.text().toInt();
						if(subnode.nodeName() == "fontFace") fontFace = elem.text();
						if(subnode.nodeName() == "valign") valign = elem.text().toInt();
						if(subnode.nodeName() == "align") align = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				lTrack = new AKLabelCommand(this, "label", command, defaultText, ignore);
				lTrack->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
				lTrack->setPaletteForegroundColor(TQColor(r, g, b));
				lTrack->setFont(TQFont(fontFace, fontSize, fontWeight, FALSE));
				lTrack->setText(defaultText);
				lTrack->setAlignment(align | valign | TQt::WordBreak);
				labelsTrack.push_back(lTrack);
			}
			
			//parse a label that changes instanteniously
			if(akfswidget.nodeName() == "labelInstant") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 12, fontWeight = 50, align = 0, valign = 0;
				TQString fontFace = "System";
				TQCString command = "", ignore = "", defaultText = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "fontR") r = elem.text().toInt();
						if(subnode.nodeName() == "fontG") g = elem.text().toInt();
						if(subnode.nodeName() == "fontB") b = elem.text().toInt();
						if(subnode.nodeName() == "command") command = elem.text();
						if(subnode.nodeName() == "ignore") ignore = elem.text();
						if(subnode.nodeName() == "defaultText") defaultText = elem.text();
						if(subnode.nodeName() == "fontSize") fontSize = elem.text().toInt();
						if(subnode.nodeName() == "fontWeight") fontWeight = elem.text().toInt();
						if(subnode.nodeName() == "fontFace") fontFace = elem.text();
						if(subnode.nodeName() == "valign") valign = elem.text().toInt();
						if(subnode.nodeName() == "align") align = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				lInstant = new AKLabelCommand(this, "label", command, defaultText, ignore);
				lInstant->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
				lInstant->setPaletteForegroundColor(TQColor(r, g, b));
				lInstant->setFont(TQFont(fontFace, fontSize, fontWeight, FALSE));
				lInstant->setText(defaultText);
				lInstant->setAlignment(align | valign | TQt::WordBreak);
				labelsInstant.push_back(lInstant);
			}
			
			//parse a static label 
			if(akfswidget.nodeName() == "labelStatic") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 12, fontWeight = 50, align = 0, valign = 0;
				TQString text = "", fontFace = "System", textString="";
				TQCString command = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "fontR") r = elem.text().toInt();
						if(subnode.nodeName() == "fontG") g = elem.text().toInt();
						if(subnode.nodeName() == "fontB") b = elem.text().toInt();
						if(subnode.nodeName() == "fontSize") fontSize = elem.text().toInt();
						if(subnode.nodeName() == "fontWeight") fontWeight = elem.text().toInt();
						if(subnode.nodeName() == "fontFace") fontFace = elem.text();
						if(subnode.nodeName() == "valign") valign = elem.text().toInt();
						if(subnode.nodeName() == "align") align = elem.text().toInt();
						if(subnode.nodeName() == "text") textString = elem.text();
					}
					subnode = subnode.nextSibling();
				}
				lStatic = new AKLabelCommand(this, "label", command, textString);
				lStatic->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
				lStatic->setPaletteForegroundColor(TQColor(r, g, b));
				lStatic->setFont(TQFont(fontFace, fontSize, fontWeight, FALSE));
				lStatic->setText(textString);
				lStatic->setAlignment(align | valign | TQt::WordBreak);
				labelsStatic.push_back(lStatic);
			}
			
			//parse a pixmap label 
			if(akfswidget.nodeName() == "labelPixmap") {
				int x = 0, y = 0, w = 0, h = 0;
				TQString path = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "path") path = elem.text();
					}
					subnode = subnode.nextSibling();
				}
				lPixmap = new AKLabelPixmap(this, "pixmap", path);
				lPixmap->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
				labelsPixmap.push_back(lPixmap);
			}
			
			//parse a cd-cover label
			if(akfswidget.nodeName() == "cdcover") {
				int x = 0, y = 0;//, w = 0, h = 0;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				cdcover = new TQLabel(this, "cd-cover");
				TQImage cdcoverImage;
				cdcoverImage.load( "images/logo.png");
				cdimagePixmap = cdcoverImage;
				cdcover->setPixmap(cdimagePixmap);
				cdcover->resize(COVERSIZE, COVERSIZE);
				cdcover->move(x + leftOffset, y + topOffset);
				cdcoverPosX = x + leftOffset;
				cdcoverPosY = y + topOffset;
			}
			
			//parse a cd-reflection
			if(akfswidget.nodeName() == "reflection") {
				int x = 0, y = 0;//, w = 0, h = 0;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				gradient = new TQLabel(this, "gradient");
				TQImage gradientImage;
				gradientImage.load( "images/gradient.png");
				gradientPixmap = gradientImage;
				//gradientPixmap.resize(cdimagePixmap.width(), 120);
				gradient->setPaletteBackgroundColor(TQColor(0,0,0));
				gradient->setPixmap(gradientPixmap);
				gradient->move(x + leftOffset, y + topOffset);
			}

			//parse a list of next playing songs and its settings
			if(akfswidget.nodeName() == "nextPlaying") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 10, fontWeight = 50, align = 0, valign = 0, trackCount = 1;
				TQString fontFace, introduction, format;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						TQDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "introduction") introduction = elem.text();
						if(subnode.nodeName() == "format") format = elem.text();
						if(subnode.nodeName() == "trackCount") trackCount = elem.text().toInt();
						if(subnode.nodeName() == "fontR") r = elem.text().toInt();
						if(subnode.nodeName() == "fontG") g = elem.text().toInt();
						if(subnode.nodeName() == "fontB") b = elem.text().toInt();
						if(subnode.nodeName() == "fontSize") fontSize = elem.text().toInt();
						if(subnode.nodeName() == "fontWeight") fontWeight = elem.text().toInt();
						if(subnode.nodeName() == "fontFace") fontFace = elem.text();
						if(subnode.nodeName() == "valign") valign = elem.text().toInt();
						if(subnode.nodeName() == "align") align = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				nextSongs = new TQLabel(this, "nextSong");
				nextSongCount = trackCount;
				nextSongIntroduction = introduction;
				nextSongFormat = format;
				nextSongs->setGeometry(TQRect(x + leftOffset, y + topOffset, w, h));
				nextSongs->setFont(TQFont(fontFace, fontSize, fontWeight, FALSE));
				nextSongs->setAlignment(align | valign | TQt::WordBreak);
				nextSongs->setPaletteForegroundColor(TQColor(r, g, b));
				nextSongs->setText(introduction);
			}
	
		}
		node = node.nextSibling();
	}
	
	dcopClient = new DCOPClient();
	dcopClient->attach();
	
	//disable screensaver (if the user wants it)
        if (isSetDisScrSvr) {
		TQByteArray data;
		TQDataStream arg(data, IO_WriteOnly);
		arg << false;
		if (!dcopClient->send("kdesktop", "KScreensaverIface", "enable(bool)", data))
			tqDebug("there was some error in disabling the KScreensaver.");
	}
	
	//disable Amarok OSD (if the user wants it)
	if (isSetDisAmaOSD) {
		TQByteArray data;
		TQDataStream arg(data, IO_WriteOnly);
		arg << false;
		if (!dcopClient->send("amarok", "player", "enableOSD(bool)", data))
			tqDebug("there was some error in disabling the Amarok OSD.");
	}
	
	timer = new TQTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(updateTime()) );
	timer->start( 1000, FALSE );
	
	hintTimer = new TQTimer( this );
	connect( hintTimer, SIGNAL(timeout()), this, SLOT(hideHint()) );
	hintTimer->start( 5000, TRUE );

	mouseTimer = new TQTimer( this );
	connect( mouseTimer, SIGNAL(timeout()), this, SLOT(hideMouse()) );
	mouseTimer->start( 5000, TRUE );
	
	setMouseTracking(TRUE);
	
	nowPlaying = "";
	updateTrackLabels();
}
    
    
AmfsXML::~AmfsXML() {
}

void AmfsXML::restoreScreenSaver() {
	TQByteArray data;
	TQDataStream arg(data, IO_WriteOnly);
	arg << true;
	if (!dcopClient->send("kdesktop", "KScreensaverIface", "enable(bool)", data)) 
		tqDebug("there was some error in enabling the KScreensaver.");
	cout << "Screen saver restored\n";
}

void AmfsXML::restoreAmarokOSD() {
	TQByteArray data;
	TQDataStream arg(data, IO_WriteOnly);
	arg << true;
	if (!dcopClient->send("amarok", "player", "enableOSD(bool)", data))
		tqDebug("there was some error in enabling the Amarok OSD.");
	cout << "Amarok OSD restored\n";
}

void AmfsXML::closeEvent( TQCloseEvent* ce )
{
	//restore the screen saver if it was disabled
	if(isSetDisScrSvr) restoreScreenSaver();
	//restore OSD if it was disabled
	if(isSetDisAmaOSD) restoreAmarokOSD();
        ce->accept();
        return;
}

void AmfsXML::wheelEvent ( TQWheelEvent * e ) {
	if (e->delta() > 0) sendPlayerCommand("volumeUp()");
	if (e->delta() < 0) sendPlayerCommand("volumeDown()");
}

void AmfsXML::mouseMoveEvent( TQMouseEvent *e ) {
	hideMouse(FALSE);
	if(isEnabled()) mouseTimer->start( 5000, TRUE );
}

void AmfsXML::hideMouse(bool hide) {
	if(hide) tqApp->setOverrideCursor( TQCursor( TQt::BlankCursor ) );
	else tqApp->setOverrideCursor( TQCursor( TQt::ArrowCursor ) );
}

void AmfsXML::contextMenuEvent(TQContextMenuEvent *event) {
	mouseTimer->stop();
	hideMouse(FALSE);
	contextMenu->exec(event->globalPos());
	connect( contextMenu, SIGNAL(aboutToHide()), this, SLOT(hideMouse()) );
}

void AmfsXML::launchAbout() {
	if(aboutWindow != 0)
		aboutWindow->show();	
}

void AmfsXML::launchConfig() {
	configWindow = new AmarokFS_config(this, "config");
	configWindow->show();
}

void AmfsXML::hideHint() {
	if(!isSetDisThemeHint)		
	themeHint->hide();
}

//reads the current trackt time and updates the progress bar
void AmfsXML::updateTime() {	
	//get the time and percentage
	int totalTime = getDCOPDataInt("trackTotalTime()");
	int currentTime = getDCOPDataInt("trackCurrentTime()");
	
	//if there is an initialized progress bar then we have to update it
	if(progressBar != (TQProgressBar *) 0) progressBar->setProgress( currentTime, totalTime);
	
	//update all labels that have to be updated instantaniously 
	updateInstantLabels();
	
	//if the track has changed title, artist, album and cover also have to be updated
	if(nowPlaying != getDCOPDataText("nowPlaying()")) {
		//updateTrackData();
		updateTrackLabels();
		nowPlaying = getDCOPDataText("nowPlaying()");
		cout << "New track: ";
		cout << getDCOPDataText("nowPlaying()") + "\n";
		
		//if there is a cdcover it has to be updated
		if(cdcover != (TQLabel *) 0 ) updateCover();
		if(gradient != (TQLabel *) 0 ) updateGradient();
		if(nextSongs != (TQLabel *) 0 ) updateNextSongs();
	}
	
}

void AmfsXML::updateTrackLabels() {
	for (unsigned int i=0; i<labelsTrack.size(); i++) {
		TQCString tempCommand = labelsTrack[i]->getCommand();
		if(!tempCommand.isEmpty()) {
			TQString dcopText = getDCOPDataText(tempCommand);
			TQRegExp ignore (labelsTrack[i]->getIgnore());
			if(ignore.exactMatch(dcopText))
				labelsTrack[i]->setText(labelsTrack[i]->getDefaultText());
			else
				labelsTrack[i]->setText(dcopText);
		}
	}
	
}

void AmfsXML::updateInstantLabels() {
	for (unsigned int i=0; i<labelsInstant.size(); i++) {
		TQCString tempCommand = labelsInstant[i]->getCommand();
		if(!tempCommand.isEmpty()) {
			TQString dcopText = getDCOPDataText(tempCommand);
			TQRegExp ignore (labelsInstant[i]->getIgnore());
			if(ignore.exactMatch(dcopText))
				labelsInstant[i]->setText(labelsInstant[i]->getDefaultText());
			else
				labelsInstant[i]->setText(dcopText);
		}
	}
	
}

void AmfsXML::updateCover() {
	TQString currentArtist = getDCOPDataText("artist()") ;
	TQString currentAlbum = getDCOPDataText("album()");
	TQImage cdcoverImage;
	
	cdcoverImage = getCoverPath(currentArtist, currentAlbum);
	cdcoverImage = cdcoverImage.smoothScale(COVERSIZE, COVERSIZE, TQImage::ScaleMin);
		
	cdimagePixmap = cdcoverImage;
	cdcover->setPixmap(cdimagePixmap);
	cdcover->move(cdcoverPosX,cdcoverPosY + COVERSIZE - cdimagePixmap.height());
	cdcover->resize(cdimagePixmap.width(), cdimagePixmap.height());
}

void AmfsXML::updateGradient() {
	TQString currentArtist = getDCOPDataText("artist()") ;
	TQString currentAlbum = getDCOPDataText("album()");
	TQImage cdReflected;
	
	cdReflected = getCoverPath(currentArtist, currentAlbum, TRUE);
	cdReflected = cdReflected.smoothScale(COVERSIZE, COVERSIZE, TQImage::ScaleMin);
	
	cdReflected = cdReflected.mirror(FALSE, TRUE);
	reflectionPixmap = cdReflected;
	
	TQImage gradientImage;
	gradientImage.load( "/usr/share/amarokFS/images/gradient.png");
	gradientImage = gradientImage.scale(reflectionPixmap.width(), 120);
	gradientPixmap = gradientImage;
	gradient->setPixmap(gradientPixmap);
	gradient->setPaletteBackgroundPixmap(reflectionPixmap);
	gradient->resize(reflectionPixmap.width(), 120);
}

void AmfsXML::updateNextSongs() {
	// Get Playlist file name and position
	TQByteArray dcopReplyData;
	
	TQString playlistFileName;
	dcopClient->call("amarok", "playlist", "saveCurrentPlaylist()",
		(TQByteArray) 0, *(new TQCString) , dcopReplyData);
	*(new TQDataStream (dcopReplyData, IO_ReadOnly)) >> playlistFileName;

	int playlistPosition;	
	dcopClient->call("amarok", "playlist", "getActiveIndex()",
		(TQByteArray) 0, *(new TQCString) , dcopReplyData);
	*(new TQDataStream (dcopReplyData, IO_ReadOnly)) >> playlistPosition;

	TQFile playlistFile( playlistFileName );
	if ( !playlistFile.open( IO_ReadOnly ) ) {
		cout << "Error\nCannot open playlist file \n";
		return;
	}
	if ( !domTree.setContent( &playlistFile ) ) {
		cout << "Error\nCannot parse playlist file \n";
		playlistFile.close();
		return;
	}
	playlistFile.close();
	
	// Parse playlist for next songs
	TQDomElement root = domTree.documentElement();
	TQDomNode node = root.firstChild();
	TQString nextSongList = nextSongIntroduction;
	for(int i = 0; i <= playlistPosition + nextSongCount && !node.isNull(); i++) {
		if(i > playlistPosition && node.isElement()) {
			// Bring information to format specified by theme
			TQString append = nextSongFormat;
			TQRegExp rx( "%(.+)%" );
			// Replace Placeholders			
			rx.setMinimal(TRUE); 
			while(rx.search(append) != -1)
				append.replace(rx.cap(0), node.toElement().elementsByTagName(rx.cap(1)).item(0).toElement().text());
			nextSongList += "\n" + append;
		}
		node = node.nextSibling();
	}
	nextSongs->setText(nextSongList);

}

TQImage AmfsXML::getCoverPath(TQString artist, TQString album, bool transDef) {
	TQString coverPathQuery = "select path from images where artist='" + artist + "' and album='" + album + "'";
	TQStringList coverPath = getDCOPDataSQL(coverPathQuery);
	
	TQString deviceMountPointQuery = "select lastmountpoint from devices where id=(select deviceid from images where artist='" + artist + "' and album='" + album + "')";
	TQStringList deviceMountPoint = getDCOPDataSQL(deviceMountPointQuery);
	
	//path - location relative within the media library
	TQString coverPathSQLMedia = coverPath.last();
	//complete absolute path
	TQString coverPathSQL = 	deviceMountPoint.first() + coverPathSQLMedia.right(coverPathSQLMedia.length()-1);
	//path from dcop (only 100x100px image)
	TQString coverPathDCOP =  getDCOPDataText("coverImage()");
	//$HOME/.trinity/share/apps/amarok/albumcovers/large/.... - some covers are stored here
	TQString coverPathDCOPlarge = coverPathDCOP.replace(TQRegExp("cache/\\d*@"), "large/");
	//$HOME/.trinity/share/apps/amarok/albumcovers/tagcover/.... - and some are stored here
	TQString coverPathDCOPtagcover = coverPathDCOP.replace(TQRegExp("cache/\\d*@"), "tagcover/");
	
	//debug-stuff
	/*cout << "dcop-large: " << coverPathDCOPlarge << "\n";
	cout << "dcop-tagcover: " << coverPathDCOPtagcover << "\n";
	cout << "sql: " << coverPathSQL << "\n";*/
	
	TQImage result = (TQImage) 0;
	result.load(coverPathDCOPlarge);
	if (result.isNull()) {
		result.load(coverPathSQL);
	}
	if (result.isNull()) {
		result.load(coverPathDCOPtagcover);
	}
	if (result.isNull()) {
		if(transDef == TRUE) result.load("/usr/share/amarokFS/images/logoB.png");
		else result.load("/usr/share/amarokFS/images/logo.png");
	}
	return result;
	
}

void AmfsXML::controlButtonClicked(TQCString command) {
	sendPlayerCommand(command);
}

void AmfsXML::sendPlayerCommand(TQCString command) {
	if (!dcopClient->send("amarok", "player", command, (TQByteArray) 0))
  		tqDebug("there was some error using DCOP.");
	//cout << "called: " << command << "\n";
}


TQString AmfsXML::getDCOPDataText(TQCString func) {
	TQByteArray dcopCallData, dcopReplyData;
	TQCString dcopReplyType;
	if (!dcopClient->call("amarok", "player", func, (TQByteArray) 0, dcopReplyType, dcopReplyData))
		tqDebug("there was some error calling DCOP function " + func);
	else {
		TQDataStream reply(dcopReplyData, IO_ReadOnly);
		if (dcopReplyType == "TQString") {
			TQString result;
			reply >> result;
			return result;
		} else {
			tqDebug("doIt returned an unexpected type of reply!");	
			tqDebug(dcopReplyType);
		}
	}
	return "";
}

int AmfsXML::getDCOPDataInt(TQCString func) {
	TQByteArray dcopCallData, dcopReplyData;
	TQCString dcopReplyType;
	if (!dcopClient->call("amarok", "player", func, (TQByteArray) 0, dcopReplyType, dcopReplyData))
		tqDebug("there was some error calling DCOP function " + func);
	else {
		TQDataStream reply(dcopReplyData, IO_ReadOnly);
		if (dcopReplyType == "int") {
			int result;
			reply >> result;
			return result;
		} else {
			tqDebug("doIt returned an unexpected type of reply!");	
		}
	}
	return 0;
}

TQStringList AmfsXML::getDCOPDataSQL(TQString query) {
	TQByteArray dcopCallData, dcopReplyData;
	TQCString dcopReplyType;
	TQDataStream arg(dcopCallData, IO_WriteOnly);
	arg << query;
	if (!dcopClient->call("amarok", "collection", "query(TQString)", dcopCallData, dcopReplyType, dcopReplyData))
		tqDebug("there was some error calling DCOP sql-query: " + query);
	else {
		TQDataStream reply(dcopReplyData, IO_ReadOnly);
		if (dcopReplyType == "TQStringList") {
			TQStringList result;
			reply >> result;
			return result;
		} else {
			tqDebug("doIt returned an unexpected type of reply!");	
		}
	}
	return (TQStringList) 0;
} 
