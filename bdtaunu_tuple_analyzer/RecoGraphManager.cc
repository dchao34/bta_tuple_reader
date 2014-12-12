#include <cassert>
#include <vector>
#include <map>
#include <iostream>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>

#include "GraphDef.h"
#include "BDtaunuReader.h"
#include "RecoGraphManager.h"
#include "BDtaunuGraphWriter.h"

using namespace boost;
using namespace RecoGraph;

RecoGraphManager::RecoGraphManager() : reader(nullptr) { 
}

RecoGraphManager::RecoGraphManager(BDtaunuReader *_reader) : reader(_reader) { 
}

// Clear all cache.
void RecoGraphManager::clear() {
  ClearGraph();
  ClearAnalysis();
}

// Clear graph cache. 
void RecoGraphManager::ClearGraph() {
  reco_vertex_map.clear();
  reco_indexer.clear();
  g.clear();
}

// Construct BGL graph from BDtaunuReader's buffer, whose format
// is determined by BtaTupleMaker in BToDTauNuSemiLepHadUser.
void RecoGraphManager::construct_graph() {

  ClearGraph();
  assert(reader != nullptr);

  // The reco indexer maps each reco particle candidate to a 
  // unique ID in the BGL graph to be constructed. 
  reco_indexer.set({reader->nY, reader->nB, reader->nD, reader->nC, 
                    reader->nh, reader->nl, reader->ngamma});

  // Pointer structure needed for AddCandidates(). See 
  // RecoGraphManager.h for more info. 
  std::vector<int*> YdauIdx{ reader->Yd1Idx, reader->Yd2Idx };
  std::vector<int*> YdauLund{ reader->Yd1Lund, reader->Yd2Lund };
  std::vector<int*> BdauIdx{ reader->Bd1Idx, reader->Bd2Idx, reader->Bd3Idx, reader->Bd4Idx };
  std::vector<int*> BdauLund{ reader->Bd1Lund, reader->Bd2Lund, reader->Bd3Lund, reader->Bd4Lund };
  std::vector<int*> DdauIdx{ reader->Dd1Idx, reader->Dd2Idx, reader->Dd3Idx, reader->Dd4Idx, reader->Dd5Idx };
  std::vector<int*> DdauLund{ reader->Dd1Lund, reader->Dd2Lund, reader->Dd3Lund, reader->Dd4Lund, reader->Dd5Lund };
  std::vector<int*> CdauIdx{ reader->Cd1Idx, reader->Cd2Idx };
  std::vector<int*> CdauLund{ reader->Cd1Lund, reader->Cd2Lund };
  std::vector<int*> hdauIdx{ reader->hd1Idx, reader->hd2Idx };
  std::vector<int*> hdauLund{ reader->hd1Lund, reader->hd2Lund };
  std::vector<int*> ldauIdx{ reader->ld1Idx, reader->ld2Idx, reader->ld3Idx };
  std::vector<int*> ldauLund{ reader->ld1Lund, reader->ld2Lund, reader->ld3Lund };

  // Populate the BGL graph. 
  AddCandidates(reader->nY, reader->YLund, YdauIdx, YdauLund);
  AddCandidates(reader->nB, reader->BLund, BdauIdx, BdauLund);
  AddCandidates(reader->nD, reader->DLund, DdauIdx, DdauLund);
  AddCandidates(reader->nC, reader->CLund, CdauIdx, CdauLund);
  AddCandidates(reader->nh, reader->hLund, hdauIdx, hdauLund);
  AddCandidates(reader->nl, reader->lLund, ldauIdx, ldauLund);

  return;
}


// Clear graph analysis cache. 
void RecoGraphManager::ClearAnalysis() {
  Y_map.clear();
  B_map.clear();
  D_map.clear();
  Lepton_map.clear();
}

// Traverses BGL graph to compute analysis statistics.
void RecoGraphManager::analyze_graph() {

  ClearAnalysis();

  // See RecoGraphDfsVisitor.h for more information. 
  depth_first_search(g, visitor(RecoGraphDfsVisitor(this)));

  return;
}

