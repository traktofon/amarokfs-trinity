#ifndef LABEL_COMMAND_H
#define LABEL_COMMAND_H

#include <tqwidget.h>
#include <tqlabel.h>
#include <tqstring.h>
#include <tqpixmap.h>
#include <tqregexp.h>

class AKLabelCommand : public TQLabel
{
	TQ_OBJECT

public:
	AKLabelCommand(TQWidget *parent, const char *name = 0, TQCString commandS = "", TQString text = "", TQString ignore = "");
	~AKLabelCommand();
	TQCString getCommand();
	TQString getIgnore();
	TQString getDefaultText();

private:
	TQCString command;
	TQString ignore;
	TQString defaultText;

};

#endif
 
