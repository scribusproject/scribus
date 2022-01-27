/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef DOCPREFERENCE_H
#define DOCPREFERENCE_H

#include <QObject>
#include <QDomNode>
#include <QDomElement>
#include <QString>

struct docPrefer
{
	QString self;
	QString PageHeight;
	QString PageWidth;
	QString DocumentBleedTopOffset;
	QString DocBleedBotOffset;
	QString DocBleedLeftOffset;
	QString DocBleedRightOffset;
	QString ColumnDirection;
	QString MasterTextFrame;
};

struct marginPrefer
{
	QString top;
	QString bottom;
	QString right;
	QString left;
};

struct PageItemDefault
{
	QString FillColor;
	QString FillTint;
	QString StrokeWeight;
	QString StrokeColor;
	QString StrokeTint;
	QString GradFillAngle;
	QString GradStrokeAngle;
};

struct StoryPrefer
{
	QString OpticalMarginSize;
	QString StoryDirection;
	QString StoryOrientation;
};

struct TextDefault
{
	QString Self;
	QString FirstLineIndent;
	QString LeftIndent;
	QString RightIndent;
	QString SpaceBefore;
	QString SpaceAfter;
	QString Justification;
	QString MinWordSpacing;
	QString MaxWordSpacing;
	QString DesWordSpacing;
	QString MinGlyphScaling;
	QString MaxGlyphScaling;
	QString AppliedPStyle;
	QString AppliedCStyle;
	QString FontStyle;
	QString PointSize;
	QString Capitalization;
	QString Underline;
	QString HorizontalScale;
	QString VerticalScale;
	QString BaselineShift;
	QString FillTint;
	QString StrokeTint;
	QString StrokeWeight;
	QString UnderlineOffset;
	QString UnderlineWeight;
	QString StrikeThruOffset;
	QString StrikeThruWeight;
	QString FillColor;
	QString StrokeColor;
	QString AppliedLanguage;
	QString BasedOn;
	QString AppliedFont;
};



/*!
\author Jain Basil Aliyas
\class DocPreferences
\brief A class to represent the necessary preferences from Resources/Preferences.xml in the idml package. These values will be used as default if the attribute value returned is null. Hence, every element in the above defined structures are QString type, which will be converted to their natives types while assigning it to the Scribus native objects!.
*/

class DocPreferences : public QObject
{
  Q_OBJECT
  public:
	
	DocPreferences();
	~DocPreferences();
	bool processFile(QDomElement& root);
	docPrefer preference;
	marginPrefer marginPreference;
	PageItemDefault pageDefault;
	StoryPrefer storyPreference;
	TextDefault textDefault;
};
#endif