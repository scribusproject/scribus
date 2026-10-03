/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "inddinspection.h"

#include "inddmetadata.h"

#include <QSet>
#include <QVector>
#include <QXmlStreamReader>

namespace Indd
{
namespace
{
constexpr int maxDepth = 64;
constexpr int maxPageCount = 1000000;
constexpr int maxFonts = 512;
constexpr int maxIngredients = 2048;
constexpr int maxValueLength = 4096;

struct Element
{
	QString uri;
	QString name;

	bool is(const char* expectedUri, const char* expectedName) const
	{
		return uri == QLatin1String(expectedUri) && name == QLatin1String(expectedName);
	}
};

enum class Field { None, PageCount, FontName, IngredientUri };

bool parsePageCount(const QString& text, int& pageCount)
{
	bool ok = false;
	const int value = text.trimmed().toInt(&ok);
	if (!ok || value < 1 || value > maxPageCount || (pageCount != 0 && pageCount != value))
		return false;
	pageCount = value;
	return true;
}
} // namespace

InspectionResult inspectDocument(const QString& filePath)
{
	InspectionResult result;
	const XmpPacketResult xmp = readXmpPacket(filePath);
	if (!xmp.found())
	{
		if (xmp.status == MetadataStatus::NoXmp)
			result.status = InspectionStatus::NoXmp;
		else if (xmp.status == MetadataStatus::IoError)
			result.status = InspectionStatus::IoError;
		else
			result.status = InspectionStatus::InvalidDocument;
		return result;
	}

	QXmlStreamReader xml(xmp.packet);
	QVector<Element> stack;
	QSet<QString> seenFonts;
	QSet<QString> seenIngredients;
	int rootRdfDepth = 0;
	int descriptionDepth = 0;
	int fontsDepth = 0;
	int ingredientsDepth = 0;
	int fieldDepth = 0;
	Field field = Field::None;
	QString fieldText;

	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isDTD() || xml.isEntityReference())
			return { InspectionStatus::MalformedXmp };
		if (xml.isStartElement())
		{
			if (stack.size() >= maxDepth)
				return { InspectionStatus::LimitExceeded };
			const bool topLevelDescription = rootRdfDepth > 0 && stack.size() == rootRdfDepth &&
				stack.last().is("http://www.w3.org/1999/02/22-rdf-syntax-ns#", "RDF") &&
				xml.namespaceUri() == QLatin1String("http://www.w3.org/1999/02/22-rdf-syntax-ns#") &&
				xml.name() == QLatin1String("Description");
			const bool descriptionChild = descriptionDepth > 0 && stack.size() == descriptionDepth &&
				stack.last().is("http://www.w3.org/1999/02/22-rdf-syntax-ns#", "Description");
			stack.append({ xml.namespaceUri().toString(), xml.name().toString() });
			const int depth = stack.size();

			if (rootRdfDepth == 0 && stack.last().is("http://www.w3.org/1999/02/22-rdf-syntax-ns#", "RDF"))
				rootRdfDepth = depth;
			if (topLevelDescription)
			{
				descriptionDepth = depth;
				for (const auto& attribute : xml.attributes())
				{
					if (attribute.namespaceUri() == QLatin1String("http://ns.adobe.com/xap/1.0/t/pg/") &&
						attribute.name() == QLatin1String("NPages") &&
						!parsePageCount(attribute.value().toString(), result.pageCount))
						return { InspectionStatus::InvalidValue };
				}
			}
			if (descriptionChild && stack.last().is("http://ns.adobe.com/xap/1.0/t/pg/", "Fonts"))
				fontsDepth = depth;
			else if (descriptionChild && stack.last().is("http://ns.adobe.com/xap/1.0/mm/", "Ingredients"))
				ingredientsDepth = depth;
			else if (descriptionChild && stack.last().is("http://ns.adobe.com/xap/1.0/t/pg/", "NPages"))
				field = Field::PageCount;
			else if (fontsDepth > 0 && stack.last().is("http://ns.adobe.com/xap/1.0/sType/Font#", "fontName"))
				field = Field::FontName;
			else if (ingredientsDepth > 0 && stack.last().is("http://ns.adobe.com/xap/1.0/sType/ResourceRef#", "filePath"))
				field = Field::IngredientUri;
			else
				continue;

			fieldDepth = depth;
			fieldText.clear();
		}
		else if (xml.isCharacters() && field != Field::None && stack.size() == fieldDepth)
		{
			fieldText += xml.text();
			if (fieldText.size() > maxValueLength)
				return { InspectionStatus::LimitExceeded };
		}
		else if (xml.isEndElement() && !stack.isEmpty())
		{
			const int depth = stack.size();
			if (field != Field::None && fieldDepth == depth)
			{
				const QString value = fieldText.trimmed();
				if (field == Field::PageCount && !parsePageCount(value, result.pageCount))
					return { InspectionStatus::InvalidValue };
				if (field == Field::FontName && !value.isEmpty() && !seenFonts.contains(value))
				{
					if (result.fontPostScriptNames.size() >= maxFonts)
						return { InspectionStatus::LimitExceeded };
					seenFonts.insert(value);
					result.fontPostScriptNames.append(value);
				}
				if (field == Field::IngredientUri && !value.isEmpty() && !seenIngredients.contains(value))
				{
					if (result.ingredientUris.size() >= maxIngredients)
						return { InspectionStatus::LimitExceeded };
					seenIngredients.insert(value);
					result.ingredientUris.append(value);
				}
				field = Field::None;
				fieldDepth = 0;
			}
			if (depth == fontsDepth)
				fontsDepth = 0;
			if (depth == ingredientsDepth)
				ingredientsDepth = 0;
			if (depth == descriptionDepth)
				descriptionDepth = 0;
			stack.removeLast();
		}
	}
	result.status = xml.hasError() || rootRdfDepth == 0 ? InspectionStatus::MalformedXmp : InspectionStatus::Found;
	if (!result.found())
	{
		result.pageCount = 0;
		result.fontPostScriptNames.clear();
		result.ingredientUris.clear();
	}
	return result;
}

} // namespace Indd
