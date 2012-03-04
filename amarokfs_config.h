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

#include <qvariant.h>
#include <qlabel.h>
#include <kurlrequester.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qdialog.h>
#include <qcheckbox.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class KURLRequester;
class QPushButton;

class AmarokFS_config : public QDialog
{
    Q_OBJECT

public:
    AmarokFS_config( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~AmarokFS_config();

private:
	QString themeKUrl;
	QLabel* textLabel1, *textLabel2, *textLabel3;
	KURLRequester* kURLRequester1;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QCheckBox *disScrSvr;
	QCheckBox *disAmaOSD;
	QCheckBox *disThemeHint;
	
	bool isSetDisScrSvr;
	bool isSetDisAmaOSD;
	bool isSetDisThemeHint;
	
	
private slots:
	void saveData();
	void dataChange(const QString & newKUrl);
	void scrSvrChanged(int state);
	void amaOSDChanged(int state);
	void themeHintChanged(int state);
};

#endif // AMAROKFS_CONFIG_H
