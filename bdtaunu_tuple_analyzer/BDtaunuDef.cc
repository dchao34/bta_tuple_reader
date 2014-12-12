#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cassert>

#include "BDtaunuDef.h"

using namespace bdtaunu;

// RecoDType
// ---------

RecoDTypeCatalogue::DType 
RecoDTypeCatalogue::search_d_catalogue(std::vector<int> lund_list) const {
  std::vector<Alphabet> word;
  for (auto l : lund_list) word.push_back(LundToAlphabet(l));
  std::sort(word.begin(), word.end());
  word.push_back(Alphabet::null);
  return d_catalogue.find(word);
}

RecoDTypeCatalogue::DstarType 
RecoDTypeCatalogue::search_dstar_catalogue(std::vector<int> lund_list) const {
  std::vector<Alphabet> word;
  for (auto l : lund_list) word.push_back(LundToAlphabet(l));
  std::sort(word.begin(), word.end());
  word.push_back(Alphabet::null);
  return dstar_catalogue.find(word);
}


void RecoDTypeCatalogue::RegisterDecays() {

  // D catalogue
  d_catalogue.insert({
      Alphabet::Dc, Alphabet::K, Alphabet::pi, Alphabet::pi, 
      Alphabet::null
    }, DType::Dc_Kpipi);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::K, Alphabet::pi, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, DType::Dc_Kpipipi0);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::K,
      Alphabet::null
    }, DType::Dc_KsK);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::pi,
      Alphabet::null
    }, DType::Dc_Kspi);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, DType::Dc_Kspipi0);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::pi, Alphabet::pi, Alphabet::pi,
      Alphabet::null
    }, DType::Dc_Kspipipi);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::K, Alphabet::K, Alphabet::pi,
      Alphabet::null
    }, DType::Dc_KKpi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi,
      Alphabet::null
    }, DType::D0_Kpi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, DType::D0_Kpipi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi, Alphabet::pi, Alphabet::pi,
      Alphabet::null
    }, DType::D0_Kpipipi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi, Alphabet::pi, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, DType::D0_Kpipipipi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::Ks, Alphabet::pi, Alphabet::pi,
      Alphabet::null
    }, DType::D0_Kspipi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::Ks, Alphabet::pi, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, DType::D0_Kspipipi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::Ks, Alphabet::pi0,
      Alphabet::null
    }, DType::D0_Kspi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::K,
      Alphabet::null
    }, DType::D0_KK);

  // Dstar catalogue
  dstar_catalogue.insert({
      Alphabet::Dstar0, Alphabet::D0, Alphabet::pi0,
      Alphabet::null
    }, DstarType::Dstar0_D0pi0);

  dstar_catalogue.insert({
      Alphabet::Dstar0, Alphabet::D0, Alphabet::gamma,
      Alphabet::null
    }, DstarType::Dstar0_D0gamma);

  dstar_catalogue.insert({
      Alphabet::Dstarc, Alphabet::D0, Alphabet::pi,
      Alphabet::null
    }, DstarType::Dstarc_D0pi);

  dstar_catalogue.insert({
      Alphabet::Dstarc, Alphabet::Dc, Alphabet::pi0,
      Alphabet::null
    }, DstarType::Dstarc_Dcpi0);

  dstar_catalogue.insert({
      Alphabet::Dstarc, Alphabet::Dc, Alphabet::gamma,
      Alphabet::null
    }, DstarType::Dstarc_Dcgamma);

}

RecoDTypeCatalogue::Alphabet RecoDTypeCatalogue::LundToAlphabet(int lund) const {
  switch (abs(lund)) {
    case bdtaunu::DstarcLund:
      return Alphabet::Dstarc;
      break;
    case bdtaunu::Dstar0Lund:
      return Alphabet::Dstar0;
      break;
    case bdtaunu::DcLund:
      return Alphabet::Dc;
      break;
    case bdtaunu::D0Lund:
      return Alphabet::D0;
      break;
    case bdtaunu::KLund:
      return Alphabet::K;
      break;
    case bdtaunu::KSLund:
      return Alphabet::Ks;
      break;
    case bdtaunu::piLund:
      return Alphabet::pi;
      break;
    case bdtaunu::pi0Lund:
      return Alphabet::pi0;
      break;
    case bdtaunu::gammaLund:
      return Alphabet::gamma;
      break;
    default:
      assert(false);
      return Alphabet::null;
  }
}


