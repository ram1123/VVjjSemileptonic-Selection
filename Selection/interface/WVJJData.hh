#ifndef WVJJ_DATA_HH
#define WVJJ_DATA_HH

#include "TTree.h"

class WVJJData {

public:

  WVJJData(TTree *t) {
    fTree = t;
    init();
  };
  
  ~WVJJData() {
    delete fTree;
  };

  void clearVars();
  void init();  
  
  TTree *fTree;

  //------------------------------------//
  //       METADATA AND EVENT WEIGHTS   //
  //------------------------------------//
  
  uint run;
  uint ls;
  uint evt;
  float nPV;
  float nPU_mean;
  float genWeight;
  float puWeight;
  float puWeight_Up;
  float puWeight_Dn;
  float L1PFWeight;
  float L1PFWeight_Up;
  float L1PFWeight_Dn;
  float LHEWeight[1164] = {};
  uint nScaleWeight;
  uint nPdfWeight;
  uint nAqgcWeight;
  float scaleWeight[200] = {};
  float pdfWeight[200] = {};
  float aqgcWeight[1000] = {};

  //njet counters
  uint nJet30;
  uint nJet50;
  uint nJet30_noClean;
  uint nJet50_noClean;

  //btag counters
  uint nBtag_loose;
  uint nBtag_medium;
  uint nBtag_tight;

  float btagWeight;

  //triggers
  bool trigger_1Mu;
  bool trigger_2Mu;
  bool trigger_1El;
  bool trigger_2El;

  //signal vs anti-iso
  bool isAntiIso;
  float lepFakeRate;
  
  //------------------------------------//
  //       LEPTONS                      //
  //------------------------------------//

  //lepton 1
  float lep1_pt;
  float lep1_eta;
  float lep1_phi;
  float lep1_m;
  float lep1_q;
  float lep1_iso;
  float lep1_dxy;
  float lep1_dz;
  float lep1_sip3d;
  float lep1_idEffWeight;

  //lepton 1 scale variations
  float lep1_pt_scaleUp;
  float lep1_pt_scaleDown;
  
  //lepton 2
  float lep2_pt;
  float lep2_eta;
  float lep2_phi;
  float lep2_m;
  float lep2_q;
  float lep2_iso;
  float lep2_dxy;
  float lep2_dz;
  float lep2_sip3d;
  float lep2_idEffWeight;

  //lepton 2 scale variations
  float lep2_pt_scaleUp;
  float lep2_pt_scaleDown;

  //dilepton final state
  float dilep_m;
  float dilep_mt;
  float dilep_pt;
  float dilep_eta;
  float dilep_phi;

  //dilepton JES variations
  float dilep_m_jesFlavorQCDUp;
  float dilep_m_jesFlavorQCDDown;
  float dilep_m_jesRelativeBalUp;
  float dilep_m_jesRelativeBalDown;
  float dilep_m_jesHFUp;
  float dilep_m_jesHFDown;
  float dilep_m_jesBBEC1Up;
  float dilep_m_jesBBEC1Down;
  float dilep_m_jesEC2Up;
  float dilep_m_jesEC2Down;
  float dilep_m_jesAbsoluteUp;
  float dilep_m_jesAbsoluteDown;
  float dilep_m_jesBBEC1_YearUp;
  float dilep_m_jesBBEC1_YearDown;
  float dilep_m_jesEC2_YearUp;
  float dilep_m_jesEC2_YearDown;
  float dilep_m_jesAbsolute_YearUp;
  float dilep_m_jesAbsolute_YearDown;
  float dilep_m_jesHF_YearUp;
  float dilep_m_jesHF_YearDown;
  float dilep_m_jesRelativeSample_YearUp;
  float dilep_m_jesRelativeSample_YearDown;
  float dilep_m_jesTotalUp;
  float dilep_m_jesTotalDown;

  float dilep_mt_jesFlavorQCDUp;
  float dilep_mt_jesFlavorQCDDown;
  float dilep_mt_jesRelativeBalUp;
  float dilep_mt_jesRelativeBalDown;
  float dilep_mt_jesHFUp;
  float dilep_mt_jesHFDown;
  float dilep_mt_jesBBEC1Up;
  float dilep_mt_jesBBEC1Down;
  float dilep_mt_jesEC2Up;
  float dilep_mt_jesEC2Down;
  float dilep_mt_jesAbsoluteUp;
  float dilep_mt_jesAbsoluteDown;
  float dilep_mt_jesBBEC1_YearUp;
  float dilep_mt_jesBBEC1_YearDown;
  float dilep_mt_jesEC2_YearUp;
  float dilep_mt_jesEC2_YearDown;
  float dilep_mt_jesAbsolute_YearUp;
  float dilep_mt_jesAbsolute_YearDown;
  float dilep_mt_jesHF_YearUp;
  float dilep_mt_jesHF_YearDown;
  float dilep_mt_jesRelativeSample_YearUp;
  float dilep_mt_jesRelativeSample_YearDown;
  float dilep_mt_jesTotalUp;
  float dilep_mt_jesTotalDown;

