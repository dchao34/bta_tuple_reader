#ifndef __GRAPHDEF_H_
#define __GRAPHDEF_H_

#include <initializer_list>
#include <boost/graph/adjacency_list.hpp>

#include "BDtaunuDef.h"

/** @file GraphDef.h
 *  @brief Definition of graph quantities used in the tuple analysis. 
 *
 *  Several types of quantities are defined in this file:
 *  * Boost graph typedefs.
 *  * Graph satallite data. e.g. particle satellite data attached to vertices. 
 *
 */

namespace boost {
  enum vertex_lund_id_t { vertex_lund_id };
  BOOST_INSTALL_PROPERTY(vertex, lund_id);
}

namespace boost {
  enum vertex_reco_index_t { vertex_reco_index };
  enum vertex_block_index_t { vertex_block_index };
  BOOST_INSTALL_PROPERTY(vertex, reco_index);
  BOOST_INSTALL_PROPERTY(vertex, block_index);
}

namespace boost {
  enum vertex_mc_index_t { vertex_mc_index };
  BOOST_INSTALL_PROPERTY(vertex, mc_index);
}

//! Quantities for the reconstructed particle graph. 
namespace RecoGraph {

// Boost graph typedefs
typedef boost::adjacency_list<
  boost::vecS, boost::vecS, boost::bidirectionalS,
  boost::property<boost::vertex_reco_index_t, int,
  boost::property<boost::vertex_block_index_t, int,
  boost::property<boost::vertex_lund_id_t, int>>>,
  boost::property<boost::edge_index_t, int>
> Graph;

typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

typedef typename boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;

typedef typename boost::property_map<Graph, boost::vertex_lund_id_t>::type LundIdPropertyMap;
typedef typename boost::property_map<Graph, boost::vertex_reco_index_t>::type RecoIndexPropertyMap;
typedef typename boost::property_map<Graph, boost::vertex_block_index_t>::type BlockIndexPropertyMap;

//! Data attached to vertices of reconstructed leptons. 
/** Leptons can be actual leptons or placeholder tau leptons 
 * which are either \f$\pi\f$ or \f$\rho\f$ mesons. 
 */
struct Lepton {
  int l_block_idx = -1;
  int pi_block_idx = -1;
  bdtaunu::TauType tau_mode = bdtaunu::TauType::null;
};

//! Data attached to vertices of reconstructed \f$D/D^*\f$ mesons. 
/** This stores information for both \f$D\f$ and \f$D^*\f$ mesons. 
 * For \f$D^*\f$'s all entries are filled. For \f$D\f$'s, the `Dstar_mode`
 * entry will indicate `NoDstar` as in RecoDTypeCatalogue::DstarType.
 */
struct D {
  D() = default;
  bdtaunu::RecoDTypeCatalogue::DType D_mode 
    = bdtaunu::RecoDTypeCatalogue::DType::null;
  bdtaunu::RecoDTypeCatalogue::DstarType Dstar_mode 
    = bdtaunu::RecoDTypeCatalogue::DstarType::NoDstar;
};

//! Data attached to vertices of reconstructed \f$B\f$ mesons. 
struct B {
  B() = default;
  bdtaunu::BFlavor flavor = bdtaunu::BFlavor::null;
  D *D = nullptr;
  Lepton *Lepton = nullptr;
};

//! Data attached to vertices of reconstructed \f$\Upsilon(4S)\f$ mesons. 
struct Y {
  Y() = default;
  B *tagB = nullptr;
  B *sigB = nullptr;
};


/** @brief This class assigns a unique index to each reconstructed particle.
 *
 * @detail 
 * # Motivation
 * The need for every reconstructed particle of the event to have a unique
 * index arises primarily in accessessing vertex information in the BGL graph. 
 * However, such an indexing is more convenient to use than the existing 
 * BtaTupleMaker indexing. 
 *
 * # Implementation
 * This class builds a hash function from the reco particle's index in the 
 * BtaTupleMaker block to a unique index. 
 *
 * See RecoIndexer.cc for details.
 */
class RecoIndexer {

  private:
    int nY, nB, nD, nC, nh, nl, ngamma;

  public:
    RecoIndexer();
    RecoIndexer(int _nY, int _nB, int _nD, 
                int _nC, int _nh, int _nl, int _ngamma);
    RecoIndexer(const RecoIndexer &r) = default;
    RecoIndexer &operator=(const RecoIndexer &r) = default;
    ~RecoIndexer() {};

    //! Given the lundId and block index, return the unique reco index.
    int get_reco_idx(int lund, int block_idx) const;

    //! Given the reco index, decide if it is a h candidate.
    bool is_h_candidate(int reco_index) const;

    //! Given the reco index, decide if it is a l candidate.
    bool is_l_candidate(int reco_index) const;

    //! Given the reco index, decide if it is a gamm candidate.
    bool is_gamma_candidate(int reco_index) const;

    //! Return total number of reco particles in this event. 
    int total() const { return nY + nB + nD + nC + nh + nl + ngamma; }

    //! Set the total number of each type of reco particle.
    /*! The list order is { nY, nB, nD, nC, nh, nl, ngamma } */
    void set(std::initializer_list<int> l);

    //! Clear cache.
    void clear();
};

}


//! Quantities for the MC truth particle graph. 
namespace McGraph {

// Boost graph typedefs
typedef boost::adjacency_list<
  boost::vecS, boost::vecS, boost::bidirectionalS,
  boost::property<boost::vertex_mc_index_t, int,
  boost::property<boost::vertex_lund_id_t, int>>,
  boost::property<boost::edge_index_t, int>
> Graph;

typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

typedef typename boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;

typedef typename boost::property_map<Graph, boost::vertex_lund_id_t>::type LundIdPropertyMap;
typedef typename boost::property_map<Graph, boost::vertex_mc_index_t>::type McIndexPropertyMap;

//! Data attached to vertices of \f$\tau\f$ leptons. 
struct Tau {
  Tau() = default;
  bdtaunu::TauMcType mc_type = bdtaunu::TauMcType::null;
};

//! Data attached to vertices of \f$B\f$ mesons. 
struct B {
  B() = default;
  bdtaunu::BFlavor flavor = bdtaunu::BFlavor::null;
  bdtaunu::McBTypeCatalogue::BMcType mc_type 
    = bdtaunu::McBTypeCatalogue::BMcType::null;
  Tau *tau = nullptr;
};

//! Data attached to vertices of \f$\Upsilon(4S)\f$ mesons. 
struct Y {
  Y() = default;
  bool isBBbar = true;
  B *B1 = nullptr;
  B *B2 = nullptr;
};

}

#endif
