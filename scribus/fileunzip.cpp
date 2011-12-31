/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "scconfig.h"
#include "fileunzip.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifndef _MSC_VER
#include <utime.h>
#else
#include <direct.h>
#include <time.h>
#include <sys/utime.h>
#endif

#include <iostream>
#include "unzip.h"
#include <zlib.h>

#include <QDir>
#include <QByteArray>

#include "scpaths.h"

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* password);
int do_extract_onefile(unzFile uf, const QString& filename, int opt_extract_without_path, int opt_overwrite, const char* password);
FILE*    openfile(const QString& filename, const QString& mode);
unzFile  unzOpenFile(const QString& filename);
int mymkdir(const QString& dirname);
int makedir (const QString& newdir);
void change_file_date(const QString& filename, uLong dosdate, tm_unz tmu_date);

#if defined(_WIN32) && defined(HAVE_UNICODE)
zlib_filefunc_def file_api;
static voidpf ZCALLBACK fopen_filefunc_unicode(voidpf opaque, const char *filename, int mode);
#define utimbuf _utimbuf
#endif

FileUnzip::FileUnzip(QString zipFilePath)
{
	zipFile = zipFilePath;
}

QString FileUnzip::getFile(QString name)
{
	QString pwd = QDir::currentPath();
	QString outDir = ScPaths::getTempFileDir();
	QFile f(outDir);
	QFileInfo fi(f);
	if (!fi.isWritable())
		outDir = ScPaths::getApplicationDataDir();
	QDir::setCurrent(outDir);
	unzFile uf = unzOpenFile(zipFile);
	int ret = do_extract_onefile(uf,name, 0, 1, NULL);
	unzClose(uf);
	QDir::setCurrent(pwd);
	if (ret != 0)
		return NULL;
	else
		return outDir + name;
}

FileUnzip::~FileUnzip()
{

}

FILE* openfile(const QString& filename, const QString& mode)
{
	FILE* fout = NULL;
#if defined(_WIN32) && defined(HAVE_UNICODE)
	fout = _wfopen((const wchar_t*) filename.utf16(), (const wchar_t*) mode.utf16());
#else
	QByteArray fname = filename.toLocal8Bit();
	QByteArray fmode  = mode.toLocal8Bit();
    fout = fopen(fname.data(), fmode.data());
#endif
	return fout;
}

unzFile unzOpenFile(const QString& filename)
{
	unzFile unz;
#if defined(_WIN32) && defined(HAVE_UNICODE)
	fill_fopen_filefunc(&file_api);
	file_api.zopen_file = fopen_filefunc_unicode;
	unz = unzOpen2((const char*)filename.utf16(), &file_api); 
#else
	QByteArray fname(filename.toLocal8Bit());
	unz = unzOpen(fname.data()); 
#endif
	return unz;
}

#if defined(_WIN32) && defined(HAVE_UNICODE)
static voidpf ZCALLBACK fopen_filefunc_unicode(voidpf opaque, const char *filename, int mode)
{
	FILE* file = NULL;
	const wchar_t* mode_fopen = NULL;
	if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER)==ZLIB_FILEFUNC_MODE_READ)
		mode_fopen = L"rb";
	else if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
		mode_fopen = L"r+b";
	else if (mode & ZLIB_FILEFUNC_MODE_CREATE)
		mode_fopen = L"wb";
	if (!((filename == NULL) || (mode_fopen == NULL)))
		file = _wfopen((const wchar_t *)filename, mode_fopen);
	return file;
}
#endif

int mymkdir(const QString& dirname)
{
    int ret=0;
#if defined(_WIN32) && defined(HAVE_UNICODE)
	ret = _wmkdir((const wchar_t*) dirname.utf16());
#elif defined(_WIN32)
	QByteArray cdir = dirname.toLocal8Bit();
    ret = _mkdir(cdir.data());
#else
	QByteArray cdir = dirname.toLocal8Bit();
    ret = mkdir (cdir.data(), 0775);
#endif
    return ret;
}

