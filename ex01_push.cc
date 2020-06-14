/***********************************************************************
* Copyright(C) 2020 - LBS - (Single person developer.)                 *
* Tue Mar 24 11:53:15 CET 2020                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include <libpushVectorInRoot.h>

//c, c++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <vector>

//root
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>

int main(int argc, char *argv[]) {
  int nargcMin = 5;
  if(argc > nargcMin && atoi(argv[1]) == 0 ){
    TString outRootFile = argv[2];
    int vecparID = atoi(argv[3]);
    int nv = atoi(argv[4]);
    //std::cout<<"nv "<<nv<<std::endl;
    std::vector<float> v;
    for(int i = 0; i < nv; i++){
      v.push_back(atof(argv[nargcMin+i]));
      //std::cout<<atof(argv[nargcMin+i])<<std::endl;
    }
    int ut = time(0);
    pushVectorInRoot ( outRootFile, vecparID, ut, v);
  }
  else{
    std::cout<<" ---> ERROR in input arguments "<<std::endl;
    assert(0);
  }
  return 0;
}
