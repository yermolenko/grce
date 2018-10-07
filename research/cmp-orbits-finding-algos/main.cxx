/*
  Compare orbits finding algorithms in batch.

  Copyright (C) 2003, 2004, 2017, 2018 Alexander Yermolenko
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

#include "grctk/AdjMatrix.hpp"
#include "grctk/algo/formats/BinCodeFile.hpp"
#include "grctk/algo/orbits/CmpSubgraphIsoAndEdgeTensions.hpp"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>
#include <exception>

using namespace std;
using namespace yaatk;
using namespace grctk;

int main(int argc, char *argv[])
{
  try
  {
    CmpSubgraphIsoAndEdgeTensions cmpAlgorithms;
    Attribute<int> aOrbitSubgraphIso;
    Attribute<Rational> aOrbitEdgeTensions;

    for(int i = 1; i < argc; i++)
    {
      std::string filename = argv[i];
      cerr << "Processing file : " << filename << endl;

      ofstream fo((filename + "-diff-pc").c_str());

      vector<AdjMatrix> failedGraphs;
      double a_pc = 0.0, a_vpc = 0.0;

      BinCodeFileReader bicfile(filename.c_str());
      cerr << "Checksum is "
           << (bicfile.checksumIsCorrect()?"":"NOT ")
           << "correct" << endl;
      cerr << "Total number of graphs : "
           << bicfile.numberOfGraphs() << endl;
      for(unsigned long gindex = 0;
          gindex < bicfile.numberOfGraphs(); gindex++)
      {
        AdjMatrix g = bicfile.getGraph(gindex);

        if (!cmpAlgorithms(g,aOrbitSubgraphIso,aOrbitEdgeTensions))
        {
          failedGraphs.push_back(g);

          size_t o1 = CmpSubgraphIsoAndEdgeTensions::getOrbitsCount(g,aOrbitSubgraphIso);
          size_t o2 = CmpSubgraphIsoAndEdgeTensions::getOrbitsCount(g,aOrbitEdgeTensions);
          double pc = double(o2)/double(o1);
          double vpc = double(o2-1)/double(o1-1);

          fo << setw(10) << o1 << " " << setw(10) << o2 << " "
             << setw(10) << pc << " " << setw(10) << vpc << endl;
          a_pc += pc;
          a_vpc += vpc;
        }
      }

      cerr << "Number of graphs on which the algorithms results differ: "
           << failedGraphs.size() << endl;

      if (failedGraphs.size() > 0)
      {
        a_pc /= failedGraphs.size();
        a_vpc /= failedGraphs.size();
        fo << "----\n" << setw(10) << a_pc << " " << setw(10) << a_vpc << endl;

        BinCodeFileWriter checkList(
          BinCodeFileWriter::proposedBasename(failedGraphs[0]) + "-check.R");

        for(size_t fgi = 0; fgi < failedGraphs.size(); fgi++)
          checkList.addGraph(failedGraphs[fgi]);
      }

      fo.close();
    }
  }
  catch(exception& e)
  {
    cerr << e.what() << endl;
  }
  catch(...)
  {
    cerr << "Unknown exception" << endl;
  }

  return 0;
}