  float dilep_pt_jesFlavorQCDUp;
  float dilep_pt_jesFlavorQCDDown;
  float dilep_pt_jesRelativeBalUp;
  float dilep_pt_jesRelativeBalDown;
  float dilep_pt_jesHFUp;
  float dilep_pt_jesHFDown;
  float dilep_pt_jesBBEC1Up;
  float dilep_pt_jesBBEC1Down;
  float dilep_pt_jesEC2Up;
  float dilep_pt_jesEC2Down;
  float dilep_pt_jesAbsoluteUp;
  float dilep_pt_jesAbsoluteDown;
  float dilep_pt_jesBBEC1_YearUp;
  float dilep_pt_jesBBEC1_YearDown;
  float dilep_pt_jesEC2_YearUp;
  float dilep_pt_jesEC2_YearDown;
  float dilep_pt_jesAbsolute_YearUp;
  float dilep_pt_jesAbsolute_YearDown;
  float dilep_pt_jesHF_YearUp;
  float dilep_pt_jesHF_YearDown;
  float dilep_pt_jesRelativeSample_YearUp;
  float dilep_pt_jesRelativeSample_YearDown;
  float dilep_pt_jesTotalUp;
  float dilep_pt_jesTotalDown;

  //dilepton lepton scale variations
  float dilep_m_scaleUp;
  float dilep_m_scaleDown;
  float dilep_mt_scaleUp;
  float dilep_mt_scaleDown;
  float dilep_pt_scaleUp;
  float dilep_pt_scaleDown;

  //------------------------------------//
  //       MET                          //
  //------------------------------------//

  float MET;
  float MET_phi;
  float MET_2017;

  float MET_jesFlavorQCDUp;
  float MET_jesFlavorQCDDown;
  float MET_jesRelativeBalUp;
  float MET_jesRelativeBalDown;
  float MET_jesHFUp;
  float MET_jesHFDown;
  float MET_jesBBEC1Up;
  float MET_jesBBEC1Down;
  float MET_jesEC2Up;
  float MET_jesEC2Down;
  float MET_jesAbsoluteUp;
  float MET_jesAbsoluteDown;
  float MET_jesBBEC1_YearUp;
  float MET_jesBBEC1_YearDown;
  float MET_jesEC2_YearUp;
  float MET_jesEC2_YearDown;
  float MET_jesAbsolute_YearUp;
  float MET_jesAbsolute_YearDown;
  float MET_jesHF_YearUp;
  float MET_jesHF_YearDown;
  float MET_jesRelativeSample_YearUp;
  float MET_jesRelativeSample_YearDown;
  float MET_jesTotalUp;
  float MET_jesTotalDown;

  float MET_phi_jesFlavorQCDUp;
  float MET_phi_jesFlavorQCDDown;
  float MET_phi_jesRelativeBalUp;
  float MET_phi_jesRelativeBalDown;
  float MET_phi_jesHFUp;
  float MET_phi_jesHFDown;
  float MET_phi_jesBBEC1Up;
  float MET_phi_jesBBEC1Down;
  float MET_phi_jesEC2Up;
  float MET_phi_jesEC2Down;
  float MET_phi_jesAbsoluteUp;
  float MET_phi_jesAbsoluteDown;
  float MET_phi_jesBBEC1_YearUp;
  float MET_phi_jesBBEC1_YearDown;
  float MET_phi_jesEC2_YearUp;
  float MET_phi_jesEC2_YearDown;
  float MET_phi_jesAbsolute_YearUp;
  float MET_phi_jesAbsolute_YearDown;
  float MET_phi_jesHF_YearUp;
  float MET_phi_jesHF_YearDown;
  float MET_phi_jesRelativeSample_YearUp;
  float MET_phi_jesRelativeSample_YearDown;
  float MET_phi_jesTotalUp;
  float MET_phi_jesTotalDown;


  float PuppiMET;
  float PuppiMET_phi;
  
  //W neutrino pZ
  float neu_pz_type0;

  //------------------------------------//
  //       VBF/TAGGING JETS             //
  //------------------------------------//

