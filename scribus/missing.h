#ifndef MISSINGFONT_H
#define MISSINGFONT_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <scribusdoc.h>

class DmF : public QDialog
{
    Q_OBJECT

public:
    DmF( QWidget* parent, QString fon, SCFonts &avail );
    ~DmF() {};

    QPushButton* PushButton1;
    QLabel* TextLabel6;
    QLabel* PixmapLabel1;
    QComboBox* Replace;
    QLabel* TextLabel1;
    QLabel* TextLabel4;
    QString Ersatz;

public slots:
    virtual void NeuerFont(const QString& e);

protected:
    QHBoxLayout* MissingFontLayout;
    QGridLayout* Layout1;

};

#endif // MISSINGFONT_H
