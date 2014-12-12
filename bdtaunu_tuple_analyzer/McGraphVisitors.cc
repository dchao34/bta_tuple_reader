#include <cmath>
#include <cassert>
#include <vector>

#include "BDtaunuDef.h"
#include "GraphDef.h"
#include "McGraphVisitors.h"
#include "McGraphManager.h"

using namespace boost;
using namespace bdtaunu;
using namespace McGraph;

McGraphDfsVisitor::McGraphDfsVisitor(McGraphManager *_manager) 
  : manager(_manager) {
  lund_map = get(vertex_lund_id, manager->g);
}

const McBTypeCatalogue McGraphDfsVisitor::mcB_catalogue = McBTypeCatalogue();

// Determine whether to analyze a MC particle 
// when its vertex is colored black. 
void McGraphDfsVisitor::finish_vertex(Vertex u, const Graph &g) {
  int lund = std::abs(get(lund_map, u));
  switch (lund) {
    case UpsilonLund:
      AnalyzeY(u, g);
      break;
    case B0Lund:
    case BcLund:
      AnalyzeB(u, g);
      break;
    case tauLund:
      AnalyzeTau(u, g);
      break;
    default:
      return;
  }
  return;
}


// Analyze Y(4S). The quantities computed are:
// 1. Pointers to the daughter B mesons.
void McGraphDfsVisitor::AnalyzeY(const Vertex &u, const Graph &g) {

  Y mcY;

  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {

    int lund = abs(get(lund_map, *ai));
    switch (lund) {
      case B0Lund:
      case BcLund:
        (mcY.B1 == nullptr) ? 
          (mcY.B1 = &(manager->B_map)[*ai]) : 
          (mcY.B2 = &(manager->B_map)[*ai]);
        break;
      default:
        mcY.isBBbar = false;
        return;
    }
  }

  (manager->Y_map).insert(std::make_pair(u, mcY));
}

// Analyze B meson. The quantities computed are:
// 1. B flavor. 
// 2. Pointer to daughter tau. 
// 3. MC type. See GraphDef.h.
void McGraphDfsVisitor::AnalyzeB(const Vertex &u, const Graph &g) {

  B mcB;

  if (abs(get(lund_map, u)) == B0Lund) {
    mcB.flavor = BFlavor::B0;
  } else {
    mcB.flavor = BFlavor::Bc;
  }

  std::vector<int> daulund_list;
  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {
    int lund = get(lund_map, *ai);
    switch (abs(lund)) {
      case tauLund:
        mcB.tau = &(manager->Tau_map)[*ai];
      default:
        daulund_list.push_back(lund);
    }
  }
  mcB.mc_type = mcB_catalogue.search_catalogue(daulund_list);

  (manager->B_map).insert(std::make_pair(u, mcB));
}

// Analyze tau. The quantities computed are:
// 1. MC type. See GraphDef.h.
void McGraphDfsVisitor::AnalyzeTau(const Vertex &u, const Graph &g) {

  Tau mcTau;

  AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {
    int lund = abs(get(lund_map, *ai));
    if (lund == eLund) {
      mcTau.mc_type = TauMcType::tau_e;
      break;
    } else if (lund == muLund) {
      mcTau.mc_type = TauMcType::tau_mu;
      break;
    } else {
      mcTau.mc_type = TauMcType::tau_h;
    }
  }

  (manager->Tau_map).insert(std::make_pair(u, mcTau));
}
