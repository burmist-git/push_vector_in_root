#pragma once

//root
#include <TString.h>
#include <TGraph.h>

//pushVectorInRoot.cc
void pushVectorInRoot (TString outRootFile, int vecparID, int unixTimeInt, const std::vector<float>& val);
std::vector<TGraph*> getGraphsVector (TString inRootFile, TString inDatFile, Int_t saveKey);
void dumpVectorOfinDataValuesNames (std::vector<TString> dataValNamStr);
std::vector<TString> getVectorOfinDataValuesNames (TString inDatFile);
std::vector<std::vector<TGraph*>> getVectorOfGraphsVector (std::vector<TString> inRootFile, std::vector<TString> inDatFile);
std::vector<float> getDataVector (TString inRootFile, Long64_t jentry);
