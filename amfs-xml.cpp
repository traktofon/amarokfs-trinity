#include "amfs-xml.h"
#include "amarokfs_config.h"

#define COVERSIZE (500)

#include <kapp.h>
#include <kconfig.h>
#include <kiconloader.h>
#include <qiconset.h>

#include <qfile.h>
#include <qdom.h>
#include <qwidget.h>
#include <qdialog.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <qmessagebox.h>
#include <qerrormessage.h>
#include <qregexp.h>

using namespace std;

		
AmfsXML::AmfsXML( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl ) {
	
	
	progressBar = (QProgressBar *) 0;
	cdcover = (QLabel *) 0;
	gradient = (QLabel *) 0;
	
	//read the setting whether the user wants to disable the screensaver
	kapp->config()->setGroup ("Settings");
	isSetDisScrSvr =  (bool) ((kapp->config()->readEntry ("DisableScreenSaver", "0")).toInt());
	isSetDisAmaOSD =  (bool) ((kapp->config()->readEntry ("DisableAmarokOSD", "0")).toInt());
	isSetDisThemeHint =  (bool) ((kapp->config()->readEntry ("DisableThemeHint", "0")).toInt());
	
	//load the path from user theme
	kapp->config()->setGroup ("Theme");
	QString themeCustomPath = kapp->config()->readEntry ("KUrl", "");
	//strip the filename to get the directory containing the file
	QString themeCustomPrepath = themeCustomPath;
	themeCustomPrepath.replace(QFileInfo(themeCustomPath).fileName(), "");
	
	//cout << themeCustomPath << "\n";
	//cout << themeCustomPrepath << "\n";
	
	//set the default path
	QString themePath = "/usr/share/amarokFS/theme.xml";
	//default prepath for the images directory	
	QString themeImagesPrePath = "/usr/share/amarokFS/";
		
	//if the custom theme file exists, use it, otherwise use the default theme
	if(QFile(themeCustomPath).exists()) {
		themePath = themeCustomPath;
		themeImagesPrePath = themeCustomPrepath;
		
	}
	cout << "Using skin: " << themePath << "\n";
	
	QFile opmlFile( themePath );
	if ( !opmlFile.open( IO_ReadOnly ) ) {
		cout << "Critical Error\n";
		cout << "Cannot open file \n";
		QMessageBox::critical( 0, "amarokFS", "The theme file is not readable.\nPlease select a new theme \nand start the application again.");
		launchConfig();
		close();
	
	}

	if ( !domTree.setContent( &opmlFile ) ) {
		cout << "Critical Error\n";
		cout << "Cannot parse file \n";
		opmlFile.close();
		QMessageBox::critical( 0, "amarokFS", "The theme file is broken.\nPlease select a new theme \nand start the application again.");
		launchConfig();
		close();
	}
	opmlFile.close();
	
	//parse the document
	QDomElement root = domTree.documentElement();
	
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
	setPaletteBackgroundColor(QColor(bgcolorR, bgcolorG, bgcolorB));
	
	//get the screen resolution:
	int screenResX, screenResY;
	QDesktopWidget desktop;
	screenResX = desktop.screenGeometry(this).width();
	screenResY = desktop.screenGeometry(this).height();
		
	//compute the offset to make the theme appear at the center off the screen;
	leftOffset = (int) (screenResX - themeWidth)/2;
	topOffset = (int) (screenResY - themeHeight)/2;
	
	contextMenu = new QPopupMenu(this);
	reloadAct = new QAction(tr("&Reload"), tr("F5"), this);
	reloadAct->setIconSet(SmallIconSet("reload"));
	reloadAct->setStatusTip(tr("Reload."));
	connect(reloadAct, SIGNAL(activated()), this, SLOT(updateNextSongs()));
	reloadAct->addTo(contextMenu);
	setupAct = new QAction(tr("&Settings"), 0, this);
	setupAct->setIconSet(SmallIconSet("configure"));
	setupAct->setStatusTip(tr("Change settings."));
	connect(setupAct, SIGNAL(activated()), this, SLOT(launchConfig()));
	setupAct->addTo(contextMenu);

	contextMenu->insertSeparator();
	aboutAct = new QAction(tr("&About"), 0, this);
	aboutAct->setIconSet(QIconSet(QPixmap("/usr/share/icons/amarokFS.png")));
	aboutAct->setStatusTip(tr("Show About Dialog."));
	connect(aboutAct, SIGNAL(activated()), this, SLOT(launchAbout()));
	aboutAct->addTo(contextMenu);
	closeAct = new QAction(tr("&Quit"), tr("Esc"), this);
	closeAct->setIconSet(SmallIconSet("exit"));
	closeAct->setStatusTip(tr("Quit Amarok Full Screen."));
	connect(closeAct, SIGNAL(activated()), this, SLOT(close()));
	closeAct->addTo(contextMenu);
	
	if(!isSetDisThemeHint) {
	themeHint = new QLabel(this, "themeHint", 0);
	themeHint->setText("To change settings do a rightclick.");
	themeHint->setGeometry(0, 0, 650, 25);
	themeHint->setPaletteForegroundColor(QColor::QColor(255, 255, 255));
	themeHint->setFont(QFont::QFont("System", 13, 50, FALSE));
	}
	
	QDomNode node, subnode;
	QDomElement akfswidget;
	node = root.firstChild();
	
	//parse the document
	while ( !node.isNull() ) {
		if ( node.isElement() ) {
			akfswidget = node.toElement();
		
			
			//parse a button and its settings
			if(akfswidget.nodeName() == "button") {
				int x = 0, y = 0, w = 0, h = 0;
				QString clicked = "", path = "";
				QCString command = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
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
				cb->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
				connect(cb, SIGNAL(clickedB(QCString)), this, SLOT(controlButtonClicked(QCString)));
				controlButtons.push_back(cb);
			}
			
			//parse a progress bar and its settings
			if(akfswidget.nodeName() == "progressbar") {
				int x = 0, y = 0, w = 0, h = 0;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				progressBar = new QProgressBar(this, "progressbar");
				progressBar->setPercentageVisible(FALSE);
				progressBar->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
			}
			
			//parse a label that changes every track and its settings
			if(akfswidget.nodeName() == "labelTrack") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 12, fontWeight = 50, align = 0, valign = 0;
				QString fontFace = "System";
				QCString command = "", ignore = "", defaultText = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
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
				lTrack->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
				lTrack->setPaletteForegroundColor(QColor::QColor(r, g, b));
				lTrack->setFont(QFont::QFont(fontFace, fontSize, fontWeight, FALSE));
				lTrack->setText(defaultText);
				lTrack->setAlignment(align | valign | Qt::WordBreak);
				labelsTrack.push_back(lTrack);
			}
			
			//parse a label that changes instanteniously
			if(akfswidget.nodeName() == "labelInstant") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 12, fontWeight = 50, align = 0, valign = 0;
				QString fontFace = "System";
				QCString command = "", ignore = "", defaultText = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
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
				lInstant->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
				lInstant->setPaletteForegroundColor(QColor::QColor(r, g, b));
				lInstant->setFont(QFont::QFont(fontFace, fontSize, fontWeight, FALSE));
				lInstant->setText(defaultText);
				lInstant->setAlignment(align | valign | Qt::WordBreak);
				labelsInstant.push_back(lInstant);
			}
			
			//parse a static label 
			if(akfswidget.nodeName() == "labelStatic") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 12, fontWeight = 50, align = 0, valign = 0;
				QString text = "", fontFace = "System", textString="";
				QCString command = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
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
				lStatic->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
				lStatic->setPaletteForegroundColor(QColor::QColor(r, g, b));
				lStatic->setFont(QFont::QFont(fontFace, fontSize, fontWeight, FALSE));
				lStatic->setText(textString);
				lStatic->setAlignment(align | valign | Qt::WordBreak);
				labelsStatic.push_back(lStatic);
			}
			
			//parse a pixmap label 
			if(akfswidget.nodeName() == "labelPixmap") {
				int x = 0, y = 0, w = 0, h = 0;
				QString path = "";
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
						if(subnode.nodeName() == "w") w = elem.text().toInt();
						if(subnode.nodeName() == "h") h = elem.text().toInt();
						if(subnode.nodeName() == "path") path = elem.text();
					}
					subnode = subnode.nextSibling();
				}
				lPixmap = new AKLabelPixmap(this, "pixmap", path);
				lPixmap->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
				labelsPixmap.push_back(lPixmap);
			}
			
			//parse a cd-cover label
			if(akfswidget.nodeName() == "cdcover") {
				int x = 0, y = 0;//, w = 0, h = 0;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				cdcover = new QLabel(this, "cd-cover");
				QImage cdcoverImage;
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
						QDomElement elem = subnode.toElement();
						if(subnode.nodeName() == "x") x = elem.text().toInt();
						if(subnode.nodeName() == "y") y = elem.text().toInt();
					}
					subnode = subnode.nextSibling();
				}
				gradient = new QLabel(this, "gradient");
				QImage gradientImage;
				gradientImage.load( "images/gradient.png");
				gradientPixmap = gradientImage;
				//gradientPixmap.resize(cdimagePixmap.width(), 120);
				gradient->setPaletteBackgroundColor(QColor(0,0,0));
				gradient->setPixmap(gradientPixmap);
				gradient->move(x + leftOffset, y + topOffset);
			}

			//parse a list of next playing songs and its settings
			if(akfswidget.nodeName() == "nextPlaying") {
				int x = 0, y = 0, w = 0, h = 0, r = 0, g = 0, b = 0, fontSize = 10, fontWeight = 50, align = 0, valign = 0, trackCount = 1;
				QString fontFace, introduction, format;
				subnode = node.firstChild();
				while ( !subnode.isNull() ) {
					if ( subnode.isElement() ) {
						QDomElement elem = subnode.toElement();
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
				nextSongs = new QLabel(this, "nextSong");
				nextSongCount = trackCount;
				nextSongIntroduction = introduction;
				nextSongFormat = format;
				nextSongs->setGeometry(QRect(x + leftOffset, y + topOffset, w, h));
				nextSongs->setFont(QFont::QFont(fontFace, fontSize, fontWeight, FALSE));
				nextSongs->setAlignment(align | valign | Qt::WordBreak);
				nextSongs->setPaletteForegroundColor(QColor::QColor(r, g, b));
				nextSongs->setText(introduction);
			}
	
		}
		node = node.nextSibling();
	}
	
	dcopClient = new DCOPClient();
	dcopClient->attach();
	
	//disable screensaver (if the user wants it)
        if (isSetDisScrSvr) {
		QByteArray data;
		QDataStream arg(data, IO_WriteOnly);
		arg << false;
		if (!dcopClient->send("kdesktop", "KScreensaverIface", "enable(bool)", data))
			qDebug("there was some error in disabling the KScreensaver.");
	}
	
	//disable Amarok OSD (if the user wants it)
	if (isSetDisAmaOSD) {
		QByteArray data;
		QDataStream arg(data, IO_WriteOnly);
		arg << false;
		if (!dcopClient->send("amarok", "player", "enableOSD(bool)", data))
			qDebug("there was some error in disabling the Amarok OSD.");
	}
	
	timer = new QTimer( this );
	connect( timer, SIGNAL(timeout()), this, SLOT(updateTime()) );
	timer->start( 1000, FALSE );
	
	hintTimer = new QTimer( this );
	connect( hintTimer, SIGNAL(timeout()), this, SLOT(hideHint()) );
	hintTimer->start( 5000, TRUE );

	mouseTimer = new QTimer( this );
	connect( mouseTimer, SIGNAL(timeout()), this, SLOT(hideMouse()) );
	mouseTimer->start( 5000, TRUE );
	
	setMouseTracking(TRUE);
	
	nowPlaying = "";
	updateTrackLabels();
}
    
    
AmfsXML::~AmfsXML() {
}

