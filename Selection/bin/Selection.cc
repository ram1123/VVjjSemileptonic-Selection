#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <iomanip>
#include <ctime>
#include <map>
#include <math.h>

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TChain.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TClass.h"
#include "TApplication.h"
#include "TLorentzVector.h"
#include "TF1.h"
#include "TH2.h"
#include <TClonesArray.h>

#include "../interface/NanoReader.hh"
#include "../interface/ScaleFactors.hh"
#include "../interface/Utils.hh"
#include "../interface/METzCalculator.h"

int main (int ac, char** av) {

  std::string inputFile = av[1];
  std::string outputFile = av[2];
  int isMC = atoi(av[3]);
  int era = atoi(av[4]);
  int VERBOSE = atoi(av[5]);
  int nanoVersion = atoi(av[6]);

  const float MUON_MASS = 0.1056583745;
  const float ELE_MASS  = 0.000511;
  const float W_MASS = 80.385;
  //const float Z_MASS = 91.1876;

  //lepton cuts
  const float LEP_PT_VETO_CUT = 10;
  const float EL_PT_CUT = 20;
  const float EL_ETA_CUT = 2.5;
  const float MU_PT_CUT = 20;
  const float MU_ETA_CUT = 2.4;

  //ak8 jet cuts
  const float AK8_MIN_PT = 200;
  const float AK8_MAX_ETA = 2.4;
  const float AK8_MIN_SDM = 40;
  const float AK8_MAX_SDM = 150;

  //ak4 jet cuts
  //const float AK4_PT_VETO_CUT = 20;
  const float AK4_ETA_CUT = 2.4;
  const float AK4_PT_CUT = 30;
  const float AK4_JJ_MIN_M = 40.0;
  const float AK4_JJ_MAX_M = 150.0;

  //cleaning cuts
  const float AK8_LEP_DR_CUT = 0.8;
  const float AK4_AK8_DR_CUT = 0.8;
  const float AK4_DR_CUT = 0.4;

  // btag deepCSV a.k.a. DeepB working points
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy
  float btag_loose_wp = 0.0;
  float btag_tight_wp = 0.0;
  float btag_medium_wp = 0.0;
  if (era==2018) {
    btag_loose_wp = 0.1241;
    btag_medium_wp = 0.4184;
    btag_tight_wp = 0.7527;
  }
  if (era==2017) {
    btag_loose_wp = 0.1522;
    btag_medium_wp = 0.4941;
    btag_tight_wp = 0.8001;
  }
  if (era==2016) {
    btag_loose_wp = 0.2217;
    btag_medium_wp = 0.6321;
    btag_tight_wp = 0.8953;
  }

  ScaleFactors scaleFactor(era);

  std::vector<TLorentzVector> tightMuon;
  std::vector<TLorentzVector> tightEle;
  std::vector<int>goodJetIndex;

  //
  //
  //   INPUT/OUTPUT
  //
  //

  TFile *of = new TFile(outputFile.c_str(),"RECREATE");
  TTree *ot = new TTree("Events","Events");
  WVJJData* WVJJTree = new WVJJData(ot);  
  TH1F *totalEvents = new TH1F("TotalEvents","TotalEvents",2,-1,1);
  TH1F *totalCutFlow = new TH1F("TotalCutFlow","TotalCutFlow",7,0,7);
  totalCutFlow->GetXaxis()->SetBinLabel(1,"MC Gen");
  totalCutFlow->GetXaxis()->SetBinLabel(2,"Skim NanoAOD");
  totalCutFlow->GetXaxis()->SetBinLabel(3,"Trigger");
  totalCutFlow->GetXaxis()->SetBinLabel(4,"Lepton Selection");
  totalCutFlow->GetXaxis()->SetBinLabel(5,"MET > 0");
  totalCutFlow->GetXaxis()->SetBinLabel(6,"Hadronic V");
  totalCutFlow->GetXaxis()->SetBinLabel(7,"VBS Pair");

  TH1F *zvCutFlow = (TH1F*)totalCutFlow->Clone("zvCutFlow");
  zvCutFlow->SetTitle("zvCutFlow");
  TH1F *zjjCutFlow = (TH1F*)totalCutFlow->Clone("zjjCutFlow");
  zjjCutFlow->SetTitle("zjjCutFlow");
  TH1F *wvCutFlow = (TH1F*)totalCutFlow->Clone("wvCutFlow");
  wvCutFlow->SetTitle("wvCutFlow");
  TH1F *wjjCutFlow = (TH1F*)totalCutFlow->Clone("wjjCutFlow");
  wjjCutFlow->SetTitle("wjjCutFlow");

  TFile *f=0;
  TTree *t=0, *r=0;

  Bool_t has_HLT_IsoMu22=false, has_HLT_IsoTkMu22=false, has_HLT_IsoMu24=false, has_HLT_IsoTkMu24=false,
    has_HLT_IsoMu27=false, has_HLT_IsoMu30=false, has_HLT_Mu50=false,
    has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ=false, has_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ=false,
    has_HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ=false, has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8=false,
    has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8=false, has_HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8=false,
    has_HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8=false, has_HLT_Ele27_WPTight_Gsf=false, has_HLT_Ele28_WPTight_Gsf=false,
    has_HLT_Ele32_WPTight_Gsf=false, has_HLT_Ele35_WPLoose_Gsf=false, has_HLT_Ele35_WPTight_Gsf=false, has_HLT_Ele38_WPTight_Gsf=false,
    has_HLT_Ele40_WPTight_Gsf=false, has_HLT_Ele25_eta2p1_WPTight_Gsf=false, has_HLT_Ele27_eta2p1_WPLoose_Gsf=false,
    has_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ=false, has_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL=false,
    has_HLT_DiEle27_WPTightCaloOnly_L1DoubleEG=false,
    has_HLT_DoubleEle33_CaloIdL_MW=false, has_HLT_DoubleEle25_CaloIdL_MW=false, has_HLT_DoubleEle27_CaloIdL_MW=false;

  std::ifstream ifs;
  ifs.open(inputFile.data());
  assert(ifs.is_open());
  std::string line;
  int lineCount=0;
  while (getline(ifs,line)) {
    std::stringstream ss(line);
    std::string filetoopen;
    ss >> filetoopen;
    
    lineCount+=1;

    f = TFile::Open(TString(filetoopen),"read");
    // f = TFile::Open(TString("root://cms-xrd-global.cern.ch//")+TString(filetoopen),"read");
    // f = TFile::Open(TString("root://cmseos.fnal.gov/")+TString(filetoopen),"read");
    //f = TFile::Open(TString("root://xrootd-cms.infn.it/")+TString(filetoopen),"read");
    t = (TTree *)f->Get("Events");
    r = (TTree *)f->Get("Runs");
    if (t==NULL) continue;
    
    if(VERBOSE) std::cout << "File to open: " << filetoopen << std::endl;
    if(VERBOSE) std::cout << "\tEntries: " << t->GetEntries() << std::endl;
    
    NanoReader nr = NanoReader(t);

    NanoReader NanoWeightReader = NanoReader(r);
    NanoWeightReader.GetEntry(0);
    float genEventSumw=0.0;
    float genEventCount=0.0;
    if (isMC && nanoVersion == 6) {
      genEventSumw = *NanoWeightReader.genEventSumw_;
      genEventCount = *NanoWeightReader.genEventCount_;
    }
    else if (isMC && nanoVersion == 7) {
      genEventSumw = *NanoWeightReader.genEventSumw;
      genEventCount = *NanoWeightReader.genEventCount;
    }
    totalEvents->SetBinContent(2,totalEvents->GetBinContent(2)+genEventSumw);
    totalCutFlow->Fill("MC Gen",genEventCount);
    zvCutFlow->Fill("MC Gen",genEventCount);
    zjjCutFlow->Fill("MC Gen",genEventCount);
    wvCutFlow->Fill("MC Gen",genEventCount);
    wjjCutFlow->Fill("MC Gen",genEventCount);

    //check if tree has these hlt branches
    if (lineCount == 1){
      has_HLT_IsoMu22 = t->GetBranchStatus("HLT_IsoMu22");
      has_HLT_IsoTkMu22 = t->GetBranchStatus("HLT_IsoTkMu22");
      has_HLT_IsoMu24 = t->GetBranchStatus("HLT_IsoMu24");
      has_HLT_IsoTkMu24 = t->GetBranchStatus("HLT_IsoTkMu24");
      has_HLT_IsoMu27 = t->GetBranchStatus("HLT_IsoMu27");
      has_HLT_IsoMu30 = t->GetBranchStatus("HLT_IsoMu30");
      has_HLT_Mu50 = t->GetBranchStatus("HLT_Mu50");

      has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = t->GetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ");
      has_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = t->GetBranchStatus("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ");
      has_HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = t->GetBranchStatus("HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ");
      has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8 = t->GetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8");
      has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 = t->GetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8");
      has_HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8 = t->GetBranchStatus("HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8");
      has_HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8 = t->GetBranchStatus("HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8");

      has_HLT_Ele27_WPTight_Gsf = t->GetBranchStatus("HLT_Ele27_WPTight_Gsf");
      has_HLT_Ele28_WPTight_Gsf = t->GetBranchStatus("HLT_Ele28_WPTight_Gsf");
      has_HLT_Ele32_WPTight_Gsf = t->GetBranchStatus("HLT_Ele32_WPTight_Gsf");
      has_HLT_Ele35_WPLoose_Gsf = t->GetBranchStatus("HLT_Ele35_WPLoose_Gsf");
      has_HLT_Ele35_WPTight_Gsf = t->GetBranchStatus("HLT_Ele35_WPTight_Gsf");
      has_HLT_Ele38_WPTight_Gsf = t->GetBranchStatus("HLT_Ele38_WPTight_Gsf");
      has_HLT_Ele40_WPTight_Gsf = t->GetBranchStatus("HLT_Ele40_WPTight_Gsf");
      has_HLT_Ele25_eta2p1_WPTight_Gsf = t->GetBranchStatus("HLT_Ele25_eta2p1_WPTight_Gsf");
      has_HLT_Ele27_eta2p1_WPLoose_Gsf = t->GetBranchStatus("HLT_Ele27_eta2p1_WPLoose_Gsf");

      has_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = t->GetBranchStatus("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
      has_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL = t->GetBranchStatus("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL");
      has_HLT_DiEle27_WPTightCaloOnly_L1DoubleEG = t->GetBranchStatus("HLT_DiEle27_WPTightCaloOnly_L1DoubleEG");
      has_HLT_DoubleEle33_CaloIdL_MW = t->GetBranchStatus("HLT_DoubleEle33_CaloIdL_MW");
      has_HLT_DoubleEle25_CaloIdL_MW = t->GetBranchStatus("HLT_DoubleEle25_CaloIdL_MW");
      has_HLT_DoubleEle27_CaloIdL_MW = t->GetBranchStatus("HLT_DoubleEle27_CaloIdL_MW");
    }

    for (uint i=0; i < t->GetEntries(); i++) {
    //for (uint i=0; i < 1000000; i++) {
      WVJJTree->clearVars();
      nr.GetEntry(i);
      totalCutFlow->Fill("Skim NanoAOD",1);
      zvCutFlow->Fill("Skim NanoAOD",1);
      zjjCutFlow->Fill("Skim NanoAOD",1);
      wvCutFlow->Fill("Skim NanoAOD",1);
      wjjCutFlow->Fill("Skim NanoAOD",1);

      if (i%100000==0) std::cout <<"file " << lineCount << ": event " << i << std::endl;

      if (isMC==1) {
        WVJJTree->genWeight=*nr.Generator_weight;
      }

      WVJJTree->trigger_1Mu = ((has_HLT_IsoMu22 ? *nr.HLT_IsoMu22 : 0) ||
                               (has_HLT_IsoTkMu22 ? *nr.HLT_IsoTkMu22 : 0) ||
                               (has_HLT_IsoMu24 ? *nr.HLT_IsoMu24 : 0) ||
                               (has_HLT_IsoTkMu24 ? *nr.HLT_IsoTkMu24 : 0) ||
                               (has_HLT_IsoMu27 ? *nr.HLT_IsoMu27 : 0) ||
                               (has_HLT_IsoMu30 ? *nr.HLT_IsoMu30 : 0) ||
                               (has_HLT_Mu50 ? *nr.HLT_Mu50 : 0));

      WVJJTree->trigger_2Mu = ((has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ ? *nr.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ : 0) ||
                               (has_HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ ? *nr.HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ : 0) ||
                               (has_HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ ? *nr.HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ : 0) ||
                               (has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8 ? *nr.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8 : 0) ||
                               (has_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 ? *nr.HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8 : 0) ||
                               (has_HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8 ? *nr.HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass3p8 : 0) ||
                               (has_HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8 ? *nr.HLT_Mu19_TrkIsoVVL_Mu9_TrkIsoVVL_DZ_Mass8 : 0));

      WVJJTree->trigger_1El = ((has_HLT_Ele27_WPTight_Gsf ? *nr.HLT_Ele27_WPTight_Gsf : 0) ||
                               (has_HLT_Ele28_WPTight_Gsf ? *nr.HLT_Ele28_WPTight_Gsf : 0) ||
                               (has_HLT_Ele32_WPTight_Gsf ? *nr.HLT_Ele32_WPTight_Gsf : 0) ||
                               (has_HLT_Ele35_WPLoose_Gsf ? *nr.HLT_Ele35_WPLoose_Gsf : 0) ||
                               (has_HLT_Ele35_WPTight_Gsf ? *nr.HLT_Ele35_WPTight_Gsf : 0) ||
                               (has_HLT_Ele38_WPTight_Gsf ? *nr.HLT_Ele38_WPTight_Gsf : 0) ||
                               (has_HLT_Ele40_WPTight_Gsf ? *nr.HLT_Ele40_WPTight_Gsf : 0) ||
                               (has_HLT_Ele25_eta2p1_WPTight_Gsf ? *nr.HLT_Ele25_eta2p1_WPTight_Gsf : 0) ||
                               (has_HLT_Ele27_eta2p1_WPLoose_Gsf ? *nr.HLT_Ele27_eta2p1_WPLoose_Gsf : 0));

      WVJJTree->trigger_2El = ((has_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ ? *nr.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ : 0) ||
                               (has_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL ? *nr.HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL : 0) ||
                               (has_HLT_DiEle27_WPTightCaloOnly_L1DoubleEG ? *nr.HLT_DiEle27_WPTightCaloOnly_L1DoubleEG : 0) ||
                               (has_HLT_DoubleEle33_CaloIdL_MW ? *nr.HLT_DoubleEle33_CaloIdL_MW : 0) ||
                               (has_HLT_DoubleEle25_CaloIdL_MW ? *nr.HLT_DoubleEle25_CaloIdL_MW : 0) ||
                               (has_HLT_DoubleEle27_CaloIdL_MW ? *nr.HLT_DoubleEle27_CaloIdL_MW : 0));

      //std::cout << "passed trigger: ";
      //if (WVJJTree->trigger_1Mu) std::cout << "1 muon ";
      //if (WVJJTree->trigger_2Mu) std::cout << "2 muon ";
      //if (WVJJTree->trigger_1El) std::cout << "1 ele ";
      //if (WVJJTree->trigger_2El) std::cout << "2 ele ";
      //std::cout << std::endl;

      if ( ! ( WVJJTree->trigger_1Mu || WVJJTree->trigger_2Mu || WVJJTree->trigger_1El || WVJJTree->trigger_2El ) ) continue;
      totalCutFlow->Fill("Trigger",1);
      zvCutFlow->Fill("Trigger",1);
      zjjCutFlow->Fill("Trigger",1);
      wvCutFlow->Fill("Trigger",1);
      wjjCutFlow->Fill("Trigger",1);

      tightMuon.clear();
      tightEle.clear();

      WVJJTree->run = *nr.run;
      WVJJTree->evt = *nr.event;
      WVJJTree->ls = *nr.luminosityBlock;

      WVJJTree->nPV = *nr.PV_npvsGood;
      if (isMC) WVJJTree->nPU_mean = *nr.Pileup_nPU;

      WVJJTree->puWeight = scaleFactor.GetPUWeight(WVJJTree->nPU_mean, 0);
      WVJJTree->puWeight_Up = scaleFactor.GetPUWeight(WVJJTree->nPU_mean, 1);
      WVJJTree->puWeight_Down = scaleFactor.GetPUWeight(WVJJTree->nPU_mean, -1);

      // LEPTON SELECTION

      int nTightEle=0;
      int nTightMu=0;
      int nVetoEle=0;
      int nVetoMu=0;
      
      for (uint j=0; j < *nr.nMuon; j++) {
        if ( abs(nr.Muon_eta[j]) > MU_ETA_CUT ) continue;
        //using conservative uncertainty value of 3%
        if ( 1.03*nr.Muon_pt[j] < LEP_PT_VETO_CUT ) continue;

        if (!nr.Muon_looseId[j]) continue;
        if (nr.Muon_pfRelIso04_all[j]>0.25) continue;
        nVetoMu++;

        //using conservative uncertainty value of 3%
        if ( 1.03*nr.Muon_pt[j] < MU_PT_CUT ) continue;
        if (!nr.Muon_tightId[j]) continue;
        if (nr.Muon_pt[j] > 20 && abs(nr.Muon_dxy[j])>0.01) continue;
        if (nr.Muon_pt[j] < 20 && abs(nr.Muon_dxy[j])>0.02) continue;
        if (abs(nr.Muon_dz[j])>0.1) continue;
        if (nr.Muon_pfRelIso04_all[j]>0.15) continue;

        nTightMu++;
        tightMuon.push_back(TLorentzVector(0,0,0,0));
        tightMuon.back().SetPtEtaPhiM(nr.Muon_pt[j], nr.Muon_eta[j],
                                      nr.Muon_phi[j], MUON_MASS);

        if ( nr.Muon_pt[j] > WVJJTree->lep1_pt ) {
          WVJJTree->lep2_pt = WVJJTree->lep1_pt;
          WVJJTree->lep2_eta = WVJJTree->lep1_eta;
          WVJJTree->lep2_phi = WVJJTree->lep1_phi;
          WVJJTree->lep2_m = WVJJTree->lep1_m;
          WVJJTree->lep2_iso = WVJJTree->lep1_iso;
          WVJJTree->lep2_dxy = WVJJTree->lep1_dxy;
          WVJJTree->lep2_dz = WVJJTree->lep1_dz;
          WVJJTree->lep2_q = WVJJTree->lep1_q;

          WVJJTree->lep1_pt = nr.Muon_pt[j];
          WVJJTree->lep1_eta = nr.Muon_eta[j];
          WVJJTree->lep1_phi = nr.Muon_phi[j];
          WVJJTree->lep1_m = MUON_MASS;
          WVJJTree->lep1_iso = nr.Muon_pfRelIso04_all[j];
          WVJJTree->lep1_dxy = nr.Muon_dxy[j];
          WVJJTree->lep1_dz = nr.Muon_dz[j];
          WVJJTree->lep1_q = nr.Muon_charge[j];
        }
        else if ( nr.Muon_pt[j] > WVJJTree->lep2_pt ) {
        //if (WVJJTree->lep1_q*nr.Muon_charge[j]>0) continue;

          WVJJTree->lep2_pt = nr.Muon_pt[j];
          WVJJTree->lep2_eta = nr.Muon_eta[j];
          WVJJTree->lep2_phi = nr.Muon_phi[j];
          WVJJTree->lep2_m = MUON_MASS;
          WVJJTree->lep2_iso = nr.Muon_pfRelIso04_all[j];
          WVJJTree->lep2_dxy = nr.Muon_dxy[j];
          WVJJTree->lep2_dz = nr.Muon_dz[j];
          WVJJTree->lep2_q = nr.Muon_charge[j];
        }
      }

      for (uint j=0; j < *nr.nElectron; j++) {
        if ( abs(nr.Electron_eta[j]) > EL_ETA_CUT ) continue;
          //using conservative uncertainty value of 3%
          if ( 1.03*nr.Electron_pt[j] < LEP_PT_VETO_CUT ) continue;

          //cut-based ID Fall17 V2 (0:fail, 1:veto, 2:loose, 3:medium, 4:tight)
          if ( nr.Electron_cutBased[j]<2 ) continue;
          if ( abs(nr.Electron_eta[j])>1.5) {
          if (nr.Electron_sieie[j]>0.03) continue;
          if (nr.Electron_eInvMinusPInv[j]>0.014) continue;
          if (abs(nr.Electron_dxy[j])>0.1) continue;
          if (abs(nr.Electron_dz[j])>0.2) continue;
        }
        else {
          if (abs(nr.Electron_dxy[j])>0.05) continue;
          if (abs(nr.Electron_dz[j])>0.1) continue;
        }

        nVetoEle++;

        //using conservative uncertainty value of 3%
        if ( 1.03*nr.Electron_pt[j] < EL_PT_CUT ) continue;

        //if (nr.Electron_cutBased[j]<4) continue;

        if (!nr.Electron_mvaFall17V2Iso_WP90[j]) continue;
        nTightEle++;

        tightEle.push_back(TLorentzVector(0,0,0,0));
        tightEle.back().SetPtEtaPhiM(nr.Electron_pt[j],nr.Electron_eta[j],
				     nr.Electron_phi[j],ELE_MASS);

        //don't try to select electrons unless we don't already
        //have muons
        if (WVJJTree->lep1_m == MUON_MASS) continue;

        if ( nr.Electron_pt[j] > WVJJTree->lep1_pt ) {
          WVJJTree->lep2_pt = WVJJTree->lep1_pt;
          WVJJTree->lep2_eta = WVJJTree->lep1_eta;
          WVJJTree->lep2_phi = WVJJTree->lep1_phi;
          WVJJTree->lep2_m = WVJJTree->lep1_m;
          WVJJTree->lep2_iso = WVJJTree->lep1_iso;
          WVJJTree->lep2_dxy = WVJJTree->lep1_dxy;
          WVJJTree->lep2_dz = WVJJTree->lep1_dz;
          WVJJTree->lep2_sip3d = WVJJTree->lep1_sip3d;
          WVJJTree->lep2_q = WVJJTree->lep1_q;

          WVJJTree->lep1_pt = nr.Electron_pt[j];
          WVJJTree->lep1_eta = nr.Electron_eta[j];
          WVJJTree->lep1_phi = nr.Electron_phi[j];
          WVJJTree->lep1_m = ELE_MASS;
          WVJJTree->lep1_iso = nr.Electron_pfRelIso03_all[j];
          WVJJTree->lep1_dxy = nr.Electron_dxy[j];
          WVJJTree->lep1_dz = nr.Electron_dz[j];
          WVJJTree->lep1_sip3d = nr.Electron_sip3d[j];
          WVJJTree->lep1_q = nr.Electron_charge[j];
        }
        else if ( nr.Electron_pt[j] > WVJJTree->lep2_pt ) {
          WVJJTree->lep2_pt = nr.Electron_pt[j];
          WVJJTree->lep2_eta = nr.Electron_eta[j];
          WVJJTree->lep2_phi = nr.Electron_phi[j];
          WVJJTree->lep2_m = ELE_MASS;
          WVJJTree->lep2_iso = nr.Electron_pfRelIso03_all[j];
          WVJJTree->lep2_dxy = nr.Electron_dxy[j];
          WVJJTree->lep2_dz = nr.Electron_dz[j];
          WVJJTree->lep2_sip3d = nr.Electron_sip3d[j];
          WVJJTree->lep2_q = nr.Electron_charge[j];
        }
      }

      //check conditions

      bool passLepSel = true;
      if(!(WVJJTree->lep1_pt>0)) passLepSel=false;
      if ((nTightMu+nTightEle)==0) passLepSel=false; //no leptons with required ID
      if((nVetoEle+nVetoMu)>2) passLepSel=false;
      if(nTightMu>0 && nVetoEle>0) passLepSel=false;
      if(nTightEle>0 && nVetoMu>0) passLepSel=false;
      if(nTightMu==1 && nVetoMu>1) passLepSel=false;
      if(nTightEle==1 && nVetoEle>1) passLepSel=false;

      //if (0) {
      if (passLepSel==false && (nTightMu+nTightEle)==0 && ((nVetoEle>0) ^ (nVetoMu>0))) {
        if (nVetoEle>0) {

          for (uint j=0; j < *nr.nElectron; j++) {
            if ( abs(nr.Electron_eta[j]) > EL_ETA_CUT ) continue;
            if ( 1.03*nr.Electron_pt[j] < EL_PT_CUT ) continue;
            if ( nr.Electron_cutBased[j]<2 ) continue;

            if ( nr.Electron_pt[j] > WVJJTree->lep1_pt ) {
              WVJJTree->lep1_pt = nr.Electron_pt[j];
              WVJJTree->lep1_eta = nr.Electron_eta[j];
              WVJJTree->lep1_phi = nr.Electron_phi[j];
              WVJJTree->lep1_m = ELE_MASS;
              WVJJTree->lep1_iso = nr.Electron_pfRelIso03_all[j];
              WVJJTree->lep1_dxy = nr.Electron_dxy[j];
              WVJJTree->lep1_dz = nr.Electron_dz[j];
              WVJJTree->lep1_sip3d = nr.Electron_sip3d[j];
              WVJJTree->lep1_q = nr.Electron_charge[j];
            }
          }
          tightEle.push_back(TLorentzVector(0,0,0,0));
          tightEle.back().SetPtEtaPhiM(WVJJTree->lep1_pt, WVJJTree->lep1_eta,
                                       WVJJTree->lep1_phi, ELE_MASS);

          WVJJTree->isAntiIso=true;
          WVJJTree->lepFakeRate = scaleFactor.GetLeptonFakeWeights(WVJJTree->lep1_pt, WVJJTree->lep1_eta, 11);
        }

        if (nVetoMu>0) {

          for (uint j=0; j < *nr.nMuon; j++) {
            if ( abs(nr.Muon_eta[j]) > MU_ETA_CUT ) continue;
            if (nr.Muon_pfRelIso04_all[j]>0.25) continue;
            if ( 1.03*nr.Muon_pt[j] < MU_PT_CUT ) continue;
            if (!nr.Muon_tightId[j]) continue;

            if ( nr.Muon_pt[j] > WVJJTree->lep1_pt ) {
              WVJJTree->lep1_pt = nr.Muon_pt[j];
              WVJJTree->lep1_eta = nr.Muon_eta[j];
              WVJJTree->lep1_phi = nr.Muon_phi[j];
              WVJJTree->lep1_m = MUON_MASS;
              WVJJTree->lep1_iso = nr.Muon_pfRelIso04_all[j];
              WVJJTree->lep1_dxy = nr.Muon_dxy[j];
              WVJJTree->lep1_dz = nr.Muon_dz[j];
              WVJJTree->lep1_q = nr.Muon_charge[j];
            }
          }

          tightMuon.push_back(TLorentzVector(0,0,0,0));
          tightMuon.back().SetPtEtaPhiM(WVJJTree->lep1_pt, WVJJTree->lep1_eta,
                                        WVJJTree->lep1_phi,MUON_MASS);

          WVJJTree->isAntiIso=true;
          WVJJTree->lepFakeRate = scaleFactor.GetLeptonFakeWeights(WVJJTree->lep1_pt, WVJJTree->lep1_eta, 13);
        }
      }

      if (passLepSel) totalCutFlow->Fill("Lepton Selection",1);
      if (passLepSel && WVJJTree->lep2_pt > 0) zvCutFlow->Fill("Lepton Selection",1);
      if (passLepSel && WVJJTree->lep2_pt > 0) zjjCutFlow->Fill("Lepton Selection",1);
      if (passLepSel && WVJJTree->lep2_pt < 0) wvCutFlow->Fill("Lepton Selection",1);
      if (passLepSel && WVJJTree->lep2_pt < 0) wjjCutFlow->Fill("Lepton Selection",1);

      if (!passLepSel && !WVJJTree->isAntiIso) continue;

      //muon scale variations
      if (WVJJTree->lep1_m == MUON_MASS) {
        //https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceScaleResolRun2
        if (WVJJTree->lep1_eta<-2.1) {
          WVJJTree->lep1_pt_scaleUp = 1.027 * WVJJTree->lep1_pt;
          WVJJTree->lep1_pt_scaleDown = 0.973 * WVJJTree->lep1_pt;
        }
        else if (WVJJTree->lep1_eta<-1.2) {
          WVJJTree->lep1_pt_scaleUp = 1.009 * WVJJTree->lep1_pt;
          WVJJTree->lep1_pt_scaleDown = 0.991 * WVJJTree->lep1_pt;
        }
        else if (WVJJTree->lep1_eta<1.2) {
          WVJJTree->lep1_pt_scaleUp = 1.004 * WVJJTree->lep1_pt;
          WVJJTree->lep1_pt_scaleDown = 0.996 * WVJJTree->lep1_pt;
        }
        else if (WVJJTree->lep1_eta<2.1) {
          WVJJTree->lep1_pt_scaleUp = 1.009 * WVJJTree->lep1_pt;
          WVJJTree->lep1_pt_scaleDown = 0.991 * WVJJTree->lep1_pt;
        }
        else {
          WVJJTree->lep1_pt_scaleUp = 1.017 * WVJJTree->lep1_pt;
          WVJJTree->lep1_pt_scaleDown = 0.983 * WVJJTree->lep1_pt;
        }

        if (WVJJTree->lep2_pt>0) {
          if (WVJJTree->lep2_eta<-2.1) {
            WVJJTree->lep2_pt_scaleUp = 1.027 * WVJJTree->lep2_pt;
            WVJJTree->lep2_pt_scaleDown = 0.973 * WVJJTree->lep2_pt;
          }
          else if (WVJJTree->lep2_eta<-1.2) {
            WVJJTree->lep2_pt_scaleUp = 1.009 * WVJJTree->lep2_pt;
            WVJJTree->lep2_pt_scaleDown = 0.991 * WVJJTree->lep2_pt;
          }
          else if (WVJJTree->lep2_eta<1.2) {
            WVJJTree->lep2_pt_scaleUp = 1.004 * WVJJTree->lep2_pt;
            WVJJTree->lep2_pt_scaleDown = 0.996 * WVJJTree->lep2_pt;
          }
          else if (WVJJTree->lep2_eta<2.1) {
            WVJJTree->lep2_pt_scaleUp = 1.009 * WVJJTree->lep2_pt;
            WVJJTree->lep2_pt_scaleDown = 0.991 * WVJJTree->lep2_pt;
          }
          else {
            WVJJTree->lep2_pt_scaleUp = 1.017 * WVJJTree->lep2_pt;
            WVJJTree->lep2_pt_scaleDown = 0.983 * WVJJTree->lep2_pt;
          }
        }
      }

      //electron scale variations
      if (WVJJTree->lep1_m == ELE_MASS) {
        WVJJTree->lep1_pt_scaleUp = 1.01 * WVJJTree->lep1_pt;
        WVJJTree->lep1_pt_scaleDown = 0.99 * WVJJTree->lep1_pt;
        if (WVJJTree->lep2_pt>0) {
          WVJJTree->lep2_pt_scaleUp = 1.01 * WVJJTree->lep2_pt;
          WVJJTree->lep2_pt_scaleDown = 0.99 * WVJJTree->lep2_pt;
        }
      }
      
      if (WVJJTree->lep1_pt > 0 && WVJJTree->lep2_pt > 0) {

        TLorentzVector lep1(0,0,0,0);
        lep1.SetPtEtaPhiM( WVJJTree->lep1_pt, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m );

        TLorentzVector lep2(0,0,0,0);
        lep2.SetPtEtaPhiM( WVJJTree->lep2_pt, WVJJTree->lep2_eta, WVJJTree->lep2_phi, WVJJTree->lep2_m );

        TLorentzVector dilep = lep1+lep2;

        WVJJTree->dilep_m = dilep.M();
        WVJJTree->dilep_mt = dilep.Mt();
        WVJJTree->dilep_pt = dilep.Pt();
        WVJJTree->dilep_eta = dilep.Eta();
        WVJJTree->dilep_phi = dilep.Phi();

        //dilepton scale variations

        lep1.SetPtEtaPhiM( WVJJTree->lep1_pt_scaleUp, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
        lep2.SetPtEtaPhiM( WVJJTree->lep2_pt_scaleUp, WVJJTree->lep2_eta, WVJJTree->lep2_phi, WVJJTree->lep2_m);

        WVJJTree->dilep_m_scaleUp = (lep1+lep2).M();
        WVJJTree->dilep_mt_scaleUp = (lep1+lep2).Mt();
        WVJJTree->dilep_pt_scaleUp = (lep1+lep2).Pt();

        lep1.SetPtEtaPhiM( WVJJTree->lep1_pt_scaleDown, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
        lep2.SetPtEtaPhiM( WVJJTree->lep2_pt_scaleDown, WVJJTree->lep2_eta, WVJJTree->lep2_phi, WVJJTree->lep2_m);

        WVJJTree->dilep_m_scaleDown = (lep1+lep2).M();
        WVJJTree->dilep_mt_scaleDown = (lep1+lep2).Mt();
        WVJJTree->dilep_pt_scaleDown = (lep1+lep2).Pt();

      }
      
      //lepton ID/iso/trigger efficiencies
      WVJJTree->lep1_idEffWeight = scaleFactor.GetLeptonWeights(WVJJTree->lep1_pt, WVJJTree->lep1_eta,
                                                                WVJJTree->lep1_m == ELE_MASS ? 11 : 13);
      WVJJTree->lep1_trigEffWeight = 1.0;

      if (WVJJTree->lep2_pt>0) {
        WVJJTree->lep2_idEffWeight = scaleFactor.GetLeptonWeights(WVJJTree->lep2_pt, WVJJTree->lep2_eta,
                                                                  WVJJTree->lep1_m == ELE_MASS ? 11 : 13);
        WVJJTree->lep1_trigEffWeight = 1.0;
      }

      // drop events with no leading lepton
      if (WVJJTree->lep1_pt < 0) continue;

      // MET
      if (passLepSel && *nr.MET_pt > 0) totalCutFlow->Fill("MET > 0",1);
      if (passLepSel && *nr.MET_pt > 0 && WVJJTree->lep2_pt > 0) zvCutFlow->Fill("MET > 0",1);
      if (passLepSel && *nr.MET_pt > 0 && WVJJTree->lep2_pt > 0) zjjCutFlow->Fill("MET > 0",1);
      if (passLepSel && *nr.MET_pt > 0 && WVJJTree->lep2_pt < 0) wvCutFlow->Fill("MET > 0",1);
      if (passLepSel && *nr.MET_pt > 0 && WVJJTree->lep2_pt < 0) wjjCutFlow->Fill("MET > 0",1);
      if (*nr.MET_pt < 0) continue;
      WVJJTree->MET = isMC ? *nr.MET_T1Smear_pt : *nr.MET_T1_pt;
      WVJJTree->MET_phi = isMC ? *nr.MET_T1Smear_phi : *nr.MET_T1_phi;
      if (era==2017) {
        WVJJTree->MET_2017 = *nr.METFixEE2017_pt;
      }

      WVJJTree->PuppiMET = *nr.PuppiMET_pt;
      WVJJTree->PuppiMET_phi = *nr.PuppiMET_phi;

      if (isMC) {
        WVJJTree->MET_jesFlavorQCDUp = *nr.MET_T1Smear_pt_jesFlavorQCDUp;
        WVJJTree->MET_jesFlavorQCDDown = *nr.MET_T1Smear_pt_jesFlavorQCDDown;
        WVJJTree->MET_jesRelativeBalUp = *nr.MET_T1Smear_pt_jesRelativeBalUp;
        WVJJTree->MET_jesRelativeBalDown = *nr.MET_T1Smear_pt_jesRelativeBalDown;
        WVJJTree->MET_jesHFUp = *nr.MET_T1Smear_pt_jesHFUp;
        WVJJTree->MET_jesHFDown = *nr.MET_T1Smear_pt_jesHFDown;
        WVJJTree->MET_jesBBEC1Up = *nr.MET_T1Smear_pt_jesBBEC1Up;
        WVJJTree->MET_jesBBEC1Down = *nr.MET_T1Smear_pt_jesBBEC1Down;
        WVJJTree->MET_jesEC2Up = *nr.MET_T1Smear_pt_jesEC2Up;
        WVJJTree->MET_jesEC2Down = *nr.MET_T1Smear_pt_jesEC2Down;
        WVJJTree->MET_jesAbsoluteUp = *nr.MET_T1Smear_pt_jesAbsoluteUp;
        WVJJTree->MET_jesAbsoluteDown = *nr.MET_T1Smear_pt_jesAbsoluteDown;

        WVJJTree->MET_phi_jesFlavorQCDUp = *nr.MET_T1Smear_phi_jesFlavorQCDUp;
        WVJJTree->MET_phi_jesFlavorQCDDown = *nr.MET_T1Smear_phi_jesFlavorQCDDown;
        WVJJTree->MET_phi_jesRelativeBalUp = *nr.MET_T1Smear_phi_jesRelativeBalUp;
        WVJJTree->MET_phi_jesRelativeBalDown = *nr.MET_T1Smear_phi_jesRelativeBalDown;
        WVJJTree->MET_phi_jesHFUp = *nr.MET_T1Smear_phi_jesHFUp;
        WVJJTree->MET_phi_jesHFDown = *nr.MET_T1Smear_phi_jesHFDown;
        WVJJTree->MET_phi_jesBBEC1Up = *nr.MET_T1Smear_phi_jesBBEC1Up;
        WVJJTree->MET_phi_jesBBEC1Down = *nr.MET_T1Smear_phi_jesBBEC1Down;
        WVJJTree->MET_phi_jesEC2Up = *nr.MET_T1Smear_phi_jesEC2Up;
        WVJJTree->MET_phi_jesEC2Down = *nr.MET_T1Smear_phi_jesEC2Down;
        WVJJTree->MET_phi_jesAbsoluteUp = *nr.MET_T1Smear_phi_jesAbsoluteUp;
        WVJJTree->MET_phi_jesAbsoluteDown = *nr.MET_T1Smear_phi_jesAbsoluteDown;

        if (era==2016) {
          WVJJTree->MET_jesBBEC1_YearUp = *nr.MET_T1Smear_pt_jesBBEC1_2016Up;
          WVJJTree->MET_jesBBEC1_YearDown = *nr.MET_T1Smear_pt_jesBBEC1_2016Down;
          WVJJTree->MET_jesEC2_YearUp = *nr.MET_T1Smear_pt_jesEC2_2016Up;
          WVJJTree->MET_jesEC2_YearDown = *nr.MET_T1Smear_pt_jesEC2_2016Down;
          WVJJTree->MET_jesAbsolute_YearUp = *nr.MET_T1Smear_pt_jesAbsolute_2016Up;
          WVJJTree->MET_jesAbsolute_YearDown = *nr.MET_T1Smear_pt_jesAbsolute_2016Down;
          WVJJTree->MET_jesHF_YearUp = *nr.MET_T1Smear_pt_jesHF_2016Up;
          WVJJTree->MET_jesHF_YearDown = *nr.MET_T1Smear_pt_jesHF_2016Down;
          WVJJTree->MET_jesRelativeSample_YearUp = *nr.MET_T1Smear_pt_jesRelativeSample_2016Up;
          WVJJTree->MET_jesRelativeSample_YearDown = *nr.MET_T1Smear_pt_jesRelativeSample_2016Down;

          WVJJTree->MET_phi_jesBBEC1_YearUp = *nr.MET_T1Smear_phi_jesBBEC1_2016Up;
          WVJJTree->MET_phi_jesBBEC1_YearDown = *nr.MET_T1Smear_phi_jesBBEC1_2016Down;
          WVJJTree->MET_phi_jesEC2_YearUp = *nr.MET_T1Smear_phi_jesEC2_2016Up;
          WVJJTree->MET_phi_jesEC2_YearDown = *nr.MET_T1Smear_phi_jesEC2_2016Down;
          WVJJTree->MET_phi_jesAbsolute_YearUp = *nr.MET_T1Smear_phi_jesAbsolute_2016Up;
          WVJJTree->MET_phi_jesAbsolute_YearDown = *nr.MET_T1Smear_phi_jesAbsolute_2016Down;
          WVJJTree->MET_phi_jesHF_YearUp = *nr.MET_T1Smear_phi_jesHF_2016Up;
          WVJJTree->MET_phi_jesHF_YearDown = *nr.MET_T1Smear_phi_jesHF_2016Down;
          WVJJTree->MET_phi_jesRelativeSample_YearUp = *nr.MET_T1Smear_phi_jesRelativeSample_2016Up;
          WVJJTree->MET_phi_jesRelativeSample_YearDown = *nr.MET_T1Smear_phi_jesRelativeSample_2016Down;
        }
        if (era==2017) {
          WVJJTree->MET_jesBBEC1_YearUp = *nr.MET_T1Smear_pt_jesBBEC1_2017Up;
          WVJJTree->MET_jesBBEC1_YearDown = *nr.MET_T1Smear_pt_jesBBEC1_2017Down;
          WVJJTree->MET_jesEC2_YearUp = *nr.MET_T1Smear_pt_jesEC2_2017Up;
          WVJJTree->MET_jesEC2_YearDown = *nr.MET_T1Smear_pt_jesEC2_2017Down;
          WVJJTree->MET_jesAbsolute_YearUp = *nr.MET_T1Smear_pt_jesAbsolute_2017Up;
          WVJJTree->MET_jesAbsolute_YearDown = *nr.MET_T1Smear_pt_jesAbsolute_2017Down;
          WVJJTree->MET_jesHF_YearUp = *nr.MET_T1Smear_pt_jesHF_2017Up;
          WVJJTree->MET_jesHF_YearDown = *nr.MET_T1Smear_pt_jesHF_2017Down;
          WVJJTree->MET_jesRelativeSample_YearUp = *nr.MET_T1Smear_pt_jesRelativeSample_2017Up;
          WVJJTree->MET_jesRelativeSample_YearDown = *nr.MET_T1Smear_pt_jesRelativeSample_2017Down;

          WVJJTree->MET_phi_jesBBEC1_YearUp = *nr.MET_T1Smear_phi_jesBBEC1_2017Up;
          WVJJTree->MET_phi_jesBBEC1_YearDown = *nr.MET_T1Smear_phi_jesBBEC1_2017Down;
          WVJJTree->MET_phi_jesEC2_YearUp = *nr.MET_T1Smear_phi_jesEC2_2017Up;
          WVJJTree->MET_phi_jesEC2_YearDown = *nr.MET_T1Smear_phi_jesEC2_2017Down;
          WVJJTree->MET_phi_jesAbsolute_YearUp = *nr.MET_T1Smear_phi_jesAbsolute_2017Up;
          WVJJTree->MET_phi_jesAbsolute_YearDown = *nr.MET_T1Smear_phi_jesAbsolute_2017Down;
          WVJJTree->MET_phi_jesHF_YearUp = *nr.MET_T1Smear_phi_jesHF_2017Up;
          WVJJTree->MET_phi_jesHF_YearDown = *nr.MET_T1Smear_phi_jesHF_2017Down;
          WVJJTree->MET_phi_jesRelativeSample_YearUp = *nr.MET_T1Smear_phi_jesRelativeSample_2017Up;
          WVJJTree->MET_phi_jesRelativeSample_YearDown = *nr.MET_T1Smear_phi_jesRelativeSample_2017Down;
        }
        if (era==2018) {
          WVJJTree->MET_jesBBEC1_YearUp = *nr.MET_T1Smear_pt_jesBBEC1_2018Up;
          WVJJTree->MET_jesBBEC1_YearDown = *nr.MET_T1Smear_pt_jesBBEC1_2018Down;
          WVJJTree->MET_jesEC2_YearUp = *nr.MET_T1Smear_pt_jesEC2_2018Up;
          WVJJTree->MET_jesEC2_YearDown = *nr.MET_T1Smear_pt_jesEC2_2018Down;
          WVJJTree->MET_jesAbsolute_YearUp = *nr.MET_T1Smear_pt_jesAbsolute_2018Up;
          WVJJTree->MET_jesAbsolute_YearDown = *nr.MET_T1Smear_pt_jesAbsolute_2018Down;
          WVJJTree->MET_jesHF_YearUp = *nr.MET_T1Smear_pt_jesHF_2018Up;
          WVJJTree->MET_jesHF_YearDown = *nr.MET_T1Smear_pt_jesHF_2018Down;
          WVJJTree->MET_jesRelativeSample_YearUp = *nr.MET_T1Smear_pt_jesRelativeSample_2018Up;
          WVJJTree->MET_jesRelativeSample_YearDown = *nr.MET_T1Smear_pt_jesRelativeSample_2018Down;

          WVJJTree->MET_phi_jesBBEC1_YearUp = *nr.MET_T1Smear_phi_jesBBEC1_2018Up;
          WVJJTree->MET_phi_jesBBEC1_YearDown = *nr.MET_T1Smear_phi_jesBBEC1_2018Down;
          WVJJTree->MET_phi_jesEC2_YearUp = *nr.MET_T1Smear_phi_jesEC2_2018Up;
          WVJJTree->MET_phi_jesEC2_YearDown = *nr.MET_T1Smear_phi_jesEC2_2018Down;
          WVJJTree->MET_phi_jesAbsolute_YearUp = *nr.MET_T1Smear_phi_jesAbsolute_2018Up;
          WVJJTree->MET_phi_jesAbsolute_YearDown = *nr.MET_T1Smear_phi_jesAbsolute_2018Down;
          WVJJTree->MET_phi_jesHF_YearUp = *nr.MET_T1Smear_phi_jesHF_2018Up;
          WVJJTree->MET_phi_jesHF_YearDown = *nr.MET_T1Smear_phi_jesHF_2018Down;
          WVJJTree->MET_phi_jesRelativeSample_YearUp = *nr.MET_T1Smear_phi_jesRelativeSample_2018Up;
          WVJJTree->MET_phi_jesRelativeSample_YearDown = *nr.MET_T1Smear_phi_jesRelativeSample_2018Down;
        }

        WVJJTree->MET_jesTotalUp = *nr.MET_T1Smear_pt_jesTotalUp;
        WVJJTree->MET_jesTotalDown = *nr.MET_T1Smear_pt_jesTotalDown;

        WVJJTree->MET_phi_jesTotalUp = *nr.MET_T1Smear_phi_jesTotalUp;
        WVJJTree->MET_phi_jesTotalDown = *nr.MET_T1Smear_phi_jesTotalDown;
      }


      if (WVJJTree->lep2_pt<0) {
        TLorentzVector lep1(0,0,0,0);
        lep1.SetPtEtaPhiM( WVJJTree->lep1_pt, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m );

        TLorentzVector tempMet(0,0,0,0);
        tempMet.SetPtEtaPhiM(WVJJTree->MET, 0.0, WVJJTree->MET_phi, 0.0);

        //TLorentzVector tempMet(0,0,0,0);
        //tempMet.SetPxPyPzE(WVJJTree->MET*TMath::Cos(WVJJTree->MET_phi),
        //                   WVJJTree->MET*TMath::Sin(WVJJTree->MET_phi),
        //                   0.0, WVJJTree->MET);

        //METzCalculator NeutrinoPz_type0;
        //NeutrinoPz_type0.SetMET(tempMet);
        //NeutrinoPz_type0.SetLepton(lep1);
        //NeutrinoPz_type0.SetLeptonType(WVJJTree->lep1_m == ELE_MASS ? "el" : "mu");

        //WVJJTree->neu_pz_type0 = NeutrinoPz_type0.Calculate();
        //tempMet.SetPxPyPzE(WVJJTree->MET_scaleUp*TMath::Cos(WVJJTree->MET_phi),
        //                   WVJJTree->MET_scaleUp*TMath::Sin(WVJJTree->MET_phi),
        //                   0.0, WVJJTree->MET_scaleUp);
        //NeutrinoPz_type0.SetMET(tempMet);
        //WVJJTree->neu_pz_type0_scaleUp = NeutrinoPz_type0.Calculate();

        //tempMet.SetPxPyPzE(WVJJTree->MET_scaleDown*TMath::Cos(WVJJTree->MET_phi),
        //                   WVJJTree->MET_scaleDown*TMath::Sin(WVJJTree->MET_phi),
        //                   0.0, WVJJTree->MET_scaleDown);
        //NeutrinoPz_type0.SetMET(tempMet);
        //WVJJTree->neu_pz_type0_scaleDown = NeutrinoPz_type0.Calculate();

        //tempMet.SetPxPyPzE(WVJJTree->MET*TMath::Cos(WVJJTree->MET_phi),
        //                   WVJJTree->MET*TMath::Sin(WVJJTree->MET_phi),
        //                   0.0, WVJJTree->MET);

        //TLorentzVector neutrino(0,0,0,0);
        //neutrino.SetPxPyPzE(tempMet.Px(), tempMet.Py(), WVJJTree->neu_pz_type0,
        //                    sqrt(tempMet.Px()*tempMet.Px()+tempMet.Py()*tempMet.Py()+ WVJJTree->neu_pz_type0* WVJJTree->neu_pz_type0));

        //TLorentzVector bosLep = lep1+neutrino;

        TLorentzVector bosLep = lep1+tempMet;
        WVJJTree->dilep_m = bosLep.M();
        WVJJTree->dilep_mt = bosLep.Mt();
        WVJJTree->dilep_pt = bosLep.Pt();
        WVJJTree->dilep_eta = bosLep.Eta();
        WVJJTree->dilep_phi = bosLep.Phi();

        //dilepton JES variations
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesFlavorQCDUp, 0.0, WVJJTree->MET_phi_jesFlavorQCDUp, 0.0);
        WVJJTree->dilep_m_jesFlavorQCDUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesFlavorQCDUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesFlavorQCDUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesFlavorQCDDown, 0.0, WVJJTree->MET_phi_jesFlavorQCDDown, 0.0);
        WVJJTree->dilep_m_jesFlavorQCDDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesFlavorQCDDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesFlavorQCDDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesRelativeBalUp, 0.0, WVJJTree->MET_phi_jesRelativeBalUp, 0.0);
        WVJJTree->dilep_m_jesRelativeBalUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesRelativeBalUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesRelativeBalUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesRelativeBalDown, 0.0, WVJJTree->MET_phi_jesRelativeBalDown, 0.0);
        WVJJTree->dilep_m_jesRelativeBalDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesRelativeBalDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesRelativeBalDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesHFUp, 0.0, WVJJTree->MET_phi_jesHFUp, 0.0);
        WVJJTree->dilep_m_jesHFUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesHFUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesHFUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesHFDown, 0.0, WVJJTree->MET_phi_jesHFDown, 0.0);
        WVJJTree->dilep_m_jesHFDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesHFDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesHFDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1Up, 0.0, WVJJTree->MET_phi_jesBBEC1Up, 0.0);
        WVJJTree->dilep_m_jesBBEC1Up = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesBBEC1Up = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesBBEC1Up = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1Down, 0.0, WVJJTree->MET_phi_jesBBEC1Down, 0.0);
        WVJJTree->dilep_m_jesBBEC1Down = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesBBEC1Down = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesBBEC1Down = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesEC2Up, 0.0, WVJJTree->MET_phi_jesEC2Up, 0.0);
        WVJJTree->dilep_m_jesEC2Up = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesEC2Up = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesEC2Up = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesEC2Down, 0.0, WVJJTree->MET_phi_jesEC2Down, 0.0);
        WVJJTree->dilep_m_jesEC2Down = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesEC2Down = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesEC2Down = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesAbsoluteUp, 0.0, WVJJTree->MET_phi_jesAbsoluteUp, 0.0);
        WVJJTree->dilep_m_jesAbsoluteUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesAbsoluteUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesAbsoluteUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesAbsoluteDown, 0.0, WVJJTree->MET_phi_jesAbsoluteDown, 0.0);
        WVJJTree->dilep_m_jesAbsoluteDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesAbsoluteDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesAbsoluteDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1_YearUp, 0.0, WVJJTree->MET_phi_jesBBEC1_YearUp, 0.0);
        WVJJTree->dilep_m_jesBBEC1_YearUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesBBEC1_YearUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesBBEC1_YearUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1_YearDown, 0.0, WVJJTree->MET_phi_jesBBEC1_YearDown, 0.0);
        WVJJTree->dilep_m_jesBBEC1_YearDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesBBEC1_YearDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesBBEC1_YearDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesEC2_YearUp, 0.0, WVJJTree->MET_phi_jesEC2_YearUp, 0.0);
        WVJJTree->dilep_m_jesEC2_YearUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesEC2_YearUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesEC2_YearUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesEC2_YearDown, 0.0, WVJJTree->MET_phi_jesEC2_YearDown, 0.0);
        WVJJTree->dilep_m_jesEC2_YearDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesEC2_YearDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesEC2_YearDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesAbsolute_YearUp, 0.0, WVJJTree->MET_phi_jesAbsolute_YearUp, 0.0);
        WVJJTree->dilep_m_jesAbsolute_YearUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesAbsolute_YearUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesAbsolute_YearUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesAbsolute_YearDown, 0.0, WVJJTree->MET_phi_jesAbsolute_YearDown, 0.0);
        WVJJTree->dilep_m_jesAbsolute_YearDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesAbsolute_YearDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesAbsolute_YearDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesHF_YearUp, 0.0, WVJJTree->MET_phi_jesHF_YearUp, 0.0);
        WVJJTree->dilep_m_jesHF_YearUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesHF_YearUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesHF_YearUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesHF_YearDown, 0.0, WVJJTree->MET_phi_jesHF_YearDown, 0.0);
        WVJJTree->dilep_m_jesHF_YearDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesHF_YearDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesHF_YearDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesRelativeSample_YearUp, 0.0, WVJJTree->MET_phi_jesRelativeSample_YearUp, 0.0);
        WVJJTree->dilep_m_jesRelativeSample_YearUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesRelativeSample_YearUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesRelativeSample_YearUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesRelativeSample_YearDown, 0.0, WVJJTree->MET_phi_jesRelativeSample_YearDown, 0.0);
        WVJJTree->dilep_m_jesRelativeSample_YearDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesRelativeSample_YearDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesRelativeSample_YearDown = (lep1+tempMet).Pt();

        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesTotalUp, 0.0, WVJJTree->MET_phi_jesTotalUp, 0.0);
        WVJJTree->dilep_m_jesTotalUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesTotalUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesTotalUp = (lep1+tempMet).Pt();
        tempMet.SetPtEtaPhiM(WVJJTree->MET_jesTotalDown, 0.0, WVJJTree->MET_phi_jesTotalDown, 0.0);
        WVJJTree->dilep_m_jesTotalDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_jesTotalDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_jesTotalDown = (lep1+tempMet).Pt();

        //dilepton lepton pt scale variations
        tempMet.SetPtEtaPhiM(WVJJTree->MET, 0.0, WVJJTree->MET_phi, 0.0);

        lep1.SetPtEtaPhiM( WVJJTree->lep1_pt_scaleUp, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
        WVJJTree->dilep_m_scaleUp = (lep1+tempMet).M();
        WVJJTree->dilep_mt_scaleUp = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_scaleUp = (lep1+tempMet).Pt();

        lep1.SetPtEtaPhiM( WVJJTree->lep1_pt_scaleDown, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
        WVJJTree->dilep_m_scaleDown = (lep1+tempMet).M();
        WVJJTree->dilep_mt_scaleDown = (lep1+tempMet).Mt();
        WVJJTree->dilep_pt_scaleDown = (lep1+tempMet).Pt();
      }

      // AK8

      float dmW = 3000.0;
      int nGoodFatJet=0;

      for (uint j=0; j<*nr.nFatJet; j++) {

        if ( fabs(nr.FatJet_eta[j]) > AK8_MAX_ETA ) continue;

        if ( isMC ) {
          // maybe individuals?
          if ( ! (nr.FatJet_pt_nom[j]>AK8_MIN_PT ||
                  nr.FatJet_pt_jesTotalUp[j]>AK8_MIN_PT || nr.FatJet_pt_jesTotalDown[j]>AK8_MIN_PT) ) continue;

          // soft drop mass: No JES variation
          if ( nr.FatJet_msoftdrop[j]<AK8_MIN_SDM ) continue;
          if ( nr.FatJet_msoftdrop[j]>AK8_MAX_SDM ) continue;

        } else {
          if ( nr.FatJet_pt_nom[j]<AK8_MIN_PT ) continue;
          if ( nr.FatJet_msoftdrop[j]<AK8_MIN_SDM ) continue;
          if ( nr.FatJet_msoftdrop[j]>AK8_MAX_SDM ) continue;
        }

        bool isClean=true;
        //lepton cleaning
        for ( std::size_t k=0; k<tightEle.size(); k++) {
          if (deltaR(tightEle.at(k).Eta(), tightEle.at(k).Phi(),
                     nr.FatJet_eta[j], nr.FatJet_phi[j]) < AK8_LEP_DR_CUT)
          isClean = false;
        }
        for ( std::size_t k=0; k<tightMuon.size(); k++) {
          if (deltaR(tightMuon.at(k).Eta(), tightMuon.at(k).Phi(),
                     nr.FatJet_eta[j], nr.FatJet_phi[j]) < AK8_LEP_DR_CUT)
          isClean = false;
        }

        if ( isClean == false ) continue;

        if ( fabs(nr.FatJet_msoftdrop[j] - W_MASS) > dmW ) continue;

        WVJJTree->bos_PuppiAK8_m_sd0 = nr.FatJet_msoftdrop[j];
        WVJJTree->bos_PuppiAK8_m_sd0_corr = nr.FatJet_msoftdrop[j];
        WVJJTree->bos_PuppiAK8_tau2tau1 = nr.FatJet_tau2[j]/nr.FatJet_tau1[j];
        WVJJTree->bos_PuppiAK8_pt = nr.FatJet_pt[j];
        WVJJTree->bos_PuppiAK8_eta = nr.FatJet_eta[j];
        WVJJTree->bos_PuppiAK8_phi = nr.FatJet_phi[j];

        if (isMC) {
          WVJJTree->bos_PuppiAK8_pt_jesFlavorQCDUp = nr.FatJet_pt_jesFlavorQCDUp[j];
          WVJJTree->bos_PuppiAK8_pt_jesFlavorQCDDown = nr.FatJet_pt_jesFlavorQCDDown[j];
          WVJJTree->bos_PuppiAK8_pt_jesRelativeBalUp = nr.FatJet_pt_jesRelativeBalUp[j];
          WVJJTree->bos_PuppiAK8_pt_jesRelativeBalDown = nr.FatJet_pt_jesRelativeBalDown[j];
          WVJJTree->bos_PuppiAK8_pt_jesHFUp = nr.FatJet_pt_jesHFUp[j];
          WVJJTree->bos_PuppiAK8_pt_jesHFDown = nr.FatJet_pt_jesHFDown[j];
          WVJJTree->bos_PuppiAK8_pt_jesBBEC1Up = nr.FatJet_pt_jesBBEC1Up[j];
          WVJJTree->bos_PuppiAK8_pt_jesBBEC1Down = nr.FatJet_pt_jesBBEC1Down[j];
          WVJJTree->bos_PuppiAK8_pt_jesEC2Up = nr.FatJet_pt_jesEC2Up[j];
          WVJJTree->bos_PuppiAK8_pt_jesEC2Down = nr.FatJet_pt_jesEC2Down[j];
          WVJJTree->bos_PuppiAK8_pt_jesAbsoluteUp = nr.FatJet_pt_jesAbsoluteUp[j];
          WVJJTree->bos_PuppiAK8_pt_jesAbsoluteDown = nr.FatJet_pt_jesAbsoluteDown[j];

          if (era==2016) {
            WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearUp = nr.FatJet_pt_jesBBEC1_2016Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearDown = nr.FatJet_pt_jesBBEC1_2016Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesEC2_YearUp = nr.FatJet_pt_jesEC2_2016Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesEC2_YearDown = nr.FatJet_pt_jesEC2_2016Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearUp = nr.FatJet_pt_jesAbsolute_2016Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearDown = nr.FatJet_pt_jesAbsolute_2016Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesHF_YearUp = nr.FatJet_pt_jesHF_2016Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesHF_YearDown = nr.FatJet_pt_jesHF_2016Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearUp = nr.FatJet_pt_jesRelativeSample_2016Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearDown = nr.FatJet_pt_jesRelativeSample_2016Down[j];
          }
          if (era==2017) {
            WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearUp = nr.FatJet_pt_jesBBEC1_2017Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearDown = nr.FatJet_pt_jesBBEC1_2017Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesEC2_YearUp = nr.FatJet_pt_jesEC2_2017Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesEC2_YearDown = nr.FatJet_pt_jesEC2_2017Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearUp = nr.FatJet_pt_jesAbsolute_2017Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearDown = nr.FatJet_pt_jesAbsolute_2017Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesHF_YearUp = nr.FatJet_pt_jesHF_2017Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesHF_YearDown = nr.FatJet_pt_jesHF_2017Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearUp = nr.FatJet_pt_jesRelativeSample_2017Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearDown = nr.FatJet_pt_jesRelativeSample_2017Down[j];
          }
          if (era==2018) {
            WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearUp = nr.FatJet_pt_jesBBEC1_2018Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearDown = nr.FatJet_pt_jesBBEC1_2018Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesEC2_YearUp = nr.FatJet_pt_jesEC2_2018Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesEC2_YearDown = nr.FatJet_pt_jesEC2_2018Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearUp = nr.FatJet_pt_jesAbsolute_2018Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearDown = nr.FatJet_pt_jesAbsolute_2018Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesHF_YearUp = nr.FatJet_pt_jesHF_2018Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesHF_YearDown = nr.FatJet_pt_jesHF_2018Down[j];
            WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearUp = nr.FatJet_pt_jesRelativeSample_2018Up[j];
            WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearDown = nr.FatJet_pt_jesRelativeSample_2018Down[j];
          }

          WVJJTree->bos_PuppiAK8_pt_jesTotalUp = nr.FatJet_pt_jesTotalUp[j];
          WVJJTree->bos_PuppiAK8_pt_jesTotalDown = nr.FatJet_pt_jesTotalDown[j];
        }

        dmW = fabs(nr.FatJet_msoftdrop[j] - W_MASS);
        nGoodFatJet++;
      }

      goodJetIndex.clear();
      for (uint j=0; j<*nr.nJet; j++) {

        //jet energy scale variations
        if ( isMC && ( nr.Jet_pt_nom[j] < AK4_PT_CUT && nr.Jet_pt_jesTotalUp[j] < AK4_PT_CUT &&
                      nr.Jet_pt_jesTotalDown[j] < AK4_PT_CUT ) ) continue;
        else if ( !isMC && nr.Jet_pt_nom[j] < AK4_PT_CUT ) continue;
        //jet ID??

        //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
        float btag_eff_loose = 1.0;
        float btag_eff_medium = 1.0;
        float btag_eff_tight = 1.0;

        if (fabs(nr.Jet_eta[j])<2.4 && nr.Jet_pt_nom[j]>30) {

          if (isMC) {
            btag_eff_loose = scaleFactor.GetBtagEff(nr.Jet_hadronFlavour[j], nr.Jet_pt_nom[j], nr.Jet_eta[j], "loose");
            btag_eff_medium = scaleFactor.GetBtagEff(nr.Jet_hadronFlavour[j], nr.Jet_pt_nom[j], nr.Jet_eta[j], "medium");
            btag_eff_tight = scaleFactor.GetBtagEff(nr.Jet_hadronFlavour[j], nr.Jet_pt_nom[j], nr.Jet_eta[j], "tight");
          }

          if (nr.Jet_btagDeepB[j] > btag_loose_wp) {
            WVJJTree->nBtag_loose++;
            if (isMC) {
              WVJJTree->btagWeight_loose *= nr.Jet_btagSF_deepcsv_L[j];
              WVJJTree->btagWeight_loose_Up *= nr.Jet_btagSF_deepcsv_L_up[j];
              WVJJTree->btagWeight_loose_Down *= nr.Jet_btagSF_deepcsv_L_down[j];
            }
          }
          else {
            if (isMC) {
              if (btag_eff_loose == 1.0) {
                WVJJTree->btagWeight_loose *= nr.Jet_btagSF_deepcsv_L[j];
                WVJJTree->btagWeight_loose_Up *= nr.Jet_btagSF_deepcsv_L_up[j];
                WVJJTree->btagWeight_loose_Down *= nr.Jet_btagSF_deepcsv_L_down[j];
              }
              else {
                WVJJTree->btagWeight_loose *= (1.0 - nr.Jet_btagSF_deepcsv_L[j] * btag_eff_loose) / (1.0 - btag_eff_loose);
                WVJJTree->btagWeight_loose_Up *= (1.0 - nr.Jet_btagSF_deepcsv_L_up[j] * btag_eff_loose) / (1.0 - btag_eff_loose);
                WVJJTree->btagWeight_loose_Down *= (1.0 - nr.Jet_btagSF_deepcsv_L_down[j] * btag_eff_loose) / (1.0 - btag_eff_loose);
              }
            }
          }
          if (nr.Jet_btagDeepB[j] > btag_medium_wp) {
            WVJJTree->nBtag_medium++;
            if (isMC) {
              WVJJTree->btagWeight_medium *= nr.Jet_btagSF_deepcsv_M[j];
              WVJJTree->btagWeight_medium_Up *= nr.Jet_btagSF_deepcsv_M_up[j];
              WVJJTree->btagWeight_medium_Down *= nr.Jet_btagSF_deepcsv_M_down[j];
            }
          }
          else {
            if (isMC) {
              if (btag_eff_medium == 1.0) {
                WVJJTree->btagWeight_medium *= nr.Jet_btagSF_deepcsv_M[j];
                WVJJTree->btagWeight_medium_Up *= nr.Jet_btagSF_deepcsv_M_up[j];
                WVJJTree->btagWeight_medium_Down *= nr.Jet_btagSF_deepcsv_M_down[j];
              }
              else {
                WVJJTree->btagWeight_medium *= (1.0 - nr.Jet_btagSF_deepcsv_M[j] * btag_eff_medium) / (1.0 - btag_eff_medium);
                WVJJTree->btagWeight_medium_Up *= (1.0 - nr.Jet_btagSF_deepcsv_M_up[j] * btag_eff_medium) / (1.0 - btag_eff_medium);
                WVJJTree->btagWeight_medium_Down *= (1.0 - nr.Jet_btagSF_deepcsv_M_down[j] * btag_eff_medium) / (1.0 - btag_eff_medium);
              }
            }
          }
          if (nr.Jet_btagDeepB[j] > btag_tight_wp) {
            WVJJTree->nBtag_tight++;
            if (isMC) {
              WVJJTree->btagWeight_tight *= nr.Jet_btagSF_deepcsv_T[j];
              WVJJTree->btagWeight_tight_Up *= nr.Jet_btagSF_deepcsv_T_up[j];
              WVJJTree->btagWeight_tight_Down *= nr.Jet_btagSF_deepcsv_T_down[j];
            }
          }
          else {
            if (isMC) {
              if (btag_eff_tight == 1.0) {
                WVJJTree->btagWeight_tight *= nr.Jet_btagSF_deepcsv_T[j];
                WVJJTree->btagWeight_tight_Up *= nr.Jet_btagSF_deepcsv_T_up[j];
                WVJJTree->btagWeight_tight_Down *= nr.Jet_btagSF_deepcsv_T_down[j];
              }
              else {
                WVJJTree->btagWeight_tight *= (1.0 - nr.Jet_btagSF_deepcsv_T[j] * btag_eff_tight) / (1.0 - btag_eff_tight);
                WVJJTree->btagWeight_tight_Up *= (1.0 - nr.Jet_btagSF_deepcsv_T_up[j] * btag_eff_tight) / (1.0 - btag_eff_tight);
                WVJJTree->btagWeight_tight_Down *= (1.0 - nr.Jet_btagSF_deepcsv_T_down[j] * btag_eff_tight) / (1.0 - btag_eff_tight);
              }
            }
          }
        }


        bool isClean=true;

        // object cleaning
        if (nGoodFatJet>0) {
          if (deltaR(WVJJTree->bos_PuppiAK8_eta, WVJJTree->bos_PuppiAK8_phi,
                     nr.Jet_eta[j], nr.Jet_phi[j]) < AK4_AK8_DR_CUT) {
            isClean = false;
          }
        }

        for ( std::size_t k=0; k<goodJetIndex.size(); k++) {
          if (deltaR(nr.Jet_eta[goodJetIndex.at(k)], nr.Jet_phi[goodJetIndex.at(k)],
                     nr.Jet_eta[j], nr.Jet_phi[j]) < AK4_DR_CUT) {
            isClean = false;
          }
        }

        for ( std::size_t k=0; k<tightEle.size(); k++) {
          if (deltaR(tightEle.at(k).Eta(), tightEle.at(k).Phi(),
                     nr.Jet_eta[j], nr.Jet_phi[j]) < AK4_DR_CUT) {
            isClean = false;
          }
        }

        for ( std::size_t k=0; k<tightMuon.size(); k++) {
          if (deltaR(tightMuon.at(k).Eta(), tightMuon.at(k).Phi(),
                     nr.Jet_eta[j], nr.Jet_phi[j]) < AK4_DR_CUT) {
            isClean = false;
          }
        }

        if ( isClean == false ) continue;
        if (nr.Jet_pt_nom[j]>30) WVJJTree->nJet30++;
        if (nr.Jet_pt_nom[j]>50) WVJJTree->nJet50++;

        goodJetIndex.push_back(j);
      }

      int nGoodDijet=0;

      uint sel1=1000, sel2=1000;
      if (nGoodFatJet==0) {
        TLorentzVector tmpV1, tmpV2;
        dmW=3000.0;

        for (uint j=0; j<goodJetIndex.size(); j++) {
          if ( fabs( nr.Jet_eta[goodJetIndex.at(j)] ) > AK4_ETA_CUT ) continue;

          for(uint k=j+1; k<goodJetIndex.size(); k++) {
            if ( fabs( nr.Jet_eta[goodJetIndex.at(k)] ) > AK4_ETA_CUT ) continue;

            TLorentzVector tmp1(0,0,0,0);
            tmp1.SetPtEtaPhiM( nr.Jet_pt_nom[goodJetIndex.at(j)], nr.Jet_eta[goodJetIndex.at(j)],
                              nr.Jet_phi[goodJetIndex.at(j)], nr.Jet_mass[goodJetIndex.at(j)] );

            TLorentzVector tmp2(0,0,0,0);
            tmp2.SetPtEtaPhiM( nr.Jet_pt_nom[goodJetIndex.at(k)], nr.Jet_eta[goodJetIndex.at(k)],
                              nr.Jet_phi[goodJetIndex.at(k)], nr.Jet_mass[goodJetIndex.at(k)] );

            TLorentzVector tmpV=tmp1+tmp2;

            if (tmpV.M()<AK4_JJ_MIN_M || tmpV.M()>AK4_JJ_MAX_M) continue;

            if (fabs(tmpV.M()-W_MASS)>dmW) continue;

            WVJJTree->bos_j1_AK4_pt =  nr.Jet_pt_nom[goodJetIndex.at(j)];
            WVJJTree->bos_j1_AK4_eta = nr.Jet_eta[goodJetIndex.at(j)];
            WVJJTree->bos_j1_AK4_phi = nr.Jet_phi[goodJetIndex.at(j)];
            WVJJTree->bos_j1_AK4_m =   nr.Jet_mass[goodJetIndex.at(j)];
            WVJJTree->bos_j1_AK4_qgid = nr.Jet_qgl[goodJetIndex.at(j)];
            WVJJTree->bos_j1_AK4_puid_tight = nr.Jet_puId[goodJetIndex.at(j)] == 7 ? true : false;
            if (isMC) {
              WVJJTree->bos_j1_AK4_puidSF_tight = nr.Jet_PUIDSF_tight[goodJetIndex.at(j)];
              WVJJTree->bos_j1_AK4_puidSF_tight_Up = nr.Jet_PUIDSF_tight_up[goodJetIndex.at(j)];
              WVJJTree->bos_j1_AK4_puidSF_tight_Down = nr.Jet_PUIDSF_tight_down[goodJetIndex.at(j)];
            }

            WVJJTree->bos_j2_AK4_pt =  nr.Jet_pt_nom[goodJetIndex.at(k)];
            WVJJTree->bos_j2_AK4_eta = nr.Jet_eta[goodJetIndex.at(k)];
            WVJJTree->bos_j2_AK4_phi = nr.Jet_phi[goodJetIndex.at(k)];
            WVJJTree->bos_j2_AK4_m =   nr.Jet_mass[goodJetIndex.at(k)];
            WVJJTree->bos_j2_AK4_qgid = nr.Jet_qgl[goodJetIndex.at(k)];
            WVJJTree->bos_j2_AK4_puid_tight = nr.Jet_puId[goodJetIndex.at(k)] == 7 ? true : false;
            if (isMC) {
              WVJJTree->bos_j2_AK4_puidSF_tight = nr.Jet_PUIDSF_tight[goodJetIndex.at(k)];
              WVJJTree->bos_j2_AK4_puidSF_tight_Up = nr.Jet_PUIDSF_tight_up[goodJetIndex.at(k)];
              WVJJTree->bos_j2_AK4_puidSF_tight_Down = nr.Jet_PUIDSF_tight_down[goodJetIndex.at(k)];
            }

            WVJJTree->bos_AK4AK4_pt =  tmpV.Pt();
            WVJJTree->bos_AK4AK4_eta = tmpV.Eta();
            WVJJTree->bos_AK4AK4_phi = tmpV.Phi();
            WVJJTree->bos_AK4AK4_m =   tmpV.M();

            sel1=j; sel2=k;
            dmW=fabs(tmpV.M()-W_MASS);
            nGoodDijet=1;
          }
        }

        if (nGoodDijet==0) continue;

        if (isMC) {
          // resolved boson JES variation
          WVJJTree->bos_j1_AK4_pt_jesFlavorQCDUp = nr.Jet_pt_jesFlavorQCDUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesFlavorQCDDown = nr.Jet_pt_jesFlavorQCDDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesRelativeBalUp = nr.Jet_pt_jesRelativeBalUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesRelativeBalDown = nr.Jet_pt_jesRelativeBalDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesHFUp = nr.Jet_pt_jesHFUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesHFDown = nr.Jet_pt_jesHFDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesBBEC1Up = nr.Jet_pt_jesBBEC1Up[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesBBEC1Down = nr.Jet_pt_jesBBEC1Down[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesEC2Up = nr.Jet_pt_jesEC2Up[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesEC2Down = nr.Jet_pt_jesEC2Down[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesAbsoluteUp = nr.Jet_pt_jesAbsoluteUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesAbsoluteDown = nr.Jet_pt_jesAbsoluteDown[goodJetIndex.at(sel1)];

          WVJJTree->bos_j1_AK4_m_jesFlavorQCDUp = nr.Jet_mass_jesFlavorQCDUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesFlavorQCDDown = nr.Jet_mass_jesFlavorQCDDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesRelativeBalUp = nr.Jet_mass_jesRelativeBalUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesRelativeBalDown = nr.Jet_mass_jesRelativeBalDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesHFUp = nr.Jet_mass_jesHFUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesHFDown = nr.Jet_mass_jesHFDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesBBEC1Up = nr.Jet_mass_jesBBEC1Up[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesBBEC1Down = nr.Jet_mass_jesBBEC1Down[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesEC2Up = nr.Jet_mass_jesEC2Up[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesEC2Down = nr.Jet_mass_jesEC2Down[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesAbsoluteUp = nr.Jet_mass_jesAbsoluteUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesAbsoluteDown = nr.Jet_mass_jesAbsoluteDown[goodJetIndex.at(sel1)];

          WVJJTree->bos_j2_AK4_pt_jesFlavorQCDUp = nr.Jet_pt_jesFlavorQCDUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesFlavorQCDDown = nr.Jet_pt_jesFlavorQCDDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesRelativeBalUp = nr.Jet_pt_jesRelativeBalUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesRelativeBalDown = nr.Jet_pt_jesRelativeBalDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesHFUp = nr.Jet_pt_jesHFUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesHFDown = nr.Jet_pt_jesHFDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesBBEC1Up = nr.Jet_pt_jesBBEC1Up[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesBBEC1Down = nr.Jet_pt_jesBBEC1Down[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesEC2Up = nr.Jet_pt_jesEC2Up[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesEC2Down = nr.Jet_pt_jesEC2Down[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesAbsoluteUp = nr.Jet_pt_jesAbsoluteUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesAbsoluteDown = nr.Jet_pt_jesAbsoluteDown[goodJetIndex.at(sel2)];

          WVJJTree->bos_j2_AK4_m_jesFlavorQCDUp = nr.Jet_mass_jesFlavorQCDUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesFlavorQCDDown = nr.Jet_mass_jesFlavorQCDDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesRelativeBalUp = nr.Jet_mass_jesRelativeBalUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesRelativeBalDown = nr.Jet_mass_jesRelativeBalDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesHFUp = nr.Jet_mass_jesHFUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesHFDown = nr.Jet_mass_jesHFDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesBBEC1Up = nr.Jet_mass_jesBBEC1Up[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesBBEC1Down = nr.Jet_mass_jesBBEC1Down[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesEC2Up = nr.Jet_mass_jesEC2Up[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesEC2Down = nr.Jet_mass_jesEC2Down[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesAbsoluteUp = nr.Jet_mass_jesAbsoluteUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesAbsoluteDown = nr.Jet_mass_jesAbsoluteDown[goodJetIndex.at(sel2)];

          if (era==2016) {
            WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearUp = nr.Jet_pt_jesBBEC1_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearDown = nr.Jet_pt_jesBBEC1_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesEC2_YearUp = nr.Jet_pt_jesEC2_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesEC2_YearDown = nr.Jet_pt_jesEC2_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearUp = nr.Jet_pt_jesAbsolute_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearDown = nr.Jet_pt_jesAbsolute_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesHF_YearUp = nr.Jet_pt_jesHF_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesHF_YearDown = nr.Jet_pt_jesHF_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearUp = nr.Jet_pt_jesRelativeSample_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearDown = nr.Jet_pt_jesRelativeSample_2016Down[goodJetIndex.at(sel1)];

            WVJJTree->bos_j1_AK4_m_jesBBEC1_YearUp = nr.Jet_mass_jesBBEC1_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesBBEC1_YearDown = nr.Jet_mass_jesBBEC1_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesEC2_YearUp = nr.Jet_mass_jesEC2_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesEC2_YearDown = nr.Jet_mass_jesEC2_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesAbsolute_YearUp = nr.Jet_mass_jesAbsolute_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesAbsolute_YearDown = nr.Jet_mass_jesAbsolute_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesHF_YearUp = nr.Jet_mass_jesHF_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesHF_YearDown = nr.Jet_mass_jesHF_2016Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearUp = nr.Jet_mass_jesRelativeSample_2016Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearDown = nr.Jet_mass_jesRelativeSample_2016Down[goodJetIndex.at(sel1)];

            WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearUp = nr.Jet_pt_jesBBEC1_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearDown = nr.Jet_pt_jesBBEC1_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesEC2_YearUp = nr.Jet_pt_jesEC2_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesEC2_YearDown = nr.Jet_pt_jesEC2_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearUp = nr.Jet_pt_jesAbsolute_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearDown = nr.Jet_pt_jesAbsolute_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesHF_YearUp = nr.Jet_pt_jesHF_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesHF_YearDown = nr.Jet_pt_jesHF_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearUp = nr.Jet_pt_jesRelativeSample_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearDown = nr.Jet_pt_jesRelativeSample_2016Down[goodJetIndex.at(sel2)];

            WVJJTree->bos_j2_AK4_m_jesBBEC1_YearUp = nr.Jet_mass_jesBBEC1_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesBBEC1_YearDown = nr.Jet_mass_jesBBEC1_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesEC2_YearUp = nr.Jet_mass_jesEC2_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesEC2_YearDown = nr.Jet_mass_jesEC2_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesAbsolute_YearUp = nr.Jet_mass_jesAbsolute_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesAbsolute_YearDown = nr.Jet_mass_jesAbsolute_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesHF_YearUp = nr.Jet_mass_jesHF_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesHF_YearDown = nr.Jet_mass_jesHF_2016Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearUp = nr.Jet_mass_jesRelativeSample_2016Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearDown = nr.Jet_mass_jesRelativeSample_2016Down[goodJetIndex.at(sel2)];
          }
          if (era==2017) {
            WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearUp = nr.Jet_pt_jesBBEC1_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearDown = nr.Jet_pt_jesBBEC1_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesEC2_YearUp = nr.Jet_pt_jesEC2_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesEC2_YearDown = nr.Jet_pt_jesEC2_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearUp = nr.Jet_pt_jesAbsolute_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearDown = nr.Jet_pt_jesAbsolute_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesHF_YearUp = nr.Jet_pt_jesHF_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesHF_YearDown = nr.Jet_pt_jesHF_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearUp = nr.Jet_pt_jesRelativeSample_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearDown = nr.Jet_pt_jesRelativeSample_2017Down[goodJetIndex.at(sel1)];

            WVJJTree->bos_j1_AK4_m_jesBBEC1_YearUp = nr.Jet_mass_jesBBEC1_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesBBEC1_YearDown = nr.Jet_mass_jesBBEC1_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesEC2_YearUp = nr.Jet_mass_jesEC2_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesEC2_YearDown = nr.Jet_mass_jesEC2_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesAbsolute_YearUp = nr.Jet_mass_jesAbsolute_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesAbsolute_YearDown = nr.Jet_mass_jesAbsolute_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesHF_YearUp = nr.Jet_mass_jesHF_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesHF_YearDown = nr.Jet_mass_jesHF_2017Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearUp = nr.Jet_mass_jesRelativeSample_2017Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearDown = nr.Jet_mass_jesRelativeSample_2017Down[goodJetIndex.at(sel1)];

            WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearUp = nr.Jet_pt_jesBBEC1_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearDown = nr.Jet_pt_jesBBEC1_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesEC2_YearUp = nr.Jet_pt_jesEC2_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesEC2_YearDown = nr.Jet_pt_jesEC2_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearUp = nr.Jet_pt_jesAbsolute_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearDown = nr.Jet_pt_jesAbsolute_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesHF_YearUp = nr.Jet_pt_jesHF_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesHF_YearDown = nr.Jet_pt_jesHF_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearUp = nr.Jet_pt_jesRelativeSample_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearDown = nr.Jet_pt_jesRelativeSample_2017Down[goodJetIndex.at(sel2)];

            WVJJTree->bos_j2_AK4_m_jesBBEC1_YearUp = nr.Jet_mass_jesBBEC1_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesBBEC1_YearDown = nr.Jet_mass_jesBBEC1_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesEC2_YearUp = nr.Jet_mass_jesEC2_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesEC2_YearDown = nr.Jet_mass_jesEC2_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesAbsolute_YearUp = nr.Jet_mass_jesAbsolute_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesAbsolute_YearDown = nr.Jet_mass_jesAbsolute_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesHF_YearUp = nr.Jet_mass_jesHF_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesHF_YearDown = nr.Jet_mass_jesHF_2017Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearUp = nr.Jet_mass_jesRelativeSample_2017Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearDown = nr.Jet_mass_jesRelativeSample_2017Down[goodJetIndex.at(sel2)];
          }
          if (era==2018) {
            WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearUp = nr.Jet_pt_jesBBEC1_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearDown = nr.Jet_pt_jesBBEC1_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesEC2_YearUp = nr.Jet_pt_jesEC2_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesEC2_YearDown = nr.Jet_pt_jesEC2_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearUp = nr.Jet_pt_jesAbsolute_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearDown = nr.Jet_pt_jesAbsolute_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesHF_YearUp = nr.Jet_pt_jesHF_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesHF_YearDown = nr.Jet_pt_jesHF_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearUp = nr.Jet_pt_jesRelativeSample_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearDown = nr.Jet_pt_jesRelativeSample_2018Down[goodJetIndex.at(sel1)];

            WVJJTree->bos_j1_AK4_m_jesBBEC1_YearUp = nr.Jet_mass_jesBBEC1_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesBBEC1_YearDown = nr.Jet_mass_jesBBEC1_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesEC2_YearUp = nr.Jet_mass_jesEC2_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesEC2_YearDown = nr.Jet_mass_jesEC2_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesAbsolute_YearUp = nr.Jet_mass_jesAbsolute_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesAbsolute_YearDown = nr.Jet_mass_jesAbsolute_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesHF_YearUp = nr.Jet_mass_jesHF_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesHF_YearDown = nr.Jet_mass_jesHF_2018Down[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearUp = nr.Jet_mass_jesRelativeSample_2018Up[goodJetIndex.at(sel1)];
            WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearDown = nr.Jet_mass_jesRelativeSample_2018Down[goodJetIndex.at(sel1)];

            WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearUp = nr.Jet_pt_jesBBEC1_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearDown = nr.Jet_pt_jesBBEC1_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesEC2_YearUp = nr.Jet_pt_jesEC2_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesEC2_YearDown = nr.Jet_pt_jesEC2_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearUp = nr.Jet_pt_jesAbsolute_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearDown = nr.Jet_pt_jesAbsolute_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesHF_YearUp = nr.Jet_pt_jesHF_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesHF_YearDown = nr.Jet_pt_jesHF_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearUp = nr.Jet_pt_jesRelativeSample_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearDown = nr.Jet_pt_jesRelativeSample_2018Down[goodJetIndex.at(sel2)];

            WVJJTree->bos_j2_AK4_m_jesBBEC1_YearUp = nr.Jet_mass_jesBBEC1_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesBBEC1_YearDown = nr.Jet_mass_jesBBEC1_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesEC2_YearUp = nr.Jet_mass_jesEC2_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesEC2_YearDown = nr.Jet_mass_jesEC2_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesAbsolute_YearUp = nr.Jet_mass_jesAbsolute_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesAbsolute_YearDown = nr.Jet_mass_jesAbsolute_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesHF_YearUp = nr.Jet_mass_jesHF_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesHF_YearDown = nr.Jet_mass_jesHF_2018Down[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearUp = nr.Jet_mass_jesRelativeSample_2018Up[goodJetIndex.at(sel2)];
            WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearDown = nr.Jet_mass_jesRelativeSample_2018Down[goodJetIndex.at(sel2)];
          }

          WVJJTree->bos_j1_AK4_pt_jesTotalUp = nr.Jet_pt_jesTotalUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_pt_jesTotalDown = nr.Jet_pt_jesTotalDown[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesTotalUp = nr.Jet_mass_jesTotalUp[goodJetIndex.at(sel1)];
          WVJJTree->bos_j1_AK4_m_jesTotalDown = nr.Jet_mass_jesTotalDown[goodJetIndex.at(sel1)];

          WVJJTree->bos_j2_AK4_pt_jesTotalUp = nr.Jet_pt_jesTotalUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_pt_jesTotalDown = nr.Jet_pt_jesTotalDown[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesTotalUp = nr.Jet_mass_jesTotalUp[goodJetIndex.at(sel2)];
          WVJJTree->bos_j2_AK4_m_jesTotalDown = nr.Jet_mass_jesTotalDown[goodJetIndex.at(sel2)];

          // resolved boson candidate JES variation
          TLorentzVector tempBos1(0,0,0,0);
          TLorentzVector tempBos2(0,0,0,0);

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesFlavorQCDUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesFlavorQCDUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesFlavorQCDUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesFlavorQCDUp);

          WVJJTree->bos_AK4AK4_pt_jesFlavorQCDUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesFlavorQCDUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesFlavorQCDDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesFlavorQCDDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesFlavorQCDDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesFlavorQCDDown);

          WVJJTree->bos_AK4AK4_pt_jesFlavorQCDDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesFlavorQCDDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesRelativeBalUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesRelativeBalUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesRelativeBalUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesRelativeBalUp);

          WVJJTree->bos_AK4AK4_pt_jesRelativeBalUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesRelativeBalUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesRelativeBalDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesRelativeBalDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesRelativeBalDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesRelativeBalDown);

          WVJJTree->bos_AK4AK4_pt_jesRelativeBalDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesRelativeBalDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesHFUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesHFUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesHFUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesHFUp);

          WVJJTree->bos_AK4AK4_pt_jesHFUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesHFUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesHFDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesHFDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesHFDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesHFDown);

          WVJJTree->bos_AK4AK4_pt_jesHFDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesHFDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesBBEC1Up, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesBBEC1Up);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesBBEC1Up, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesBBEC1Up);

          WVJJTree->bos_AK4AK4_pt_jesBBEC1Up = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesBBEC1Up = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesBBEC1Down, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesBBEC1Down);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesBBEC1Down, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesBBEC1Down);

          WVJJTree->bos_AK4AK4_pt_jesBBEC1Down = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesBBEC1Down = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesEC2Up, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesEC2Up);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesEC2Up, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesEC2Up);

          WVJJTree->bos_AK4AK4_pt_jesEC2Up = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesEC2Up = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesEC2Down, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesEC2Down);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesEC2Down, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesEC2Down);

          WVJJTree->bos_AK4AK4_pt_jesEC2Down = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesEC2Down = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesAbsoluteUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesAbsoluteUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesAbsoluteUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesAbsoluteUp);

          WVJJTree->bos_AK4AK4_pt_jesAbsoluteUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesAbsoluteUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesAbsoluteDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesAbsoluteDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesAbsoluteDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesAbsoluteDown);

          WVJJTree->bos_AK4AK4_pt_jesAbsoluteDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesAbsoluteDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesBBEC1_YearUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesBBEC1_YearUp);

          WVJJTree->bos_AK4AK4_pt_jesBBEC1_YearUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesBBEC1_YearUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesBBEC1_YearDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesBBEC1_YearDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesBBEC1_YearDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesBBEC1_YearDown);

          WVJJTree->bos_AK4AK4_pt_jesBBEC1_YearDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesBBEC1_YearDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesEC2_YearUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesEC2_YearUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesEC2_YearUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesEC2_YearUp);

          WVJJTree->bos_AK4AK4_pt_jesEC2_YearUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesEC2_YearUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesEC2_YearDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesEC2_YearDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesEC2_YearDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesEC2_YearDown);

          WVJJTree->bos_AK4AK4_pt_jesEC2_YearDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesEC2_YearDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesAbsolute_YearUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesAbsolute_YearUp);

          WVJJTree->bos_AK4AK4_pt_jesAbsolute_YearUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesAbsolute_YearUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesAbsolute_YearDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesAbsolute_YearDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesAbsolute_YearDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesAbsolute_YearDown);

          WVJJTree->bos_AK4AK4_pt_jesAbsolute_YearDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesAbsolute_YearDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesHF_YearUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesHF_YearUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesHF_YearUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesHF_YearUp);

          WVJJTree->bos_AK4AK4_pt_jesHF_YearUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesHF_YearUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesHF_YearDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesHF_YearDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesHF_YearDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesHF_YearDown);

          WVJJTree->bos_AK4AK4_pt_jesHF_YearDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesHF_YearDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearUp);

          WVJJTree->bos_AK4AK4_pt_jesRelativeSample_YearUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesRelativeSample_YearUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesRelativeSample_YearDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesRelativeSample_YearDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesRelativeSample_YearDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesRelativeSample_YearDown);

          WVJJTree->bos_AK4AK4_pt_jesRelativeSample_YearDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesRelativeSample_YearDown = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesTotalUp, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesTotalUp);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesTotalUp, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesTotalUp);

          WVJJTree->bos_AK4AK4_pt_jesTotalUp = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesTotalUp = (tempBos1+tempBos2).M();

          tempBos1.SetPtEtaPhiM(WVJJTree->bos_j1_AK4_pt_jesTotalDown, WVJJTree->bos_j1_AK4_eta,
                                WVJJTree->bos_j1_AK4_phi, WVJJTree->bos_j1_AK4_m_jesTotalDown);
          tempBos2.SetPtEtaPhiM(WVJJTree->bos_j2_AK4_pt_jesTotalDown, WVJJTree->bos_j2_AK4_eta,
                                WVJJTree->bos_j2_AK4_phi, WVJJTree->bos_j2_AK4_m_jesTotalDown);

          WVJJTree->bos_AK4AK4_pt_jesTotalDown = (tempBos1+tempBos2).Pt();
          WVJJTree->bos_AK4AK4_m_jesTotalDown = (tempBos1+tempBos2).M();
        }
      } //if (nGoodFatJet==0)

      //check we have a hadronic boson candidate
      if ( nGoodFatJet == 0 && nGoodDijet == 0 ) continue;
      if (passLepSel) totalCutFlow->Fill("Hadronic V",1);
      if (passLepSel && WVJJTree->lep2_pt > 0 && nGoodFatJet > 0) zvCutFlow->Fill("Hadronic V",1);
      if (passLepSel && WVJJTree->lep2_pt > 0 && nGoodDijet > 0) zjjCutFlow->Fill("Hadronic V",1);
      if (passLepSel && WVJJTree->lep2_pt < 0 && nGoodFatJet > 0) wvCutFlow->Fill("Hadronic V",1);
      if (passLepSel && WVJJTree->lep2_pt < 0 && nGoodDijet > 0) wjjCutFlow->Fill("Hadronic V",1);

      // bosHad JES
      TLorentzVector bosHad(0, 0, 0, 0), bosHad_jesFlavorQCDUp(0, 0, 0, 0), bosHad_jesFlavorQCDDown(0, 0, 0, 0), bosHad_jesRelativeBalUp(0, 0, 0, 0),
          bosHad_jesRelativeBalDown(0, 0, 0, 0), bosHad_jesHFUp(0, 0, 0, 0), bosHad_jesHFDown(0, 0, 0, 0), bosHad_jesBBEC1Up(0, 0, 0, 0),
          bosHad_jesBBEC1Down(0, 0, 0, 0), bosHad_jesEC2Up(0, 0, 0, 0), bosHad_jesEC2Down(0, 0, 0, 0), bosHad_jesAbsoluteUp(0, 0, 0, 0),
          bosHad_jesAbsoluteDown(0, 0, 0, 0), bosHad_jesBBEC1_YearUp(0, 0, 0, 0), bosHad_jesBBEC1_YearDown(0, 0, 0, 0), bosHad_jesEC2_YearUp(0, 0, 0, 0),
          bosHad_jesEC2_YearDown(0, 0, 0, 0), bosHad_jesAbsolute_YearUp(0, 0, 0, 0), bosHad_jesAbsolute_YearDown(0, 0, 0, 0),
          bosHad_jesHF_YearUp(0, 0, 0, 0), bosHad_jesHF_YearDown(0, 0, 0, 0), bosHad_jesRelativeSample_YearUp(0, 0, 0, 0),
          bosHad_jesRelativeSample_YearDown(0, 0, 0, 0), bosHad_jesTotalUp(0, 0, 0, 0), bosHad_jesTotalDown(0, 0, 0, 0);

      // bosLep JES
      TLorentzVector bosLep(0, 0, 0, 0), bosLep_jesFlavorQCDUp(0, 0, 0, 0), bosLep_jesFlavorQCDDown(0, 0, 0, 0), bosLep_jesRelativeBalUp(0, 0, 0, 0),
          bosLep_jesRelativeBalDown(0, 0, 0, 0), bosLep_jesHFUp(0, 0, 0, 0), bosLep_jesHFDown(0, 0, 0, 0), bosLep_jesBBEC1Up(0, 0, 0, 0),
          bosLep_jesBBEC1Down(0, 0, 0, 0), bosLep_jesEC2Up(0, 0, 0, 0), bosLep_jesEC2Down(0, 0, 0, 0), bosLep_jesAbsoluteUp(0, 0, 0, 0),
          bosLep_jesAbsoluteDown(0, 0, 0, 0), bosLep_jesBBEC1_YearUp(0, 0, 0, 0), bosLep_jesBBEC1_YearDown(0, 0, 0, 0), bosLep_jesEC2_YearUp(0, 0, 0, 0),
          bosLep_jesEC2_YearDown(0, 0, 0, 0), bosLep_jesAbsolute_YearUp(0, 0, 0, 0), bosLep_jesAbsolute_YearDown(0, 0, 0, 0),
          bosLep_jesHF_YearUp(0, 0, 0, 0), bosLep_jesHF_YearDown(0, 0, 0, 0), bosLep_jesRelativeSample_YearUp(0, 0, 0, 0),
          bosLep_jesRelativeSample_YearDown(0, 0, 0, 0), bosLep_jesTotalUp(0, 0, 0, 0), bosLep_jesTotalDown(0, 0, 0, 0);

      // bosLep lepton scale
      TLorentzVector bosLep_scaleUp(0, 0, 0, 0), bosLep_scaleDown(0, 0, 0, 0);

      //boosted event
      if (WVJJTree->bos_PuppiAK8_m_sd0_corr > 0) {
        bosHad.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt, WVJJTree->bos_PuppiAK8_eta,
                            WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

        if (isMC) {
          bosHad_jesFlavorQCDUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesFlavorQCDUp, WVJJTree->bos_PuppiAK8_eta,
                                             WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesFlavorQCDDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesFlavorQCDDown, WVJJTree->bos_PuppiAK8_eta,
                                               WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesRelativeBalUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesRelativeBalUp, WVJJTree->bos_PuppiAK8_eta,
                                               WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesRelativeBalDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesRelativeBalDown, WVJJTree->bos_PuppiAK8_eta,
                                                 WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesHFUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesHFUp, WVJJTree->bos_PuppiAK8_eta,
                                      WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesHFDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesHFDown, WVJJTree->bos_PuppiAK8_eta,
                                        WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesBBEC1Up.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesBBEC1Up, WVJJTree->bos_PuppiAK8_eta,
                                         WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesBBEC1Down.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesBBEC1Down, WVJJTree->bos_PuppiAK8_eta,
                                           WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesEC2Up.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesEC2Up, WVJJTree->bos_PuppiAK8_eta,
                                       WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesEC2Down.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesEC2Down, WVJJTree->bos_PuppiAK8_eta,
                                         WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesAbsoluteUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesAbsoluteUp, WVJJTree->bos_PuppiAK8_eta,
                                            WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesAbsoluteDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesAbsoluteDown, WVJJTree->bos_PuppiAK8_eta,
                                              WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesBBEC1_YearUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearUp, WVJJTree->bos_PuppiAK8_eta,
                                              WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesBBEC1_YearDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesBBEC1_YearDown, WVJJTree->bos_PuppiAK8_eta,
                                                WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesEC2_YearUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesEC2_YearUp, WVJJTree->bos_PuppiAK8_eta,
                                            WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesEC2_YearDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesEC2_YearDown, WVJJTree->bos_PuppiAK8_eta,
                                              WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesAbsolute_YearUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearUp, WVJJTree->bos_PuppiAK8_eta,
                                                 WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesAbsolute_YearDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesAbsolute_YearDown, WVJJTree->bos_PuppiAK8_eta,
                                                   WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesHF_YearUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesHF_YearUp, WVJJTree->bos_PuppiAK8_eta,
                                           WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesHF_YearDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesHF_YearDown, WVJJTree->bos_PuppiAK8_eta,
                                             WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesRelativeSample_YearUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearUp, WVJJTree->bos_PuppiAK8_eta,
                                                       WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesRelativeSample_YearDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesRelativeSample_YearDown, WVJJTree->bos_PuppiAK8_eta,
                                                         WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);

          bosHad_jesTotalUp.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesTotalUp, WVJJTree->bos_PuppiAK8_eta,
                                         WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
          bosHad_jesTotalDown.SetPtEtaPhiM(WVJJTree->bos_PuppiAK8_pt_jesTotalDown, WVJJTree->bos_PuppiAK8_eta,
                                           WVJJTree->bos_PuppiAK8_phi, WVJJTree->bos_PuppiAK8_m_sd0_corr);
        }
      }

      //resolved event
      else if (WVJJTree->bos_AK4AK4_m > 0) {
        bosHad.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt, WVJJTree->bos_AK4AK4_eta,
                            WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m);

        if (isMC) {
          bosHad_jesFlavorQCDUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesFlavorQCDUp, WVJJTree->bos_AK4AK4_eta,
                                             WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesFlavorQCDUp);
          bosHad_jesFlavorQCDDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesFlavorQCDDown, WVJJTree->bos_AK4AK4_eta,
                                               WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesFlavorQCDDown);

          bosHad_jesRelativeBalUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesRelativeBalUp, WVJJTree->bos_AK4AK4_eta,
                                               WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesRelativeBalUp);
          bosHad_jesRelativeBalDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesRelativeBalDown, WVJJTree->bos_AK4AK4_eta,
                                                 WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesRelativeBalDown);

          bosHad_jesHFUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesHFUp, WVJJTree->bos_AK4AK4_eta,
                                      WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesHFUp);
          bosHad_jesHFDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesHFDown, WVJJTree->bos_AK4AK4_eta,
                                        WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesHFDown);

          bosHad_jesBBEC1Up.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesBBEC1Up, WVJJTree->bos_AK4AK4_eta,
                                         WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesBBEC1Up);
          bosHad_jesBBEC1Down.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesBBEC1Down, WVJJTree->bos_AK4AK4_eta,
                                           WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesBBEC1Down);

          bosHad_jesEC2Up.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesEC2Up, WVJJTree->bos_AK4AK4_eta,
                                       WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesEC2Up);
          bosHad_jesEC2Down.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesEC2Down, WVJJTree->bos_AK4AK4_eta,
                                         WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesEC2Down);

          bosHad_jesAbsoluteUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesAbsoluteUp, WVJJTree->bos_AK4AK4_eta,
                                            WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesAbsoluteUp);
          bosHad_jesAbsoluteDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesAbsoluteDown, WVJJTree->bos_AK4AK4_eta,
                                              WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesAbsoluteDown);

          bosHad_jesBBEC1_YearUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesBBEC1_YearUp, WVJJTree->bos_AK4AK4_eta,
                                              WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesBBEC1_YearUp);
          bosHad_jesBBEC1_YearDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesBBEC1_YearDown, WVJJTree->bos_AK4AK4_eta,
                                                WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesBBEC1_YearDown);

          bosHad_jesEC2_YearUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesEC2_YearUp, WVJJTree->bos_AK4AK4_eta,
                                            WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesEC2_YearUp);
          bosHad_jesEC2_YearDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesEC2_YearDown, WVJJTree->bos_AK4AK4_eta,
                                              WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesEC2_YearDown);

          bosHad_jesAbsolute_YearUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesAbsolute_YearUp, WVJJTree->bos_AK4AK4_eta,
                                                 WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesAbsolute_YearUp);
          bosHad_jesAbsolute_YearDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesAbsolute_YearDown, WVJJTree->bos_AK4AK4_eta,
                                                   WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesAbsolute_YearDown);

          bosHad_jesHF_YearUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesHF_YearUp, WVJJTree->bos_AK4AK4_eta,
                                           WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesHF_YearUp);
          bosHad_jesHF_YearDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesHF_YearDown, WVJJTree->bos_AK4AK4_eta,
                                             WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesHF_YearDown);

          bosHad_jesRelativeSample_YearUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesRelativeSample_YearUp, WVJJTree->bos_AK4AK4_eta,
                                                       WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesRelativeSample_YearUp);
          bosHad_jesRelativeSample_YearDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesRelativeSample_YearDown, WVJJTree->bos_AK4AK4_eta,
                                                         WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesRelativeSample_YearDown);

          bosHad_jesTotalUp.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesTotalUp, WVJJTree->bos_AK4AK4_eta,
                                         WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesTotalUp);
          bosHad_jesTotalDown.SetPtEtaPhiM(WVJJTree->bos_AK4AK4_pt_jesTotalDown, WVJJTree->bos_AK4AK4_eta,
                                           WVJJTree->bos_AK4AK4_phi, WVJJTree->bos_AK4AK4_m_jesTotalDown);
        }
      }

      // 1 lepton + met event: JES and lepton pt scale variation
      // 2 lepton event: only lepton pt scale variation

      // 1 lepton
      if (WVJJTree->lep2_pt < 0) {
        TLorentzVector lep(0,0,0,0), met(0,0,0,0);
        lep.SetPtEtaPhiM(WVJJTree->lep1_pt, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
        met.SetPtEtaPhiM(WVJJTree->MET, 0.0, WVJJTree->MET_phi, 0.0);
        bosLep = lep+met;

        if (isMC) {
          // bosLep JES variation
          met.SetPtEtaPhiM(WVJJTree->MET_jesFlavorQCDUp, 0.0, WVJJTree->MET_phi_jesFlavorQCDUp, 0.0);
          bosLep_jesFlavorQCDUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesFlavorQCDDown, 0.0, WVJJTree->MET_phi_jesFlavorQCDDown, 0.0);
          bosLep_jesFlavorQCDDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesRelativeBalUp, 0.0, WVJJTree->MET_phi_jesRelativeBalUp, 0.0);
          bosLep_jesRelativeBalUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesRelativeBalDown, 0.0, WVJJTree->MET_phi_jesRelativeBalDown, 0.0);
          bosLep_jesRelativeBalDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesHFUp, 0.0, WVJJTree->MET_phi_jesHFUp, 0.0);
          bosLep_jesHFUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesHFDown, 0.0, WVJJTree->MET_phi_jesHFDown, 0.0);
          bosLep_jesHFDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1Up, 0.0, WVJJTree->MET_phi_jesBBEC1Up, 0.0);
          bosLep_jesBBEC1Up = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1Down, 0.0, WVJJTree->MET_phi_jesBBEC1Down, 0.0);
          bosLep_jesBBEC1Down = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesEC2Up, 0.0, WVJJTree->MET_phi_jesEC2Up, 0.0);
          bosLep_jesEC2Up = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesEC2Down, 0.0, WVJJTree->MET_phi_jesEC2Down, 0.0);
          bosLep_jesEC2Down = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesAbsoluteUp, 0.0, WVJJTree->MET_phi_jesAbsoluteUp, 0.0);
          bosLep_jesAbsoluteUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesAbsoluteDown, 0.0, WVJJTree->MET_phi_jesAbsoluteDown, 0.0);
          bosLep_jesAbsoluteDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1_YearUp, 0.0, WVJJTree->MET_phi_jesBBEC1_YearUp, 0.0);
          bosLep_jesBBEC1_YearUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesBBEC1_YearDown, 0.0, WVJJTree->MET_phi_jesBBEC1_YearDown, 0.0);
          bosLep_jesBBEC1_YearDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesEC2_YearUp, 0.0, WVJJTree->MET_phi_jesEC2_YearUp, 0.0);
          bosLep_jesEC2_YearUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesEC2_YearDown, 0.0, WVJJTree->MET_phi_jesEC2_YearDown, 0.0);
          bosLep_jesEC2_YearDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesAbsolute_YearUp, 0.0, WVJJTree->MET_phi_jesAbsolute_YearUp, 0.0);
          bosLep_jesAbsolute_YearUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesAbsolute_YearDown, 0.0, WVJJTree->MET_phi_jesAbsolute_YearDown, 0.0);
          bosLep_jesAbsolute_YearDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesHF_YearUp, 0.0, WVJJTree->MET_phi_jesHF_YearUp, 0.0);
          bosLep_jesHF_YearUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesHF_YearDown, 0.0, WVJJTree->MET_phi_jesHF_YearDown, 0.0);
          bosLep_jesHF_YearDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesRelativeSample_YearUp, 0.0, WVJJTree->MET_phi_jesRelativeSample_YearUp, 0.0);
          bosLep_jesRelativeSample_YearUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesRelativeSample_YearDown, 0.0, WVJJTree->MET_phi_jesRelativeSample_YearDown, 0.0);
          bosLep_jesRelativeSample_YearDown = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesTotalUp, 0.0, WVJJTree->MET_phi_jesTotalUp, 0.0);
          bosLep_jesTotalUp = lep+met;

          met.SetPtEtaPhiM(WVJJTree->MET_jesTotalDown, 0.0, WVJJTree->MET_phi_jesTotalDown, 0.0);
          bosLep_jesTotalDown = lep+met;

          // bosLep lepton pt scale variation
          met.SetPtEtaPhiM(WVJJTree->MET, 0.0, WVJJTree->MET_phi, 0.0);

          lep.SetPtEtaPhiM(WVJJTree->lep1_pt_scaleUp, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
          bosLep_scaleUp = lep+met;

          lep.SetPtEtaPhiM(WVJJTree->lep1_pt_scaleDown, WVJJTree->lep1_eta, WVJJTree->lep1_phi, WVJJTree->lep1_m);
          bosLep_scaleDown = lep+met;
        }
      }
      // 2 lepton
      else if (WVJJTree->lep2_pt > 0) {
        bosLep.SetPtEtaPhiM(WVJJTree->dilep_pt, WVJJTree->dilep_eta, WVJJTree->dilep_phi, WVJJTree->dilep_m);

        if (isMC) {
          // no JES, but set them equal to central
          bosLep_jesFlavorQCDUp = bosLep;
          bosLep_jesFlavorQCDDown = bosLep;
          bosLep_jesRelativeBalUp = bosLep;
          bosLep_jesRelativeBalDown = bosLep;
          bosLep_jesHFUp = bosLep;
          bosLep_jesHFDown = bosLep;
          bosLep_jesBBEC1Up = bosLep;
          bosLep_jesBBEC1Down = bosLep;
          bosLep_jesEC2Up = bosLep;
          bosLep_jesEC2Down = bosLep;
          bosLep_jesAbsoluteUp = bosLep;
          bosLep_jesAbsoluteDown = bosLep;
          bosLep_jesBBEC1_YearUp = bosLep;
          bosLep_jesBBEC1_YearDown = bosLep;
          bosLep_jesEC2_YearUp = bosLep;
          bosLep_jesEC2_YearDown = bosLep;
          bosLep_jesAbsolute_YearUp = bosLep;
          bosLep_jesAbsolute_YearDown = bosLep;
          bosLep_jesHF_YearUp = bosLep;
          bosLep_jesHF_YearDown = bosLep;
          bosLep_jesRelativeSample_YearUp = bosLep;
          bosLep_jesRelativeSample_YearDown = bosLep;
          bosLep_jesTotalUp = bosLep;
          bosLep_jesTotalDown = bosLep;

          // bosLep lepton pt scale variation
          bosLep_scaleUp.SetPtEtaPhiM(WVJJTree->dilep_pt_scaleUp, WVJJTree->dilep_eta,
                                      WVJJTree->dilep_phi, WVJJTree->dilep_m_scaleUp);
          bosLep_scaleDown.SetPtEtaPhiM(WVJJTree->dilep_pt_scaleDown, WVJJTree->dilep_eta,
                                        WVJJTree->dilep_phi, WVJJTree->dilep_m_scaleDown);
        }
      }

      TLorentzVector diBos = bosHad+bosLep;
      // lepton scale
      TLorentzVector diBos_scaleUp = bosHad+bosLep_scaleUp;
      TLorentzVector diBos_scaleDown = bosHad+bosLep_scaleDown;
      // JES
      TLorentzVector diBos_jesFlavorQCDUp = bosHad_jesFlavorQCDUp+bosLep_jesFlavorQCDUp;
      TLorentzVector diBos_jesFlavorQCDDown = bosHad_jesFlavorQCDDown+bosLep_jesFlavorQCDDown;
      TLorentzVector diBos_jesRelativeBalUp = bosHad_jesRelativeBalUp+bosLep_jesRelativeBalUp;
      TLorentzVector diBos_jesRelativeBalDown = bosHad_jesRelativeBalDown+bosLep_jesRelativeBalDown;
      TLorentzVector diBos_jesHFUp = bosHad_jesHFUp+bosLep_jesHFUp;
      TLorentzVector diBos_jesHFDown = bosHad_jesHFDown+bosLep_jesHFDown;
      TLorentzVector diBos_jesBBEC1Up = bosHad_jesBBEC1Up+bosLep_jesBBEC1Up;
      TLorentzVector diBos_jesBBEC1Down = bosHad_jesBBEC1Down+bosLep_jesBBEC1Down;
      TLorentzVector diBos_jesEC2Up = bosHad_jesEC2Up+bosLep_jesEC2Up;
      TLorentzVector diBos_jesEC2Down = bosHad_jesEC2Down+bosLep_jesEC2Down;
      TLorentzVector diBos_jesAbsoluteUp = bosHad_jesAbsoluteUp+bosLep_jesAbsoluteUp;
      TLorentzVector diBos_jesAbsoluteDown = bosHad_jesAbsoluteDown+bosLep_jesAbsoluteDown;
      TLorentzVector diBos_jesBBEC1_YearUp = bosHad_jesBBEC1_YearUp+bosLep_jesBBEC1_YearUp;
      TLorentzVector diBos_jesBBEC1_YearDown = bosHad_jesBBEC1_YearDown+bosLep_jesBBEC1_YearDown;
      TLorentzVector diBos_jesEC2_YearUp = bosHad_jesEC2_YearUp+bosLep_jesEC2_YearUp;
      TLorentzVector diBos_jesEC2_YearDown = bosHad_jesEC2_YearDown+bosLep_jesEC2_YearDown;
      TLorentzVector diBos_jesAbsolute_YearUp = bosHad_jesAbsolute_YearUp+bosLep_jesAbsolute_YearUp;
      TLorentzVector diBos_jesAbsolute_YearDown = bosHad_jesAbsolute_YearDown+bosLep_jesAbsolute_YearDown;
      TLorentzVector diBos_jesHF_YearUp = bosHad_jesHF_YearUp+bosLep_jesHF_YearUp;
      TLorentzVector diBos_jesHF_YearDown = bosHad_jesHF_YearDown+bosLep_jesHF_YearDown;
      TLorentzVector diBos_jesRelativeSample_YearUp = bosHad_jesRelativeSample_YearUp+bosLep_jesRelativeSample_YearUp;
      TLorentzVector diBos_jesRelativeSample_YearDown = bosHad_jesRelativeSample_YearDown+bosLep_jesRelativeSample_YearDown;
      TLorentzVector diBos_jesTotalUp = bosHad_jesTotalUp+bosLep_jesTotalUp;
      TLorentzVector diBos_jesTotalDown = bosHad_jesTotalDown+bosLep_jesTotalDown;

      WVJJTree->dibos_m = diBos.M();
      WVJJTree->dibos_mt = diBos.Mt();
      WVJJTree->dibos_pt = diBos.Pt();
      WVJJTree->dibos_eta = diBos.Eta();
      WVJJTree->dibos_phi = diBos.Phi();

      WVJJTree->dibos_m_scaleUp = diBos_scaleUp.M();
      WVJJTree->dibos_m_scaleDown = diBos_scaleDown.M();
      WVJJTree->dibos_mt_scaleUp = diBos_scaleUp.Mt();
      WVJJTree->dibos_mt_scaleDown = diBos_scaleDown.Mt();
      WVJJTree->dibos_pt_scaleUp = diBos_scaleUp.Pt();
      WVJJTree->dibos_pt_scaleDown = diBos_scaleDown.Pt();

      WVJJTree->dibos_m_jesFlavorQCDUp = diBos_jesFlavorQCDUp.M();
      WVJJTree->dibos_m_jesFlavorQCDDown = diBos_jesFlavorQCDDown.M();
      WVJJTree->dibos_mt_jesFlavorQCDUp = diBos_jesFlavorQCDUp.Mt();
      WVJJTree->dibos_mt_jesFlavorQCDDown = diBos_jesFlavorQCDDown.Mt();
      WVJJTree->dibos_pt_jesFlavorQCDUp = diBos_jesFlavorQCDUp.Pt();
      WVJJTree->dibos_pt_jesFlavorQCDDown = diBos_jesFlavorQCDDown.Pt();
      WVJJTree->dibos_m_jesRelativeBalUp = diBos_jesRelativeBalUp.M();
      WVJJTree->dibos_m_jesRelativeBalDown = diBos_jesRelativeBalDown.M();
      WVJJTree->dibos_mt_jesRelativeBalUp = diBos_jesRelativeBalUp.Mt();
      WVJJTree->dibos_mt_jesRelativeBalDown = diBos_jesRelativeBalDown.Mt();
      WVJJTree->dibos_pt_jesRelativeBalUp = diBos_jesRelativeBalUp.Pt();
      WVJJTree->dibos_pt_jesRelativeBalDown = diBos_jesRelativeBalDown.Pt();
      WVJJTree->dibos_m_jesHFUp = diBos_jesHFUp.M();
      WVJJTree->dibos_m_jesHFDown = diBos_jesHFDown.M();
      WVJJTree->dibos_mt_jesHFUp = diBos_jesHFUp.Mt();
      WVJJTree->dibos_mt_jesHFDown = diBos_jesHFDown.Mt();
      WVJJTree->dibos_pt_jesHFUp = diBos_jesHFUp.Pt();
      WVJJTree->dibos_pt_jesHFDown = diBos_jesHFDown.Pt();
      WVJJTree->dibos_m_jesBBEC1Up = diBos_jesBBEC1Up.M();
      WVJJTree->dibos_m_jesBBEC1Down = diBos_jesBBEC1Down.M();
      WVJJTree->dibos_mt_jesBBEC1Up = diBos_jesBBEC1Up.Mt();
      WVJJTree->dibos_mt_jesBBEC1Down = diBos_jesBBEC1Down.Mt();
      WVJJTree->dibos_pt_jesBBEC1Up = diBos_jesBBEC1Up.Pt();
      WVJJTree->dibos_pt_jesBBEC1Down = diBos_jesBBEC1Down.Pt();
      WVJJTree->dibos_m_jesEC2Up = diBos_jesEC2Up.M();
      WVJJTree->dibos_m_jesEC2Down = diBos_jesEC2Down.M();
      WVJJTree->dibos_mt_jesEC2Up = diBos_jesEC2Up.Mt();
      WVJJTree->dibos_mt_jesEC2Down = diBos_jesEC2Down.Mt();
      WVJJTree->dibos_pt_jesEC2Up = diBos_jesEC2Up.Pt();
      WVJJTree->dibos_pt_jesEC2Down = diBos_jesEC2Down.Pt();
      WVJJTree->dibos_m_jesAbsoluteUp = diBos_jesAbsoluteUp.M();
      WVJJTree->dibos_m_jesAbsoluteDown = diBos_jesAbsoluteDown.M();
      WVJJTree->dibos_mt_jesAbsoluteUp = diBos_jesAbsoluteUp.Mt();
      WVJJTree->dibos_mt_jesAbsoluteDown = diBos_jesAbsoluteDown.Mt();
      WVJJTree->dibos_pt_jesAbsoluteUp = diBos_jesAbsoluteUp.Pt();
      WVJJTree->dibos_pt_jesAbsoluteDown = diBos_jesAbsoluteDown.Pt();
      WVJJTree->dibos_m_jesBBEC1_YearUp = diBos_jesBBEC1_YearUp.M();
      WVJJTree->dibos_m_jesBBEC1_YearDown = diBos_jesBBEC1_YearDown.M();
      WVJJTree->dibos_mt_jesBBEC1_YearUp = diBos_jesBBEC1_YearUp.Mt();
      WVJJTree->dibos_mt_jesBBEC1_YearDown = diBos_jesBBEC1_YearDown.Mt();
      WVJJTree->dibos_pt_jesBBEC1_YearUp = diBos_jesBBEC1_YearUp.Pt();
      WVJJTree->dibos_pt_jesBBEC1_YearDown = diBos_jesBBEC1_YearDown.Pt();
      WVJJTree->dibos_m_jesEC2_YearUp = diBos_jesEC2_YearUp.M();
      WVJJTree->dibos_m_jesEC2_YearDown = diBos_jesEC2_YearDown.M();
      WVJJTree->dibos_mt_jesEC2_YearUp = diBos_jesEC2_YearUp.Mt();
      WVJJTree->dibos_mt_jesEC2_YearDown = diBos_jesEC2_YearDown.Mt();
      WVJJTree->dibos_pt_jesEC2_YearUp = diBos_jesEC2_YearUp.Pt();
      WVJJTree->dibos_pt_jesEC2_YearDown = diBos_jesEC2_YearDown.Pt();
      WVJJTree->dibos_m_jesAbsolute_YearUp = diBos_jesAbsolute_YearUp.M();
      WVJJTree->dibos_m_jesAbsolute_YearDown = diBos_jesAbsolute_YearDown.M();
      WVJJTree->dibos_mt_jesAbsolute_YearUp = diBos_jesAbsolute_YearUp.Mt();
      WVJJTree->dibos_mt_jesAbsolute_YearDown = diBos_jesAbsolute_YearDown.Mt();
      WVJJTree->dibos_pt_jesAbsolute_YearUp = diBos_jesAbsolute_YearUp.Pt();
      WVJJTree->dibos_pt_jesAbsolute_YearDown = diBos_jesAbsolute_YearDown.Pt();
      WVJJTree->dibos_m_jesHF_YearUp = diBos_jesHF_YearUp.M();
      WVJJTree->dibos_m_jesHF_YearDown = diBos_jesHF_YearDown.M();
      WVJJTree->dibos_mt_jesHF_YearUp = diBos_jesHF_YearUp.Mt();
      WVJJTree->dibos_mt_jesHF_YearDown = diBos_jesHF_YearDown.Mt();
      WVJJTree->dibos_pt_jesHF_YearUp = diBos_jesHF_YearUp.Pt();
      WVJJTree->dibos_pt_jesHF_YearDown = diBos_jesHF_YearDown.Pt();
      WVJJTree->dibos_m_jesRelativeSample_YearUp = diBos_jesRelativeSample_YearUp.M();
      WVJJTree->dibos_m_jesRelativeSample_YearDown = diBos_jesRelativeSample_YearDown.M();
      WVJJTree->dibos_mt_jesRelativeSample_YearUp = diBos_jesRelativeSample_YearUp.Mt();
      WVJJTree->dibos_mt_jesRelativeSample_YearDown = diBos_jesRelativeSample_YearDown.Mt();
      WVJJTree->dibos_pt_jesRelativeSample_YearUp = diBos_jesRelativeSample_YearUp.Pt();
      WVJJTree->dibos_pt_jesRelativeSample_YearDown = diBos_jesRelativeSample_YearDown.Pt();
      WVJJTree->dibos_m_jesTotalUp = diBos_jesTotalUp.M();
      WVJJTree->dibos_m_jesTotalDown = diBos_jesTotalDown.M();
      WVJJTree->dibos_mt_jesTotalUp = diBos_jesTotalUp.Mt();
      WVJJTree->dibos_mt_jesTotalDown = diBos_jesTotalDown.Mt();
      WVJJTree->dibos_pt_jesTotalUp = diBos_jesTotalUp.Pt();
      WVJJTree->dibos_pt_jesTotalDown = diBos_jesTotalDown.Pt();

      if (isMC==1) {
        WVJJTree->nScaleWeight = *nr.nLHEScaleWeight;
        WVJJTree->nPdfWeight = *nr.nLHEPdfWeight;

        for (uint j=0; j<WVJJTree->nScaleWeight; j++) {
          //LHE scale variation weights (w_var / w_nominal); [0] is MUR="0.5" MUF="0.5";
          //[1] is MUR="0.5" MUF="1.0"; [2] is MUR="0.5" MUF="2.0"; [3] is MUR="1.0" MUF="0.5";
          //[4] is MUR="1.0" MUF="2.0"; [5] is MUR="2.0" MUF="0.5"; [6] is MUR="2.0" MUF="1.0";
          //[7] is MUR="2.0" MUF="2.0"
          WVJJTree->scaleWeight[j]=nr.LHEScaleWeight[j];
        }
        for (uint j=0; j<WVJJTree->nPdfWeight; j++) {
          //LHE pdf variation weights (w_var / w_nominal) for LHA IDs 91400 - 91432
          WVJJTree->pdfWeight[j]=nr.LHEPdfWeight[j];
        }

        // tZq events in ZV signal sample
        if ( (nr.GenPart_genPartIdxMother[2] == 0 && abs(nr.GenPart_pdgId[2]) == 6)
            || (nr.GenPart_genPartIdxMother[3] == 0 && abs(nr.GenPart_pdgId[3]) == 6)
           ) {
          WVJJTree->is_tZq = true;
        }
        //std::cout<<abs(nr.GenPart_pdgId[2])<< " " << WVJJTree->is_tZq << std::endl;
      }

      if (isMC==1 && *nr.nLHEReweightingWeight!=0) {
        WVJJTree->nAqgcWeight=*nr.nLHEReweightingWeight;

        for (uint j=0; j<WVJJTree->nAqgcWeight; j++) {
          WVJJTree->aqgcWeight[j]=nr.LHEReweightingWeight[j];
        }

      }

      if (isMC) {
        //if (era==2016) {
        //  WVJJTree->btagWeight = *nr.btagWeight_CMVA;
        //}
        //else {
        //  WVJJTree->btagWeight = *nr.btagWeight_DeepCSVB;
        //}

        if (era!=2018) {
          WVJJTree->L1PFWeight = *nr.L1PreFiringWeight_Nom;
          WVJJTree->L1PFWeight_Up = *nr.L1PreFiringWeight_Up;
          WVJJTree->L1PFWeight_Down = *nr.L1PreFiringWeight_Dn;
        }
      }

      ot->Fill();
    }

    delete t;
    delete r;
    delete f;
    //t=0; 
    //r=0;
    //f=0;
  }

  of->Write();
  of->Close();

  return 0;

}
