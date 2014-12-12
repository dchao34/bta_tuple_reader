#ifndef __BDTAUNUDEF_H__
#define __BDTAUNUDEF_H__

#include <vector>
#include <custom_cpp_utilities/trie.h>

/** @file BDtaunuDef.h
 *  @brief Definitions of relevant quantities to the \f$B\rightarrow D\tau\nu\f$ analysis. 
 *
 *  Several types of quantities are defined in this file:
 *  * Constants used throughout the analysis. 
 *  * Particle decay modes. 
 *
 */

namespace bdtaunu {

// Lund ID of frequently encountered particles.
const int UpsilonLund = 70553;    
const int B0Lund = 511;          
const int BcLund = 521;         
const int D0Lund = 421;        
const int DcLund = 411;
const int Dstar0Lund = 423;
const int DstarcLund = 413;
const int KSLund = 310;
const int K0Lund = 311;
const int rhoLund = 213;
const int pi0Lund = 111;
const int KLund = 321;
const int piLund = 211;
const int eLund = 11;
const int nu_eLund = 12;
const int muLund = 13;
const int nu_muLund = 14;
const int tauLund = 15;
const int nu_tauLund = 16;
const int gammaLund = 22;
const int protonLund = 2212;
const int neutronLund = 2112;


//! B meson flavors.
enum class BFlavor {
  NoB = 0,                /*!< No \f$B\f$ meson. */
  B0 = 1,                 /*!< \f$ B^0 \f$ */
  Bc = 2,                 /*!< \f$ B^\pm \f$ */
  null = -1,  /*!< Undefined */
};


//! \f$D/D^*\f$ modes that are reconstructed. 
class RecoDTypeCatalogue {

  public:
    enum class DType { 
      Dc_Kpipi = 1,          /*!< \f$ D^+\rightarrow K^-\pi^+\pi^- \f$ */
      Dc_Kpipipi0 = 2,       /*!< \f$ D^+\rightarrow K^-\pi^+\pi^-\pi^0 \f$ */
      Dc_KsK = 3,            /*!< \f$ D^+\rightarrow K_s K^+ \f$ */
      Dc_Kspi = 4,           /*!< \f$ D^+\rightarrow K_s\pi^+ \f$ */
      Dc_Kspipi0 = 5,        /*!< \f$ D^+\rightarrow K_s\pi^+\pi^0 \f$ */
      Dc_Kspipipi = 6,       /*!< \f$ D^+\rightarrow K_s\pi^+\pi^-\pi^+ \f$ */
      Dc_KKpi = 7,           /*!< \f$ D^+\rightarrow K^+K^-\pi^+ \f$ */
      D0_Kpi = 8,            /*!< \f$ D^0\rightarrow K^-\pi^+ \f$ */
      D0_Kpipi0 = 9,         /*!< \f$ D^0\rightarrow K^-\pi^+\pi^0 \f$ */
      D0_Kpipipi = 10,       /*!< \f$ D^0\rightarrow K^-\pi^+\pi^+\pi^- \f$ */
      D0_Kpipipipi0 = 11,    /*!< \f$ D^0\rightarrow K^-\pi^+\pi^+\pi^-\pi^0 \f$ */
      D0_Kspipi = 12,        /*!< \f$ D^0\rightarrow K_s\pi^+\pi^- \f$ */
      D0_Kspipipi0 = 13,     /*!< \f$ D^0\rightarrow K_s\pi^+\pi^-\pi^0 \f$ */
      D0_Kspi0 = 14,         /*!< \f$ D^0\rightarrow K_s\pi^0 \f$ */
      D0_KK = 15,            /*!< \f$ D^0\rightarrow K^+K^- \f$ */
      null = -1,             /*!< Undefined */
    };

    enum class DstarType {
      NoDstar = 0,                  /*!< No \f$ D^* \f$ in \f$ B \f$ decay */
      Dstar0_D0pi0 = 1,             /*!< \f$ D^{*0}\rightarrow D^0\pi^0 \f$ */
      Dstar0_D0gamma = 2,           /*!< \f$ D^{*0}\rightarrow D^0\gamma \f$ */
      Dstarc_D0pi = 3,              /*!< \f$ D^{*+}\rightarrow D^0\pi^+ \f$ */
      Dstarc_Dcpi0 = 4,             /*!< \f$ D^{*+}\rightarrow D^+\pi^0 \f$ */
      Dstarc_Dcgamma = 5,           /*!< \f$ D^{*+}\rightarrow D^+\gamma \f$ */
      null = -1,                    /*!< Undefined */
    };


    //! Given a vector of lund Id's of a \f$D\f$ and its daughters, return its decay mode or null.
    DType search_d_catalogue(std::vector<int>) const;

    //! Given a vector of lund Id's of a \f$D^*\f$ and its daughters, return its decay mode or null.
    DstarType search_dstar_catalogue(std::vector<int>) const;

    RecoDTypeCatalogue() { RegisterDecays(); }
    ~RecoDTypeCatalogue() {};

  private:
    enum class Alphabet { 
      Dstarc, Dstar0, Dc, D0, K, Ks, pi, pi0, gamma, null = -1,
    };

    void RegisterDecays();
    Alphabet LundToAlphabet(int lund) const;