// Access statistics of the ith Y candidate that are computed from graph analysis.
const Y* RecoGraphManager::get_recoY(int i) const { 

  std::map<int, RecoGraph::Vertex>::const_iterator reco_vertex_it;
  reco_vertex_it = reco_vertex_map.find(reco_indexer.get_reco_idx(bdtaunu::UpsilonLund, i));
  assert(reco_vertex_it != reco_vertex_map.end());

  std::map<Vertex, Y>::const_iterator y_it;
  y_it = Y_map.find(reco_vertex_it->second);
  assert(y_it != Y_map.end());

  return &y_it->second;
}


// Print graphviz file. See BDtaunuGraphWriter.h.
void RecoGraphManager::print(std::ostream &os) const {
  auto lund_pm = get(vertex_lund_id, g);
  auto reco_pm = get(vertex_reco_index, g);
  BDtaunuGraphvizManager<Graph, decltype(lund_pm), decltype(reco_pm)> gv_manager(
      g, lund_pm, reco_pm, BDtaunuReader::lund_to_name);

  gv_manager.set_title("Reco Graph");
  gv_manager.set_vertex_property({"color", "red"});
  gv_manager.set_vertex_property({"style", "filled"});
  gv_manager.set_vertex_property({"fillcolor", "white"});

  boost::write_graphviz(
      os, g, 
      gv_manager.construct_vertex_writer(),
      gv_manager.construct_edge_writer(),
      gv_manager.construct_graph_writer());
}


// Populate graph with one type of reco particle candidate; 
// e.g. Y, B candidates.
void RecoGraphManager::AddCandidates(
    int nCand,
    int *CandLund,
    std::vector<int*> &CandDauIdx,
    std::vector<int*> &CandDauLund) {

  // Each vertex has the following information attached:
  // 
  // 1. vertex_reco_index: The unique reco particle index assigned by 
  // reco_indexer (See GraphDef.h). This tracks which 
  // vertex a particlar reco particle is associated with. 
  //
  // 2. vertex_block_index: This is the index of where this reco particle 
  // belongs in the candidate block as determined by BtaTupleMaker.
  // e.g. vertex_block_index = i for the ith Y candidate of the event. 
  //
  // 3. vertex_lund_id: Lund ID of the reco particle. 
  RecoIndexPropertyMap reco_index = get(vertex_reco_index, g);
  BlockIndexPropertyMap block_index = get(vertex_block_index, g);
  LundIdPropertyMap lund_id = get(vertex_lund_id, g);

  Vertex u, v;
  std::map<int, Vertex>::iterator pos;
  bool inserted;

  for (int i = 0; i < nCand; i++) {

    // Attempt to insert this candidate into graph. If not already
    // inserted, add a new vertex and return a handler to it. Otherwise, 
    // just get the handler to its vertex. 
    //
    // reco_vertex_map keeps track of which particle has been inserted. 
    // The key is the unique reco index from reco_indexer.
    int u_idx = reco_indexer.get_reco_idx(CandLund[i], i);
    tie(pos, inserted) = reco_vertex_map.insert(std::make_pair(u_idx, Vertex()));
    if (inserted) {
      u = add_vertex(g);
      reco_index[u] = u_idx;
      block_index[u] = i;
      lund_id[u] = CandLund[i];
      reco_vertex_map[u_idx] = u;
    } else {
      u = reco_vertex_map[u_idx];
    }

    // Examine all daughters of this candidate and insert any daughter
    // that has not already been inserted. An edge is also inserted between 
    // the particle and all its daughters.
    for (std::vector<int*>::size_type j = 0; j < CandDauIdx.size(); j++) {
      if (CandDauIdx[j][i] == -1) {
        break;
      } else {

        int v_idx = reco_indexer.get_reco_idx(CandDauLund[j][i], CandDauIdx[j][i]);
        tie(pos, inserted) = reco_vertex_map.insert(std::make_pair(v_idx, Vertex()));
        if (inserted) {
          v = add_vertex(g);
          reco_index[v] = v_idx;
          block_index[v] = CandDauIdx[j][i];
          lund_id[v] = CandDauLund[j][i];
          reco_vertex_map[v_idx] = v;
        } else {
          v = reco_vertex_map[v_idx];
        }

        add_edge(u, v, g);
      }
    }
  }
}