int makedir (const QString& newdir)
{
	QChar p;
	QString buffer;
	int len = newdir.length();
	
	if (len <= 0)
		return 0;
	buffer = newdir;

	if (buffer.endsWith("/") || buffer.endsWith("\\"))
		buffer.left(len - 1);

	if (mymkdir(buffer) == 0)
		return 1;

	uint cIndex = 1;
	p = buffer[1];
	while (1)
	{
		QString tmpBuf;
		while(!p.isNull() && p != '\\' && p != '/')
			p = buffer[cIndex++];
		tmpBuf = buffer.left(cIndex - 1);
		if ((mymkdir(tmpBuf) == -1) && (errno == ENOENT))
			return 0;
		if (p.isNull())
			break;
		p = buffer[++cIndex];
	}
	return 1;
}

int do_extract_onefile(unzFile uf, const QString& filename, int opt_extract_without_path, int opt_overwrite, const char* password)
{
//    int err = UNZ_OK;
	QByteArray fname = filename.toLocal8Bit();
    if (unzLocateFile(uf,fname.data(),CASESENSITIVITY)!=UNZ_OK)
        return 2;

    if (do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password) == UNZ_OK)
        return 0;
    else
        return 1;
}

int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, int*, const char* password)
{
	char  fn_inzip[256];
	QString filename_inzip;
	QString filename_withoutpath;
	int err=UNZ_OK;
	FILE *fout=NULL;
	void* buf;
	uInt size_buf;

	unz_file_info file_info;
	//uLong ratio=0;
	err = unzGetCurrentFileInfo(uf,&file_info,fn_inzip,sizeof(fn_inzip),NULL,0,NULL,0);

	if (err!=UNZ_OK)
		return err;

	size_buf = WRITEBUFFERSIZE;
	buf = (void*)malloc(size_buf);
	if (buf==NULL)
		return UNZ_INTERNALERROR;

	QChar p;
//	uint cIndex = 0;
	filename_inzip = QString::fromLocal8Bit(fn_inzip); 
	filename_withoutpath = filename_inzip;
	for (int i = 0; i < filename_inzip.length(); i++)
	{
		p = filename_inzip[i];
		if( (p == '/') || (p =='\\'))
			filename_withoutpath = filename_inzip.mid(i+1);
	}

	if (filename_withoutpath.isEmpty())
	{
		if ((*popt_extract_without_path)==0)
			mymkdir(filename_inzip);
	}
	else
    {
		QString write_filename;
		int skip=0;

		if ((*popt_extract_without_path)==0)
			write_filename = filename_inzip;
		else
			write_filename = filename_withoutpath;
		
		err = unzOpenCurrentFilePassword(uf,password);

		if ((skip==0) && (err==UNZ_OK))
		{
			fout = openfile(write_filename, "wb");
            /* some zipfile don't contain directory alone before file */
			if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
								(filename_withoutpath != filename_inzip))
			{
				uint len = filename_inzip.length() - filename_withoutpath.length() - 1;
				QString write_dir = filename_inzip.left(len);
				makedir(write_dir);
				fout = openfile(write_filename, "wb");
			}
		}

		if (fout!=NULL)
		{
			do
			{
				err = unzReadCurrentFile(uf,buf,size_buf);
				if (err<0)
					break;
				if (err>0)
					if (fwrite(buf,err,1,fout)!=1)
					{
						err=UNZ_ERRNO;
						break;
					}
			}
			while (err>0);
			if (fout)
				fclose(fout);

			if (err==0)
				change_file_date(write_filename,file_info.dosDate,
									file_info.tmu_date);
		}

		if (err==UNZ_OK)
			err = unzCloseCurrentFile (uf);
		else
			unzCloseCurrentFile(uf); /* don't lose the error */
	}

	free(buf);
	return err;
}

/* change_file_date : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
void change_file_date(const QString& filename, uLong, tm_unz tmu_date)
{
	struct utimbuf ut;
	struct tm newdate;
	newdate.tm_sec = tmu_date.tm_sec;
	newdate.tm_min=tmu_date.tm_min;
	newdate.tm_hour=tmu_date.tm_hour;
	newdate.tm_mday=tmu_date.tm_mday;
	newdate.tm_mon=tmu_date.tm_mon;
	if (tmu_date.tm_year > 1900)
		newdate.tm_year=tmu_date.tm_year - 1900;
	else
		newdate.tm_year=tmu_date.tm_year ;
	newdate.tm_isdst=-1;

	ut.actime=ut.modtime=mktime(&newdate);
#if defined(_WIN32) && defined(HAVE_UNICODE)
	_wutime((const wchar_t*) filename.utf16(), &ut);
#else
	QByteArray fname = filename.toLocal8Bit();
	utime(fname.data(), &ut);
#endif
}