void AmfsXML::restoreScreenSaver() {
	QByteArray data;
	QDataStream arg(data, IO_WriteOnly);
	arg << true;
	if (!dcopClient->send("kdesktop", "KScreensaverIface", "enable(bool)", data)) 
		qDebug("there was some error in enabling the KScreensaver.");
	cout << "Screen saver restored\n";
}

void AmfsXML::restoreAmarokOSD() {
	QByteArray data;
	QDataStream arg(data, IO_WriteOnly);
	arg << true;
	if (!dcopClient->send("amarok", "player", "enableOSD(bool)", data))
		qDebug("there was some error in enabling the Amarok OSD.");
	cout << "Amarok OSD restored\n";
}

void AmfsXML::closeEvent( QCloseEvent* ce )
{
	//restore the screen saver if it was disabled
	if(isSetDisScrSvr) restoreScreenSaver();
	//restore OSD if it was disabled
	if(isSetDisAmaOSD) restoreAmarokOSD();
        ce->accept();
        return;
}

void AmfsXML::wheelEvent ( QWheelEvent * e ) {
	if (e->delta() > 0) sendPlayerCommand("volumeUp()");
	if (e->delta() < 0) sendPlayerCommand("volumeDown()");
}

void AmfsXML::mouseMoveEvent( QMouseEvent *e ) {
	hideMouse(FALSE);
	if(isEnabled()) mouseTimer->start( 5000, TRUE );
}

