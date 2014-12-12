#ifndef __RECOGRAPHVISITOR_H__
#define __RECOGRAPHVISITOR_H__

/** @file RecoGraphVisitors.h
 *
 * @brief BGL visitors used to analyze reconstructed particle graph.
 * 
 * @detail 
 * This file defines the following visitors:
 * * RecoGraphDfsVisitor: 
 * Subclass of [dfs_visitor](http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/dfs_visitor.html).
 */
#include <boost/graph/depth_first_search.hpp>

#include "BDtaunuDef.h"
#include "GraphDef.h"

class RecoGraphManager;

/** @brief A DfsVisitor class that computes information associated with 
 * reconstructed particles. 
 *
 * @detail 
 * #Purpose
 *
 * This class computes analysis information associated with particular
 * types of reconstructed particles. See GraphDef.h for the quantities 
 * computed. 
 *
 * Since most quantities about a reconstructed particle can only be 
 * determined after all its daughter particles have been analyzed, 
 * [depth first search](http://en.wikipedia.org/wiki/Depth-first_search "DFS)
 * is the preferred method of traversing the graph. Quantities about 
 * a reconstructed particle is computed only when its associated vertex
 * is colored black. 
 *
 * # Implementation
 * This class is a subclass of
 * [dfs_visitor](http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/dfs_visitor.html), 
 * and implements the `finish_vertex()` method. 
 *
 * Every time a vertex is colored black, the visitor checks whether it is 
 * a particle type that we are interested in analyzing. If so, it calls
 * the corresponding `AnalyzeX()` method and puts the computed result in 
 * the cache that its supervising class (See RecoGraphManager.h) manages.
 */
class RecoGraphDfsVisitor : public boost::default_dfs_visitor {

  public:
    RecoGraphDfsVisitor() = default;
    RecoGraphDfsVisitor(RecoGraphManager*);
    ~RecoGraphDfsVisitor() {};

    void finish_vertex(RecoGraph::Vertex u, const RecoGraph::Graph &g);

  private:
    static const bdtaunu::RecoDTypeCatalogue recoD_catalogue;

  private:
    RecoGraphManager *manager = nullptr;
    RecoGraph::LundIdPropertyMap lund_map;
    RecoGraph::BlockIndexPropertyMap block_idx_map;

    void AnalyzeY(const RecoGraph::Vertex &u, const RecoGraph::Graph &g);
    void AnalyzeB(const RecoGraph::Vertex &u, const RecoGraph::Graph &g);
    void AnalyzeDstar(const RecoGraph::Vertex &u, const RecoGraph::Graph &g);
    void AnalyzeD(const RecoGraph::Vertex &u, const RecoGraph::Graph &g);
    void AnalyzeLepton(const RecoGraph::Vertex &u, const RecoGraph::Graph &g);
};

#endif
