#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <fstream>
#include <cassert>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

#include "BDtaunuDef.h"
#include "TruthMatchManager.h"
#include "BDtaunuMcReader.h"
#include "GraphDef.h"
#include "RecoGraphManager.h"
#include "McGraphManager.h"
#include "BDtaunuGraphWriter.h"

using namespace boost;
using namespace bdtaunu;

// TruthMatchMananger
// ------------------

TruthMatchManager::TruthMatchManager() : reader(nullptr) {
}

TruthMatchManager::TruthMatchManager(BDtaunuMcReader *_reader) : reader(_reader) {
}

int TruthMatchManager::get_truth_match_status(int reco_idx) const {
  std::map<int, int>::const_iterator it = truth_match.find(reco_idx);
  assert(it != truth_match.end());
  return it->second;
}

// Update cached graph. This classes analyzes whatever graphs that are stored there. 
void TruthMatchManager::update_graph(
    const RecoGraphManager &reco_graph_manager, 
    const McGraphManager &mc_graph_manager) {

  // Detector hits from BtaTupleMaker. 
  hMCIdx = reader->hMCIdx;
  lMCIdx = reader->lMCIdx;
  gammaMCIdx = reader->gammaMCIdx;

  // Get copies of the MC and reconstructed graphs. Copies because 
  // we will need to modify them for the algorithm.  
  reco_graph = reco_graph_manager.get_reco_graph();
  reco_indexer = reco_graph_manager.get_reco_indexer();
  mc_graph = mc_graph_manager.get_mc_graph();

  // Edge contract the MC graph.
  contract_mc_graph();
}

// Analyze cached graph. Entry point to the algorithm. 
void TruthMatchManager::analyze_graph() {
  truth_match.clear();
  depth_first_search(reco_graph, visitor(TruthMatchDfsVisitor(this)));
}

// Given a vertex on the original MC graph, decide whether it needs to 
// be ``cleaved''. These are vertices we do not wish to truth match. 
bool TruthMatchManager::is_cleave_vertex(
    const McGraph::Vertex &v, 
    const McGraph::Graph &g, 
    const McGraph::LundIdPropertyMap &lund_id_pm,
    const McGraph::McIndexPropertyMap &mc_idx_pm) const {

  // neutrinos, tau, and K0
  switch (abs(lund_id_pm[v])) {
    case nu_eLund:
    case nu_muLund:
    case nu_tauLund:
    case tauLund:
    case K0Lund:
      return true;
  }

  // initial e+e- beam particles
  switch (mc_idx_pm[v]) {
    case 0:
    case 1:
      return true;
  }

  // elimimate final state particles' daughters
  graph_traits<McGraph::Graph>::in_edge_iterator ie, ie_end;
  tie(ie, ie_end) = in_edges(v, g);
  if ((ie != ie_end) && (lund_id_pm[v] != UpsilonLund)) {
    assert(ie + 1 == ie_end);
    if (is_final_state_particle(lund_id_pm[source(*ie, g)])) {
      return true;
    }
  }

  // eliminate MC added photons
  if ((ie != ie_end) && (lund_id_pm[v] == gammaLund)) {
    assert(ie + 1 == ie_end);
    if (lund_id_pm[source(*ie, g)] != pi0Lund) {
      return true;
    }
  }

  return false;

}

