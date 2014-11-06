/**
 * @file EASEAClientRow.hpp
 * @author Pallamidessi Joseph
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
**/  
#include <iostream>
#include <cstdio>
#include <cstdlib>

class EASEAClientRow {
public:
  EASEAClientRow ():mBest(0),mWorst(0),mStdev(0),mAverage(0){}
  EASEAClientRow (float best, float worst,float average,float stdev)
                  :mBest(best),mWorst(worst),mStdev(stdev),mAverage(average){}
  virtual ~EASEAClientRow (){}
  
  float best(){
    return mBest;
  }

  float worst(){
    return mWorst;
  }

  float stdev(){
    return mStdev;
  }

  float average(){
    return mAverage;
  }

private:
  /* data */
  float mBest;
  float mWorst;
  float mStdev;
  float mAverage;
};

