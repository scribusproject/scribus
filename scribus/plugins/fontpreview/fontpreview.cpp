/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstring.h>
#include <qspinbox.h>
#include <qregexp.h>

#include "fontpreview.h"
#include "scribus.h"
#include "prefsfile.h"
#include "commonstrings.h"
#include "prefsmanager.h"
#include "scfontmetrics.h"
#include "selection.h"
#include "sampleitem.h"


extern QPixmap SCRIBUS_API loadIcon(QString nam);


FontPreview::FontPreview(QString fontName)
	: FontPreviewBase(ScMW, "FontPreview", true, 0)
{
	setIcon(loadIcon("AppIcon.png"));

	sampleItem = new SampleItem();

	languageChange();

	fontList->setAllColumnsShowFocus(true);
	fontList->setShowSortIndicator(true);
	fontList->setColumnAlignment(1, Qt::AlignCenter);
	fontList->setColumnAlignment(3, Qt::AlignCenter);
	resetDisplayButton->setPixmap(loadIcon("u_undo16.png"));

	/* go through available fonts and check their properties */
	reallyUsedFonts.clear();
	ScMW->doc->getUsedFonts(&reallyUsedFonts);
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	okIcon = loadIcon("ok.png");

	updateFontList("");

	// set initial listitem
	QListViewItem *item;
	if (!fontName.isEmpty())
		item = fontList->findItem(fontName, 0);
	else
	{
		if (ScMW->doc->selection->count() != 0)
			item = fontList->findItem(ScMW->doc->CurrFont, 0);
		else
			item = fontList->findItem(PrefsManager::instance()->appPrefs.toolSettings.defFont, 0);
	}
	if (item != 0)
	{
		fontList->setCurrentItem(item);
		paintSample(fontList->currentItem());
	}

	// scribus config
	defaultStr = tr("Woven silk pyjamas exchanged for blue quartz", "font preview");
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("fontpreview");
	sortColumn = prefs->getUInt("sortColumn", 0);
	fontList->setSorting(sortColumn);
	xsize = prefs->getUInt("xsize", 640);
	ysize = prefs->getUInt("ysize", 480);
	sizeSpin->setValue(prefs->getUInt("fontSize", 18));
	QString ph = prefs->get("phrase", defaultStr);
	displayEdit->setText(ph);
	displayButton_clicked();
	resize(QSize(xsize, ysize).expandedTo(minimumSizeHint()));
}

/**
 * Writes configuration, destroys the object and frees any allocated resources.
 */
FontPreview::~FontPreview()
{
	prefs->set("sortColumn", fontList->sortColumn());
	prefs->set("xsize", width());
	prefs->set("ysize", height());
	prefs->set("fontSize", sizeSpin->value());
	prefs->set("phrase", displayEdit->text());
	sampleItem->cleanupTemporary(); // just to be sure
}

/**
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FontPreview::languageChange()
{
	QToolTip::add(okButton, "<qt>" + tr("Append selected font into Style, Font menu", "font preview") + "</qt>");
	QToolTip::add(cancelButton,tr("Leave preview", "font preview"));
	QToolTip::add(searchEdit, "<qt>" + tr("Typing the text here provides quick searching in the font names. Searching is case insensitive. You can provide a common wild cards (*, ?, [...]) in your phrase. Examples: t* will list all fonts starting with t or T. *bold* will list all fonts with word bold, bolder etc. in the name.") + "</qt>");
	QToolTip::add(searchButton, tr("Start searching"));
	QToolTip::add(sizeSpin, tr("Size of the selected font"));
}

bool FontPreview::allowSample()
{
	if (fontList->childCount() != 0)
		return true;
	fontPreview->setText("No font selected");
	return false;
}

void FontPreview::keyReleaseEvent(QKeyEvent *k)
{
	if (k->isAutoRepeat())
	{
		fontPreview->setText(tr("Sample will be shown after key release"));
		return;
	}
	if (allowSample())
		paintSample(fontList->currentItem());
}

//void FontPreview::fontList_currentChanged(QListViewItem * item)
void FontPreview::fontList_mouseButtonClicked( int, QListViewItem *item, const QPoint &, int )
{
	if (allowSample())
		paintSample(item);
}

void FontPreview::paintSample(QListViewItem *item)
{
	sampleItem->setFontSize(sizeSpin->value() * 10, true);
	sampleItem->setFont(item->text(0));
	QPixmap pixmap = sampleItem->getSample(fontPreview->maximumWidth(),
										   fontPreview->maximumHeight());
	fontPreview->clear();
	if (!pixmap.isNull())
		fontPreview->setPixmap(pixmap);
}

void FontPreview::updateFontList(QString searchStr)
{
	fontList->clear();
	QRegExp re(searchStr);
	re.setCaseSensitive(false);
	re.setWildcard(true);

	for (SCFontsIterator fontIter(PrefsManager::instance()->appPrefs.AvailFonts);
			fontIter.current(); ++fontIter)
	{
		if (searchStr.length()!=0 & !re.exactMatch(fontIter.current()->scName()))
			continue;

		if (fontIter.current()->UseFont)
		{
			QListViewItem *row = new QListViewItem(fontList);
			Foi::FontType type = fontIter.current()->typeCode;

			row->setText(0, fontIter.current()->scName());
			// searching
			if (reallyUsedFonts.contains(fontIter.current()->scName()))
				row->setPixmap(1, okIcon);

			if (type == Foi::OTF)
			{
				row->setPixmap(2, otfFont);
				row->setText(2, "OpenType");
			}
			else
				if (fontIter.current()->Subset)
					row->setPixmap(3, okIcon);

			if (type == Foi::TYPE1) // type1
			{
				row->setPixmap(2, psFont);
				row->setText(2, "Type1");
			}

			if (type == Foi::TTF)
			{
				row->setPixmap(2, ttfFont);
				row->setText(2, "TrueType");
			}

			QFileInfo fi(fontIter.current()->fontFilePath());
			fi.absFilePath().contains(QDir::homeDirPath()) ?
					row->setText(4, tr("User", "font preview")):
					row->setText(4, tr("System", "font preview"));

			fontList->insertItem(row);
		}
	} // for fontIter
}

void FontPreview::searchEdit_textChanged(const QString &s)
{
	if (s.length()==0)
		updateFontList(s);
}

void FontPreview::searchButton_clicked()
{
	updateFontList(searchEdit->text());
}

QString FontPreview::getCurrentFont()
{
	return fontList->currentItem()->text(0);
}

void FontPreview::displayButton_clicked()
{
	sampleItem->setText(displayEdit->text());
	if (allowSample())
		paintSample(fontList->currentItem());
}

void FontPreview::okButton_clicked()
{
	accept();
}

void FontPreview::cancelButton_clicked()
{
	reject();
}

void FontPreview::resetDisplayButton_clicked()
{
	displayEdit->setText(defaultStr);
	displayButton_clicked();
}

#include "fontpreview.moc"
