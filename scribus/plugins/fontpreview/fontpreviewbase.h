/****************************************************************************
** Form interface generated from reading ui file 'fontpreviewbase.ui'
**
** Created: Út bře 21 13:21:09 2006
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FONTPREVIEWBASE_H
#define FONTPREVIEWBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;
class QListView;
class QListViewItem;
class QSpinBox;

class FontPreviewBase : public QDialog
{
    Q_OBJECT

public:
    FontPreviewBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~FontPreviewBase();

    QLabel* searchLabel;
    QLineEdit* searchEdit;
    QPushButton* searchButton;
    QListView* fontList;
    QLabel* fontPreview;
    QLabel* sizeLabel;
    QSpinBox* sizeSpin;
    QLabel* displayLabel;
    QLineEdit* displayEdit;
    QPushButton* displayButton;
    QPushButton* resetDisplayButton;
    QPushButton* okButton;
    QPushButton* cancelButton;

public slots:
    virtual void displayButton_clicked();
    virtual void searchEdit_textChanged( const QString & );
    virtual void searchButton_clicked();
    virtual void okButton_clicked();
    virtual void cancelButton_clicked();
    virtual void resetDisplayButton_clicked();
    virtual void fontList_mouseButtonClicked( int, QListViewItem *, const QPoint &, int );
    virtual void sizeSpin_valueChanged( int );

protected:
    QGridLayout* FontPreviewBaseLayout;
    QVBoxLayout* layout13;
    QHBoxLayout* layout1;
    QHBoxLayout* layout12;
    QHBoxLayout* layout3;
    QHBoxLayout* layout11;
    QHBoxLayout* layout4;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // FONTPREVIEWBASE_H