// McBType
// -------

McBTypeCatalogue::BMcType 
McBTypeCatalogue::search_catalogue(std::vector<int> lund_list) const {
  std::vector<Alphabet> word;
  bool hasX = false;
  for (auto l : lund_list) {
    Alphabet a = LundToAlphabet(l);
    if (a == Alphabet::I) {
      continue;
    } else if (a == Alphabet::X) {
      hasX = true;
    } else {
      word.push_back(a);
    }
  }

  std::sort(word.begin(), word.end());
  if (hasX) word.push_back(Alphabet::X);
  word.push_back(Alphabet::null);

  return catalogue.find(word);
}

void McBTypeCatalogue::RegisterDecays() {

  // nu_tau branch
  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, 
      Alphabet::null
    }, BMcType::SL);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, Alphabet::D,
      Alphabet::null
    }, BMcType::Dtau);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, Alphabet::Dstar,
      Alphabet::null
    }, BMcType::Dstartau);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, Alphabet::Dstarstar,
      Alphabet::null
    }, BMcType::Dstarstar_res);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, Alphabet::X,
      Alphabet::null
    }, BMcType::SL);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, 
      Alphabet::D, Alphabet::X,
      Alphabet::null
    }, BMcType::Dstarstar_nonres);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, 
      Alphabet::Dstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Dstarstar_nonres);

  catalogue.insert({
      Alphabet::nu_tau, Alphabet::tau, 
      Alphabet::Dstarstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Dstarstar_res);

  // nu_ell branch
  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, 
      Alphabet::null
    }, BMcType::SL);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, Alphabet::D,
      Alphabet::null
    }, BMcType::Dl);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, Alphabet::Dstar,
      Alphabet::null
    }, BMcType::Dstarl);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, Alphabet::Dstarstar,
      Alphabet::null
    }, BMcType::Dstarstar_res);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, Alphabet::X,
      Alphabet::null
    }, BMcType::SL);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, 
      Alphabet::D, Alphabet::X,
      Alphabet::null
    }, BMcType::Dstarstar_nonres);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, 
      Alphabet::Dstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Dstarstar_nonres);

  catalogue.insert({
      Alphabet::nu_ell, Alphabet::ell, 
      Alphabet::Dstarstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Dstarstar_res);

  // hadron branch
  catalogue.insert({
      Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::X, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstar, Alphabet::X, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstarstar, Alphabet::X, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::D, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::Dstar, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::Dstarstar, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstar, Alphabet::Dstar, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstar, Alphabet::Dstarstar, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstarstar, Alphabet::Dstarstar, 
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::D, Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::Dstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::D, Alphabet::Dstarstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstar, Alphabet::Dstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstar, Alphabet::Dstarstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

  catalogue.insert({
      Alphabet::Dstarstar, Alphabet::Dstarstar, Alphabet::X,
      Alphabet::null
    }, BMcType::Had);

}

McBTypeCatalogue::Alphabet McBTypeCatalogue::LundToAlphabet(int lund) const {
  switch (std::abs(lund)) {
    case 12: // nu_e
    case 14: // nu_mu
      return Alphabet::nu_ell;
      break;
    case 16: // nu_tau
      return Alphabet::nu_tau;
      break;
    case 11: // e
    case 13: // mu
      return Alphabet::ell;
      break;
    case 15: // tau
      return Alphabet::tau;
      break;
    case 411: // D+
    case 421: // D0
      return Alphabet::D;
      break;
    case 413: // D*+
    case 423: // D*0
      return Alphabet::Dstar;
      break;
    case 10411: // D_0*+
    case 10421: // D_0*0
    case 10413: // D_1+
    case 10423: // D_10
    case 415: // D_2*+
    case 425: // D_2*0
    case 20413: // D'_1+
    case 20423: // D'_10
    case 30411: // D(2S)+
    case 30421: // D(2S)0
    case 30413: // D*(2S)+
    case 30423: // D*(2S)0
      return Alphabet::Dstarstar;
      break;
    case 22: 
      return Alphabet::I;
      break;
    default: 
      return Alphabet::X;
      break;
  }
}
