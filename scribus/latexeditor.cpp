/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexeditor.cpp  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "latexeditor.h"
#include "latexhighlighter.h"
#include "pageitem_latexframe.h"
#include "prefsmanager.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QFrame>
#include <QFontComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <math.h>

LatexEditor::LatexEditor(PageItem_LatexFrame *frame):QDialog(), frame(frame)
{
	setupUi(this);
	highlighter = new LatexHighlighter(sourceTextEdit->document());
	connect(buttonBox, SIGNAL(accepted()), 
			this, SLOT(okClicked()));
	connect(buttonBox, SIGNAL(rejected()), 
			this, SLOT(cancelClicked()));
	connect(updatePushButton, SIGNAL(clicked(bool)), 
			this, SLOT(updateClicked(bool)));
	connect(revertPushButton, SIGNAL(clicked(bool)), 
			this, SLOT(revertClicked(bool)));
	connect(killPushButton, SIGNAL(clicked(bool)), 
			frame, SLOT(killProcess()));
	connect(frame, SIGNAL(formulaAutoUpdate(QString, QString)), 
			this, SLOT(formulaChanged(QString, QString)));
	connect(frame, SIGNAL(latexFinished()), 
			this, SLOT(latexFinished()));
	connect(frame, SIGNAL(stateChanged(QProcess::ProcessState)), 
			this, SLOT(stateChanged(QProcess::ProcessState)));
	setConfigFile(PrefsManager::instance()->latexEditorConfig());
}

LatexEditor::~LatexEditor()
{
	buttonBox->disconnect();
	exitEditor();
	delete highlighter;
}

void LatexEditor::startEditor()
{
	revert();
	initialize();
	show();
}

void LatexEditor::exitEditor()
{
	hide();
}

void LatexEditor::revert()
{
	sourceTextEdit->setPlainText(frame->getFormula());
}

void LatexEditor::initialize()
{
	preambleCheckBox->setChecked(frame->getUsePreamble());
	dpiSpinBox->setValue(frame->getDpi());
	stateChanged(frame->getState());
	messagesTextEdit->setPlainText(frame->getOutput());
}

void LatexEditor::apply(bool force)
{
	bool changed = false;
	if (frame->getUsePreamble() != preambleCheckBox->isChecked() ||
		frame->getDpi() != dpiSpinBox->value()) {
		changed = true;
		frame->setUsePreamble(preambleCheckBox->isChecked());
		frame->setDpi(dpiSpinBox->value());
	}
	QString key;
	QString value;
	
	QMapIterator<QString, XmlWidget *> i(widgetMap);
	while (i.hasNext()) {
		i.next();
		key = i.key();
		value = i.value()->toString();
		if (frame->editorProperties[key] != value) {
			changed = true;
			frame->editorProperties[key] = value;
		}
	}
	
	
	if (frame->setFormula(sourceTextEdit->toPlainText()) || changed || force) {
		frame->rerunApplication(true);
	}
}

void LatexEditor::formulaChanged(QString oldText, QString newText)
{
	sourceTextEdit->setPlainText(newText);
}

void LatexEditor::okClicked()
{
	apply();
	exitEditor();
}

void LatexEditor::cancelClicked()
{
	revert();
	exitEditor();
}

void LatexEditor::revertClicked(bool unused)
{
	revert();
}

void LatexEditor::updateClicked(bool unused)
{
	apply(true);
}

void LatexEditor::latexFinished()
{
	messagesTextEdit->setPlainText(frame->getOutput());
}

void LatexEditor::stateChanged(QProcess::ProcessState state)
{
	if (state == QProcess::Starting) {
		messagesTextEdit->setPlainText("");
	}
	QString text = tr("Status: ");
	if (state == QProcess::NotRunning) {
		if (frame->getError()) {
			text += tr("Error");
		} else {
			text += tr("Finished");
		}
	} else {
		text += tr("Running");
	}
	statusLabel->setText(text);
	killPushButton->setEnabled(state != QProcess::NotRunning);
}