void AmfsXML::hideMouse(bool hide) {
	if(hide) qApp->setOverrideCursor( QCursor( Qt::BlankCursor ) );
	else qApp->setOverrideCursor( QCursor( Qt::ArrowCursor ) );
}

void AmfsXML::contextMenuEvent(QContextMenuEvent *event) {
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
	if(progressBar != (QProgressBar *) 0) progressBar->setProgress( currentTime, totalTime);
	
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
		if(cdcover != (QLabel *) 0 ) updateCover();
		if(gradient != (QLabel *) 0 ) updateGradient();
		if(nextSongs != (QLabel *) 0 ) updateNextSongs();
	}
	
}

void AmfsXML::updateTrackLabels() {
	for (unsigned int i=0; i<labelsTrack.size(); i++) {
		QCString tempCommand = labelsTrack[i]->getCommand();
		if(!tempCommand.isEmpty()) {
			QString dcopText = getDCOPDataText(tempCommand);
			QRegExp ignore (labelsTrack[i]->getIgnore());
			if(ignore.exactMatch(dcopText))
				labelsTrack[i]->setText(labelsTrack[i]->getDefaultText());
			else
				labelsTrack[i]->setText(dcopText);
		}
	}
	
}

void AmfsXML::updateInstantLabels() {
	for (unsigned int i=0; i<labelsInstant.size(); i++) {
		QCString tempCommand = labelsInstant[i]->getCommand();
		if(!tempCommand.isEmpty()) {
			QString dcopText = getDCOPDataText(tempCommand);
			QRegExp ignore (labelsInstant[i]->getIgnore());
			if(ignore.exactMatch(dcopText))
				labelsInstant[i]->setText(labelsInstant[i]->getDefaultText());
			else
				labelsInstant[i]->setText(dcopText);
		}
	}
	
}

