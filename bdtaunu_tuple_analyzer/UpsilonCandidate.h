#ifndef __UPSILONCANDIDATE__
#define __UPSILONCANDIDATE__

#include <string> 

#include "BDtaunuDef.h"

//! Class representing an \f$\Upsilon(4S)\f$ candidate
class UpsilonCandidate {

  public:
    //! Constructs candidate with non-physical attributes.
    UpsilonCandidate();

    //! Constructs candidate with specified attributes. 
    UpsilonCandidate(
      std::string &eventId,
      int block_index,
      int reco_index,
      int truth_match,
      bdtaunu::BFlavor bflavor,
      float eextra50,
      float mmiss_prime2,
      float cosThetaT,
      float tag_lp3,
      float tag_cosBY,
      float tag_cosThetaDl,
      float tag_Dmass,
      float tag_deltaM,
      float tag_cosThetaDSoft,
      float tag_softP3MagCM,
      bdtaunu::RecoDTypeCatalogue::DType tag_d_mode,
      bdtaunu::RecoDTypeCatalogue::DstarType tag_dstar_mode,
      int l_ePidMap,
      int l_muPidMap,
      float sig_hp3,
      float sig_cosBY,
      float sig_cosThetaDtau,
      float sig_vtxB,
      float sig_Dmass,
      float sig_deltaM,
      float sig_cosThetaDSoft,
      float sig_softP3MagCM,
      float sig_hmass,
      float sig_vtxh,
      bdtaunu::RecoDTypeCatalogue::DType sig_d_mode,
      bdtaunu::RecoDTypeCatalogue::DstarType sig_dstar_mode,
      bdtaunu::TauType sig_tau_mode,
      int h_ePidMap,
      int h_muPidMap);
    UpsilonCandidate(const UpsilonCandidate &cand);
    UpsilonCandidate & operator=(const UpsilonCandidate &cand);
    ~UpsilonCandidate() {};


    //! The babar ID of the event this candidate belongs to. 
    std::string get_eventId() const { return eventId; }
    void set_eventId(std::string _eventId) { eventId = _eventId; }

    //! The index that uniquely identifies the upsilon candidate within the candidate block.
    int get_block_index() const { return block_index; }
    void set_block_index(int _block_index) { block_index = _block_index; }

    //! The index that uniquely identifies the reconstructed candidate within the event.
    int get_reco_index() const { return reco_index; }
    void set_reco_index(int _reco_index) { reco_index = _reco_index; }

    //! The index that uniquely identifies the reconstructed candidate within the event.
    int get_truth_match() const { return truth_match; }
    void set_truth_match(int _truth_match) { truth_match = _truth_match; }

    //! \f$ E_{extra} \f$ 
    /*! The energy sum of photons above 50 MeV that are not used in the
     * reconstruction of this candidate. */
    float get_eextra50() const { return eextra50; }
    void set_eextra50(float _eextra50) { eextra50 = _eextra50; }

    //! \f$ M^2_{miss} \f$
    /*! This is just like the ordinary missing mass squared, except the
     * beam energy information is also incorporated to reduce the
     * uncertainty. */
    float get_mmiss_prime2() const { return mmiss_prime2; }
    void set_mmiss_prime2(float _mmiss_prime2) { mmiss_prime2 = _mmiss_prime2; }

    //! \f$ |\vec{p}_\ell| \f$ of the \f$B_{tag}\f$
    float get_tag_lp3() const { return tag_lp3; }
    void set_tag_lp3(float _tag_lp3) { tag_lp3 = _tag_lp3; }

    //! \f$ |\vec{p}_h| \f$ of the \f$B_{sig}\f$
    /*! \f$h\f$ is either \f$\pi\f$ or \f$\rho\f$. The letter "h"
     * is for \f$\tau\f$ decays to <B>h</B>adrons. It is the proxy for
     * the reconstructed \f$\tau\f$ since the neutrino is
     * undetectable. */
    float get_sig_hp3() const { return sig_hp3; }
    void set_sig_hp3(float _sig_hp3) { sig_hp3 = _sig_hp3; }

    //! \f$ \cos\theta_{BY} \f$ of the \f$B_{tag}\f$
    float get_tag_cosBY() const { return tag_cosBY; }
    void set_tag_cosBY(float _tag_cosBY) { tag_cosBY = _tag_cosBY; }

    //! \f$ \cos\theta_{BY} \f$ of the \f$B_{sig}\f$
    float get_sig_cosBY() const { return sig_cosBY; }
    void set_sig_cosBY(float _sig_cosBY) { sig_cosBY = _sig_cosBY; }

    //! \f$ \cos\theta_{D\ell} \f$ of the \f$B_{tag}\f$
    float get_tag_cosThetaDl() const { return tag_cosThetaDl; }
    void set_tag_cosThetaDl(float _tag_cosThetaDl) { tag_cosThetaDl = _tag_cosThetaDl; }