  //VBF jet 1
  float vbf1_AK4_pt;
  float vbf1_AK4_eta;
  float vbf1_AK4_phi;
  float vbf1_AK4_m;
  //float vbf1_AK4_e;
  float vbf1_AK4_qgid;
  float vbf1_AK4_axis2;
  float vbf1_AK4_ptD;

  //VBF jet 1 variations
  float vbf1_AK4_pt_jesFlavorQCDUp;
  float vbf1_AK4_pt_jesFlavorQCDDown;
  float vbf1_AK4_m_jesFlavorQCDUp;
  float vbf1_AK4_m_jesFlavorQCDDown;
  float vbf1_AK4_pt_jesRelativeBalUp;
  float vbf1_AK4_pt_jesRelativeBalDown;
  float vbf1_AK4_m_jesRelativeBalUp;
  float vbf1_AK4_m_jesRelativeBalDown;
  float vbf1_AK4_pt_jesHFUp;
  float vbf1_AK4_pt_jesHFDown;
  float vbf1_AK4_m_jesHFUp;
  float vbf1_AK4_m_jesHFDown;
  float vbf1_AK4_pt_jesBBEC1Up;
  float vbf1_AK4_pt_jesBBEC1Down;
  float vbf1_AK4_m_jesBBEC1Up;
  float vbf1_AK4_m_jesBBEC1Down;
  float vbf1_AK4_pt_jesEC2Up;
  float vbf1_AK4_pt_jesEC2Down;
  float vbf1_AK4_m_jesEC2Up;
  float vbf1_AK4_m_jesEC2Down;
  float vbf1_AK4_pt_jesAbsoluteUp;
  float vbf1_AK4_pt_jesAbsoluteDown;
  float vbf1_AK4_m_jesAbsoluteUp;
  float vbf1_AK4_m_jesAbsoluteDown;
  float vbf1_AK4_pt_jesBBEC1_YearUp;
  float vbf1_AK4_pt_jesBBEC1_YearDown;
  float vbf1_AK4_m_jesBBEC1_YearUp;
  float vbf1_AK4_m_jesBBEC1_YearDown;
  float vbf1_AK4_pt_jesEC2_YearUp;
  float vbf1_AK4_pt_jesEC2_YearDown;
  float vbf1_AK4_m_jesEC2_YearUp;
  float vbf1_AK4_m_jesEC2_YearDown;
  float vbf1_AK4_pt_jesAbsolute_YearUp;
  float vbf1_AK4_pt_jesAbsolute_YearDown;
  float vbf1_AK4_m_jesAbsolute_YearUp;
  float vbf1_AK4_m_jesAbsolute_YearDown;
  float vbf1_AK4_pt_jesHF_YearUp;
  float vbf1_AK4_pt_jesHF_YearDown;
  float vbf1_AK4_m_jesHF_YearUp;
  float vbf1_AK4_m_jesHF_YearDown;
  float vbf1_AK4_pt_jesRelativeSample_YearUp;
  float vbf1_AK4_pt_jesRelativeSample_YearDown;
  float vbf1_AK4_m_jesRelativeSample_YearUp;
  float vbf1_AK4_m_jesRelativeSample_YearDown;
  float vbf1_AK4_pt_jesTotalUp;
  float vbf1_AK4_pt_jesTotalDown;
  float vbf1_AK4_m_jesTotalUp;
  float vbf1_AK4_m_jesTotalDown;

  //VBF jet 2
  float vbf2_AK4_pt;
  float vbf2_AK4_eta;
  float vbf2_AK4_phi;
  float vbf2_AK4_m;
  //float vbf2_AK4_e;
  float vbf2_AK4_qgid;
  float vbf2_AK4_axis2;
  float vbf2_AK4_ptD;

