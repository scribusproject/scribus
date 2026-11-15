/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          cupsoptions.cpp  -  description
                             -------------------
    begin                : Fre Jan 3 2003
    copyright            : (C) 2003 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cupsoptions.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPixmap>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPushButton>
#include <QSpacerItem>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QVBoxLayout>

#include "commonstrings.h"
#include "iconmanager.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"

CupsOptions::CupsOptions(QWidget* parent, const QString& device) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Printer Options" ) );
	setWindowIcon(IconManager::instance().loadIcon("app-icon"));
	setSizeGripEnabled(true);

	prefs = PrefsManager::instance().prefsFile->getContext("cups_options");

	CupsOptionsLayout = new QVBoxLayout( this );
	CupsOptionsLayout->setSpacing(6);
	CupsOptionsLayout->setContentsMargins(9, 9, 9, 9);
	Table = new QTableWidget(0, 2, this);
	Table->setSortingEnabled(false);
	Table->setSelectionMode(QAbstractItemView::NoSelection);
	Table->verticalHeader()->hide();
	Table->setHorizontalHeaderItem(0, new QTableWidgetItem( tr("Option")));
	Table->setHorizontalHeaderItem(1, new QTableWidgetItem( tr("Value")));
	QHeaderView* headerH = Table->horizontalHeader();
	headerH->setStretchLastSection(true);
	headerH->setSectionsClickable(false );
	headerH->setSectionsMovable( false );
	headerH->setSectionResizeMode(QHeaderView::Fixed);
	Table->setMinimumSize(300, 100);

