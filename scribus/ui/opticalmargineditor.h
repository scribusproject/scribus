#ifndef OPTICALMARGINEDITOR_H
#define OPTICALMARGINEDITOR_H

#include <QWidget>

#include "opticalmarginlookup.h"
#include "ui_opticalmargineditor.h"


class OpticalMarginEditor : public QWidget, private Ui::OpticalMarginEditor
{
	Q_OBJECT

public:
	explicit OpticalMarginEditor(QWidget *parent = nullptr);

	void setOpticalMarginSets(const OpticalMarginSets& sets);
	const OpticalMarginSets& opticalMarginSets() { return m_sets; };

	void setOpticalMarginSetId(const QString& id);
	const QString& opticalMarginSetId() { return m_setId; };

	void createTable();

private:
	OpticalMarginSets m_sets;
	QString m_setId;

	void addRow(const QString &s, double left, double right, int unit);
	void updateSetSelector();

protected:
	void changeEvent(QEvent *e) override;

private slots:
	void resetCurrentSet();
	void removeCurrentSet();
	void addNewSet();
	void resetAllSets();
	void addMissingDefaults();
	void loadSet();
	void removeCurrentRow();
	void addNewRow();
	void updateMarginRules();
	void updatePreview();
	void updateUnit();

public slots:
	void languageChange();
	void iconSetChange();

signals:
	void setChanged(); // event not used yet
	void setDeleted(QString oldSetId, QString newSetId);

};

#endif // OPTICALMARGINEDITOR_H
