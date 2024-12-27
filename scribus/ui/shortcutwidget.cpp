/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QEvent>
#include <QToolTip>
#include <QKeyEvent>

#include "shortcutwidget.h"


ShortcutWidget::ShortcutWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	setKeyButton->setCheckable(true);

	languageChange();

	connect(noKey, SIGNAL(clicked()), this, SLOT(setNoKey()));
	connect(setKeyButton, SIGNAL(clicked()), this, SLOT(setKeyText()));
}

void ShortcutWidget::setAllowedModifiers(int allowed, int required)
{
	allowedModifiers = allowed;
	requiredModifiers = required;
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
	setKeyButton->setToolTip( tr("Assign a shortcut for the style")); // activate shortcut assigning
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
			tl = keyDisplay->text().split("+", Qt::SkipEmptyParts);
			m_part4 = tl.last();
			if (m_part4 == tr("Alt") || m_part4 == tr("Ctrl") || m_part4 == tr("Shift") || m_part4 == tr("Meta"))
				m_part4.clear();
		}
		else
			m_part4.clear();
		switch (k->key())
		{
			case Qt::Key_Meta:
				if ((allowedModifiers & Qt::META) == Qt::META)
				{
					m_part0 = tr("Meta+");
					m_keyCode |= Qt::META;
				}
				break;
			case Qt::Key_Shift:
				if ((allowedModifiers & Qt::SHIFT) == Qt::SHIFT)
				{
					m_part3 = tr("Shift+");
					m_keyCode |= Qt::SHIFT;
				}
				break;
			case Qt::Key_Alt:
				if ((allowedModifiers & Qt::ALT) == Qt::ALT)
				{
					m_part2 = tr("Alt+");
					m_keyCode |= Qt::ALT;
				}
				break;
			case Qt::Key_Control:
				if ((allowedModifiers & Qt::CTRL) == Qt::CTRL)
				{
					m_part1 = tr("Ctrl+");
					m_keyCode |= Qt::CTRL;
				}
				break;
			default:
				m_keyCode |= k->key();
				keyDisplay->setText(getKeyText(m_keyCode));
				setKeyButton->setChecked(false);
				userDef->setChecked(true);
				releaseKeyboard();
				emit newKey(keyDisplay->text());
		}
	}
	if (setKeyButton->isChecked())
	{
		keyDisplay->setText(m_part0 + m_part1 + m_part2 + m_part3 + m_part4);
	}
}

void ShortcutWidget::keyReleaseEvent(QKeyEvent *k)
{
	if (setKeyButton->isChecked())
	{
		if (!keyDisplay->text().isEmpty())
		{
			QStringList tl;
			tl = keyDisplay->text().split("+", Qt::SkipEmptyParts);
			m_part4 = tl.last();
			if (m_part4 == tr("Alt") || m_part4 == tr("Ctrl") || m_part4 == tr("Shift") || m_part4 == tr("Meta"))
				m_part4.clear();
		}
		else
			m_part4.clear();
		if (k->key() == Qt::Key_Meta)
		{
			m_part0.clear();
			m_keyCode &= ~Qt::META;
		}
		if (k->key() == Qt::Key_Shift)
		{
			m_part3.clear();
			m_keyCode &= ~Qt::SHIFT;
		}
		if (k->key() == Qt::Key_Alt)
		{
			m_part2.clear();
			m_keyCode &= ~Qt::ALT;
		}
		if (k->key() == Qt::Key_Control)
		{
			m_part1.clear();
			m_keyCode &= ~Qt::CTRL;
		}
		keyDisplay->setText(m_part0 + m_part1 + m_part2 + m_part3 + m_part4);
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
	return res + QString(QKeySequence(KeyC & ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT)).toString());
}

void ShortcutWidget::setKeyText()
{
	if (setKeyButton->isChecked())
	{
		m_keyCode = 0;
		m_part0.clear();
		m_part1.clear();
		m_part2.clear();
		m_part3.clear();
		m_part4.clear();
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
		emit newKey(QString());
	}
}

ShortcutWidget::~ShortcutWidget()
{

}
