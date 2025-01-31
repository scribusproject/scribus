#include "opticalmarginlookup.h"

#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QString>

//#include "util_text.h"

OpticalMarginLookup::OpticalMarginLookup(QObject *parent) : QObject(parent)
{
	buildDefaults();
}

void OpticalMarginLookup::buildDefaults()
{
	// https://en.wikipedia.org/wiki/Optical_margin_alignment
	// https://symbl.cc/en/unicode-table/

	// Please define offset values in percent or pt only.
	// Use value2pts(double unitValue, int unit) to convert other units into pt.

	/********************
	 *  Latin Sans Serif
	 ********************/
	OpticalMarginChars latinSansSerifRules;
	latinSansSerifRules.insert(QChar(0x0022), OpticalMarginRule(QChar(0x0022), 0.5, 0.5, SC_PERCENT));// Quotation Mark "
	latinSansSerifRules.insert(QChar(0x0027), OpticalMarginRule(QChar(0x0027), 0.75, 0.75, SC_PERCENT)); // Apostrophe '
	latinSansSerifRules.insert(QChar(0x0028), OpticalMarginRule(QChar(0x0028), 0.5, 0.5, SC_PERCENT)); // Left Parenthesis (
	latinSansSerifRules.insert(QChar(0x0029), OpticalMarginRule(QChar(0x0029), 0.5, 0.5, SC_PERCENT)); // Right Parenthesis )
	latinSansSerifRules.insert(QChar(0x002a), OpticalMarginRule(QChar(0x002a), 0.75, 0.75, SC_PERCENT)); // Asterisk *
	latinSansSerifRules.insert(QChar(0x002c), OpticalMarginRule(QChar(0x002c), 0.0, 0.75, SC_PERCENT)); // Comma ,
	latinSansSerifRules.insert(QChar(0x002e), OpticalMarginRule(QChar(0x002e), 0.0, 0.75, SC_PERCENT)); // Full Stop .
	latinSansSerifRules.insert(QChar(0x002d), OpticalMarginRule(QChar(0x002d), 0.0, 0.75, SC_PERCENT)); // Hyphen-Minus -
	latinSansSerifRules.insert(QChar(0x003a), OpticalMarginRule(QChar(0x003a), 0.0, 0.5, SC_PERCENT)); // Colon :
	latinSansSerifRules.insert(QChar(0x003b), OpticalMarginRule(QChar(0x003b), 0.0, 0.5, SC_PERCENT)); // Semicolon ;
	latinSansSerifRules.insert(QChar(0x005b), OpticalMarginRule(QChar(0x005b), 0.5, 0.5, SC_PERCENT)); // Left Square Bracket [
	latinSansSerifRules.insert(QChar(0x005d), OpticalMarginRule(QChar(0x005d), 0.5, 0.5, SC_PERCENT)); // Right Square Bracket ]
	latinSansSerifRules.insert(QChar(0x007e), OpticalMarginRule(QChar(0x007e), 0.75, 0.75, SC_PERCENT)); // Tilde ~
	latinSansSerifRules.insert(QChar(0x00B4), OpticalMarginRule(QChar(0x00B4), 0.75, 0.75, SC_PERCENT)); // Acute Accent
	latinSansSerifRules.insert(QChar(0x0060), OpticalMarginRule(QChar(0x0060), 0.75, 0.75, SC_PERCENT)); // Gravis
	latinSansSerifRules.insert(QChar(0x00ab), OpticalMarginRule(QChar(0x00ab), 0.5, 0.5, SC_PERCENT)); // Left-Pointing Double Angle Quotation Mark <<
	latinSansSerifRules.insert(QChar(0x00bb), OpticalMarginRule(QChar(0x00bb), 0.5, 0.5, SC_PERCENT)); // Right-Pointing Double Angle Quotation Mark >>
	latinSansSerifRules.insert(QChar(0x2010), OpticalMarginRule(QChar(0x2010), 0.0, 0.75, SC_PERCENT)); // Hyphen
	latinSansSerifRules.insert(QChar(0x2013), OpticalMarginRule(QChar(0x2013), 0.0, 0.5, SC_PERCENT)); // En Dash
	latinSansSerifRules.insert(QChar(0x2014), OpticalMarginRule(QChar(0x2014), 0.25, 0.25, SC_PERCENT)); // Em Dash
	latinSansSerifRules.insert(QChar(0x2018), OpticalMarginRule(QChar(0x2018), 0.75, 0.75, SC_PERCENT)); // Left Single Quotation Mark
	latinSansSerifRules.insert(QChar(0x2019), OpticalMarginRule(QChar(0x2019), 0.75, 0.75, SC_PERCENT)); // Right Single Quotation Mark
	latinSansSerifRules.insert(QChar(0x201a), OpticalMarginRule(QChar(0x201a), 0.75, 0.75, SC_PERCENT)); // Single Low-9 Quotation Mark
	latinSansSerifRules.insert(QChar(0x201b), OpticalMarginRule(QChar(0x201b), 0.75, 0.75, SC_PERCENT)); // Single High-Reversed-9 Quotation Mark
	latinSansSerifRules.insert(QChar(0x201c), OpticalMarginRule(QChar(0x201c), 0.5, 0.5, SC_PERCENT)); // Left Double Quotation Mark
	latinSansSerifRules.insert(QChar(0x201d), OpticalMarginRule(QChar(0x201d), 0.5, 0.5, SC_PERCENT)); // Right Double Quotation Mark
	latinSansSerifRules.insert(QChar(0x201e), OpticalMarginRule(QChar(0x201e), 0.5, 0.5, SC_PERCENT)); // Double Low-9 Quotation Mark
	latinSansSerifRules.insert(QChar(0x201f), OpticalMarginRule(QChar(0x201f), 0.5, 0.5, SC_PERCENT)); // Double High-Reversed-9 Quotation Mark
	latinSansSerifRules.insert(QChar(0x2032), OpticalMarginRule(QChar(0x2032), 0.0, 0.75, SC_PERCENT)); // Prime
	latinSansSerifRules.insert(QChar(0x2033), OpticalMarginRule(QChar(0x2033), 0.0, 0.5, SC_PERCENT)); // Double Prime
	latinSansSerifRules.insert(QChar(0x2035), OpticalMarginRule(QChar(0x2035), 0.75, 0.0, SC_PERCENT)); // Reversed Prime
	latinSansSerifRules.insert(QChar(0x2036), OpticalMarginRule(QChar(0x2036), 0.5, 0.0, SC_PERCENT)); // Reversed Double Prime
	latinSansSerifRules.insert(QChar(0x2039), OpticalMarginRule(QChar(0x2039), 0.75, 0.75, SC_PERCENT)); // Single Left-Pointing Angle Quotation Mark <
	latinSansSerifRules.insert(QChar(0x203a), OpticalMarginRule(QChar(0x203a), 0.75, 0.75, SC_PERCENT)); // Single Right-Pointing Angle Quotation Mark >

	OpticalMarginSet latinSansSerifSet;
	latinSansSerifSet.id = "preset_0";
	latinSansSerifSet.type = "preset";
	latinSansSerifSet.name = tr("Default");
	latinSansSerifSet.rules = latinSansSerifRules;

	m_defaultSets.insert("preset_0", latinSansSerifSet);

	/********************
	 *  Latin Serif
	 ********************/
	// OpticalMarginChars latinSerifRules = latinSansSerifRules;
	// latinSerifRules.insert(QChar(0x0041), OpticalMarginRule(QChar(0x0041), 0.2, 0.2, SC_PERCENT)); // A
	// latinSerifRules.insert(QChar(0x0043), OpticalMarginRule(QChar(0x0043), 0.1, 0.0, SC_PERCENT)); // C
	// latinSerifRules.insert(QChar(0x0044), OpticalMarginRule(QChar(0x0044), 0.0, 0.1, SC_PERCENT)); // D
	// latinSerifRules.insert(QChar(0x0047), OpticalMarginRule(QChar(0x0047), 0.1, 0.0, SC_PERCENT)); // G
	// latinSerifRules.insert(QChar(0x004F), OpticalMarginRule(QChar(0x004F), 0.1, 0.1, SC_PERCENT)); // O
	// latinSerifRules.insert(QChar(0x0054), OpticalMarginRule(QChar(0x0054), 0.2, 0.2, SC_PERCENT)); // T
	// latinSerifRules.insert(QChar(0x0056), OpticalMarginRule(QChar(0x0056), 0.2, 0.2, SC_PERCENT)); // V
	// latinSerifRules.insert(QChar(0x0057), OpticalMarginRule(QChar(0x0057), 0.2, 0.2, SC_PERCENT)); // W
	// latinSerifRules.insert(QChar(0x0059), OpticalMarginRule(QChar(0x0059), 0.2, 0.2, SC_PERCENT)); // Y

	// OpticalMarginSet latinSerifSet;
	// latinSerifSet.id = "preset_1";
	// latinSerifSet.type = "preset";
	// latinSerifSet.name = tr("Latin Serif");
	// latinSerifSet.rules = latinSerifRules;

	// m_defaultSets.insert("preset_1", latinSerifSet);


	/********************
	 *  Asian Mono
	 ********************/
	// OpticalMarginChars asianRules;
	// asianRules.insert(QChar(0x3002), OpticalMarginRule(QChar(0x3002), 0.0, 0.5, SC_PERCENT)); // Ideographic Full Stop
	// asianRules.insert(QChar(0xff0c), OpticalMarginRule(QChar(0xff0c), 0.5, 0.5, SC_PERCENT)); // Fullwidth Comma
	// asianRules.insert(QChar(0xff1a), OpticalMarginRule(QChar(0xff1a), 0.5, 0.5, SC_PERCENT)); // Fullwidth Colon
	// asianRules.insert(QChar(0xff1b), OpticalMarginRule(QChar(0xff1b), 0.5, 0.5, SC_PERCENT)); // Fullwidth Semicolon

	// OpticalMarginSet asianSet;
	// asianSet.id = "preset_2";
	// asianSet.type = "preset";
	// asianSet.name = tr("Asian Mono");
	// asianSet.rules = asianRules;

	// m_defaultSets.insert("preset_2", asianSet);
}

