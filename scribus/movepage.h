#ifndef MOVEPAGES_H
#define MOVEPAGES_H

class QDialog;
class QCombobox;
class QLabel;
class QPushbutton;
class QSpinBox;
class QLayout;

class MovePages : public QDialog
{ 
    Q_OBJECT

public:
    MovePages( QWidget* parent, int currentPage, int maxPages, bool moving );
    ~MovePages() {};

	const int getFromPage();
	const int getToPage();
	const int getWhere();
	const int getWherePage();


private:
    QLabel* moveLabel;
    QLabel* toLabel;
    QSpinBox* fromPageData;
    QSpinBox* toPageData;
    QSpinBox* mvWherePageData;
    QComboBox* mvWhereData;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QVBoxLayout* dialogLayout;
    QGridLayout* fromToLayout;
    QHBoxLayout* okCancelLayout;

    bool move;

private slots:
    virtual void fromChanged(int);
    virtual void toChanged(int);
	void mvWherePageDataDisable(int index);
};

#endif // MOVEPAGES_H
