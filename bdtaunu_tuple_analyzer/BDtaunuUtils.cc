#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <map>

#include "BDtaunuUtils.h"

// Reads in __PDT_FILE_PATHNAME which contains one particle type on
// each line in the format: "particle name" lundId, and puts the
// information into a map with particle name : lundId.
std::map<std::string, int> bdtaunu::NameToLundMap() {
  std::map<std::string, int> m;

  std::ifstream pdt_file;
  pdt_file.open(__PDT_FILE_PATHNAME);
  if (!pdt_file.is_open()) {
    std::stringstream ss;
    ss << "\nError: In file " << __FILE__ << " line " << __LINE__ << ": \n"; 
    ss << "File not found: " << __PDT_FILE_PATHNAME << "\n";
    throw std::ios_base::failure(ss.str());
  }

  // Scan each line from the first character until the first space for
  // the particle name. The remainder of the line is the lundId. 
  std::string line, lundId_string, particle_name;
  while (std::getline(pdt_file, line)) {
    for (std::string::size_type i = 0; i != line.size(); ++i) {
      if (std::isspace(line[i])) {
        lundId_string = line.substr(i + 1);
        particle_name = line.substr(0, i);
        m[particle_name] = std::stoi(lundId_string);
        break;
      }
    }
  }

  pdt_file.close();

  assert(!m.empty());
  assert(m["B0"] == 511);

  return m;
}

// Reads in __PDT_FILE_PATHNAME which contains one particle type on
// each line in the format: "particle name" lundId, and puts the
// information into a map with lundId : particle name
std::map<int, std::string> bdtaunu::LundToNameMap() {
  std::map<int, std::string> m;

  std::ifstream pdt_file;
  pdt_file.open(__PDT_FILE_PATHNAME);
  if (!pdt_file.is_open()) {
    std::stringstream ss;
    ss << "\nError: In file " << __FILE__ << " line " << __LINE__ << ": \n"; 
    ss << "File not found: " << __PDT_FILE_PATHNAME << "\n";
    throw std::ios_base::failure(ss.str());
  }

  // Scan each line from the first character until the first space for
  // the particle name. The remainder of the line is the lundId. 
  std::string line, lundId_string, particle_name;
  while (std::getline(pdt_file, line)) {
    for (std::string::size_type i = 0; i != line.size(); ++i) {
      if (std::isspace(line[i])) {
        lundId_string = line.substr(i + 1);
        particle_name = line.substr(0, i);
        m[std::stoi(lundId_string)] = particle_name;
        break;
      }
    }
  }

  pdt_file.close();

  assert(!m.empty());
  assert(m[511] == "B0");

  return m;
}