// void OpticalMarginLookup::writeDocument()
// {
// 	QFile file("opticalmargins.xml");
// 	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
// 	{
// 		qWarning() << "Already opened or there is another issue";
// 		file.close();
// 	}

// 	QTextStream xmlContent(&file);
// 	QDomDocument doc;
// 	doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

// 	QDomElement nodeAuthor = doc.createElement("author");
// 	nodeAuthor.appendChild(doc.createTextNode("Scribus"));

// 	QDomElement nodeLicense = doc.createElement("license");
// 	nodeLicense.appendChild(doc.createTextNode("CC0"));

// 	QDomElement nodeSets = doc.createElement("sets");

// 	int i = 0;

// 	for (auto [set, rules] : m_defaultSets.asKeyValueRange())
// 	{
// 		if (set == defaultSetId())
// 			nodeSets.setAttribute("default", QString::number(i));

// 		OpticalMarginGroups groups = mergeCharsToGroups(rules.rules);

// 		QDomElement nodeName = doc.createElement("name");
// 		nodeName.setAttribute("lang", "en_US");
// 		nodeName.appendChild(doc.createTextNode(set));

// 		QDomElement nodeNames = doc.createElement("names");
// 		nodeNames.appendChild(nodeName);

// 		QDomElement nodeRules = doc.createElement("rules");