  //VBF jet 2 variations
  float vbf2_AK4_pt_jesFlavorQCDUp;
  float vbf2_AK4_pt_jesFlavorQCDDown;
  float vbf2_AK4_m_jesFlavorQCDUp;
  float vbf2_AK4_m_jesFlavorQCDDown;
  float vbf2_AK4_pt_jesRelativeBalUp;
  float vbf2_AK4_pt_jesRelativeBalDown;
  float vbf2_AK4_m_jesRelativeBalUp;
  float vbf2_AK4_m_jesRelativeBalDown;
  float vbf2_AK4_pt_jesHFUp;
  float vbf2_AK4_pt_jesHFDown;
  float vbf2_AK4_m_jesHFUp;
  float vbf2_AK4_m_jesHFDown;
  float vbf2_AK4_pt_jesBBEC1Up;
  float vbf2_AK4_pt_jesBBEC1Down;
  float vbf2_AK4_m_jesBBEC1Up;
  float vbf2_AK4_m_jesBBEC1Down;
  float vbf2_AK4_pt_jesEC2Up;
  float vbf2_AK4_pt_jesEC2Down;
  float vbf2_AK4_m_jesEC2Up;
  float vbf2_AK4_m_jesEC2Down;
  float vbf2_AK4_pt_jesAbsoluteUp;
  float vbf2_AK4_pt_jesAbsoluteDown;
  float vbf2_AK4_m_jesAbsoluteUp;
  float vbf2_AK4_m_jesAbsoluteDown;
  float vbf2_AK4_pt_jesBBEC1_YearUp;
  float vbf2_AK4_pt_jesBBEC1_YearDown;
  float vbf2_AK4_m_jesBBEC1_YearUp;
  float vbf2_AK4_m_jesBBEC1_YearDown;
  float vbf2_AK4_pt_jesEC2_YearUp;
  float vbf2_AK4_pt_jesEC2_YearDown;
  float vbf2_AK4_m_jesEC2_YearUp;
  float vbf2_AK4_m_jesEC2_YearDown;
  float vbf2_AK4_pt_jesAbsolute_YearUp;
  float vbf2_AK4_pt_jesAbsolute_YearDown;
  float vbf2_AK4_m_jesAbsolute_YearUp;
  float vbf2_AK4_m_jesAbsolute_YearDown;
  float vbf2_AK4_pt_jesHF_YearUp;
  float vbf2_AK4_pt_jesHF_YearDown;
  float vbf2_AK4_m_jesHF_YearUp;
  float vbf2_AK4_m_jesHF_YearDown;
  float vbf2_AK4_pt_jesRelativeSample_YearUp;
  float vbf2_AK4_pt_jesRelativeSample_YearDown;
  float vbf2_AK4_m_jesRelativeSample_YearUp;
  float vbf2_AK4_m_jesRelativeSample_YearDown;
  float vbf2_AK4_pt_jesTotalUp;
  float vbf2_AK4_pt_jesTotalDown;
  float vbf2_AK4_m_jesTotalUp;
  float vbf2_AK4_m_jesTotalDown;

  //VBF dijet object
  float vbf_pt;
  float vbf_eta;
  float vbf_phi;
  float vbf_m;
  float vbf_deta;

  //VBF dijet variations
  float vbf_pt_jesFlavorQCDUp;
  float vbf_pt_jesFlavorQCDDown;
  float vbf_m_jesFlavorQCDUp;
  float vbf_m_jesFlavorQCDDown;
  float vbf_pt_jesRelativeBalUp;
  float vbf_pt_jesRelativeBalDown;
  float vbf_m_jesRelativeBalUp;
  float vbf_m_jesRelativeBalDown;
  float vbf_pt_jesHFUp;
  float vbf_pt_jesHFDown;
  float vbf_m_jesHFUp;
  float vbf_m_jesHFDown;
  float vbf_pt_jesBBEC1Up;
  float vbf_pt_jesBBEC1Down;
  float vbf_m_jesBBEC1Up;
  float vbf_m_jesBBEC1Down;
  float vbf_pt_jesEC2Up;
  float vbf_pt_jesEC2Down;
  float vbf_m_jesEC2Up;
  float vbf_m_jesEC2Down;
  float vbf_pt_jesAbsoluteUp;
  float vbf_pt_jesAbsoluteDown;
  float vbf_m_jesAbsoluteUp;
  float vbf_m_jesAbsoluteDown;
  float vbf_pt_jesBBEC1_YearUp;
  float vbf_pt_jesBBEC1_YearDown;
  float vbf_m_jesBBEC1_YearUp;
  float vbf_m_jesBBEC1_YearDown;
  float vbf_pt_jesEC2_YearUp;
  float vbf_pt_jesEC2_YearDown;
  float vbf_m_jesEC2_YearUp;
  float vbf_m_jesEC2_YearDown;
  float vbf_pt_jesAbsolute_YearUp;
  float vbf_pt_jesAbsolute_YearDown;
  float vbf_m_jesAbsolute_YearUp;
  float vbf_m_jesAbsolute_YearDown;
  float vbf_pt_jesHF_YearUp;
  float vbf_pt_jesHF_YearDown;
  float vbf_m_jesHF_YearUp;
  float vbf_m_jesHF_YearDown;
  float vbf_pt_jesRelativeSample_YearUp;
  float vbf_pt_jesRelativeSample_YearDown;
  float vbf_m_jesRelativeSample_YearUp;
  float vbf_m_jesRelativeSample_YearDown;
  float vbf_pt_jesTotalUp;
  float vbf_pt_jesTotalDown;
  float vbf_m_jesTotalUp;
  float vbf_m_jesTotalDown;

