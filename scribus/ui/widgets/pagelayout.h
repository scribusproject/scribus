/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

#include <QGroupBox>
#include <QList>
#include <QListWidget>

class FormWidget;
class QHBoxLayout;
class QLabel;
class QMenu;
class QToolButton;

#include "scribusapi.h"
#include "scribusstructs.h"
#include "scribusdoc.h"

/*! \brief A widget for changing pages layout.
User can change layouts on-the-fly changing widget's
comboboxes. */
class SCRIBUS_API PageLayouts : public QWidget
{
	Q_OBJECT

public:
	PageLayouts(QWidget* parent);
	~PageLayouts() {};

	void updateSchemeSelector(QList<PageSet> pageSets, int pagePositioning);
	void setFirstPage(int nr);
	int firstPage() const { return m_firstPage; };
	void setScheme(int nr);
	int scheme() const { return m_scheme; };

	void setHideLabelsPermanently(bool hide);
	bool hideLabelsPermanently() const { return m_hideLabels; };

public slots:
	void toggleLabelVisibility(bool visibility);

signals:
	void schemeChanged(int);
	void firstPageChanged(int);

private:
	int m_scheme {0};
	int m_firstPage {0};
	bool m_hideLabels {false};
	int docPagePositioning {0};
	QList<PageSet> m_pageSets;

	FormWidget* labelScheme { nullptr };
	FormWidget* labelPages { nullptr };
	QToolButton* buttonScheme { nullptr };
	QToolButton* buttonFirstPage { nullptr };
	QMenu* menuScheme { nullptr };
	QMenu* menuFirstPage { nullptr };
	QHBoxLayout* layoutGroupLayout { nullptr };

	void reloadScheme();
	void reloadFirstPage(int scheme);

protected slots:
	void languageChange();
	void changeScheme(QAction* action);
	void changeFirstPage(QAction* action);

};

#endif // PAGELAYOUT_H
