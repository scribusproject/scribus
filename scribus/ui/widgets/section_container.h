#ifndef SECTIONCONTAINER_H
#define SECTIONCONTAINER_H

#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "scribusapi.h"

/* ********************************************************************************* *
 *
 * Section Container Header
 *
 * ********************************************************************************* */

class SCRIBUS_API SectionContainerHeader : public QWidget
{
	Q_OBJECT

	friend class SectionContainer;

public:
	SectionContainerHeader(QWidget *parent = nullptr);
	SectionContainerHeader(QString title, QWidget *parent = nullptr);

	QPushButton * buttonCollapse;

private:

	QHBoxLayout * layoutHeader;
	QHBoxLayout * layoutHeaderPrefix;
	QHBoxLayout * layoutHeaderSuffix;

	QLabel * labelTitle;

	bool boolHasStyle;

protected:
	void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *mouseEvent);

signals:
	 void toggleCollpasedState();

};

/* ********************************************************************************* *
 *
 * Section Container
 *
 * ********************************************************************************* */

class SCRIBUS_API SectionContainer : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Version", "1.0.0")

    Q_PROPERTY(QString title READ text WRITE setText)
    Q_PROPERTY(bool isCollapsible READ isCollapsible WRITE setIsCollapsible)
    Q_PROPERTY(bool isCollapsed READ isCollapsed WRITE setIsCollapsed)
    Q_PROPERTY(bool hasStyle READ hasStyle WRITE setHasStyle)


public:
    SectionContainer(QWidget *parent = nullptr);
    SectionContainer(QString title, bool isCollapsible = true, QWidget *parent = nullptr);

    void setText(QString text);
    QString text() const;

    void setFont(QFont font);
    QFont font();

	void setHasStyle(bool hasStyle);
	bool hasStyle();

    void setIsCollapsible(bool isCollapsible);
    bool isCollapsible();
    bool isCollapsed();

	void setCollapseIcons(QIcon collapsed, QIcon expanded);

	void setWidget(QWidget * widget);
	QWidget *widget();

	void addHeaderSuffixWidget(QWidget *widget);
	void insertHeaderSuffixWidget(int index, QWidget *widget);
	void removeHeaderSuffixWidget(QWidget *widget);
	void addHeaderSuffixSpacing(int size);
	void insertHeaderSuffixSpacing(int index, int size);

	void addHeaderPrefixWidget(QWidget *widget);
	void insertHeaderPrefixWidget(int index, QWidget *widget);
	void removeHeaderPrefixWidget(QWidget *widget);
	void addHeaderPrefixSpacing(int size);
	void insertHeaderPrefixSpacing(int index, int size);

private:
	SectionContainerHeader *widgetHeader;
	QVBoxLayout *mainLayout;
	QIcon iconCollapsed;
	QIcon iconExpanded;
    bool boolIsCollapsible;
	bool boolIsCollapsed;
    bool boolHasStyle;

    void connectSlots();

	void paintEvent(QPaintEvent *event);
//	bool eventFilter(QObject *object, QEvent *event);

public slots:
    void toggleCollpasedState();
    void setIsCollapsed(bool state);
	void iconSetChange();

signals:
    void collapsedState(bool);
    void currentIndexChanged(int index);


};

#endif // SECTIONCONTAINER_H
