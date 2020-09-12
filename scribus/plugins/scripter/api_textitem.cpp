/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.'
*/
#include "api_textitem.h"
#include "units.h"
#include "scribusdoc.h"
#include "selection.h"
#include "scribusview.h"
#include "appmodes.h"
#include "utils.h"
#include "hyphenator.h"
#include "scripterimpl.h"

template<typename T>
class ApplyCharstyleHelper {
	PageItem* item;
	T value;
public:
	ApplyCharstyleHelper(PageItem* i, T v) : item(i), value(v) {}

	void apply(void (CharStyle::*f)(T), int p, int len)
	{
		CharStyle cs;
		(cs.*f)(value);
		if (item->HasSel)
		{
			int max = qMax(p+len, item->itemText.length());
			for (int b = p; b < max; b++)
			{
				if (item->itemText.selected(b))
					item->itemText.applyCharStyle(b, 1, cs);
			}
		}
		else
		{
			item->itemText.applyCharStyle(p, len, cs);
		}
	}

};

TextAPI::TextAPI(PageItem_TextFrame* inner) : ItemAPI(inner)
{
	qDebug() << "TextItemWrapper loaded";
	setObjectName("textItem");
	item = inner;
}

QString TextAPI::font()
{
	if (item->HasSel)
	{
		for (int b = 0; b < item->itemText.length(); b++)
			if (item->itemText.selected(b))
				return item->itemText.charStyle(b).font().scName();
		return nullptr;
	}
	else
		return item->currentCharStyle().font().scName();
}

void TextAPI::setFont(QString name)
{
	if (PrefsManager::instance()->appPrefs.fontPrefs.AvailFonts.contains(name))
	{
		RAISE("Font not found");
		return;
	}
	
	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetFont(name);
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
}

double TextAPI::fontSize()
{
	if (item->HasSel)
	{
		for (int b = 0; b < item->itemText.length(); b++)
			if (item->itemText.selected(b))
				return item->itemText.charStyle(b).fontSize() / 10.0;
		return nullptr;
	}
	else
	{
		return item->currentCharStyle().fontSize() / 10.0;
	}
}

void TextAPI::setFontSize(double size)
{
	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetFontSize(qRound(size * 10.0));
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
}

void TextAPI::setText(QString text)
{
	text.replace("\r\n", SpecialChars::PARSEP);
	text.replace(QChar('\n') , SpecialChars::PARSEP);
	item->itemText.clear();
//	for (int a = 0; a < ScCore->primaryMainWindow()->doc->FrameItems.count(); ++a)
//	{
//		ScCore->primaryMainWindow()->doc->FrameItems[a]->ItemNr = a;
//	} TODO fix this :FrameItems has been changed to QHash from QList
	qDebug()<<"text : "<<text;
	item->itemText.insertChars(0, text);
	item->invalidateLayout();
	item->Dirty = false;
	ScCore->primaryMainWindow()->view->DrawNew();
}

QString TextAPI::text()
{
	QString text = "";
	for (int a = 0; a < item->itemText.length(); a++)
	{
		if (item->HasSel)
		{
			if (item->itemText.selected(a))
				text += item->itemText.text(a);
		}
		else
		{
			text += item->itemText.text(a);
		}
	} // for
	return text;
}

int TextAPI::textLines()
{
	return item->textLayout.lines();
}

int TextAPI::textLength()
{
	return item->itemText.length();
}

double TextAPI::lineSpacing()
{
	return item->currentStyle().lineSpacing();
}

void TextAPI::setLineSpacing(double value)
{
	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetLineSpacing(value);
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
}

QList<QVariant> TextAPI::distances()
{
	QList<QVariant> l;
	l.append(pts2value(item->textToFrameDistLeft(), ScCore->primaryMainWindow()->doc->unitIndex()));
	l.append(pts2value(item->textToFrameDistRight(), ScCore->primaryMainWindow()->doc->unitIndex()));
	l.append(pts2value(item->textToFrameDistTop(), ScCore->primaryMainWindow()->doc->unitIndex()));
	l.append(pts2value(item->textToFrameDistBottom(), ScCore->primaryMainWindow()->doc->unitIndex()));
	return l;
}