// Edge contract the MC graph to get rid of the vertices we wanted to cleave. 
// http://en.wikipedia.org/wiki/Edge_contraction
void TruthMatchManager::contract_mc_graph() {

  McGraph::Graph &g = mc_graph;
  McGraph::McIndexPropertyMap mc_idx_pm = get(vertex_mc_index, g);
  McGraph::LundIdPropertyMap lund_id_pm = get(vertex_lund_id, g);

  // Scan through all MC graph vertices and decide which ones need to be cleaved.
  // Save the mc index in a vector.
  std::vector<int> to_cleave;
  graph_traits<McGraph::Graph>::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    if (is_cleave_vertex(*vi, g, lund_id_pm, mc_idx_pm)) to_cleave.push_back(mc_idx_pm[*vi]);

  // Cleave away a vertex by contracting the edge between its mother and itself.
  for (auto i : to_cleave) {

    // Get access to the actual vertex object.
    graph_traits<McGraph::Graph>::vertex_iterator vi_begin, vi_end;
    tie(vi_begin, vi_end) = vertices(g);
    McGraph::Vertex v = *std::find_if(vi_begin, vi_end, 
        [i, mc_idx_pm] (const McGraph::Vertex &v) { return (i == mc_idx_pm[v]); });

    // Build edges from its mother to its daughters.
    McGraph::Vertex m;
    graph_traits<McGraph::Graph>::in_edge_iterator ie, ie_end;
    tie(ie, ie_end) = in_edges(v, g);
    if (ie != ie_end) {
      assert(ie + 1 == ie_end);
      m = source(*ie, g);
      McGraph::Vertex d;
      graph_traits<McGraph::Graph>::out_edge_iterator oe, oe_end;
      for (tie(oe, oe_end) = out_edges(v, g); oe != oe_end; ++oe) {
        d = target(*oe, g);
        add_edge(m, d, g);
      }
    }

    // Get rid of this vertex and edges attached to it. 
    clear_vertex(v, g);
    remove_vertex(v, g);
    
  }

}

// Print cached MC graph (edge contracted). 
void TruthMatchManager::print_mc(std::ostream &os) const {

  auto lund_pm = get(vertex_lund_id, mc_graph);
  auto mc_idx_pm = get(vertex_mc_index, mc_graph);
  BDtaunuGraphvizManager<decltype(mc_graph), decltype(lund_pm), decltype(mc_idx_pm)> gv_manager(
      mc_graph, lund_pm, mc_idx_pm, BDtaunuMcReader::lund_to_name);

  gv_manager.set_title("MC Graph with Edge Contraction");
  gv_manager.set_vertex_property({"color", "blue"});
  gv_manager.set_vertex_property({"style", "filled"});
  gv_manager.set_vertex_property({"fillcolor", "white"});

  boost::write_graphviz(
      os, mc_graph, 
      gv_manager.construct_vertex_writer(),
      gv_manager.construct_edge_writer(),
      gv_manager.construct_graph_writer());
}

// Print the reco graph with truth matched candidates highlighted. 
void TruthMatchManager::print_reco(std::ostream &os) const {

  auto lund_pm = get(vertex_lund_id, reco_graph);
  auto reco_idx_pm = get(vertex_reco_index, reco_graph);
  BDtaunuGraphvizManager<decltype(reco_graph), decltype(lund_pm), decltype(reco_idx_pm)> gv_manager(
      reco_graph, lund_pm, reco_idx_pm, BDtaunuMcReader::lund_to_name, truth_match);

  gv_manager.set_title("Reco Graph with Truth Match");
  gv_manager.set_vertex_property({"color", "red"});
  gv_manager.set_vertex_property({"style", "filled"});
  gv_manager.set_vertex_property({"fillcolor", "white"});

  gv_manager.set_tm_vertex_property({"color", "red"});
  gv_manager.set_tm_vertex_property({"penwidth", "3"});
  gv_manager.set_tm_vertex_property({"style", "filled"});
  gv_manager.set_tm_vertex_property({"fillcolor", "lightskyblue"});

  gv_manager.set_edge_property({"color", "grey"});

  gv_manager.set_tm_edge_property({"color", "black"});
  gv_manager.set_tm_edge_property({"weight", "1"});
  gv_manager.set_tm_edge_property({"penwidth", "3"});

  boost::write_graphviz(
      os, reco_graph, 
      gv_manager.construct_vertex_writer(),
      gv_manager.construct_edge_writer(),
      gv_manager.construct_graph_writer());
}






// TruthMatchVisitor
// -----------------

TruthMatchDfsVisitor::TruthMatchDfsVisitor() : manager(nullptr) {
}

TruthMatchDfsVisitor::TruthMatchDfsVisitor(TruthMatchManager *_manager) : manager(_manager) {
  reco_lund_pm = get(vertex_lund_id, manager->reco_graph);
  reco_idx_pm = get(vertex_reco_index, manager->reco_graph);
  block_idx_pm = get(vertex_block_index, manager->reco_graph);
  mc_lund_pm = get(vertex_lund_id, manager->mc_graph);
  mc_idx_pm = get(vertex_mc_index, manager->mc_graph);
}

