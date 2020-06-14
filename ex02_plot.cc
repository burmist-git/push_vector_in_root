/***********************************************************************
* Copyright(C) 2020 - LBS - (Single person developer.)                 *
* Tue Mar 24 17:56:37 CET 2020                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//my
#include "libpushVectorInRoot.h"

//root
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TCanvas.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){
  if(argc == 4 && atoi(argv[1])==0){
    TString rootFileIn = argv[2];
    TString vecNamesFile = argv[3];
    Int_t saveKey = 1;
    std::vector<TGraph*> gr_v = getGraphsVector (rootFileIn, vecNamesFile, saveKey);
    TCanvas *c1 = new TCanvas("c1","c1",10,10,1200,800);
    gStyle->SetPalette(1);
    gStyle->SetFrameBorderMode(0);
    gROOT->ForceStyle();
    gStyle->SetStatColor(kWhite);
    c1->SetRightMargin(0.12);
    c1->SetLeftMargin(0.12);
    c1->SetTopMargin(0.1);
    c1->SetBottomMargin(0.15);
    c1->SetGrid();
    gr_v.at(0)->SetLineColor(kRed);
    gr_v.at(0)->SetMarkerColor(kRed);
    gr_v.at(0)->SetMarkerStyle(20);
    gr_v.at(0)->SetLineStyle(kSolid);
    gr_v.at(0)->Draw("APL");
    gr_v.at(0)->GetYaxis()->SetTitle("v1, v1");  
    gr_v.at(0)->GetXaxis()->SetTimeDisplay(1);
    gr_v.at(0)->GetXaxis()->SetTimeFormat("#splitline{%m/%d}{%H:%M}%F1970-01-01 00:00:00");
    Double_t timeAxisLabelOffset = 0.02;
    gr_v.at(0)->GetXaxis()->SetLabelOffset(timeAxisLabelOffset);
    TString pdfOutFile = rootFileIn;
    pdfOutFile += ".pdf";
    c1->SaveAs(pdfOutFile.Data());
  }
  else{
    cout<<" --> ERROR in input arguments "<<endl
	<<" runID [1] = 0 (execution ID number)"<<endl
      	<<"       [2] - rootFileIn"<<endl
	<<"       [3] - vecNamesFile"<<endl;
  }
  return 0;
}
