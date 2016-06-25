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
#include <tqstring.h>
#include <tqwidget.h>
#include <tqlabel.h>
#include <tqpixmap.h>
#include <tqimage.h>
#include <tqregexp.h>
#include <iostream>
using namespace std;

AKLabelCommand::AKLabelCommand(TQWidget *parent, const char *name, TQCString commandS, TQString text, TQString ignoreS)
    : TQLabel( parent, name ) {
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
    // no need to delete child widgets, TQt does it all for us
} 

TQCString AKLabelCommand::getCommand() {
	return command;
}

TQString AKLabelCommand::getIgnore() {
	return ignore;
}

TQString AKLabelCommand::getDefaultText() {
	return defaultText;
}
