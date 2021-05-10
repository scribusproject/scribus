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
	MARKNoType = -1, //undefined mark type (wrong situation!)
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
	MarkData() {}

	QString   text;
	QString   itemName;
	PageItem* itemPtr { nullptr };
	TextNote* notePtr { nullptr };
	QString   destMarkName;
	MarkType  destMarkType {MARKNoType};
};

class SCRIBUS_API Mark
{
	friend class ScribusDoc;
	friend class ScribusMainWindow;
	friend class BulNumMark;

	//only ScribusDoc && ScribusMainWindow can create and delete marks
private:
	Mark() : m_data() {}
	Mark(const Mark& other);

public:
    virtual ~Mark() {}

	QString label;
	int OwnPage {-1};

	MarkType getType() const { return m_type; }
	void setType(MarkType t) { m_type = t; }

	const MarkData& getData() const { return m_data; }
	void setData(const MarkData& d) { m_data = d; }

	PageItem* getItemPtr() const { return m_data.itemPtr; }
	void setItemPtr( PageItem* ptr ) { m_data.itemPtr = ptr; }

	const QString getItemName() const { return m_data.itemName; }
	void setItemName( const QString name) { m_data.itemName = name; }

	const QString& getString() const { return m_data.text; }
	void setString(const QString& str) { m_data.text = str; }

	TextNote* getNotePtr() const { return m_data.notePtr; }
	void setNotePtr(TextNote *note);

	void setValues(const QString& l, int p, MarkType t, const MarkData& d);

	//for marks to marks - return label and type of target mark by reference
	void getDestMark(QString& l, MarkType &t) const;
	const QString& getDestMarkName() const { return m_data.destMarkName; }
	MarkType getDestMarkType() const { return m_data.destMarkType; }
	//for marks to marks - set label and type of target mark from mark pointer
	void setDestMark(Mark* mP);
	void setDestMark(const QString& l, MarkType t);

	bool hasItemPtr() const;
	bool hasString() const;
	bool hasMark() const;
	bool isUnique() const;
	bool isNoteType() const;
	bool isType(const MarkType t) const;

	void clearString();

protected:
	MarkType m_type {MARKNoType};
	MarkData m_data;
};

class SCRIBUS_API BulNumMark : public Mark
{
public:
	BulNumMark();
	~BulNumMark() {}
};

#endif // MARKS_H