void AmfsXML::updateCover() {
	QString currentArtist = getDCOPDataText("artist()") ;
	QString currentAlbum = getDCOPDataText("album()");
	QImage cdcoverImage;
	
	cdcoverImage = getCoverPath(currentArtist, currentAlbum);
	cdcoverImage = cdcoverImage.smoothScale(COVERSIZE, COVERSIZE, QImage::ScaleMin);
		
	cdimagePixmap = cdcoverImage;
	cdcover->setPixmap(cdimagePixmap);
	cdcover->move(cdcoverPosX,cdcoverPosY + COVERSIZE - cdimagePixmap.height());
	cdcover->resize(cdimagePixmap.width(), cdimagePixmap.height());
}

void AmfsXML::updateGradient() {
	QString currentArtist = getDCOPDataText("artist()") ;
	QString currentAlbum = getDCOPDataText("album()");
	QImage cdReflected;
	
	cdReflected = getCoverPath(currentArtist, currentAlbum, TRUE);
	cdReflected = cdReflected.smoothScale(COVERSIZE, COVERSIZE, QImage::ScaleMin);
	
	cdReflected = cdReflected.mirror(FALSE, TRUE);
	reflectionPixmap = cdReflected;
	
	QImage gradientImage;
	gradientImage.load( "/usr/share/amarokFS/images/gradient.png");
	gradientImage = gradientImage.scale(reflectionPixmap.width(), 120);
	gradientPixmap = gradientImage;
	gradient->setPixmap(gradientPixmap);
	gradient->setPaletteBackgroundPixmap(reflectionPixmap);
	gradient->resize(reflectionPixmap.width(), 120);
}

