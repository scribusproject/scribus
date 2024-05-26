#ifndef LINESTYLESELECTOR_H
#define LINESTYLESELECTOR_H

#include <QWidget>
#include "scribusapi.h"
#include "scribusdoc.h"
#include "ui_linestyleselector.h"

struct LineStyleValue;

class SCRIBUS_API LineStyleSelector : public QWidget, private Ui::LineStyleSelector
{
	Q_OBJECT

public:
	explicit LineStyleSelector(QWidget *parent = nullptr);
	~LineStyleSelector();

	void setDoc(ScribusDoc *doc);
	void updateLineStyles();

	QListWidget *list() { return lineStyles; }
	QSize iconSize();

private:
	ScribusDoc *m_doc { nullptr };
	QPixmap renderPixmap(LineStyleValue item);
	QSize m_size { QSize(128, 16) };

public slots:
	void languageChange();

private slots:
	void newLine();

signals:
	void itemClicked(QListWidgetItem*);
};

#endif // LINESTYLESELECTOR_H
