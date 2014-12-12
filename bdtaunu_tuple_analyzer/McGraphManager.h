#ifndef __MCGRAPHMANAGER_H_
#define __MCGRAPHMANAGER_H_

#include <iostream>
#include <set>
#include <map>

#include "GraphManager.h"
#include "GraphDef.h"
#include "McGraphVisitors.h"

class BDtaunuMcReader;

/** @brief This class builds and analyzes the Monte Carlo truth particle 
 * decay graph using infromation read into BDtaunuMcReader's buffer. 
 *
 * @detail
 *
 * # Graph Overview
 *
 * The MC truth particles form a directed graph \f$G=(V,E)\f$:
 * 
 * * \f$V\f$: The MC truth particles themselves.
 *
 * * \f$E\f$: \f$(u,v)\in E\f$ if \f$v\f$ is a daughter particle of \f$u\f$.
 *
 * # Implementation Details
 *
 * We use the [boost graph library (BGL)](http://www.boost.org/doc/libs/1_56_0/libs/graph/doc/ "BGL")
 * for graph operations.
 *
 * ### `BtaTupleMaker` Input format. 
 *
 * All MC truth information is stored in arrays of length equalling the number of 
 * MC truth particles in the event. Element `i` of the arrays corresponds to the 
 * information about the particle assigned to that index. Call this index the 
 * MC index of the particle. 
 *
 * Each event saves the information below for MC truth particles:
 *
 * * `int mcLen`: Number of MC truth particles.
 *
 * * `int *mcLund`: Pointer to an array that holds the Lund ID of each 
 *                  particle. Array element `i` holds the info to the 
 *                  `i`th particle. 
 *
 * * `int *dauIdx`: Pointer to an array. Element `i` holds the 
 *                  `i`th particles's first daughter's array index.
 *                  -1 if this candidate does not have this daughter. 
 *
 * * `int *mothIdx`: Pointer to an array. Element `i` holds the 
 *                  `i`th particles's mother particle array index.
 *                  -1 if this candidate does not have this daughter. 
 *
 * * `int *dauLen`: Pointer to an array. Element `i` holds the number of daughters
 *                  the `i`th particle has.
 *
 *                  ####Example 
 *                  Here's how to iterate over the daughters of MC particle `i`
 *                  and print their lund Ids:
 *
 *                      for (int j = 0; j < dauLen[i]; j++) {
 *                        cout << mcLund[dauIdx[i] + j] << endl;
 *                      }
 *
 *
 * ### BGL graph operations.
 *
 * See the [BGL documentation](http://www.boost.org/doc/libs/1_56_0/libs/graph/doc/table_of_contents.html)
 * for its API. 
 * 
 * #### Graph construction
 * This is more straight forward than the reco particle case. See source for details.
 *
 * The resulting graph is cached for further analysis. 
 *
 * #### Graph analysis
 * We use BGL's generic algorithms and visitor classes to analyze the graph. 
 *
 * The methodology is very similar to those for the reco particle graph. 
 *
 */
class McGraphManager : public GraphManager {

  friend bool is_final_state_particle(int lund);
  friend class McGraphDfsVisitor;

  public:

    // Constructors 
    
    //! Constructor
    /*! Construction of an object should be associated with a 
     * supervising `BDtaunuReader` object. */
    McGraphManager(BDtaunuMcReader*);
    McGraphManager();
    McGraphManager(const McGraphManager&) = default;
    McGraphManager &operator=(const McGraphManager&) = default;
    ~McGraphManager() {};

    //! Construct and cache BGL graph. 
    void construct_graph();

    //! Analyze cached BGL graph. 
    void analyze_graph();

    //! Print graphviz of data of BGL graph to ostream. 
    void print(std::ostream &os) const;

    //! Clear cache. 
    void clear();

    //! Get the mc graph
    const McGraph::Graph& get_mc_graph() const { return g; }

    //! Returns pointer to the MC truth \f$\Upsilon(4S)\f$ if it exists, nullptr otherwise.
    const McGraph::Y* get_mcY() const;

    //! Returns pointer to one of the MC truth \f$B\f$ if it exists, nullptr otherwise.
    const McGraph::B* get_mcB1() const;

    //! Returns pointer to the other MC truth \f$B\f$ if it exists, nullptr otherwise.
    const McGraph::B* get_mcB2() const;

  private:

    // Set of particle types considered to be final state. 
    static const std::set<int> final_state_particles;

  private:

    // Supervising event reader class. 
    BDtaunuMcReader *reader;

    // Cached BGL graph.
    McGraph::Graph g;

    // Graph construction
    std::map<int, McGraph::Vertex> mc_vertex_map;
    void ClearGraph();

    // Graph analysis
    std::map<McGraph::Vertex, McGraph::Y> Y_map;
    std::map<McGraph::Vertex, McGraph::B> B_map;
    std::map<McGraph::Vertex, McGraph::Tau> Tau_map;
    void ClearAnalysis();

};

//! Decides if a particle is one of the final states.
bool is_final_state_particle(int lund);

#endif