// 		for (const auto &group : groups)
// 		{
// 			QDomElement nodeRule = doc.createElement("rule");
// 			nodeRule.setAttribute("left", group.Left);
// 			nodeRule.setAttribute("right", group.Right);
// 			nodeRule.setAttribute("unit", group.Unit);
// 			nodeRule.appendChild(doc.createTextNode(stringToUnicode(group.Chars)));

// 			nodeRules.appendChild(nodeRule);
// 		}

// 		QDomElement nodeSet = doc.createElement("set");
// 		nodeSet.setAttribute("id", QString::number(i));
// 		nodeSet.setAttribute("type", "preset");
// 		nodeSet.appendChild(nodeNames);
// 		nodeSet.appendChild(nodeRules);

// 		nodeSets.appendChild(nodeSet);
// 		i++;
// 	}


// 	QDomElement root = doc.createElement("opticalmarginsets");
// 	root.setAttribute("minVersion", "1.7.0");
// 	root.setAttribute("version", "1.7.0");
// 	root.appendChild(nodeAuthor);
// 	root.appendChild(nodeLicense);
// 	root.appendChild(nodeSets);

// 	doc.appendChild(root);

// 	xmlContent << doc.toString();
// }

const OpticalMarginRules OpticalMarginLookup::mergeCharsToRules(const OpticalMarginChars &chars)
{
	OpticalMarginRules out;

	for (auto [chr, rule] : chars.asKeyValueRange())
	{
		// check if value pair exists
		if (out.contains(rule))
		{
			int index = out.indexOf(rule);
			OpticalMarginRule tmp = out.at(index);
			tmp.appendChars(rule.Chars);
			out.replace(index, tmp);
		}
		else
			out.append(OpticalMarginRule(chr, rule.Left, rule.Right, rule.Unit));
	}

	std::sort(out.begin(), out.end());

	return out;
}

