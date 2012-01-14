/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "docpreference.h"

/*!

\param root is the root node of the preferences.xml document
\return true if success
*/


DocPreferences::DocPreferences() : QObject(0)
{
}

bool DocPreferences::processFile(QDomElement& root)
{
	if( root.tagName() != "idPkg:Preferences" )
		return false;
	else
	{
		QDomNode node = root.firstChild();
		while(!node.isNull())
		{
			QDomElement elem = node.toElement();
			if(!elem.isNull())
			{
				if( elem.tagName() == "StoryPreference" )
				{
					storyPreference.OpticalMarginSize = elem.attribute("OpticalMarginSize","0");
					storyPreference.StoryDirection = elem.attribute("StoryDirection","");
					storyPreference.StoryOrientation = elem.attribute("StoryOrientation","");
				}
				if(elem.tagName() == "TextDefault" )
				{
					textDefault.Self = elem.attribute("Self","");
					textDefault.AppliedLanguage = elem.attribute("AppliedLanguage","");
					textDefault.AppliedCStyle = elem.attribute("AppliedCharacterStyle","");
					textDefault.AppliedPStyle = elem.attribute("AppliedParagraphStyle","");
					textDefault.BaselineShift = elem.attribute("BaselineShift","");
					textDefault.Capitalization = elem.attribute("Capitalization","");
					textDefault.DesWordSpacing = elem.attribute("DesiredWordSpacing","");
					textDefault.FillColor = elem.attribute("FillColor","");
					textDefault.FillTint = elem.attribute("FillTint","");
					textDefault.FirstLineIndent = elem.attribute("FirstLineIndent","");
					textDefault.FontStyle = elem.attribute("FontStyle","");
					textDefault.HorizontalScale = elem.attribute("HorizontalScale","");
					textDefault.Justification = elem.attribute("Justification","");
					textDefault.LeftIndent = elem.attribute("LeftIndent","");
					textDefault.MaxGlyphScaling = elem.attribute("MaximumGlyphSpacing","");
					textDefault.MaxWordSpacing = elem.attribute("MaximumWordSpacing","");
					textDefault.MinGlyphScaling = elem.attribute("MinimumGlyphScaling","");
					textDefault.MinWordSpacing = elem.attribute("MinimumWordSpacing","");
					textDefault.PointSize = elem.attribute("PointSize","");
					textDefault.RightIndent = elem.attribute("RightIndent","");
					textDefault.SpaceAfter = elem.attribute("SpaceAfter","");
					textDefault.SpaceBefore = elem.attribute("SpaceBefore","");
					textDefault.StrikeThruOffset = elem.attribute("StrikeThroughOffset","");
					textDefault.StrikeThruWeight = elem.attribute("StrikeThroughWeight","");
					textDefault.StrokeColor = elem.attribute("StrokeColor","");
					textDefault.StrokeTint = elem.attribute("StrokeTint","");
					textDefault.StrokeWeight = elem.attribute("StrokeWeight","");
					textDefault.Underline = elem.attribute("Underline","");
					textDefault.UnderlineOffset = elem.attribute("UnderlineOffset","");
					textDefault.UnderlineWeight = elem.attribute("UnderlineWeight","");
					textDefault.VerticalScale = elem.attribute("VerticalScale","");
				}
				if( elem.tagName() == "DocumentPreference")
				{
					preference.ColumnDirection = elem.attribute("ColumnDirection","");
					preference.DocBleedBotOffset = elem.attribute("DocumentBleedBottomOffset","0");
					preference.DocBleedLeftOffset = elem.attribute("DocumentBleedLeftOffset","0");
					preference.DocBleedRightOffset = elem.attribute("DocumentBleedRightOffset","0");
					preference.DocumentBleedTopOffset = elem.attribute("DocumentBleedTopOffset","0");
					preference.MasterTextFrame = elem.attribute("MasterTextFrame","");
					preference.PageHeight = elem.attribute("PageHeight","0");
					preference.PageWidth = elem.attribute("PageWidth","0");
					preference.self = elem.attribute("Self","");
				}
				if( elem.tagName() == "PageItemDefault")
				{
					pageDefault.FillColor = elem.attribute("FillColor","");
					pageDefault.FillTint = elem.attribute("FillTint","0");
					pageDefault.GradFillAngle = elem.attribute("GradientFillAngle","0");
					pageDefault.GradStrokeAngle = elem.attribute("GradientStrokeAngle","0");
					pageDefault.StrokeColor = elem.attribute("StrokeColor","0");
					pageDefault.StrokeTint = elem.attribute("StrokeTint","0");
					pageDefault.StrokeWeight = elem.attribute("StrokeWeight","0");
				}
				if( elem.tagName() == "MarginPreference")
				{
					marginPreference.bottom = elem.attribute("Bottom","0");
					marginPreference.left = elem.attribute("Left","0");
					marginPreference.right = elem.attribute("Right","0");
					marginPreference.top = elem.attribute("Top","0");
				}
			}
			node=node.nextSibling();
		}
	}
	return true;
}


DocPreferences::~DocPreferences()
{
}
