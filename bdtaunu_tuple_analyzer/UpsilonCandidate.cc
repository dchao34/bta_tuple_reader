#include <string> 
#include <cassert> 

#include "BDtaunuDef.h"
#include "UpsilonCandidate.h"

using namespace bdtaunu;

UpsilonCandidate::UpsilonCandidate() :
  eventId(""),
  block_index(-999),
  reco_index(-1),
  truth_match(-1),
  bflavor(BFlavor::null),
  eextra50(-999),
  mmiss_prime2(-999),
  cosThetaT(-999),
  tag_lp3(-999),
  tag_cosBY(-999),
  tag_cosThetaDl(-999),
  tag_Dmass(-999),
  tag_deltaM(-999),
  tag_cosThetaDSoft(-999),
  tag_softP3MagCM(-999),
  tag_d_mode(RecoDTypeCatalogue::DType::null),
  tag_dstar_mode(RecoDTypeCatalogue::DstarType::null),
  l_ePidMap(0),
  l_muPidMap(0),
  sig_hp3(-999),
  sig_cosBY(-999),
  sig_cosThetaDtau(-999),
  sig_vtxB(-999),
  sig_Dmass(-999),
  sig_deltaM(-999),
  sig_cosThetaDSoft(-999),
  sig_softP3MagCM(-999),
  sig_hmass(-999),
  sig_vtxh(-999),
  sig_d_mode(RecoDTypeCatalogue::DType::null),
  sig_dstar_mode(RecoDTypeCatalogue::DstarType::null),
  sig_tau_mode(TauType::null),
  h_ePidMap(0),
  h_muPidMap(0) {}

UpsilonCandidate::UpsilonCandidate(
  std::string &_eventId,
  int _block_index,
  int _reco_index,
  int _truth_match,
  BFlavor _bflavor,
  float _eextra50,
  float _mmiss_prime2,
  float _cosThetaT,
  float _tag_lp3,
  float _tag_cosBY,
  float _tag_cosThetaDl,
  float _tag_Dmass,
  float _tag_deltaM,
  float _tag_cosThetaDSoft,
  float _tag_softP3MagCM,
  RecoDTypeCatalogue::DType _tag_d_mode,
  RecoDTypeCatalogue::DstarType _tag_dstar_mode,
  int _l_ePidMap,
  int _l_muPidMap,
  float _sig_hp3,
  float _sig_cosBY,
  float _sig_cosThetaDtau,
  float _sig_vtxB,
  float _sig_Dmass,
  float _sig_deltaM,
  float _sig_cosThetaDSoft,
  float _sig_softP3MagCM,
  float _sig_hmass,
  float _sig_vtxh,
  RecoDTypeCatalogue::DType _sig_d_mode,
  RecoDTypeCatalogue::DstarType _sig_dstar_mode,
  bdtaunu::TauType _sig_tau_mode,
  int _h_ePidMap,
  int _h_muPidMap) : 
  eventId(_eventId),
  block_index(_block_index),
  reco_index(_reco_index),
  truth_match(_truth_match),
  bflavor(_bflavor),
  eextra50(_eextra50),
  mmiss_prime2(_mmiss_prime2),
  cosThetaT(_cosThetaT),
  tag_lp3(_tag_lp3),
  tag_cosBY(_tag_cosBY),
  tag_cosThetaDl(_tag_cosThetaDl),
  tag_Dmass(_tag_Dmass),
  tag_deltaM(_tag_deltaM),
  tag_cosThetaDSoft(_tag_cosThetaDSoft),
  tag_softP3MagCM(_tag_softP3MagCM),
  tag_d_mode(_tag_d_mode),
  tag_dstar_mode(_tag_dstar_mode),
  l_ePidMap(_l_ePidMap),
  l_muPidMap(_l_muPidMap),
  sig_hp3(_sig_hp3),
  sig_cosBY(_sig_cosBY),
  sig_cosThetaDtau(_sig_cosThetaDtau),
  sig_vtxB(_sig_vtxB),
  sig_Dmass(_sig_Dmass),
  sig_deltaM(_sig_deltaM),
  sig_cosThetaDSoft(_sig_cosThetaDSoft),
  sig_softP3MagCM(_sig_softP3MagCM),
  sig_hmass(_sig_hmass),
  sig_vtxh(_sig_vtxh),
  sig_d_mode(_sig_d_mode),
  sig_dstar_mode(_sig_dstar_mode),
  sig_tau_mode(_sig_tau_mode),
  h_ePidMap(_h_ePidMap),
  h_muPidMap(_h_muPidMap) {}