void LatexEditor::setConfigFile(QString newConfig)
{
	if (configFile == newConfig) return;
	//TODO: Check that file exists
	configFile = newConfig;
	QFileInfo fi(newConfig);
	iconFile = fi.path() + "/" + fi.completeBaseName() + ".tar"; //May be overriden in xml
	loadSettings();
	
	QMapIterator<QString, XmlWidget *> i(widgetMap);
	while (i.hasNext()) {
		i.next();
		QString key = i.key();
		XmlWidget *value = i.value();
		if (frame->editorProperties.contains(key)) {
			value->fromString(frame->editorProperties[key]);
		}
	}
}

#define xmlError() qDebug() << "XML-ERROR:" << xml->lineNumber() \
	<< ":" << xml->columnNumber() << ":"

void LatexEditor::loadSettings()
{
	while (tabWidget->count()>1) {
		QWidget *widget = tabWidget->widget(1);
		tabWidget->removeTab(1);
		delete widget;
	}
	widgetMap.clear();
	
	QFile f(configFile);
	f.open(QIODevice::ReadOnly);
	QXmlStreamReader xml(&f);
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment()) continue;
		if (xml.isStartElement() && xml.name() == "tab") {
			if (xml.attributes().value("type") == "settings") {
				createNewSettingsTab(&xml);
			} else if (xml.attributes().value("type") == "items") {
				createNewItemsTab(&xml);
			} else {
				qDebug() << "XML-ERROR: " << xml.lineNumber() << ":" 
						<< xml.columnNumber() << ":" << "Unknow tab type"
						<< xml.attributes().value("type").toString();
			}
		}
	}
	if (xml.hasError()) {
		qDebug() << "XML-ERROR: " << xml.lineNumber() << ":" 
				<< xml.columnNumber() << ":" << xml.errorString();
	}
}

void LatexEditor::createNewSettingsTab(QXmlStreamReader *xml)
{
	qDebug() << "Creating new tab";
	QStringRef tagname;
	QFrame *newTab = new QFrame();
	newTab->setFrameShape(QFrame::NoFrame);
	QGridLayout *layout = new QGridLayout(newTab);
	layout->setColumnStretch(1, 10);
	QString type = xml->attributes().value("type").toString();
	
	QString title = "No Title";
		
	while (!xml->atEnd()) {
		xml->readNext();
		if (xml->isWhitespace() || xml->isComment()) continue;
		tagname = xml->name();
		if (xml->isEndElement() && (tagname == "tab")) {
			break;
		}
		if (!xml->isStartElement()) {
			xmlError() << "Unexpected element (not a start element)!";
			continue;
		}
		
		if (tagname == "comment") {
			QLabel *label = new QLabel(XmlWidget::readI18nText(xml));
			int row = layout->rowCount();
			label->setWordWrap(true);
			layout->addWidget(label, row, 0, 1, 3);
		} else if (tagname == "title") {
			title = XmlWidget::readI18nText(xml);
		} else {
			XmlWidget *widget = XmlWidget::fromXml(xml);
			if (dynamic_cast<QWidget *>(widget)) {
				QLabel *label = new QLabel(widget->description());
				label->setWordWrap(true);
				QString name = widget->name();
				
				int row = layout->rowCount();
				layout->addWidget(label, row, 0);
				layout->addWidget(dynamic_cast<QWidget *>(widget), row, 1);
			
				/* Commented out, because it doesn't make much sense. All 
				   the options should be handled in the preamble. Keeping this
				   around as a reference for future widgets.
				StringPushButton *button = new StringPushButton(
						tr("Insert"), name);
				connect(button, SIGNAL(clickedWithData(QString)), 
						this, SLOT(tagButtonClicked(QString)));
				layout->addWidget(button, row, 2);*/
			
				if (widgetMap.contains(name)) {
					xmlError() << "There is already an widget with name" << 
						name << "!";
				}
				widgetMap[name] = widget;
			} else {
				xmlError() << "Unexpected tag" << tagname.toString() << 
						"in settings tab";
			}
		}
	}
	layout->setRowStretch(layout->rowCount(), 10);
	tabWidget->addTab(newTab, title);
}

