#ifndef OPTICALMARGINLOOKUP_H
#define OPTICALMARGINLOOKUP_H

#include <QMap>
#include <QObject>

#include "scribusapi.h"
#include "units.h"
#include "util_math.h"

struct OpticalMarginRule
{
	double Left { 0.5 };
	double Right { 0.5 };
	int Unit { SC_PERCENT };
	QString Chars;

	OpticalMarginRule(const QString &chars, double left, double right, int unit)
	{
		Left = left;
		Right = right;
		Unit = unit;
		Chars = chars;
	}

	void appendChars(const QString &chars) { Chars += chars; }
	bool equalChars(const QString &chars) const
	{
		return (Chars == chars);
	}

	bool operator==(const OpticalMarginRule& other) const
	{
		if (!compareDouble(Left, other.Left) || !compareDouble(Right, other.Right) || (Unit != other.Unit))
			return false;
		return true;
	}
	bool operator!=(const OpticalMarginRule& other) const
	{
		return !(*this == other);
	}
	bool operator<(const OpticalMarginRule& other) const
	{
		return (QPair<double, double>(Left, Right) < QPair<double, double>(other.Left, other.Right));
	}
};

using OpticalMarginRules = QList<OpticalMarginRule>;
using OpticalMarginChars = QMap<QChar, OpticalMarginRule>;

struct OpticalMarginSet
{
	QString id;
	QString type {"undefined"};
	QString name;
	OpticalMarginChars rules;
};

using OpticalMarginSets = QMap<QString, OpticalMarginSet>;

class SCRIBUS_API OpticalMarginLookup : public QObject
{
	Q_OBJECT

public:
	OpticalMarginLookup(OpticalMarginLookup const&) = delete;
	void operator=(OpticalMarginLookup const&) = delete;

	static OpticalMarginLookup& instance();

	OpticalMarginRule offset(const QChar &c, const OpticalMarginSet& set);

	const OpticalMarginRules mergeCharsToRules(const OpticalMarginChars &chars);
	const OpticalMarginChars splitRulesToChars(const OpticalMarginRules &groups);

	const OpticalMarginSets& defaultSets() const { return m_defaultSets; };
	QString defaultSetId() const;

	QString nextAvailableUserId(const OpticalMarginSets& sets);

	QMap<QString,QString> trNames(const OpticalMarginSets& sets) const;
	QString trName(const OpticalMarginSets& sets, const QString& id) const;
	bool hasTrName(const OpticalMarginSets& sets, const QString& trName) const;

private:
	OpticalMarginLookup(QObject *parent = nullptr);
	~OpticalMarginLookup() = default;

	OpticalMarginSets m_defaultSets;

	void buildDefaults();

	//void writeDocument();

};


#endif // OPTICALMARGINLOOKUP_H
