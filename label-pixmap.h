#ifndef LABEL_PIXMAP_H
#define LABEL_PIXMAP_H

#include <tqwidget.h>
#include <tqlabel.h>
#include <tqstring.h>
#include <tqpixmap.h>

class AKLabelPixmap : public TQLabel
{
	TQ_OBJECT

public:
	AKLabelPixmap(TQWidget *parent, const char *name = 0, TQString path = "");
	~AKLabelPixmap();
	

private:
	TQString picturePath;
	TQPixmap picturePixmap;

};

#endif
 
