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
	layout5->addWidget(fontList);
	// columns
	fontList->addColumn(tr("Font Name"));
	fontList->addColumn(tr("Doc"));
	fontList->addColumn(tr("Type"));
	fontList->addColumn(tr("Subset"));

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
	QPixmap ttfFont = loadIcon("font_truetype.png");
	QPixmap otfFont = loadIcon("font_otf.png");
	QPixmap psFont = loadIcon("font_type1.png");
	QPixmap okIcon = loadIcon("ok.png");
	for (SCFontsIterator fontIter(carrier->Prefs.AvailFonts); fontIter.current(); ++fontIter)
	{
		if (fontIter.current()->UseFont)
		{
			QFileInfo fi = QFileInfo(fontIter.current()->Datei);
			QString ext = fi.extension(false).lower();
			QListViewItem *row = new QListViewItem(fontList);
			row->setText(0, fontIter.current()->SCName);
			if (reallyUsedFonts.contains(fontIter.current()->SCName))
				row->setPixmap(1, okIcon);
			if ((ext == "pfa") || (ext == "pfb")) // type1
				row->setPixmap(2, psFont);
			if (ext == "ttf")
				row->setPixmap(2, ttfFont);
			if (ext == "otf")
				row->setPixmap(2, otfFont);
			if (fontIter.current()->Subset)
				row->setPixmap(3, okIcon);
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
   prefs->set("xsize", xsize);
   prefs->set("ysize", ysize);
}

/**
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FontPreview::languageChange()
{
	setCaption( tr( "Fonts Preview" ) );
	okButton->setText( tr( "&OK" ) );
	okButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
	cancelButton->setText( tr( "&Cancel" ) );
	cancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
	QToolTip::add(okButton, tr("Append selected font into Style, Font menu"));
	QToolTip::add(cancelButton,tr("Leave preview"));
}

/**
 * Creates pixmap with font sample
 */
void FontPreview::fontList_changed(QListViewItem *item)
{
	uint size = 16;
	QString t = tr("Woven silk pyjamas exchanged for blue quartz");
	if (carrier->doc->toolSettings.defSize && carrier->doc->toolSettings.defSize < 28 && carrier->doc->toolSettings.defSize > 10)
		size = carrier->doc->toolSettings.defSize;
	t.replace('\n', " "); // remove French <NL> from translation...
	QString da = carrier->Prefs.AvailFonts[item->text(0)]->Datei;
	QPixmap pixmap = fontSamples(da, size, t, paletteBackgroundColor());
	fontPreview->clear();
	if (!pixmap.isNull())
		fontPreview->setPixmap(pixmap);
}

