/*
   Yet another auxiliary toolkit (header file).

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

#ifndef yaatk_hpp
#define yaatk_hpp

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <yaatk/Exception.hpp>
#include <yaatk/config.hpp>

#include <cstdio>
#include <stdexcept>
#include <sstream>

#ifdef _MSC_VER
  typedef unsigned char uint8_t;
  typedef int int32_t;
  typedef unsigned int uint32_t;
#else
  #include <stdint.h>
#endif

#ifndef __WIN32__
#include <unistd.h>
#include <errno.h>
#define _open open
#define _close close
#define _O_RDONLY O_RDONLY
#define _O_BINARY 0
#define _filelength(HANDLE) lseek(HANDLE,0,SEEK_END)
#endif

#ifdef __WIN32__
  #include <direct.h>
#else
  #include <sys/stat.h>
#endif

#include <fcntl.h>
#ifdef __WIN32__
#include <io.h>
#else
#endif  

#include <ctime>

enum YAATK_FSTREAM_MODE {YAATK_FSTREAM_TEXT = 0, YAATK_FSTREAM_BIN = 1};

namespace yaatk
{

#define MPI_TEST_SUCCESS(x) \
{ \
  int mpi_err_code = (x);\
  if (mpi_err_code!=MPI_SUCCESS) \
  { \
    int rank; \
    std::cerr << "*** SIMPLE_MPI_ERROR; At" << #x << std::endl << std::flush; \
    int local_rc = MPI_Comm_rank(MPI_COMM_WORLD,&rank); \
    if (local_rc == MPI_SUCCESS) \
      std::cerr << "*** MPI_ERROR; rank = " << rank << ". At" << #x << std::endl << std::flush; \
    else \
      std::cerr << "*** MPI_ERROR; At" << #x << std::endl << std::flush; \
    throw MPI_Exception(mpi_err_code); \
  } \
};

#define REQUIREM(cond,msg) \
{ \
  if (!(cond)) \
  { \
    std::cerr << __FILE__ << "(" << __LINE__ << "): "  \
              << (msg) << std::endl << std::flush;     \
    throw std::logic_error(msg);                       \
  }  \
}

#define REQUIRE(cond) \
{ \
  if (!(cond)) \
  { \
    std::cerr << __FILE__ << "(" << __LINE__ << "): "      \
              << "Assertion "  << (#cond) << " FAILED !!!" \
              << std::endl << std::flush;                  \
    throw std::logic_error((#cond));                       \
  }  \
}

#define REQUIRE_SILENT(cond) \
{ \
  if (!(cond)) \
  { \
    throw std::logic_error((#cond));            \
  }  \
}

#define TRACE_EMPH(x) { std::cout << #x << " : " << "<<<##  " << (x) << "" << std::endl; }

#define FPRINT(x) { std::cout << x << std::flush; }
#define FEPRINT(x) { std::cerr << x << std::flush; }
#define FTRACE(x) { std::cout << #x << " : " << (x) << std::endl; }
#define FETRACE(x) { std::cerr << #x << " : " << (x) << std::endl; }

#ifdef YAATK_PARALLEL
#define PRINT(x) { if (yaatk::verboseTrace && yaatk::mpi_comm_rank==0) std::cout << x << std::flush; }
#define EPRINT(x) { if (yaatk::verboseTrace && yaatk::mpi_comm_rank==0) std::cerr << x << std::flush; }
#define TRACE(x) { if (yaatk::verboseTrace && yaatk::mpi_comm_rank==0) std::cout << #x << " : " << (x) << std::endl; }
#define ETRACE(x) { if (yaatk::verboseTrace && yaatk::mpi_comm_rank==0) std::cerr << #x << " : " << (x) << std::endl; }
#define VPRINT(x) { if (yaatk::mpi_comm_rank==0) std::cout << x << std::flush; }
#define VEPRINT(x) { if (yaatk::mpi_comm_rank==0) std::cerr << x << std::flush; }
#define VTRACE(x) { if (yaatk::mpi_comm_rank==0) std::cout << #x << " : " << (x) << std::endl; }
#define VETRACE(x) { if (yaatk::mpi_comm_rank==0) std::cerr << #x << " : " << (x) << std::endl; }
#else
#define PRINT(x) { if (yaatk::verboseTrace) std::cout << x << std::flush; }
#define EPRINT(x) { if (yaatk::verboseTrace) std::cerr << x << std::flush; }
#define TRACE(x) { if (yaatk::verboseTrace) std::cout << #x << " : " << (x) << std::endl; }
#define ETRACE(x) { if (yaatk::verboseTrace) std::cerr << #x << " : " << (x) << std::endl; }
#define VPRINT(x) { std::cout << x << std::flush; }
#define VEPRINT(x) { std::cerr << x << std::flush; }
#define VTRACE(x) { std::cout << #x << " : " << (x) << std::endl; }
#define VETRACE(x) { std::cerr << #x << " : " << (x) << std::endl; }
#endif


#define TRACESS(x) << #x << " : " << (x) << std::endl;
#define TRACESS_NO_ENDL(x,delim) << #x << " : " << (x) << delim;


#define YAATK_BIN_WRITE(FSTREAM_INST,VAR_INST) \
  FSTREAM_INST.write((char*)&(VAR_INST),sizeof(VAR_INST))

#define YAATK_BIN_READ(FSTREAM_INST,VAR_INST) \
  FSTREAM_INST.read((char*)&(VAR_INST),sizeof(VAR_INST))

#define YAATK_FSTREAM_WRITE(FSTREAM_INST,VAR_INST,SMODE) \
  if (SMODE == YAATK_FSTREAM_TEXT) {FSTREAM_INST << VAR_INST << "\n";} else YAATK_BIN_WRITE(FSTREAM_INST, VAR_INST);

#define YAATK_FSTREAM_WRITE_NONL(FSTREAM_INST,VAR_INST,SMODE) \
  if (SMODE == YAATK_FSTREAM_TEXT) {FSTREAM_INST << VAR_INST;} else YAATK_BIN_WRITE(FSTREAM_INST, VAR_INST);

#define YAATK_FSTREAM_READ(FSTREAM_INST,VAR_INST,SMODE) \
  {                                                                     \
  if (SMODE == YAATK_FSTREAM_TEXT) {FSTREAM_INST >> VAR_INST;} else YAATK_BIN_READ (FSTREAM_INST, VAR_INST); \
  if (FSTREAM_INST.fail()) throw std::runtime_error("Error in reading variable "#VAR_INST); \
  }

inline
int mkdir(const char *name)
{
#ifdef __WIN32__
  return ::_mkdir(name);
#else
  return ::mkdir(name,S_IRWXU);
#endif
}

inline
int chdir(const char *name)
{
#ifdef __WIN32__
  return ::_chdir(name);
#else
  return ::chdir(name);
#endif
}

inline
std::string getcwd()
{
  const int maxlen = 1000;
  char dir[maxlen];
  char* getcwd_retval;
#ifdef __WIN32__
  getcwd_retval = ::_getcwd(dir,maxlen);
#else
  getcwd_retval = ::getcwd(dir,maxlen);
#endif
  REQUIRE(getcwd_retval);
  return std::string(dir);
}

inline
int remove(const char *name)
{
  return std::remove(name);
}

inline
int remove(const std::string name)
{
//  std::cout << "Removing file " << name.c_str() << std::endl;
  return std::remove(name.c_str());
}

inline
int rename(const char *oldname, const char *newname)
{
  return std::rename(oldname,newname);
}

inline
int rename(const std::string oldname, const std::string newname)
{
  return std::rename(oldname.c_str(),newname.c_str());
}

std::vector<std::string>
listFilesystemItems(std::string dir, bool listRegularFiles, bool listDirectories);

inline
std::vector<std::string>
listFiles(std::string dir)
{
  return listFilesystemItems(dir, true, false);
}

inline
std::vector<std::string>
listDirectories(std::string dir)
{
  return listFilesystemItems(dir, false, true);
}

inline
std::vector<std::string>
listFilesAndDirectories(std::string dir)
{
  return listFilesystemItems(dir, true, true);
}


#define DIR_DELIMIT_CHAR '/'
#define DIR_DELIMIT_STR "/"


  class Stream : public std::stringstream
  {
    static const size_t YAATK_ZIP_BUFFER_SIZE;
    uint8_t* inbuf;
    uint8_t* outbuf;
    int zipMe();
    int unZipMe();
#ifdef YAATK_ENABLE_ZLIB
    int zipMe_gzip_internal();
    int unZipMe_gzip_internal();
#endif
#ifdef YAATK_ENABLE_LIBLZMA
    int zipMe_xz_internal();
    int unZipMe_xz_internal();
#endif
    std::string filename;
    bool output;
    bool opened;
  public:
    bool isOpened() {return opened;}
    struct ZipInvokeInfo
    {
      std::string command;
      std::string extension;
      ZipInvokeInfo(std::string c, std::string e)
        :command(c),extension(e){}
      bool works() const;
    };
    static std::vector<ZipInvokeInfo> zipInvokeInfoList;
    static std::vector<ZipInvokeInfo> initZipInvokeInfoList();
    static ZipInvokeInfo chooseZipMethod();
    std::string getZippedExt();
    std::string getFileName() {return filename;}
    void guessZipTypeByExtension();
    void guessZipTypeByPresence();
    void setFileName(std::string fname) {filename = fname;}
    void setOutputMode(bool om) {output = om;}
    std::string getZippedFileName() {return filename+getZippedExt();}
    static ZipInvokeInfo zipInvokeInfoGlobal;
    ZipInvokeInfo zipInvokeInfo;
    Stream(std::string fname,bool isOutput,bool isBinary);
    virtual ~Stream();
    void open();
    void close();
  };



  class binary_fstream : public Stream
  {
  public:
    binary_fstream(std::string fname,bool isOutput)
      :Stream(fname,isOutput,true) {}
    virtual ~binary_fstream() {}
  };

  class binary_ifstream : public binary_fstream
  {
  public:
    binary_ifstream(std::string fname)
      :binary_fstream(fname,false) {}
    virtual ~binary_ifstream() {}
    int getDataLength()
      {
        seekg(0, end);
        int length = tellg();
        seekg(0, beg);
        return length;
      }
  };

  class binary_ofstream : public binary_fstream
  {
  public:
    binary_ofstream(std::string fname)
      :binary_fstream(fname,true) {}
    virtual ~binary_ofstream() {}
  };


  class text_fstream : public Stream
  {
  public:
    text_fstream(std::string fname,bool isOutput)
      :Stream(fname,isOutput,false) {}
    virtual ~text_fstream() {}
  };

  class text_ifstream : public text_fstream
  {
  public:
    text_ifstream(std::string fname)
      :text_fstream(fname,false) {}
    virtual ~text_ifstream() {}
  };

  class text_ofstream : public text_fstream
  {
  public:
    text_ofstream(std::string fname)
      :text_fstream(fname,true) {}
    virtual ~text_ofstream() {}
  };

inline
bool exists(std::string filename)
{
  bool retval = false;
  try
  {
    yaatk::binary_ifstream fi(filename);
    if (fi.isOpened()) {fi.close(); retval = true;}
  }catch (...) {;}
  return retval;
}

std::string extractDir(std::string trajNameFinal);
std::string extractLastItem(std::string trajNameFinal);
std::string extractItemFromEnd(std::string trajNameFinal, int fromEnd = 0);

#ifdef YAATK_ENABLE_ZLIB
bool
isIdentical(const std::string& file1,const std::string& file2);
#endif

struct StreamToFileRedirect
{
  std::ostream& stream;
  std::string filename;
  std::streambuf* origstream_sbuf;
  std::ofstream filestream;
  StreamToFileRedirect(std::ostream& s = std::cout,
                       std::string fname = "stdout.txt")
    :stream(s),
     filename(fname),
     origstream_sbuf(stream.rdbuf()),
     filestream(filename.c_str(),std::ios::app)
    {
      stream.rdbuf(filestream.rdbuf());
    }
  ~StreamToFileRedirect()
    {
      filestream.close();
      stream.rdbuf(origstream_sbuf);
    }
};

struct ChDir
{
  yaatk::StreamToFileRedirect* cout_redir;
  yaatk::StreamToFileRedirect* cerr_redir;
  std::string prevDir;
  ChDir(std::string dir = "_tmp", bool stdStreamsRedirect = true)
    :cout_redir(NULL),
     cerr_redir(NULL),
     prevDir(yaatk::getcwd())
    {
      yaatk::mkdir(dir.c_str());
      int chdir_retval = yaatk::chdir(dir.c_str());
      REQUIRE(chdir_retval == 0);
      if (stdStreamsRedirect)
      {
        cout_redir = new yaatk::StreamToFileRedirect(std::cout,"stdout.txt");
        cerr_redir = new yaatk::StreamToFileRedirect(std::cerr,"stderr.txt");
      }
    }
  ~ChDir()
    {
      int chdir_retval = yaatk::chdir(prevDir.c_str());
      REQUIRE(chdir_retval == 0);
      if (cout_redir != NULL)
        delete cout_redir;
      if (cerr_redir != NULL)
        delete cerr_redir;
    }
};

class DataState
{
  static const std::string flagFilename;
  static size_t flagRequestCount;
public:
  DataState()
    {
      std::ifstream flagtest(flagFilename.c_str());
      if (!flagtest)
      {
        std::ofstream flag(flagFilename.c_str());
        flag.close();
      }
      ++flagRequestCount;
    }
  ~DataState()
    {
      --flagRequestCount;
      std::ifstream flagtest(flagFilename.c_str());
      REQUIRE(flagtest);
      if (flagtest && flagRequestCount == 0)
      {
        flagtest.close();
        int retval = yaatk::remove(flagFilename);
        if (retval)
          throw std::runtime_error("Cannot remove flag file");
      }
    }
  static bool isClean()
    {
      std::ifstream flagtest(flagFilename.c_str());
      if (flagtest)
        return false;
      return true;
    }
};

class StreamMod
{
  std::ostream& stream2modify;
  char fill_prev;
  std::streamsize width_prev;
  std::streamsize precision_prev;
public:
  StreamMod(std::ostream& stream,
                char fill,
                std::streamsize width,
                std::streamsize precision)
    :stream2modify(stream)
    {
      fill_prev = stream2modify.fill(fill);
      width_prev = stream2modify.width(width);
      precision_prev = stream2modify.precision(precision);
    }
  StreamMod(std::ostream& stream,
                char fill,
                std::streamsize width)
    :stream2modify(stream)
    {
      fill_prev = stream2modify.fill(fill);
      width_prev = stream2modify.width(width);
      precision_prev = stream2modify.precision();
    }
  StreamMod(std::ostream& stream,
                char fill)
    :stream2modify(stream)
    {
      fill_prev = stream2modify.fill(fill);
      width_prev = stream2modify.width();
      precision_prev = stream2modify.precision();
    }
  ~StreamMod()
    {
      stream2modify.precision(precision_prev);
      stream2modify.width(width_prev);
      stream2modify.fill(fill_prev);
    }
};

#define PRINT2STREAM_FWP(stream,x,fill,width,precision)         \
  {                                                             \
    yaatk::StreamMod smod(stream, fill, width, precision);      \
    stream << x << std::flush;                                  \
  }

#define PRINT2STREAM_FW(stream,x,fill,width)                 \
  {                                                          \
    yaatk::StreamMod smod(stream, fill, width);              \
    stream << x << std::flush;                               \
  }

#define PRINT2STREAM_P(stream,x,precision)                              \
  {                                                                     \
    yaatk::StreamMod smod(stream,                                       \
                          stream.fill(), stream.width(),                \
                          precision);                                   \
    stream << x << std::flush;                                          \
  }

inline
std::string
getDateTime_YYYYMMDD_HHMMSS()
{
  std::time_t rawtime;
  std::time(&rawtime);
  std::tm *ptm = gmtime(&rawtime);

  std::ostringstream osstime;
  PRINT2STREAM_FW(osstime,(ptm->tm_year + 1900),'0',4);
  PRINT2STREAM_FW(osstime,(ptm->tm_mon + 1),'0',2);
  PRINT2STREAM_FW(osstime,ptm->tm_mday,'0',2);
  osstime << "-";
  PRINT2STREAM_FW(osstime,ptm->tm_hour,'0',2);
  PRINT2STREAM_FW(osstime,ptm->tm_min,'0',2);
  PRINT2STREAM_FW(osstime,ptm->tm_sec,'0',2);
  return osstime.str();
}

inline
bool
isOption(const std::string arg, const std::string longOption, const char shortOption = '\0')
{
  bool is = false;
  if (arg == std::string("--") + longOption)
  {
    is = true;
  }
  if (shortOption != '\0' &&
      arg.size() >= 2 &&
      arg[0] == '-' && arg[1] != '-' &&
      arg.find(shortOption) != std::string::npos)
  {
    is = true;
  }
  return is;
}

}

#endif



