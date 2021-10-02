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

	uchar toPdfDocEncoding(QChar c)
	{
		uchar row = c.row();
		uchar cell = c.cell();
		switch (row)
		{
			case 0:
				if (cell <= 23
					|| (cell >= 32 && cell <= 127)
					|| cell >= 161)
				{
					return cell;
				}
				else
				{
					return 0;
				}
				break;
			case 0x01:
				switch (cell)
				{
					case 0x92:
						return 134;
					case 0x41:
						return 149;
					case 0x52:
						return 150;
					case 0x60:
						return 151;
					case 0x78:
						return 152;
					case 0x7d:
						return 153;
					case 0x31:
						return 154;
					case 0x42:
						return 155;
					case 0x53:
						return 156;
					case 0x61:
						return 157;
					case 0x7e:
						return 158;
					default:
						return 0;
						break;
				}
				break;

			case 0x02:
				switch (cell)
				{
					case 0xc7:
						return 25;
					case 0xc6:
						return 26;
					case 0xd8:
						return 24;
					case 0xd9:
						return 27;
					case 0xda:
						return 30;
					case 0xdb:
						return 29;
					case 0xdc:
						return 31;
					case 0xdd:
						return 28;
					default:
						return 0;
						break;
				}
				break;

			case 0x20:
				switch (cell)
				{
					case 0x13:
						return 133;
					case 0x14:
						return 132;
					case 0x18:
						return 143;
					case 0x19:
						return 144;
					case 0x1a:
						return 145;
					case 0x1c:
						return 141;
					case 0x1d:
						return 142;
					case 0x1e:
						return 140;
					case 0x20:
						return 129;
					case 0x21:
						return 130;
					case 0x22:
						return 128;
					case 0x26:
						return 131;
					case 0x30:
						return 139;
					case 0x39:
						return 136;
					case 0x3a:
						return 137;
					case 0x44:
						return 135;
					case 0xac:
						return 160;
						
					default:
						return 0;
						break;
				}
				break;

			case 0x21:
				switch (cell)
				{
					case 0x22:
						return 146;
					default:
						return 0;
						break;
				}
				break;

			case 0x22:
				switch (cell)
				{
					case 0x12:
						return 138;
					default:
						return 0;
						break;
				}
				break;

			case 0xfb:
				switch (cell)
				{
					case 0x01:
						return 147;
					case 0x02:
						return 148;
					default:
						return 0;
						break;
				}
				break;
		}

		return 0;
	}
	
	QByteArray toPdfDocEncoding(QString s)
	{
		QByteArray result;
		for (int i = 0; i < s.length(); ++i)
		{
			uchar pdfChar = toPdfDocEncoding(s[i]);
			if ((pdfChar != 0) || s[i].isNull())
				result += pdfChar;
			else
				result += "?";
		}
		return result;
	}

	int fromPDFDocEncoding(int code)
	{
		if (code <= 23
			|| (code >= 32 && code <= 127)
			|| (code >= 161))
		{
			return code;
		}

		if (code == 24)  return 0x02d8;
		if (code == 25)  return 0x02c7;
		if (code == 26)  return 0x02c6;
		if (code == 27)  return 0x02d9;
		if (code == 28)  return 0x02dd;
		if (code == 29)  return 0x02db;
		if (code == 30)  return 0x02da;
		if (code == 31)  return 0x02dc;

		if (code == 128) return 0x2022;
		if (code == 129) return 0x2020;

		if (code == 130) return 0x2021;
		if (code == 131) return 0x2026;
		if (code == 132) return 0x2014;
		if (code == 133) return 0x2013;
		if (code == 134) return 0x0192;
		if (code == 135) return 0x2044;
		if (code == 136) return 0x2039;
		if (code == 137) return 0x203a;
		if (code == 138) return 0x2212;
		if (code == 139) return 0x2030;

		if (code == 140) return 0x201e;
		if (code == 141) return 0x201c;
		if (code == 142) return 0x201d;
		if (code == 143) return 0x2018;
		if (code == 144) return 0x2019;
		if (code == 145) return 0x201a;
		if (code == 146) return 0x2122;
		if (code == 147) return 0xfb01;
		if (code == 148) return 0xfb02;
		if (code == 149) return 0x0141;

		if (code == 150) return 0x0152;
		if (code == 151) return 0x0160;
		if (code == 152) return 0x0178;
		if (code == 153) return 0x017d;
		if (code == 154) return 0x0131;
		if (code == 155) return 0x0142;
		if (code == 156) return 0x0153;
		if (code == 157) return 0x0161;
		if (code == 158) return 0x017e;

		if (code == 160) return 0x20ac;

		return 0;
	}
	
	QByteArray toUTF16(QString s)
	{
		QByteArray result;
		result.append('\xfe');
		result.append('\xff');
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
	
	
	QByteArray toLiteralString(const QString& s)
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
	
	QByteArray toName(const QString& s)
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
	
	QByteArray toDateString(const QDateTime& dt)
	{
		QString tmp = dt.toString("yyyy:MM:dd:HH:mm:ss");
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
		m_ObjCounter = 4;
		return true;
	}
	
	ScStreamFilter* Writer::openStreamFilter(bool encrypted, PdfId objId)
	{
		if (encrypted)
		{
			QByteArray step1 = ComputeRC4Key(objId);
			return new ScRC4EncodeFilter(&m_outStream, step1.data(), qMin(m_KeyLen+5, 16));
		}
		return new ScNullEncodeFilter(&m_outStream);
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
	
	void Writer::writeHeader(const PDFVersion& Version)
	{
		switch (Version)
		{
			case PDFVersion::PDF_X1a:
			case PDFVersion::PDF_X3:
			case PDFVersion::PDF_13:
				write("%PDF-1.3\n");
				break;
			case PDFVersion::PDF_14:
				write("%PDF-1.4\n");
				break;
			case PDFVersion::PDF_15:
				write("%PDF-1.5\n");
				break;
			case PDFVersion::PDF_16:
			case PDFVersion::PDF_X4:
				write("%PDF-1.6\n");
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
			for (it4p = dict.Properties.begin(); it4p != dict.Properties.end(); ++it4p)
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
