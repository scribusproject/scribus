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
#include "latexhelpers.h"
#include "pageitem_latexframe.h"
#include "prefsmanager.h"

#include <QDebug>
#include <QFile>
#include <QFrame>
#include <QFontComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>
#include <math.h>

LatexEditor::LatexEditor(PageItem_LatexFrame *frame):QDialog(), frame(frame)
{
	setupUi(this);
	
	//Fill application list
	programComboBox->clear();
	QStringList configs = PrefsManager::instance()->latexConfigs();
	foreach (QString config, configs) {
		QString name = LatexConfigCache::instance()->parser(config)->description();
		programComboBox->addItem(name, config);
		QString iconname = LatexConfigCache::instance()->parser(config)->icon();
		if (!iconname.isEmpty()) {
			programComboBox->setItemIcon(programComboBox->count()-1,
				icon(config, iconname));
		}
	}
	
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
	connect(frame, SIGNAL(applicationChanged()),
			this, SLOT(updateConfigFile()));
	updateConfigFile();
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
	sourceTextEdit->setPlainText(frame->formula());
}

void LatexEditor::initialize()
{
	preambleCheckBox->setChecked(frame->usePreamble());
	dpiSpinBox->setValue(frame->dpi());
	stateChanged(frame->state());
	messagesTextEdit->setPlainText(frame->output());
}

void LatexEditor::apply(bool force)
{
	bool changed = frame->setFormula(sourceTextEdit->toPlainText());
	
	QString newConfig = programComboBox->itemData(programComboBox->currentIndex()).toString();
	if (newConfig != frame->configFile()) {
		changed = true;
		frame->setConfigFile(newConfig);
	}
	
	if (frame->usePreamble() != preambleCheckBox->isChecked() ||
		frame->dpi() != dpiSpinBox->value()) {
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
	
	
	if (changed || force) {
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
	messagesTextEdit->setPlainText(frame->output());
}

void LatexEditor::stateChanged(QProcess::ProcessState state)
{
	if (state == QProcess::Starting) {
		messagesTextEdit->setPlainText("");
	}
	QString text = tr("Status: ");
	if (state == QProcess::NotRunning) {
		if (frame->error()) {
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

QIcon LatexEditor::icon(QString config, QString fn)
{
	QFileInfo fiConfig(config);
	QFileInfo fiIcon(fiConfig.path()+"/"+fn);
	if (fiIcon.exists() && fiIcon.isReadable()) {
		return QIcon(fiConfig.path()+"/"+fn);
	} else {
		QIcon *tmp = IconBuffer::instance()->icon(
			fiConfig.path() + "/" + fiConfig.completeBaseName() + ".tar", fn);
		if (tmp) return *tmp; else return QIcon();
	}
}


QString LatexEditor::iconFile(QString config)
{
	QFileInfo fiConfig(config);
	return fiConfig.path() + "/" + fiConfig.completeBaseName() + ".tar";
}

void LatexEditor::updateConfigFile()
{
	if (currentConfigFile == frame->configFile()) return;
	currentConfigFile = frame->configFile();
	currentIconFile = iconFile(currentConfigFile);
	QFileInfo fi(currentConfigFile);
	
	if (!fi.exists() || !fi.isReadable()) {
		QMessageBox::critical(0, QObject::tr("Error"), "<qt>" + 
				QObject::tr("Configfile %1 not found or the file is not readable").
				arg(currentConfigFile) + "</qt>", 1, 0, 0);
		return;
	}

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
	
	qDebug() << "Loading editor GUI from " << frame->configFile();
	QFile f(frame->configFile());
	f.open(QIODevice::ReadOnly);
	I18nXmlStreamReader xml(&f);
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
	f.close();
}

void LatexEditor::createNewSettingsTab(I18nXmlStreamReader *xml)
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
			QLabel *label = new QLabel(xml->readI18nText());
			int row = layout->rowCount();
			label->setWordWrap(true);
			layout->addWidget(label, row, 0, 1, 3);
		} else if (tagname == "title") {
			title = xml->readI18nText();
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

void LatexEditor::createNewItemsTab(I18nXmlStreamReader *xml) 
{
	QString title = "No Title!";
	
	
	QFrame *newTab = new QFrame();
	newTab->setFrameShape(QFrame::NoFrame);
	QVBoxLayout *vLayout = new QVBoxLayout(newTab);
	
	QListWidget *iconList = new QListWidget();
	iconList->setViewMode(QListView::IconMode);
	iconList->setGridSize(QSize(55, 55));
	iconList->setMovement(QListView::Static);
	iconList->setFlow(QListView::LeftToRight);
	iconList->setWrapping(true);
	iconList->setResizeMode(QListView::Adjust);
	
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
			title = xml->readI18nText();
		} else if (tagname == "item") {
			QString value = xml->attributes().value("value").toString();
			QString img = xml->attributes().value("image").toString();
			QString text = xml->readI18nText();
			
			QString status = value;
			if (text.isEmpty()) {
				text = value;
			} else if (text != value) {
				status = text + "(" + value +")";
			}
			
			QIcon *icon = 0;
			if (!img.isEmpty()) {
				icon = IconBuffer::instance()->icon(currentIconFile, img);
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
		XmlFontComboBox(I18nXmlStreamReader *xml) : XmlWidget(xml), QFontComboBox()
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
		XmlSpinBox(I18nXmlStreamReader *xml) :  XmlWidget(xml, false), QSpinBox() {
			setRange(
				xml->attributes().value("min").toString().toInt(),
				xml->attributes().value("max").toString().toInt()
			);
			setSingleStep(xml->attributes().value("step").toString().toInt());
			setSpecialValueText(xml->attributes().value("special").toString());
			fromString(m_defaultValue);
			m_description = xml->readI18nText();
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
		XmlDoubleSpinBox(I18nXmlStreamReader *xml) :
			XmlWidget(xml, false), QDoubleSpinBox() {
			setRange(
				xml->attributes().value("min").toString().toDouble(),
				xml->attributes().value("max").toString().toDouble()
			);
			setSingleStep(
				xml->attributes().value("step").toString().toDouble());
			setSpecialValueText(xml->attributes().value("special").toString());
			fromString(m_defaultValue);
			m_description = xml->readI18nText();
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
		XmlLineEdit(I18nXmlStreamReader *xml) :  XmlWidget(xml), QLineEdit() {
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
		XmlTextEdit(I18nXmlStreamReader *xml) :  XmlWidget(xml), QTextEdit() {
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
		XmlColorPicker(I18nXmlStreamReader *xml) :  XmlWidget(xml), 
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
		XmlComboBox(I18nXmlStreamReader *xml) :  XmlWidget(xml, false), QComboBox() 
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
					m_description = xml->readI18nText();
				} else if (tagname == "option") {
					QString value = xml->attributes().value("value").toString();
					QString text = xml->readI18nText();
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

XmlWidget* XmlWidget::fromXml(I18nXmlStreamReader *xml)
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

XmlWidget::XmlWidget(I18nXmlStreamReader *xml, bool readDescription)
{
	m_name = xml->attributes().value("name").toString();
	m_defaultValue = xml->attributes().value("default").toString();
	if (readDescription) 
		m_description = xml->readI18nText();
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

QIcon *IconBuffer::icon(QString filename, QString name)
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
