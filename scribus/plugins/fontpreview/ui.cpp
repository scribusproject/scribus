#include "ui.h"
#include "ui.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstring.h>

#include <prefsfile.h>

extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern QPixmap loadIcon(QString nam);
extern PrefsFile *prefsFile;

/*! Main window for "Font Preview" plugin. It's only gui.
 *
 *  Constructs a FontPreview as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FontPreview::FontPreview(ScribusApp *carrier, QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	this->carrier = carrier;
	if (!name)
		setName( "FontPreview" );
	setIcon(loadIcon("AppIcon.png"));
	// scribus config
	prefs = prefsFile->getPluginContext("fontpreview");
	sortColumn = prefs->getUInt("sortColumn", 0);
	xsize = prefs->getUInt("xsize", 640);
	ysize = prefs->getUInt("ysize", 480);

	FontPreviewLayout = new QGridLayout(this, 1, 1, 10, 5, "FontPreviewLayout");
	layout6 = new QVBoxLayout(0, 0, 5, "layout6");
	layout5 = new QHBoxLayout(0, 0, 5, "layout5");
	fontList = new QListView(this, "fontList" );
	fontList->setAllColumnsShowFocus(true);
	fontList->setShowSortIndicator(true);
	layout5->addWidget(fontList);
	// columns
	fontList->addColumn(tr("Font Name", "font preview"));
	fontList->addColumn(tr("Doc", "font preview"));
	fontList->setColumnAlignment(1, Qt::AlignCenter);
	fontList->addColumn(tr("Type", "font preview"));
	fontList->addColumn(tr("Subset", "font preview"));
	fontList->setColumnAlignment(3, Qt::AlignCenter);
	fontList->addColumn(tr("Access", "font preview"));

	layout2 = new QVBoxLayout(0, 0, 5, "layout2");
	layout1 = new QVBoxLayout(0, 0, 5, "layout1");
	okButton = new QPushButton(this, "okButton");
	layout1->addWidget(okButton);
	cancelButton = new QPushButton(this, "cancelButton");
	layout1->addWidget(cancelButton);
	layout2->addLayout(layout1);
	QSpacerItem* spacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout2->addItem(spacer);
	layout5->addLayout(layout2);
	layout6->addLayout(layout5);
	fontPreview = new QLabel(this, "fontPreview");
	fontPreview->setMinimumSize(QSize(400, 90));
	fontPreview->setFrameShape(QFrame::Box);
	fontPreview->setPaletteBackgroundColor(paletteBackgroundColor());
	layout6->addWidget(fontPreview);

	FontPreviewLayout->addLayout(layout6, 0, 0);
	languageChange();
	resize(QSize(xsize, ysize).expandedTo(minimumSizeHint()));
	layout()->activate();
	clearWState(WState_Polished);

	/* go through available fonts and check their properties */
	QMap<QString,QFont> reallyUsedFonts;
	reallyUsedFonts.clear();
	carrier->GetUsedFonts(&reallyUsedFonts);
	QPixmap ttfFont = loadIcon("font_truetype16.png");
	QPixmap otfFont = loadIcon("font_otf16.png");
	QPixmap psFont = loadIcon("font_type1_16.png");
	QPixmap okIcon = loadIcon("ok.png");

	for (SCFontsIterator fontIter(carrier->Prefs.AvailFonts); fontIter.current(); ++fontIter)
	{
		if (fontIter.current()->UseFont)
		{
			QListViewItem *row = new QListViewItem(fontList);
			QFileInfo fi = QFileInfo(fontIter.current()->Datei);
			QString ext = fi.extension(false).lower();

			row->setText(0, fontIter.current()->SCName);
			if (reallyUsedFonts.contains(fontIter.current()->SCName))
				row->setPixmap(1, okIcon);

			if (ext == "otf")
			{
				row->setPixmap(2, otfFont);
				row->setText(2, "OpenType");
			}
			else
				if (fontIter.current()->Subset)
					row->setPixmap(3, okIcon);

			if ((ext == "pfa") || (ext == "pfb")) // type1
			{
				row->setPixmap(2, psFont);
				row->setText(2, "Type1");
			}

			if (ext == "ttf")
			{
				row->setPixmap(2, ttfFont);
				row->setText(2, "TrueType");
			}

			fi.absFilePath().contains(QDir::homeDirPath()) ?
					row->setText(4, tr("User", "font preview")):
					row->setText(4, tr("System", "font preview"));

			fontList->insertItem(row);
		}
	} // for fontIter

	fontList->setSorting(sortColumn);

	QListViewItem *item;
	if (carrier->DLLinput != "")
		item = fontList->findItem(carrier->DLLinput, 0);
	else
	{
		if (carrier->view->SelItem.count() != 0)
			item = fontList->findItem(carrier->doc->CurrFont, 0);
		else
			item = fontList->findItem(carrier->Prefs.toolSettings.defFont, 0);
	}
	if (item != 0)
	{
		fontList_changed(item);
		fontList->setCurrentItem(item);
	}

	// signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(fontList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(fontList_changed(QListViewItem*)));
}

/**
 * Writes configuration, destroys the object and frees any allocated resources.
 */
FontPreview::~FontPreview()
{
   prefs->set("sortColumn", fontList->sortColumn());
   prefs->set("xsize", width());
   prefs->set("ysize", height());
}

/**
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FontPreview::languageChange()
{
	setCaption(tr("Fonts Preview", "font preview"));
	okButton->setText(tr("&OK", "font preview"));
	okButton->setAccel(QKeySequence(tr("Alt+O", "font preview")));
	cancelButton->setText(tr("&Cancel", "font preview"));
	cancelButton->setAccel(QKeySequence(tr("Alt+C", "font preview")));
	QToolTip::add(okButton, tr("Append selected font into Style, Font menu", "font preview"));
	QToolTip::add(cancelButton,tr("Leave preview", "font preview"));
}

/**
 * Creates pixmap with font sample
 */
void FontPreview::fontList_changed(QListViewItem *item)
{
	uint size = 16;
	QString t = tr("Woven silk pyjamas exchanged for blue quartz", "font preview");
	if (carrier->doc->toolSettings.defSize && carrier->doc->toolSettings.defSize < 28 && carrier->doc->toolSettings.defSize > 10)
		size = carrier->doc->toolSettings.defSize;
	t.replace('\n', " "); // remove French <NL> from translation...
	QString da = carrier->Prefs.AvailFonts[item->text(0)]->Datei;
	QPixmap pixmap = fontSamples(da, size, t, paletteBackgroundColor());
	fontPreview->clear();
	if (!pixmap.isNull())
		fontPreview->setPixmap(pixmap);
}

