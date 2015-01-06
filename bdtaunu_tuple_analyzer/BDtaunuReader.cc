#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cassert>

#include "BDtaunuDef.h"
#include "BDtaunuUtils.h"
#include "RootReader.h"
#include "BDtaunuReader.h"
#include "UpsilonCandidate.h"
#include "RecoGraphManager.h"

// Lund to particle name map needed for printing.
const std::map<int, std::string> BDtaunuReader::lund_to_name = bdtaunu::LundToNameMap();

// The maximum number of candidates allowed in an event. This should
// be consistent with the number set in BtaTupleMaker. 
const int BDtaunuReader::maximum_Y_candidates = 800;
const int BDtaunuReader::maximum_B_candidates = 400;
const int BDtaunuReader::maximum_D_candidates = 200;
const int BDtaunuReader::maximum_C_candidates = 100;
const int BDtaunuReader::maximum_h_candidates = 100;
const int BDtaunuReader::maximum_l_candidates = 100;
const int BDtaunuReader::maximum_gamma_candidates = 100;

// The constructor just needs to allocate and initialize the buffer 
// and the reco graph manager.
BDtaunuReader::BDtaunuReader(
    const char *root_fname, 
    const char *root_trname) : RootReader(root_fname, root_trname) {
  AllocateBuffer();
  ClearBuffer();
  reco_graph_manager = RecoGraphManager(this);
}

BDtaunuReader::~BDtaunuReader() {
  DeleteBuffer();
}