#ifdef HAVE_CUPS
	cups_dest_t *dests = nullptr;
	cups_dest_t *dest = nullptr;
	int num_dests = 0;

	// cupsGetDests is still valid but cupsGetDests2 is recommended
	num_dests = cupsGetDests2(CUPS_HTTP_DEFAULT, &dests);
	dest = cupsGetDest(device.toLocal8Bit().constData(), nullptr, num_dests, dests);

	if (dest != nullptr)
	{
		// Use cupsCopyDestInfo instead of cupsGetPPD + ppdOpenFile
		cups_dinfo_t *dinfo = cupsCopyDestInfo(CUPS_HTTP_DEFAULT, dest);

		if (dinfo != nullptr)
		{
			m_keyToDataMap.clear();
			m_keyToDefault.clear();

			// Common IPP attributes to enumerate
			const char *ipp_attributes[] = {
				"media",              // Paper size (e.g. iso_a4_210x297mm, na_letter_8.5x11in)
				"media-source",       // Paper tray (e.g. auto, manual, tray-1)
				"sides",              // Duplex mode (one-sided, two-sided-long-edge, two-sided-short-edge)
				"print-quality",      // Quality enum (3=Draft, 4=Normal, 5=High)
				"print-color-mode",   // Color mode (color, monochrome, auto)
				"output-bin",         // Output tray (face-up, face-down, etc.)
				"finishings",         // Finishing operations enum (3=none, 4=staple, 5=punch, etc.)
				"number-up",          // Pages per sheet (1, 2, 4, 6, 9, 16)
				nullptr
			};

			// Enumerate all supported IPP options

			for (int attr_idx = 0; ipp_attributes[attr_idx] != nullptr; attr_idx++)
			{
				addIPPOption(ipp_attributes[attr_idx], dest, dinfo);
			}
			cupsFreeDestInfo(dinfo);
		}
		cupsFreeDests(num_dests, dests);
	}

	struct OptionData optionData;

	Table->setRowCount(Table->rowCount() + 1);
	Table->setItem(Table->rowCount() - 1, 0, new QTableWidgetItem(QString( tr("Page Set"))));
	QComboBox *item4 = new QComboBox( this );
	item4->setEditable(false);
	m_optionCombos.append(item4);
	optionData.comboIndex = m_optionCombos.count() - 1;
	optionData.keyword = "page-set";
	m_keyToDataMap["Page Set"] = optionData;
	item4->addItem( tr("All Pages"));
	item4->addItem( tr("Even Pages only"));
	item4->addItem( tr("Odd Pages only"));
	int lastSelected = prefs->getInt( tr("Page Set"), 0);
	if (lastSelected >= 3)
		lastSelected = 0;
	item4->setCurrentIndex(lastSelected);
	m_keyToDefault["Page Set"] = tr("All Pages");
	Table->setCellWidget(Table->rowCount() - 1, 1, item4);
	
	Table->setRowCount(Table->rowCount() + 1);
	Table->setItem(Table->rowCount() - 1, 0, new QTableWidgetItem(QString( tr("Mirror"))));
	QComboBox *item2 = new QComboBox( this );
	item2->setEditable(false);
	m_optionCombos.append(item2);
	optionData.comboIndex = m_optionCombos.count() - 1;
	optionData.keyword = "mirror";
	m_keyToDataMap["Mirror"] = optionData;
	item2->addItem(CommonStrings::trNo);
	item2->addItem(CommonStrings::trYes);
	item2->setCurrentIndex(0);
	lastSelected = prefs->getInt( tr("Mirror"), 0);
	if (lastSelected >= 2)
		lastSelected = 0;
	item2->setCurrentIndex(lastSelected);
	m_keyToDefault["Mirror"] = CommonStrings::trNo;
	Table->setCellWidget(Table->rowCount() - 1, 1, item2);
	
	Table->setRowCount(Table->rowCount() + 1);
	Table->setItem(Table->rowCount() - 1, 0, new QTableWidgetItem(QString( tr("Orientation"))));
	QComboBox *item5 = new QComboBox( this );
	item5->setEditable(false);
	m_optionCombos.append(item5);
	optionData.comboIndex = m_optionCombos.count() - 1;
	optionData.keyword = "orientation";
	m_keyToDataMap["Orientation"] = optionData;
	item5->addItem( tr("Portrait"));
	item5->addItem( tr("Landscape"));
	item5->setCurrentIndex(0);
	lastSelected = prefs->getInt( tr("Orientation"), 0);
	if (lastSelected >= 2)
		lastSelected = 0;
	item5->setCurrentIndex(lastSelected);
	m_keyToDefault["Orientation"] = tr("Portrait");
	Table->setCellWidget(Table->rowCount() - 1, 1, item5);

