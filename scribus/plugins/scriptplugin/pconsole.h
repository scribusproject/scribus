#ifndef PCONSOLE_H
#define PCONSOLE_H

// #include <qdialog.h>
#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <conswin.h>

class PConsole : public QWidget
{ 
    Q_OBJECT

public:
    PConsole( QWidget* parent );
    ~PConsole() {};
    void closeEvent(QCloseEvent *ce);
    ConsWin* OutWin;

signals:
		void Schliessen();

protected:
    QVBoxLayout* PConsoleLayout;
};

#endif // PCONSOLE_H
