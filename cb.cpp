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

#include "cb.h"
#include <qstring.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qimage.h>
#include <iostream>
using namespace std;

AKControlButton::AKControlButton(QWidget *parent, const char *name, const QString &fileNameN, const QString &fileNameC, QCString commandS)
    : QLabel( parent, name ) {
	setNormal(fileNameN);
	if(!fileNameC.isEmpty()) setClicked(fileNameC);
	else setClicked(fileNameN);
	command = commandS;
	//path = new QString("aaa");
}

/*
 *  Destroys the object and frees any allocated resources
 */
AKControlButton::~AKControlButton()
{
    // no need to delete child widgets, Qt does it all for us
}

void AKControlButton::setNormal(const QString & fileName) {
	QImage tempImg;
	tempImg.load( fileName);
	normal = tempImg;
	wasReleased();
}

void AKControlButton::setClicked(const QString & fileName) {
	QImage tempImg;
	tempImg.load( fileName);
	clicked = tempImg;
	//setPixmap(normal);
}

void AKControlButton::wasClicked() {
	setPixmap(clicked);
}

void AKControlButton::wasReleased() {
	setPixmap(normal);
}
void AKControlButton::mouseReleaseEvent( QMouseEvent *e )
{
	if((rect()).contains(e->pos())) {
		wasReleased();
		emit clickedB(command);
	}
}

void AKControlButton::mouseMoveEvent( QMouseEvent *e )
{
	if(!( (rect()).contains(e->pos()) )) {
		wasReleased();
	}
}

void AKControlButton::mousePressEvent( QMouseEvent *e )
{
	if((rect()).contains(e->pos())) {
		setPixmap(clicked);
	}
}