    custom_cpp_utilities::trie<Alphabet, DType, Alphabet::null, DType::null> d_catalogue;
    custom_cpp_utilities::trie<Alphabet, DstarType, Alphabet::null, DstarType::null> dstar_catalogue;

};

//! \f$\tau\f$ modes that are reconstructed. 
enum class TauType {
  NoTau = 0,               /*!< \f$ \tau^+\rightarrow \pi^+ \f$ */
  tau_pi = 1,              /*!< \f$ \tau^+\rightarrow \pi^+ \f$ */
  tau_rho = 2,             /*!< \f$ \tau^+\rightarrow \rho^+ \f$ */
  tau_e = 3,               /*!< \f$ \tau^+\rightarrow \e^+ \f$ */
  tau_mu = 4,              /*!< \f$ \tau^+\rightarrow \mu^+ \f$ */
  null = -1,               /*!< Undefined */
};


//! Monte carlo truth \f$B\f$ meson types. 
class McBTypeCatalogue {

  public:

    //! See BDtaunuDef.cc for definitions. 
    enum class BMcType { 
      NoB = 0,
      Dtau = 1,
      Dstartau = 2,
      Dl = 3,
      Dstarl = 4, 
      Dstarstar_res = 5,
      Dstarstar_nonres = 6,
      SL = 7,
      Had = 8,
      null = -1,
    };

    //! Given a vector of \f$B^*\f$ daughters, return its MC type or null.
    BMcType search_catalogue(std::vector<int>) const;

    McBTypeCatalogue() { RegisterDecays(); }
    ~McBTypeCatalogue() {};

  private:
    enum class Alphabet { 
      nu_ell, nu_tau, ell, tau, 
      D, Dstar, Dstarstar, X, I, null = -1, 
    };

    void RegisterDecays();
    Alphabet LundToAlphabet(int lund) const;

    custom_cpp_utilities::trie<Alphabet, BMcType, Alphabet::null, BMcType::null> catalogue;

};



//! Truth \f$\tau\f$ MC Types. See Details. 
/*! Truth \f$\tau\f$ MC Types for truth \f$B\f$'s decaying as 
 * \f$B\rightarrow D^{(*)}\tau\nu_\tau\f$. Any other \f$B\f$ decay types
 * will have this value undefined. */
enum class TauMcType {

  //! Truth \f$\tau\f$ decays as \f$\tau\rightarrow e+\nu_e+X\f$.
  NoTau = 0, 

  //! Truth \f$\tau\f$ decays as \f$\tau\rightarrow e+\nu_e+X\f$.
  tau_e = 1, 

  //! Truth \f$\tau\f$ decays as \f$\tau\rightarrow \mu+\nu_\mu+X\f$.
  tau_mu = 2, 

  //! Truth \f$\tau\f$ decays as \f$\tau\rightarrow K+X\f$.
  tau_k = 3, 

  //! Truth \f$\tau\f$ decays as \f$\tau\rightarrow X\f$.
  /*! \f$X\f$ is any sequence of particles not involving \f$e, \mu, K\f$. */
  tau_h = 4, 

  //! Undefined \f$\tau\f$ MC type.
  null = -1, 
};

//! Candidate types.
/*! Different candidate types have different sets of features that are
 * useful in the analysis. They are distinguished based on the type of
 * D meson (D or D*) that is present on the signal and tag side, and
 * also whether the tau decays into an alleged pi or rho meson. 
 * 
 * A notable use of this distinction is in best candidate selection.
 * Each candidate type has a unique trained learner to assign a score 
 * for any candidates assigned to that category. */
enum class CandType {
  kDDpi = 0,                  /*!< \f$ B_{tag}\rightarrow D,\,B_{sig}\rightarrow D,\,\tau\rightarrow\pi \f$ */
  kDDstarpi = 1,              /*!< \f$ B_{tag}\rightarrow D,\,B_{sig}\rightarrow D^*,\,\tau\rightarrow\pi \f$ */
  kDstarDpi = 2,              /*!< \f$ B_{tag}\rightarrow D^*,\,B_{sig}\rightarrow D,\,\tau\rightarrow\pi \f$ */
  kDstarDstarpi = 3,          /*!< \f$ B_{tag}\rightarrow D^*,\,B_{sig}\rightarrow D^*,\,\tau\rightarrow\pi \f$ */
  kDDrho = 4,                 /*!< \f$ B_{tag}\rightarrow D,\,B_{sig}\rightarrow D,\,\tau\rightarrow\rho \f$ */
  kDDstarrho = 5,             /*!< \f$ B_{tag}\rightarrow D,\,B_{sig}\rightarrow D^*,\,\tau\rightarrow\rho \f$ */
  kDstarDrho = 6,             /*!< \f$ B_{tag}\rightarrow D^*,\,B_{sig}\rightarrow D,\,\tau\rightarrow\rho \f$ */
  kDstarDstarrho = 7,         /*!< \f$ B_{tag}\rightarrow D^*,\,B_{sig}\rightarrow D^*,\,\tau\rightarrow\rho \f$ */
  null = -1,                  /*!< Undefined */
};

//! Sample types. 
/*! This is an attribute attached to \f$\Upsilon(4S)\f$ candidates. It
 * is the sample type that the event would be assigned to if this
 * candidate were to be chosen as the best candidate to represent it. */
enum class SampleType {
  kBcD = 0,                     /*!< \f$ B^u \rightarrow D \f$ */
  kBcDstar = 1,                 /*!< \f$ B^u \rightarrow D^* \f$ */
  kB0D = 2,                     /*!< \f$ B^d \rightarrow D \f$ */
  kB0Dstar = 3,                 /*!< \f$ B^d \rightarrow D^* \f$ */
  null = -1,                    /*!< Undefined */
};


}

#endif
