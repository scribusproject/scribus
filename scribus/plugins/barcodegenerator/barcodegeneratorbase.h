/****************************************************************************
** Form interface generated from reading ui file 'barcodegeneratorbase.ui'
**
** Created: Út bře 7 10:41:43 2006
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef BARCODEGENERATORBASE_H
#define BARCODEGENERATORBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QTextEdit;

class BarcodeGeneratorBase : public QDialog
{
    Q_OBJECT

public:
    BarcodeGeneratorBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~BarcodeGeneratorBase();

    QGroupBox* bcodeBox;
    QLabel* bcLabel;
    QLabel* codeLabel;
    QComboBox* bcCombo;
    QLineEdit* codeEdit;
    QPushButton* resetButton;
    QCheckBox* textCheck;
    QCheckBox* guardCheck;
    QCheckBox* includeCheck;
    QCheckBox* includeCheckInText;
    QGroupBox* colorBox;
    QPushButton* bgColorButton;
    QPushButton* lnColorButton;
    QPushButton* txtColorButton;
    QLabel* bgLabel;
    QLabel* linesLabel;
    QLabel* txtLabel;
    QTextEdit* commentEdit;
    QLabel* sampleLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;

public slots:
    virtual void bcComboChanged();
    virtual void textCheck_changed();
    virtual void guardCheck_changed();
    virtual void bgColorButton_pressed();
    virtual void lnColorButton_pressed();
    virtual void txtColorButton_pressed();
    virtual void okButton_pressed();
    virtual void cancelButton_pressed();
    virtual void codeEdit_textChanged( const QString & );
    virtual void resetButton_clicked();
    virtual void includeCheck_stateChanged( int );
    virtual void includeCheckInText_stateChanged( int );

protected:
    QGridLayout* BarcodeGeneratorBaseLayout;
    QVBoxLayout* layout15;
    QHBoxLayout* layout14;
    QGridLayout* bcodeBoxLayout;
    QVBoxLayout* layout13;
    QHBoxLayout* layout15_2;
    QVBoxLayout* layout10;
    QVBoxLayout* layout14_2;
    QHBoxLayout* layout12;
    QGridLayout* colorBoxLayout;
    QHBoxLayout* layout8;
    QVBoxLayout* layout6;
    QVBoxLayout* layout7;
    QHBoxLayout* layout14_3;
    QHBoxLayout* layout10_2;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // BARCODEGENERATORBASE_H
