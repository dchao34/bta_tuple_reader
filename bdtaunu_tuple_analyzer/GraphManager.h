#ifndef __GRAPHMANAGER_H_
#define __GRAPHMANAGER_H_

#include <iostream>

//! Abstract base class for GraphManagers.
class GraphManager {

  public:
    virtual ~GraphManager() {}
    virtual void construct_graph() = 0;
    virtual void analyze_graph() = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual void clear() = 0;

};

#endif
