#ifndef __BDTAUNUMCREADER_H__
#define __BDTAUNUMCREADER_H__

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "BDtaunuDef.h"
#include "BDtaunuReader.h"
#include "McGraphManager.h"

#include "TruthMatchManager.h"

/** 
 * @brief 
 * Much like its parent class BDtaunuReader, this class reads 
 * BtaTupleMaker Monte Carlo ntuples generated from the 
 * BToDTauNuSemiLepHadUser package at SLAC.
 *
 *
 * @detail
 * In addition to the operations that BDtaunuReader performs, this
 * class also reads and derives Monte Carlo information. The mechanism and
 * usage is the same as BDtaunuReader.
 *
 * Usage Example
 * -------------
 *
 * See BDtaunuReader. 
 *
 */
class BDtaunuMcReader : public BDtaunuReader {

  friend class McGraphManager;
  friend class TruthMatchManager;

  public: 

    // API
    // ---

    // Constructors
    BDtaunuMcReader() = delete;
    BDtaunuMcReader(const char *root_fname, const char *root_trname = "ntp1");
    BDtaunuMcReader(const BDtaunuMcReader&) = delete;
    BDtaunuMcReader &operator=(const BDtaunuMcReader&) = delete;
    ~BDtaunuMcReader();

    //! Read in the next event. 
    virtual RootReader::Status next_record();

    //! Flag whether the MC truth is Continuum. 
    bool is_continuum() const { return continuum; }

    //! MC type of the one MC truth B. 
    bdtaunu::McBTypeCatalogue::BMcType get_b1_mctype() const { return b1_mctype; }

    //! MC type of the other MC truth B. 
    bdtaunu::McBTypeCatalogue::BMcType get_b2_mctype() const { return b2_mctype; }

    //! MC type of the tau of one MC truth B. 
    bdtaunu::TauMcType get_b1_tau_mctype() const { return b1_tau_mctype; }

    //! MC type of the tau of the other MC truth B. 
    bdtaunu::TauMcType get_b2_tau_mctype() const { return b2_tau_mctype; }

    //! Printer
    void print_mc_graph(std::ostream &os) const { mc_graph_manager.print(os); }
    void print_contracted_mc_graph(std::ostream &os) const { truth_match_manager.print_mc(os); }
    void print_truthmatch_reco_graph(std::ostream &os) const { truth_match_manager.print_reco(os); }

    std::map<int, int> get_truth_map() const { return truth_match_manager.get_truth_map(); }

  private:

    // Static members
    // --------------
    const static int max_mc_length;

    // Buffer elements
    // ---------------
    int mcLen;
    int *mcLund; 
    int *mothIdx;
    int *dauIdx;
    int *dauLen;
    float *mcenergy;
    int *hMCIdx; 
    int *lMCIdx; 
    int *gammaMCIdx; 

    // Class members
    // -------------
    bool continuum;
    bdtaunu::McBTypeCatalogue::BMcType b1_mctype, b2_mctype;
    bdtaunu::TauMcType b1_tau_mctype, b2_tau_mctype;

    McGraphManager mc_graph_manager;
    TruthMatchManager truth_match_manager;

    // Helper functions
    // ----------------
    void AllocateBuffer();
    void DeleteBuffer();
    void ClearBuffer();

    bool is_max_mc_exceeded() { return (mcLen > max_mc_length) ? true : false; }
    void FillMcInfo();


};

#endif
