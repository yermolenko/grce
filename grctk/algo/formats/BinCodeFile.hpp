/*
  The BinCodeFileReader/Writer classes (header file).

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

#ifndef grctk_BinCodeFile_hpp
#define grctk_BinCodeFile_hpp

#include "grctk/AdjMatrix.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace grctk
{

class BinCodeFileReader
{
  std::ifstream in;
  unsigned short nv, ne;
  unsigned long ng, checksum;
  void parseHeader();
  unsigned long getChecksum();
public:
  BinCodeFileReader(const std::string filename);
  virtual ~BinCodeFileReader() {}

  unsigned short numberOfVertices() { return nv; }
  unsigned short numberOfEdges() { return ne; }
  unsigned long numberOfGraphs() { return ng; }

  void listGraphs(std::vector<std::string>&);
  AdjMatrix getGraph(unsigned long i);

  bool checksumIsCorrect() {return getChecksum() == checksum; }
private:
  BinCodeFileReader(const BinCodeFileReader&);
  BinCodeFileReader& operator=(const BinCodeFileReader&);
};

class BinCodeFileWriter
{
  std::ofstream out;
  unsigned short nv, ne;
  unsigned long ng, checksum;
  void writeHeader();
public:
  BinCodeFileWriter(const std::string filename);
  virtual ~BinCodeFileWriter();

  static std::string proposedBasename(const AdjMatrix& g,
                                      bool accountEdges = true);

  unsigned short numberOfVertices() { return nv; }
  unsigned short numberOfEdges() { return ne; }
  unsigned long numberOfGraphs() { return ng; }
  unsigned long getChecksum() { return checksum; }

  void addGraph(const AdjMatrix&);
private:
  BinCodeFileWriter(const BinCodeFileWriter&);
  BinCodeFileWriter& operator=(const BinCodeFileWriter&);
};

}

#endif
