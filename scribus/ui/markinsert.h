#ifndef MARKINSERT_H
#define MARKINSERT_H

#include <QDialog>
#include "scribusapi.h"

class NotesStyle;
class Mark;
class PageItem;

class SCRIBUS_API MarkInsert : public QDialog
{
	Q_OBJECT

public:
	MarkInsert(const QList<Mark*>&, QWidget* parent = nullptr) : QDialog(parent) {}
	MarkInsert(const Mark*, QWidget* parent = nullptr) : QDialog(parent) {}
	MarkInsert(const QList<NotesStyle*>&, QWidget* parent = nullptr) : QDialog(parent) {}
	MarkInsert(QWidget* parent = nullptr) : QDialog(parent) {}
	virtual ~MarkInsert() {}

	virtual void values(QString &label) {}
	virtual void values(QString &label, PageItem* &item) {}
	virtual Mark* values(QString& label, QString& text) { return NULL; }
	virtual void values(QString& label, Mark* &mrk) {}
	virtual NotesStyle* values() { return NULL; }

	virtual void setValues(const QString label) {}
	virtual void setValues(const QString label, const QString text) {}
	virtual void setValues(const QString label, const PageItem* ptr) {}
	virtual void setValues(const QString label, const Mark* ptr) {}
	virtual void setValues(const NotesStyle* defaultStyle) {}
	
signals:

public slots:

};

#endif // MARKINSERT_H
