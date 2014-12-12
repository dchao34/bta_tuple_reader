#ifndef __GRAPHWRITER_H_
#define __GRAPHWRITER_H_

/** @file BDtaunuGraphWriter.h
 *  @brief Classes for generating graphviz output. 
 *
 *  @detail 
 *  This file contains classes that are used to generate graphviz output
 *  for the Monte Carlo and Reconstructed particle graphs.
 *
 *  The main class that users interact with is `BDtaunuGraphvizManager`. 
 *  You configure the vertex and edge properties and then ask it to 
 *  construct the corresponding writer object. 
 *
 *  #Usage Example
 *  Let's say we want to generate graphviz output for a truth matched
 *  reconstructed particle graph, and want to write the dot code to the 
 *  object `ostream os`. 
 *
 *      // Construct the graphviz manager, `gv_manager`. 
 *      auto lund_pm = get(vertex_lund_id, reco_graph);
 *      auto reco_idx_pm = get(vertex_reco_index, reco_graph);
 *      BDtaunuGraphvizManager<decltype(reco_graph), decltype(lund_pm), decltype(reco_idx_pm)> gv_manager(
 *          reco_graph, lund_pm, reco_idx_pm, BDtaunuMcReader::lund_to_name, truth_match);
 *
 *      // Configure graph properties. 
 *      gv_manager.set_title("Reco Graph with Truth Match");
 *      gv_manager.set_vertex_property({"color", "red"});
 *      gv_manager.set_vertex_property({"style", "filled"});
 *      gv_manager.set_vertex_property({"fillcolor", "white"});
 *
 *      gv_manager.set_tm_vertex_property({"color", "red"});
 *      gv_manager.set_tm_vertex_property({"penwidth", "3"});
 *      gv_manager.set_tm_vertex_property({"style", "filled"});
 *      gv_manager.set_tm_vertex_property({"fillcolor", "lightskyblue"});
 *
 *      gv_manager.set_edge_property({"color", "grey"});
 *
 *      gv_manager.set_tm_edge_property({"color", "black"});
 *      gv_manager.set_tm_edge_property({"weight", "1"});
 *      gv_manager.set_tm_edge_property({"penwidth", "3"});
 *
 *      // Use the ``boost::write_graphviz`` to generate the output. 
 *       boost::write_graphviz(
 *         os, reco_graph, 
 *         gv_manager.construct_vertex_writer(),
 *         gv_manager.construct_edge_writer(),
 *         gv_manager.construct_graph_writer());
 */
#include <iostream> 
#include <fstream> 
#include <map> 
#include <string> 
#include <cassert> 
#include <initializer_list> 

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

/** @brief This is the vertex writer. 
 *
 * @detail The normal use case is for users to construct this object 
 * through `BDtaunuGraphvizManager`.
 */ 
template <typename Graph, typename LundPM, typename IdxPM>
class BDtaunuVertexWriter {

  private:
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

  public:
    BDtaunuVertexWriter(
        const Graph &_graph, 
        const LundPM &_lund_pm, 
        const IdxPM &_idx_pm,
        const std::map<int, std::string> &_lund_map, 
        const std::map<int, int> &_tm_map,
        const std::map<std::string, std::string> &_vertex_properties, 
        const std::map<std::string, std::string> &_tm_vertex_properties) :
        g(_graph), 
        lund_pm(_lund_pm),
        idx_pm(_idx_pm),
        lund_map(_lund_map),
        tm_map(_tm_map), 
        vertex_properties(_vertex_properties),
        tm_vertex_properties(_tm_vertex_properties) {}
    virtual ~BDtaunuVertexWriter() {};

    void operator()(std::ostream &out, const Vertex &v) const;

  private:
    Graph g;
    LundPM lund_pm;
    IdxPM idx_pm;
    std::map<int, std::string> lund_map;
    std::map<int, int> tm_map;
    std::map<std::string, std::string> vertex_properties;
    std::map<std::string, std::string> tm_vertex_properties;
};

template <typename Graph, typename LundPM, typename IdxPM> 
void BDtaunuVertexWriter<Graph, LundPM, IdxPM>::operator() (
    std::ostream &out, const Vertex &v) const {

  std::string label = std::to_string(idx_pm[v]) + ": ";
  auto lund_it = lund_map.find(lund_pm[v]);
  (lund_it != lund_map.end()) ? label += lund_it->second : label += std::to_string(lund_pm[v]);

  out << "[label=\"" <<  label << "\"";

  const auto *vtx_prop = &vertex_properties;
  auto tm_it = tm_map.find(idx_pm[v]);
  if (tm_it != tm_map.end() && tm_it->second >= 0) vtx_prop = &tm_vertex_properties;
  for (auto p : *vtx_prop) {
    out << ", " << p.first << "=\"" << p.second << "\"";
  }
  out << "]";

}


/** @brief This is the edge writer. 
 *
 * @detail The normal use case is for users to construct this object 
 * through `BDtaunuGraphvizManager`.
 */ 
template <typename Graph, typename LundPM, typename IdxPM>
class BDtaunuEdgeWriter {

  private:
    typedef typename boost::graph_traits<Graph>::edge_descriptor Edge;

  public:
    BDtaunuEdgeWriter(
        const Graph &_graph, 
        const LundPM &_lund_pm, 
        const IdxPM &_idx_pm,
        const std::map<int, int> &_tm_map,
        const std::map<std::string, std::string> &_edge_properties, 
        const std::map<std::string, std::string> &_tm_edge_properties) :
        g(_graph), 
        lund_pm(_lund_pm), 
        idx_pm(_idx_pm), 
        tm_map(_tm_map), 
        edge_properties(_edge_properties),
        tm_edge_properties(_tm_edge_properties) {}

    void operator()(std::ostream &out, const Edge &v) const;

