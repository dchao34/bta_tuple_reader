#ifndef __RECOGRAPHMANAGER_H_
#define __RECOGRAPHMANAGER_H_

#include <iostream>
#include <vector>
#include <map>

#include "GraphDef.h"
#include "GraphManager.h"
#include "RecoGraphVisitors.h"

class BDtaunuReader;

/** @brief This class builds and analyzes the reconstructed particle 
 * graph using infromation read into BDtaunuReader's buffer. 
 *
 * @detail
 *
 * # Graph Overview
 *
 * The reconstucted particles form a directed graph \f$G=(V,E)\f$:
 * 
 * * \f$V\f$: The reconstructed particles themselves.
 *
 * * \f$E\f$: \f$(u,v)\in E\f$ if \f$v\f$ is a daughter particle of \f$u\f$.
 *
 * # Reconstructed Particle Types
 * 
 * There are a few types of reconstructed particles.
 *
 * * Y: \f$\Upsilon(4S)\f$ candidates. These have only out edges. 
 * * B: \f$B\f$ meson candidates. Depending on its daughters, it is either 
 *      a tag or a signal candidate.
 * * D: \f$D\f$ meson candidates.
 * * C: \f$K_s\f$, \f$\rho^\pm\f$, and \f$\pi^0\f$ candidates.
 * * h: \f$\pi^\pm\f$ and \f$K^\pm\f$ candidates.
 * * l: \f$e\f$ and \f$\mu\f$ candidates.
 * * gamma: \f$\gamma\f$ candidates.
 *
 * # Implementation Details
 *
 * We use the [boost graph library (BGL)](http://www.boost.org/doc/libs/1_56_0/libs/graph/doc/ "BGL")
 * for graph operations.
 *
 * ### `BtaTupleMaker` Input format. 
 *
 * Each event saves the information below for each type of reconstructed particle. Take the Y candidate 
 * for example:
 *
 * * `int nY`: Number of Y candidates.
 *
 * * `int *YLund`: Pointer to an array that holds the Lund ID of each 
 *                 candidate. Array element `i` holds the info to the 
 *                 `i`th candidate. 
 *
 * * `int *Yd1Idx`: Pointer to an array. Element `i` holds the info to the 
 *                  `i`th candidate's first daughter's array element index 
 *                  within its set of candidate arrays. -1 if this candidate 
 *                  does not have this daughter. 
 *
 *                  In general, `Yd`n`Idx` for the n'th daughter. n is 
 *                  hard coded into `BtaTupleMaker`, so you need to go to 
 *                  `BToDTauNuSemiLepHadUser` to look this up or just examine
 *                  the ntuple structure. 
 *
 * * `int *Yd1Lund`: Same idea as `Yd1Idx`, but holds the lund Id of the 
 *                   first daughter. Again, `Yd`n`Lund` in general. 0 if this
 *                   candidate does not have this daughter. 
 *
 *                  ####Example 
 *                  Since each Y candidate always has 2 B candidates by design, 
 *                  we can access the lund Id of the first B daughter of the
 *                  `i`the Y candidate in one of two ways:
 *
 *                      int B1_lundId; 
 *                      assert(Yd1Idx[i] != -1);
 *
 *                      // Method 1
 *                      B1lund_Id = BLund[Yd1Idx[i]];
 *
 *                      // Method 2
 *                      B1lund_Id = Yd1Lund[i];
 *
 * ### BGL graph operations.
 *
 * See the [BGL documentation](http://www.boost.org/doc/libs/1_56_0/libs/graph/doc/table_of_contents.html)
 * for its API. 
 * 
 * #### Graph construction
 * For each type of reconstructed candidate, define two `vector<int*>`'s:
 * * `vector<int*> YdauIdx`: Element `i` stores pointer to the array `YdiIdx`.
 * * `vector<int*> YdauLund`: Element `i` stores pointer to the array `YdiLund`.
 * 
 * To actualy build the graph, call `AddCandidates(...)` for each type 
 * of particle candidate. This function uses BGL's API. 
 *
 * The resulting graph is then cached for further analysis. 
 *
 * #### Graph analysis
 * We use BGL's generic algorithms and visitor classes to analyze the graph. 
 * Since we are often interested in specific reco particles, we cache `map`s
 * of specific particles and its satellite data (see GraphDef.h); the contents of the 
 * `map`s can be reported to the supervising class for analysis. 
 *
 */
class RecoGraphManager : public GraphManager {

  friend class RecoGraphDfsVisitor;

  public:

    // Constructors

    //! Constructor
    /*! Construction of an object should be associated with a 
     * supervising `BDtaunuReader` object. */
    RecoGraphManager(BDtaunuReader*);
    RecoGraphManager();
    RecoGraphManager(const RecoGraphManager&) = default;
    RecoGraphManager &operator=(const RecoGraphManager&) = default;
    ~RecoGraphManager() {};

    //! Construct and cache BGL graph. 
    void construct_graph();

    //! Analyze cached BGL graph. 
    void analyze_graph();

    //! Print graphviz of data of BGL graph to ostream. 
    void print(std::ostream &os) const;

    //! Clear cache. 
    void clear();

    //! Get the unique reco particle index.
    const RecoGraph::RecoIndexer& get_reco_indexer() const { return reco_indexer; }

    //! Get the reco graph
    const RecoGraph::Graph& get_reco_graph() const { return g; }

    //! Access information about the `i`th Y candidate. See GraphDef.h.
    const RecoGraph::Y* get_recoY(int i) const;

  private:

    // Supervising event reader class. 
    BDtaunuReader *reader;

    // Cached BGL graph. 
    RecoGraph::Graph g;

    // Graph construction
    RecoGraph::RecoIndexer reco_indexer;
    std::map<int, RecoGraph::Vertex> reco_vertex_map;
    void ClearGraph();
    void AddCandidates(
        int nCand, int *CandLund,
        std::vector<int*> &CandDauIdx, 
        std::vector<int*> &CandDauLund);

    // Graph analysis
    std::map<RecoGraph::Vertex, RecoGraph::Y> Y_map;
    std::map<RecoGraph::Vertex, RecoGraph::B> B_map;
    std::map<RecoGraph::Vertex, RecoGraph::D> D_map;
    std::map<RecoGraph::Vertex, RecoGraph::Lepton> Lepton_map;
    void ClearAnalysis();
};

#endif
