#ifndef DELCOLOR_H
#define DELCOLOR_H

class QDialog;
class QLayout;
class QLabel;
class QPushbutton;
class QCombobox;

#include "scribusapi.h"

class SCRIBUS_API DelColor : public QDialog
{ 
    Q_OBJECT

public:
    DelColor(QWidget* parent, ColorList colorList, QString colorName, bool haveDoc );
    ~DelColor() {};
	const QString getReplacementColor();

private:
    QLabel* deleteLabel;
    QLabel* colorToDelLabel;
    QLabel* replaceLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QComboBox* replacementColData;
    QVBoxLayout* dialogLayout;
    QGridLayout* delColorLayout;
    QHBoxLayout* okCancelLayout;

    QString replacementColor;

private slots:
    virtual void ReplaceColor(int);
};

#endif // DELCOLOR_H