void LatexEditor::createNewItemsTab(QXmlStreamReader *xml) 
{
	QString title = "No Title!";
	
	
	QFrame *newTab = new QFrame();
	newTab->setFrameShape(QFrame::NoFrame);
	QVBoxLayout *vLayout = new QVBoxLayout(newTab);
	
	QListWidget *iconList = new QListWidget();
	iconList->setViewMode(QListView::IconMode);
	iconList->setGridSize(QSize(55, 55));
	iconList->setMovement(QListView::Static);
	connect(iconList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
			this,     SLOT  (newItemSelected   (QListWidgetItem *, QListWidgetItem *)));
	connect(iconList, SIGNAL(itemDoubleClicked (QListWidgetItem *)),
			this,     SLOT  (itemDoubleClicked (QListWidgetItem *)));
	
	QHBoxLayout *hLayout = new QHBoxLayout();
	QLabel *statusLabel = new QLabel(tr("No item selected!"));
	DataPushButton *insertPushButton = new DataPushButton(
			tr("Insert symbol"), iconList);
	connect(insertPushButton, SIGNAL(clickedWithData(QObject *)), 
			this, SLOT(insertButtonClicked(QObject *)));
	hLayout->addWidget(statusLabel, 100);
	hLayout->addWidget(insertPushButton, 0);
	
	vLayout->addWidget(iconList, 100);
	vLayout->addLayout(hLayout, 0);
	
	QStringRef tagname;
	while (!xml->atEnd()) {
		xml->readNext();
		if (xml->isWhitespace() || xml->isComment()) continue;
		tagname = xml->name();
		if (xml->isEndElement() && (tagname == "tab")) {
			break;
		}
		if (!xml->isStartElement()) {
			xmlError() << "Unexpected end element "
					<<tagname.toString()<<"in item tab";
			continue;
		}
		if (tagname == "title") {
			title = XmlWidget::readI18nText(xml);
		} else if (tagname == "item") {
			QString value = xml->attributes().value("value").toString();
			QString img = xml->attributes().value("image").toString();
			QString text = XmlWidget::readI18nText(xml);
			
			QString status = value;
			if (text.isEmpty()) {
				text = value;
			} else if (text != value) {
				status = text + "(" + value +")";
			}
			
			QIcon *icon = 0;
			if (!img.isEmpty()) {
				icon = IconBuffer::instance()->getIcon(iconFile, img);
			}
			QListWidgetItem *item;
			if (!icon) {
				item = new QListWidgetItem(text, iconList);
			} else {
				item = new QListWidgetItem(*icon, "", iconList);
			}
			item->setData(Qt::UserRole, value);
			item->setData(Qt::UserRole + 1, 
				qVariantFromValue((void *) statusLabel)); //UGLY
			item->setToolTip(text);
			item->setStatusTip(status);
		} else {
			xmlError() << "Unexpected tag" << tagname.toString() << 
				"in item tab!";
			continue;
		}
	}
	tabWidget->addTab(newTab, title);
}

void LatexEditor::tagButtonClicked(QString tagname)
{
	sourceTextEdit->insertPlainText("$scribus_"+tagname+"$");
}

void LatexEditor::newItemSelected(QListWidgetItem *newItem, QListWidgetItem *)
{
	QLabel *label = (QLabel *)(newItem->data(Qt::UserRole + 1).value<void *>());
	label->setText(newItem->statusTip());
}

void LatexEditor::itemDoubleClicked(QListWidgetItem *item)
{
	sourceTextEdit->insertPlainText(item->data(Qt::UserRole).toString());
}

void LatexEditor::insertButtonClicked(QObject *widget)
{
	QListWidget *list = dynamic_cast<QListWidget*>(widget);
	Q_ASSERT(list);
	sourceTextEdit->insertPlainText(
		list->currentItem()->data(Qt::UserRole).toString());
}


class SCRIBUS_API XmlFontComboBox : public XmlWidget, public QFontComboBox
{
	public:
		XmlFontComboBox(QXmlStreamReader *xml) : XmlWidget(xml), QFontComboBox()
		{
			fromString(m_defaultValue);
		}
		
		QString toString() const {
			return currentFont().toString();
		}
		
		void fromString(QString str) {
			QFont font;
			font.fromString(str);
			this->setCurrentFont(font);
		}
};

class SCRIBUS_API XmlSpinBox : public XmlWidget, public QSpinBox
{
	public:
		XmlSpinBox(QXmlStreamReader *xml) :  XmlWidget(xml, false), QSpinBox() {
			setRange(
				xml->attributes().value("min").toString().toInt(),
				xml->attributes().value("max").toString().toInt()
			);
			setSingleStep(xml->attributes().value("step").toString().toInt());
			setSpecialValueText(xml->attributes().value("special").toString());
			fromString(m_defaultValue);
			m_description = readI18nText(xml);
		}
		
