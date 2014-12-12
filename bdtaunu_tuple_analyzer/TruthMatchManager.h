#ifndef _TRUTHMATCHMANAGER_H_
#define _TRUTHMATCHMANAGER_H_

#include <map>
#include <boost/graph/depth_first_search.hpp>

#include "GraphDef.h"
#include "RecoGraphManager.h"
#include "McGraphManager.h"

class BDtaunuMcReader;

/**
 * @brief
 * This class manages truth matching.
 *
 * @detail
 * # Truth Match Definition
 * A reconstructed particle \f$R\f$ truth matches to a Monte Carlo truth
 * particle \f$M\f$ if:
 * - Case 1: \f$R\f$ is a final state particle. These are particles that are
 *   detectable directly; e.g. particles that leave hits or deposit energy. 
 *   - The hit pattern \f$R\f$ generates in the detector should agree with
 *     those of \f$M\f$. Use the builtin results provided from Babar. 
 * - Case 2: \f$R\f$ is a composite.
 *   - All daughters of \f$R\f$ must truth match to daughters of \f$M\f$. 
 *   - All \f$M\f$'s daughters are truth matched.
 *   - The particle hypothesis of \f$R\f$ must agree with \f$M\f$'s
 *     identity. 
 *
 * This is currently the only definition for truth matching. One could add
 * varying levels of truth match to indicate how strict the criteria are. 
 * Therefore, the truth match status of a candidate will be returned as 
 * an integer allow for future possibilities of additional definitions.
 *
 * # Implementation Details
 * The detector hit inputs come from information saved with BtaTupleMaker, 
 * which is accessed through `BDtaunuMcReader.h`. This class has direct 
 * access to it. 
 *
 * The truth matching algorithm uses the MC truth graph from `McGraphManager.h`
 * and the reconstructed particle graph from `RecoGraphManager.h`. This 
 * class does not have direct access to these graphs, so it much be 
 * explicitly passed in. 
 *
 * The actual truth matching is outsourced to `TruthMatchDfsVisitor.h`. It has
 * direct write access to private members for reporting the truth match result. 
 *
 */
class TruthMatchManager {

  friend class TruthMatchDfsVisitor;

  // API
  // ---

  public:
    
    // Constructors and copy control
    TruthMatchManager();
    TruthMatchManager(BDtaunuMcReader *reader);
    TruthMatchManager(const TruthMatchManager&) = default;
    TruthMatchManager &operator=(const TruthMatchManager&) = default;
    ~TruthMatchManager() = default;

    //! Given the reco_idx of a reconstructed particle, return its truth match level.
    int get_truth_match_status(int reco_idx) const;

    //! Get a copy of the truth match status map. 
    /*! A map with key : value = reco_idx : truth match level */
    std::map<int, int> get_truth_map() const { return truth_match; }

    //! Update the cached particle graphs to analyze. 
    void update_graph(const RecoGraphManager&, const McGraphManager&);

    //! Analyze cached graphs.
    void analyze_graph();

    //! Print the edge contracted MC graph. 
    void print_mc(std::ostream &os) const;

    //! Print the reco graph with truth matched candidates highlighted. 
    void print_reco(std::ostream &os) const;

  private:

    // Class Members
    // -------------

    // TruthMatchDfsVisitor writes its truth match results to this map. 
    std::map<int, int> truth_match;

    BDtaunuMcReader *reader;
    const int *hMCIdx;
    const int *lMCIdx;
    const int *gammaMCIdx;
    RecoGraph::Graph reco_graph;
    RecoGraph::RecoIndexer reco_indexer;

    // Edge contracted MC graph. `McGraphManager.h` has the original.
    McGraph::Graph mc_graph;

    // Helper Functions
    // ----------------
    void contract_mc_graph();
    bool is_cleave_vertex(
        const McGraph::Vertex &v, 
        const McGraph::Graph &g, 
        const McGraph::LundIdPropertyMap &lund_id_pm,
        const McGraph::McIndexPropertyMap &mc_idx_pm) const;
};


/**
 * @brief
 * This class actually does the truth matching. 
 *
 * @detail
 * This class uses the Boost Graph Library (BGL). 
 *
 * The algorithm traverses the reconstructed particle graph and attempts 
 * to match it to some particle in the edge contracted MC graph. 
 *
 * Depth first search is the natural choice since we can only determine 
 * the truth match status of a reconstructed particle only after we have 
 * determined the truth match status of its daughters.
 *
 * See source file and TruthMatchManager.h other descriptions. 
 *
 */
class TruthMatchDfsVisitor : public boost::default_dfs_visitor {

  public:
    TruthMatchDfsVisitor();
    TruthMatchDfsVisitor(TruthMatchManager*);
    ~TruthMatchDfsVisitor() {};

    void finish_vertex(RecoGraph::Vertex u, const RecoGraph::Graph &g);

  private:
    TruthMatchManager *manager;
    RecoGraph::LundIdPropertyMap reco_lund_pm;
    RecoGraph::RecoIndexPropertyMap reco_idx_pm;
    RecoGraph::BlockIndexPropertyMap block_idx_pm;
    McGraph::LundIdPropertyMap mc_lund_pm;
    McGraph::McIndexPropertyMap mc_idx_pm;

    void MatchFinalState(const RecoGraph::Vertex &u);
    void MatchCompositeState(const RecoGraph::Vertex &u, const RecoGraph::Graph &g);
};

#endif
