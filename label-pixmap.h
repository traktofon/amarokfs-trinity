#ifndef LABEL_PIXMAP_H
#define LABEL_PIXMAP_H

#include <qwidget.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpixmap.h>

class AKLabelPixmap : public QLabel
{
	Q_OBJECT

public:
	AKLabelPixmap(QWidget *parent, const char *name = 0, QString path = "");
	~AKLabelPixmap();
	

private:
	QString picturePath;
	QPixmap picturePixmap;

};

#endif
 