// Determine whether the vertex just blackened is a final state or composite.
void TruthMatchDfsVisitor::finish_vertex(RecoGraph::Vertex u, const RecoGraph::Graph &g) {
  int lund = std::abs(reco_lund_pm[u]);
  switch (lund) {
    case bdtaunu::UpsilonLund:
    case bdtaunu::B0Lund:
    case bdtaunu::BcLund:
    case bdtaunu::Dstar0Lund:
    case bdtaunu::DstarcLund:
    case bdtaunu::D0Lund:
    case bdtaunu::DcLund:
    case bdtaunu::KSLund:
    case bdtaunu::rhoLund:
    case bdtaunu::pi0Lund:
      MatchCompositeState(u, g);
      break;
    case bdtaunu::eLund:
    case bdtaunu::muLund:
    case bdtaunu::piLund:
    case bdtaunu::KLund:
    case bdtaunu::gammaLund:
      MatchFinalState(u);
      break;
    default:
      assert(false);
      return;
  }
}

// For final states, follow Case 1 described in `TruthMatchMananger.h`.
void TruthMatchDfsVisitor::MatchFinalState(const RecoGraph::Vertex &u) {
  const int *hitMap = nullptr;
  switch (abs(reco_lund_pm[u])) {
    case eLund:
    case muLund:
      hitMap = manager->lMCIdx;
      break;
    case piLund:
    case KLund:
      hitMap = manager->hMCIdx;
      break;
    case gammaLund:
      hitMap = manager->gammaMCIdx;
      break;
    default:
      assert(false);
  }

  manager->truth_match.insert(
      std::make_pair(reco_idx_pm[u], hitMap[block_idx_pm[u]]));

}

// For composite states, follow Case 2 described in `TruthMatchMananger.h`.
void TruthMatchDfsVisitor::MatchCompositeState(const RecoGraph::Vertex &u, const RecoGraph::Graph &g) {

  int tm_mc_idx = -1;

  // For each daughter, get the mc index of the MC particle it truth matches to. 
  std::vector<int> dau_tm_mc_idx;
  RecoGraph::AdjacencyIterator ai, ai_end;
  for (tie(ai, ai_end) = adjacent_vertices(u, g); ai != ai_end; ++ai) {
    dau_tm_mc_idx.push_back((manager->truth_match).find(reco_idx_pm[*ai])->second);
  }
  std::sort(dau_tm_mc_idx.begin(), dau_tm_mc_idx.end());

  // If some daughter doesn't truth match, then this particle also doesn't. 
  if (dau_tm_mc_idx[0] < 0) {
    manager->truth_match.insert(
        std::make_pair(reco_idx_pm[u], tm_mc_idx));
    return; 
  }

  // Scan the MC graph for particles to try to truth match to. 
  graph_traits<McGraph::Graph>::vertex_iterator vi, vi_end;
  for (tie(vi, vi_end) = vertices(manager->mc_graph); vi != vi_end; ++vi) {

    // Consider only MC particles with the correct identity. 
    if (reco_lund_pm[u] == mc_lund_pm[*vi]) {

      // Get a list of the MC particle's daughter and store their mc_idx. 
      std::vector<int> dau_mc_idx;
      McGraph::AdjacencyIterator bi, bi_end;
      for (tie(bi, bi_end) = adjacent_vertices(*vi, manager->mc_graph); bi != bi_end; ++bi) {
        dau_mc_idx.push_back(mc_idx_pm[*bi]);
      }
      std::sort(dau_mc_idx.begin(), dau_mc_idx.end());

      // This checks whether that the reco daughters and mc daughters truth
      // match to each other. This was the reason for the std::sort earlier. 
      if (dau_tm_mc_idx == dau_mc_idx) {
        tm_mc_idx = mc_idx_pm[*vi];
        break;
      }
    }

  }

  manager->truth_match.insert(
      std::make_pair(reco_idx_pm[u], tm_mc_idx));

}
