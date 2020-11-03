/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGESELECTOR_H
#define PAGESELECTOR_H

#include <QWidget>

class QEvent;
class QHBoxLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QIntValidator;

#include "scribusapi.h"
#include "styleoptions.h"

class QComboBox;

class SCRIBUS_API PageSelector : public QWidget
{
	Q_OBJECT

public:
	PageSelector(QWidget* parent, int maximumPage = 0 );
	~PageSelector() {};
	
	bool hasFocus();
	void focusPolicy(Qt::FocusPolicy policy);
	void setFont(const QFont& font);
	int getCurrentPage();

public slots:
	virtual void setGUIForPage(int i);
	virtual void setMaximum(int i);
	void iconSetChange();
	void languageChange();
	void clearFocus();

private slots:
	virtual void gotoPage(int);
	virtual void goToStart();
	virtual void goToEnd();
	virtual void goBackward();
	virtual void goForward();

protected:
	QPushButton* startButton { nullptr };
	QPushButton* backButton { nullptr };
	QPushButton* forwardButton { nullptr };
	QPushButton* lastButton { nullptr };
	QHBoxLayout *pageSelectorLayout { nullptr };
	QLabel *pageCountLabel { nullptr };
	QComboBox *m_pageCombo { nullptr };
	QIntValidator *m_validator { nullptr };

	int m_lastPage { 1 };
	int m_currentPage { 1 };
	QString pageCountString;
	
	void changeEvent(QEvent *e) override;

signals:
	void pageChanged(int);
};

#endif // PAGESELECTOR_H
