/****************************************************************************
** Form interface generated from reading ui file 'amarokfs_config.ui'
**
** Created: Wed Feb 7 13:43:13 2007
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef AMAROKFS_CONFIG_H
#define AMAROKFS_CONFIG_H

#include <tqvariant.h>
#include <tqlabel.h>
#include <kurlrequester.h>
#include <tqpushbutton.h>
#include <tqlayout.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>
#include <tqdialog.h>
#include <tqcheckbox.h>

class TQVBoxLayout;
class TQHBoxLayout;
class TQGridLayout;
class TQSpacerItem;
class TQLabel;
class KURLRequester;
class TQPushButton;

class AmarokFS_config : public TQDialog
{
    TQ_OBJECT

public:
    AmarokFS_config( TQWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AmarokFS_config();

private:
	TQString themeKUrl;
	TQLabel* textLabel1, *textLabel2, *textLabel3;
	KURLRequester* kURLRequester1;
	TQPushButton* okButton;
	TQPushButton* cancelButton;
	TQCheckBox *disScrSvr;
	TQCheckBox *disAmaOSD;
	TQCheckBox *disThemeHint;
	
	bool isSetDisScrSvr;
	bool isSetDisAmaOSD;
	bool isSetDisThemeHint;
	
	
private slots:
	void saveData();
	void dataChange(const TQString & newKUrl);
	void scrSvrChanged(int state);
	void amaOSDChanged(int state);
	void themeHintChanged(int state);
};

#endif // AMAROKFS_CONFIG_H
