/****************************************************************************
** Form interface generated from reading ui file 'barcodegeneratorbase.ui'
**
** Created: Út led 24 12:48:51 2006
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
class QCheckBox;
class QPushButton;
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
    QCheckBox* textCheck;
    QCheckBox* guardCheck;
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

protected:
    QGridLayout* BarcodeGeneratorBaseLayout;
    QVBoxLayout* layout15;
    QHBoxLayout* layout13;
    QGridLayout* bcodeBoxLayout;
    QVBoxLayout* layout12;
    QHBoxLayout* layout11;
    QVBoxLayout* layout10;
    QVBoxLayout* layout9;
    QGridLayout* colorBoxLayout;
    QHBoxLayout* layout8;
    QVBoxLayout* layout6;
    QVBoxLayout* layout7;
    QHBoxLayout* layout14;
    QHBoxLayout* layout10_2;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

};

#endif // BARCODEGENERATORBASE_H