void AmfsXML::updateNextSongs() {
	// Get Playlist file name and position
	QByteArray dcopReplyData;
	
	QString playlistFileName;
	dcopClient->call("amarok", "playlist", "saveCurrentPlaylist()",
		(QByteArray) 0, *(new QCString) , dcopReplyData);
	*(new QDataStream (dcopReplyData, IO_ReadOnly)) >> playlistFileName;

	int playlistPosition;	
	dcopClient->call("amarok", "playlist", "getActiveIndex()",
		(QByteArray) 0, *(new QCString) , dcopReplyData);
	*(new QDataStream (dcopReplyData, IO_ReadOnly)) >> playlistPosition;

	QFile playlistFile( playlistFileName );
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
	QDomElement root = domTree.documentElement();
	QDomNode node = root.firstChild();
	QString nextSongList = nextSongIntroduction;
	for(int i = 0; i <= playlistPosition + nextSongCount && !node.isNull(); i++) {
		if(i > playlistPosition && node.isElement()) {
			// Bring information to format specified by theme
			QString append = nextSongFormat;
			QRegExp rx( "%(.+)%" );
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

QImage AmfsXML::getCoverPath(QString artist, QString album, bool transDef) {
	QString coverPathQuery = "select path from images where artist='" + artist + "' and album='" + album + "'";
	QStringList coverPath = getDCOPDataSQL(coverPathQuery);
	
	QString deviceMountPointQuery = "select lastmountpoint from devices where id=(select deviceid from images where artist='" + artist + "' and album='" + album + "')";
	QStringList deviceMountPoint = getDCOPDataSQL(deviceMountPointQuery);
	
	//path - location relative within the media library
	QString coverPathSQLMedia = coverPath.last();
	//complete absolute path
	QString coverPathSQL = 	deviceMountPoint.first() + coverPathSQLMedia.right(coverPathSQLMedia.length()-1);
	//path from dcop (only 100x100px image)
	QString coverPathDCOP =  getDCOPDataText("coverImage()");
	//$HOME/.trinity/share/apps/amarok/albumcovers/large/.... - some covers are stored here
	QString coverPathDCOPlarge = coverPathDCOP.replace(QRegExp("cache/\\d*@"), "large/");
	//$HOME/.trinity/share/apps/amarok/albumcovers/tagcover/.... - and some are stored here
	QString coverPathDCOPtagcover = coverPathDCOP.replace(QRegExp("cache/\\d*@"), "tagcover/");
	
	//debug-stuff
	/*cout << "dcop-large: " << coverPathDCOPlarge << "\n";
	cout << "dcop-tagcover: " << coverPathDCOPtagcover << "\n";
	cout << "sql: " << coverPathSQL << "\n";*/
	
	QImage result = (QImage) 0;
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

void AmfsXML::controlButtonClicked(QCString command) {
	sendPlayerCommand(command);
}

void AmfsXML::sendPlayerCommand(QCString command) {
	if (!dcopClient->send("amarok", "player", command, (QByteArray) 0))
  		qDebug("there was some error using DCOP.");
	//cout << "called: " << command << "\n";
}


QString AmfsXML::getDCOPDataText(QCString func) {
	QByteArray dcopCallData, dcopReplyData;
	QCString dcopReplyType;
	if (!dcopClient->call("amarok", "player", func, (QByteArray) 0, dcopReplyType, dcopReplyData))
		qDebug("there was some error calling DCOP function " + func);
	else {
		QDataStream reply(dcopReplyData, IO_ReadOnly);
		if (dcopReplyType == "TQString") {
			QString result;
			reply >> result;
			return result;
		} else {
			qDebug("doIt returned an unexpected type of reply!");	
			qDebug(dcopReplyType);
		}
	}
	return "";
}

int AmfsXML::getDCOPDataInt(QCString func) {
	QByteArray dcopCallData, dcopReplyData;
	QCString dcopReplyType;
	if (!dcopClient->call("amarok", "player", func, (QByteArray) 0, dcopReplyType, dcopReplyData))
		qDebug("there was some error calling DCOP function " + func);
	else {
		QDataStream reply(dcopReplyData, IO_ReadOnly);
		if (dcopReplyType == "int") {
			int result;
			reply >> result;
			return result;
		} else {
			qDebug("doIt returned an unexpected type of reply!");	
		}
	}
	return 0;
}

QStringList AmfsXML::getDCOPDataSQL(QString query) {
	QByteArray dcopCallData, dcopReplyData;
	QCString dcopReplyType;
	QDataStream arg(dcopCallData, IO_WriteOnly);
	arg << query;
	if (!dcopClient->call("amarok", "collection", "query(TQString)", dcopCallData, dcopReplyType, dcopReplyData))
		qDebug("there was some error calling DCOP sql-query: " + query);
	else {
		QDataStream reply(dcopReplyData, IO_ReadOnly);
		if (dcopReplyType == "TQStringList") {
			QStringList result;
			reply >> result;
			return result;
		} else {
			qDebug("doIt returned an unexpected type of reply!");	
		}
	}
	return (QStringList) 0;
} 
