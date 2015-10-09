/*
    Copyright (C)  2010  Brad Hards <bradh@frogmouth.net>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PictDestination.h"

#include "rtfreader.h"

namespace RtfReader
{
	PictDestination::PictDestination(Reader *reader, AbstractRtfOutput *output, const QString &name) : Destination(reader, output, name)
	{
	}

	PictDestination::~PictDestination()
	{
	}

	void PictDestination::handleControlWord( const QString &controlWord, bool hasValue, const int value )
	{
	//	qDebug() << "ControlWord = " << controlWord;
		if (controlWord == "jpegblip")
			m_img_type = 0;
		else if (controlWord == "wmetafile")
			m_img_type = 1;
		else if (controlWord == "emfblip")
			m_img_type = 2;
		else if (controlWord == "macpict")
			m_img_type = 3;
		else if (controlWord == "pngblip")
			m_img_type = 4;
		else if (controlWord == "picw")
			m_width = value;
		else if (controlWord == "pich")
			m_height = value;
		else if (controlWord == "picscalex")
			m_scale_x = value;
		else if (controlWord == "picscaley")
			m_scale_y = value;
		else if (controlWord == "piccropl")
			m_crop_left = value;
		else if (controlWord == "piccropr")
			m_crop_right = value;
		else if (controlWord == "piccropt")
			m_crop_top = value;
		else if (controlWord == "piccropb")
			m_crop_bottom = value;
		else if (controlWord == "pichgoal")
			m_dest_height = value;
		else if (controlWord == "picwgoal")
			m_dest_width = value;
	}

	void PictDestination::handlePlainText(const QByteArray &plainText)
	{
		m_pictHexData = plainText;
	}

	void PictDestination::aboutToEndDestination()
	{
		QByteArray image = QByteArray::fromHex(m_pictHexData);
		m_output->createImage(image, m_dest_width, m_dest_height, m_img_type);
	}
}