  //------------------------------------//
  //       HADRONIC BOOSTED OBJECTS     //
  //------------------------------------//

  //Boson AK8 jet
  float bos_PuppiAK8_m_sd0;
  float bos_PuppiAK8_m_sd0_corr;
  float bos_PuppiAK8_pt;
  float bos_PuppiAK8_eta;
  float bos_PuppiAK8_phi;
  //float bos_PuppiAK8_e_ungroomed;
  float bos_PuppiAK8_tau2tau1;

  //Boson AK8 jet variations
  //float bos_PuppiAK8_m_sd0_corr_scaleUp;
  //float bos_PuppiAK8_m_sd0_corr_scaleDown;
  float bos_PuppiAK8_pt_jesFlavorQCDUp;
  float bos_PuppiAK8_pt_jesFlavorQCDDown;
  float bos_PuppiAK8_pt_jesRelativeBalUp;
  float bos_PuppiAK8_pt_jesRelativeBalDown;
  float bos_PuppiAK8_pt_jesHFUp;
  float bos_PuppiAK8_pt_jesHFDown;
  float bos_PuppiAK8_pt_jesBBEC1Up;
  float bos_PuppiAK8_pt_jesBBEC1Down;
  float bos_PuppiAK8_pt_jesEC2Up;
  float bos_PuppiAK8_pt_jesEC2Down;
  float bos_PuppiAK8_pt_jesAbsoluteUp;
  float bos_PuppiAK8_pt_jesAbsoluteDown;
  float bos_PuppiAK8_pt_jesBBEC1_YearUp;
  float bos_PuppiAK8_pt_jesBBEC1_YearDown;
  float bos_PuppiAK8_pt_jesEC2_YearUp;
  float bos_PuppiAK8_pt_jesEC2_YearDown;
  float bos_PuppiAK8_pt_jesAbsolute_YearUp;
  float bos_PuppiAK8_pt_jesAbsolute_YearDown;
  float bos_PuppiAK8_pt_jesHF_YearUp;
  float bos_PuppiAK8_pt_jesHF_YearDown;
  float bos_PuppiAK8_pt_jesRelativeSample_YearUp;
  float bos_PuppiAK8_pt_jesRelativeSample_YearDown;
  float bos_PuppiAK8_pt_jesTotalUp;
  float bos_PuppiAK8_pt_jesTotalDown;
  float bos_PuppiAK8_e_ungroomed_scaleUp;
  float bos_PuppiAK8_e_ungroomed_scaleDown;

  float bos_PuppiAK8_e2_sdb1; 
  float bos_PuppiAK8_e3_sdb1; 
  float bos_PuppiAK8_e3_v1_sdb1; 
  float bos_PuppiAK8_e3_v2_sdb1; 
  float bos_PuppiAK8_e4_v1_sdb1; 
  float bos_PuppiAK8_e4_v2_sdb1;
  float bos_PuppiAK8_e2_sdb2; 
  float bos_PuppiAK8_e3_sdb2; 
  float bos_PuppiAK8_e3_v1_sdb2; 
  float bos_PuppiAK8_e3_v2_sdb2; 
  float bos_PuppiAK8_e4_v1_sdb2; 
  float bos_PuppiAK8_e4_v2_sdb2; 

  //------------------------------------//
  //       HADRONIC RESOLVED OBJECTS    //
  //------------------------------------//

  //Boson AK4 jet 1
  float bos_j1_AK4_pt;
  float bos_j1_AK4_eta;
  float bos_j1_AK4_phi;
  float bos_j1_AK4_m;
  float bos_j1_AK4_qgid;
  //float bos_j1_AK4_e;