// Initializes the input buffer.
void BDtaunuReader::AllocateBuffer() {

  // Allocate space to read in arrays from ntuples. 
  YBPairMmissPrime2 = new float[maximum_Y_candidates];
  YBPairEextra50 = new float[maximum_Y_candidates];
  YTagBlP3MagCM = new float[maximum_Y_candidates];
  YSigBhP3MagCM = new float[maximum_Y_candidates];
  YTagBCosBY = new float[maximum_Y_candidates];
  YSigBCosBY = new float[maximum_Y_candidates];
  YTagBCosThetaDlCM = new float[maximum_Y_candidates];
  YSigBCosThetaDtauCM = new float[maximum_Y_candidates];
  YSigBVtxProbB = new float[maximum_Y_candidates];
  YBPairCosThetaT = new float[maximum_Y_candidates];
  YTagBDMass = new float[maximum_Y_candidates];
  YTagBDstarDeltaM = new float[maximum_Y_candidates];
  YTagBCosThetaDSoftCM = new float[maximum_Y_candidates];
  YTagBsoftP3MagCM = new float[maximum_Y_candidates];
  YSigBDMass = new float[maximum_Y_candidates];
  YSigBDstarDeltaM = new float[maximum_Y_candidates];
  YSigBCosThetaDSoftCM = new float[maximum_Y_candidates];
  YSigBsoftP3MagCM = new float[maximum_Y_candidates];
  YSigBhMass = new float[maximum_Y_candidates];
  YSigBVtxProbh = new float[maximum_Y_candidates];

  lTrkIdx = new int[maximum_l_candidates];
  hTrkIdx = new int[maximum_h_candidates];
  eSelectorsMap = new int[maximum_h_candidates + maximum_l_candidates];
  muSelectorsMap = new int[maximum_h_candidates + maximum_l_candidates];
  KSelectorsMap = new int[maximum_h_candidates + maximum_l_candidates];
  piSelectorsMap = new int[maximum_h_candidates + maximum_l_candidates];

  YLund = new int[maximum_Y_candidates];
  BLund = new int[maximum_B_candidates];
  DLund = new int[maximum_D_candidates];
  CLund = new int[maximum_C_candidates];
  hLund = new int[maximum_h_candidates];
  lLund = new int[maximum_l_candidates];
  gammaLund = new int[maximum_gamma_candidates];

  Yd1Idx = new int[maximum_Y_candidates];
  Yd2Idx = new int[maximum_Y_candidates];
  Bd1Idx = new int[maximum_B_candidates];
  Bd2Idx = new int[maximum_B_candidates];
  Bd3Idx = new int[maximum_B_candidates];
  Bd4Idx = new int[maximum_B_candidates];
  Dd1Idx = new int[maximum_D_candidates];
  Dd2Idx = new int[maximum_D_candidates];
  Dd3Idx = new int[maximum_D_candidates];
  Dd4Idx = new int[maximum_D_candidates];
  Dd5Idx = new int[maximum_D_candidates];
  Cd1Idx = new int[maximum_C_candidates];
  Cd2Idx = new int[maximum_C_candidates];
  hd1Idx = new int[maximum_h_candidates];
  hd2Idx = new int[maximum_h_candidates];
  ld1Idx = new int[maximum_l_candidates];
  ld2Idx = new int[maximum_l_candidates];
  ld3Idx = new int[maximum_l_candidates];
  Yd1Lund = new int[maximum_Y_candidates];
  Yd2Lund = new int[maximum_Y_candidates];
  Bd1Lund = new int[maximum_B_candidates];
  Bd2Lund = new int[maximum_B_candidates];
  Bd3Lund = new int[maximum_B_candidates];
  Bd4Lund = new int[maximum_B_candidates];
  Dd1Lund = new int[maximum_D_candidates];
  Dd2Lund = new int[maximum_D_candidates];
  Dd3Lund = new int[maximum_D_candidates];
  Dd4Lund = new int[maximum_D_candidates];
  Dd5Lund = new int[maximum_D_candidates];
  Cd1Lund = new int[maximum_C_candidates];
  Cd2Lund = new int[maximum_C_candidates];
  hd1Lund = new int[maximum_h_candidates];
  hd2Lund = new int[maximum_h_candidates];
  ld1Lund = new int[maximum_l_candidates];
  ld2Lund = new int[maximum_l_candidates];
  ld3Lund = new int[maximum_l_candidates];

  // Specify the variables where each ntuple branch should be read into. 
  tr->SetBranchAddress("platform", &platform);
  tr->SetBranchAddress("partition", &partition);
  tr->SetBranchAddress("upperID", &upperID);
  tr->SetBranchAddress("lowerID", &lowerID);
  tr->SetBranchAddress("nTRK", &nTrk);
  tr->SetBranchAddress("R2All", &R2All);

  tr->SetBranchAddress("YBPairMmissPrime2", YBPairMmissPrime2);
  tr->SetBranchAddress("YBPairEextra50", YBPairEextra50);
  tr->SetBranchAddress("YTagBlP3MagCM", YTagBlP3MagCM);
  tr->SetBranchAddress("YSigBhP3MagCM", YSigBhP3MagCM);
  tr->SetBranchAddress("YTagBCosBY", YTagBCosBY);
  tr->SetBranchAddress("YSigBCosBY", YSigBCosBY);
  tr->SetBranchAddress("YTagBCosThetaDlCM", YTagBCosThetaDlCM);
  tr->SetBranchAddress("YSigBCosThetaDtauCM", YSigBCosThetaDtauCM);
  tr->SetBranchAddress("YSigBVtxProbB", YSigBVtxProbB);
  tr->SetBranchAddress("YBPairCosThetaT", YBPairCosThetaT);
  tr->SetBranchAddress("YTagBDMass", YTagBDMass);
  tr->SetBranchAddress("YTagBDstarDeltaM", YTagBDstarDeltaM);
  tr->SetBranchAddress("YTagBCosThetaDSoftCM", YTagBCosThetaDSoftCM);
  tr->SetBranchAddress("YTagBsoftP3MagCM", YTagBsoftP3MagCM);
  tr->SetBranchAddress("YSigBDMass", YSigBDMass);
  tr->SetBranchAddress("YSigBDstarDeltaM", YSigBDstarDeltaM);
  tr->SetBranchAddress("YSigBCosThetaDSoftCM", YSigBCosThetaDSoftCM);
  tr->SetBranchAddress("YSigBsoftP3MagCM", YSigBsoftP3MagCM);
  tr->SetBranchAddress("YSigBhMass", YSigBhMass);
  tr->SetBranchAddress("YSigBVtxProbh", YSigBVtxProbh);

  tr->SetBranchAddress("lTrkIdx", lTrkIdx);
  tr->SetBranchAddress("hTrkIdx", hTrkIdx);
  tr->SetBranchAddress("eSelectorsMap", eSelectorsMap);
  tr->SetBranchAddress("muSelectorsMap", muSelectorsMap);
  tr->SetBranchAddress("KSelectorsMap", KSelectorsMap);
  tr->SetBranchAddress("piSelectorsMap", piSelectorsMap);

  tr->SetBranchAddress("nY", &nY);
  tr->SetBranchAddress("nB", &nB);
  tr->SetBranchAddress("nD", &nD);
  tr->SetBranchAddress("nC", &nC);
  tr->SetBranchAddress("nh", &nh);
  tr->SetBranchAddress("nl", &nl);
  tr->SetBranchAddress("ngamma", &ngamma); 
  tr->SetBranchAddress("YLund", YLund);
  tr->SetBranchAddress("BLund", BLund);
  tr->SetBranchAddress("DLund", DLund);
  tr->SetBranchAddress("CLund", CLund);
  tr->SetBranchAddress("hLund", hLund);
  tr->SetBranchAddress("lLund", lLund);
  tr->SetBranchAddress("gammaLund", gammaLund);
  tr->SetBranchAddress("Yd1Idx", Yd1Idx);
  tr->SetBranchAddress("Yd2Idx", Yd2Idx);
  tr->SetBranchAddress("Bd1Idx", Bd1Idx);
  tr->SetBranchAddress("Bd2Idx", Bd2Idx);
  tr->SetBranchAddress("Bd3Idx", Bd3Idx);
  tr->SetBranchAddress("Bd4Idx", Bd4Idx);
  tr->SetBranchAddress("Dd1Idx", Dd1Idx);
  tr->SetBranchAddress("Dd2Idx", Dd2Idx);
  tr->SetBranchAddress("Dd3Idx", Dd3Idx);
  tr->SetBranchAddress("Dd4Idx", Dd4Idx);
  tr->SetBranchAddress("Dd5Idx", Dd5Idx);
  tr->SetBranchAddress("Cd1Idx", Cd1Idx);
  tr->SetBranchAddress("Cd2Idx", Cd2Idx);
  tr->SetBranchAddress("hd1Idx", hd1Idx);
  tr->SetBranchAddress("hd2Idx", hd2Idx);
  tr->SetBranchAddress("ld1Idx", ld1Idx);
  tr->SetBranchAddress("ld2Idx", ld2Idx);
  tr->SetBranchAddress("ld3Idx", ld3Idx);
  tr->SetBranchAddress("Yd1Lund", Yd1Lund);
  tr->SetBranchAddress("Yd2Lund", Yd2Lund);
  tr->SetBranchAddress("Bd1Lund", Bd1Lund);
  tr->SetBranchAddress("Bd2Lund", Bd2Lund);
  tr->SetBranchAddress("Bd3Lund", Bd3Lund);
  tr->SetBranchAddress("Bd4Lund", Bd4Lund);
  tr->SetBranchAddress("Dd1Lund", Dd1Lund);
  tr->SetBranchAddress("Dd2Lund", Dd2Lund);
  tr->SetBranchAddress("Dd3Lund", Dd3Lund);
  tr->SetBranchAddress("Dd4Lund", Dd4Lund);
  tr->SetBranchAddress("Dd5Lund", Dd5Lund);
  tr->SetBranchAddress("Cd1Lund", Cd1Lund);
  tr->SetBranchAddress("Cd2Lund", Cd2Lund);
  tr->SetBranchAddress("hd1Lund", hd1Lund);
  tr->SetBranchAddress("hd2Lund", hd2Lund);
  tr->SetBranchAddress("ld1Lund", ld1Lund);
  tr->SetBranchAddress("ld2Lund", ld2Lund);
  tr->SetBranchAddress("ld3Lund", ld3Lund);

}

