#include <cmath>
#include <vector>
#include <cassert>

#include "GraphDef.h"
#include "RecoGraphVisitors.h"
#include "RecoGraphManager.h"

using namespace boost;
using namespace RecoGraph;
using namespace bdtaunu;

RecoGraphDfsVisitor::RecoGraphDfsVisitor(RecoGraphManager *_manager) 
  : manager(_manager) {
  lund_map = get(vertex_lund_id, manager->g);
  block_idx_map = get(vertex_block_index, manager->g);
}

const RecoDTypeCatalogue RecoGraphDfsVisitor::recoD_catalogue = RecoDTypeCatalogue();

// Determine whether to analyze a reco particle 
// when its vertex is colored black. 
void RecoGraphDfsVisitor::finish_vertex(Vertex u, const Graph &g) {
  int lund = std::abs(get(lund_map, u));
  switch (lund) {
    case bdtaunu::UpsilonLund:
      AnalyzeY(u, g);
      break;
    case bdtaunu::B0Lund:
    case bdtaunu::BcLund:
      AnalyzeB(u, g);
      break;
    case bdtaunu::Dstar0Lund:
    case bdtaunu::DstarcLund:
      AnalyzeDstar(u, g);
      break;
    case bdtaunu::D0Lund:
    case bdtaunu::DcLund:
      AnalyzeD(u, g);
      break;
    case bdtaunu::piLund:
    case bdtaunu::rhoLund:
    case bdtaunu::eLund:
    case bdtaunu::muLund:
      AnalyzeLepton(u, g);
      break;
    default:
      return;
  }
  return;
}

// Analyze D meson. The quantities computed are:
// 1. D reconstruction mode. See RecoDTypeCatalogue.h for the definitions.
void RecoGraphDfsVisitor::AnalyzeD(const Vertex &u, const Graph &g) {

  D recoD;

  // Compute D reconstruction mode. Scan all of its daughters and 
  // look up the mode in recoD_catalogue. 
  std::vector<int> lund_list;
  lund_list.push_back(get(lund_map, u));

  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {
    lund_list.push_back(get(lund_map, *ai));
  }
  recoD.D_mode = recoD_catalogue.search_d_catalogue(lund_list);

  // Insert results into supervisor's cache. 
  (manager->D_map).insert(std::make_pair(u, recoD));

}

// Analyze Dstar meson. The quantities computed are:
// 1. Dstar reconstruction mode. See RecoDTypeCatalogue.h for the definitions.
// 2. Daughter D meson's reconstructed mode.
void RecoGraphDfsVisitor::AnalyzeDstar(const Vertex &u, const Graph &g) {

  D recoD;

  // Scan all daughters and do the following:
  // 1. Look up Dstar mode in recoD_catalogue. 
  // 2. Look up daughter D's mode that is already stored in 
  // the supervising class' cache.
  std::vector<int> lund_list;
  lund_list.push_back(get(lund_map, u));

  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {

    int lund = get(lund_map, *ai);
    switch (abs(lund)) {
      case bdtaunu::D0Lund:
      case bdtaunu::DcLund:
        recoD.D_mode = (manager->D_map)[*ai].D_mode;
      case bdtaunu::piLund:
      case bdtaunu::pi0Lund:
      case bdtaunu::gammaLund:
        lund_list.push_back(get(lund_map, *ai));
        break;
      default:
        assert(false);
        return;
    }
  }
  recoD.Dstar_mode = recoD_catalogue.search_dstar_catalogue(lund_list);

  // Insert results into supervisor's cache. 
  (manager->D_map).insert(std::make_pair(u, recoD));

}

// Analyze "Leptons". These are either actual leptons are placeholder 
// hadron for the tau of the B decay. See GraphDef.h for more info.
// The quantities computed are:
// 1. The tau decay mode. For actual leptons, this is what the 
// tau mode would have been if it were actually from a tau decay. 
// 2. The corresponding block index of the lepton or hadron. This is 
// used to access PID information later. 
void RecoGraphDfsVisitor::AnalyzeLepton(const Vertex &u, const Graph &g) {

  Lepton recoLepton;

  // Scan all daughters
  AdjacencyIterator ai, ai_end;
  int lund = abs(get(lund_map, u));
  switch (lund) {

    case bdtaunu::eLund:
      recoLepton.l_block_idx = block_idx_map[u];
      recoLepton.pi_block_idx = -1;
      recoLepton.tau_mode = bdtaunu::TauType::tau_e;

    case bdtaunu::muLund:
      recoLepton.l_block_idx = block_idx_map[u];
      recoLepton.pi_block_idx = -1;
      recoLepton.tau_mode = bdtaunu::TauType::tau_mu;
      break;

    case bdtaunu::piLund:
      recoLepton.l_block_idx = -1;
      recoLepton.pi_block_idx = block_idx_map[u];
      recoLepton.tau_mode = bdtaunu::TauType::tau_pi;
      break;

    // when a rho is encountered, scan its daughters to find the pion.
    case bdtaunu::rhoLund:
      recoLepton.l_block_idx = -1;
      for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {
        if (abs(get(lund_map, *ai)) == bdtaunu::piLund) {
          recoLepton.pi_block_idx = (manager->Lepton_map)[*ai].pi_block_idx;
          break;
        }
      }
      recoLepton.tau_mode = bdtaunu::TauType::tau_rho;
      break;

    default:
      assert(false);
      return;
  }

  (manager->Lepton_map).insert(std::make_pair(u, recoLepton));
}


// Analyze B mesons. Computed quantities are:
// 1. B flavor. 
// 2. Pointer to the corresponding D and Lepton daughter stored in
// the cache of the supervisor class (See BDtaunuReader.h). 
void RecoGraphDfsVisitor::AnalyzeB(const Vertex &u, const Graph &g) {

  B recoB;

  if (abs(get(lund_map, u)) == bdtaunu::B0Lund) {
    recoB.flavor = bdtaunu::BFlavor::B0;
  } else {
    recoB.flavor = bdtaunu::BFlavor::Bc;
  }

  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {

    int lund = abs(get(lund_map, *ai));
    switch (lund) {
      case bdtaunu::D0Lund:
      case bdtaunu::DcLund:
      case bdtaunu::Dstar0Lund:
      case bdtaunu::DstarcLund:
        recoB.d = &(manager->D_map)[*ai];
        break;
      case bdtaunu::eLund:
      case bdtaunu::muLund:
      case bdtaunu::piLund:
      case bdtaunu::rhoLund:
        recoB.lepton = &(manager->Lepton_map)[*ai];
        break;
      default:
        assert(false);
        return;
    }
  }

  (manager->B_map).insert(std::make_pair(u, recoB));
}



// Analyze Y(4S) candidates. Computed quantities are:
// 1. B flavor. 
// 2. Pointer to the corresponding D and Lepton daughter stored in
// the cache of the supervisor class (See BDtaunuReader.h). 
void RecoGraphDfsVisitor::AnalyzeY(const Vertex &u, const Graph &g) {

  Y recoY;

  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {

    int lund = abs(get(lund_map, *ai));
    switch (lund) {
      case bdtaunu::B0Lund:
      case bdtaunu::BcLund:
        if ((manager->B_map)[*ai].lepton->l_block_idx >= 0) {
          recoY.tagB = &(manager->B_map)[*ai];
        } else {
          recoY.sigB = &(manager->B_map)[*ai];
        }
        break;
      default:
        assert(false);
        return;
    }
  }

  (manager->Y_map).insert(std::make_pair(u, recoY));
}

