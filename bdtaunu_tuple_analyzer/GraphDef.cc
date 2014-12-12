#include "BDtaunuDef.h"
#include "GraphDef.h"

#include <cmath>
#include <cstdlib>
#include <cassert>
#include <initializer_list>

// RecoIndexer
// -----------

RecoGraph::RecoIndexer::RecoIndexer() : 
  nY(0), nB(0), nD(0), nC(0), 
  nh(0), nl(0), ngamma(0) {};

RecoGraph::RecoIndexer::RecoIndexer(
    int _nY, int _nB, int _nD, 
    int _nC, int _nh, int _nl, int _ngamma) : 
    nY(_nY), nB(_nB), nD(_nD), nC(_nC), 
    nh(_nh), nl(_nl), ngamma(_ngamma) {};

// The reco indexing is as follows:
// Y candidates: 0, ..., nY - 1. The ith Y candidate is assigned index i. 
// B candidates: nY, ..., nY + nB - 1. The ith B candidate is assigned index nY + i.
// D candidates: ... continue pattern.
// C candidates: ... continue pattern.
// h candidates: ... continue pattern.
// gamma candidates: ... continue pattern.
int RecoGraph::RecoIndexer::get_reco_idx(int lund, int idx) const {

  int abslund = std::abs(lund);
  switch (abslund) {
    case bdtaunu::UpsilonLund:
      return idx;
    case bdtaunu::B0Lund:
    case bdtaunu::BcLund:
      return nY + idx;
    case bdtaunu::D0Lund:
    case bdtaunu::DcLund:
    case bdtaunu::Dstar0Lund:
    case bdtaunu::DstarcLund:
      return nY + nB + idx;
    case bdtaunu::KSLund:
    case bdtaunu::rhoLund:
    case bdtaunu::pi0Lund:
      return nY + nB + nD + idx;
    case bdtaunu::KLund:
    case bdtaunu::piLund:
      return nY + nB + nD + nC + idx;
    case bdtaunu::eLund:
    case bdtaunu::muLund:
      return nY + nB + nD + nC + nh + idx;
    case bdtaunu::gammaLund:
      return nY + nB + nD + nC + nh + nl + idx;
  }

  return -1;
}

bool RecoGraph::RecoIndexer::is_h_candidate(int reco_index) const {
  int h_offset = nY + nB + nD + nC;
  return ((reco_index >= h_offset) && 
          (reco_index < h_offset + nh)) ? true : false;
}

bool RecoGraph::RecoIndexer::is_l_candidate(int reco_index) const {
  int l_offset = nY + nB + nD + nC + nh;
  return ((reco_index >= l_offset) && 
          (reco_index < l_offset + nl)) ? true : false;
}

bool RecoGraph::RecoIndexer::is_gamma_candidate(int reco_index) const {
  int gamma_offset = nY + nB + nD + nC + nh + nl;
  return ((reco_index >= gamma_offset) && 
          (reco_index < gamma_offset + ngamma)) ? true : false;
}

void RecoGraph::RecoIndexer::clear() {
  nY = 0;
  nB = 0;
  nD = 0;
  nC = 0;
  nh = 0;
  nl = 0;
  ngamma = 0;
}

void RecoGraph::RecoIndexer::set(std::initializer_list<int> l) {

  assert(l.size() == 7);

  std::initializer_list<int>::iterator iter = l.begin();

  nY = *iter++;
  nB = *iter++;
  nD = *iter++;
  nC = *iter++;
  nh = *iter++;
  nl = *iter++;
  ngamma = *iter++;
}