		QString toString() const {
			if (value() == minimum() && !specialValueText().isEmpty()) {
				return specialValueText();
			} else {
				return QString::number(value());
			}
		}
		
		void fromString(QString str) {
			if (str == specialValueText()) {
				setValue(minimum());
			} else {
				setValue(str.toInt());
			}
		}
};

class SCRIBUS_API XmlDoubleSpinBox : public XmlWidget, public QDoubleSpinBox
{
	public:
		XmlDoubleSpinBox(QXmlStreamReader *xml) :
			XmlWidget(xml, false), QDoubleSpinBox() {
			setRange(
				xml->attributes().value("min").toString().toDouble(),
				xml->attributes().value("max").toString().toDouble()
			);
			setSingleStep(
				xml->attributes().value("step").toString().toDouble());
			setSpecialValueText(xml->attributes().value("special").toString());
			fromString(m_defaultValue);
			m_description = readI18nText(xml);
		}
		
		QString toString() const {
			if (value() == minimum() && !specialValueText().isEmpty()) {
				return specialValueText();
			} else {
				return QString::number(value());
			}
		}
		
		void fromString(QString str) {
			if (str == specialValueText()) {
				setValue(minimum());
			} else {
				setValue(str.toDouble());
			}
		}
};

class SCRIBUS_API XmlLineEdit : public XmlWidget, public QLineEdit
{
	public:
		XmlLineEdit(QXmlStreamReader *xml) :  XmlWidget(xml), QLineEdit() {
			fromString(m_defaultValue);
		}
		
		QString toString() const {
			return text();
		}
		
		void fromString(QString str) {
			setText(str);
		}
};

class SCRIBUS_API XmlTextEdit : public XmlWidget, public QTextEdit
{
	public:
		XmlTextEdit(QXmlStreamReader *xml) :  XmlWidget(xml), QTextEdit() {
			fromString(m_defaultValue);
		}
		
		QString toString() const {
			return toPlainText();
		}
		
		void fromString(QString str) {
			setPlainText(str);
		}
};

class SCRIBUS_API XmlColorPicker : public XmlWidget, public QLabel
{
	public:
		XmlColorPicker(QXmlStreamReader *xml) :  XmlWidget(xml), 
			QLabel("Colorpickers are not implemented yet!") 
		{
			setWordWrap(true);
			fromString(m_defaultValue);
		}
		
		QString toString() const {
			return "Not implemented!";
		}
		
		void fromString(QString str) {
			qDebug() << "Color pickers are not implemented yet!";
		}
};

class SCRIBUS_API XmlComboBox : public XmlWidget, public QComboBox
{
	public:
		XmlComboBox(QXmlStreamReader *xml) :  XmlWidget(xml, false), QComboBox() 
		{
			QStringRef tagname;
			while (!xml->atEnd()) {
				xml->readNext();
				if (xml->isWhitespace() || xml->isComment()) continue;
				tagname = xml->name();
				if (xml->isEndElement() && (tagname == "list")) {
					fromString(m_defaultValue);
					return;
				}
				if (xml->isEndElement()) {
					xmlError() << "Unexpected end element" << tagname.toString();
					continue;
				}
				if (tagname == "title") {
					m_description = readI18nText(xml);
				} else if (tagname == "option") {
					QString value = xml->attributes().value("value").toString();
					QString text = readI18nText(xml);
					addItem(text, value);
				} else {
					xmlError() << "Unexpected tag" << tagname.toString() << 
						"in list!";
				}
			}
		}
		
		QString toString() const {
			return itemData(currentIndex()).toString();
		}
		
		void fromString(QString str) {
			setCurrentIndex(findData(str));
		}
};

XmlWidget* XmlWidget::fromXml(QXmlStreamReader *xml)
{
	QStringRef tagname = xml->name();
	if (tagname == "font") {
		return new XmlFontComboBox(xml);
	}
	if (tagname == "spinbox") {
		if (xml->attributes().value("type") == "double")
			return new XmlDoubleSpinBox(xml);
		else
			return new XmlSpinBox(xml);
	}
	if (tagname == "color") {
		return new XmlColorPicker(xml);
	}
	if (tagname == "text") {
		if (xml->attributes().value("type") == "long")
			return new XmlTextEdit(xml);
		else
			return new XmlLineEdit(xml);
	}
	if (tagname == "list") {
		return new XmlComboBox(xml);
	}
	return 0;
}