  //Boson AK4 jet 1 variations
  float bos_j1_AK4_pt_jesFlavorQCDUp;
  float bos_j1_AK4_pt_jesFlavorQCDDown;
  float bos_j1_AK4_m_jesFlavorQCDUp;
  float bos_j1_AK4_m_jesFlavorQCDDown;
  float bos_j1_AK4_pt_jesRelativeBalUp;
  float bos_j1_AK4_pt_jesRelativeBalDown;
  float bos_j1_AK4_m_jesRelativeBalUp;
  float bos_j1_AK4_m_jesRelativeBalDown;
  float bos_j1_AK4_pt_jesHFUp;
  float bos_j1_AK4_pt_jesHFDown;
  float bos_j1_AK4_m_jesHFUp;
  float bos_j1_AK4_m_jesHFDown;
  float bos_j1_AK4_pt_jesBBEC1Up;
  float bos_j1_AK4_pt_jesBBEC1Down;
  float bos_j1_AK4_m_jesBBEC1Up;
  float bos_j1_AK4_m_jesBBEC1Down;
  float bos_j1_AK4_pt_jesEC2Up;
  float bos_j1_AK4_pt_jesEC2Down;
  float bos_j1_AK4_m_jesEC2Up;
  float bos_j1_AK4_m_jesEC2Down;
  float bos_j1_AK4_pt_jesAbsoluteUp;
  float bos_j1_AK4_pt_jesAbsoluteDown;
  float bos_j1_AK4_m_jesAbsoluteUp;
  float bos_j1_AK4_m_jesAbsoluteDown;
  float bos_j1_AK4_pt_jesBBEC1_YearUp;
  float bos_j1_AK4_pt_jesBBEC1_YearDown;
  float bos_j1_AK4_m_jesBBEC1_YearUp;
  float bos_j1_AK4_m_jesBBEC1_YearDown;
  float bos_j1_AK4_pt_jesEC2_YearUp;
  float bos_j1_AK4_pt_jesEC2_YearDown;
  float bos_j1_AK4_m_jesEC2_YearUp;
  float bos_j1_AK4_m_jesEC2_YearDown;
  float bos_j1_AK4_pt_jesAbsolute_YearUp;
  float bos_j1_AK4_pt_jesAbsolute_YearDown;
  float bos_j1_AK4_m_jesAbsolute_YearUp;
  float bos_j1_AK4_m_jesAbsolute_YearDown;
  float bos_j1_AK4_pt_jesHF_YearUp;
  float bos_j1_AK4_pt_jesHF_YearDown;
  float bos_j1_AK4_m_jesHF_YearUp;
  float bos_j1_AK4_m_jesHF_YearDown;
  float bos_j1_AK4_pt_jesRelativeSample_YearUp;
  float bos_j1_AK4_pt_jesRelativeSample_YearDown;
  float bos_j1_AK4_m_jesRelativeSample_YearUp;
  float bos_j1_AK4_m_jesRelativeSample_YearDown;
  float bos_j1_AK4_pt_jesTotalUp;
  float bos_j1_AK4_pt_jesTotalDown;
  float bos_j1_AK4_m_jesTotalUp;
  float bos_j1_AK4_m_jesTotalDown;

  //Boson AK4 jet 2
  float bos_j2_AK4_pt;
  float bos_j2_AK4_eta;
  float bos_j2_AK4_phi;
  float bos_j2_AK4_m;
  float bos_j2_AK4_qgid;
  //float bos_j2_AK4_e;

  //Boson AK4 jet 2 variations
  float bos_j2_AK4_pt_jesFlavorQCDUp;
  float bos_j2_AK4_pt_jesFlavorQCDDown;
  float bos_j2_AK4_m_jesFlavorQCDUp;
  float bos_j2_AK4_m_jesFlavorQCDDown;
  float bos_j2_AK4_pt_jesRelativeBalUp;
  float bos_j2_AK4_pt_jesRelativeBalDown;
  float bos_j2_AK4_m_jesRelativeBalUp;
  float bos_j2_AK4_m_jesRelativeBalDown;
  float bos_j2_AK4_pt_jesHFUp;
  float bos_j2_AK4_pt_jesHFDown;
  float bos_j2_AK4_m_jesHFUp;
  float bos_j2_AK4_m_jesHFDown;
  float bos_j2_AK4_pt_jesBBEC1Up;
  float bos_j2_AK4_pt_jesBBEC1Down;
  float bos_j2_AK4_m_jesBBEC1Up;
  float bos_j2_AK4_m_jesBBEC1Down;
  float bos_j2_AK4_pt_jesEC2Up;
  float bos_j2_AK4_pt_jesEC2Down;
  float bos_j2_AK4_m_jesEC2Up;
  float bos_j2_AK4_m_jesEC2Down;
  float bos_j2_AK4_pt_jesAbsoluteUp;
  float bos_j2_AK4_pt_jesAbsoluteDown;
  float bos_j2_AK4_m_jesAbsoluteUp;
  float bos_j2_AK4_m_jesAbsoluteDown;
  float bos_j2_AK4_pt_jesBBEC1_YearUp;
  float bos_j2_AK4_pt_jesBBEC1_YearDown;
  float bos_j2_AK4_m_jesBBEC1_YearUp;
  float bos_j2_AK4_m_jesBBEC1_YearDown;
  float bos_j2_AK4_pt_jesEC2_YearUp;
  float bos_j2_AK4_pt_jesEC2_YearDown;
  float bos_j2_AK4_m_jesEC2_YearUp;
  float bos_j2_AK4_m_jesEC2_YearDown;
  float bos_j2_AK4_pt_jesAbsolute_YearUp;
  float bos_j2_AK4_pt_jesAbsolute_YearDown;
  float bos_j2_AK4_m_jesAbsolute_YearUp;
  float bos_j2_AK4_m_jesAbsolute_YearDown;
  float bos_j2_AK4_pt_jesHF_YearUp;
  float bos_j2_AK4_pt_jesHF_YearDown;
  float bos_j2_AK4_m_jesHF_YearUp;
  float bos_j2_AK4_m_jesHF_YearDown;
  float bos_j2_AK4_pt_jesRelativeSample_YearUp;
  float bos_j2_AK4_pt_jesRelativeSample_YearDown;
  float bos_j2_AK4_m_jesRelativeSample_YearUp;
  float bos_j2_AK4_m_jesRelativeSample_YearDown;
  float bos_j2_AK4_pt_jesTotalUp;
  float bos_j2_AK4_pt_jesTotalDown;
  float bos_j2_AK4_m_jesTotalUp;
  float bos_j2_AK4_m_jesTotalDown;

