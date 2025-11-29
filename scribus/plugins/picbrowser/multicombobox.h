/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef MULTICOMBOBOXMODEL_H
#define MULTICOMBOBOXMODEL_H

#include <QComboBox>
#include <QListView>
#include <QStandardItemModel>

class MultiCombobox;
class QEvent;
class QModelIndex;
class QObject;
class QWidget;

class MultiComboboxModel : public QStandardItemModel
{
	Q_OBJECT
	public:
		explicit MultiComboboxModel(QObject* parent = nullptr);
		Qt::ItemFlags flags(const QModelIndex& index) const override;
};

class MultiView : public QListView
{
	Q_OBJECT
	public:
		explicit MultiView(QWidget* parent = nullptr);
		explicit MultiView(MultiCombobox* parent);

		bool eventFilter(QObject* object, QEvent* event) override;

	private:
		MultiCombobox* parentMcb { nullptr };
};

class MultiCombobox : public QComboBox
{
	Q_OBJECT
	public:
		explicit MultiCombobox(QWidget* parent = nullptr);

		int  checkState(int index) const;
		void setCheckState(int index, int checked);
		void switchCheckState(int row);

		int addItem(const QString& text, int checked = 0);

	private:
		MultiComboboxModel *mcbModel { nullptr };
		MultiView *mcbView { nullptr };

	signals:
		void checkStateChanged(int);
};

#endif
