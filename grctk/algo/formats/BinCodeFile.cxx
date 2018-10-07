/*
  The BinCodeFileReader/Writer classes.

  Copyright (C) 2003, 2004, 2015, 2018 Alexander Yermolenko
  <yaa.mbox@gmail.com>

  This file is part of GRCE, the Graph Research and Computing Environment.

  GRCE is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  GRCE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GRCE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "BinCodeFile.hpp"
#include "BinCode.hpp"
#include <sstream>
#include <stdexcept>

#define BINCODE_HEADER_LENGTH (2+2+4+4)

namespace grctk
{

void
BinCodeFileReader::parseHeader()
{
  in.seekg(0, std::ios::beg);

  in.read((char*)&nv,2);
  in.read((char*)&ne,2);
  in.read((char*)&ng,4);
  in.read((char*)&checksum,4);
}

unsigned long
BinCodeFileReader::getChecksum()
{
  unsigned long nChecksum = 0;

  for(size_t i = 0; i < ng; ++i)
  {
    BinCode bc(nv);
    in.seekg(BINCODE_HEADER_LENGTH + bc.getLen()*i, std::ios::beg);
    bc.getGraph(in);
    nChecksum += bc.checksum();
    in.seekg(BINCODE_HEADER_LENGTH, std::ios::beg);
  }

  nChecksum &= 0xffffffff;

  return nChecksum;
}

BinCodeFileReader::BinCodeFileReader(const std::string filename):
  in(),
  nv(0),ne(0),
  ng(0),checksum(0)
{
  in.open(filename.c_str(), std::ios::in | std::ios::binary);
  if (!in)
    throw std::runtime_error("BinCodeFileReader: opening file failed!");

  parseHeader();
}

void
BinCodeFileReader::listGraphs(std::vector<std::string>& items)
{
  items.resize(0);
  for(size_t i = 0; i < ng; i++)
  {
    std::ostringstream s;
    s << i;
    items.push_back(s.str());
  }
}

AdjMatrix
BinCodeFileReader::getGraph(unsigned long i)
{
  AdjMatrix g;
  if (/*i >=0 &&*/ i < ng)
  {
    BinCode bc(nv);
    in.seekg(BINCODE_HEADER_LENGTH + bc.getLen()*i, std::ios::beg);
    g.cloneFrom(bc.getGraph(in));
    in.seekg(BINCODE_HEADER_LENGTH, std::ios::beg);
  }
  else
    throw std::runtime_error("BinCodeFileReader: wrong graph index");

  return g;
}

void
BinCodeFileWriter::writeHeader()
{
  out.seekp(0, std::ios::beg);

  out.write((char*)&nv,2);
  out.write((char*)&ne,2);
  out.write((char*)&ng,4);
  out.write((char*)&checksum,4);
}

void
BinCodeFileWriter::addGraph(const AdjMatrix& g)
{
  if (ng == 0)
  {
    REQUIRE(nv == 0 && ne == 0);
    nv = g.size();
    ne = 0;
    for(size_t i = 0; i < g.size(); ++i)
      for(size_t j = i+1; j < g.size(); ++j)
        if (g.s(i,j))
          ++ne;
  }
  else
  {
    REQUIRE(nv == g.size());
  }

  BinCode bc(nv);
  out.seekp(BINCODE_HEADER_LENGTH + bc.getLen()*ng, std::ios::beg);
  bc.writeGraph(out, g);
  out.seekp(BINCODE_HEADER_LENGTH, std::ios::beg);

  ng++;
  checksum += bc.checksum();

  checksum &= 0xffffffff;
}

BinCodeFileWriter::BinCodeFileWriter(const std::string filename):
  out(),
  nv(0),ne(0),
  ng(0),checksum(0)
{
  out.open(filename.c_str(), std::ios::out | std::ios::binary);
  if (!out)
    throw std::runtime_error("BinCodeFile::opening file failed!");
}

BinCodeFileWriter::~BinCodeFileWriter()
{
  writeHeader();
}

std::string
BinCodeFileWriter::proposedBasename(const AdjMatrix& g, bool accountEdges)
{
  unsigned short nv = g.size();

  std::ostringstream os;
  if (nv < 10)
    os << "0";
  os << nv << "V";
  if (nv > 0 && accountEdges)
    os << g.vertexDegree(0) << "P";
  return os.str();
}

}
