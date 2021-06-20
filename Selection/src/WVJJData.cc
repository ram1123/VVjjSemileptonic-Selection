#include "../interface/WVJJData.hh"

void WVJJData::init() {

  clearVars();

  //metadata and weights
  fTree->Branch("run",&run,"run/i");
  fTree->Branch("ls",&ls,"ls/i");
  fTree->Branch("evt",&evt,"evt/l");
  fTree->Branch("nPV",&nPV,"nPV/F");
  fTree->Branch("nPU_mean",&nPU_mean,"nPU_mean/F");
  fTree->Branch("genWeight",&genWeight,"genWeight/F");
  fTree->Branch("puWeight",&puWeight,"puWeight/F");
  fTree->Branch("puWeight_Up",&puWeight_Up,"puWeight_Up/F");
  fTree->Branch("puWeight_Down",&puWeight_Down,"puWeight_Down/F");
  fTree->Branch("L1PFWeight",&L1PFWeight,"L1PFWeight/F");
  fTree->Branch("L1PFWeight_Up",&L1PFWeight_Up,"L1PFWeight_Up/F");
  fTree->Branch("L1PFWeight_Down",&L1PFWeight_Down,"L1PFWeight_Down/F");
  // LHE Weights
  fTree->Branch("nScaleWeight",&nScaleWeight,"nScaleWeight/I");
  fTree->Branch("nPdfWeight",&nPdfWeight,"nPdfWeight/I");
  fTree->Branch("nAqgcWeight",&nAqgcWeight,"nAqgcWeight/I");
  fTree->Branch("scaleWeight",&scaleWeight[0],"scaleWeight[nScaleWeight]/F");
  fTree->Branch("pdfWeight",&pdfWeight[0],"pdfWeight[nPdfWeight]/F");
  fTree->Branch("aqgcWeight",&aqgcWeight[0],"aqgcWeight[nAqgcWeight]/F");
  // tZq veto
  fTree->Branch("is_tZq",&is_tZq,"is_tZq/O");
  //jet counters
  fTree->Branch("nJet30",&nJet30,"nJet30/I");
  fTree->Branch("nJet50",&nJet50,"nJet50/I");
  //btag counters
  fTree->Branch("nBtag_loose",&nBtag_loose,"nBtag_loose/I");
  fTree->Branch("nBtag_medium",&nBtag_medium,"nBtag_medium/I");
  fTree->Branch("nBtag_tight",&nBtag_tight,"nBtag_tight/I");
  fTree->Branch("btagWeight_loose",&btagWeight_loose,"btagWeight_loose/F");
  fTree->Branch("btagWeight_loose_Up",&btagWeight_loose_Up,"btagWeight_loose_Up/F");
  fTree->Branch("btagWeight_loose_Down",&btagWeight_loose_Down,"btagWeight_loose_Down/F");
  fTree->Branch("btagWeight_medium",&btagWeight_medium,"btagWeight_medium/F");
  fTree->Branch("btagWeight_medium_Up",&btagWeight_medium_Up,"btagWeight_medium_Up/F");
  fTree->Branch("btagWeight_medium_Down",&btagWeight_medium_Down,"btagWeight_medium_Down/F");
  fTree->Branch("btagWeight_tight",&btagWeight_tight,"btagWeight_tight/F");
  fTree->Branch("btagWeight_tight_Up",&btagWeight_tight_Up,"btagWeight_tight_Up/F");
  fTree->Branch("btagWeight_tight_Down",&btagWeight_tight_Down,"btagWeight_tight_Down/F");
  //trigger
  fTree->Branch("trigger_1Mu",&trigger_1Mu,"trigger_1Mu/O");
  fTree->Branch("trigger_2Mu",&trigger_2Mu,"trigger_2Mu/O");
  fTree->Branch("trigger_1El",&trigger_1El,"trigger_1El/O");
  fTree->Branch("trigger_2El",&trigger_2El,"trigger_2El/O");

  fTree->Branch("isAntiIso",&isAntiIso,"isAntiIso/O");
  fTree->Branch("lepFakeRate",&lepFakeRate,"lepFakeRate/F");
  //lepton 1
  fTree->Branch("lep1_pt",&lep1_pt,"lep1_pt/F");
  fTree->Branch("lep1_eta",&lep1_eta,"lep1_eta/F");
  fTree->Branch("lep1_phi",&lep1_phi,"lep1_phi/F");
  fTree->Branch("lep1_m",&lep1_m,"lep1_m/F");
  fTree->Branch("lep1_q",&lep1_q,"lep1_q/F");
  fTree->Branch("lep1_iso",&lep1_iso,"lep1_iso/F");
  fTree->Branch("lep1_dxy",&lep1_dxy,"lep1_dxy/F");
  fTree->Branch("lep1_dz",&lep1_dz,"lep1_dz/F");
  fTree->Branch("lep1_idEffWeight",&lep1_idEffWeight,"lep1_idEffWeight/F");
  fTree->Branch("lep1_trigEffWeight",&lep1_trigEffWeight,"lep1_trigEffWeight/F");
  //lepton 1 scale variations
  fTree->Branch("lep1_pt_scaleUp",&lep1_pt_scaleUp,"lep1_pt_scaleUp/F");
  fTree->Branch("lep1_pt_scaleDown",&lep1_pt_scaleDown,"lep1_pt_scaleDown/F");
  //lepton 2
  fTree->Branch("lep2_pt",&lep2_pt,"lep2_pt/F");
  fTree->Branch("lep2_eta",&lep2_eta,"lep2_eta/F");
  fTree->Branch("lep2_phi",&lep2_phi,"lep2_phi/F");
  fTree->Branch("lep2_m",&lep2_m,"lep2_m/F");
  fTree->Branch("lep2_q",&lep2_q,"lep2_q/F");
  fTree->Branch("lep2_iso",&lep2_iso,"lep2_iso/F");
  fTree->Branch("lep2_dxy",&lep2_dxy,"lep2_dxy/F");
  fTree->Branch("lep2_dz",&lep2_dz,"lep2_dz/F");
  fTree->Branch("lep2_idEffWeight",&lep2_idEffWeight,"lep2_idEffWeight/F");
  fTree->Branch("lep2_trigEffWeight",&lep2_trigEffWeight,"lep2_trigEffWeight/F");
  //lepton 2 scale variations
  fTree->Branch("lep2_pt_scaleUp",&lep2_pt_scaleUp,"lep2_pt_scaleUp/F");
  fTree->Branch("lep2_pt_scaleDown",&lep2_pt_scaleDown,"lep2_pt_scaleDown/F");
  //dilepton final state
  fTree->Branch("dilep_m",&dilep_m,"dilep_m/F");
  fTree->Branch("dilep_mt",&dilep_mt,"dilep_mt/F");
  fTree->Branch("dilep_pt",&dilep_pt,"dilep_pt/F");
  fTree->Branch("dilep_eta",&dilep_eta,"dilep_eta/F");
  fTree->Branch("dilep_phi",&dilep_phi,"dilep_phi/F");
  //dilepton JES variations
  fTree->Branch("dilep_m_jesFlavorQCDUp",&dilep_m_jesFlavorQCDUp,"dilep_m_jesFlavorQCDUp/F");
  fTree->Branch("dilep_m_jesFlavorQCDDown",&dilep_m_jesFlavorQCDDown,"dilep_m_jesFlavorQCDDown/F");
  fTree->Branch("dilep_m_jesRelativeBalUp",&dilep_m_jesRelativeBalUp,"dilep_m_jesRelativeBalUp/F");
  fTree->Branch("dilep_m_jesRelativeBalDown",&dilep_m_jesRelativeBalDown,"dilep_m_jesRelativeBalDown/F");
  fTree->Branch("dilep_m_jesHFUp",&dilep_m_jesHFUp,"dilep_m_jesHFUp/F");
  fTree->Branch("dilep_m_jesHFDown",&dilep_m_jesHFDown,"dilep_m_jesHFDown/F");
  fTree->Branch("dilep_m_jesBBEC1Up",&dilep_m_jesBBEC1Up,"dilep_m_jesBBEC1Up/F");
  fTree->Branch("dilep_m_jesBBEC1Down",&dilep_m_jesBBEC1Down,"dilep_m_jesBBEC1Down/F");
  fTree->Branch("dilep_m_jesEC2Up",&dilep_m_jesEC2Up,"dilep_m_jesEC2Up/F");
  fTree->Branch("dilep_m_jesEC2Down",&dilep_m_jesEC2Down,"dilep_m_jesEC2Down/F");
  fTree->Branch("dilep_m_jesAbsoluteUp",&dilep_m_jesAbsoluteUp,"dilep_m_jesAbsoluteUp/F");
  fTree->Branch("dilep_m_jesAbsoluteDown",&dilep_m_jesAbsoluteDown,"dilep_m_jesAbsoluteDown/F");
  fTree->Branch("dilep_m_jesBBEC1_YearUp",&dilep_m_jesBBEC1_YearUp,"dilep_m_jesBBEC1_YearUp/F");
  fTree->Branch("dilep_m_jesBBEC1_YearDown",&dilep_m_jesBBEC1_YearDown,"dilep_m_jesBBEC1_YearDown/F");
  fTree->Branch("dilep_m_jesEC2_YearUp",&dilep_m_jesEC2_YearUp,"dilep_m_jesEC2_YearUp/F");
  fTree->Branch("dilep_m_jesEC2_YearDown",&dilep_m_jesEC2_YearDown,"dilep_m_jesEC2_YearDown/F");
  fTree->Branch("dilep_m_jesAbsolute_YearUp",&dilep_m_jesAbsolute_YearUp,"dilep_m_jesAbsolute_YearUp/F");
  fTree->Branch("dilep_m_jesAbsolute_YearDown",&dilep_m_jesAbsolute_YearDown,"dilep_m_jesAbsolute_YearDown/F");
  fTree->Branch("dilep_m_jesHF_YearUp",&dilep_m_jesHF_YearUp,"dilep_m_jesHF_YearUp/F");
  fTree->Branch("dilep_m_jesHF_YearDown",&dilep_m_jesHF_YearDown,"dilep_m_jesHF_YearDown/F");
  fTree->Branch("dilep_m_jesRelativeSample_YearUp",&dilep_m_jesRelativeSample_YearUp,"dilep_m_jesRelativeSample_YearUp/F");
  fTree->Branch("dilep_m_jesRelativeSample_YearDown",&dilep_m_jesRelativeSample_YearDown,"dilep_m_jesRelativeSample_YearDown/F");
  fTree->Branch("dilep_m_jesTotalUp",&dilep_m_jesTotalUp,"dilep_m_jesTotalUp/F");
  fTree->Branch("dilep_m_jesTotalDown",&dilep_m_jesTotalDown,"dilep_m_jesTotalDown/F");
  fTree->Branch("dilep_mt_jesFlavorQCDUp",&dilep_mt_jesFlavorQCDUp,"dilep_mt_jesFlavorQCDUp/F");
  fTree->Branch("dilep_mt_jesFlavorQCDDown",&dilep_mt_jesFlavorQCDDown,"dilep_mt_jesFlavorQCDDown/F");
  fTree->Branch("dilep_mt_jesRelativeBalUp",&dilep_mt_jesRelativeBalUp,"dilep_mt_jesRelativeBalUp/F");
  fTree->Branch("dilep_mt_jesRelativeBalDown",&dilep_mt_jesRelativeBalDown,"dilep_mt_jesRelativeBalDown/F");
  fTree->Branch("dilep_mt_jesHFUp",&dilep_mt_jesHFUp,"dilep_mt_jesHFUp/F");
  fTree->Branch("dilep_mt_jesHFDown",&dilep_mt_jesHFDown,"dilep_mt_jesHFDown/F");
  fTree->Branch("dilep_mt_jesBBEC1Up",&dilep_mt_jesBBEC1Up,"dilep_mt_jesBBEC1Up/F");
  fTree->Branch("dilep_mt_jesBBEC1Down",&dilep_mt_jesBBEC1Down,"dilep_mt_jesBBEC1Down/F");
  fTree->Branch("dilep_mt_jesEC2Up",&dilep_mt_jesEC2Up,"dilep_mt_jesEC2Up/F");
  fTree->Branch("dilep_mt_jesEC2Down",&dilep_mt_jesEC2Down,"dilep_mt_jesEC2Down/F");
  fTree->Branch("dilep_mt_jesAbsoluteUp",&dilep_mt_jesAbsoluteUp,"dilep_mt_jesAbsoluteUp/F");
  fTree->Branch("dilep_mt_jesAbsoluteDown",&dilep_mt_jesAbsoluteDown,"dilep_mt_jesAbsoluteDown/F");
  fTree->Branch("dilep_mt_jesBBEC1_YearUp",&dilep_mt_jesBBEC1_YearUp,"dilep_mt_jesBBEC1_YearUp/F");
  fTree->Branch("dilep_mt_jesBBEC1_YearDown",&dilep_mt_jesBBEC1_YearDown,"dilep_mt_jesBBEC1_YearDown/F");
  fTree->Branch("dilep_mt_jesEC2_YearUp",&dilep_mt_jesEC2_YearUp,"dilep_mt_jesEC2_YearUp/F");
  fTree->Branch("dilep_mt_jesEC2_YearDown",&dilep_mt_jesEC2_YearDown,"dilep_mt_jesEC2_YearDown/F");
  fTree->Branch("dilep_mt_jesAbsolute_YearUp",&dilep_mt_jesAbsolute_YearUp,"dilep_mt_jesAbsolute_YearUp/F");
  fTree->Branch("dilep_mt_jesAbsolute_YearDown",&dilep_mt_jesAbsolute_YearDown,"dilep_mt_jesAbsolute_YearDown/F");
  fTree->Branch("dilep_mt_jesHF_YearUp",&dilep_mt_jesHF_YearUp,"dilep_mt_jesHF_YearUp/F");
  fTree->Branch("dilep_mt_jesHF_YearDown",&dilep_mt_jesHF_YearDown,"dilep_mt_jesHF_YearDown/F");
  fTree->Branch("dilep_mt_jesRelativeSample_YearUp",&dilep_mt_jesRelativeSample_YearUp,"dilep_mt_jesRelativeSample_YearUp/F");
  fTree->Branch("dilep_mt_jesRelativeSample_YearDown",&dilep_mt_jesRelativeSample_YearDown,"dilep_mt_jesRelativeSample_YearDown/F");
  fTree->Branch("dilep_mt_jesTotalUp",&dilep_mt_jesTotalUp,"dilep_mt_jesTotalUp/F");
  fTree->Branch("dilep_mt_jesTotalDown",&dilep_mt_jesTotalDown,"dilep_mt_jesTotalDown/F");
  fTree->Branch("dilep_pt_jesFlavorQCDUp",&dilep_pt_jesFlavorQCDUp,"dilep_pt_jesFlavorQCDUp/F");
  fTree->Branch("dilep_pt_jesFlavorQCDDown",&dilep_pt_jesFlavorQCDDown,"dilep_pt_jesFlavorQCDDown/F");
  fTree->Branch("dilep_pt_jesRelativeBalUp",&dilep_pt_jesRelativeBalUp,"dilep_pt_jesRelativeBalUp/F");
  fTree->Branch("dilep_pt_jesRelativeBalDown",&dilep_pt_jesRelativeBalDown,"dilep_pt_jesRelativeBalDown/F");
  fTree->Branch("dilep_pt_jesHFUp",&dilep_pt_jesHFUp,"dilep_pt_jesHFUp/F");
  fTree->Branch("dilep_pt_jesHFDown",&dilep_pt_jesHFDown,"dilep_pt_jesHFDown/F");
  fTree->Branch("dilep_pt_jesBBEC1Up",&dilep_pt_jesBBEC1Up,"dilep_pt_jesBBEC1Up/F");
  fTree->Branch("dilep_pt_jesBBEC1Down",&dilep_pt_jesBBEC1Down,"dilep_pt_jesBBEC1Down/F");
  fTree->Branch("dilep_pt_jesEC2Up",&dilep_pt_jesEC2Up,"dilep_pt_jesEC2Up/F");
  fTree->Branch("dilep_pt_jesEC2Down",&dilep_pt_jesEC2Down,"dilep_pt_jesEC2Down/F");
  fTree->Branch("dilep_pt_jesAbsoluteUp",&dilep_pt_jesAbsoluteUp,"dilep_pt_jesAbsoluteUp/F");
  fTree->Branch("dilep_pt_jesAbsoluteDown",&dilep_pt_jesAbsoluteDown,"dilep_pt_jesAbsoluteDown/F");
  fTree->Branch("dilep_pt_jesBBEC1_YearUp",&dilep_pt_jesBBEC1_YearUp,"dilep_pt_jesBBEC1_YearUp/F");
  fTree->Branch("dilep_pt_jesBBEC1_YearDown",&dilep_pt_jesBBEC1_YearDown,"dilep_pt_jesBBEC1_YearDown/F");
  fTree->Branch("dilep_pt_jesEC2_YearUp",&dilep_pt_jesEC2_YearUp,"dilep_pt_jesEC2_YearUp/F");
  fTree->Branch("dilep_pt_jesEC2_YearDown",&dilep_pt_jesEC2_YearDown,"dilep_pt_jesEC2_YearDown/F");
  fTree->Branch("dilep_pt_jesAbsolute_YearUp",&dilep_pt_jesAbsolute_YearUp,"dilep_pt_jesAbsolute_YearUp/F");
  fTree->Branch("dilep_pt_jesAbsolute_YearDown",&dilep_pt_jesAbsolute_YearDown,"dilep_pt_jesAbsolute_YearDown/F");
  fTree->Branch("dilep_pt_jesHF_YearUp",&dilep_pt_jesHF_YearUp,"dilep_pt_jesHF_YearUp/F");
  fTree->Branch("dilep_pt_jesHF_YearDown",&dilep_pt_jesHF_YearDown,"dilep_pt_jesHF_YearDown/F");
  fTree->Branch("dilep_pt_jesRelativeSample_YearUp",&dilep_pt_jesRelativeSample_YearUp,"dilep_pt_jesRelativeSample_YearUp/F");
  fTree->Branch("dilep_pt_jesRelativeSample_YearDown",&dilep_pt_jesRelativeSample_YearDown,"dilep_pt_jesRelativeSample_YearDown/F");
  fTree->Branch("dilep_pt_jesTotalUp",&dilep_pt_jesTotalUp,"dilep_pt_jesTotalUp/F");
  fTree->Branch("dilep_pt_jesTotalDown",&dilep_pt_jesTotalDown,"dilep_pt_jesTotalDown/F");
  //dilepton lepton scale variations
  fTree->Branch("dilep_m_scaleUp",&dilep_m_scaleUp,"dilep_m_scaleUp/F");
  fTree->Branch("dilep_m_scaleDown",&dilep_m_scaleDown,"dilep_m_scaleDown/F");
  fTree->Branch("dilep_mt_scaleUp",&dilep_mt_scaleUp,"dilep_mt_scaleUp/F");
  fTree->Branch("dilep_mt_scaleDown",&dilep_mt_scaleDown,"dilep_mt_scaleDown/F");
  fTree->Branch("dilep_pt_scaleUp",&dilep_pt_scaleUp,"dilep_pt_scaleUp/F");
  fTree->Branch("dilep_pt_scaleDown",&dilep_pt_scaleDown,"dilep_pt_scaleDown/F");
  //MET
  fTree->Branch("MET",&MET,"MET/F");
  fTree->Branch("MET_phi",&MET_phi,"MET_phi/F");
  fTree->Branch("MET_2017",&MET_2017,"MET_2017/F");
  fTree->Branch("MET_jesFlavorQCDUp",&MET_jesFlavorQCDUp,"MET_jesFlavorQCDUp/F");
  fTree->Branch("MET_jesFlavorQCDDown",&MET_jesFlavorQCDDown,"MET_jesFlavorQCDDown/F");
  fTree->Branch("MET_jesRelativeBalUp",&MET_jesRelativeBalUp,"MET_jesRelativeBalUp/F");
  fTree->Branch("MET_jesRelativeBalDown",&MET_jesRelativeBalDown,"MET_jesRelativeBalDown/F");
  fTree->Branch("MET_jesHFUp",&MET_jesHFUp,"MET_jesHFUp/F");
  fTree->Branch("MET_jesHFDown",&MET_jesHFDown,"MET_jesHFDown/F");
  fTree->Branch("MET_jesBBEC1Up",&MET_jesBBEC1Up,"MET_jesBBEC1Up/F");
  fTree->Branch("MET_jesBBEC1Down",&MET_jesBBEC1Down,"MET_jesBBEC1Down/F");
  fTree->Branch("MET_jesEC2Up",&MET_jesEC2Up,"MET_jesEC2Up/F");
  fTree->Branch("MET_jesEC2Down",&MET_jesEC2Down,"MET_jesEC2Down/F");
  fTree->Branch("MET_jesAbsoluteUp",&MET_jesAbsoluteUp,"MET_jesAbsoluteUp/F");
  fTree->Branch("MET_jesAbsoluteDown",&MET_jesAbsoluteDown,"MET_jesAbsoluteDown/F");
  fTree->Branch("MET_jesBBEC1_YearUp",&MET_jesBBEC1_YearUp,"MET_jesBBEC1_YearUp/F");
  fTree->Branch("MET_jesBBEC1_YearDown",&MET_jesBBEC1_YearDown,"MET_jesBBEC1_YearDown/F");
  fTree->Branch("MET_jesEC2_YearUp",&MET_jesEC2_YearUp,"MET_jesEC2_YearUp/F");
  fTree->Branch("MET_jesEC2_YearDown",&MET_jesEC2_YearDown,"MET_jesEC2_YearDown/F");
  fTree->Branch("MET_jesAbsolute_YearUp",&MET_jesAbsolute_YearUp,"MET_jesAbsolute_YearUp/F");
  fTree->Branch("MET_jesAbsolute_YearDown",&MET_jesAbsolute_YearDown,"MET_jesAbsolute_YearDown/F");
  fTree->Branch("MET_jesHF_YearUp",&MET_jesHF_YearUp,"MET_jesHF_YearUp/F");
  fTree->Branch("MET_jesHF_YearDown",&MET_jesHF_YearDown,"MET_jesHF_YearDown/F");
  fTree->Branch("MET_jesRelativeSample_YearUp",&MET_jesRelativeSample_YearUp,"MET_jesRelativeSample_YearUp/F");
  fTree->Branch("MET_jesRelativeSample_YearDown",&MET_jesRelativeSample_YearDown,"MET_jesRelativeSample_YearDown/F");
  fTree->Branch("MET_jesTotalUp",&MET_jesTotalUp,"MET_jesTotalUp/F");
  fTree->Branch("MET_jesTotalDown",&MET_jesTotalDown,"MET_jesTotalDown/F");
  fTree->Branch("MET_phi_jesFlavorQCDUp",&MET_phi_jesFlavorQCDUp,"MET_phi_jesFlavorQCDUp/F");
  fTree->Branch("MET_phi_jesFlavorQCDDown",&MET_phi_jesFlavorQCDDown,"MET_phi_jesFlavorQCDDown/F");
  fTree->Branch("MET_phi_jesRelativeBalUp",&MET_phi_jesRelativeBalUp,"MET_phi_jesRelativeBalUp/F");
  fTree->Branch("MET_phi_jesRelativeBalDown",&MET_phi_jesRelativeBalDown,"MET_phi_jesRelativeBalDown/F");
  fTree->Branch("MET_phi_jesHFUp",&MET_phi_jesHFUp,"MET_phi_jesHFUp/F");
  fTree->Branch("MET_phi_jesHFDown",&MET_phi_jesHFDown,"MET_phi_jesHFDown/F");
  fTree->Branch("MET_phi_jesBBEC1Up",&MET_phi_jesBBEC1Up,"MET_phi_jesBBEC1Up/F");
  fTree->Branch("MET_phi_jesBBEC1Down",&MET_phi_jesBBEC1Down,"MET_phi_jesBBEC1Down/F");
  fTree->Branch("MET_phi_jesEC2Up",&MET_phi_jesEC2Up,"MET_phi_jesEC2Up/F");
  fTree->Branch("MET_phi_jesEC2Down",&MET_phi_jesEC2Down,"MET_phi_jesEC2Down/F");
  fTree->Branch("MET_phi_jesAbsoluteUp",&MET_phi_jesAbsoluteUp,"MET_phi_jesAbsoluteUp/F");
  fTree->Branch("MET_phi_jesAbsoluteDown",&MET_phi_jesAbsoluteDown,"MET_phi_jesAbsoluteDown/F");
  fTree->Branch("MET_phi_jesBBEC1_YearUp",&MET_phi_jesBBEC1_YearUp,"MET_phi_jesBBEC1_YearUp/F");
  fTree->Branch("MET_phi_jesBBEC1_YearDown",&MET_phi_jesBBEC1_YearDown,"MET_phi_jesBBEC1_YearDown/F");
  fTree->Branch("MET_phi_jesEC2_YearUp",&MET_phi_jesEC2_YearUp,"MET_phi_jesEC2_YearUp/F");
  fTree->Branch("MET_phi_jesEC2_YearDown",&MET_phi_jesEC2_YearDown,"MET_phi_jesEC2_YearDown/F");
  fTree->Branch("MET_phi_jesAbsolute_YearUp",&MET_phi_jesAbsolute_YearUp,"MET_phi_jesAbsolute_YearUp/F");
  fTree->Branch("MET_phi_jesAbsolute_YearDown",&MET_phi_jesAbsolute_YearDown,"MET_phi_jesAbsolute_YearDown/F");
  fTree->Branch("MET_phi_jesHF_YearUp",&MET_phi_jesHF_YearUp,"MET_phi_jesHF_YearUp/F");
  fTree->Branch("MET_phi_jesHF_YearDown",&MET_phi_jesHF_YearDown,"MET_phi_jesHF_YearDown/F");
  fTree->Branch("MET_phi_jesRelativeSample_YearUp",&MET_phi_jesRelativeSample_YearUp,"MET_phi_jesRelativeSample_YearUp/F");
  fTree->Branch("MET_phi_jesRelativeSample_YearDown",&MET_phi_jesRelativeSample_YearDown,"MET_phi_jesRelativeSample_YearDown/F");
  fTree->Branch("MET_phi_jesTotalUp",&MET_phi_jesTotalUp,"MET_phi_jesTotalUp/F");
  fTree->Branch("MET_phi_jesTotalDown",&MET_phi_jesTotalDown,"MET_phi_jesTotalDown/F");
  //W neutrino pZ
  fTree->Branch("neu_pz_type0",&neu_pz_type0,"neu_pz_type0/F");
  //Boson AK8 jet
  fTree->Branch("bos_PuppiAK8_m_sd0",&bos_PuppiAK8_m_sd0,"bos_PuppiAK8_m_sd0/F");
  fTree->Branch("bos_PuppiAK8_m_sd0_corr",&bos_PuppiAK8_m_sd0_corr,"bos_PuppiAK8_m_sd0_corr/F");
  fTree->Branch("bos_PuppiAK8_pt",&bos_PuppiAK8_pt,"bos_PuppiAK8_pt/F");
  fTree->Branch("bos_PuppiAK8_eta",&bos_PuppiAK8_eta,"bos_PuppiAK8_eta/F");
  fTree->Branch("bos_PuppiAK8_phi",&bos_PuppiAK8_phi,"bos_PuppiAK8_phi/F");
  fTree->Branch("bos_PuppiAK8_tau2tau1",&bos_PuppiAK8_tau2tau1,"bos_PuppiAK8_tau2tau1/F");
  //Boson AK8 jet variations
  fTree->Branch("bos_PuppiAK8_pt_jesFlavorQCDUp",&bos_PuppiAK8_pt_jesFlavorQCDUp,"bos_PuppiAK8_pt_jesFlavorQCDUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesFlavorQCDDown",&bos_PuppiAK8_pt_jesFlavorQCDDown,"bos_PuppiAK8_pt_jesFlavorQCDDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesRelativeBalUp",&bos_PuppiAK8_pt_jesRelativeBalUp,"bos_PuppiAK8_pt_jesRelativeBalUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesRelativeBalDown",&bos_PuppiAK8_pt_jesRelativeBalDown,"bos_PuppiAK8_pt_jesRelativeBalDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesHFUp",&bos_PuppiAK8_pt_jesHFUp,"bos_PuppiAK8_pt_jesHFUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesHFDown",&bos_PuppiAK8_pt_jesHFDown,"bos_PuppiAK8_pt_jesHFDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesBBEC1Up",&bos_PuppiAK8_pt_jesBBEC1Up,"bos_PuppiAK8_pt_jesBBEC1Up/F");
  fTree->Branch("bos_PuppiAK8_pt_jesBBEC1Down",&bos_PuppiAK8_pt_jesBBEC1Down,"bos_PuppiAK8_pt_jesBBEC1Down/F");
  fTree->Branch("bos_PuppiAK8_pt_jesEC2Up",&bos_PuppiAK8_pt_jesEC2Up,"bos_PuppiAK8_pt_jesEC2Up/F");
  fTree->Branch("bos_PuppiAK8_pt_jesEC2Down",&bos_PuppiAK8_pt_jesEC2Down,"bos_PuppiAK8_pt_jesEC2Down/F");
  fTree->Branch("bos_PuppiAK8_pt_jesAbsoluteUp",&bos_PuppiAK8_pt_jesAbsoluteUp,"bos_PuppiAK8_pt_jesAbsoluteUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesAbsoluteDown",&bos_PuppiAK8_pt_jesAbsoluteDown,"bos_PuppiAK8_pt_jesAbsoluteDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesBBEC1_YearUp",&bos_PuppiAK8_pt_jesBBEC1_YearUp,"bos_PuppiAK8_pt_jesBBEC1_YearUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesBBEC1_YearDown",&bos_PuppiAK8_pt_jesBBEC1_YearDown,"bos_PuppiAK8_pt_jesBBEC1_YearDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesEC2_YearUp",&bos_PuppiAK8_pt_jesEC2_YearUp,"bos_PuppiAK8_pt_jesEC2_YearUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesEC2_YearDown",&bos_PuppiAK8_pt_jesEC2_YearDown,"bos_PuppiAK8_pt_jesEC2_YearDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesAbsolute_YearUp",&bos_PuppiAK8_pt_jesAbsolute_YearUp,"bos_PuppiAK8_pt_jesAbsolute_YearUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesAbsolute_YearDown",&bos_PuppiAK8_pt_jesAbsolute_YearDown,"bos_PuppiAK8_pt_jesAbsolute_YearDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesHF_YearUp",&bos_PuppiAK8_pt_jesHF_YearUp,"bos_PuppiAK8_pt_jesHF_YearUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesHF_YearDown",&bos_PuppiAK8_pt_jesHF_YearDown,"bos_PuppiAK8_pt_jesHF_YearDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesRelativeSample_YearUp",&bos_PuppiAK8_pt_jesRelativeSample_YearUp,"bos_PuppiAK8_pt_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesRelativeSample_YearDown",&bos_PuppiAK8_pt_jesRelativeSample_YearDown,"bos_PuppiAK8_pt_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_PuppiAK8_pt_jesTotalUp",&bos_PuppiAK8_pt_jesTotalUp,"bos_PuppiAK8_pt_jesTotalUp/F");
  fTree->Branch("bos_PuppiAK8_pt_jesTotalDown",&bos_PuppiAK8_pt_jesTotalDown,"bos_PuppiAK8_pt_jesTotalDown/F");
  //Boson AK8 correlation variations
  //fTree->Branch("bos_PuppiAK8_e2_sdb1", &bos_PuppiAK8_e2_sdb1, "bos_PuppiAK8_e2_sdb1/F");
  //fTree->Branch("bos_PuppiAK8_e3_sdb1", &bos_PuppiAK8_e3_sdb1, "bos_PuppiAK8_e3_sdb1/F");
  //fTree->Branch("bos_PuppiAK8_e3_v1_sdb1", &bos_PuppiAK8_e3_v1_sdb1, "bos_PuppiAK8_e3_v1_sdb1/F");
  //fTree->Branch("bos_PuppiAK8_e3_v2_sdb1", &bos_PuppiAK8_e3_v2_sdb1, "bos_PuppiAK8_e3_v2_sdb1/F");
  //fTree->Branch("bos_PuppiAK8_e4_v1_sdb1", &bos_PuppiAK8_e4_v1_sdb1, "bos_PuppiAK8_e4_v1_sdb1/F");
  //fTree->Branch("bos_PuppiAK8_e4_v2_sdb1", &bos_PuppiAK8_e4_v2_sdb1, "bos_PuppiAK8_e4_v2_sdb1/F");
  //fTree->Branch("bos_PuppiAK8_e2_sdb2", &bos_PuppiAK8_e2_sdb2, "bos_PuppiAK8_e2_sdb2/F");
  //fTree->Branch("bos_PuppiAK8_e3_sdb2", &bos_PuppiAK8_e3_sdb2, "bos_PuppiAK8_e3_sdb2/F");
  //fTree->Branch("bos_PuppiAK8_e3_v1_sdb2", &bos_PuppiAK8_e3_v1_sdb2, "bos_PuppiAK8_e3_v1_sdb2/F");
  //fTree->Branch("bos_PuppiAK8_e3_v2_sdb2", &bos_PuppiAK8_e3_v2_sdb2, "bos_PuppiAK8_e3_v2_sdb2/F");
  //fTree->Branch("bos_PuppiAK8_e4_v1_sdb2", &bos_PuppiAK8_e4_v1_sdb2, "bos_PuppiAK8_e4_v1_sdb2/F");
  //fTree->Branch("bos_PuppiAK8_e4_v2_sdb2", &bos_PuppiAK8_e4_v2_sdb2, "bos_PuppiAK8_e4_v2_sdb2/F");
  //Boson AK4 jet 1
  fTree->Branch("bos_j1_AK4_pt",&bos_j1_AK4_pt,"bos_j1_AK4_pt/F");
  fTree->Branch("bos_j1_AK4_eta",&bos_j1_AK4_eta,"bos_j1_AK4_eta/F");
  fTree->Branch("bos_j1_AK4_phi",&bos_j1_AK4_phi,"bos_j1_AK4_phi/F");
  fTree->Branch("bos_j1_AK4_m",&bos_j1_AK4_m,"bos_j1_AK4_m/F");
  fTree->Branch("bos_j1_AK4_puid_tight",&bos_j1_AK4_puid_tight,"bos_j1_AK4_puid_tight/O");
  fTree->Branch("bos_j1_AK4_puidSF_tight",&bos_j1_AK4_puidSF_tight,"bos_j1_AK4_puidSF_tight/F");
  fTree->Branch("bos_j1_AK4_puidSF_tight_Up",&bos_j1_AK4_puidSF_tight_Up,"bos_j1_AK4_puidSF_tight_Up/F");
  fTree->Branch("bos_j1_AK4_puidSF_tight_Down",&bos_j1_AK4_puidSF_tight_Down,"bos_j1_AK4_puidSF_tight_Down/F");
  //Boson AK4 jet 1 variations
  fTree->Branch("bos_j1_AK4_pt_jesFlavorQCDUp",&bos_j1_AK4_pt_jesFlavorQCDUp,"bos_j1_AK4_pt_jesFlavorQCDUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesFlavorQCDDown",&bos_j1_AK4_pt_jesFlavorQCDDown,"bos_j1_AK4_pt_jesFlavorQCDDown/F");
  fTree->Branch("bos_j1_AK4_m_jesFlavorQCDUp",&bos_j1_AK4_m_jesFlavorQCDUp,"bos_j1_AK4_m_jesFlavorQCDUp/F");
  fTree->Branch("bos_j1_AK4_m_jesFlavorQCDDown",&bos_j1_AK4_m_jesFlavorQCDDown,"bos_j1_AK4_m_jesFlavorQCDDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesRelativeBalUp",&bos_j1_AK4_pt_jesRelativeBalUp,"bos_j1_AK4_pt_jesRelativeBalUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesRelativeBalDown",&bos_j1_AK4_pt_jesRelativeBalDown,"bos_j1_AK4_pt_jesRelativeBalDown/F");
  fTree->Branch("bos_j1_AK4_m_jesRelativeBalUp",&bos_j1_AK4_m_jesRelativeBalUp,"bos_j1_AK4_m_jesRelativeBalUp/F");
  fTree->Branch("bos_j1_AK4_m_jesRelativeBalDown",&bos_j1_AK4_m_jesRelativeBalDown,"bos_j1_AK4_m_jesRelativeBalDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesHFUp",&bos_j1_AK4_pt_jesHFUp,"bos_j1_AK4_pt_jesHFUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesHFDown",&bos_j1_AK4_pt_jesHFDown,"bos_j1_AK4_pt_jesHFDown/F");
  fTree->Branch("bos_j1_AK4_m_jesHFUp",&bos_j1_AK4_m_jesHFUp,"bos_j1_AK4_m_jesHFUp/F");
  fTree->Branch("bos_j1_AK4_m_jesHFDown",&bos_j1_AK4_m_jesHFDown,"bos_j1_AK4_m_jesHFDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesBBEC1Up",&bos_j1_AK4_pt_jesBBEC1Up,"bos_j1_AK4_pt_jesBBEC1Up/F");
  fTree->Branch("bos_j1_AK4_pt_jesBBEC1Down",&bos_j1_AK4_pt_jesBBEC1Down,"bos_j1_AK4_pt_jesBBEC1Down/F");
  fTree->Branch("bos_j1_AK4_m_jesBBEC1Up",&bos_j1_AK4_m_jesBBEC1Up,"bos_j1_AK4_m_jesBBEC1Up/F");
  fTree->Branch("bos_j1_AK4_m_jesBBEC1Down",&bos_j1_AK4_m_jesBBEC1Down,"bos_j1_AK4_m_jesBBEC1Down/F");
  fTree->Branch("bos_j1_AK4_pt_jesEC2Up",&bos_j1_AK4_pt_jesEC2Up,"bos_j1_AK4_pt_jesEC2Up/F");
  fTree->Branch("bos_j1_AK4_pt_jesEC2Down",&bos_j1_AK4_pt_jesEC2Down,"bos_j1_AK4_pt_jesEC2Down/F");
  fTree->Branch("bos_j1_AK4_m_jesEC2Up",&bos_j1_AK4_m_jesEC2Up,"bos_j1_AK4_m_jesEC2Up/F");
  fTree->Branch("bos_j1_AK4_m_jesEC2Down",&bos_j1_AK4_m_jesEC2Down,"bos_j1_AK4_m_jesEC2Down/F");
  fTree->Branch("bos_j1_AK4_pt_jesAbsoluteUp",&bos_j1_AK4_pt_jesAbsoluteUp,"bos_j1_AK4_pt_jesAbsoluteUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesAbsoluteDown",&bos_j1_AK4_pt_jesAbsoluteDown,"bos_j1_AK4_pt_jesAbsoluteDown/F");
  fTree->Branch("bos_j1_AK4_m_jesAbsoluteUp",&bos_j1_AK4_m_jesAbsoluteUp,"bos_j1_AK4_m_jesAbsoluteUp/F");
  fTree->Branch("bos_j1_AK4_m_jesAbsoluteDown",&bos_j1_AK4_m_jesAbsoluteDown,"bos_j1_AK4_m_jesAbsoluteDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesBBEC1_YearUp",&bos_j1_AK4_pt_jesBBEC1_YearUp,"bos_j1_AK4_pt_jesBBEC1_YearUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesBBEC1_YearDown",&bos_j1_AK4_pt_jesBBEC1_YearDown,"bos_j1_AK4_pt_jesBBEC1_YearDown/F");
  fTree->Branch("bos_j1_AK4_m_jesBBEC1_YearUp",&bos_j1_AK4_m_jesBBEC1_YearUp,"bos_j1_AK4_m_jesBBEC1_YearUp/F");
  fTree->Branch("bos_j1_AK4_m_jesBBEC1_YearDown",&bos_j1_AK4_m_jesBBEC1_YearDown,"bos_j1_AK4_m_jesBBEC1_YearDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesEC2_YearUp",&bos_j1_AK4_pt_jesEC2_YearUp,"bos_j1_AK4_pt_jesEC2_YearUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesEC2_YearDown",&bos_j1_AK4_pt_jesEC2_YearDown,"bos_j1_AK4_pt_jesEC2_YearDown/F");
  fTree->Branch("bos_j1_AK4_m_jesEC2_YearUp",&bos_j1_AK4_m_jesEC2_YearUp,"bos_j1_AK4_m_jesEC2_YearUp/F");
  fTree->Branch("bos_j1_AK4_m_jesEC2_YearDown",&bos_j1_AK4_m_jesEC2_YearDown,"bos_j1_AK4_m_jesEC2_YearDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesAbsolute_YearUp",&bos_j1_AK4_pt_jesAbsolute_YearUp,"bos_j1_AK4_pt_jesAbsolute_YearUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesAbsolute_YearDown",&bos_j1_AK4_pt_jesAbsolute_YearDown,"bos_j1_AK4_pt_jesAbsolute_YearDown/F");
  fTree->Branch("bos_j1_AK4_m_jesAbsolute_YearUp",&bos_j1_AK4_m_jesAbsolute_YearUp,"bos_j1_AK4_m_jesAbsolute_YearUp/F");
  fTree->Branch("bos_j1_AK4_m_jesAbsolute_YearDown",&bos_j1_AK4_m_jesAbsolute_YearDown,"bos_j1_AK4_m_jesAbsolute_YearDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesHF_YearUp",&bos_j1_AK4_pt_jesHF_YearUp,"bos_j1_AK4_pt_jesHF_YearUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesHF_YearDown",&bos_j1_AK4_pt_jesHF_YearDown,"bos_j1_AK4_pt_jesHF_YearDown/F");
  fTree->Branch("bos_j1_AK4_m_jesHF_YearUp",&bos_j1_AK4_m_jesHF_YearUp,"bos_j1_AK4_m_jesHF_YearUp/F");
  fTree->Branch("bos_j1_AK4_m_jesHF_YearDown",&bos_j1_AK4_m_jesHF_YearDown,"bos_j1_AK4_m_jesHF_YearDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesRelativeSample_YearUp",&bos_j1_AK4_pt_jesRelativeSample_YearUp,"bos_j1_AK4_pt_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesRelativeSample_YearDown",&bos_j1_AK4_pt_jesRelativeSample_YearDown,"bos_j1_AK4_pt_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_j1_AK4_m_jesRelativeSample_YearUp",&bos_j1_AK4_m_jesRelativeSample_YearUp,"bos_j1_AK4_m_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_j1_AK4_m_jesRelativeSample_YearDown",&bos_j1_AK4_m_jesRelativeSample_YearDown,"bos_j1_AK4_m_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_j1_AK4_pt_jesTotalUp",&bos_j1_AK4_pt_jesTotalUp,"bos_j1_AK4_pt_jesTotalUp/F");
  fTree->Branch("bos_j1_AK4_pt_jesTotalDown",&bos_j1_AK4_pt_jesTotalDown,"bos_j1_AK4_pt_jesTotalDown/F");
  fTree->Branch("bos_j1_AK4_m_jesTotalUp",&bos_j1_AK4_m_jesTotalUp,"bos_j1_AK4_m_jesTotalUp/F");
  fTree->Branch("bos_j1_AK4_m_jesTotalDown",&bos_j1_AK4_m_jesTotalDown,"bos_j1_AK4_m_jesTotalDown/F");
  //Boson AK4 jet 2
  fTree->Branch("bos_j2_AK4_pt",&bos_j2_AK4_pt,"bos_j2_AK4_pt/F");
  fTree->Branch("bos_j2_AK4_eta",&bos_j2_AK4_eta,"bos_j2_AK4_eta/F");
  fTree->Branch("bos_j2_AK4_phi",&bos_j2_AK4_phi,"bos_j2_AK4_phi/F");
  fTree->Branch("bos_j2_AK4_m",&bos_j2_AK4_m,"bos_j2_AK4_m/F");
  fTree->Branch("bos_j2_AK4_puid_tight",&bos_j2_AK4_puid_tight,"bos_j2_AK4_puid_tight/O");
  fTree->Branch("bos_j2_AK4_puidSF_tight",&bos_j2_AK4_puidSF_tight,"bos_j2_AK4_puidSF_tight/F");
  fTree->Branch("bos_j2_AK4_puidSF_tight_Up",&bos_j2_AK4_puidSF_tight_Up,"bos_j2_AK4_puidSF_tight_Up/F");
  fTree->Branch("bos_j2_AK4_puidSF_tight_Down",&bos_j2_AK4_puidSF_tight_Down,"bos_j2_AK4_puidSF_tight_Down/F");
  //Boson AK4 jet 2 variations
  fTree->Branch("bos_j2_AK4_pt_jesFlavorQCDUp",&bos_j2_AK4_pt_jesFlavorQCDUp,"bos_j2_AK4_pt_jesFlavorQCDUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesFlavorQCDDown",&bos_j2_AK4_pt_jesFlavorQCDDown,"bos_j2_AK4_pt_jesFlavorQCDDown/F");
  fTree->Branch("bos_j2_AK4_m_jesFlavorQCDUp",&bos_j2_AK4_m_jesFlavorQCDUp,"bos_j2_AK4_m_jesFlavorQCDUp/F");
  fTree->Branch("bos_j2_AK4_m_jesFlavorQCDDown",&bos_j2_AK4_m_jesFlavorQCDDown,"bos_j2_AK4_m_jesFlavorQCDDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesRelativeBalUp",&bos_j2_AK4_pt_jesRelativeBalUp,"bos_j2_AK4_pt_jesRelativeBalUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesRelativeBalDown",&bos_j2_AK4_pt_jesRelativeBalDown,"bos_j2_AK4_pt_jesRelativeBalDown/F");
  fTree->Branch("bos_j2_AK4_m_jesRelativeBalUp",&bos_j2_AK4_m_jesRelativeBalUp,"bos_j2_AK4_m_jesRelativeBalUp/F");
  fTree->Branch("bos_j2_AK4_m_jesRelativeBalDown",&bos_j2_AK4_m_jesRelativeBalDown,"bos_j2_AK4_m_jesRelativeBalDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesHFUp",&bos_j2_AK4_pt_jesHFUp,"bos_j2_AK4_pt_jesHFUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesHFDown",&bos_j2_AK4_pt_jesHFDown,"bos_j2_AK4_pt_jesHFDown/F");
  fTree->Branch("bos_j2_AK4_m_jesHFUp",&bos_j2_AK4_m_jesHFUp,"bos_j2_AK4_m_jesHFUp/F");
  fTree->Branch("bos_j2_AK4_m_jesHFDown",&bos_j2_AK4_m_jesHFDown,"bos_j2_AK4_m_jesHFDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesBBEC1Up",&bos_j2_AK4_pt_jesBBEC1Up,"bos_j2_AK4_pt_jesBBEC1Up/F");
  fTree->Branch("bos_j2_AK4_pt_jesBBEC1Down",&bos_j2_AK4_pt_jesBBEC1Down,"bos_j2_AK4_pt_jesBBEC1Down/F");
  fTree->Branch("bos_j2_AK4_m_jesBBEC1Up",&bos_j2_AK4_m_jesBBEC1Up,"bos_j2_AK4_m_jesBBEC1Up/F");
  fTree->Branch("bos_j2_AK4_m_jesBBEC1Down",&bos_j2_AK4_m_jesBBEC1Down,"bos_j2_AK4_m_jesBBEC1Down/F");
  fTree->Branch("bos_j2_AK4_pt_jesEC2Up",&bos_j2_AK4_pt_jesEC2Up,"bos_j2_AK4_pt_jesEC2Up/F");
  fTree->Branch("bos_j2_AK4_pt_jesEC2Down",&bos_j2_AK4_pt_jesEC2Down,"bos_j2_AK4_pt_jesEC2Down/F");
  fTree->Branch("bos_j2_AK4_m_jesEC2Up",&bos_j2_AK4_m_jesEC2Up,"bos_j2_AK4_m_jesEC2Up/F");
  fTree->Branch("bos_j2_AK4_m_jesEC2Down",&bos_j2_AK4_m_jesEC2Down,"bos_j2_AK4_m_jesEC2Down/F");
  fTree->Branch("bos_j2_AK4_pt_jesAbsoluteUp",&bos_j2_AK4_pt_jesAbsoluteUp,"bos_j2_AK4_pt_jesAbsoluteUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesAbsoluteDown",&bos_j2_AK4_pt_jesAbsoluteDown,"bos_j2_AK4_pt_jesAbsoluteDown/F");
  fTree->Branch("bos_j2_AK4_m_jesAbsoluteUp",&bos_j2_AK4_m_jesAbsoluteUp,"bos_j2_AK4_m_jesAbsoluteUp/F");
  fTree->Branch("bos_j2_AK4_m_jesAbsoluteDown",&bos_j2_AK4_m_jesAbsoluteDown,"bos_j2_AK4_m_jesAbsoluteDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesBBEC1_YearUp",&bos_j2_AK4_pt_jesBBEC1_YearUp,"bos_j2_AK4_pt_jesBBEC1_YearUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesBBEC1_YearDown",&bos_j2_AK4_pt_jesBBEC1_YearDown,"bos_j2_AK4_pt_jesBBEC1_YearDown/F");
  fTree->Branch("bos_j2_AK4_m_jesBBEC1_YearUp",&bos_j2_AK4_m_jesBBEC1_YearUp,"bos_j2_AK4_m_jesBBEC1_YearUp/F");
  fTree->Branch("bos_j2_AK4_m_jesBBEC1_YearDown",&bos_j2_AK4_m_jesBBEC1_YearDown,"bos_j2_AK4_m_jesBBEC1_YearDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesEC2_YearUp",&bos_j2_AK4_pt_jesEC2_YearUp,"bos_j2_AK4_pt_jesEC2_YearUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesEC2_YearDown",&bos_j2_AK4_pt_jesEC2_YearDown,"bos_j2_AK4_pt_jesEC2_YearDown/F");
  fTree->Branch("bos_j2_AK4_m_jesEC2_YearUp",&bos_j2_AK4_m_jesEC2_YearUp,"bos_j2_AK4_m_jesEC2_YearUp/F");
  fTree->Branch("bos_j2_AK4_m_jesEC2_YearDown",&bos_j2_AK4_m_jesEC2_YearDown,"bos_j2_AK4_m_jesEC2_YearDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesAbsolute_YearUp",&bos_j2_AK4_pt_jesAbsolute_YearUp,"bos_j2_AK4_pt_jesAbsolute_YearUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesAbsolute_YearDown",&bos_j2_AK4_pt_jesAbsolute_YearDown,"bos_j2_AK4_pt_jesAbsolute_YearDown/F");
  fTree->Branch("bos_j2_AK4_m_jesAbsolute_YearUp",&bos_j2_AK4_m_jesAbsolute_YearUp,"bos_j2_AK4_m_jesAbsolute_YearUp/F");
  fTree->Branch("bos_j2_AK4_m_jesAbsolute_YearDown",&bos_j2_AK4_m_jesAbsolute_YearDown,"bos_j2_AK4_m_jesAbsolute_YearDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesHF_YearUp",&bos_j2_AK4_pt_jesHF_YearUp,"bos_j2_AK4_pt_jesHF_YearUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesHF_YearDown",&bos_j2_AK4_pt_jesHF_YearDown,"bos_j2_AK4_pt_jesHF_YearDown/F");
  fTree->Branch("bos_j2_AK4_m_jesHF_YearUp",&bos_j2_AK4_m_jesHF_YearUp,"bos_j2_AK4_m_jesHF_YearUp/F");
  fTree->Branch("bos_j2_AK4_m_jesHF_YearDown",&bos_j2_AK4_m_jesHF_YearDown,"bos_j2_AK4_m_jesHF_YearDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesRelativeSample_YearUp",&bos_j2_AK4_pt_jesRelativeSample_YearUp,"bos_j2_AK4_pt_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesRelativeSample_YearDown",&bos_j2_AK4_pt_jesRelativeSample_YearDown,"bos_j2_AK4_pt_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_j2_AK4_m_jesRelativeSample_YearUp",&bos_j2_AK4_m_jesRelativeSample_YearUp,"bos_j2_AK4_m_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_j2_AK4_m_jesRelativeSample_YearDown",&bos_j2_AK4_m_jesRelativeSample_YearDown,"bos_j2_AK4_m_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_j2_AK4_pt_jesTotalUp",&bos_j2_AK4_pt_jesTotalUp,"bos_j2_AK4_pt_jesTotalUp/F");
  fTree->Branch("bos_j2_AK4_pt_jesTotalDown",&bos_j2_AK4_pt_jesTotalDown,"bos_j2_AK4_pt_jesTotalDown/F");
  fTree->Branch("bos_j2_AK4_m_jesTotalUp",&bos_j2_AK4_m_jesTotalUp,"bos_j2_AK4_m_jesTotalUp/F");
  fTree->Branch("bos_j2_AK4_m_jesTotalDown",&bos_j2_AK4_m_jesTotalDown,"bos_j2_AK4_m_jesTotalDown/F");
  //Boson dijet object
  fTree->Branch("bos_AK4AK4_pt", &bos_AK4AK4_pt, "bos_AK4AK4_pt/F");
  fTree->Branch("bos_AK4AK4_eta",&bos_AK4AK4_eta,"bos_AK4AK4_eta/F");
  fTree->Branch("bos_AK4AK4_phi",&bos_AK4AK4_phi,"bos_AK4AK4_phi/F");
  fTree->Branch("bos_AK4AK4_m",  &bos_AK4AK4_m,  "bos_AK4AK4_m/F");
  //Boson dijet variations
  fTree->Branch("bos_AK4AK4_pt_jesFlavorQCDUp",&bos_AK4AK4_pt_jesFlavorQCDUp,"bos_AK4AK4_pt_jesFlavorQCDUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesFlavorQCDDown",&bos_AK4AK4_pt_jesFlavorQCDDown,"bos_AK4AK4_pt_jesFlavorQCDDown/F");
  fTree->Branch("bos_AK4AK4_m_jesFlavorQCDUp",&bos_AK4AK4_m_jesFlavorQCDUp,"bos_AK4AK4_m_jesFlavorQCDUp/F");
  fTree->Branch("bos_AK4AK4_m_jesFlavorQCDDown",&bos_AK4AK4_m_jesFlavorQCDDown,"bos_AK4AK4_m_jesFlavorQCDDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesRelativeBalUp",&bos_AK4AK4_pt_jesRelativeBalUp,"bos_AK4AK4_pt_jesRelativeBalUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesRelativeBalDown",&bos_AK4AK4_pt_jesRelativeBalDown,"bos_AK4AK4_pt_jesRelativeBalDown/F");
  fTree->Branch("bos_AK4AK4_m_jesRelativeBalUp",&bos_AK4AK4_m_jesRelativeBalUp,"bos_AK4AK4_m_jesRelativeBalUp/F");
  fTree->Branch("bos_AK4AK4_m_jesRelativeBalDown",&bos_AK4AK4_m_jesRelativeBalDown,"bos_AK4AK4_m_jesRelativeBalDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesHFUp",&bos_AK4AK4_pt_jesHFUp,"bos_AK4AK4_pt_jesHFUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesHFDown",&bos_AK4AK4_pt_jesHFDown,"bos_AK4AK4_pt_jesHFDown/F");
  fTree->Branch("bos_AK4AK4_m_jesHFUp",&bos_AK4AK4_m_jesHFUp,"bos_AK4AK4_m_jesHFUp/F");
  fTree->Branch("bos_AK4AK4_m_jesHFDown",&bos_AK4AK4_m_jesHFDown,"bos_AK4AK4_m_jesHFDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesBBEC1Up",&bos_AK4AK4_pt_jesBBEC1Up,"bos_AK4AK4_pt_jesBBEC1Up/F");
  fTree->Branch("bos_AK4AK4_pt_jesBBEC1Down",&bos_AK4AK4_pt_jesBBEC1Down,"bos_AK4AK4_pt_jesBBEC1Down/F");
  fTree->Branch("bos_AK4AK4_m_jesBBEC1Up",&bos_AK4AK4_m_jesBBEC1Up,"bos_AK4AK4_m_jesBBEC1Up/F");
  fTree->Branch("bos_AK4AK4_m_jesBBEC1Down",&bos_AK4AK4_m_jesBBEC1Down,"bos_AK4AK4_m_jesBBEC1Down/F");
  fTree->Branch("bos_AK4AK4_pt_jesEC2Up",&bos_AK4AK4_pt_jesEC2Up,"bos_AK4AK4_pt_jesEC2Up/F");
  fTree->Branch("bos_AK4AK4_pt_jesEC2Down",&bos_AK4AK4_pt_jesEC2Down,"bos_AK4AK4_pt_jesEC2Down/F");
  fTree->Branch("bos_AK4AK4_m_jesEC2Up",&bos_AK4AK4_m_jesEC2Up,"bos_AK4AK4_m_jesEC2Up/F");
  fTree->Branch("bos_AK4AK4_m_jesEC2Down",&bos_AK4AK4_m_jesEC2Down,"bos_AK4AK4_m_jesEC2Down/F");
  fTree->Branch("bos_AK4AK4_pt_jesAbsoluteUp",&bos_AK4AK4_pt_jesAbsoluteUp,"bos_AK4AK4_pt_jesAbsoluteUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesAbsoluteDown",&bos_AK4AK4_pt_jesAbsoluteDown,"bos_AK4AK4_pt_jesAbsoluteDown/F");
  fTree->Branch("bos_AK4AK4_m_jesAbsoluteUp",&bos_AK4AK4_m_jesAbsoluteUp,"bos_AK4AK4_m_jesAbsoluteUp/F");
  fTree->Branch("bos_AK4AK4_m_jesAbsoluteDown",&bos_AK4AK4_m_jesAbsoluteDown,"bos_AK4AK4_m_jesAbsoluteDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesBBEC1_YearUp",&bos_AK4AK4_pt_jesBBEC1_YearUp,"bos_AK4AK4_pt_jesBBEC1_YearUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesBBEC1_YearDown",&bos_AK4AK4_pt_jesBBEC1_YearDown,"bos_AK4AK4_pt_jesBBEC1_YearDown/F");
  fTree->Branch("bos_AK4AK4_m_jesBBEC1_YearUp",&bos_AK4AK4_m_jesBBEC1_YearUp,"bos_AK4AK4_m_jesBBEC1_YearUp/F");
  fTree->Branch("bos_AK4AK4_m_jesBBEC1_YearDown",&bos_AK4AK4_m_jesBBEC1_YearDown,"bos_AK4AK4_m_jesBBEC1_YearDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesEC2_YearUp",&bos_AK4AK4_pt_jesEC2_YearUp,"bos_AK4AK4_pt_jesEC2_YearUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesEC2_YearDown",&bos_AK4AK4_pt_jesEC2_YearDown,"bos_AK4AK4_pt_jesEC2_YearDown/F");
  fTree->Branch("bos_AK4AK4_m_jesEC2_YearUp",&bos_AK4AK4_m_jesEC2_YearUp,"bos_AK4AK4_m_jesEC2_YearUp/F");
  fTree->Branch("bos_AK4AK4_m_jesEC2_YearDown",&bos_AK4AK4_m_jesEC2_YearDown,"bos_AK4AK4_m_jesEC2_YearDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesAbsolute_YearUp",&bos_AK4AK4_pt_jesAbsolute_YearUp,"bos_AK4AK4_pt_jesAbsolute_YearUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesAbsolute_YearDown",&bos_AK4AK4_pt_jesAbsolute_YearDown,"bos_AK4AK4_pt_jesAbsolute_YearDown/F");
  fTree->Branch("bos_AK4AK4_m_jesAbsolute_YearUp",&bos_AK4AK4_m_jesAbsolute_YearUp,"bos_AK4AK4_m_jesAbsolute_YearUp/F");
  fTree->Branch("bos_AK4AK4_m_jesAbsolute_YearDown",&bos_AK4AK4_m_jesAbsolute_YearDown,"bos_AK4AK4_m_jesAbsolute_YearDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesHF_YearUp",&bos_AK4AK4_pt_jesHF_YearUp,"bos_AK4AK4_pt_jesHF_YearUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesHF_YearDown",&bos_AK4AK4_pt_jesHF_YearDown,"bos_AK4AK4_pt_jesHF_YearDown/F");
  fTree->Branch("bos_AK4AK4_m_jesHF_YearUp",&bos_AK4AK4_m_jesHF_YearUp,"bos_AK4AK4_m_jesHF_YearUp/F");
  fTree->Branch("bos_AK4AK4_m_jesHF_YearDown",&bos_AK4AK4_m_jesHF_YearDown,"bos_AK4AK4_m_jesHF_YearDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesRelativeSample_YearUp",&bos_AK4AK4_pt_jesRelativeSample_YearUp,"bos_AK4AK4_pt_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesRelativeSample_YearDown",&bos_AK4AK4_pt_jesRelativeSample_YearDown,"bos_AK4AK4_pt_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_AK4AK4_m_jesRelativeSample_YearUp",&bos_AK4AK4_m_jesRelativeSample_YearUp,"bos_AK4AK4_m_jesRelativeSample_YearUp/F");
  fTree->Branch("bos_AK4AK4_m_jesRelativeSample_YearDown",&bos_AK4AK4_m_jesRelativeSample_YearDown,"bos_AK4AK4_m_jesRelativeSample_YearDown/F");
  fTree->Branch("bos_AK4AK4_pt_jesTotalUp",&bos_AK4AK4_pt_jesTotalUp,"bos_AK4AK4_pt_jesTotalUp/F");
  fTree->Branch("bos_AK4AK4_pt_jesTotalDown",&bos_AK4AK4_pt_jesTotalDown,"bos_AK4AK4_pt_jesTotalDown/F");
  fTree->Branch("bos_AK4AK4_m_jesTotalUp",&bos_AK4AK4_m_jesTotalUp,"bos_AK4AK4_m_jesTotalUp/F");
  fTree->Branch("bos_AK4AK4_m_jesTotalDown",&bos_AK4AK4_m_jesTotalDown,"bos_AK4AK4_m_jesTotalDown/F");
  //final state variables
  fTree->Branch("dibos_m", &dibos_m, "dibos_m/F");
  fTree->Branch("dibos_mt", &dibos_mt, "dibos_mt/F");
  fTree->Branch("dibos_pt", &dibos_pt, "dibos_pt/F");
  fTree->Branch("dibos_eta", &dibos_eta, "dibos_eta/F");
  fTree->Branch("dibos_phi", &dibos_phi, "dibos_phi/F");

  fTree->Branch("dibos_m_scaleUp", &dibos_m_scaleUp, "dibos_m_scaleUp/F");
  fTree->Branch("dibos_m_scaleDown", &dibos_m_scaleDown, "dibos_m_scaleDown/F");
  fTree->Branch("dibos_mt_scaleUp", &dibos_mt_scaleUp, "dibos_mt_scaleUp/F");
  fTree->Branch("dibos_mt_scaleDown", &dibos_mt_scaleDown, "dibos_mt_scaleDown/F");
  fTree->Branch("dibos_pt_scaleUp", &dibos_pt_scaleUp, "dibos_pt_scaleUp/F");
  fTree->Branch("dibos_pt_scaleDown", &dibos_pt_scaleDown, "dibos_pt_scaleDown/F");

  fTree->Branch("dibos_m_jesFlavorQCDUp", &dibos_m_jesFlavorQCDUp, "dibos_m_jesFlavorQCDUp/F");
  fTree->Branch("dibos_m_jesFlavorQCDDown", &dibos_m_jesFlavorQCDDown, "dibos_m_jesFlavorQCDDown/F");
  fTree->Branch("dibos_mt_jesFlavorQCDUp", &dibos_mt_jesFlavorQCDUp, "dibos_mt_jesFlavorQCDUp/F");
  fTree->Branch("dibos_mt_jesFlavorQCDDown", &dibos_mt_jesFlavorQCDDown, "dibos_mt_jesFlavorQCDDown/F");
  fTree->Branch("dibos_pt_jesFlavorQCDUp", &dibos_pt_jesFlavorQCDUp, "dibos_pt_jesFlavorQCDUp/F");
  fTree->Branch("dibos_pt_jesFlavorQCDDown", &dibos_pt_jesFlavorQCDDown, "dibos_pt_jesFlavorQCDDown/F");
  fTree->Branch("dibos_m_jesRelativeBalUp", &dibos_m_jesRelativeBalUp, "dibos_m_jesRelativeBalUp/F");
  fTree->Branch("dibos_m_jesRelativeBalDown", &dibos_m_jesRelativeBalDown, "dibos_m_jesRelativeBalDown/F");
  fTree->Branch("dibos_mt_jesRelativeBalUp", &dibos_mt_jesRelativeBalUp, "dibos_mt_jesRelativeBalUp/F");
  fTree->Branch("dibos_mt_jesRelativeBalDown", &dibos_mt_jesRelativeBalDown, "dibos_mt_jesRelativeBalDown/F");
  fTree->Branch("dibos_pt_jesRelativeBalUp", &dibos_pt_jesRelativeBalUp, "dibos_pt_jesRelativeBalUp/F");
  fTree->Branch("dibos_pt_jesRelativeBalDown", &dibos_pt_jesRelativeBalDown, "dibos_pt_jesRelativeBalDown/F");
  fTree->Branch("dibos_m_jesHFUp", &dibos_m_jesHFUp, "dibos_m_jesHFUp/F");
  fTree->Branch("dibos_m_jesHFDown", &dibos_m_jesHFDown, "dibos_m_jesHFDown/F");
  fTree->Branch("dibos_mt_jesHFUp", &dibos_mt_jesHFUp, "dibos_mt_jesHFUp/F");
  fTree->Branch("dibos_mt_jesHFDown", &dibos_mt_jesHFDown, "dibos_mt_jesHFDown/F");
  fTree->Branch("dibos_pt_jesHFUp", &dibos_pt_jesHFUp, "dibos_pt_jesHFUp/F");
  fTree->Branch("dibos_pt_jesHFDown", &dibos_pt_jesHFDown, "dibos_pt_jesHFDown/F");
  fTree->Branch("dibos_m_jesBBEC1Up", &dibos_m_jesBBEC1Up, "dibos_m_jesBBEC1Up/F");
  fTree->Branch("dibos_m_jesBBEC1Down", &dibos_m_jesBBEC1Down, "dibos_m_jesBBEC1Down/F");
  fTree->Branch("dibos_mt_jesBBEC1Up", &dibos_mt_jesBBEC1Up, "dibos_mt_jesBBEC1Up/F");
  fTree->Branch("dibos_mt_jesBBEC1Down", &dibos_mt_jesBBEC1Down, "dibos_mt_jesBBEC1Down/F");
  fTree->Branch("dibos_pt_jesBBEC1Up", &dibos_pt_jesBBEC1Up, "dibos_pt_jesBBEC1Up/F");
  fTree->Branch("dibos_pt_jesBBEC1Down", &dibos_pt_jesBBEC1Down, "dibos_pt_jesBBEC1Down/F");
  fTree->Branch("dibos_m_jesEC2Up", &dibos_m_jesEC2Up, "dibos_m_jesEC2Up/F");
  fTree->Branch("dibos_m_jesEC2Down", &dibos_m_jesEC2Down, "dibos_m_jesEC2Down/F");
  fTree->Branch("dibos_mt_jesEC2Up", &dibos_mt_jesEC2Up, "dibos_mt_jesEC2Up/F");
  fTree->Branch("dibos_mt_jesEC2Down", &dibos_mt_jesEC2Down, "dibos_mt_jesEC2Down/F");
  fTree->Branch("dibos_pt_jesEC2Up", &dibos_pt_jesEC2Up, "dibos_pt_jesEC2Up/F");
  fTree->Branch("dibos_pt_jesEC2Down", &dibos_pt_jesEC2Down, "dibos_pt_jesEC2Down/F");
  fTree->Branch("dibos_m_jesAbsoluteUp", &dibos_m_jesAbsoluteUp, "dibos_m_jesAbsoluteUp/F");
  fTree->Branch("dibos_m_jesAbsoluteDown", &dibos_m_jesAbsoluteDown, "dibos_m_jesAbsoluteDown/F");
  fTree->Branch("dibos_mt_jesAbsoluteUp", &dibos_mt_jesAbsoluteUp, "dibos_mt_jesAbsoluteUp/F");
  fTree->Branch("dibos_mt_jesAbsoluteDown", &dibos_mt_jesAbsoluteDown, "dibos_mt_jesAbsoluteDown/F");
  fTree->Branch("dibos_pt_jesAbsoluteUp", &dibos_pt_jesAbsoluteUp, "dibos_pt_jesAbsoluteUp/F");
  fTree->Branch("dibos_pt_jesAbsoluteDown", &dibos_pt_jesAbsoluteDown, "dibos_pt_jesAbsoluteDown/F");
  fTree->Branch("dibos_m_jesBBEC1_YearUp", &dibos_m_jesBBEC1_YearUp, "dibos_m_jesBBEC1_YearUp/F");
  fTree->Branch("dibos_m_jesBBEC1_YearDown", &dibos_m_jesBBEC1_YearDown, "dibos_m_jesBBEC1_YearDown/F");
  fTree->Branch("dibos_mt_jesBBEC1_YearUp", &dibos_mt_jesBBEC1_YearUp, "dibos_mt_jesBBEC1_YearUp/F");
  fTree->Branch("dibos_mt_jesBBEC1_YearDown", &dibos_mt_jesBBEC1_YearDown, "dibos_mt_jesBBEC1_YearDown/F");
  fTree->Branch("dibos_pt_jesBBEC1_YearUp", &dibos_pt_jesBBEC1_YearUp, "dibos_pt_jesBBEC1_YearUp/F");
  fTree->Branch("dibos_pt_jesBBEC1_YearDown", &dibos_pt_jesBBEC1_YearDown, "dibos_pt_jesBBEC1_YearDown/F");
  fTree->Branch("dibos_m_jesEC2_YearUp", &dibos_m_jesEC2_YearUp, "dibos_m_jesEC2_YearUp/F");
  fTree->Branch("dibos_m_jesEC2_YearDown", &dibos_m_jesEC2_YearDown, "dibos_m_jesEC2_YearDown/F");
  fTree->Branch("dibos_mt_jesEC2_YearUp", &dibos_mt_jesEC2_YearUp, "dibos_mt_jesEC2_YearUp/F");
  fTree->Branch("dibos_mt_jesEC2_YearDown", &dibos_mt_jesEC2_YearDown, "dibos_mt_jesEC2_YearDown/F");
  fTree->Branch("dibos_pt_jesEC2_YearUp", &dibos_pt_jesEC2_YearUp, "dibos_pt_jesEC2_YearUp/F");
  fTree->Branch("dibos_pt_jesEC2_YearDown", &dibos_pt_jesEC2_YearDown, "dibos_pt_jesEC2_YearDown/F");
  fTree->Branch("dibos_m_jesAbsolute_YearUp", &dibos_m_jesAbsolute_YearUp, "dibos_m_jesAbsolute_YearUp/F");
  fTree->Branch("dibos_m_jesAbsolute_YearDown", &dibos_m_jesAbsolute_YearDown, "dibos_m_jesAbsolute_YearDown/F");
  fTree->Branch("dibos_mt_jesAbsolute_YearUp", &dibos_mt_jesAbsolute_YearUp, "dibos_mt_jesAbsolute_YearUp/F");
  fTree->Branch("dibos_mt_jesAbsolute_YearDown", &dibos_mt_jesAbsolute_YearDown, "dibos_mt_jesAbsolute_YearDown/F");
  fTree->Branch("dibos_pt_jesAbsolute_YearUp", &dibos_pt_jesAbsolute_YearUp, "dibos_pt_jesAbsolute_YearUp/F");
  fTree->Branch("dibos_pt_jesAbsolute_YearDown", &dibos_pt_jesAbsolute_YearDown, "dibos_pt_jesAbsolute_YearDown/F");
  fTree->Branch("dibos_m_jesHF_YearUp", &dibos_m_jesHF_YearUp, "dibos_m_jesHF_YearUp/F");
  fTree->Branch("dibos_m_jesHF_YearDown", &dibos_m_jesHF_YearDown, "dibos_m_jesHF_YearDown/F");
  fTree->Branch("dibos_mt_jesHF_YearUp", &dibos_mt_jesHF_YearUp, "dibos_mt_jesHF_YearUp/F");
  fTree->Branch("dibos_mt_jesHF_YearDown", &dibos_mt_jesHF_YearDown, "dibos_mt_jesHF_YearDown/F");
  fTree->Branch("dibos_pt_jesHF_YearUp", &dibos_pt_jesHF_YearUp, "dibos_pt_jesHF_YearUp/F");
  fTree->Branch("dibos_pt_jesHF_YearDown", &dibos_pt_jesHF_YearDown, "dibos_pt_jesHF_YearDown/F");
  fTree->Branch("dibos_m_jesRelativeSample_YearUp", &dibos_m_jesRelativeSample_YearUp, "dibos_m_jesRelativeSample_YearUp/F");
  fTree->Branch("dibos_m_jesRelativeSample_YearDown", &dibos_m_jesRelativeSample_YearDown, "dibos_m_jesRelativeSample_YearDown/F");
  fTree->Branch("dibos_mt_jesRelativeSample_YearUp", &dibos_mt_jesRelativeSample_YearUp, "dibos_mt_jesRelativeSample_YearUp/F");
  fTree->Branch("dibos_mt_jesRelativeSample_YearDown", &dibos_mt_jesRelativeSample_YearDown, "dibos_mt_jesRelativeSample_YearDown/F");
  fTree->Branch("dibos_pt_jesRelativeSample_YearUp", &dibos_pt_jesRelativeSample_YearUp, "dibos_pt_jesRelativeSample_YearUp/F");
  fTree->Branch("dibos_pt_jesRelativeSample_YearDown", &dibos_pt_jesRelativeSample_YearDown, "dibos_pt_jesRelativeSample_YearDown/F");
  fTree->Branch("dibos_m_jesTotalUp", &dibos_m_jesTotalUp, "dibos_m_jesTotalUp/F");
  fTree->Branch("dibos_m_jesTotalDown", &dibos_m_jesTotalDown, "dibos_m_jesTotalDown/F");
  fTree->Branch("dibos_mt_jesTotalUp", &dibos_mt_jesTotalUp, "dibos_mt_jesTotalUp/F");
  fTree->Branch("dibos_mt_jesTotalDown", &dibos_mt_jesTotalDown, "dibos_mt_jesTotalDown/F");
  fTree->Branch("dibos_pt_jesTotalUp", &dibos_pt_jesTotalUp, "dibos_pt_jesTotalUp/F");
  fTree->Branch("dibos_pt_jesTotalDown", &dibos_pt_jesTotalDown, "dibos_pt_jesTotalDown/F");

};