  //Boson dijet object
  float bos_AK4AK4_pt;
  float bos_AK4AK4_eta;
  float bos_AK4AK4_phi;
  float bos_AK4AK4_m;

  //Boson dijet variations
  float bos_AK4AK4_pt_jesFlavorQCDUp;
  float bos_AK4AK4_pt_jesFlavorQCDDown;
  float bos_AK4AK4_m_jesFlavorQCDUp;
  float bos_AK4AK4_m_jesFlavorQCDDown;
  float bos_AK4AK4_pt_jesRelativeBalUp;
  float bos_AK4AK4_pt_jesRelativeBalDown;
  float bos_AK4AK4_m_jesRelativeBalUp;
  float bos_AK4AK4_m_jesRelativeBalDown;
  float bos_AK4AK4_pt_jesHFUp;
  float bos_AK4AK4_pt_jesHFDown;
  float bos_AK4AK4_m_jesHFUp;
  float bos_AK4AK4_m_jesHFDown;
  float bos_AK4AK4_pt_jesBBEC1Up;
  float bos_AK4AK4_pt_jesBBEC1Down;
  float bos_AK4AK4_m_jesBBEC1Up;
  float bos_AK4AK4_m_jesBBEC1Down;
  float bos_AK4AK4_pt_jesEC2Up;
  float bos_AK4AK4_pt_jesEC2Down;
  float bos_AK4AK4_m_jesEC2Up;
  float bos_AK4AK4_m_jesEC2Down;
  float bos_AK4AK4_pt_jesAbsoluteUp;
  float bos_AK4AK4_pt_jesAbsoluteDown;
  float bos_AK4AK4_m_jesAbsoluteUp;
  float bos_AK4AK4_m_jesAbsoluteDown;
  float bos_AK4AK4_pt_jesBBEC1_YearUp;
  float bos_AK4AK4_pt_jesBBEC1_YearDown;
  float bos_AK4AK4_m_jesBBEC1_YearUp;
  float bos_AK4AK4_m_jesBBEC1_YearDown;
  float bos_AK4AK4_pt_jesEC2_YearUp;
  float bos_AK4AK4_pt_jesEC2_YearDown;
  float bos_AK4AK4_m_jesEC2_YearUp;
  float bos_AK4AK4_m_jesEC2_YearDown;
  float bos_AK4AK4_pt_jesAbsolute_YearUp;
  float bos_AK4AK4_pt_jesAbsolute_YearDown;
  float bos_AK4AK4_m_jesAbsolute_YearUp;
  float bos_AK4AK4_m_jesAbsolute_YearDown;
  float bos_AK4AK4_pt_jesHF_YearUp;
  float bos_AK4AK4_pt_jesHF_YearDown;
  float bos_AK4AK4_m_jesHF_YearUp;
  float bos_AK4AK4_m_jesHF_YearDown;
  float bos_AK4AK4_pt_jesRelativeSample_YearUp;
  float bos_AK4AK4_pt_jesRelativeSample_YearDown;
  float bos_AK4AK4_m_jesRelativeSample_YearUp;
  float bos_AK4AK4_m_jesRelativeSample_YearDown;
  float bos_AK4AK4_pt_jesTotalUp;
  float bos_AK4AK4_pt_jesTotalDown;
  float bos_AK4AK4_m_jesTotalUp;
  float bos_AK4AK4_m_jesTotalDown;

