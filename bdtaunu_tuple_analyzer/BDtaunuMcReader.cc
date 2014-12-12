#include <map>
#include <string>
#include <cmath>
#include <cassert>

#include "BDtaunuDef.h"
#include "BDtaunuUtils.h"
#include "BDtaunuReader.h"
#include "BDtaunuMcReader.h"
#include "McGraphManager.h"
#include "TruthMatchManager.h"

using namespace boost;
using namespace bdtaunu;

const int BDtaunuMcReader::max_mc_length = 100;

// The constructor just needs to allocate and initialize the buffer 
// and the mc graph manager.
BDtaunuMcReader::BDtaunuMcReader(
  const char *root_fname,
  const char *root_trname) : 
  BDtaunuReader(root_fname, root_trname) {

  AllocateBuffer();
  ClearBuffer();
  mc_graph_manager = McGraphManager(this);
  truth_match_manager = TruthMatchManager(this);

}

BDtaunuMcReader::~BDtaunuMcReader() {
  DeleteBuffer();
}


// Initializes the input buffer.
void BDtaunuMcReader::AllocateBuffer() {

  // Allocate space to read in arrays from ntuples. 
  mcLund = new int[max_mc_length];
  mothIdx = new int[max_mc_length];
  dauIdx = new int[max_mc_length];
  dauLen = new int[max_mc_length];
  mcenergy = new float[max_mc_length];
  hMCIdx = new int[maximum_h_candidates];
  lMCIdx = new int[maximum_l_candidates];
  gammaMCIdx = new int[maximum_gamma_candidates];

  // Specify the variables where each ntuple branch should be read into. 
  tr->SetBranchAddress("mcLen", &mcLen);
  tr->SetBranchAddress("mcLund", mcLund);
  tr->SetBranchAddress("mothIdx", mothIdx);
  tr->SetBranchAddress("dauIdx", dauIdx);
  tr->SetBranchAddress("dauLen", dauLen);
  tr->SetBranchAddress("mcenergy", mcenergy);
  tr->SetBranchAddress("hMCIdx", hMCIdx);
  tr->SetBranchAddress("lMCIdx", lMCIdx);
  tr->SetBranchAddress("gammaMCIdx", gammaMCIdx);

}

// Zeros out buffer elements
void BDtaunuMcReader::ClearBuffer() {
  mcLen = -999;
  continuum = true;
  b1_mctype = McBTypeCatalogue::BMcType::NoB;
  b2_mctype = McBTypeCatalogue::BMcType::NoB;
  b1_tau_mctype = TauMcType::NoTau;
  b2_tau_mctype = TauMcType::NoTau;
}

// Free the buffer. Used for destructor. 
void BDtaunuMcReader::DeleteBuffer() {
  delete[] mcLund;
  delete[] mothIdx;
  delete[] dauIdx;
  delete[] dauLen;
  delete[] mcenergy;
  delete[] hMCIdx;
  delete[] lMCIdx;
  delete[] gammaMCIdx;
}

// Read in the next event in the ntuple and update the buffer
// with the new information.
RootReader::Status BDtaunuMcReader::next_record() {

  ClearBuffer();

  // Read next event into the buffer. Calls BDtaunuReader::next_record()
  // first to compute reco information. 
  RootReader::Status reader_status = BDtaunuReader::next_record();

  // Derive additional mc information from the ntuple. 
  if (reader_status == RootReader::Status::kReadSucceeded) {

    // This check is necessary since BtaTupleMaker does not save 
    // this kind of event correctly. Our solution is to skip this
    // kind of event altogether. 
    if (is_max_mc_exceeded()) {
      reader_status = RootReader::Status::kMaxMcParticlesExceeded;
    } else {

      // Outsource graph operations to graph manager
      mc_graph_manager.construct_graph();
      mc_graph_manager.analyze_graph();

      // Outsource truth match operations to truth matcher
      truth_match_manager.update_graph(reco_graph_manager, mc_graph_manager);
      truth_match_manager.analyze_graph();

      // Make derived information ready for access
      FillMcInfo();
    }
  }
  
  return reader_status;
}

void BDtaunuMcReader::FillMcInfo() {
  if (mc_graph_manager.get_mcY()) 
    continuum = !(mc_graph_manager.get_mcY()->isBBbar);
  if (mc_graph_manager.get_mcB1()) {
    b1_mctype = mc_graph_manager.get_mcB1()->mc_type;
    if (mc_graph_manager.get_mcB1()->tau) 
      b1_tau_mctype = mc_graph_manager.get_mcB1()->tau->mc_type;
  }
  if (mc_graph_manager.get_mcB2()) {
    b2_mctype = mc_graph_manager.get_mcB2()->mc_type;
    if (mc_graph_manager.get_mcB2()->tau) 
      b2_tau_mctype = mc_graph_manager.get_mcB2()->tau->mc_type;
  }

  auto it = upsilon_candidates.begin();
  while (it != upsilon_candidates.end()) {
    it->set_truth_match(
        truth_match_manager.get_truth_match_status(it->get_reco_index()));
    ++it;
  }

  return;
}
