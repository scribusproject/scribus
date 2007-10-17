/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QEvent>
#include <QToolTip>
#include <QKeyEvent>

#include "shortcutwidget.h"


ShortcutWidget::ShortcutWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	Part0 = "";
	Part1 = "";
	Part2 = "";
	Part3 = "";
	keyCode = 0;
	setKeyButton->setCheckable(true);

	languageChange();

	connect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
}

void ShortcutWidget::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void ShortcutWidget::languageChange()
{
	noKey->setToolTip(        tr("No shortcut for the style")); // set no shortcut for this style
	userDef->setToolTip(      tr("Style has user defined shortcut")); // not sure what this thing does
	setKeyButton->setToolTip( tr("Assign a shortcut for the style")); // activate shorcut assigning
}

bool ShortcutWidget::event( QEvent* ev )
{
	bool ret = QWidget::event( ev );
	if ( ev->type() == QEvent::KeyPress )
		keyPressEvent((QKeyEvent*)ev);
	if ( ev->type() == QEvent::KeyRelease )
		keyReleaseEvent((QKeyEvent*)ev);
	return ret;
}

void ShortcutWidget::keyPressEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		QStringList tl;
		if (!keyDisplay->text().isEmpty())
		{
			tl = keyDisplay->text().split("+", QString::SkipEmptyParts);
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift") || Part4 == tr("Meta"))
				Part4 = "";
		}
		else
			Part4 = "";
		switch (k->key())
		{
			case Qt::Key_Meta:
				Part0 = tr("Meta+");
				keyCode |= Qt::META;
				break;
			case Qt::Key_Shift:
				Part3 = tr("Shift+");
				keyCode |= Qt::SHIFT;
				break;
			case Qt::Key_Alt:
				Part2 = tr("Alt+");
				keyCode |= Qt::ALT;
				break;
			case Qt::Key_Control:
				Part1 = tr("Ctrl+");
				keyCode |= Qt::CTRL;
				break;
			default:
				keyCode |= k->key();
				keyDisplay->setText(getKeyText(keyCode));
				setKeyButton->setChecked(false);
				userDef->setChecked(true);
				releaseKeyboard();
				emit newKey(keyDisplay->text());
		}
	}
	if (setKeyButton->isChecked())
	{
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
	}
}

void ShortcutWidget::keyReleaseEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		if (!keyDisplay->text().isEmpty())
		{
			QStringList tl;
			tl = keyDisplay->text().split("+", QString::SkipEmptyParts);
			Part4 = tl[tl.count()-1];
			if (Part4 == tr("Alt") || Part4 == tr("Ctrl") || Part4 == tr("Shift") || Part4 == tr("Meta"))
				Part4 = "";
		}
		else
			Part4 = "";
		if (k->key() == Qt::Key_Meta)
		{
			Part0 = "";
			keyCode &= ~Qt::META;
		}
		if (k->key() == Qt::Key_Shift)
		{
			Part3 = "";
			keyCode &= ~Qt::SHIFT;
		}
		if (k->key() == Qt::Key_Alt)
		{
			Part2 = "";
			keyCode &= ~Qt::ALT;
		}
		if (k->key() == Qt::Key_Control)
		{
			Part1 = "";
			keyCode &= ~Qt::CTRL;
		}
		keyDisplay->setText(Part0+Part1+Part2+Part3+Part4);
	}
}

QString ShortcutWidget::getKeyText(int KeyC)
{
	if ((KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)) == 0)
		return "";
	// on OSX Qt translates modifiers to forsaken symbols, arrows and the like
	// we prefer plain English
	QString res;
	if ((KeyC & Qt::META) != 0)
		res += "Meta+";
	if ((KeyC & Qt::CTRL) != 0)
		res += "Ctrl+";
	if ((KeyC & Qt::ALT) != 0)
		res += "Alt+";
	if ((KeyC & Qt::SHIFT) != 0)
		res += "Shift+";
	return res + QString(QKeySequence(KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)));
}

void ShortcutWidget::setKeyText()
{
	if (setKeyButton->isChecked())
	{
		keyCode = 0;
		Part0 = "";
		Part1 = "";
		Part2 = "";
		Part3 = "";
		Part4 = "";
		grabKeyboard();
	}
	else
		releaseKeyboard();
}

void ShortcutWidget::setShortcut(const QString &shortcut)
{
	disconnect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	disconnect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));

	setKeyButton->setChecked(false);
	if (shortcut.length() > 0)
	{
		userDef->setChecked(true);
		keyDisplay->setText(shortcut);
	}
	else
	{
		noKey->setChecked(true);
		keyDisplay->setText("");
	}

	connect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
}

void ShortcutWidget::setNoKey()
{
	if (noKey->isChecked())
	{
		keyDisplay->setText("");
		emit newKey(QString::null);
	}
}

ShortcutWidget::~ShortcutWidget()
{

}
