/****************************************************************************
** Form interface generated from reading ui file './scripterprefs.ui'
**
** Created: Mo Feb 14 21:47:47 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SCRIPTERPREFERENCES_H
#define SCRIPTERPREFERENCES_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QCheckBox;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QLabel;

class ScripterPreferences : public QDialog
{
    Q_OBJECT

public:
    ScripterPreferences( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ScripterPreferences();

    QCheckBox* extPythonChk;
    QGroupBox* startupScriptGroup;
    QPushButton* startupScriptBrowseBtn;
    QLineEdit* startupScriptPath;
    QLabel* textLabel1;
    QPushButton* cancelButton;
    QPushButton* okButton;
    QGroupBox* advancedOptionsGroup;
    QCheckBox* importNamesChk;
    QCheckBox* legacyAliasesChk;
    QCheckBox* useFakeStdinChk;

public slots:
    virtual void browseForScript();

signals:
    void pathChanged(const QString &);

protected:
    QGridLayout* ScripterPreferencesLayout;
    QSpacerItem* spacer1;
    QGridLayout* startupScriptGroupLayout;
    QGridLayout* advancedOptionsGroupLayout;

protected slots:
    virtual void languageChange();

};

#endif // SCRIPTERPREFERENCES_H
