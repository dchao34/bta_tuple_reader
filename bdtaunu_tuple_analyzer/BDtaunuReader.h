#ifndef __BDTAUNUREADER_H__
#define __BDTAUNUREADER_H__

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "RootReader.h"
#include "UpsilonCandidate.h"
#include "RecoGraphManager.h"


/** 
 * @brief 
 * This class is responsible for reading in BtaTupleMaker ntuples
 * generated from the `BToDTauNuSemiLepHadUser` package at SLAC. 
 *
 *
 * @detail
 * This class manages a buffer. For each event, it reads in all information 
 * related to reconstruction into this buffer and computes information needed 
 * for the analysis. 
 *
 * Usage Example
 * -------------
 *
 *     // Open the root file and the TTree (defaults to tree "ntp1").
 *     BDtaunuReader reader("sp1235r1.root");
 *
 *     // Loop over each event. 
 *     while (reader.next_record() != RootReader::Status::kEOF) {
 *       // Do stuff... 
 *       // reader.get_nTrk(); etc.
 *     }
 *
 */
class BDtaunuReader : public RootReader {

  friend class RecoGraphManager;

  public: 

    // API
    // ---

    //! No default constructor.
    BDtaunuReader() = delete;

    //! Open root file root_fname and read the TTree root_trname.
    BDtaunuReader(const char *root_fname, const char *root_trname = "ntp1");

    //! No copy constructor.
    BDtaunuReader(const BDtaunuReader&) = delete;

    //! No copy assignment.
    BDtaunuReader &operator=(const BDtaunuReader&) = delete;
    virtual ~BDtaunuReader();

    //! Read in the next event. 
    /*! Reader in next event and returns RootReader::Status.
     * Calling this automatically computes all features associated
     * with the event that the analysis is interested in. */
    virtual RootReader::Status next_record();

    //! Babar event Id. 
    std::string get_eventId() const;

    //! nTRK defined in BtaTupleMaker. 
    int get_nTrk() const { return nTrk; }

    //! Number of \f$\Upsilon(4S)\f$ candidates associated with this event. 
    int get_nY() const { return nY; }

    //! Second Fox-Wolfram moment. 
    float get_R2All() const { return R2All; }

    //! Return list of \f$\Upsilon(4S)\f$ candidates in this event.
    const std::vector<UpsilonCandidate> &get_upsilon_candidates() const { return upsilon_candidates; }

    //! Prints graphviz file of the reco graph to ostream.
    void print_reco_graph(std::ostream &os) const { reco_graph_manager.print(os); }

  protected:

    // Static members
    // --------------
    static const std::map<int, std::string> lund_to_name;
    static const int maximum_h_candidates;
    static const int maximum_l_candidates;
    static const int maximum_gamma_candidates;

    // Class members
    // -------------

    // Reco graph manager
    RecoGraphManager reco_graph_manager;

    // Upsilon candidates derived from the buffer candidates
    std::vector<UpsilonCandidate> upsilon_candidates;

  private: 

    // Static members
    // --------------
    static const int maximum_Y_candidates;
    static const int maximum_B_candidates;
    static const int maximum_D_candidates;
    static const int maximum_C_candidates;

    // Buffer elements 
    // ---------------
    int platform, partition, upperID, lowerID;
    int nTrk;
    float R2All;

    float *YBPairMmissPrime2, *YBPairEextra50;
    float *YTagBlP3MagCM, *YSigBhP3MagCM;
    float *YTagBCosBY, *YSigBCosBY;
    float *YTagBCosThetaDlCM, *YSigBCosThetaDtauCM;
    float *YSigBVtxProbB;
    float *YBPairCosThetaT;
    float *YTagBDMass, *YTagBDstarDeltaM;
    float *YTagBCosThetaDSoftCM, *YTagBsoftP3MagCM;
    float *YSigBDMass, *YSigBDstarDeltaM;
    float *YSigBCosThetaDSoftCM, *YSigBsoftP3MagCM;
    float *YSigBhMass, *YSigBVtxProbh;

    int *lTrkIdx, *hTrkIdx;
    int *eSelectorsMap, *muSelectorsMap, *KSelectorsMap, *piSelectorsMap;

    int nY, nB, nD, nC, nh, nl, ngamma; 
    int *YLund, *BLund, *DLund, *CLund, *hLund, *lLund, *gammaLund;
    int *Yd1Idx, *Yd2Idx;
    int *Bd1Idx, *Bd2Idx, *Bd3Idx, *Bd4Idx;
    int *Dd1Idx, *Dd2Idx, *Dd3Idx, *Dd4Idx, *Dd5Idx;
    int *Cd1Idx, *Cd2Idx;
    int *hd1Idx, *hd2Idx;
    int *ld1Idx, *ld2Idx, *ld3Idx;
    int *Yd1Lund, *Yd2Lund;
    int *Bd1Lund, *Bd2Lund, *Bd3Lund, *Bd4Lund;
    int *Dd1Lund, *Dd2Lund, *Dd3Lund, *Dd4Lund, *Dd5Lund;
    int *Cd1Lund, *Cd2Lund;
    int *hd1Lund, *hd2Lund;
    int *ld1Lund, *ld2Lund, *ld3Lund;

    // Helper functions
    // ----------------
  
    void AllocateBuffer();
    void DeleteBuffer();
    void ClearBuffer();

    bool is_max_reco_exceeded() const;
    void FillRecoInfo();

};

#endif
