#ifndef CB_H
#define CB_H

#include <tqwidget.h>
#include <tqlabel.h>
#include <tqstring.h>
#include <tqpixmap.h>

class AKControlButton : public TQLabel
{
	TQ_OBJECT

public:
	AKControlButton(TQWidget *parent, const char *name = 0, const TQString &fileNameN = "", const TQString &fileNameC = "", TQCString commandS = "");
	~AKControlButton();
	void setNormal(const TQString & fileName);
	void setClicked(const TQString & fileName);

private:
	void mouseReleaseEvent( TQMouseEvent *e );
	void mousePressEvent( TQMouseEvent *e );
	void mouseMoveEvent( TQMouseEvent *e );

	void wasClicked();
	void wasReleased();

	TQCString command;
	TQPixmap normal, clicked;

signals:
	void clickedB(TQCString);

};

#endif
