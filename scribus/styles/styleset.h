

#ifndef STYLESET_H
#define STYLESET_H

#include <assert.h>
#include "style.h"


template<class STYLE>
class StyleSet : public StyleBase {
public:
	STYLE& operator[] (uint index) { 
		assert(index < styles.count()); 
		return * styles[index]; 
	}
	
	const STYLE& operator[] (const QString& name) const { 
		return * dynamic_cast<const STYLE*>(resolve(name)); 
	}
	
	const STYLE& operator[] (uint index) const { 
		assert(index < styles.count()); 
		return * styles[index]; 
	}
	
	inline int find(const QString& name) const;

	inline const Style* resolve(const QString& name) const;
	
	uint count() const { 
		return (uint) styles.count(); 
	}
	
	STYLE* append(STYLE* style) { 
		styles.append(style); 
		style->setBase(this); 
		invalidate(); 
		return style; 
	}
	
	inline void remove(uint index);
	
	inline void redefine(const StyleSet<STYLE>& defs, bool removeUnused=false);
	
	STYLE* create(const STYLE& proto) { 
		return append(new STYLE(proto)); 
	}
	
	void makeDefault(STYLE* def) { 
		m_default = def; 
		if(def) 
			def->setBase(this); 
	}
	
	bool isDefault(const STYLE& style) {
		return &style == m_default;
	}
	
	StyleSet() : styles(), m_base(NULL), m_baseversion(-1), m_default(NULL) {}
	
	~StyleSet() { clear(); }
	
	void clear() { 
		while(styles.count()>0) 
		{ 
			delete styles.front(); 
			styles.pop_front(); 
		} 
	}

	void setBase(const StyleBase* base) { 
		m_base = base; 
		m_baseversion = -1;
	}
	
	const StyleBase* base() const { 
		return m_base; 
	}
	
private:
	StyleSet(const StyleSet&)             { assert(false); }
	StyleSet& operator= (const StyleSet&) { assert(false); return *this; }

	QValueList<STYLE*> styles;
	const StyleBase* m_base;
	int m_baseversion;
	STYLE* m_default;
};

template<class STYLE>
inline void StyleSet<STYLE>::remove(uint index)
{
	assert(index < styles.count()); 
	typename QValueList<STYLE*>::Iterator it = styles.at(index);
	if (*it == m_default)
		return;
	delete (*it);
	styles.erase(it);
	invalidate();
}

template<class STYLE>
inline int StyleSet<STYLE>::find(const QString& name) const
{
	for (uint i=0; i < styles.count(); ++i)
		if (styles[i]->name() == name)
			return i;
	return -1;
}

template<class STYLE>
inline const Style* StyleSet<STYLE>::resolve(const QString& name) const
{
	if (name.isEmpty())
		return m_default;
	for (uint i=0; i < styles.count(); ++i)
	{
		if (styles[i]->name() == name)
			return styles[i];
	}
	return m_base ? m_base->resolve(name) : NULL;
}

template<class STYLE>
inline void StyleSet<STYLE>::redefine(const StyleSet<STYLE>& defs, bool removeUnused)
{
	for (int i=signed(styles.count())-1; i >= 0; --i) 
	{
		bool found = false;
		for (uint j=0; j < defs.count(); ++j) 
		{
			if (styles[i]->name() == defs[j].name()) 
			{
				found = true;
				(*styles[i]) = defs[j];
				(*styles[i]).setBase(this);
				if (defs.m_default == defs.styles[j])
					makeDefault(styles[i]);
				break;
			}
		}
		if (!found && removeUnused) 
		{
			if (styles[i] == m_default)
				makeDefault(NULL);
			remove(i);
		}
	}
	for (uint j=0; j < defs.count(); ++j) 
	{
		if (find(defs[j].name()) < 0) 
		{
			STYLE* newStyle = create(defs[j]);
			if (defs.m_default == defs.styles[j])
				makeDefault(newStyle);
		}
	}
	invalidate();
}

#endif