    //! \f$ \cos\theta_{D\tau} \f$ of the \f$B_{sig}\f$
    /*! This is actually \f$ \cos\theta_{D\tau} \f$ to be precise,
     * since the \f$\tau\f$ itself is of course unreconstructable. */
    float get_sig_cosThetaDtau() const { return sig_cosThetaDtau; }
    void set_sig_cosThetaDtau(float _sig_cosThetaDtau) { sig_cosThetaDtau = _sig_cosThetaDtau; }

    //! \f$B_{sig}\f$ vertex quality. 
    /*! It is really (1 - pvalue)? of the \f$\chi^2\f$ fit. */
    float get_sig_vtxB() const { return sig_vtxB; }
    void set_sig_vtxB(float _sig_vtxB) { sig_vtxB = _sig_vtxB; }

    //! \f$ \cos\theta_{T} \f$
    /*! This is the cosine of the angle that the thrust vector makes
     * with the beam axis.  */
    float get_cosThetaT() const { return cosThetaT; }
    void set_cosThetaT(float _cosThetaT) { cosThetaT = _cosThetaT; }

    //! Mass of the \f$ D \f$ meson of the \f$B_{tag}\f$.
    float get_tag_Dmass() const { return tag_Dmass; }
    void set_tag_Dmass(float _tag_Dmass) { tag_Dmass = _tag_Dmass; }

    //! \f$\Delta m\f$ of the \f$ D \f$ meson of the \f$B_{tag}\f$.
    float get_tag_deltaM() const { return tag_deltaM; }
    void set_tag_deltaM(float _tag_deltaM) { tag_deltaM = _tag_deltaM; }

    //! \f$ \cos\theta_{D soft} \f$ of the \f$B_{tag}\f$
    /*! This is the cosine of the angle between the daughters of the
     * \f$D^*\f$ decay; namely the angle between the \f$ D \f$ meson
     * and the soft daughter (photon or a \f$\pi^0\f$). */
    float get_tag_cosThetaDSoft() const { return tag_cosThetaDSoft; }
    void set_tag_cosThetaDSoft(float _tag_cosThetaDSoft) { tag_cosThetaDSoft = _tag_cosThetaDSoft; }

    //! \f$ |\vec{p}_{soft}| \f$ of the \f$B_{tag}\f$
    /*! Magnitude of the three momentum of the \f$D^*\f$ meson's soft
     * daughter in the center of mass frame.  */
    float get_tag_softP3MagCM() const { return tag_softP3MagCM; }
    void set_tag_softP3MagCM(float _tag_softP3MagCM) { tag_softP3MagCM = _tag_softP3MagCM; }

    //! Mass of the \f$ D \f$ meson of the \f$B_{sig}\f$.
    float get_sig_Dmass() const { return sig_Dmass; }
    void set_sig_Dmass(float _sig_Dmass) { sig_Dmass = _sig_Dmass; }

    //! \f$\Delta m\f$ of the \f$ D \f$ meson of the \f$B_{sig}\f$.
    float get_sig_deltaM() const { return sig_deltaM; }
    void set_sig_deltaM(float _sig_deltaM) { sig_deltaM = _sig_deltaM; }

    //! \f$ \cos\theta_{D soft} \f$ of the \f$B_{sig}\f$
    /*! This is the cosine of the angle between the daughters of the
     * \f$D^*\f$ decay; namely the angle between the \f$ D \f$ meson
     * and the soft daughter (photon or a \f$\pi^0\f$). */
    float get_sig_cosThetaDSoft() const { return sig_cosThetaDSoft; }
    void set_sig_cosThetaDSoft(float _sig_cosThetaDSoft) { sig_cosThetaDSoft = _sig_cosThetaDSoft; }

    //! \f$ |\vec{p}_{soft}| \f$ of the \f$B_{sig}\f$
    /*! Magnitude of the three momentum of the \f$D^*\f$ meson's soft
     * daughter in the center of mass frame. */
    float get_sig_softP3MagCM() const { return sig_softP3MagCM; }
    void set_sig_softP3MagCM(float _sig_softP3MagCM) { sig_softP3MagCM = _sig_softP3MagCM; }

    //! Mass of the \f$h\f$. 
    float get_sig_hmass() const { return sig_hmass; }
    void set_sig_hmass(float _sig_hmass) { sig_hmass = _sig_hmass; }

    //! \f$h\f$ vertex quality. 
    /*! Vertex quality of the \f$h\f$. In this case, it can only be
     * the \f$\rho^+\f$. */
    float get_sig_vtxh() const { return sig_vtxh; }
    void set_sig_vtxh(float _sig_vtxh) { sig_vtxh = _sig_vtxh; }

    //! \f$B\f$ flavor. 
    /*! Returns an integer that corresponds to the #BFlavor enum in
     * bdtaunu_definitions.h */
    bdtaunu::BFlavor get_bflavor() const { return bflavor; }
    void set_bflavor(bdtaunu::BFlavor _bflavor) { bflavor = _bflavor; }
 
    //! Reconstructed \f$D^*\f$ mode index of the \f$B_{tag}\f$. 
    /*! Returns an integer that corresponds to the #DstarMode enum in
     * bdtaunu_definitions.h */
    bdtaunu::RecoDTypeCatalogue::DstarType get_tag_dstar_mode() const { return tag_dstar_mode; }
    void set_tag_dstar_mode(bdtaunu::RecoDTypeCatalogue::DstarType _tag_dstar_mode) { tag_dstar_mode = _tag_dstar_mode; }

