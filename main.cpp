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

#include <tqapplication.h>
#include <tdeapplication.h>
#include <tdeaboutdata.h>
#include <tdecmdlineargs.h>
#include <tdeaboutapplication.h>
#include <tqimage.h>
#include <vector>
#include "amfs-xml.h"
#include "amarokfs_config.h"

using namespace std;

int main( int argc, char ** argv )
{
	TDEAboutData* about = new TDEAboutData("amarokFS", "Amarok Full Screen", "0.4.2",
		"A full screen front end for Amarok", TDEAboutData::License_GPL_V2,
		"© 2007, Jan Zarnikov", 0,
		"http://www.kde-apps.org/content/show.php?content=52641");
	about->addAuthor("Jan Zarnikov", 0, "jan.zarnikov@gmail.com");
	about->addAuthor("Martin Koßler", 0, "martin.kossler@students.uni-marburg.de");
	TDECmdLineArgs::init(argc, argv, about);
	TDECmdLineOptions options[] =
	{
	    { "settings", "show config dialog", 0 },
	    { "kiosk", "disable Settings entry in contextmenu", 0 },
	    TDECmdLineLastOption
	};
	TDECmdLineArgs::addCmdLineOptions(options);
	TDECmdLineArgs* args = TDECmdLineArgs::parsedArgs();

	TDEApplication a;
	QWidget* x;

	if(args->isSet("settings")) {
		x = new AmarokFS_config();
		x->show();
	} else {
		x = new AmfsXML();
		if(args->isSet("kiosk"))
			((AmfsXML*)x)->setupAct->setEnabled(FALSE);
		((AmfsXML*)x)->aboutWindow = new TDEAboutApplication(about, x, "about", FALSE);
		x->showFullScreen();
	}

	x->setIcon(QImage("/usr/share/icons/amarokFS.png"));
	a.setMainWidget(x);
	return a.exec();
}
	
