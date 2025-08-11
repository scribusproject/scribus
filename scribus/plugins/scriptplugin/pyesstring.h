/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PYESSTRING_H
#define PYESSTRING_H

#include <cstring>

class PyESString
{
public:
	PyESString() = default;
	PyESString(const PyESString&) = delete;
	~PyESString();
	
	PyESString& operator=(const PyESString&) = delete;
	
	char** const ptr() { return &m_pStr; }

	const char* c_str() const { return m_pStr ? m_pStr : ""; }
	const char* data() const { return m_pStr; }
	const char* defaulted(const char* def) const { return (!isEmpty() ? m_pStr : def); }

	void free();

	bool isEmpty() const { return (!m_pStr || strlen(m_pStr) == 0); }
	size_t length() const { return (m_pStr ? strlen(m_pStr) : 0); }
	
private:
	char* m_pStr { nullptr };
};

#endif
