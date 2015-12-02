/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QCryptographicHash>

#include "pdfwriter.h"
#include "rc4.h"
#include "scstreamfilter_rc4.h"
#include "util.h"

namespace Pdf
{

	
	bool isWhiteSpace(char c)
	{
		switch (c)
		{
			case 0:
			case 9:
			case 10:
			case 12:
			case 13:
			case 32:
				return true;
			default:
				return false;
		}
	}
	
	bool isDelimiter(char c)
	{
		QByteArray delims("()<>[]{}/%");
		return delims.indexOf(c) >= 0;
	}
	
	bool isRegular(char c)
	{
		return !isWhiteSpace(c) && !isDelimiter(c);
	}
	
	QByteArray toPdfDocEncoding(QString s)
	{
		QByteArray result;
		for (int i = 0; i < s.length(); ++i)
		{
			uchar row = s[i].row();
			uchar cell = s[i].cell();
			switch(row)
			{
				case 0:
					if (cell <= 23
					    || (cell >= 32 && cell <= 127)
					    || cell >= 161)
					{
						result.append(cell);
					}
					else
					{
						result.append("?");
					}
					break;
				case 0x01:
					switch (cell)
				{
					case 0x92:
						result.append(134);
						break;
					case 0x41:
						result.append(149);
						break;
					case 0x52:
						result.append(150);
						break;
					case 0x60:
						result.append(151);
						break;
					case 0x78:
						result.append(152);
						break;
					case 0x7d:
						result.append(153);
						break;
					case 0x31:
						result.append(154);
						break;
					case 0x42:
						result.append(155);
						break;
					case 0x53:
						result.append(156);
						break;
					case 0x61:
						result.append(157);
						break;
					case 0x7e:
						result.append(158);
						break;
					default:
						result.append("?");
						break;
				}
					
				case 0x02:
					switch (cell)
				{
					case 0xc7:
						result.append(25);
						break;
					case 0xc6:
						result.append(26);
						break;
					case 0xd8:
						result.append(24);
						break;
					case 0xd9:
						result.append(27);
						break;
					case 0xda:
						result.append(30);
						break;
					case 0xdb:
						result.append(29);
						break;
					case 0xdc:
						result.append(31);
						break;
					case 0xdd:
						result.append(28);
						break;
					default:
						result.append("?");
						break;
				}
				case 0x20:
					switch (cell)
				{
					case 0x13:
						result.append(133);
						break;
					case 0x14:
						result.append(132);
						break;
					case 0x18:
						result.append(143);
						break;
					case 0x19:
						result.append(144);
						break;
					case 0x1a:
						result.append(145);
						break;
					case 0x1c:
						result.append(141);
						break;
					case 0x1d:
						result.append(142);
						break;
					case 0x1e:
						result.append(140);
						break;
					case 0x20:
						result.append(129);
						break;
					case 0x21:
						result.append(130);
						break;
					case 0x22:
						result.append(128);
						break;
					case 0x26:
						result.append(131);
						break;
					case 0x30:
						result.append(139);
						break;
					case 0x39:
						result.append(136);
						break;
					case 0x3a:
						result.append(137);
						break;
					case 0x44:
						result.append(135);
						break;
					case 0xac:
						result.append(160);
						break;
						
					default:
						result.append("?");
						break;
				}
				case 0x21:
					switch (cell)
				{
					case 0x22:
						result.append(146);
						break;
					default:
						result.append("?");
						break;
				}
				case 0x22:
					switch (cell)
				{
					case 0x12:
						result.append(138);
						break;
					default:
						result.append("?");
						break;
				}
				case 0xfb:
					switch (cell)
				{
					case 0x01:
						result.append(147);
						break;
					case 0x02:
						result.append(148);
						break;
					default:
						result.append("?");
						break;
				}
					
			}
		}
		return result;
	}
	
	QByteArray toUTF16(QString s)
	{
		QByteArray result;
		result.append(0xfe);
		result.append(0xff);
		for (int i = 0; i < s.length(); ++i)
		{
			result.append(s[i].row());
			result.append(s[i].cell());
		}
		return result;
	}
	
