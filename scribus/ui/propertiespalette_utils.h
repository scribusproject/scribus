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
#include "page.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "sclistboxpixmap.h"
#include "sctreewidget.h"

class ScribusDoc;
class ScribusMainWindow;

struct SCRIBUS_API LineFormatValue
{
	multiLine m_Line;
	ScGuardedPtr<ScribusDoc> m_doc;
	QString m_name;
	
	LineFormatValue();
	LineFormatValue( const multiLine& line, ScribusDoc* doc, const QString name );
	LineFormatValue(const LineFormatValue& other);
	LineFormatValue& operator= (const LineFormatValue& other);
};


Q_DECLARE_METATYPE(LineFormatValue);


class SCRIBUS_API LineFormatItem : public QListWidgetItem
{	
	enum {
		LineFormatUserType = UserType + 2
	} usrtyp;
	
public:	
	LineFormatItem( ScribusDoc* doc, const multiLine& line, const QString& name ) : QListWidgetItem(NULL, LineFormatUserType)
	{		
		setText(name);
		setData(Qt::UserRole, QVariant::fromValue(LineFormatValue(line, doc, name))); 
	};
	LineFormatItem( ) : QListWidgetItem(NULL, LineFormatUserType)
	{		
		setText("");
		setData(Qt::UserRole, QVariant::fromValue(LineFormatValue())); 
	};
	LineFormatItem * clone () const { return new LineFormatItem(*this); }
};



class SCRIBUS_API LineFormatItemDelegate : public ScListBoxPixmap<37, 37>
{
public:
	LineFormatItemDelegate() : ScListBoxPixmap<37, 37>() {}
	virtual int rtti() const { return 148523874; }
	virtual QString text(const QVariant&) const;
	virtual void redraw(const QVariant&) const;
};

class SCRIBUS_API NameWidget : public QLineEdit
{
	Q_OBJECT

public:
	NameWidget(QWidget* parent);
	~NameWidget() {};

signals:
	void Leaved();

protected:
	virtual void focusOutEvent(QFocusEvent *);
};

class SCRIBUS_API UserActionSniffer : public QObject
{
	Q_OBJECT

public:
	UserActionSniffer( QObject* parent );
protected:
	bool eventFilter( QObject *o, QEvent *e );
signals:
	void actionStart();
	void actionEnd();
};

#endif