    //! Reconstructed \f$D\f$ mode index of the \f$B_{tag}\f$. 
    /*! Returns an integer that corresponds to the #DMode enum in
     * bdtaunu_definitions.h */
    bdtaunu::RecoDTypeCatalogue::DType get_tag_d_mode() const { return tag_d_mode; }
    void set_tag_d_mode(bdtaunu::RecoDTypeCatalogue::DType _tag_d_mode) { tag_d_mode = _tag_d_mode; }

    //! Reconstructed \f$D^*\f$ mode index of the \f$B_{sig}\f$. 
    /*! Returns an integer that corresponds to the #DstarMode enum in
     * bdtaunu_definitions.h */
    bdtaunu::RecoDTypeCatalogue::DstarType get_sig_dstar_mode() const { return sig_dstar_mode; }
    void set_sig_dstar_mode(bdtaunu::RecoDTypeCatalogue::DstarType _sig_dstar_mode) { sig_dstar_mode = _sig_dstar_mode; }

    //! Reconstructed \f$D\f$ mode index of the \f$B_{sig}\f$. 
    /*! Returns an integer that corresponds to the #DMode enum in
     * bdtaunu_definitions.h */
    bdtaunu::RecoDTypeCatalogue::DType get_sig_d_mode() const { return sig_d_mode; }
    void set_sig_d_mode(bdtaunu::RecoDTypeCatalogue::DType _sig_d_mode) { sig_d_mode = _sig_d_mode; }

    //! Reconstructed \f$\tau\f$ mode index of the \f$B_{sig}\f$. 
    /*! Returns an integer that corresponds to the #TauMode enum in
     * bdtaunu_definitions.h */
    bdtaunu::TauType get_sig_tau_mode() const { return sig_tau_mode; }
    void set_sig_tau_mode(bdtaunu::TauType _sig_tau_mode) { sig_tau_mode = _sig_tau_mode; }

    //! Electron PID map of tag lepton.
    /*! Bit map is here:
     * http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Selectors/r24c/selectors.html
     */
    int get_l_ePidMap() const { return l_ePidMap; }
    void set_l_ePidMap(int _l_ePidMap) { l_ePidMap = _l_ePidMap; }
    
    //! Electron PID map of sig pion.
    /*! Bit map is here:
     * http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Selectors/r24c/selectors.html
     */
    int get_h_ePidMap() const { return h_ePidMap; }
    void set_h_ePidMap(int _h_ePidMap) { h_ePidMap = _h_ePidMap; }

    //! Muon PID map of tag lepton.
    /*! Bit map is here:
     * http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Selectors/r24c/selectors.html
     */
    int get_l_muPidMap() const { return l_muPidMap; }
    void set_l_muPidMap(int _l_muPidMap) { l_muPidMap = _l_muPidMap; }

    //! Muon PID map of sig pion.
    /*! Bit map is here:
     * http://www.slac.stanford.edu/BFROOT/www/Physics/Tools/Pid/Selectors/r24c/selectors.html
     */
    int get_h_muPidMap() const { return h_muPidMap; }
    void set_h_muPidMap(int _h_muPidMap) { h_muPidMap = _h_muPidMap; }

    //! Candidate type. 
    /*! Returns an int that corresponds to the #CandType enum in
     * BDtaunuDef.h */
    bdtaunu::CandType get_cand_type() const;

    //! Sample type. 
    /*! Returns an int that corresponds to the #SampleType enum in
     * BDtaunuDef.h */
    bdtaunu::SampleType get_sample_type() const;

  private:
    std::string eventId;
    int block_index;
    int reco_index;
    int truth_match;
    bdtaunu::BFlavor bflavor;
    float eextra50;
    float mmiss_prime2;
    float cosThetaT;
    float tag_lp3;
    float tag_cosBY;
    float tag_cosThetaDl;
    float tag_Dmass;
    float tag_deltaM;
    float tag_cosThetaDSoft;
    float tag_softP3MagCM;
    bdtaunu::RecoDTypeCatalogue::DType tag_d_mode;
    bdtaunu::RecoDTypeCatalogue::DstarType tag_dstar_mode;
    int l_ePidMap;
    int l_muPidMap;
    float sig_hp3;
    float sig_cosBY;
    float sig_cosThetaDtau;
    float sig_vtxB;
    float sig_Dmass;
    float sig_deltaM;
    float sig_cosThetaDSoft;
    float sig_softP3MagCM;
    float sig_hmass;
    float sig_vtxh;
    bdtaunu::RecoDTypeCatalogue::DType sig_d_mode;
    bdtaunu::RecoDTypeCatalogue::DstarType sig_dstar_mode;
    bdtaunu::TauType sig_tau_mode;
    int h_ePidMap;
    int h_muPidMap;

    // constructor helper
    void copy_candidate(const UpsilonCandidate &cand);
};

#endif
