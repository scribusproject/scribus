/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UTIL_KEYBOARD_H
#define UTIL_KEYBOARD_H

#include <qnamespace.h>

// These functions are intended to help concatenating Qt::Modifier and Qt::Key enums 
// into a single integer while avoiding -Wdeprecated-enum-enum-conversion warnings 
// when compiling in C++20 mode

template<typename T>
int makeKeySequence(T first)
{
	return static_cast<int>(first);
}

template<typename T, typename... Ts>
int makeKeySequence(T first, Ts... args)
{
	return static_cast<int>(first) + makeKeySequence(args...);
}

#if __cplusplus >= 202002L
inline int operator+(int k1, Qt::Key k2)
{
	return k1 + static_cast<int>(k2);
}

inline int operator+(int k1, Qt::KeyboardModifier k2)
{
	return k1 + static_cast<int>(k2);
}

inline int operator+(int k1, Qt::Modifier k2)
{
	return k1 + static_cast<int>(k2);
}

inline int operator+(Qt::Key k1, int k2)
{
	return static_cast<int>(k1) + k2;
}

inline int operator+(Qt::Key k1, Qt::KeyboardModifier k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::Key k1, Qt::Modifier k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::KeyboardModifier k1, int k2)
{
	return static_cast<int>(k1) + k2;
}

inline int operator+(Qt::KeyboardModifier k1, Qt::Key k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::KeyboardModifier k1, Qt::KeyboardModifier k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::KeyboardModifier k1, Qt::Modifier k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::Modifier k1, int k2)
{
	return static_cast<int>(k1) + k2;
}

inline int operator+(Qt::Modifier k1, Qt::Key k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::Modifier k1, Qt::KeyboardModifier k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator+(Qt::Modifier k1, Qt::Modifier k2)
{
	return static_cast<int>(k1) + static_cast<int>(k2);
}

inline int operator|(int k1, Qt::Key k2)
{
	return k1 | static_cast<int>(k2);
}

inline int operator|(int k1, Qt::KeyboardModifier k2)
{
	return k1 | static_cast<int>(k2);
}

inline int operator|(int k1, Qt::Modifier k2)
{
	return k1 | static_cast<int>(k2);
}

inline int operator|(Qt::Key k1, int k2)
{
	return static_cast<int>(k1) | k2;
}

inline int operator|(Qt::Key k1, Qt::KeyboardModifier k2)
{
	return static_cast<int>(k1) | static_cast<int>(k2);
}

inline int operator|(Qt::Key k1, Qt::Modifier k2)
{
	return static_cast<int>(k1) | static_cast<int>(k2);
}

inline int operator|(Qt::KeyboardModifier k1, int k2)
{
	return static_cast<int>(k1) | k2;
}

inline int operator|(Qt::KeyboardModifier k1, Qt::Key k2)
{
	return static_cast<int>(k1) | static_cast<int>(k2);
}

inline int operator|(Qt::KeyboardModifier k1, Qt::Modifier k2)
{
	return static_cast<int>(k1) | static_cast<int>(k2);
}

inline int operator|(Qt::Modifier k1, int k2)
{
	return static_cast<int>(k1) | k2;
}

inline int operator|(Qt::Modifier k1, Qt::Key k2)
{
	return static_cast<int>(k1) | static_cast<int>(k2);
}

inline int operator|(Qt::Modifier k1, Qt::KeyboardModifier k2)
{
	return static_cast<int>(k1) | static_cast<int>(k2);
}
#endif // #if __cplusplus >= 202002L

#endif
