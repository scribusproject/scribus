#ifndef MARKINDEX_H
#define MARKINDEX_H

#include "markinsert.h"
#include "ui_markindex.h"

class SCRIBUS_API MarkIndex : public MarkInsert, private Ui::MarkIndexDlg
{
    Q_OBJECT

public:
	explicit MarkIndex(QWidget *parent = nullptr) {};
	explicit MarkIndex(const QString& indexEntry, const QString& index, const QStringList& indexList, QWidget *parent = nullptr);
	~MarkIndex() {}

	void values(QString &label) override;
	void setValues(const QString label) override;
	void indexValues(QString &indexEntry, QString &index);
	void setIndexValues(const QString &indexEntry, const QStringList &indexes);

protected:
	void changeEvent(QEvent *e) override;
};

#endif // MARKINDEX_H