// Zeros out buffer elements
void BDtaunuReader::ClearBuffer() {
  platform = -999;
  partition = -999;
  upperID = -999;
  lowerID = -999;
  nTrk = -999;
  R2All = -999;
  nY = -999;
  nB = -999;
  nD = -999;
  nC = -999;
  nh = -999;
  nl = -999;
  ngamma = -999;
  upsilon_candidates.clear();
}

// Free the buffer. Used for destructor. 
void BDtaunuReader::DeleteBuffer() {

  delete[] YBPairMmissPrime2;
  delete[] YBPairEextra50;
  delete[] YTagBlP3MagCM;
  delete[] YSigBhP3MagCM;
  delete[] YTagBCosBY;
  delete[] YSigBCosBY;
  delete[] YTagBCosThetaDlCM;
  delete[] YSigBCosThetaDtauCM;
  delete[] YSigBVtxProbB;
  delete[] YBPairCosThetaT;
  delete[] YTagBDMass;
  delete[] YTagBDstarDeltaM;
  delete[] YTagBCosThetaDSoftCM;
  delete[] YTagBsoftP3MagCM;
  delete[] YSigBDMass;
  delete[] YSigBDstarDeltaM;
  delete[] YSigBCosThetaDSoftCM;
  delete[] YSigBsoftP3MagCM;
  delete[] YSigBhMass;
  delete[] YSigBVtxProbh;

  delete[] lTrkIdx;
  delete[] hTrkIdx;
  delete[] eSelectorsMap;
  delete[] muSelectorsMap;
  delete[] KSelectorsMap;
  delete[] piSelectorsMap;

  delete[] YLund;
  delete[] BLund;
  delete[] DLund;
  delete[] CLund;
  delete[] hLund;
  delete[] lLund;
  delete[] gammaLund;

  delete[] Yd1Idx;
  delete[] Yd2Idx;
  delete[] Bd1Idx;
  delete[] Bd2Idx;
  delete[] Bd3Idx;
  delete[] Bd4Idx;
  delete[] Dd1Idx;
  delete[] Dd2Idx;
  delete[] Dd3Idx;
  delete[] Dd4Idx;
  delete[] Dd5Idx;
  delete[] Cd1Idx;
  delete[] Cd2Idx;
  delete[] hd1Idx;
  delete[] hd2Idx;
  delete[] ld1Idx;
  delete[] ld2Idx;
  delete[] ld3Idx;
  delete[] Yd1Lund;
  delete[] Yd2Lund;
  delete[] Bd1Lund;
  delete[] Bd2Lund;
  delete[] Bd3Lund;
  delete[] Bd4Lund;
  delete[] Dd1Lund;
  delete[] Dd2Lund;
  delete[] Dd3Lund;
  delete[] Dd4Lund;
  delete[] Dd5Lund;
  delete[] Cd1Lund;
  delete[] Cd2Lund;
  delete[] hd1Lund;
  delete[] hd2Lund;
  delete[] ld1Lund;
  delete[] ld2Lund;
  delete[] ld3Lund;

}

