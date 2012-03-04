#ifndef CB_H
#define CB_H

#include <qwidget.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpixmap.h>

class AKControlButton : public QLabel
{
	Q_OBJECT

public:
	AKControlButton(QWidget *parent, const char *name = 0, const QString &fileNameN = "", const QString &fileNameC = "", QCString commandS = "");
	~AKControlButton();
	void setNormal(const QString & fileName);
	void setClicked(const QString & fileName);

private:
	void mouseReleaseEvent( QMouseEvent *e );
	void mousePressEvent( QMouseEvent *e );
	void mouseMoveEvent( QMouseEvent *e );

	void wasClicked();
	void wasReleased();

	QCString command;
	QPixmap normal, clicked;

signals:
	void clickedB(QCString);

};

#endif
