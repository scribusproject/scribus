/****************************************************************************
** Form interface generated from reading ui file 'Cdel.ui'
**
** Created: Tue Apr 24 22:08:26 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef DELCOLOR_H
#define DELCOLOR_H

class QDialog;
class QLayout;
class QLabel;
class QPushbutton;
class QCombobox;

class DelColor : public QDialog
{ 
    Q_OBJECT

public:
    DelColor(QWidget* parent, CListe colorList, QString colorName, bool haveDoc );
    ~DelColor() {};
	const QString getReplacementColor();

private:
    QLabel* deleteQLabel;
    QLabel* colorToDelQLabel;
    QLabel* replaceQLabel;
    QPushButton* okQPButton;
    QPushButton* cancelQPButton;
    QComboBox* replacementColQCBox;
    QVBoxLayout* dialogLayout;
    QGridLayout* delColorLayout;
    QHBoxLayout* okCancelLayout;

    QString replacementColor;

private slots:
    virtual void ReplaceColor(int);
};

#endif // DELCOLOR_H
