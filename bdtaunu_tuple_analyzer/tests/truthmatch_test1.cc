#include <iostream> 
#include <fstream> 
#include <vector> 
#include <string> 
#include <map> 
#include <utility> 
#include <cassert> 

#include <bdtaunu_tuple_analyzer/BDtaunuDef.h>
#include <bdtaunu_tuple_analyzer/BDtaunuMcReader.h>
#include <bdtaunu_tuple_analyzer/UpsilonCandidate.h>

using namespace std;

int main() {
  BDtaunuMcReader reader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11444r1.root");
  int nTM = 0, nevents = 0;
  while (reader.next_record() != RootReader::Status::kEOF) {
    ++nevents;
    vector<UpsilonCandidate> upsilons = reader.get_upsilon_candidates();
    for (auto ups : upsilons) {
      if (ups.get_truth_match() >= 0) ++nTM;
      break;
    }
  }
  cout << nTM << " / " << nevents << endl;
  return 0;
}
