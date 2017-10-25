/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QItemSelectionModel>

#include "fonts/scface.h"
#include "scribusdoc.h"
#include "scpainter.h"
#include "chartablemodel.h"


CharTableModel::CharTableModel(QObject *parent, int cols, ScribusDoc * doc, const QString & font)
		: QAbstractTableModel(parent),
		m_doc(doc),
		m_cols(cols),
		m_viewWidth(200),
		m_fontInUse(font)
{
	m_selectionModel = new QItemSelectionModel(this);
	m_characters.clear();
	m_fonts.clear();
}

int CharTableModel::rowCount(const QModelIndex & parent) const
{
	return m_doc || !parent.isValid() ? m_characters.count() / m_cols + 1: 0;
}

int CharTableModel::columnCount(const QModelIndex & /* parent */) const
{
	return m_cols;
}

QVariant CharTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || m_doc == 0)
		return QVariant();

	int ix = index.row() * m_cols + index.column();
	uint currentChar;
	QString currentFont = m_fontInUse;
	if (ix < m_characters.count())
	{
		currentChar = m_characters[ix];
		currentFont = m_fonts[ix];
	}
	else
		return QVariant();

	// for mimeData()
	if (role == CharTableModel::CharTextRole)
		return QString("%1").arg(QChar(currentChar));

	if (role == CharTableModel::CharTextAndFontRole)
		return QString("%1#%2").arg(currentChar).arg(currentFont);

	// tooltip
	if (role == Qt::ToolTipRole)
		return QString("Unicode:\n0x%1").arg(currentChar, 4, 16, QChar('0'));

	// status tip
	if ( role == Qt::StatusTipRole )
	{
		QString tmp = QString("%1").arg(currentChar, 4, 16, QChar('0')).toUpper();
		QStringList lst;
		lst << tmp << currentFont;
		return lst;
	}

	// pixmap
	if (role == Qt::DecorationRole)
	{
		// m_cols should not become 0. Never.
		int baseSize = m_viewWidth / m_cols;
		QTransform chma;
		chma.scale(baseSize/10, baseSize/10);

		ScFace face = (*m_doc->AllFonts)[currentFont];
		uint gl = face.char2CMap(currentChar);
		int size = baseSize + qRound(-face.descent() * baseSize) + 1;
		double ww = baseSize - face.glyphWidth(gl, baseSize);

		QImage pix(baseSize, size, QImage::Format_ARGB32_Premultiplied);
		ScPainter *p = new ScPainter(&pix, baseSize, size);
		p->clear();
		FPointArray gly = face.glyphOutline(gl, 1);
		if (gly.size() > 4)
		{
			gly.map(chma);
			p->translate(ww / 2, 1);
			p->setBrush(Qt::black);
			p->setFillMode(1);
			p->setupPolygon(&gly);
			p->fillPath();
			p->end();
		}
		delete p;
		return QVariant(QPixmap::fromImage(pix));
	}
	// trash
	return QVariant();
}

void CharTableModel::setDoc(ScribusDoc *doc)
{
	// repaint only when doc differs
	if (doc != m_doc)
	{
		beginResetModel();
		m_doc = doc;
		endResetModel();
	}
}

ScFace CharTableModel::fontFace()
{
	return (*m_doc->AllFonts)[m_fontInUse];
}

void CharTableModel::setCharacters(CharClassDef ch)
{
	beginResetModel();
	m_characters.clear();
	m_fonts.clear();
	m_characters = ch;
	for (int a = 0; a < m_characters.count(); a++)
	{
		m_fonts.append(m_fontInUse);
	}
	endResetModel();
}

void CharTableModel::setCharactersAndFonts(CharClassDef ch, QStringList fonts)
{
	beginResetModel();
	m_characters.clear();
	m_fonts.clear();
	m_characters = ch;
	m_fonts = fonts;
	endResetModel();
}

void CharTableModel::addCharacter(QString ch)
{
	int orig = rowCount();
	bool ok;
	int a = ch.indexOf(" ");
	QString si = ch.left(a);
	QString sf = ch.mid(a+1);
	int val = si.toInt(&ok, 10);
	if (!ok)
		return;
	beginResetModel();
	m_characters.append(val);
	m_fonts.append(sf);
	endResetModel();
	if (orig < rowCount())
		emit rowAppended();
}

void CharTableModel::setFontInUse(QString font)
{
	if (font != m_fontInUse)
	{
		beginResetModel();
		m_fontInUse = font;
		endResetModel();
	}
}

void CharTableModel::appendUnicode(const QString & s, uint base)
{
	int orig = rowCount();
	bool ok;
	int a = s.indexOf("#");
	QString si = s.left(a);
	QString sf = (a >= 0) ? s.mid(a+1) : "";
	if (sf.isEmpty())
		sf = m_fontInUse;
	int val = si.toInt(&ok, base);
	if (!ok)
		return;

	if ((!m_characters.contains(val)) || (!m_fonts.contains(sf)))
	{
		beginResetModel();
		m_characters.append(val);
		m_fonts.append(sf);
		endResetModel();
	}
	else
	{
		// select already contained glyph
		int ix = m_characters.indexOf(val);
		int ixrow = ix / m_cols;
		int ixcol = ix % m_cols;
		m_selectionModel->select(index(ixrow, ixcol, QModelIndex()), QItemSelectionModel::ClearAndSelect);
		emit selectionChanged(m_selectionModel);
	}

	if (orig < rowCount())
		emit rowAppended();
}

bool CharTableModel::removeCharacter(int index)
{
	if (index >= 0 && index < m_characters.size())
	{
		beginResetModel();
		m_characters.removeAt(index);
		m_fonts.removeAt(index);
		endResetModel();
		return true;
	}
//	qDebug("CharTable::deleteOwnCharacter: no char deleted - logical error probably");
	return false;
}

Qt::ItemFlags CharTableModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
	if (index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
	return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions CharTableModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QStringList CharTableModel::mimeTypes() const
{
	return QStringList() << "text/plain";
}

QMimeData *CharTableModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();

	QString text = data(indexes.at(0), CharTableModel::CharTextRole).toString();
	mimeData->setText(text);

	QString textAndFont = data(indexes.at(0), CharTableModel::CharTextAndFontRole).toString();
	mimeData->setData("application/x-scribus-charpaltext", textAndFont.toUtf8());

	return mimeData;
}

bool CharTableModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	bool success = false;
	if (data->hasFormat("application/x-scribus-charpaltext"))
	{
		QByteArray textAndFont = data->data("application/x-scribus-charpaltext");
		appendUnicode(QString::fromUtf8(textAndFont), 10);
		success = true;
	}
	else if (data->hasText())
	{
		QChar textChar = data->text().at(0);
		appendUnicode(QString("%1").arg(textChar.unicode(), 10));
		success = true;
	}
		
	return success;
}