	QByteArray toAscii(QString s)
	{
		QByteArray result;
		for (int i = 0; i < s.length(); ++i)
		{
			uchar row = s[i].row();
			uchar cell = s[i].cell();
			if (row == 0 && cell <= 127)
			{
				result.append(cell);
			}
			else
			{
				result.append("?");
			}
		}
		return result;
	}
	
	QByteArray toPdf(bool v)
	{
		return v? "true" : "false";
	}
	
	QByteArray toPdf(int v)
	{
		return QByteArray::number(v);
	}
	
	QByteArray toPdf(uint v)
	{
		return QByteArray::number(v);
	}
	
	QByteArray toPdf(qlonglong v)
	{
		return QByteArray::number(v);
	}
	
	QByteArray toPdf(qulonglong v)
	{
		return QByteArray::number(v);
	}
	
	QByteArray toPdf(double v)
	{
		return QByteArray::number(v, 'f');
	}
	
	QByteArray toObjRef(PdfId id)
	{
		return toPdf(id) + " 0 R";
	}
	
	
	QByteArray toLiteralString(QString s)
	{
		return toLiteralString(toPdfDocEncoding(s));
	}
	
	QByteArray toLiteralString(QByteArray s)
	{
		int linelength = 80;
		QByteArray result("(");
		for (int i = 0; i < s.length(); ++i)
		{
			uchar v = s[i];
			if ( v == '(' || v == ')' || v == '\\')
			{
				result.append('\\');
				result.append(v);
			}
			else if (v < 32 || v >= 127)
			{
				result.append('\\');
				result.append("01234567"[ (v/64) % 8]);
				result.append("01234567"[ (v/8) % 8]);
				result.append("01234567"[ v % 8]);
			}
			else
			{
				result.append(v);
			}
			if (i % linelength == linelength-1)
				result.append("\\\n");
		}
		result.append(")");
		return result;
	}
	
	QByteArray toHexString(QByteArray s)
	{
		int linelength = 80;
		QByteArray result("<");
		for (int i = 0; i < s.length(); ++i)
		{
			uchar v = s[i];
			result.append("0123456789ABCDEF"[v / 16]);
			result.append("0123456789ABCDEF"[v % 16]);
			if (i % linelength == linelength-1)
				result.append("\n");
		}
		result.append(">");
		return result;
	}
	
	QByteArray toHexString8(quint8 b)
	{
		QByteArray result("<");
		result.append("0123456789ABCDEF"[b / 16]);
		result.append("0123456789ABCDEF"[b % 16]);
		result.append(">");
		return result;
	}

