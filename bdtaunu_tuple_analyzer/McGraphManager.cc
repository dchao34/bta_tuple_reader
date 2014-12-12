#include <iostream>
#include <cmath>
#include <cassert>
#include <map>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

#include "BDtaunuDef.h"
#include "GraphDef.h"
#include "BDtaunuMcReader.h"
#include "McGraphManager.h"
#include "BDtaunuGraphWriter.h"

using namespace boost;
using namespace McGraph;

const std::set<int> McGraphManager::final_state_particles = { 
  bdtaunu::eLund, bdtaunu::muLund, bdtaunu::piLund, bdtaunu::KLund,
  bdtaunu::gammaLund, bdtaunu::protonLund, bdtaunu::neutronLund,
};

bool is_final_state_particle(int lund) {
  return 
    (McGraphManager::final_state_particles.find(std::abs(lund)) == 
     McGraphManager::final_state_particles.end()) ? false : true;
}

McGraphManager::McGraphManager() : reader(nullptr) { 
}

McGraphManager::McGraphManager(BDtaunuMcReader *_reader) : reader(_reader) { 
}

// Clear all cache.
void McGraphManager::clear() {
  ClearGraph();
  ClearAnalysis();
}

// Clear graph cache. 
void McGraphManager::ClearGraph() {
  mc_vertex_map.clear();
  g.clear();
}

// Construct BGL graph from BDtaunuMcReader's buffer.
void McGraphManager::construct_graph() {

  ClearGraph();

  assert(reader != nullptr);

  // Each vertex has the following information attached:
  // 
  // 1. vertex_mc_index: The MC index of the particle. 
  //
  // 2. vertex_lund_id: Lund ID of the MC particle. 
  McIndexPropertyMap mc_index = get(vertex_mc_index, g);
  LundIdPropertyMap lund_id = get(vertex_lund_id, g);

  Vertex u, v;
  std::map<int, Vertex>::iterator pos;
  bool inserted;

  for (int i = 0; i < reader->mcLen; i++) {

    // Attempt to insert this particle into graph. If not already
    // inserted, add a new vertex and return a handler to it. Otherwise, 
    // just get the handler to its vertex. 
    //
    // mc_vertex_map keeps track of which particle has been inserted. 
    // The key is the MC index.
    int u_idx = i;
    tie(pos, inserted) = mc_vertex_map.insert(std::make_pair(u_idx, Vertex()));
    if (inserted) {
      u = add_vertex(g);
      mc_index[u] = u_idx;
      lund_id[u] = (reader->mcLund)[i];
      mc_vertex_map[u_idx] = u;
    } else {
      u = mc_vertex_map[u_idx];
    }

    // Examine all daughters of this particle and insert any daughter
    // that has not already been inserted. An edge is also inserted between 
    // the particle and all its daughters.
    int first_dau_idx = (reader->dauIdx)[i];
    for (int j = 0; j < (reader->dauLen)[i]; j++) {

      int v_idx = first_dau_idx + j;
      tie(pos, inserted) = mc_vertex_map.insert(std::make_pair(v_idx, Vertex()));
      if (inserted) {
        v = add_vertex(g);
        mc_index[v] = v_idx;
        lund_id[v] = (reader->mcLund)[v_idx];
        mc_vertex_map[v_idx] = v;
      } else {
        v = mc_vertex_map[v_idx];
      }

      add_edge(u, v, g);
    }
  }
}

// Clear graph analysis cache. 
void McGraphManager::ClearAnalysis() {
  Y_map.clear();
  B_map.clear();
  Tau_map.clear();
}

// Traverses BGL graph to compute analysis statistics.
void McGraphManager::analyze_graph() {

  ClearAnalysis();

  // See McGraphDfsVisitor.h for more information. 
  depth_first_search(g, visitor(McGraphDfsVisitor(this)));
  assert(Y_map.size() == 0 || Y_map.size() == 1);
  assert(B_map.size() == 0 || B_map.size() == 2);
  return;
}

const Y* McGraphManager::get_mcY() const {
  if (Y_map.size()) {
    return &Y_map.begin()->second;
  } else {
    return nullptr;
  }
}

const B* McGraphManager::get_mcB1() const {
  if (B_map.size()) {
    return &B_map.begin()->second;
  } else {
    return nullptr;
  }
}

const B* McGraphManager::get_mcB2() const {
  if (B_map.size()) {
    return &(++B_map.begin())->second;
  } else {
    return nullptr;
  }
}

// Print graphviz file. See BDtaunuGraphWriter.h.
void McGraphManager::print(std::ostream &os) const {

  auto lund_pm = get(vertex_lund_id, g);
  auto mc_idx_pm = get(vertex_mc_index, g);
  BDtaunuGraphvizManager<Graph, decltype(lund_pm), decltype(mc_idx_pm)> gv_manager(
      g, lund_pm, mc_idx_pm, BDtaunuMcReader::lund_to_name);

  gv_manager.set_title("MC Graph");
  gv_manager.set_vertex_property({"color", "blue"});
  gv_manager.set_vertex_property({"style", "filled"});
  gv_manager.set_vertex_property({"fillcolor", "white"});

  boost::write_graphviz(
      os, g, 
      gv_manager.construct_vertex_writer(),
      gv_manager.construct_edge_writer(),
      gv_manager.construct_graph_writer());
}
