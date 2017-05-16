#ifndef MARKS_H
#define MARKS_H

#include <QString>
#include "scribusapi.h"
#include "desaxe/saxiohelper.h"
#include "desaxe/simple_actions.h"

class Mark;
class PageItem;
class TextNote;

enum MarkType
{
	MARKNoType = -1, //undefinied mark type (wrong situation!)
	MARKAnchorType = 0, //empty anchor mark, useful for creating references to it
	MARK2ItemType = 1, //reference to page item ID
	MARK2MarkType = 2, //reference other mark
	MARKVariableTextType = 3,//mark contain dynamic text
	MARKNoteMasterType = 4,  //mark contain footnote reference
	MARKNoteFrameType = 5,  //mark used internally in note frame at beginning of note`s text
	MARKIndexType = 6, // index entry
	MARKBullNumType = 7
};

struct MarkData
{
	QString strtxt;
	PageItem* itemPtr;
	QString destmarkName;
	MarkType destmarkType;
	TextNote* notePtr;
	//fields used for resolving to pointers for load and copy
	QString itemName;
	MarkType markTyp;
	
	MarkData() : itemPtr(NULL), destmarkType(MARKNoType), notePtr(NULL), markTyp(MARKNoType) {}
};

class SCRIBUS_API Mark
{
	friend class ScribusDoc;
	friend class ScribusMainWindow;
	friend class BulNumMark;
	//only ScribusDoc && ScribusMainWindow can create and delete marks
private:
	Mark() : OwnPage(-1), typ(MARKNoType), data() {}
	Mark(const Mark& other);

public:
	QString label;
	int OwnPage;

	void setValues(const QString& l, int p, MarkType t, MarkData d);
	MarkType getType() { return typ; }
	void setType(MarkType t) { typ = t; }
	const MarkData getData() { return data; }
	void setData(const MarkData d) { data = d; }
	PageItem* getItemPtr() { return data.itemPtr; }
	void setItemPtr( PageItem* ptr ) { data.itemPtr = ptr; }
	const QString getItemName() { return data.itemName; }
	void setItemName( const QString name ) { data.itemName = name; }

	//for marks to marks - return label and type of target mark by reference
	void getMark(QString& l, MarkType &t);
	//for marks to marks - set label and type of target mark from mark pointer
	void setMark(Mark* mP)
	{
		if (mP == NULL)
		{
			data.destmarkName = "";
			data.destmarkType = MARKNoType;
		}
		else
		{
			data.destmarkName = mP->label;
			data.destmarkType = mP->getType();
		}
	}
	void setMark(const QString& l, MarkType t);
	MarkType getMarkType() { return data.markTyp; }
	void setMarkType(MarkType t) { data.markTyp = t; }
	const QString getString();
	void setString( const QString str );
	TextNote* getNotePtr() { return data.notePtr; }
	void setNotePtr(TextNote *note);

	bool hasItemPtr();
	bool hasString();
	bool hasMark();
	bool isUnique();
	bool isNoteType();
	bool isType(const MarkType t);

    virtual ~Mark() {}

protected:
	MarkType typ;
	MarkData data;
};

class SCRIBUS_API BulNumMark : public Mark
{
public:
	BulNumMark();
	~BulNumMark() {}
};

#endif // MARKS_H