	QByteArray toHexString16(quint16 b)
	{
		QByteArray result("<");
		result.append("0123456789ABCDEF"[(b >> 12) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 8) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 4) & 0xf]);
		result.append("0123456789ABCDEF"[ b & 0xf]);
		result.append(">");
		return result;
	}
	
	QByteArray toHexString32(quint32 b)
	{
		QByteArray result("<");
		result.append("0123456789ABCDEF"[(b >> 28) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 24) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 20) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 16) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 12) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 8) & 0xf]);
		result.append("0123456789ABCDEF"[(b >> 4) & 0xf]);
		result.append("0123456789ABCDEF"[ b & 0xf]);
		result.append(">");
		return result;
	}
	
	
	QByteArray toName(QString s)
	{
		return toName(toPdfDocEncoding(s));
	}
	

	QByteArray toName(QByteArray s)
	{
		QByteArray result("/");
		for (int i = 0; i < s.length(); ++i)
		{
			uchar c = s[i];
			if (c <= 32 || c >= 127 || c == '#' || isDelimiter(c))
			{
				result.append("#");
				result.append("0123456789ABCDEF"[c / 16]);
				result.append("0123456789ABCDEF"[c % 16]);
			}
			else
			{
				result.append(c);
			}
		}
		return result;
	}
	
	QByteArray toDateString(QDateTime dt)
	{
		QString tmp = dt.toString("yyyy:MM:dd:HH:mm");
		tmp = tmp.replace(":", "");
		return "D:" + tmp.toLatin1() + "Z";
	}

	
	QByteArray toRectangleArray(QRect r)
	{
		return "[" + toPdf(r.left()) + " " + toPdf(r.bottom()) + " " + toPdf(r.right()) + " " + toPdf(r.top()) + "]";
	}
	
	QByteArray toRectangleArray(QRectF r)
	{
		return "[" + toPdf(r.left()) + " " + toPdf(r.bottom()) + " " + toPdf(r.right()) + " " + toPdf(r.top()) + "]";
	}
 
 


	Writer::Writer()
	{
		CatalogObj = 1;
		InfoObj = 2;
		OutlinesObj = 3;
		PagesObj = 4;
		DestsObj = 5;
		AcroFormObj = 6;
		NamesObj = 7;
		ThreadsObj = 8;
		
		OutputIntentObj = 0;
		EncryptObj = 0;
		MetaDataObj = 0;
		ResourcesObj = 0;
		
		m_CurrentObj = 0;
		
		m_KeyLen = 5;
		m_KeyGen.resize(32);
		m_OwnerKey.resize(32);
		m_UserKey.resize(32);
		m_FileID.resize(16);
		m_EncryKey.resize(5);
		
		int kg_array[] = {
		    0x28, 0xbf, 0x4e, 0x5e, 0x4e, 0x75, 0x8a, 0x41,
		    0x64, 0x00, 0x4e, 0x56, 0xff, 0xfa, 0x01, 0x08,
		    0x2e, 0x2e, 0x00, 0xb6, 0xd0, 0x68, 0x3e, 0x80,
		    0x2f, 0x0c, 0xa9, 0xfe, 0x64, 0x53, 0x69, 0x7a };
		for (int a = 0; a < 32; ++a)
			m_KeyGen[a] = kg_array[a];

	}
	
	
	bool Writer::open(const QString& fn)
	{
		m_Spool.setFileName(fn);
		if (!m_Spool.open(QIODevice::WriteOnly))
			return false;
		m_outStream.setDevice(&m_Spool);
		m_ObjCounter = 9;
		return true;
	}
	
	
	ScStreamFilter* Writer::openStreamFilter(bool encrypted, PdfId objId)
	{
		if (encrypted)
		{
			QByteArray step1 = ComputeRC4Key(m_ObjCounter);
			return new ScRC4EncodeFilter(&m_outStream, step1.data(), qMin(m_KeyLen+5, 16));
		}
		else
		{
			return new ScNullEncodeFilter(&m_outStream);
		}
	}
	
	
	bool Writer::close(bool abortExport)
	{
		bool result = (m_Spool.error() == QFile::NoError);

		m_Spool.close();
		if (abortExport || !result)
		{
			if (m_Spool.exists())
				m_Spool.remove();
		}
		return result;
	}
	
	
	void Writer::setFileId(const QByteArray& id)
	{
		m_FileID = QCryptographicHash::hash(id, QCryptographicHash::Md5);
	}
	
	
	void Writer::setEncryption(bool keyLen16, const QByteArray& PassOwner, const QByteArray& PassUser, int Permissions)
	{
		QByteArray ok = "";
		QByteArray uk = "";
		
		if (keyLen16)
			m_KeyLen = 16;
		else
			m_KeyLen = 5;
		CalcOwnerKey(PassOwner, PassUser);
		CalcUserKey(PassUser, Permissions);
		for (uint cl2 = 0; cl2 < 32; ++cl2)
			ok += (m_OwnerKey[cl2]);
		if (keyLen16)
		{
			for (uint cl3 = 0; cl3 < 16; ++cl3)
				uk += (m_UserKey[cl3]);
			for (uint cl3r = 0; cl3r < 16; ++cl3r)
				uk += (m_KeyGen[cl3r]);
		}
		else
		{
			for (uint cl = 0; cl < 32; ++cl)
				uk += (m_UserKey[cl]);
		}

		EncryptObj = newObject();
		startObj(EncryptObj);
		write("<<\n/Filter /Standard\n");
		write( keyLen16 ? "/R 3\n/V 2\n/Length 128\n" : "/R 2\n/V 1\n");
		write("/O "+Pdf::toHexString(ok)+"\n");
		write("/U "+Pdf::toHexString(uk)+"\n");
		write("/P "+Pdf::toPdf(Permissions)+"\n>>");
		endObj(EncryptObj);

	}
	
	
	QByteArray Writer::encryptBytes(const QByteArray& in, PdfId ObjNum)
	{
		rc4_context_t rc4;
		QByteArray result(in.length(), ' ');
		if (in.length() > 0)
		{
			QByteArray step1 = ComputeRC4Key(ObjNum);
			rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), qMin(m_KeyLen+5, 16));
			rc4_encrypt(&rc4, reinterpret_cast<const uchar*>(in.data()), reinterpret_cast<uchar*>(result.data()), in.length());
		}
		return result;
	}
	
	QByteArray Writer::ComputeRC4Key(PdfId ObjNum)
	{
		int dlen = 0;
		QByteArray data(10, ' ');
		if (m_KeyLen > 5)
			data.resize(21);
		for (int cd = 0; cd < m_KeyLen; ++cd)
		{
			data[cd] = m_EncryKey[cd];
			dlen++;
		}
		data[dlen++] = ObjNum;
		data[dlen++] = ObjNum >> 8;
		data[dlen++] = ObjNum >> 16;
		data[dlen++] = 0;
		data[dlen++] = 0;
		QByteArray rc4Key(16, ' ');
		rc4Key = QCryptographicHash::hash(data, QCryptographicHash::Md5);
		rc4Key.resize(qMin(m_KeyLen+5, 16));
		return rc4Key;
	}
	

	QByteArray Writer::FitKey(const QByteArray & pass)
	{
		QByteArray pw(pass);
		if (pw.length() < 32)
		{
			uint l = pw.length();
			for (uint a = 0; a < 32 - l; ++a)
				pw += (m_KeyGen[a]);
		}
		else
			pw = pw.left(32);
		return pw;
	}
	
	void Writer::CalcOwnerKey(const QByteArray & Owner, const QByteArray & User)
	{
		rc4_context_t rc4;
		QByteArray pw(FitKey(User));
		QByteArray pw2(FitKey(Owner.isEmpty() ? User : Owner));
		QByteArray step1(16, ' ');
		step1 = QCryptographicHash::hash(pw2, QCryptographicHash::Md5);
		if (m_KeyLen > 5)
		{
			for (int kl = 0; kl < 50; ++kl)
				step1 = QCryptographicHash::hash(step1, QCryptographicHash::Md5);
		}
		QByteArray us(32, ' ');
		QByteArray enk(16, ' ');
		if (m_KeyLen > 5)
		{
			for (uint a2 = 0; a2 < 32; ++a2)
				m_OwnerKey[a2] = QChar(pw.at(a2)).cell();
			for (int rl = 0; rl < 20; rl++)
			{
				for (int j = 0; j < 16; j ++)
					enk[j] = step1[j] ^ rl;
				rc4_init(&rc4, reinterpret_cast<uchar*>(enk.data()), 16);
				rc4_encrypt(&rc4, reinterpret_cast<uchar*>(m_OwnerKey.data()),
							reinterpret_cast<uchar*>(m_OwnerKey.data()), 32);
			}
		}
		else
		{
			for (uint a = 0; a < 32; ++a)
				us[a] = static_cast<uchar>(QChar(pw.at(a)).cell());
			rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), 5);
			rc4_encrypt(&rc4, reinterpret_cast<uchar*>(us.data()),
						reinterpret_cast<uchar*>(m_OwnerKey.data()), 32);
		}
	}
	
	void Writer::CalcUserKey(const QByteArray & User, int Permission)
	{
		rc4_context_t	rc4;
		QByteArray pw(FitKey(User));
		QByteArray step1(16, ' ');
		QByteArray perm(4, ' ');
		uint perm_value = static_cast<uint>(Permission);
		perm[0] = perm_value;
		perm[1] = perm_value >> 8;
		perm[2] = perm_value >> 16;
		perm[3] = perm_value >> 24;
		for (uint a = 0; a < 32; ++a)
			pw += (m_OwnerKey[a]);
		for (uint a1 = 0; a1 < 4; ++a1)
			pw += (perm[a1]);
		for (uint a3 = 0; a3 < 16; ++a3)
			pw += (m_FileID[a3]);
		step1 = QCryptographicHash::hash(pw, QCryptographicHash::Md5);
		if (m_KeyLen > 5)
		{
			for (int kl = 0; kl < 50; ++kl)
				step1 = QCryptographicHash::hash(step1, QCryptographicHash::Md5);
			m_EncryKey.resize(16);
		}
		for (int a2 = 0; a2 < m_KeyLen; ++a2)
			m_EncryKey[a2] = step1[a2];
		if (m_KeyLen > 5)
		{
			QByteArray pr2("");
			for (int kl3 = 0; kl3 < 32; ++kl3)
				pr2 += (m_KeyGen[kl3]);
			for (uint a4 = 0; a4 < 16; ++a4)
				pr2 += (m_FileID[a4]);
			step1 = QCryptographicHash::hash(pr2, QCryptographicHash::Md5);
			QByteArray enk(16, ' ');
			for (uint a3 = 0; a3 < 16; ++a3)
				m_UserKey[a3] = step1[a3];
			for (int rl = 0; rl < 20; rl++)
			{
				for (int j = 0; j < 16; j ++)
					enk[j] = m_EncryKey[j] ^ rl;
				rc4_init(&rc4, reinterpret_cast<uchar*>(enk.data()), 16);
				rc4_encrypt(&rc4, reinterpret_cast<uchar*>(m_UserKey.data()), reinterpret_cast<uchar*>(m_UserKey.data()), 16);
			}
		}
		else
		{
			rc4_init(&rc4, reinterpret_cast<uchar*>(step1.data()), 5);
			rc4_encrypt(&rc4, reinterpret_cast<uchar*>(m_KeyGen.data()), reinterpret_cast<uchar*>(m_UserKey.data()), 32);
		}
	}
	
