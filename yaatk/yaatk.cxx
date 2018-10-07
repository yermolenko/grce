/*
   Yet another auxiliary toolkit.

   Copyright (C) 2003, 2005, 2006, 2009, 2010, 2011, 2012, 2013, 2015, 2018
   Alexander Yermolenko <yaa.mbox@gmail.com>

   This file is part of YAATK, Yet another auxiliary toolkit.

   YAATK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   YAATK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with YAATK.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <yaatk/yaatk.hpp>
#include <yaatk/Exception.hpp>

#ifdef YAATK_ENABLE_ZLIB
#include <zlib.h>
#endif

#ifdef YAATK_ENABLE_LIBLZMA
#include <lzma.h>
#endif

#include <cstring>

#include <sstream>
#include <fstream>

#ifdef __WIN32__
  #include <windows.h>
#else
  #include <dirent.h>
#endif

namespace yaatk
{

const size_t Stream::YAATK_ZIP_BUFFER_SIZE = 500000;

Stream::ZipInvokeInfo Stream::zipInvokeInfoGlobal = chooseZipMethod();

std::vector<Stream::ZipInvokeInfo> Stream::zipInvokeInfoList = Stream::initZipInvokeInfoList();

bool
Stream::ZipInvokeInfo::works() const
{
  if (command.find("_internal") != std::string::npos)
  {
#ifdef YAATK_ENABLE_ZLIB
    if (command.find("gzip_internal") != std::string::npos)
      return true;
#endif
#ifdef YAATK_ENABLE_LIBLZMA
    if (command.find("xz_internal") != std::string::npos)
      return true;
#endif

    return false;
  }

  bool itWorks = true;

  const char testdata[] = "ABCD";
  const int resultdata_maxlen = 100;
  char resultdata[resultdata_maxlen];

  FILE* pipe;
  char cmd[2000];sprintf(cmd,"echo %s | %s -c | %s -dc",testdata,command.c_str(),command.c_str());
#ifndef __WIN32__
  pipe = popen(cmd,"r");
#else
  pipe = _popen(cmd,"rb");
#endif

  if(pipe)
  {
    fread(resultdata,1,resultdata_maxlen,pipe);

    if (strncmp(resultdata,testdata,strlen(testdata)))
      itWorks=false;

#ifndef __WIN32__
    int pclose_status = pclose(pipe);
#else
    int pclose_status = _pclose(pipe);
#endif
    if(pclose_status)
      itWorks = false;
  }
  else
    itWorks = false;

  return itWorks;
}

std::vector<Stream::ZipInvokeInfo> 
Stream::initZipInvokeInfoList()
{
  std::vector<ZipInvokeInfo> v;
#ifdef YAATK_ENABLE_LIBLZMA
  v.push_back(ZipInvokeInfo("xz_internal",".xz"));
#endif
  v.push_back(ZipInvokeInfo("xz",".xz"));
  v.push_back(ZipInvokeInfo("bzip2",".bz2"));
#ifdef YAATK_ENABLE_ZLIB
  v.push_back(ZipInvokeInfo("gzip_internal",".gz"));
#endif
  v.push_back(ZipInvokeInfo("gzip",".gz"));
  return v;
}

Stream::ZipInvokeInfo
Stream::chooseZipMethod()
{
  std::vector<Stream::ZipInvokeInfo> zil = Stream::initZipInvokeInfoList();

  for(size_t i = 0; i < zil.size(); i++)
  {
    const ZipInvokeInfo& z = zil[i];
    if (z.works())
      return z;
  }

  return ZipInvokeInfo("nozip","");
}

std::string
Stream::getZippedExt()
{
  return zipInvokeInfo.extension;
}

void
Stream::guessZipTypeByExtension()
{
  for(size_t i = 0; i < zipInvokeInfoList.size(); i++)
  {
    const ZipInvokeInfo& z = zipInvokeInfoList[i];
    if (z.command != "nozip" && 
        filename.find(z.extension) == filename.size()-z.extension.size())
      {
        zipInvokeInfo = z;
        filename = filename.substr(0,filename.size()-z.extension.size());
        return;
      }
  }
  zipInvokeInfo = ZipInvokeInfo("nozip","");
}

void
Stream::guessZipTypeByPresence()
{
  {
    std::ifstream test(getZippedFileName().c_str());
    if (test) return;
  }

  for(size_t i = 0; i < zipInvokeInfoList.size(); i++)
  {
    const ZipInvokeInfo& z = zipInvokeInfoList[i];
    std::ifstream test((filename+z.extension).c_str());
    if (test)
    {
        zipInvokeInfo = z;
        return;
    }
  }
}

Stream::Stream(std::string fname,bool isOutput,bool isBinary)
      :std::stringstream(
isBinary?
(std::stringstream::in | std::stringstream::out | std::stringstream::binary)
:
(std::stringstream::in | std::stringstream::out)
)
      ,filename(fname),output(isOutput),opened(false),zipInvokeInfo(zipInvokeInfoGlobal)
{
  REQUIRE(YAATK_ZIP_BUFFER_SIZE > 0);
  inbuf = new uint8_t[YAATK_ZIP_BUFFER_SIZE];
  outbuf = new uint8_t[YAATK_ZIP_BUFFER_SIZE];
  if (!output) guessZipTypeByExtension();
  if (!output) guessZipTypeByPresence();
  open();
}

Stream::~Stream()
{
  close();
  delete [] inbuf;
  delete [] outbuf;
}

void Stream::open()
{
  if (!opened)
  {
    if (!output) 
      opened = !unZipMe();
    else
      opened = true;
  }
}

void
Stream::close()
{
  if (opened)
  {
    if (output) 
      opened = zipMe(); 
    else
      opened = false;
  }
}

int 
Stream::zipMe()
{
#ifdef YAATK_ENABLE_ZLIB
  if (zipInvokeInfo.command=="gzip_internal") return zipMe_gzip_internal();
#endif
#ifdef YAATK_ENABLE_LIBLZMA
  if (zipInvokeInfo.command=="xz_internal") return zipMe_xz_internal();
#endif
  FILE* zipped;
  if (zipInvokeInfo.command!="nozip")
  {
    char cmd[2000];sprintf(cmd,"%s -c >\"%s\"",zipInvokeInfo.command.c_str(),getZippedFileName().c_str());
#ifndef __WIN32__
    zipped   = popen(cmd,"w");
#else
    zipped   = _popen(cmd,"wb");
#endif
  }
  else
    zipped   = fopen(getZippedFileName().c_str(),"wb");
    
  REQUIRE(zipped != 0);
  int unzippedFileSize;
  while((read((char *)inbuf,YAATK_ZIP_BUFFER_SIZE),unzippedFileSize = gcount()) > 0)
  {
    REQUIRE(unzippedFileSize != -1);
    int bytesWritten    = fwrite(inbuf,1,unzippedFileSize,zipped);
    REQUIRE(unzippedFileSize == bytesWritten);
  }
  REQUIRE(unzippedFileSize == 0);
  if (zipInvokeInfo.command!="nozip")
  {
#ifndef __WIN32__
    int pclose_status = pclose(zipped);
#else
    int pclose_status = _pclose(zipped);
#endif
    REQUIRE(!pclose_status);
  }
  else
  {
    int fclose_status = fclose(zipped);
    REQUIRE(!fclose_status);
  }
  return 0;
}

int
Stream::unZipMe()
{
#ifdef YAATK_ENABLE_ZLIB
  if (zipInvokeInfo.command=="gzip_internal") return unZipMe_gzip_internal();
#endif
#ifdef YAATK_ENABLE_LIBLZMA
  if (zipInvokeInfo.command=="xz_internal") return unZipMe_xz_internal();
#endif
  FILE* zipped;
  if (zipInvokeInfo.command!="nozip")
  {
    char cmd[2000];sprintf(cmd,"%s -dc \"%s\"",zipInvokeInfo.command.c_str(),getZippedFileName().c_str());
#ifndef __WIN32__
    zipped   = popen(cmd,"r");
#else
    zipped   = _popen(cmd,"rb");
#endif
  }
  else
    zipped   = fopen(getZippedFileName().c_str(),"rb");

  if(!zipped) return -1;
  int unzippedFileSize;
  while ((unzippedFileSize = fread(outbuf,1,YAATK_ZIP_BUFFER_SIZE,zipped)) > 0)
  {
    write((char *)outbuf,unzippedFileSize);
  }
  
  REQUIRE(unzippedFileSize == 0);
  if (zipInvokeInfo.command!="nozip")
  {
#ifndef __WIN32__
    int pclose_status = pclose(zipped);
#else
    int pclose_status = _pclose(zipped);
#endif
    if (pclose_status) return -1;
  }
  else
  {
    int fclose_status = fclose(zipped);
    if (fclose_status) return -1;
  }

  return 0;
}

#ifdef YAATK_ENABLE_ZLIB

int
Stream::zipMe_gzip_internal()
{
  gzFile   zipped   = gzopen(getZippedFileName().c_str(),"wb");
  REQUIRE(zipped != 0);
  int unzippedFileSize;
  while((read((char *)inbuf,YAATK_ZIP_BUFFER_SIZE),unzippedFileSize = gcount()) > 0)
  {
    REQUIRE(unzippedFileSize != -1);
    int bytesWritten    = gzwrite(zipped,inbuf,unzippedFileSize);
    REQUIRE(unzippedFileSize == bytesWritten);
  }
  REQUIRE(unzippedFileSize == 0);
  gzclose(zipped);
  return 0;
}

int
Stream::unZipMe_gzip_internal()
{
  gzFile zipped   = gzopen(getZippedFileName().c_str(),"rb");
  if(!zipped) return -1;
  int unzippedFileSize;
  while ((unzippedFileSize = gzread(zipped,outbuf,YAATK_ZIP_BUFFER_SIZE)) > 0)
  {
    write((char *)outbuf,unzippedFileSize);
  }
  REQUIRE(unzippedFileSize == 0);
  gzclose(zipped);
  return 0;
}

#endif

#ifdef YAATK_ENABLE_LIBLZMA

int
Stream::zipMe_xz_internal()
{
  uint32_t preset = 9;
  preset |= LZMA_PRESET_EXTREME;

  lzma_stream strm = LZMA_STREAM_INIT;

  lzma_ret ret = lzma_easy_encoder(&strm, preset, LZMA_CHECK_CRC64);
  if (ret != LZMA_OK)
    return -1;

  lzma_action action = LZMA_RUN;

  strm.next_in = NULL;
  strm.avail_in = 0;
  strm.next_out = outbuf;
  strm.avail_out = YAATK_ZIP_BUFFER_SIZE;

  FILE *outfile = fopen(getZippedFileName().c_str(), "wb");
  if (outfile == NULL)
    return -1;

  while (true)
  {
    if (strm.avail_in == 0 && !eof())
    {
      strm.next_in = inbuf;
      read((char *)inbuf,YAATK_ZIP_BUFFER_SIZE);
      strm.avail_in = gcount();

      if (eof())
        action = LZMA_FINISH;
    }

    lzma_ret ret = lzma_code(&strm, action);

    if (strm.avail_out == 0 || ret == LZMA_STREAM_END)
    {
      size_t write_size = YAATK_ZIP_BUFFER_SIZE - strm.avail_out;

      if (fwrite(outbuf, 1, write_size, outfile) != write_size)
        return -1;

      strm.next_out = outbuf;
      strm.avail_out = YAATK_ZIP_BUFFER_SIZE;
    }

    if (ret != LZMA_OK)
    {
      if (ret == LZMA_STREAM_END)
      {
        lzma_end(&strm);
        fclose(outfile);
        return 0;
      }

      fclose(outfile);
      return -1;
    }
  }

  fclose(outfile);

  return 0;
}

int
Stream::unZipMe_xz_internal()
{
  lzma_stream strm = LZMA_STREAM_INIT;

  lzma_ret ret = lzma_stream_decoder(
    &strm, UINT64_MAX, LZMA_CONCATENATED);
  if (ret != LZMA_OK)
    return -1;

  lzma_action action = LZMA_RUN;

  strm.next_in = NULL;
  strm.avail_in = 0;
  strm.next_out = outbuf;
  strm.avail_out = YAATK_ZIP_BUFFER_SIZE;

  FILE *infile = fopen(getZippedFileName().c_str(), "rb");
  if (infile == NULL)
    return -1;

  while (true)
  {
    if (strm.avail_in == 0 && !feof(infile))
    {
      strm.next_in = inbuf;
      strm.avail_in = fread(inbuf, 1, YAATK_ZIP_BUFFER_SIZE, infile);
      if (ferror(infile))
        return -1;
      if (feof(infile))
        action = LZMA_FINISH;
    }

    lzma_ret ret = lzma_code(&strm, action);

    if (strm.avail_out == 0 || ret == LZMA_STREAM_END)
    {
      size_t write_size = YAATK_ZIP_BUFFER_SIZE - strm.avail_out;
      write((char *)outbuf, write_size);
      strm.next_out = outbuf;
      strm.avail_out = YAATK_ZIP_BUFFER_SIZE;
    }
    if (ret == LZMA_STREAM_END)
    {
      lzma_end(&strm);
      fclose(infile);
      return 0;
    }
    if (ret != LZMA_OK)
    {
      fclose(infile);
      return -1;
    }
  }

  fclose(infile);

  return 0;
}

#endif

std::string extractDir(std::string trajNameFinal)
{
    std::string mde_dirname = trajNameFinal;
    {
      int i;
      for(i = mde_dirname.size()-1; i >= 0; i--)
        if (mde_dirname[i] == DIR_DELIMIT_CHAR) break;
      i++;
      mde_dirname.resize(i);
    }  
    return mde_dirname;
}  

std::string extractLastItem(std::string trajNameFinal)
{
    std::string mde_dirname = trajNameFinal;
    std::string res;
//    std::string x;
    if (mde_dirname[mde_dirname.size()-1]==DIR_DELIMIT_CHAR)
      mde_dirname.resize(mde_dirname.size()-1);
    {
      int i;
      for(i = mde_dirname.size()-1; i >= 0; i--)
        if (mde_dirname[i] == DIR_DELIMIT_CHAR) break;
      i++;
      res = mde_dirname.substr(i,mde_dirname.size()-i);
    }  
    return res;
}  

std::string extractItemFromEnd(std::string trajNameFinal, int fromEnd)
{
    std::string mde_dirname = trajNameFinal;
    std::string res;
    if (mde_dirname[mde_dirname.size()-1]==DIR_DELIMIT_CHAR)
      mde_dirname.resize(mde_dirname.size()-1);
    int delimitCount = 0;
    size_t itemLen = 0;
    {
      int i;
      for(i = mde_dirname.size()-1; i >= 0; i--)
      {
        if (mde_dirname[i] == DIR_DELIMIT_CHAR)
        {
          if (delimitCount == fromEnd) break;
          ++delimitCount;
          itemLen = 0;
        }
        else
          ++itemLen;
      }
      i++;
      res = mde_dirname.substr(i,itemLen);
    }
    return res;
}

std::vector<std::string>
listFilesystemItems(std::string dir, bool listRegularFiles, bool listDirectories)
{
  std::vector<std::string> items;

#ifdef __WIN32__

  WIN32_FIND_DATA ffd;
  char szDir[MAX_PATH];
  HANDLE hFind = INVALID_HANDLE_VALUE;
  DWORD dwError=0;

  strncpy(szDir, dir.c_str(), MAX_PATH);
  strncat(szDir, "\\*", MAX_PATH);

  hFind = FindFirstFile(szDir, &ffd);

  REQUIRE(INVALID_HANDLE_VALUE != hFind);

  do
  {
    if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && listDirectories)
    {
      items.push_back(std::string(ffd.cFileName));
    }

    if ((!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && listRegularFiles)
    {
      items.push_back(std::string(ffd.cFileName));
    }
  }
  while (FindNextFile(hFind, &ffd) != 0);

  dwError = GetLastError();
  REQUIRE(dwError == ERROR_NO_MORE_FILES);

  FindClose(hFind);

#else

  DIR* dirHandle = opendir(dir.c_str());
  REQUIRE(dirHandle != NULL);

  struct dirent* entry = readdir(dirHandle);
  while (entry != NULL)
  {
    if ((entry->d_type == DT_REG && listRegularFiles) ||
        (entry->d_type == DT_DIR && listDirectories))
    {
      items.push_back(std::string(entry->d_name));
    }

    entry = readdir(dirHandle);
  }

  closedir(dirHandle);

#endif

  return items;
}


#define  YAATK_FILECMP_BUFFER_SIZE 10000

#ifdef YAATK_ENABLE_ZLIB
bool
isIdentical(const std::string& file1,const std::string& file2)
{
  char buf1[YAATK_FILECMP_BUFFER_SIZE];
  char buf2[YAATK_FILECMP_BUFFER_SIZE];
  gzFile  f1   = gzopen(file1.c_str(),"rb");
  if (f1 == 0) return false;
  gzFile  f2   = gzopen(file2.c_str(),"rb");
  if (f2 == 0) {gzclose(f1);return false;}
  int f1FileSize, f2FileSize;
  do
  {
    f1FileSize = gzread(f1,buf1,YAATK_FILECMP_BUFFER_SIZE);
    f2FileSize = gzread(f2,buf2,YAATK_FILECMP_BUFFER_SIZE);
    REQUIRE(f1FileSize != -1 && f2FileSize != -1);
    if (f1FileSize != f2FileSize) {gzclose(f1);gzclose(f2);return false;}
    if (f1FileSize == 0 && f2FileSize == 0) break;
    if (std::memcmp(buf1,buf2,f1FileSize)) {gzclose(f1);gzclose(f2);return false;}
  }while(1);
  REQUIRE(f1FileSize != -1 && f2FileSize != -1);
  gzclose(f1);
  gzclose(f2);  
  return true;
}  
#endif

const std::string DataState::flagFilename = "flag.write-is-in-progress";
size_t DataState::flagRequestCount = 0;

}