XmlWidget::XmlWidget(QXmlStreamReader *xml, bool readDescription)
{
	m_name = xml->attributes().value("name").toString();
	m_defaultValue = xml->attributes().value("default").toString();
	if (readDescription) 
		m_description = readI18nText(xml);
}

//TODO: Which class should this belong to?
QString XmlWidget::readI18nText(QXmlStreamReader *xml)
{
	QString language = PrefsManager::instance()->guiLanguage();
	QString result;
	int matchquality = 0;
	bool i18n = false;
	Q_ASSERT(xml->isStartElement());
	QString startTag = xml->name().toString();
	while (!xml->atEnd()) {
		xml->readNext();
		if (xml->isWhitespace() || xml->isComment()) continue;
		if (xml->isStartElement() && xml->name() == startTag) {
			xmlError() << "Invalid nested elements. We are in big trouble now."
					"Parsing the rest of the document will FAIL!";
			return "Error";
		}
		if (xml->isEndElement() && xml->name() == startTag) {
			return result.trimmed();
		}
		if (i18n) {
			if (xml->isEndElement()) {
				if (xml->name() == "i18n") {
					i18n = false;
				} else {
					xmlError() << "Invalid end element" << 
							xml->name().toString();
				}
				continue;
			}
			if (!xml->isStartElement()) {
				xmlError() << "Unexpected data!";
			}
			if (xml->name() == language) {
				matchquality = 2; //Perfect match
				result = xml->readElementText();
			} else if (language.startsWith(xml->name().toString()) && matchquality <= 1) {
				matchquality = 1; //Only beginning part matches
				result = xml->readElementText();
			} else if (result.isEmpty()) {
				matchquality = 0;
				result = xml->readElementText();
			} else {
				xml->readElementText(); //Ignore the text
			}
		} else {
			if (xml->isStartElement()) {
				if (xml->name() == "i18n") {
					i18n = true;
					continue;
				} else {
					xmlError() << "Tag" << xml->name().toString() << 
						"found, but \"i18n\" or string data expected.";
					continue;
				}
			}
			if (xml->isCharacters()) {
				result = result + xml->text().toString();
			}
		}
	}
	xmlError() << "Unexpected end of XML file";
	return result;
}

void IconBuffer::loadFile(QString filename)
{
	if (loadedFiles.contains(filename)) return;
	loadedFiles << filename;
	qDebug() << "Loading icon file: " << filename;
	file = new QFile(filename);
	file->open(QIODevice::ReadOnly);
	basePos = 0;
	while (!file->atEnd()) {
		QString name = readHeader();
		if (name.isEmpty()) break;
		if (!len) continue;
		icons.insert(filename + ":" + name, readData());
	}
	file->close();
	delete file;
	file = 0;
}

QIcon *IconBuffer::getIcon(QString filename, QString name)
{
	loadFile(filename);
	QString cname = filename + ":" + name;
	if (icons.contains(cname)) {
		return &(icons[cname]);
	} else {
		qDebug() << "Icon" << cname << "not found!";
		return 0;
	}
}

QString IconBuffer::readHeader()
{
	//TODO: Error checking
	Q_ASSERT(file);
	char buffer[101];
	file->seek(basePos);
	file->read(buffer, 100);
	buffer[100] = 0;
	QString name = QString::fromAscii(buffer);
	file->seek(basePos + 124);
	file->read(buffer, 12);
	buffer[12] = 0;
	bool ok;
	len = QString::fromAscii(buffer).toInt(&ok, 8);
	basePos += 512;
	return name;
}

QIcon IconBuffer::readData()
{
	file->seek(basePos);
	QByteArray data = file->read(len);
	QPixmap pixmap;
	pixmap.loadFromData(data);
	basePos += static_cast<int>(ceil(len/512.0) * 512);
	return QIcon(pixmap);
}

IconBuffer *IconBuffer::_instance = 0;
IconBuffer *IconBuffer::instance()
{
	if (!_instance) {
		_instance = new IconBuffer();
	}
	return _instance;
}
