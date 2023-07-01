/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_UTILS_H
#define PROPERTIESPALETTE_UTILS_H

#include <QListWidgetItem>
#include <QLineEdit>

class QCloseEvent;
class QComboBox;
class QEvent;
class QEvent;
class QFocusEvent;
class QFrame;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QMenu;
class QPushButton;
class QRadioButton;
class QSpinBox;
class QStackedWidget;
class QToolBox;
class QVBoxLayout;
class QWidget;


#include "scribusapi.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "sclistboxpixmap.h"
#include "scpage.h"
#include "sctreewidget.h"

class ScribusDoc;
class ScribusMainWindow;

struct SCRIBUS_API LineStyleValue
{
	multiLine m_Line;
	ScGuardedPtr<ScribusDoc> m_doc;
	QString m_name;
	
	LineStyleValue() = default;
	LineStyleValue(const multiLine& line, ScribusDoc* doc, const QString& name);
	LineStyleValue(const LineStyleValue& other);
	LineStyleValue& operator= (const LineStyleValue& other);
};


Q_DECLARE_METATYPE(LineStyleValue);


class SCRIBUS_API LineStyleItem : public QListWidgetItem
{
	enum usrType {
		LineStyleUserType = UserType + 2
	};
	
public:	
	LineStyleItem( ScribusDoc* doc, const multiLine& line, const QString& name ) : QListWidgetItem(NULL, LineStyleUserType)
	{		
		setText(name);
		setData(Qt::UserRole, QVariant::fromValue(LineStyleValue(line, doc, name))); 
	};
	LineStyleItem( ) : QListWidgetItem(NULL, LineStyleUserType)
	{		
		setText("");
		setData(Qt::UserRole, QVariant::fromValue(LineStyleValue())); 
	};
	LineStyleItem * clone () const { return new LineStyleItem(*this); }
};



class SCRIBUS_API LineStyleItemDelegate : public ScListBoxPixmap<37, 37>
{
	Q_OBJECT
	public:
		LineStyleItemDelegate() : ScListBoxPixmap<37, 37>() {}

		QString text(const QVariant&) const override;
		void redraw(const QVariant&) const override;
};

class SCRIBUS_API NameWidget : public QLineEdit
{
	Q_OBJECT

public:
	NameWidget(QWidget* parent);
	~NameWidget() {}

signals:
	void Leaved();

protected:
	void focusOutEvent(QFocusEvent *) override;
};

class SCRIBUS_API UserActionSniffer : public QObject
{
	Q_OBJECT

public:
	UserActionSniffer( QObject* parent );
protected:
	bool eventFilter(QObject* o, QEvent* e) override;
signals:
	void actionStart();
	void actionEnd();
};

#endif