// Read in the next event in the ntuple and update the buffer
// with the new information.
RootReader::Status BDtaunuReader::next_record() {

  ClearBuffer();

  // Read next event into the buffer. Implicitly uses 
  // TTree::GetEntry() method.
  RootReader::Status reader_status = RootReader::next_record();

  // Derive additional reco information from the ntuple. 
  if (reader_status == RootReader::Status::kReadSucceeded) {

    // This check is necessary since BtaTupleMaker does not save 
    // this kind of event correctly. Our solution is to skip this
    // kind of event altogether. 
    if (is_max_reco_exceeded()) {
      reader_status = RootReader::Status::kMaxRecoCandExceeded;
    } else {

      // Outsource graph operations to graph manager
      reco_graph_manager.construct_graph();
      reco_graph_manager.analyze_graph();

      // Make derived information ready for access
      FillRecoInfo();
    }
  } 
  
  return reader_status;
}

bool BDtaunuReader::is_max_reco_exceeded() const {
    if ( 
        (nY < maximum_Y_candidates) &&
        (nB < maximum_B_candidates) &&
        (nD < maximum_D_candidates) &&
        (nC < maximum_C_candidates) &&
        (nh < maximum_h_candidates) &&
        (nl < maximum_l_candidates) &&
        (ngamma < maximum_gamma_candidates) 
       ) {
      return false;
    } else {
      return true;
    }
}

