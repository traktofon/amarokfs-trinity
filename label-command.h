#ifndef LABEL_COMMAND_H
#define LABEL_COMMAND_H

#include <qwidget.h>
#include <qlabel.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qregexp.h>

class AKLabelCommand : public QLabel
{
	Q_OBJECT

public:
	AKLabelCommand(QWidget *parent, const char *name = 0, QCString commandS = "", QString text = "", QString ignore = "");
	~AKLabelCommand();
	QCString getCommand();
	QString getIgnore();
	QString getDefaultText();

private:
	QCString command;
	QString ignore;
	QString defaultText;

};

#endif
 
