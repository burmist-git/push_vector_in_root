/***********************************************************************
* Copyright(C) 2019 - LBS - (Single person developer.)                 *
* Wed May  1 13:45:18 JST 2019                                         *
* Autor: Leonid Burmistrov                                             *
***********************************************************************/

//
#include "libpushVectorInRoot.h"

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
#include <TTree.h>
#include <TFile.h>
#include <TGraph.h>

void pushVectorInRoot (TString outRootFile, int vecparID, int unixTimeInt, const std::vector<float>& val) {
  //
  Int_t vpID = vecparID;
  Int_t ut = unixTimeInt;
  std::vector<float> *v = new std::vector<float>;
  for(unsigned i = 0; i < val.size(); i++)
    v->push_back(val.at(i));
  //
  TFile *hfile = new TFile( outRootFile, "UPDATE", "Data vector with unix timestamp", 1);
  if (hfile->IsZombie()) {
    std::cout << "PROBLEM with the initialization of the output ROOT ntuple " 
         << outRootFile << ": check that the path is correct!!!"
         <<std::endl;
    assert(0);
  }
  //
  TTree *tree;
  if(!(TTree*)hfile->Get("T")){
    //std::cout<<" FIRST TIME"<<std::endl;
    tree = new TTree("T", "Data tree");
    tree->Branch("vpID",&vpID, "vpID/I");
    tree->Branch("ut",&ut, "ut/I");
    tree->Branch("v",v);
  }
  else {
    //std::cout<<" NOT FIRST TIME"<<std::endl;
    tree = (TTree*)hfile->Get("T");
    tree->SetBranchAddress("vpID", &vpID);
    tree->SetBranchAddress("ut", &ut);
    tree->SetBranchAddress("v", &v);
  }
  //
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);
  tree->Fill();
  hfile->Write();
  hfile->Close();
}

std::vector<TString> getVectorOfinDataValuesNames (TString inDatFile) {
  std::vector<TString> dataValNamStr;
  std::ifstream indata;
  TString valName;
  indata.open(inDatFile.Data());
  assert(indata.is_open());  
  while (indata  >>  valName){
    if(indata.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    dataValNamStr.push_back(valName);
  }
  indata.close();
  return dataValNamStr;
}

void dumpVectorOfinDataValuesNames (std::vector<TString> dataValNamStr) {
  for(unsigned i = 0; i < dataValNamStr.size(); i++)
    std::cout<<"dataValNamStr.at(i) = "<<dataValNamStr.at(i)<<std::endl;
}

std::vector<std::vector<TGraph*>> getVectorOfGraphsVector (std::vector<TString> inRootFile, std::vector<TString> inDatFile) {
  std::vector<std::vector<TGraph*>> v_grv;
  if(inRootFile.size() != inDatFile.size())
    assert(0);
  for(UInt_t i = 0; i < inRootFile.size(); i++)
    v_grv.push_back(getGraphsVector(inRootFile.at(i),inDatFile.at(i),0));
  return v_grv;
}

std::vector<TGraph*> getGraphsVector (TString inRootFile, TString inDatFile, Int_t saveKey) {
  std::vector<TString> dataValNamStr = getVectorOfinDataValuesNames(inDatFile);
  //dumpVectorOfinDataValuesNames(dataValNamStr);
  //
  std::vector<TGraph*> grv;
  for (UInt_t i = 0; i < dataValNamStr.size(); ++i) {
    TGraph* gr = new TGraph();
    TString grName = "gr_"; grName += dataValNamStr.at(i); grName += "_vs_ut";
    TString grTitle = "gr "; grTitle += dataValNamStr.at(i); grTitle += "_vs_ut";
    //std::cout<<"grName  "<<grName<<std::endl
    //	     <<"grTitle "<<grTitle<<std::endl;
    gr->SetName(grName.Data());
    gr->SetTitle(grTitle.Data());
    grv.push_back(gr);
  }
  //
  TFile *f = TFile::Open(inRootFile.Data(),"READ");
  if (!f) { 
    assert(0);
  }
  TTree *t; 
  f->GetObject("T",t);
  Int_t vpID;
  Int_t ut;
  std::vector<float> *v;
  TBranch *b_vpID;
  TBranch *b_ut;
  TBranch *b_v;
  t->SetBranchAddress("vpID", &vpID, &b_vpID);
  t->SetBranchAddress("ut", &ut, &b_ut);
  t->SetBranchAddress("v", &v, &b_v);
  //Loop
  Long64_t nentries = t->GetEntriesFast();
  std::cout<<"nentries = "<<nentries<<std::endl;
  v = 0;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //Long64_t ientry = t->LoadTree(jentry);
    t->LoadTree(jentry);
    nb = t->GetEntry(jentry);   
    nbytes += nb;
    //std::cout<<ientry<<" "<<ut<<std::endl;
    //////////////////////////////////////
    if(v->size() != grv.size()){
      std::cout<<" ERROR ---> v->size() != grv.size()"<<std::endl
	       <<"            v->size()  = "<<v->size()<<std::endl
	       <<"           grv.size()  = "<<grv.size()<<std::endl;
      assert(0);
    }
    for (UInt_t i = 0; i < v->size(); ++i) {
      grv.at(i)->SetPoint(grv.at(i)->GetN(), ut, v->at(i));
    }
  }
  //////////////////////////////////////
  if(saveKey == 1){
    TString histOut = inRootFile; histOut += "_hist.root";
    TFile* outRootFile = new TFile(histOut.Data(), "RECREATE", " Histograms", 1);
    outRootFile->cd();
    if (outRootFile->IsZombie()){
      std::cout<<"  ERROR ---> file "<<histOut.Data()<<" is zombi"<<std::endl;
      assert(0);
    }
    else
      std::cout<<"  Output Histos file ---> "<<histOut.Data()<<std::endl;
    for (UInt_t i = 0; i < grv.size(); ++i) 
      grv.at(i)->Write();
    outRootFile->Close();
  }
  //////////////////////////////////////
  return grv;
}

std::vector<float> getDataVector (TString inRootFile = "", Long64_t jentry = 0) {
  //
  TFile *f = TFile::Open(inRootFile.Data(),"READ");
  if (!f) { 
    assert(0);
  }
  TTree *t; 
  f->GetObject("T",t);
  Int_t vpID;
  Int_t ut;
  std::vector<float> *v;
  TBranch *b_vpID;
  TBranch *b_ut;
  TBranch *b_v;
  t->SetBranchAddress("vpID", &vpID, &b_vpID);
  t->SetBranchAddress("ut", &ut, &b_ut);
  t->SetBranchAddress("v", &v, &b_v);
  Long64_t nentries = t->GetEntriesFast();
  std::cout<<"nentries = "<<nentries<<std::endl;
  v = 0;
  Long64_t nbytes = 0, nb = 0;
  if( nentries <= jentry){
    std::cout<<"ERROR -> nentries >= jentry"<<std::endl
	     <<"         nentries >= "<<nentries<<std::endl
      	     <<"         jentry   >= "<<jentry<<std::endl;
    assert(0);
  }
  //Long64_t ientry = t->LoadTree(jentry);
  t->LoadTree(jentry);
  nb = t->GetEntry(jentry);   
  nbytes += nb;
  //////////////////////////////////////
  std::vector<float> vv;
  for (UInt_t i = 0; i < v->size(); ++i)
    vv.push_back(v->at(i));
  return vv;
}