std::string BDtaunuReader::get_eventId() const { 
  return std::to_string(platform) 
         + ":" + std::to_string(partition) 
         + ":" + std::to_string(upperID) 
         + "/" + std::to_string(lowerID);
}

void BDtaunuReader::FillRecoInfo() {

  // Derived information about Upsilon candidates. 
  for (int i = 0; i < nY; i++) {

    UpsilonCandidate ups;

    ups.set_eventId(get_eventId());
    ups.set_block_index(i);
    ups.set_reco_index(reco_graph_manager.get_reco_indexer().get_reco_idx(bdtaunu::UpsilonLund, i));
    ups.set_bflavor(reco_graph_manager.get_recoY(i)->tagB->flavor);
    ups.set_eextra50(YBPairEextra50[i]);
    ups.set_mmiss_prime2(YBPairMmissPrime2[i]);
    ups.set_cosThetaT(YBPairCosThetaT[i]);
    ups.set_tag_lp3(YTagBlP3MagCM[i]);
    ups.set_tag_cosBY(YTagBCosBY[i]);
    ups.set_tag_cosThetaDl(YTagBCosThetaDlCM[i]);
    ups.set_tag_Dmass(YTagBDMass[i]);
    ups.set_tag_deltaM(YTagBDstarDeltaM[i]);
    ups.set_tag_cosThetaDSoft(YTagBCosThetaDSoftCM[i]);
    ups.set_tag_softP3MagCM(YTagBsoftP3MagCM[i]);
    ups.set_tag_d_mode(reco_graph_manager.get_recoY(i)->tagB->d->D_mode);
    ups.set_tag_dstar_mode(reco_graph_manager.get_recoY(i)->tagB->d->Dstar_mode);
    ups.set_l_ePidMap(eSelectorsMap[lTrkIdx[reco_graph_manager.get_recoY(i)->tagB->lepton->l_block_idx]]);
    ups.set_l_muPidMap(muSelectorsMap[lTrkIdx[reco_graph_manager.get_recoY(i)->tagB->lepton->l_block_idx]]);
    ups.set_sig_hp3(YSigBhP3MagCM[i]);
    ups.set_sig_cosBY(YSigBCosBY[i]);
    ups.set_sig_cosThetaDtau(YSigBCosThetaDtauCM[i]);
    ups.set_sig_vtxB(YSigBVtxProbB[i]);
    ups.set_sig_Dmass(YSigBDMass[i]);
    ups.set_sig_deltaM(YSigBDstarDeltaM[i]);
    ups.set_sig_cosThetaDSoft(YSigBCosThetaDSoftCM[i]);
    ups.set_sig_softP3MagCM(YSigBsoftP3MagCM[i]);
    ups.set_sig_hmass(YSigBhMass[i]);
    ups.set_sig_vtxh(YSigBVtxProbh[i]);
    ups.set_sig_d_mode(reco_graph_manager.get_recoY(i)->sigB->d->D_mode);
    ups.set_sig_dstar_mode(reco_graph_manager.get_recoY(i)->sigB->d->Dstar_mode);
    ups.set_sig_tau_mode(reco_graph_manager.get_recoY(i)->sigB->lepton->tau_mode);
    ups.set_h_ePidMap(eSelectorsMap[hTrkIdx[reco_graph_manager.get_recoY(i)->sigB->lepton->pi_block_idx]]);
    ups.set_h_muPidMap(muSelectorsMap[hTrkIdx[reco_graph_manager.get_recoY(i)->sigB->lepton->pi_block_idx]]);

    upsilon_candidates.push_back(ups);
  }
}
