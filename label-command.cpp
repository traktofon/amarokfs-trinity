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

#include "label-command.h"
#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qregexp.h>
#include <iostream>
using namespace std;

AKLabelCommand::AKLabelCommand(QWidget *parent, const char *name, QCString commandS, QString text, QString ignoreS)
    : QLabel( parent, name ) {
	command = commandS;
	ignore = ignoreS;
	defaultText = text;	
	if(ignore.isEmpty())
		setText(text);
	
}		

/*
 *  Destroys the object and frees any allocated resources
 */
AKLabelCommand::~AKLabelCommand()
{
    // no need to delete child widgets, Qt does it all for us
} 

QCString AKLabelCommand::getCommand() {
	return command;
}

QString AKLabelCommand::getIgnore() {
	return ignore;
}

QString AKLabelCommand::getDefaultText() {
	return defaultText;
}