  private:
    Graph g;
    LundPM lund_pm;
    IdxPM idx_pm;
    std::map<int, int> tm_map;
    std::map<std::string, std::string> edge_properties;
    std::map<std::string, std::string> tm_edge_properties;
};

template <typename Graph, typename LundPM, typename IdxPM>
void BDtaunuEdgeWriter<Graph, LundPM, IdxPM>::operator() (
    std::ostream &out, const Edge &e) const {

  const auto *edge_prop = &edge_properties;

  auto u = source(e, g);
  auto v = target(e, g);
  std::map<int, int>::const_iterator u_tm_it, v_tm_it;
  u_tm_it = tm_map.find(idx_pm[u]);
  v_tm_it = tm_map.find(idx_pm[v]);
  if (
      (u_tm_it != tm_map.end() && u_tm_it->second >= 0) &&
      (v_tm_it != tm_map.end() && v_tm_it->second >= 0) 
     ) {
    edge_prop = &tm_edge_properties;
  } 

  out << "[";
  auto it = edge_prop->begin();
  while (it != edge_prop->end()) {
    if (it != edge_prop->begin()) out << ", ";
    out << it->first << "=\"" << it->second << "\"";
    ++it;
  }
  out << "]";

}

/** @brief This is the graph writer. 
 *
 * @detail The normal use case is for users to construct this object 
 * through `BDtaunuGraphvizManager`.
 */ 
class BDtaunuGraphWriter {

  public:
    BDtaunuGraphWriter(std::string _title = "") : title(_title) {}
    void operator()(std::ostream &out) const {
      if (!title.empty()) {
        out << "graph[fontsize=\"32\"]" << std::endl;
        out << "labelloc=\"t\"" << std::endl;
        out << "label = " << "\"" << title << "\"" << std::endl;
      }
    };
  private:
    std::string title;
};

/** @brief This is the graphviz manager. 
 *
 * @detail 
 *
 * One constructs an instance of this class and use the `set` methods 
 * to configure the desired graph properties. 
 *
 * To write the output, one can construct the specific element writer 
 * for feeding into ``boost::write_graphviz``. 
 *
 * See the file BDtaunuGraphWriter.h for example usage. 
 */ 
template <typename Graph, typename LundPM, typename IdxPM>
class BDtaunuGraphvizManager {

  // API
  // ---
  
  public:

    // Constructor
    BDtaunuGraphvizManager(
      const Graph &_graph, 
      const LundPM &_lund_pm, 
      const IdxPM &_idx_pm,
      const std::map<int, std::string> &_lund_map, 
      std::map<int, int> _tm_map = std::map<int, int>()) : 
      g(_graph), 
      lund_pm(_lund_pm),
      idx_pm(_idx_pm),
      lund_map(_lund_map), 
      tm_map(_tm_map) {};
    BDtaunuGraphvizManager() = default;
    BDtaunuGraphvizManager(const BDtaunuGraphvizManager&) = default;
    BDtaunuGraphvizManager& operator=(const BDtaunuGraphvizManager&) = default;
    ~BDtaunuGraphvizManager() = default;

    //! Set vertex property with { "property-name" : "property-value" }.
    void set_vertex_property(std::initializer_list<std::string> l) { 
      SetElementProperty(vertex_properties, l);
    }

    //! Set truth matched vertex property with { "property-name" : "property-value" }.
    void set_tm_vertex_property(std::initializer_list<std::string> l) { 
      SetElementProperty(tm_vertex_properties, l);
    }

    //! Set edge property with { "property-name" : "property-value" }.
    void set_edge_property(std::initializer_list<std::string> l) { 
      SetElementProperty(edge_properties, l);
    }

    //! Set truth matched edge property with { "property-name" : "property-value" }.
    void set_tm_edge_property(std::initializer_list<std::string> l) { 
      SetElementProperty(tm_edge_properties, l);
    }

    //! Set graph title. 
    void set_title(std::string _title) { title = _title; }

    //! Construct and return the copy of the vertex writer. 
    BDtaunuVertexWriter<Graph, LundPM, IdxPM> construct_vertex_writer() const {
      return BDtaunuVertexWriter<Graph, LundPM, IdxPM>(
          g, lund_pm, idx_pm, lund_map, tm_map, 
          vertex_properties, tm_vertex_properties);
    }

    //! Construct and return the copy of the edge writer. 
    BDtaunuEdgeWriter<Graph, LundPM, IdxPM> construct_edge_writer() const {
      return BDtaunuEdgeWriter<Graph, LundPM, IdxPM>(
          g, lund_pm, idx_pm, tm_map, edge_properties, tm_edge_properties);
    }

    //! Construct and return a copy of the graph writer. 
    BDtaunuGraphWriter construct_graph_writer() const {
      return BDtaunuGraphWriter(title);
    }

  private:
    Graph g;
    LundPM lund_pm;
    IdxPM idx_pm;
    std::map<int, std::string> lund_map;
    std::map<int, int> tm_map;
    std::string title;
    std::map<std::string, std::string> vertex_properties;
    std::map<std::string, std::string> tm_vertex_properties;
    std::map<std::string, std::string> edge_properties;
    std::map<std::string, std::string> tm_edge_properties;

    void SetElementProperty(
        std::map<std::string, std::string> &elem_map, 
        const std::initializer_list<std::string> &prop);
};

template <typename Graph, typename LundPM, typename IdxPM>
void BDtaunuGraphvizManager<Graph, LundPM, IdxPM>::SetElementProperty(
    std::map<std::string, std::string> &elem_map, 
    const std::initializer_list<std::string> &property) {
  assert(property.size() == 2);
  auto it = property.begin();
  std::string key = *it++;
  std::string value = *it;
  elem_map[key] = value;
}

#endif