void TextAPI::insertText(QString text, int position)
{
	text.replace("\r\n", SpecialChars::PARSEP);
	text.replace(QChar('\n') , SpecialChars::PARSEP);
	if ((position < -1) || (position > static_cast<int>(item->itemText.length())))
	{
		RAISE("Value of position out of bound.");
		return;
	}
	if (position == -1)
		position = item->itemText.length();
	item->itemText.insertChars(position, text);
	item->Dirty = true;
	if (ScCore->primaryMainWindow()->doc->DoDrawing)
	{
		// FIXME adapt to Qt-4 painting style
		item->Dirty = false;
	}
	ScCore->primaryMainWindow()->view->DrawNew();
}

void TextAPI::setLineSpacingMode(int mode)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (mode < 0 || mode > 3) // Use constants?
	{
		RAISE("Line space mode invalid, must be 0, 1 or 2");
	}

	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetLineSpacingMode(mode);
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
}

void TextAPI::setDistances(double left, double right, double top, double bottom)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (left < 0.0 || right < 0.0 || top < 0.0 || bottom < 0.0)
	{
		RAISE("Text distances out of bounds, must be positive.");
	}

	item->setTextToFrameDist(ValueToPoint(left), ValueToPoint(right), ValueToPoint(top), ValueToPoint(bottom));
}

void TextAPI::setTextAlignment(int alignment)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if ((alignment > 4) || (alignment < 0))
	{
		RAISE("Alignment out of range. Should be between 0 and 4");
	}
	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->setNewAlignment(alignment);
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();
}

void TextAPI::setTextColor(QString color)
{
	if (!checkHaveDocument())
		RAISE("No document open");

	ApplyCharstyleHelper<QString>(item, color).apply(&CharStyle::setFillColor, 0, item->itemText.length());

//	for (int b = 0; b < item->itemText.length(); b++)
//	{
//		//FIXME: doc method
//		if (item->HasSel)
//		{
//			if (item->itemText.selected(b))
//				item->itemText.item(b)->setFillColor(color);
//		}
//		else
//			item->itemText.item(b)->setFillColor(color);
//	}
}

void TextAPI::setTextStroke(QString color)
{
	if (!checkHaveDocument())
		RAISE("No document open");

	ApplyCharstyleHelper<QString>(item, color).apply(&CharStyle::setStrokeColor, 0, item->itemText.length());

//	for (int b = 0; b < item->itemText.length(); b++)
//	{
//		//FIXME:NLS use document method for item
//		if (item->HasSel)
//		{
//			if (item->itemText.selected(b))
//				item->itemText.item(b)->setStrokeColor(color);
//		}
//		else
//			item->itemText.item(b)->setStrokeColor(color);
//	}
}

void TextAPI::setTextScalingV(double value)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (value < 10)
	{
		RAISE("Character scaling out of bounds, must be >= 10");
	}

	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetScaleV(qRound(value * 10));
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();

}

void TextAPI::setTextScalingH(double value)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (value < 10)
	{
		RAISE("Character scaling out of bounds, must be >= 10");
	}

	int Apm = ScCore->primaryMainWindow()->doc->appMode;
	ScCore->primaryMainWindow()->doc->m_Selection->clear();
	ScCore->primaryMainWindow()->doc->m_Selection->addItem(item);
	if (item->HasSel)
		ScCore->primaryMainWindow()->doc->appMode = modeEdit;
	ScCore->primaryMainWindow()->doc->itemSelection_SetScaleH(qRound(value * 10));
	ScCore->primaryMainWindow()->doc->appMode = Apm;
	ScCore->primaryMainWindow()->view->Deselect();

}

void TextAPI::setTextShade(int w)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if ((w < 0) || (w > 100))
	{
		RAISE("value out of bound. Should be between 0 and 100");
	}
	//FIXME:NLS use document method for that

	ApplyCharstyleHelper<double>(item, w).apply(&CharStyle::setFillShade, 0, item->itemText.length());

//	for (int b = 0; b < item->itemText.length(); ++b)
//	{
//		if (item->HasSel)
//		{
//			if (item->itemText.selected(b))
//				item->itemText.item(b)->setFillShade(w);
//		}
//		else
//			item->itemText.item(b)->setFillShade(w);
//	}
}

void TextAPI::selectText(int start, int selcount)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	
	if (selcount == -1)
	{
		// user wants to select all after the start point
		selcount = item->itemText.length() - start;
		if (selcount < 0)
			// user passed start that's > text in the frame
			selcount = 0;
	}
	if ((start < 0) || ((start + selcount) > static_cast<int>(item->itemText.length())))
	{
		RAISE("Selection index out of bounds");
	}

	item->itemText.deselectAll();
	if (selcount == 0)
	{
		item->HasSel = false;
		return;
	}
	item->itemText.select(start, selcount, true);
	item->HasSel = true;
}

