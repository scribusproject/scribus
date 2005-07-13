#ifndef PICSTATUS_H
#define PICSTATUS_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qptrlist.h>
#include <qvaluelist.h>
class ScribusDoc;
class ScribusView;
class FileSearch;

class PicStatus : public QDialog
{ 
    Q_OBJECT

public:
    PicStatus(QWidget* parent, ScribusDoc *docu, ScribusView *viewi);
    ~PicStatus() {};

    QTable* PicTable;
    QHeader *Header;
    QPushButton* CancelB;
    QPushButton* OkB;
    ScribusDoc *doc;
    ScribusView *view;
    int Zeilen;
    QPtrList<QCheckBox> FlagsPic;
    QValueList<uint> ItemNrs;

private slots:
    void GotoPic();
    void SearchPic();
    void SearchPicAborted();
    void SearchPicFinished(const QStringList & matches, const QString & fileName);
    void PrintPic();

protected:
    QVBoxLayout* PicStatusLayout;
    QHBoxLayout* Layout2;
	FileSearch* m_search;

signals:
	void selectPage(int);
	void selectMasterPage(QString);
};

#endif // PICSTATUS_H