  //------------------------------------//
  //       FINAL STATE VARIABLES        //
  //------------------------------------//

  float dibos_m;
  float dibos_mt;
  float dibos_pt;
  float dibos_eta;
  float dibos_phi;

  // lepton scale variation
  float dibos_m_scaleUp;
  float dibos_m_scaleDown;
  float dibos_mt_scaleUp;
  float dibos_mt_scaleDown;
  float dibos_pt_scaleUp;
  float dibos_pt_scaleDown;

  // JES variation
  float dibos_m_jesFlavorQCDUp;
  float dibos_m_jesFlavorQCDDown;
  float dibos_mt_jesFlavorQCDUp;
  float dibos_mt_jesFlavorQCDDown;
  float dibos_pt_jesFlavorQCDUp;
  float dibos_pt_jesFlavorQCDDown;
  float dibos_m_jesRelativeBalUp;
  float dibos_m_jesRelativeBalDown;
  float dibos_mt_jesRelativeBalUp;
  float dibos_mt_jesRelativeBalDown;
  float dibos_pt_jesRelativeBalUp;
  float dibos_pt_jesRelativeBalDown;
  float dibos_m_jesHFUp;
  float dibos_m_jesHFDown;
  float dibos_mt_jesHFUp;
  float dibos_mt_jesHFDown;
  float dibos_pt_jesHFUp;
  float dibos_pt_jesHFDown;
  float dibos_m_jesBBEC1Up;
  float dibos_m_jesBBEC1Down;
  float dibos_mt_jesBBEC1Up;
  float dibos_mt_jesBBEC1Down;
  float dibos_pt_jesBBEC1Up;
  float dibos_pt_jesBBEC1Down;
  float dibos_m_jesEC2Up;
  float dibos_m_jesEC2Down;
  float dibos_mt_jesEC2Up;
  float dibos_mt_jesEC2Down;
  float dibos_pt_jesEC2Up;
  float dibos_pt_jesEC2Down;
  float dibos_m_jesAbsoluteUp;
  float dibos_m_jesAbsoluteDown;
  float dibos_mt_jesAbsoluteUp;
  float dibos_mt_jesAbsoluteDown;
  float dibos_pt_jesAbsoluteUp;
  float dibos_pt_jesAbsoluteDown;
  float dibos_m_jesBBEC1_YearUp;
  float dibos_m_jesBBEC1_YearDown;
  float dibos_mt_jesBBEC1_YearUp;
  float dibos_mt_jesBBEC1_YearDown;
  float dibos_pt_jesBBEC1_YearUp;
  float dibos_pt_jesBBEC1_YearDown;
  float dibos_m_jesEC2_YearUp;
  float dibos_m_jesEC2_YearDown;
  float dibos_mt_jesEC2_YearUp;
  float dibos_mt_jesEC2_YearDown;
  float dibos_pt_jesEC2_YearUp;
  float dibos_pt_jesEC2_YearDown;
  float dibos_m_jesAbsolute_YearUp;
  float dibos_m_jesAbsolute_YearDown;
  float dibos_mt_jesAbsolute_YearUp;
  float dibos_mt_jesAbsolute_YearDown;
  float dibos_pt_jesAbsolute_YearUp;
  float dibos_pt_jesAbsolute_YearDown;
  float dibos_m_jesHF_YearUp;
  float dibos_m_jesHF_YearDown;
  float dibos_mt_jesHF_YearUp;
  float dibos_mt_jesHF_YearDown;
  float dibos_pt_jesHF_YearUp;
  float dibos_pt_jesHF_YearDown;
  float dibos_m_jesRelativeSample_YearUp;
  float dibos_m_jesRelativeSample_YearDown;
  float dibos_mt_jesRelativeSample_YearUp;
  float dibos_mt_jesRelativeSample_YearDown;
  float dibos_pt_jesRelativeSample_YearUp;
  float dibos_pt_jesRelativeSample_YearDown;
  float dibos_m_jesTotalUp;
  float dibos_m_jesTotalDown;
  float dibos_mt_jesTotalUp;
  float dibos_mt_jesTotalDown;
  float dibos_pt_jesTotalUp;
  float dibos_pt_jesTotalDown;

  float bosCent;
  float zeppLep;
  float zeppHad;

};

#endif