void TextAPI::linkToTextFrame(QString name2)
{
	if(name2.isEmpty())
	{
		RAISE("Destination text frame name is empty.");
	}
	if (!checkHaveDocument())
		RAISE("No document open");

	PageItem *toitem = GetUniqueItem(name2);
	if (toitem == nullptr)
		return;
	if (!(toitem->isTextFrame()))
	{
		RAISE("Can only link text frames.");
	}
	if (toitem->itemText.length() > 0)
	{
		RAISE("Target frame must be empty.");
	}
	
	if (toitem->nextInChain() != 0)
	{
		RAISE("Target frame links to another frame.");
	}
	if (toitem->prevInChain() != 0)
	{
		RAISE("Target frame is linked to by another frame.");
	}
	if (toitem->itemName() == item->itemName())
	{
		RAISE("Source and target are the same object.");
	}
	// references to the others boxes
	item->link(toitem);
	ScCore->primaryMainWindow()->view->DrawNew();
	ScCore->primaryMainWindow()->slotDocCh();
}

void TextAPI::unLinkTextFrames()
{
	if (!checkHaveDocument())
		RAISE("No document open");
	// only linked
	if (item->prevInChain() == 0)
	{
		RAISE("Object is not a linked text frame, can't unlink.");
	}
	item->prevInChain()->unlink();
	// enable 'save icon' stuff
	ScCore->primaryMainWindow()->slotDocCh();
	ScCore->primaryMainWindow()->view->DrawNew();
}

bool TextAPI::deleteText()
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item->HasSel){}
//		item->deleteSelectedTextFromFrame();
	else
	{
		item->itemText.clear();
		//for (int a = 0; a < ScCore->primaryMainWindow()->doc->FrameItems.count(); ++a)
		//{
		//	ScCore->primaryMainWindow()->doc->FrameItems.at(a)->ItemNr = a;
		//}TODO fix this,
	}
}

bool TextAPI::traceText()
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item->invalid)
		item->layout();
	ScCore->primaryMainWindow()->view->Deselect(true);
	ScCore->primaryMainWindow()->view->SelectItem(item);
	ScCore->primaryMainWindow()->view->TextToPath();
	return true;
}

int TextAPI::textOverFlows(bool checkLinks)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	/* original solution
	if (item->itemText.count() > item->MaxChars)
	return PyBool_FromLong(static_cast<long>(true));
	return PyBool_FromLong(static_cast<long>(false)); */
	/*
	 uint firstFrame = 0;
	if (nolinks)
		firstFrame = item->itemText.count();
	uint chars = item->itemText.count();
	uint maxchars = item->MaxChars;
	while (item->NextBox != 0) {
		item = item->NextBox;
		chars += item->itemText.count();
		maxchars += item->MaxChars;
	}
	// no overrun
	if (nolinks)
		return PyInt_FromLong(maxchars - firstFrame);

	if (maxchars > chars)
		return PyInt_FromLong(0);
	// number of overrunning letters
	return PyInt_FromLong(static_cast<long>(chars - maxchars));
	 */
	// refresh overflow information
	item->invalidateLayout();
	item->layout();
	return item->frameOverflows();
}

bool TextAPI::hyphenate()
{
	if (!checkHaveDocument())
		RAISE("No document open");
	ScCore->primaryMainWindow()->doc->docHyphenator->slotHyphenate(item);
	return true;
}

bool TextAPI::dehyphenate()
{
	if (!checkHaveDocument())
		RAISE("No document open");
	ScCore->primaryMainWindow()->doc->docHyphenator->slotDeHyphenate(item);
	return false;
}

bool TextAPI::PDFBookMark()
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item->isBookmark)
		return true;
	return false;
}

void TextAPI::setPDFBookMark(bool toggle)
{
	if (!checkHaveDocument())
		RAISE("No document open");
	if (item->isBookmark == toggle)
	{
		return;
	}
	if (toggle)
	{
		item->setIsAnnotation(false);
		ScCore->primaryMainWindow()->AddBookMark(item);
	}
	else
		ScCore->primaryMainWindow()->DelBookMark(item);
	item->isBookmark = toggle;
}

TextAPI::~TextAPI()
{
	qDebug() << "TextItemWrapper deleted";
}