UpsilonCandidate::UpsilonCandidate(const UpsilonCandidate &cand) {
  copy_candidate(cand);
}

UpsilonCandidate & UpsilonCandidate::operator=(const UpsilonCandidate &cand) {
  if (this != &cand) {
    copy_candidate(cand);
  }
  return *this;
}

void UpsilonCandidate::copy_candidate(const UpsilonCandidate &cand) {
  eventId = cand.eventId;
  block_index = cand.block_index;
  reco_index = cand.reco_index;
  truth_match = cand.truth_match;
  bflavor = cand.bflavor;
  eextra50 = cand.eextra50;
  mmiss_prime2 = cand.mmiss_prime2;
  cosThetaT = cand.cosThetaT;
  tag_lp3 = cand.tag_lp3;
  tag_cosBY = cand.tag_cosBY;
  tag_cosThetaDl = cand.tag_cosThetaDl;
  tag_Dmass = cand.tag_Dmass;
  tag_deltaM = cand.tag_deltaM;
  tag_cosThetaDSoft = cand.tag_cosThetaDSoft;
  tag_softP3MagCM = cand.tag_softP3MagCM;
  tag_d_mode = cand.tag_d_mode;
  tag_dstar_mode = cand.tag_dstar_mode;
  l_ePidMap = cand.l_ePidMap;
  l_muPidMap = cand.l_muPidMap;
  sig_hp3 = cand.sig_hp3;
  sig_cosBY = cand.sig_cosBY;
  sig_cosThetaDtau = cand.sig_cosThetaDtau;
  sig_vtxB = cand.sig_vtxB;
  sig_Dmass = cand.sig_Dmass;
  sig_deltaM = cand.sig_deltaM;
  sig_cosThetaDSoft = cand.sig_cosThetaDSoft;
  sig_softP3MagCM = cand.sig_softP3MagCM;
  sig_hmass = cand.sig_hmass;
  sig_vtxh = cand.sig_vtxh;
  sig_d_mode = cand.sig_d_mode;
  sig_dstar_mode = cand.sig_dstar_mode;
  sig_tau_mode = cand.sig_tau_mode;
  h_ePidMap = cand.h_ePidMap;
  h_muPidMap = cand.h_muPidMap;
}

// Examine the D, D*, and tau modes to determine the candidate type. 
CandType UpsilonCandidate::get_cand_type() const {

  assert(sig_tau_mode != TauType::null);
  assert(tag_dstar_mode != RecoDTypeCatalogue::DstarType::null);
  assert(sig_dstar_mode != RecoDTypeCatalogue::DstarType::null);

  int cand_type = 0;
  if (sig_tau_mode == TauType::tau_rho)
    cand_type += (1 << 2);
  if (tag_dstar_mode != RecoDTypeCatalogue::DstarType::NoDstar)
    cand_type += (1 << 1);
  if (sig_dstar_mode != RecoDTypeCatalogue::DstarType::NoDstar)
    cand_type += 1;

  return static_cast<CandType>(cand_type);
}


// Examine the bflavor and D* decay mode on the signal to determine
// sample type. 
SampleType UpsilonCandidate::get_sample_type() const {

  assert(bflavor != BFlavor::null);
  assert(sig_dstar_mode != RecoDTypeCatalogue::DstarType::null);

  int sample_type = 0;
  if (bflavor == BFlavor::B0)
    sample_type += (1 << 1);
  if (sig_dstar_mode != RecoDTypeCatalogue::DstarType::NoDstar)
    sample_type += 1;

  return static_cast<SampleType>(sample_type);
}
