/*
	This file is part of the Amarok Full Screen GUI
	Copyright (C) 2007 Jan Zarnikov (jan.zarnikov@gmail.com)

	This file is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public License
	along with this library; see the file COPYING.LIB.  If not, write to
	the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
	Boston, MA 02110-1301, USA.
*/

#include "amarokfs_config.h"

#include <tqvariant.h>
#include <tqlabel.h>
#include <tqdialog.h>
#include <tqpushbutton.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>
 
		
#include <kapp.h>
#include <tdeconfig.h>
#include <kurlrequester.h>

#include <iostream>
using namespace std;

/*
 *  Constructs a amarokFS_config as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
AmarokFS_config::AmarokFS_config( TQWidget* parent, const char* name, bool modal, WFlags fl )
    : TQDialog( parent, name, modal, fl )
{
	if ( !name )
		setName( "amarokFS_config" );
	
	setCaption("amarokFS Config" );

	TQVBoxLayout *configLayout = new TQVBoxLayout( this, 11, 6, "configLayout" );

	TQHBoxLayout *themeLayout = new TQHBoxLayout( 0, 0, 6, "themeLayout" );
	textLabel1 = new TQLabel( this, "textLabel1" );
	textLabel1->setGeometry( TQRect( 10, 10, 50, 20 ) );
	textLabel1->setText( "Theme:" );
	themeLayout->addWidget(textLabel1);
	kURLRequester1 = new KURLRequester( this, "kURLRequester1" );
	kURLRequester1->setGeometry( TQRect( 60, 0, 311, 41 ) );
	kURLRequester1->setFilter( "*.xml|xml Theme Files" );
	themeLayout->addWidget( kURLRequester1 );
	configLayout->addLayout(themeLayout);

	disScrSvr = new TQCheckBox(this, "disScrSvr");
	disScrSvr->setText("Disable screen saver");
	disScrSvr->setGeometry( TQRect(10, 40, 250, 20) );
	configLayout->addWidget( disScrSvr );
	
	disAmaOSD = new TQCheckBox(this, "disAmaOSD");
	disAmaOSD->setText("Disable Amarok OSD");
	disAmaOSD->setGeometry( TQRect(10, 60, 250, 20) );
	configLayout->addWidget( disAmaOSD );

	disThemeHint = new TQCheckBox(this, "disAmaOSD");
	disThemeHint->setText("Disable hint at startup");
	disThemeHint->setGeometry( TQRect(10, 80, 250, 20) );
	configLayout->addWidget( disThemeHint );
		
	textLabel2 = new TQLabel( this, "textLabel2" );
	textLabel2->setGeometry( TQRect( 10, 110, 371, 20 ) );
	textLabel2->setText("New settings will be applied after restarting the application." ) ;
	configLayout->addWidget( textLabel2 );

	TQHBoxLayout *buttonLayout = new TQHBoxLayout( 0, 0, 6, "buttonLayout" );
	TQSpacerItem *buttonSpacer = new TQSpacerItem( 20, 20, TQSizePolicy::Expanding, TQSizePolicy::Minimum );
	buttonLayout->addItem( buttonSpacer );

	okButton = new TQPushButton( this, "ok" );
	okButton->setGeometry( TQRect( 211, 130, 80, 30 ) );
	okButton->setText( "OK" );
	buttonLayout->addWidget( okButton );

	cancelButton = new TQPushButton( this, "cancel" );
	cancelButton->setGeometry( TQRect( 300, 130, 80, 30 ) );
	cancelButton->setText( "Cancel" );
	buttonLayout->addWidget( cancelButton );
	
	configLayout->addLayout( buttonLayout );
	
	
	kapp->config()->setGroup ("Theme");
	themeKUrl =  kapp->config()->readEntry ("KUrl", "");
	//cout << themeKUrl << "\n";
	kURLRequester1->setURL(themeKUrl);
	
	kapp->config()->setGroup ("Settings");
	isSetDisScrSvr =  (bool) ((kapp->config()->readEntry ("DisableScreenSaver", "0")).toInt());
	disScrSvr->setChecked(isSetDisScrSvr);
	isSetDisAmaOSD =  (bool) ((kapp->config()->readEntry ("DisableAmarokOSD", "0")).toInt());
	disAmaOSD->setChecked(isSetDisAmaOSD);
	isSetDisThemeHint =  (bool) ((kapp->config()->readEntry ("DisableThemeHint", "0")).toInt());
	disThemeHint->setChecked(isSetDisThemeHint);
	
	clearWState( WState_Polished );
	connect( okButton, SIGNAL(clicked()), this, SLOT(saveData()) );
	connect( cancelButton, SIGNAL(clicked()), this, SLOT(close()) );
	connect( kURLRequester1, SIGNAL(textChanged(const TQString &)), this, SLOT(dataChange(const TQString &)) );
	connect( kURLRequester1, SIGNAL(urlSelected(const TQString &)), this, SLOT(dataChange(const TQString &)) );
	connect( disScrSvr, SIGNAL(stateChanged(int)), this, SLOT(scrSvrChanged(int)) );
	connect( disAmaOSD, SIGNAL(stateChanged(int)), this, SLOT(amaOSDChanged(int)) );
	connect( disThemeHint, SIGNAL(stateChanged(int)), this, SLOT(themeHintChanged(int)) );
}

void AmarokFS_config::saveData() {
	kapp->config()->setGroup ("Theme");
	kapp->config()->writeEntry ("KUrl", themeKUrl);
	kapp->config()->setGroup ("Settings");
	kapp->config()->writeEntry ("DisableScreenSaver", (int) isSetDisScrSvr);
	kapp->config()->writeEntry ("DisableAmarokOSD", (int) isSetDisAmaOSD);
	kapp->config()->writeEntry ("DisableThemeHint", (int) isSetDisThemeHint);
	kapp->config()->sync();
	close();
}

void AmarokFS_config::dataChange(const TQString & newKUrl) {
	themeKUrl = newKUrl;
}

void AmarokFS_config::scrSvrChanged(int state) {
	//cout << state << endl;
	isSetDisScrSvr =  (bool) state;
}

void AmarokFS_config::amaOSDChanged(int state) {
	isSetDisAmaOSD =  (bool) state;
}

void AmarokFS_config::themeHintChanged(int state) {
	isSetDisThemeHint =  (bool) state;
}

/*
 *  Destroys the object and frees any allocated resources
 */
AmarokFS_config::~AmarokFS_config()
{
	
}