void WVJJData::clearVars() {

  //------------------------------------//
  //       METADATA AND EVENT WEIGHTS   //
  //------------------------------------//
  
  run = 0;
  ls = 0;
  evt = 0;

  nPV = 0;
  nPU_mean = 0;
  
  genWeight = 1.0;
  puWeight = 1.0;
  puWeight_Up = 1.0;
  puWeight_Down = 1.0;

  L1PFWeight = 1.0;
  L1PFWeight_Up = 1.0;
  L1PFWeight_Down = 1.0;
  
  nScaleWeight = 0;
  nPdfWeight = 0;
  nAqgcWeight = 0;
  std::fill_n(scaleWeight,200,0);
  std::fill_n(pdfWeight,200,0);
  std::fill_n(aqgcWeight,1000,0);

  is_tZq = false;

  nJet30 = 0;
  nJet50 = 0;

  nBtag_loose = 0;
  nBtag_medium = 0;
  nBtag_tight = 0;

  btagWeight_loose = 1.0;
  btagWeight_loose_Up = 1.0;
  btagWeight_loose_Down = 1.0;
  btagWeight_medium = 1.0;
  btagWeight_medium_Up = 1.0;
  btagWeight_medium_Down = 1.0;
  btagWeight_tight = 1.0;
  btagWeight_tight_Up = 1.0;
  btagWeight_tight_Down = 1.0;

  trigger_1Mu = false;
  trigger_2Mu = false;
  trigger_1El = false;
  trigger_2El = false;

  isAntiIso = false;
  lepFakeRate = 1.0;  
  
  //------------------------------------//
  //       LEPTONS                      //
  //------------------------------------//

  //lepton 1
  lep1_pt = -999.0;
  lep1_eta = -999.0;
  lep1_phi = -999.0;
  lep1_m = -999.0;
  lep1_q = -999.0;
  lep1_dxy = -999.0;
  lep1_dz = -999.0;
  lep1_sip3d = -999.0;
  lep1_iso = -999.0;
  lep1_idEffWeight = 1.0;
  lep1_trigEffWeight = 1.0;

  //lepton 1 scale variations
  lep1_pt_scaleUp = -999.0;
  lep1_pt_scaleDown = -999.0;
  
  //lepton 2
  lep2_pt = -999.0;
  lep2_eta = -999.0;
  lep2_phi = -999.0;
  lep2_m = -999.0;
  lep2_q = -999.0;
  lep2_dxy = -999.0;
  lep2_dz = -999.0;
  lep2_sip3d = -999.0;
  lep2_iso = -999.0;
  lep2_idEffWeight = 1.0;
  lep2_trigEffWeight = 1.0;

  //lepton 2 scale variations
  lep2_pt_scaleUp = -999.0;
  lep2_pt_scaleDown = -999.0;

  //dilepton final state
  dilep_m = -999.0;
  dilep_mt = -999.0;
  dilep_pt = -999.0;
  dilep_eta = -999.0;
  dilep_phi = -999.0;

  //dilepton JES variations
  dilep_m_jesFlavorQCDUp = -999.0;
  dilep_m_jesFlavorQCDDown = -999.0;
  dilep_m_jesRelativeBalUp = -999.0;
  dilep_m_jesRelativeBalDown = -999.0;
  dilep_m_jesHFUp = -999.0;
  dilep_m_jesHFDown = -999.0;
  dilep_m_jesBBEC1Up = -999.0;
  dilep_m_jesBBEC1Down = -999.0;
  dilep_m_jesEC2Up = -999.0;
  dilep_m_jesEC2Down = -999.0;
  dilep_m_jesAbsoluteUp = -999.0;
  dilep_m_jesAbsoluteDown = -999.0;
  dilep_m_jesBBEC1_YearUp = -999.0;
  dilep_m_jesBBEC1_YearDown = -999.0;
  dilep_m_jesEC2_YearUp = -999.0;
  dilep_m_jesEC2_YearDown = -999.0;
  dilep_m_jesAbsolute_YearUp = -999.0;
  dilep_m_jesAbsolute_YearDown = -999.0;
  dilep_m_jesHF_YearUp = -999.0;
  dilep_m_jesHF_YearDown = -999.0;
  dilep_m_jesRelativeSample_YearUp = -999.0;
  dilep_m_jesRelativeSample_YearDown = -999.0;
  dilep_m_jesTotalUp = -999.0;
  dilep_m_jesTotalDown = -999.0;

  dilep_mt_jesFlavorQCDUp = -999.0;
  dilep_mt_jesFlavorQCDDown = -999.0;
  dilep_mt_jesRelativeBalUp = -999.0;
  dilep_mt_jesRelativeBalDown = -999.0;
  dilep_mt_jesHFUp = -999.0;
  dilep_mt_jesHFDown = -999.0;
  dilep_mt_jesBBEC1Up = -999.0;
  dilep_mt_jesBBEC1Down = -999.0;
  dilep_mt_jesEC2Up = -999.0;
  dilep_mt_jesEC2Down = -999.0;
  dilep_mt_jesAbsoluteUp = -999.0;
  dilep_mt_jesAbsoluteDown = -999.0;
  dilep_mt_jesBBEC1_YearUp = -999.0;
  dilep_mt_jesBBEC1_YearDown = -999.0;
  dilep_mt_jesEC2_YearUp = -999.0;
  dilep_mt_jesEC2_YearDown = -999.0;
  dilep_mt_jesAbsolute_YearUp = -999.0;
  dilep_mt_jesAbsolute_YearDown = -999.0;
  dilep_mt_jesHF_YearUp = -999.0;
  dilep_mt_jesHF_YearDown = -999.0;
  dilep_mt_jesRelativeSample_YearUp = -999.0;
  dilep_mt_jesRelativeSample_YearDown = -999.0;
  dilep_mt_jesTotalUp = -999.0;
  dilep_mt_jesTotalDown = -999.0;

  dilep_pt_jesFlavorQCDUp = -999.0;
  dilep_pt_jesFlavorQCDDown = -999.0;
  dilep_pt_jesRelativeBalUp = -999.0;
  dilep_pt_jesRelativeBalDown = -999.0;
  dilep_pt_jesHFUp = -999.0;
  dilep_pt_jesHFDown = -999.0;
  dilep_pt_jesBBEC1Up = -999.0;
  dilep_pt_jesBBEC1Down = -999.0;
  dilep_pt_jesEC2Up = -999.0;
  dilep_pt_jesEC2Down = -999.0;
  dilep_pt_jesAbsoluteUp = -999.0;
  dilep_pt_jesAbsoluteDown = -999.0;
  dilep_pt_jesBBEC1_YearUp = -999.0;
  dilep_pt_jesBBEC1_YearDown = -999.0;
  dilep_pt_jesEC2_YearUp = -999.0;
  dilep_pt_jesEC2_YearDown = -999.0;
  dilep_pt_jesAbsolute_YearUp = -999.0;
  dilep_pt_jesAbsolute_YearDown = -999.0;
  dilep_pt_jesHF_YearUp = -999.0;
  dilep_pt_jesHF_YearDown = -999.0;
  dilep_pt_jesRelativeSample_YearUp = -999.0;
  dilep_pt_jesRelativeSample_YearDown = -999.0;
  dilep_pt_jesTotalUp = -999.0;
  dilep_pt_jesTotalDown = -999.0;

  //dilepton scale variations
  dilep_m_scaleUp = -999.0;
  dilep_m_scaleDown = -999.0;
  dilep_mt_scaleUp = -999.0;
  dilep_mt_scaleDown = -999.0;
  dilep_pt_scaleUp = -999.0;
  dilep_pt_scaleDown = -999.0;

  //------------------------------------//
  //       MET                          //
  //------------------------------------//

  MET = -999.0;
  MET_phi = -999.0;

  MET_2017 = -999.0;

  MET_jesFlavorQCDUp = -999.0;
  MET_jesFlavorQCDDown = -999.0;
  MET_jesRelativeBalUp = -999.0;
  MET_jesRelativeBalDown = -999.0;
  MET_jesHFUp = -999.0;
  MET_jesHFDown = -999.0;
  MET_jesBBEC1Up = -999.0;
  MET_jesBBEC1Down = -999.0;
  MET_jesEC2Up = -999.0;
  MET_jesEC2Down = -999.0;
  MET_jesAbsoluteUp = -999.0;
  MET_jesAbsoluteDown = -999.0;
  MET_jesBBEC1_YearUp = -999.0;
  MET_jesBBEC1_YearDown = -999.0;
  MET_jesEC2_YearUp = -999.0;
  MET_jesEC2_YearDown = -999.0;
  MET_jesAbsolute_YearUp = -999.0;
  MET_jesAbsolute_YearDown = -999.0;
  MET_jesHF_YearUp = -999.0;
  MET_jesHF_YearDown = -999.0;
  MET_jesRelativeSample_YearUp = -999.0;
  MET_jesRelativeSample_YearDown = -999.0;
  MET_jesTotalUp = -999.0;
  MET_jesTotalDown = -999.0;

  MET_phi_jesFlavorQCDUp = -999.0;
  MET_phi_jesFlavorQCDDown = -999.0;
  MET_phi_jesRelativeBalUp = -999.0;
  MET_phi_jesRelativeBalDown = -999.0;
  MET_phi_jesHFUp = -999.0;
  MET_phi_jesHFDown = -999.0;
  MET_phi_jesBBEC1Up = -999.0;
  MET_phi_jesBBEC1Down = -999.0;
  MET_phi_jesEC2Up = -999.0;
  MET_phi_jesEC2Down = -999.0;
  MET_phi_jesAbsoluteUp = -999.0;
  MET_phi_jesAbsoluteDown = -999.0;
  MET_phi_jesBBEC1_YearUp = -999.0;
  MET_phi_jesBBEC1_YearDown = -999.0;
  MET_phi_jesEC2_YearUp = -999.0;
  MET_phi_jesEC2_YearDown = -999.0;
  MET_phi_jesAbsolute_YearUp = -999.0;
  MET_phi_jesAbsolute_YearDown = -999.0;
  MET_phi_jesHF_YearUp = -999.0;
  MET_phi_jesHF_YearDown = -999.0;
  MET_phi_jesRelativeSample_YearUp = -999.0;
  MET_phi_jesRelativeSample_YearDown = -999.0;
  MET_phi_jesTotalUp = -999.0;
  MET_phi_jesTotalDown = -999.0;


  PuppiMET = -999.0;
  PuppiMET_phi = -999.0;

  neu_pz_type0 = -999.0;

  //------------------------------------//
  //       HADRONIC BOOSTED OBJECTS     //
  //------------------------------------//

  //Boson AK8 jet
  bos_PuppiAK8_m_sd0 = -999.0;
  bos_PuppiAK8_m_sd0_corr = -999.0;
  bos_PuppiAK8_pt = -999.0;
  bos_PuppiAK8_eta = -999.0;
  bos_PuppiAK8_phi = -999.0;
  bos_PuppiAK8_tau2tau1 = -999.0;

  //Boson AK8 jet variations
  //bos_PuppiAK8_m_sd0_corr_scaleUp = -999.0;
  //bos_PuppiAK8_m_sd0_corr_scaleDown = -999.0;
  bos_PuppiAK8_pt_jesFlavorQCDUp = -999.0;
  bos_PuppiAK8_pt_jesFlavorQCDDown = -999.0;
  bos_PuppiAK8_pt_jesRelativeBalUp = -999.0;
  bos_PuppiAK8_pt_jesRelativeBalDown = -999.0;
  bos_PuppiAK8_pt_jesHFUp = -999.0;
  bos_PuppiAK8_pt_jesHFDown = -999.0;
  bos_PuppiAK8_pt_jesBBEC1Up = -999.0;
  bos_PuppiAK8_pt_jesBBEC1Down = -999.0;
  bos_PuppiAK8_pt_jesEC2Up = -999.0;
  bos_PuppiAK8_pt_jesEC2Down = -999.0;
  bos_PuppiAK8_pt_jesAbsoluteUp = -999.0;
  bos_PuppiAK8_pt_jesAbsoluteDown = -999.0;
  bos_PuppiAK8_pt_jesBBEC1_YearUp = -999.0;
  bos_PuppiAK8_pt_jesBBEC1_YearDown = -999.0;
  bos_PuppiAK8_pt_jesEC2_YearUp = -999.0;
  bos_PuppiAK8_pt_jesEC2_YearDown = -999.0;
  bos_PuppiAK8_pt_jesAbsolute_YearUp = -999.0;
  bos_PuppiAK8_pt_jesAbsolute_YearDown = -999.0;
  bos_PuppiAK8_pt_jesHF_YearUp = -999.0;
  bos_PuppiAK8_pt_jesHF_YearDown = -999.0;
  bos_PuppiAK8_pt_jesRelativeSample_YearUp = -999.0;
  bos_PuppiAK8_pt_jesRelativeSample_YearDown = -999.0;
  bos_PuppiAK8_pt_jesTotalUp = -999.0;
  bos_PuppiAK8_pt_jesTotalDown = -999.0;

  //bos_PuppiAK8_e2_sdb1 = -999.0;
  //bos_PuppiAK8_e3_sdb1 = -999.0;
  //bos_PuppiAK8_e3_v1_sdb1 = -999.0;
  //bos_PuppiAK8_e3_v2_sdb1 = -999.0;
  //bos_PuppiAK8_e4_v1_sdb1 = -999.0;
  //bos_PuppiAK8_e4_v2_sdb1 = -999.0;

  //bos_PuppiAK8_e2_sdb2 = -999.0;
  //bos_PuppiAK8_e3_sdb2 = -999.0;
  //bos_PuppiAK8_e3_v1_sdb2 = -999.0;
  //bos_PuppiAK8_e3_v2_sdb2 = -999.0;
  //bos_PuppiAK8_e4_v1_sdb2 = -999.0;
  //bos_PuppiAK8_e4_v2_sdb2 = -999.0;

  //------------------------------------//
  //       HADRONIC RESOLVED OBJECTS    //
  //------------------------------------//

  //Boson AK4 jet 1
  bos_j1_AK4_pt = -999.0;
  bos_j1_AK4_eta = -999.0;
  bos_j1_AK4_phi = -999.0;
  bos_j1_AK4_m = -999.0;
  bos_j1_AK4_qgid = -999.0;
  bos_j1_AK4_puid_tight = true;
  bos_j1_AK4_puidSF_tight = 1.0;
  bos_j1_AK4_puidSF_tight_Up = 1.0;
  bos_j1_AK4_puidSF_tight_Down = 1.0;

  //Boson AK4 jet 1 variations
  bos_j1_AK4_pt_jesFlavorQCDUp = -999.0;
  bos_j1_AK4_pt_jesFlavorQCDDown = -999.0;
  bos_j1_AK4_m_jesFlavorQCDUp = -999.0;
  bos_j1_AK4_m_jesFlavorQCDDown = -999.0;
  bos_j1_AK4_pt_jesRelativeBalUp = -999.0;
  bos_j1_AK4_pt_jesRelativeBalDown = -999.0;
  bos_j1_AK4_m_jesRelativeBalUp = -999.0;
  bos_j1_AK4_m_jesRelativeBalDown = -999.0;
  bos_j1_AK4_pt_jesHFUp = -999.0;
  bos_j1_AK4_pt_jesHFDown = -999.0;
  bos_j1_AK4_m_jesHFUp = -999.0;
  bos_j1_AK4_m_jesHFDown = -999.0;
  bos_j1_AK4_pt_jesBBEC1Up = -999.0;
  bos_j1_AK4_pt_jesBBEC1Down = -999.0;
  bos_j1_AK4_m_jesBBEC1Up = -999.0;
  bos_j1_AK4_m_jesBBEC1Down = -999.0;
  bos_j1_AK4_pt_jesEC2Up = -999.0;
  bos_j1_AK4_pt_jesEC2Down = -999.0;
  bos_j1_AK4_m_jesEC2Up = -999.0;
  bos_j1_AK4_m_jesEC2Down = -999.0;
  bos_j1_AK4_pt_jesAbsoluteUp = -999.0;
  bos_j1_AK4_pt_jesAbsoluteDown = -999.0;
  bos_j1_AK4_m_jesAbsoluteUp = -999.0;
  bos_j1_AK4_m_jesAbsoluteDown = -999.0;
  bos_j1_AK4_pt_jesBBEC1_YearUp = -999.0;
  bos_j1_AK4_pt_jesBBEC1_YearDown = -999.0;
  bos_j1_AK4_m_jesBBEC1_YearUp = -999.0;
  bos_j1_AK4_m_jesBBEC1_YearDown = -999.0;
  bos_j1_AK4_pt_jesEC2_YearUp = -999.0;
  bos_j1_AK4_pt_jesEC2_YearDown = -999.0;
  bos_j1_AK4_m_jesEC2_YearUp = -999.0;
  bos_j1_AK4_m_jesEC2_YearDown = -999.0;
  bos_j1_AK4_pt_jesAbsolute_YearUp = -999.0;
  bos_j1_AK4_pt_jesAbsolute_YearDown = -999.0;
  bos_j1_AK4_m_jesAbsolute_YearUp = -999.0;
  bos_j1_AK4_m_jesAbsolute_YearDown = -999.0;
  bos_j1_AK4_pt_jesHF_YearUp = -999.0;
  bos_j1_AK4_pt_jesHF_YearDown = -999.0;
  bos_j1_AK4_m_jesHF_YearUp = -999.0;
  bos_j1_AK4_m_jesHF_YearDown = -999.0;
  bos_j1_AK4_pt_jesRelativeSample_YearUp = -999.0;
  bos_j1_AK4_pt_jesRelativeSample_YearDown = -999.0;
  bos_j1_AK4_m_jesRelativeSample_YearUp = -999.0;
  bos_j1_AK4_m_jesRelativeSample_YearDown = -999.0;
  bos_j1_AK4_pt_jesTotalUp = -999.0;
  bos_j1_AK4_pt_jesTotalDown = -999.0;
  bos_j1_AK4_m_jesTotalUp = -999.0;
  bos_j1_AK4_m_jesTotalDown = -999.0;

  //Boson AK4 jet 2
  bos_j2_AK4_pt = -999.0;
  bos_j2_AK4_eta = -999.0;
  bos_j2_AK4_phi = -999.0;
  bos_j2_AK4_m = -999.0;
  bos_j2_AK4_qgid = -999.0;
  bos_j2_AK4_puid_tight = true;
  bos_j2_AK4_puidSF_tight = 1.0;
  bos_j2_AK4_puidSF_tight_Up = 1.0;
  bos_j2_AK4_puidSF_tight_Down = 1.0;

  //Boson AK4 jet 2 variations
  bos_j2_AK4_pt_jesFlavorQCDUp = -999.0;
  bos_j2_AK4_pt_jesFlavorQCDDown = -999.0;
  bos_j2_AK4_m_jesFlavorQCDUp = -999.0;
  bos_j2_AK4_m_jesFlavorQCDDown = -999.0;
  bos_j2_AK4_pt_jesRelativeBalUp = -999.0;
  bos_j2_AK4_pt_jesRelativeBalDown = -999.0;
  bos_j2_AK4_m_jesRelativeBalUp = -999.0;
  bos_j2_AK4_m_jesRelativeBalDown = -999.0;
  bos_j2_AK4_pt_jesHFUp = -999.0;
  bos_j2_AK4_pt_jesHFDown = -999.0;
  bos_j2_AK4_m_jesHFUp = -999.0;
  bos_j2_AK4_m_jesHFDown = -999.0;
  bos_j2_AK4_pt_jesBBEC1Up = -999.0;
  bos_j2_AK4_pt_jesBBEC1Down = -999.0;
  bos_j2_AK4_m_jesBBEC1Up = -999.0;
  bos_j2_AK4_m_jesBBEC1Down = -999.0;
  bos_j2_AK4_pt_jesEC2Up = -999.0;
  bos_j2_AK4_pt_jesEC2Down = -999.0;
  bos_j2_AK4_m_jesEC2Up = -999.0;
  bos_j2_AK4_m_jesEC2Down = -999.0;
  bos_j2_AK4_pt_jesAbsoluteUp = -999.0;
  bos_j2_AK4_pt_jesAbsoluteDown = -999.0;
  bos_j2_AK4_m_jesAbsoluteUp = -999.0;
  bos_j2_AK4_m_jesAbsoluteDown = -999.0;
  bos_j2_AK4_pt_jesBBEC1_YearUp = -999.0;
  bos_j2_AK4_pt_jesBBEC1_YearDown = -999.0;
  bos_j2_AK4_m_jesBBEC1_YearUp = -999.0;
  bos_j2_AK4_m_jesBBEC1_YearDown = -999.0;
  bos_j2_AK4_pt_jesEC2_YearUp = -999.0;
  bos_j2_AK4_pt_jesEC2_YearDown = -999.0;
  bos_j2_AK4_m_jesEC2_YearUp = -999.0;
  bos_j2_AK4_m_jesEC2_YearDown = -999.0;
  bos_j2_AK4_pt_jesAbsolute_YearUp = -999.0;
  bos_j2_AK4_pt_jesAbsolute_YearDown = -999.0;
  bos_j2_AK4_m_jesAbsolute_YearUp = -999.0;
  bos_j2_AK4_m_jesAbsolute_YearDown = -999.0;
  bos_j2_AK4_pt_jesHF_YearUp = -999.0;
  bos_j2_AK4_pt_jesHF_YearDown = -999.0;
  bos_j2_AK4_m_jesHF_YearUp = -999.0;
  bos_j2_AK4_m_jesHF_YearDown = -999.0;
  bos_j2_AK4_pt_jesRelativeSample_YearUp = -999.0;
  bos_j2_AK4_pt_jesRelativeSample_YearDown = -999.0;
  bos_j2_AK4_m_jesRelativeSample_YearUp = -999.0;
  bos_j2_AK4_m_jesRelativeSample_YearDown = -999.0;
  bos_j2_AK4_pt_jesTotalUp = -999.0;
  bos_j2_AK4_pt_jesTotalDown = -999.0;
  bos_j2_AK4_m_jesTotalUp = -999.0;
  bos_j2_AK4_m_jesTotalDown = -999.0;

  //Boson dijet object
  bos_AK4AK4_pt = -999.0;
  bos_AK4AK4_eta = -999.0;
  bos_AK4AK4_phi = -999.0;
  bos_AK4AK4_m = -999.0;

  //Boson dijet variations
  bos_AK4AK4_pt_jesFlavorQCDUp = -999.0;
  bos_AK4AK4_pt_jesFlavorQCDDown = -999.0;
  bos_AK4AK4_m_jesFlavorQCDUp = -999.0;
  bos_AK4AK4_m_jesFlavorQCDDown = -999.0;
  bos_AK4AK4_pt_jesRelativeBalUp = -999.0;
  bos_AK4AK4_pt_jesRelativeBalDown = -999.0;
  bos_AK4AK4_m_jesRelativeBalUp = -999.0;
  bos_AK4AK4_m_jesRelativeBalDown = -999.0;
  bos_AK4AK4_pt_jesHFUp = -999.0;
  bos_AK4AK4_pt_jesHFDown = -999.0;
  bos_AK4AK4_m_jesHFUp = -999.0;
  bos_AK4AK4_m_jesHFDown = -999.0;
  bos_AK4AK4_pt_jesBBEC1Up = -999.0;
  bos_AK4AK4_pt_jesBBEC1Down = -999.0;
  bos_AK4AK4_m_jesBBEC1Up = -999.0;
  bos_AK4AK4_m_jesBBEC1Down = -999.0;
  bos_AK4AK4_pt_jesEC2Up = -999.0;
  bos_AK4AK4_pt_jesEC2Down = -999.0;
  bos_AK4AK4_m_jesEC2Up = -999.0;
  bos_AK4AK4_m_jesEC2Down = -999.0;
  bos_AK4AK4_pt_jesAbsoluteUp = -999.0;
  bos_AK4AK4_pt_jesAbsoluteDown = -999.0;
  bos_AK4AK4_m_jesAbsoluteUp = -999.0;
  bos_AK4AK4_m_jesAbsoluteDown = -999.0;
  bos_AK4AK4_pt_jesBBEC1_YearUp = -999.0;
  bos_AK4AK4_pt_jesBBEC1_YearDown = -999.0;
  bos_AK4AK4_m_jesBBEC1_YearUp = -999.0;
  bos_AK4AK4_m_jesBBEC1_YearDown = -999.0;
  bos_AK4AK4_pt_jesEC2_YearUp = -999.0;
  bos_AK4AK4_pt_jesEC2_YearDown = -999.0;
  bos_AK4AK4_m_jesEC2_YearUp = -999.0;
  bos_AK4AK4_m_jesEC2_YearDown = -999.0;
  bos_AK4AK4_pt_jesAbsolute_YearUp = -999.0;
  bos_AK4AK4_pt_jesAbsolute_YearDown = -999.0;
  bos_AK4AK4_m_jesAbsolute_YearUp = -999.0;
  bos_AK4AK4_m_jesAbsolute_YearDown = -999.0;
  bos_AK4AK4_pt_jesHF_YearUp = -999.0;
  bos_AK4AK4_pt_jesHF_YearDown = -999.0;
  bos_AK4AK4_m_jesHF_YearUp = -999.0;
  bos_AK4AK4_m_jesHF_YearDown = -999.0;
  bos_AK4AK4_pt_jesRelativeSample_YearUp = -999.0;
  bos_AK4AK4_pt_jesRelativeSample_YearDown = -999.0;
  bos_AK4AK4_m_jesRelativeSample_YearUp = -999.0;
  bos_AK4AK4_m_jesRelativeSample_YearDown = -999.0;
  bos_AK4AK4_pt_jesTotalUp = -999.0;
  bos_AK4AK4_pt_jesTotalDown = -999.0;
  bos_AK4AK4_m_jesTotalUp = -999.0;
  bos_AK4AK4_m_jesTotalDown = -999.0;

  dibos_m = -999.0;
  dibos_mt = -999.0;
  dibos_pt = -999.0;
  dibos_eta = -999.0;
  dibos_phi = -999.0;

  dibos_m_scaleUp = -999.0;
  dibos_m_scaleDown = -999.0;
  dibos_mt_scaleUp = -999.0;
  dibos_mt_scaleDown = -999.0;
  dibos_pt_scaleUp = -999.0;
  dibos_pt_scaleDown = -999.0;

  dibos_m_jesFlavorQCDUp  = -999.0;
  dibos_m_jesFlavorQCDDown  = -999.0;
  dibos_mt_jesFlavorQCDUp  = -999.0;
  dibos_mt_jesFlavorQCDDown  = -999.0;
  dibos_pt_jesFlavorQCDUp  = -999.0;
  dibos_pt_jesFlavorQCDDown  = -999.0;
  dibos_m_jesRelativeBalUp  = -999.0;
  dibos_m_jesRelativeBalDown  = -999.0;
  dibos_mt_jesRelativeBalUp  = -999.0;
  dibos_mt_jesRelativeBalDown  = -999.0;
  dibos_pt_jesRelativeBalUp  = -999.0;
  dibos_pt_jesRelativeBalDown  = -999.0;
  dibos_m_jesHFUp  = -999.0;
  dibos_m_jesHFDown  = -999.0;
  dibos_mt_jesHFUp  = -999.0;
  dibos_mt_jesHFDown  = -999.0;
  dibos_pt_jesHFUp  = -999.0;
  dibos_pt_jesHFDown  = -999.0;
  dibos_m_jesBBEC1Up  = -999.0;
  dibos_m_jesBBEC1Down  = -999.0;
  dibos_mt_jesBBEC1Up  = -999.0;
  dibos_mt_jesBBEC1Down  = -999.0;
  dibos_pt_jesBBEC1Up  = -999.0;
  dibos_pt_jesBBEC1Down  = -999.0;
  dibos_m_jesEC2Up  = -999.0;
  dibos_m_jesEC2Down  = -999.0;
  dibos_mt_jesEC2Up  = -999.0;
  dibos_mt_jesEC2Down  = -999.0;
  dibos_pt_jesEC2Up  = -999.0;
  dibos_pt_jesEC2Down  = -999.0;
  dibos_m_jesAbsoluteUp  = -999.0;
  dibos_m_jesAbsoluteDown  = -999.0;
  dibos_mt_jesAbsoluteUp  = -999.0;
  dibos_mt_jesAbsoluteDown  = -999.0;
  dibos_pt_jesAbsoluteUp  = -999.0;
  dibos_pt_jesAbsoluteDown  = -999.0;
  dibos_m_jesBBEC1_YearUp  = -999.0;
  dibos_m_jesBBEC1_YearDown  = -999.0;
  dibos_mt_jesBBEC1_YearUp  = -999.0;
  dibos_mt_jesBBEC1_YearDown  = -999.0;
  dibos_pt_jesBBEC1_YearUp  = -999.0;
  dibos_pt_jesBBEC1_YearDown  = -999.0;
  dibos_m_jesEC2_YearUp  = -999.0;
  dibos_m_jesEC2_YearDown  = -999.0;
  dibos_mt_jesEC2_YearUp  = -999.0;
  dibos_mt_jesEC2_YearDown  = -999.0;
  dibos_pt_jesEC2_YearUp  = -999.0;
  dibos_pt_jesEC2_YearDown  = -999.0;
  dibos_m_jesAbsolute_YearUp  = -999.0;
  dibos_m_jesAbsolute_YearDown  = -999.0;
  dibos_mt_jesAbsolute_YearUp  = -999.0;
  dibos_mt_jesAbsolute_YearDown  = -999.0;
  dibos_pt_jesAbsolute_YearUp  = -999.0;
  dibos_pt_jesAbsolute_YearDown  = -999.0;
  dibos_m_jesHF_YearUp  = -999.0;
  dibos_m_jesHF_YearDown  = -999.0;
  dibos_mt_jesHF_YearUp  = -999.0;
  dibos_mt_jesHF_YearDown  = -999.0;
  dibos_pt_jesHF_YearUp  = -999.0;
  dibos_pt_jesHF_YearDown  = -999.0;
  dibos_m_jesRelativeSample_YearUp  = -999.0;
  dibos_m_jesRelativeSample_YearDown  = -999.0;
  dibos_mt_jesRelativeSample_YearUp  = -999.0;
  dibos_mt_jesRelativeSample_YearDown  = -999.0;
  dibos_pt_jesRelativeSample_YearUp  = -999.0;
  dibos_pt_jesRelativeSample_YearDown  = -999.0;
  dibos_m_jesTotalUp  = -999.0;
  dibos_m_jesTotalDown  = -999.0;
  dibos_mt_jesTotalUp  = -999.0;
  dibos_mt_jesTotalDown  = -999.0;
  dibos_pt_jesTotalUp  = -999.0;
  dibos_pt_jesTotalDown  = -999.0;

};

