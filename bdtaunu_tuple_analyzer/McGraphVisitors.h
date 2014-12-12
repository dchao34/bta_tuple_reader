#ifndef __MCGRAPHVISITOR_H__
#define __MCGRAPHVISITOR_H__

/** @file McGraphVisitors.h
 *
 * @brief BGL visitors used to analyze Monte Carlo truth particle graph.
 * 
 * @detail 
 * This file defines the following visitors:
 * * McGraphDfsVisitor: 
 * Subclass of [dfs_visitor](http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/dfs_visitor.html).
 */

#include <boost/graph/depth_first_search.hpp>

#include "BDtaunuDef.h"
#include "GraphDef.h"

class McGraphManager;

/** @brief A DfsVisitor class that computes information associated with 
 * MC truth particles. 
 *
 * @detail 
 * #Purpose
 *
 * This class computes analysis information associated with MC truth particles.
 * See Particles.h for the quantities computed. 
 *
 * Since most quantities about a reconstructed particle can only be 
 * determined after all its daughter particles have been analyzed, 
 * [depth first search](http://en.wikipedia.org/wiki/Depth-first_search "DFS)
 * is the preferred method of traversing the graph. Quantities about 
 * a particle is computed only when its associated vertex
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
 * the cache that its supervising class (See McGraphManager.h) manages.
 */
class McGraphDfsVisitor : public boost::default_dfs_visitor {

  public:
    McGraphDfsVisitor() = default;
    McGraphDfsVisitor(McGraphManager*);
    ~McGraphDfsVisitor() {};

    void finish_vertex(McGraph::Vertex u, const McGraph::Graph &g);

  private:
    static const bdtaunu::McBTypeCatalogue mcB_catalogue;

  private:
    McGraphManager *manager = nullptr;
    McGraph::LundIdPropertyMap lund_map;

    void AnalyzeY(const McGraph::Vertex &u, const McGraph::Graph &g);
    void AnalyzeB(const McGraph::Vertex &u, const McGraph::Graph &g);
    void AnalyzeTau(const McGraph::Vertex &u, const McGraph::Graph &g);
};

#endif
