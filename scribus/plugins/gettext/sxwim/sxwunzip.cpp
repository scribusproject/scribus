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

#include "sxwunzip.h"

#ifdef HAVE_XML

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

# include <unistd.h>
# include <utime.h>

#include <iostream>
#include "unzip.h"
#include <qdir.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* password);
int do_extract_onefile(unzFile uf, const char* filename, int opt_extract_without_path, int opt_overwrite, const char* password);
int mymkdir(const char* dirname);
int makedir (char *newdir);
void change_file_date(const char* filename, uLong dosdate, tm_unz tmu_date);

SxwUnzip::SxwUnzip(QString zipFilePath)
{
	zipFile = zipFilePath;
}

QString SxwUnzip::getFile(QString name)
{
	QString pwd = QDir::currentDirPath();
	QString outDir = zipFile.left(zipFile.findRev("/") + 1);
	QFile f(outDir);
	QFileInfo fi(f);
	if (!fi.isWritable())
		outDir = QDir::homeDirPath() + "/.scribus/";
	QDir::setCurrent(outDir);
	const char *zipfilename = zipFile.ascii();
	const char *filename_to_extract = name.ascii();
	unzFile uf = unzOpen(zipfilename);;
	int ret = do_extract_onefile(uf,filename_to_extract, 0, 1, NULL);
	unzCloseCurrentFile(uf);
	QDir::setCurrent(pwd);
	if (ret != 0)
		return NULL;
	else
		return outDir + name;
}

SxwUnzip::~SxwUnzip()
{

}


int do_extract_onefile(unzFile uf, const char* filename, int opt_extract_without_path, int opt_overwrite, const char* password)
{
//    int err = UNZ_OK;
    if (unzLocateFile(uf,filename,CASESENSITIVITY)!=UNZ_OK)
        return 2;

    if (do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password) == UNZ_OK)
        return 0;
    else
        return 1;
}

int do_extract_currentfile(unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* password)
{
    char filename_inzip[256];
    char* filename_withoutpath;
    char* p;
    int err=UNZ_OK;
    FILE *fout=NULL;
    void* buf;
    uInt size_buf;

    unz_file_info file_info;
//    uLong ratio=0;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

    if (err!=UNZ_OK)
        return err;

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
        return UNZ_INTERNALERROR;

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }

    if ((*filename_withoutpath)=='\0')
    {
        if ((*popt_extract_without_path)==0)
            mymkdir(filename_inzip);
    }
    else
    {
        char* write_filename;
        int skip=0;

        if ((*popt_extract_without_path)==0)
            write_filename = filename_inzip;
        else
            write_filename = filename_withoutpath;

        err = unzOpenCurrentFilePassword(uf,password);

        if ((skip==0) && (err==UNZ_OK))
        {
            fout=fopen(write_filename,"wb");

            /* some zipfile don't contain directory alone before file */
            if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
                                (filename_withoutpath!=(char*)filename_inzip))
            {
                char c=*(filename_withoutpath-1);
                *(filename_withoutpath-1)='\0';
                makedir(write_filename);
                *(filename_withoutpath-1)=c;
                fout=fopen(write_filename,"wb");
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

int mymkdir(char* dirname)
{
    int ret=0;
    ret = mkdir (dirname,0775);
    return ret;
}

int makedir (const char *newdir)
{
  char *buffer ;
  char *p;
  int  len = (int)strlen(newdir);

  if (len <= 0)
    return 0;

  buffer = (char*)malloc(len+1);
  strcpy(buffer,newdir);

  if (buffer[len-1] == '/') {
    buffer[len-1] = '\0';
  }
  if (mymkdir(buffer) == 0)
    {
      free(buffer);
      return 1;
    }

  p = buffer+1;
  while (1)
    {
      char hold;

      while(*p && *p != '\\' && *p != '/')
        p++;
      hold = *p;
      *p = 0;
      if ((mymkdir(buffer) == -1) && (errno == ENOENT))
        {
          free(buffer);
          return 0;
        }
      if (hold == 0)
        break;
      *p++ = hold;
    }
  free(buffer);
  return 1;
}

/* change_file_date : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
void change_file_date(const char* filename, uLong dosdate, tm_unz tmu_date)
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
  utime(filename,&ut);
}

#endif // HAVE_XML

