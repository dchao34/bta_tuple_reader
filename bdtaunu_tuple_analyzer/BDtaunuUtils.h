#ifndef __HELPERS_H__
#define __HELPERS_H__

/*! \file BDtaunuUtils.h
 *  \brief General utility functions. 
 */

#include <string>
#include <map>

namespace bdtaunu {

//! Builds particle name : lundId map
std::map<std::string, int> NameToLundMap();

//! Builds lundId : particle name map
std::map<int, std::string> LundToNameMap();

}

#endif
