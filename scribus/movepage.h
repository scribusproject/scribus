/#ifndef MOVEPAGES_H
#define MOVEPAGES_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>

class MovePages : public QDialog
{ 
    Q_OBJECT

public:
    MovePages( QWidget* parent, int act, int max, bool art );
    ~MovePages() {};

    QLabel* TextLabel2;
    QSpinBox* FromPage;
    QSpinBox* ToPage;
    QSpinBox* ActualPage;
    QLabel* TextLabel1;
    QComboBox* Where;
    QPushButton* OK;
    QPushButton* Cancel;
    bool Wie;

protected:
    QVBoxLayout* DLayout;
    QGridLayout* Layout4;
    QHBoxLayout* Layout3;

public slots:
    virtual void FromChanged(int);
    virtual void ToChanged(int);
};

#endif // MOVEPAGES_H