#endif
	Table->resizeColumnsToContents();
	CupsOptionsLayout->addWidget( Table );

	Layout2 = new QHBoxLayout;
	Layout2->setSpacing(6);
	Layout2->setContentsMargins(0, 0, 0, 0);
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout2->addItem( spacer );
	PushButton1 = new QPushButton( CommonStrings::tr_OK, this );
	PushButton1->setDefault( true );
	Layout2->addWidget( PushButton1 );
	PushButton2 = new QPushButton( CommonStrings::tr_Cancel, this );
	PushButton2->setDefault( false );
	PushButton1->setFocus();
	Layout2->addWidget( PushButton2 );
	CupsOptionsLayout->addLayout( Layout2 );
	setMinimumSize( sizeHint() );
	resize(minimumSizeHint().expandedTo(QSize(300, 100)));

	Table->setToolTip( "<qt>" + tr( "This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help > About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *." ) + "</qt>" );

	connect( PushButton2, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( PushButton1, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

#ifdef HAVE_CUPS

/**
 * Queries a printer's IPP attribute and adds it as a row to the options table.
 *
 * For each attribute (e.g., "media", "number-up"), this method:
 * 1. Checks if the printer supports the attribute
 * 2. Enumerates all available values (raw IPP values)
 * 3. Formats them into human-readable display names
 * 4. Determines the printer's current/default value
 * 5. Adds a combo box to the table with the choices
 *
 * Both raw IPP values and display values are stored - raw values are sent
 * to CUPS, display values are shown to the user.
 *
 * @param ipp_name  The IPP attribute name (e.g., "media", "sides")
 * @param dest      CUPS destination (printer)
 * @param dinfo     CUPS destination info containing capabilities
 */
void CupsOptions::addIPPOption(const char* ipp_name, cups_dest_t* dest, cups_dinfo_t* dinfo)
{
	// Find if this attribute is supported
	ipp_attribute_t *attr = cupsFindDestSupported(CUPS_HTTP_DEFAULT, dest, dinfo, ipp_name);
	if (!attr)
	{
		qDebug() << "IPP attribute not supported:" << ipp_name;
		return;
	}

	int count = ippGetCount(attr);
	if (count <= 0)
		return;

	QStringList opts;      // Display values shown in combo
	QStringList rawValues; // Raw IPP values for matching defaults
	QString marked;
	struct OptionData optionData;

	QString optionName = getIPPOptionDisplayName(ipp_name);
	// qDebug() << "Processing IPP attribute:" << ipp_name << "Display:" << optionName << "Count:" << count;

	ipp_tag_t value_tag = ippGetValueTag(attr);

	// IPP attributes can be different types depending on the option:
	// - Strings (KEYWORD/NAME/TEXT/URI): media names, sides values, color modes
	// - Integers (INTEGER/ENUM): number-up, print-quality, finishings
	// Other types (date, boolean, etc.) aren't currently used by our supported attributes
	// Get all available choices
	for (int i = 0; i < count; i++)
	{
		QString rawValue;
		QString displayValue;

		switch (value_tag)
		{
			case IPP_TAG_KEYWORD:
			case IPP_TAG_NAME:
			case IPP_TAG_TEXT:
			case IPP_TAG_URI:
				{
					const char *str = ippGetString(attr, i, nullptr);
					if (str)
					{
						rawValue = QString::fromUtf8(str);
						displayValue = formatIPPDisplayValue(ipp_name, rawValue);
					}
					break;
				}
			case IPP_TAG_INTEGER:
			case IPP_TAG_ENUM:
				{
					int int_val = ippGetInteger(attr, i);
					rawValue = QString::number(int_val);
					displayValue = formatIPPDisplayValue(ipp_name, rawValue);
					break;
				}
			default:
				continue;
		}

		if (!rawValue.isEmpty())
		{
			// qDebug() << "  Value:" << rawValue << "Display:" << displayValue;
			rawValues.append(rawValue);
			opts.append(displayValue);
		}
	}

	if (opts.isEmpty())
		return;

	// Determine the printer's current default value, in order of preference:
	// 1. cupsGetOption() - checks user-set defaults (lpoptions -o ...)
	// 2. cupsFindDestDefault() - falls back to printer's built-in default
	// 3. First available option - if no default is reported at all
	const char *current_value = cupsGetOption(ipp_name, dest->num_options, dest->options);
	if (!current_value)
	{
		ipp_attribute_t *def_attr = cupsFindDestDefault(CUPS_HTTP_DEFAULT, dest, dinfo, ipp_name);
		if (def_attr)
		{
			ipp_tag_t def_tag = ippGetValueTag(def_attr);
			if (def_tag == IPP_TAG_INTEGER || def_tag == IPP_TAG_ENUM)
			{
				static QString intDefault;  // static to keep memory valid
				intDefault = QString::number(ippGetInteger(def_attr, 0));
				current_value = intDefault.toUtf8().constData();
			}
			else
			{
				current_value = ippGetString(def_attr, 0, nullptr);
			}
		}
	}

	if (current_value)
		marked = QString::fromUtf8(current_value);
	else if (!rawValues.isEmpty())
		marked = rawValues.first();

	// qDebug() << "  Default value:" << marked;

	// Add to table
	Table->setRowCount(Table->rowCount() + 1);
	Table->setItem(Table->rowCount() - 1, 0, new QTableWidgetItem(optionName));

	QComboBox *item = new QComboBox(this);
	item->setEditable(false);
	m_optionCombos.append(item);
	optionData.comboIndex = m_optionCombos.count() - 1;
	optionData.keyword = QString::fromUtf8(ipp_name);
	optionData.rawValues = rawValues;
	m_keyToDataMap[optionName] = optionData;

	item->addItems(opts);

	// Restore previously saved selection
	int lastSelected = prefs->getInt(optionName, 0);
	if (lastSelected >= opts.count())
		lastSelected = 0;

	// Try to select the marked/default value (match against raw values)
	if (!marked.isEmpty())
	{
		int markedIdx = rawValues.indexOf(marked);
		if (markedIdx >= 0)
			lastSelected = markedIdx;
	}

	item->setCurrentIndex(lastSelected);
	m_keyToDefault[optionName] = marked;
	Table->setCellWidget(Table->rowCount() - 1, 1, item);
}

// Helper to convert IPP attribute names to human-readable names
QString CupsOptions::getIPPOptionDisplayName(const char* ipp_name) const
{
	QString name(ipp_name);

	// Map common IPP names to friendly display names
	if (name == "media")
		return tr("Paper Size");
	else if (name == "media-source")
		return tr("Paper Source");
	else if (name == "sides")
		return tr("Duplex");
	else if (name == "print-quality")
		return tr("Print Quality");
	else if (name == "print-color-mode")
		return tr("Color Mode");
	else if (name == "output-bin")
		return tr("Output Tray");
	else if (name == "finishings")
		return tr("Finishing");
	else
	{
		// Convert "some-attribute-name" to "Some Attribute Name"
		name.replace('-', ' ');
		if (!name.isEmpty())
		{
			name[0] = name[0].toUpper();
			for (int i = 1; i < name.length(); i++)
			{
				if (name[i-1] == ' ')
					name[i] = name[i].toUpper();
			}
		}
		return name;
	}
}

/**
 * Converts raw IPP values into human-readable, translatable display strings.
 *
 * For example:
 *   "iso_a4_210x297mm"  -> "A4"
 *   "two-sided-long-edge" -> "Two-Sided (Long Edge)"
 *   "3" (print-quality)   -> "Draft"
 *
 * Raw values are still stored separately and sent to CUPS - this is
 * presentation only.
 */
QString CupsOptions::formatIPPDisplayValue(const char* ipp_name, const QString& rawValue) const
{
	QString name(ipp_name);

	// Format number-up
	if (name == "number-up")
	{
		if (rawValue == "1")
			return "1 " + tr("Page per Sheet");
		return rawValue + " " + tr("Pages per Sheet");
	}

	// Format sides (duplex)
	if (name == "sides")
	{
		if (rawValue == "one-sided")
			return tr("One-Sided");
		if (rawValue == "two-sided-long-edge")
			return tr("Two-Sided (Long Edge)");
		if (rawValue == "two-sided-short-edge")
			return tr("Two-Sided (Short Edge)");
	}

	// Format print-quality (enum: 3=Draft, 4=Normal, 5=High)
	if (name == "print-quality")
	{
		if (rawValue == "3")
			return tr("Draft");
		if (rawValue == "4")
			return tr("Normal");
		if (rawValue == "5")
			return tr("High");
	}

	// Format print-color-mode
	if (name == "print-color-mode")
	{
		if (rawValue == "color")
			return tr("Color");
		if (rawValue == "monochrome")
			return tr("Monochrome");
		if (rawValue == "auto")
			return tr("Automatic");
		if (rawValue == "bi-level")
			return tr("Bi-Level");
		if (rawValue == "process-monochrome")
			return tr("Process Monochrome");
	}

	// Format media-source
	if (name == "media-source")
	{
		if (rawValue == "auto")
			return tr("Automatic");
		if (rawValue == "manual")
			return tr("Manual");
		if (rawValue.startsWith("tray-"))
		{
			QString trayNum = rawValue.mid(5);
			return tr("Tray %1").arg(trayNum);
		}
		if (rawValue == "main")
			return tr("Main Tray");
		if (rawValue == "bypass")
			return tr("Bypass Tray");
		if (rawValue == "envelope")
			return tr("Envelope Tray");
	}

	// Format output-bin
	if (name == "output-bin")
	{
		if (rawValue == "face-up")
			return tr("Face Up");
		if (rawValue == "face-down")
			return tr("Face Down");
		if (rawValue == "top")
			return tr("Top");
		if (rawValue == "bottom")
			return tr("Bottom");
	}

	// Format finishings (enum values per RFC 8011)
	if (name == "finishings")
	{
		if (rawValue == "3")
			return tr("None");
		if (rawValue == "4")
			return tr("Staple");
		if (rawValue == "5")
			return tr("Punch");
		if (rawValue == "6")
			return tr("Cover");
		if (rawValue == "7")
			return tr("Bind");
		if (rawValue == "8")
			return tr("Saddle Stitch");
		if (rawValue == "9")
			return tr("Edge Stitch");
		if (rawValue == "10")
			return tr("Fold");
		if (rawValue == "11")
			return tr("Trim");
		if (rawValue == "12")
			return tr("Bale");
		if (rawValue == "13")
			return tr("Booklet Maker");
		if (rawValue == "14")
			return tr("Jog Offset");
		if (rawValue == "20")
			return tr("Staple Top Left");
		if (rawValue == "21")
			return tr("Staple Bottom Left");
		if (rawValue == "22")
			return tr("Staple Top Right");
		if (rawValue == "23")
			return tr("Staple Bottom Right");
	}

	// Format media (paper sizes) - convert IPP names to friendly names
	if (name == "media")
		return formatMediaName(rawValue);

	return rawValue;
}

QString CupsOptions::formatMediaName(const QString& mediaName) const
{
	// Common ISO sizes
	if (mediaName == "iso_a0_841x1189mm")
		return "A0";
	if (mediaName == "iso_a1_594x841mm")
		return "A1";
	if (mediaName == "iso_a2_420x594mm")
		return "A2";
	if (mediaName == "iso_a3_297x420mm")
		return "A3";
	if (mediaName == "iso_a4_210x297mm")
		return "A4";
	if (mediaName == "iso_a5_148x210mm")
		return "A5";
	if (mediaName == "iso_a6_105x148mm")
		return "A6";
	if (mediaName == "iso_a7_74x105mm")
		return "A7";
	if (mediaName == "iso_b4_250x353mm")
		return "B4";
	if (mediaName == "iso_b5_176x250mm")
		return "B5";
	if (mediaName == "iso_b6_125x176mm")
		return "B6";
	if (mediaName == "iso_c5_162x229mm")
		return "C5 " + tr("Envelope");
	if (mediaName == "iso_dl_110x220mm")
		return "DL " + tr("Envelope");

	// JIS sizes
	if (mediaName == "jis_b4_257x364mm")
		return "JIS B4";
	if (mediaName == "jis_b5_182x257mm")
		return "JIS B5";
	if (mediaName == "jis_b6_128x182mm")
		return "JIS B6";

	// North American sizes
	if (mediaName == "na_letter_8.5x11in")
		return tr("Letter");
	if (mediaName == "na_legal_8.5x14in")
		return tr("Legal");
	if (mediaName == "na_executive_7.25x10.5in")
		return tr("Executive");
	if (mediaName == "na_foolscap_8.5x13in")
		return tr("Foolscap");
	if (mediaName == "na_oficio_8.5x13.4in")
		return tr("Oficio");
	if (mediaName == "na_ledger_11x17in")
		return tr("Ledger");
	if (mediaName == "na_tabloid_11x17in")
		return tr("Tabloid");
	if (mediaName == "na_index-4x6_4x6in")
		return tr("Index Card 4x6\"");
	if (mediaName == "na_index-5x8_5x8in")
		return tr("Index Card 5x8\"");
	if (mediaName == "na_number-10_4.125x9.5in")
		return tr("#10 Envelope");
	if (mediaName == "na_monarch_3.875x7.5in")
		return tr("Monarch Envelope");

	// Japanese sizes
	if (mediaName == "jpn_hagaki_100x148mm")
		return tr("Hagaki Postcard");
	if (mediaName == "jpn_oufuku_148x200mm")
		return tr("Oufuku Postcard");

	// Chinese sizes
	if (mediaName == "roc_16k_7.75x10.75in")
		return tr("ROC 16K");

	// Custom sizes - extract the dimensions
	if (mediaName.startsWith("custom_") || mediaName.startsWith("om_"))
	{
		// Format: custom_NAME_WxHmm or om_NAME_WxHmm
		// Try to extract dimensions from the end
		int lastUnderscore = mediaName.lastIndexOf('_');
		if (lastUnderscore > 0)
		{
			QString dims = mediaName.mid(lastUnderscore + 1);
			QString prefix = mediaName.startsWith("custom_") ? tr("Custom") : tr("Photo");
			return QString("%1 (%2)").arg(prefix, dims);
		}
		return mediaName;
	}

	// Fallback: return the raw name
	return mediaName;
}

#endif

CupsOptions::~CupsOptions()
{
	for (int i = 0; i < Table->rowCount(); ++i)
	{
		QComboBox* combo = dynamic_cast<QComboBox*>(Table->cellWidget(i, 1));
		if (combo)
			prefs->set(Table->item(i, 0)->text(), combo->currentIndex());
	}
}

QString CupsOptions::defaultOptionValue(const QString& optionKey) const
{
	return m_keyToDefault.value(optionKey, QString());
}

bool CupsOptions::useDefaultValue(const QString& optionKey) const
{
	QString defValue = defaultOptionValue(optionKey);
	QString optValue = optionRawValue(optionKey);
	return (optValue == defValue);
}

int CupsOptions::optionIndex(const QString& optionKey) const
{
	if (!m_keyToDataMap.contains(optionKey))
		return -1;
	const OptionData& optionData = m_keyToDataMap[optionKey];

	int comboIndex = optionData.comboIndex;
	if (comboIndex < 0 || comboIndex >= m_optionCombos.count())
		return -1;

	QComboBox* optionCombo = m_optionCombos.at(comboIndex);
	return optionCombo->currentIndex();
}

QString CupsOptions::optionText(const QString& optionKey) const
{
	if (!m_keyToDataMap.contains(optionKey))
		return QString();
	const OptionData& optionData = m_keyToDataMap[optionKey];

	int comboIndex = optionData.comboIndex;
	if (comboIndex < 0 || comboIndex >= m_optionCombos.count())
		return QString();

	QComboBox* optionCombo = m_optionCombos.at(comboIndex);
	return optionCombo->currentText();
}


QString CupsOptions::optionRawValue(const QString& optionKey) const
{
	if (!m_keyToDataMap.contains(optionKey))
		return QString();

	const OptionData& optionData = m_keyToDataMap[optionKey];
	int comboIndex = optionData.comboIndex;
	if (comboIndex < 0 || comboIndex >= m_optionCombos.count())
		return QString();

	QComboBox* optionCombo = m_optionCombos.at(comboIndex);
	int currentIdx = optionCombo->currentIndex();

	// If we have raw values stored, return the matching raw value
	if (!optionData.rawValues.isEmpty() && currentIdx >= 0 && currentIdx < optionData.rawValues.count())
		return optionData.rawValues.at(currentIdx);

	// Fall back to display text (for hardcoded options like Page Set, Mirror, Orientation)
	return optionCombo->currentText();
}