const OpticalMarginChars OpticalMarginLookup::splitRulesToChars(const OpticalMarginRules &rules)
{
	OpticalMarginChars out;

	for (const auto &rule : rules)
	{
		QString s = rule.Chars.trimmed();

		if (s.isEmpty())
			continue;

		// split string
		for (auto chr : s)
			out.insert(chr, OpticalMarginRule(chr, rule.Left, rule.Right, rule.Unit));
	}

	return out;
}

QString OpticalMarginLookup::defaultSetId() const
{
	return m_defaultSets.firstKey();
}

QString OpticalMarginLookup::nextAvailableUserId(const OpticalMarginSets& sets)
{
	int userId = 0;

	for (auto [id, set] : sets.asKeyValueRange())
	{
		if (set.type == "user")
		{
			int start = id.indexOf("_");
			int currentId = id.mid(start, id.length() - start).toInt();
			userId = currentId + 1;
		}
	}

	return QString("user_%1").arg(userId);
}

QMap<QString, QString> OpticalMarginLookup::trNames(const OpticalMarginSets& sets) const
{
	QMap<QString, QString> list;

	for (const auto& [id, set] : sets.asKeyValueRange())
		list.insert(id, set.name);

	return list;
}

QString OpticalMarginLookup::trName(const OpticalMarginSets& sets, const QString& id) const
{
	if (!sets.contains(id))
		return QString();

	OpticalMarginSet set = sets.value(id);
	return set.name;
}

bool OpticalMarginLookup::hasTrName(const OpticalMarginSets& sets, const QString& trName) const
{
	for (const auto& set : sets)
	{
		if (set.name == trName)
			return true;
	}

	return false;
}

OpticalMarginLookup& OpticalMarginLookup::instance()
{
	static OpticalMarginLookup m_instance;
	return m_instance;
}

OpticalMarginRule OpticalMarginLookup::offset(const QChar &c, const OpticalMarginSet& set)
{
	for (auto [chr, group] : set.rules.asKeyValueRange())
		if (chr == c)
			return group;

	return OpticalMarginRule("", 0.0, 0.0, SC_PERCENT);
}
