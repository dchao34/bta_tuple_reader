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
  ofstream reco_output;
  ofstream mc_output;
  ofstream mc_contracted_output;

  BDtaunuMcReader reader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11444r1.root");

  reader.next_record();
  reco_output.open("1.reco.gv");
  mc_output.open("1.mc.gv");
  mc_contracted_output.open("1.mc_contracted.gv");
  reader.print_mc_graph(mc_output);
  reader.print_contracted_mc_graph(mc_contracted_output);
  reader.print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader.next_record();

  reader.next_record();
  reco_output.open("2.reco.gv");
  mc_output.open("2.mc.gv");
  mc_contracted_output.open("2.mc_contracted.gv");
  reader.print_mc_graph(mc_output);
  reader.print_contracted_mc_graph(mc_contracted_output);
  reader.print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();
  map<int, int> truth_map = reader.get_truth_map();
  auto it = truth_map.begin();
  auto it_end = truth_map.end();
  while (it != it_end) {
    cout << it->first << " : " << it->second << endl;
    ++it;
  }
  vector<UpsilonCandidate> upsilons = reader.get_upsilon_candidates();
  cout << upsilons[1].get_reco_index() << endl;
  cout << upsilons[1].get_truth_match() << endl;
  return 0;
}
