/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "sccolortransform.h"

ScColorTransform::ScColorTransform() : m_data(NULL)
{
}

ScColorTransform::ScColorTransform(ScColorTransformData* data) : m_data(data)
{
}

ScColorTransform::ScColorTransform(const QSharedPointer<ScColorTransformData>& data) : m_data(data)
{
}

bool ScColorTransform::apply(void* input, void* output, uint numElem)
{
	return m_data->apply(input, output, numElem);
}

bool ScColorTransform::apply(QByteArray& input, QByteArray& output, uint numElem)
{
	return m_data->apply(input, output, numElem);
}

void ScColorTransform::changeBufferFormat(eColorFormat inputFormat, eColorFormat outputFormat)
{
	m_data->changeBufferFormat(inputFormat, outputFormat);
}

bool ScColorTransform::operator==(const ScColorTransform& other) const
{
	return m_data == other.m_data;
}