//	QByteArray PDFLibCore::ComputeMD5(const QString& in)
//	{
//		uint inlen=in.length();
//		QByteArray TBytes(inlen, ' ');
//		for (uint a = 0; a < inlen; ++a)
//			TBytes[a] = static_cast<uchar>(QChar(in.at(a)).cell());
//		return QCryptographicHash::hash(TBytes, QCryptographicHash::Md5);
//	}
	
	
	void Writer::writeHeader(PDFOptions::PDFVersion Version)
	{
		switch (Version)
		{
			case PDFOptions::PDFVersion_X1a:
			case PDFOptions::PDFVersion_X3:
			case PDFOptions::PDFVersion_13:
				write("%PDF-1.3\n");
				break;
			case PDFOptions::PDFVersion_14:
				write("%PDF-1.4\n");
				break;
			case PDFOptions::PDFVersion_X4:
			case PDFOptions::PDFVersion_15:
				write("%PDF-1.5\n");
				break;
		}
		write("%\xc7\xec\x8f\xa2\n");
	}
	
	void Writer::writeXrefAndTrailer()
	{
		QByteArray tmp;
		uint StX = bytesWritten();
		write("xref\n");
		write("0 "+Pdf::toPdf(m_ObjCounter)+"\n");
		//write("0000000000 65535 f \n");
		for (int a = 0; a < m_XRef.count(); ++a)
		{
			if (m_XRef[a] > 0)
			{
				tmp.setNum(m_XRef[a]);
				while (tmp.length()< 10)
				{
					tmp.prepend('0');
				}
				write(tmp);
				write(" 00000 n \n");
			}
			else
			{
				// unused object, mark as free-never-to-be-used-again
				write("0000000000 65535 f \n");
			}
		}
		write("trailer\n<<\n/Size "+Pdf::toPdf(m_XRef.count())+"\n");
		QByteArray IDs ="";
		for (uint cl = 0; cl < 16; ++cl)
			IDs += (m_FileID[cl]);
		QByteArray IDbytes = Pdf::toHexString(IDs);
		write("/Root 1 0 R\n/Info 2 0 R\n/ID ["+IDbytes+IDbytes+"]\n");
		if (EncryptObj > 0)
			write("/Encrypt "+Pdf::toObjRef(EncryptObj)+"\n");
		write(">>\nstartxref\n");
		write(Pdf::toPdf(StX)+"\n%%EOF\n");
	}
	
	
	void Writer::write(const QByteArray& bytes)
	{
		m_outStream.writeRawData(bytes, bytes.size());
	}
	
	void Writer::write(const ResourceDictionary& dict)
	{
		write("<< /ProcSet [/PDF /Text /ImageB /ImageC /ImageI]\n");
		if (dict.XObject.count() != 0)
		{
			write("/XObject <<\n");
			ResourceMap::ConstIterator iti;
			for (iti = dict.XObject.begin(); iti != dict.XObject.end(); ++iti)
				write(Pdf::toName(iti.key()) + " " + Pdf::toObjRef(iti.value()) + "\n");
			write(">>\n");
		}
		if (dict.Font.count() != 0)
		{
			write("/Font << \n");
			QMap<QByteArray,PdfId>::ConstIterator it2;
			for (it2 = dict.Font.begin(); it2 != dict.Font.end(); ++it2)
				write(Pdf::toName(it2.key()) + " " + Pdf::toObjRef(it2.value()) + "\n");
			write(">>\n");
		}
		if (dict.Shading.count() != 0)
		{
			write("/Shading << \n");
			QMap<QByteArray,PdfId>::ConstIterator it3;
			for (it3 = dict.Shading.begin(); it3 != dict.Shading.end(); ++it3)
				write(Pdf::toName(it3.key()) + " " + Pdf::toObjRef(it3.value()) + "\n");
			write(">>\n");
		}
		if (dict.Pattern.count() != 0)
		{
			write("/Pattern << \n");
			QMap<QByteArray,PdfId>::ConstIterator it3p;
			for (it3p = dict.Pattern.begin(); it3p != dict.Pattern.end(); ++it3p)
				write(Pdf::toName(it3p.key()) + " " + Pdf::toObjRef(it3p.value()) + "\n");
			write(">>\n");
		}
		if (dict.ExtGState.count() != 0)
		{
			write("/ExtGState << \n");
			QMap<QByteArray,PdfId>::ConstIterator it3t;
			for (it3t = dict.ExtGState.begin(); it3t != dict.ExtGState.end(); ++it3t)
				write(Pdf::toName(it3t.key()) + " " + Pdf::toObjRef(it3t.value()) + "\n");
			write(">>\n");
		}
		if (dict.Properties.count() != 0)
		{
			write("/Properties << \n");
			QMap<QByteArray,PdfId>::ConstIterator it4p;
			for (it4p = dict.ExtGState.begin(); it4p != dict.ExtGState.end(); ++it4p)
				write(Pdf::toName(it4p.key()) + " " + Pdf::toObjRef(it4p.value()) + "\n");
			write(">>\n");
		}
		if (dict.ColorSpace.count() != 0)
		{
			write("/ColorSpace << \n");
			QList<Resource>::ConstIterator it3c;
			for (it3c = dict.ColorSpace.begin(); it3c != dict.ColorSpace.end(); ++it3c)
					write(Pdf::toName(it3c->ResName) + " " + Pdf::toObjRef(it3c->ResNum) + "\n");
			
			write(">>\n");
		}
		write(">>\n");
	}
	
	
	PdfId Writer::reserveObjects(unsigned int n)
	{
		assert( n < (1<<30) ); // should only be triggered by reserveObjects(-1) or similar
		PdfId result = m_ObjCounter;
		m_ObjCounter += n;
		return result;
	}

	void Writer::startObj(PdfId id)
	{
		assert( m_CurrentObj == 0);
		m_CurrentObj = id;
		while (static_cast<uint>(m_XRef.length()) <= id)
			m_XRef.append(0);
		m_XRef[id] = bytesWritten();
		write(toPdf(id));
		write(" 0 obj\n");
	}
	
	void Writer::endObj(PdfId id)
	{
		assert( m_CurrentObj == id);
		m_CurrentObj = 0;
		write("\nendobj\n");
	}
	
	void Writer::endObjectWithStream(bool encrypted, PdfId id, const QByteArray& streamContent)
	{
		assert( m_CurrentObj == id);
		write("\nstream\n");
		write(encrypted? encryptBytes(streamContent, id): streamContent);
		write("\nendstream");
		endObj(id);
	}
	
} // namespace PDF
