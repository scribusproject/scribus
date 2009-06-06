

#ifndef STYLESET_H
#define STYLESET_H

#include <QList>

#include <assert.h>
#include "style.h"


template<class STYLE>
class StyleSet : public StyleContext {
public:
	STYLE& operator[] (int index) { 
		assert(index < styles.count()); 
		return * styles[index]; 
	}
	
	STYLE* getDefault(){ return m_default; }		
	
	const STYLE& get(const QString& name) const { 
		return * dynamic_cast<const STYLE*>(resolve(name)); 
	}
	
	const STYLE& operator[] (int index) const { 
		assert(index < styles.count()); 
		return * styles[index]; 
	}
	
	inline int find(const QString& name) const;

	inline const Style* resolve(const QString& name) const;
	
	int count() const {
		return styles.count();
	}
	
	STYLE* append(STYLE* style) { 
		styles.append(style); 
		style->setContext(this); 
		return style; 
	}
	
	inline void remove(int index);
	
	inline void redefine(const StyleSet<STYLE>& defs, bool removeUnused=false);
	
	inline void rename(const QMap<QString,QString>& newNames);
	
	STYLE* create(const STYLE& proto) { 
		return append(new STYLE(proto)); 
	}
	
	void makeDefault(STYLE* def) { 
		m_default = def; 
		if(def) 
			def->setContext(this);
		invalidate();
	}
	
	bool isDefault(const STYLE& style) const {
		return &style == m_default;
	}
	
	
	StyleSet() : styles(), m_context(NULL), m_default(NULL) {}
	
	~StyleSet() { 
		clear(); 
	}
	
	void clear() { 
		while(styles.count()>0) 
		{ 
			delete styles.front(); 
			styles.pop_front(); 
		}
		invalidate();
	}

	void setContext(const StyleContext* context) {
		bool reallyNew = m_context != context;
		m_context = context; 
		if (reallyNew)
			invalidate();
	}
	
	const StyleContext* context() const { 
		return m_context; 
	}
	
			
private:
	StyleSet(const StyleSet&)             { assert(false); }
	StyleSet& operator= (const StyleSet&) { assert(false); return *this; }

	QList<STYLE*> styles;
	const StyleContext* m_context;
	STYLE* m_default;
};

template<class STYLE>
inline void StyleSet<STYLE>::remove(int index)
{
	assert(index>=0 && index < styles.count()); 
//	QList<STYLE*> it = styles.at(index);
	if (styles.at(index) == m_default)
		return;
//	delete (*it);
//	styles.erase(it);
	styles.removeAt(index);
}

template<class STYLE>
inline int StyleSet<STYLE>::find(const QString& name) const
{
	for (int i=0; i < styles.count(); ++i)
		if (styles[i]->name() == name)
			return i;
	return -1;
}

template<class STYLE>
inline const Style* StyleSet<STYLE>::resolve(const QString& name) const
{
	if (name.isEmpty())
		return m_default;
	for (int i=0; i < styles.count(); ++i)
	{
		if (styles[i]->name() == name)
			return styles[i];
	}
	return m_context ? m_context->resolve(name) : NULL;
}

template<class STYLE>
inline void StyleSet<STYLE>::redefine(const StyleSet<STYLE>& defs, bool removeUnused)
{
	for (int i=signed(styles.count())-1; i >= 0; --i) 
	{
		bool found = false;
		for (int j=0; j < defs.count(); ++j)
		{
			if (styles[i]->name() == defs[j].name()) 
			{
				found = true;
				(*styles[i]) = defs[j];
				(*styles[i]).setContext(this);
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
	for (int j=0; j < defs.count(); ++j)
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

template<class STYLE>
inline void StyleSet<STYLE>::rename(const QMap<QString,QString>& newNames)
{
	for (int i=0; i < styles.count(); ++i)
	{ 
		QMap<QString,QString>::ConstIterator it;
		
		it = newNames.find(styles[i]->name());
		if (it != newNames.end())
			styles[i]->setName(it.value());
	
		it = newNames.find(styles[i]->parent());
		if (it != newNames.end())
			styles[i]->setParent(it.value());
	}
	invalidate();
}
	
#endif


