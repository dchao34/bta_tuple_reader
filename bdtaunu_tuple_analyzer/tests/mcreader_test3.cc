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

  map<int, string> d_map;
  d_map.insert(make_pair(1, "D+ -> K pi pi"));
  d_map.insert(make_pair(2, "D+ -> K pi pi pi0"));
  d_map.insert(make_pair(3, "D+ -> Ks K"));
  d_map.insert(make_pair(4, "D+ -> Ks pi"));
  d_map.insert(make_pair(5, "D+ -> Ks pi pi0"));
  d_map.insert(make_pair(6, "D+ -> Ks pi pi pi"));
  d_map.insert(make_pair(7, "D+ -> K K pi"));
  d_map.insert(make_pair(8, "D0 -> K pi"));
  d_map.insert(make_pair(9, "D0 -> K pi pi0"));
  d_map.insert(make_pair(10, "D0 -> K pi pi pi"));
  d_map.insert(make_pair(11, "D0 -> K pi pi pi pi0"));
  d_map.insert(make_pair(12, "D0 -> Ks pi pi"));
  d_map.insert(make_pair(13, "D0 -> Ks pi pi pi0"));
  d_map.insert(make_pair(14, "D0 -> Ks pi0"));
  d_map.insert(make_pair(15, "D0 -> K K"));

  map<int, string> dstar_map;
  dstar_map.insert(make_pair(0, "No D*"));
  dstar_map.insert(make_pair(1, "D*0 -> D0 pi0"));
  dstar_map.insert(make_pair(2, "D*0 -> D0 gamma"));
  dstar_map.insert(make_pair(3, "D*c -> D0 pi"));
  dstar_map.insert(make_pair(4, "D*c -> Dc pi0"));
  dstar_map.insert(make_pair(5, "D*c -> Dc gamma"));

  map<int, string> tau_map;
  tau_map.insert(make_pair(0, "No tau"));
  tau_map.insert(make_pair(1, "tau -> pi"));
  tau_map.insert(make_pair(2, "tau -> rho"));
  tau_map.insert(make_pair(3, "tau -> e"));
  tau_map.insert(make_pair(4, "tau -> mu"));

  map<int, string> tau_mc_map;
  tau_mc_map.insert(make_pair(0, "No tau"));
  tau_mc_map.insert(make_pair(1, "tau -> e"));
  tau_mc_map.insert(make_pair(2, "tau -> mu"));
  tau_mc_map.insert(make_pair(3, "tau -> k"));
  tau_mc_map.insert(make_pair(4, "tau -> h"));

  map<int, string> bmc_map;
  bmc_map.insert(make_pair(0, "Continuum"));
  bmc_map.insert(make_pair(1, "B -> D tau"));
  bmc_map.insert(make_pair(2, "B -> D* tau"));
  bmc_map.insert(make_pair(3, "B -> D l"));
  bmc_map.insert(make_pair(4, "B -> D* l"));
  bmc_map.insert(make_pair(5, "B -> D**_res"));
  bmc_map.insert(make_pair(6, "B -> D**_nonres"));
  bmc_map.insert(make_pair(7, "B -> SL"));
  bmc_map.insert(make_pair(8, "B -> Had"));

  ofstream reco_output;
  ofstream mc_output;

  // Generic tests
  // -----------

  cout << "Generic MC tests: " << endl;
  cout << endl;

  // sp1235r1
  // ---------
  cout << "sp1235r1: " << endl;
  cout << endl;

  // Event 0
  // -------
  BDtaunuMcReader *reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/generic/aug_12_2014/sp1235r1.root");
  reader->next_record();
  vector<UpsilonCandidate> upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 0):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp1235r1.0.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp1235r1.0.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  // Event 1
  // -------
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 1):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp1235r1.1.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp1235r1.1.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  delete reader;

  // sp1237r1
  // ---------
  cout << "sp1237r1: " << endl;
  cout << endl;

  // Event 0
  // -------
  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/generic/aug_12_2014/sp1237r1.root");
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 0):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp1237r1.0.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp1237r1.0.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  // Event 1
  // -------
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 1):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp1237r1.1.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp1237r1.1.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  delete reader;

  // sp1005r1
  // ---------
  cout << "sp1005r1: " << endl;
  cout << endl;

  // Event 0
  // -------
  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/generic/aug_12_2014/sp1005r1.root");
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 0):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp1005r1.0.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp1005r1.0.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  // Event 1
  // -------
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 1):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp1005r1.1.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp1005r1.1.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  delete reader;

  // sp998r1
  // ---------
  cout << "sp998r1: " << endl;
  cout << endl;

  // Event 0
  // -------
  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/generic/aug_12_2014/sp998r1.root");
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 0):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp998r1.0.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp998r1.0.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  // Event 1
  // -------
  reader->next_record();
  upsilons = reader->get_upsilon_candidates();

  cout << "\tEvent " << reader->get_eventId() << " (alias 1):" << endl;
  cout << endl;
  cout << "\t\tB1McType = " << bmc_map[static_cast<int>(reader->get_b1_mctype())] << endl;
  cout << "\t\tB2McType = " << bmc_map[static_cast<int>(reader->get_b2_mctype())] << endl;
  cout << "\t\tB1TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b1_tau_mctype())] << endl;
  cout << "\t\tB2TauMcType = " << tau_mc_map[static_cast<int>(reader->get_b2_tau_mctype())] << endl;
  cout << endl;

  cout << "\t\t# of Y(4S) candidates = " << upsilons.size() << endl;
  cout << endl;
  cout << "\t\tCandidate 0: " << endl;
  cout << "\t\t\ttag D mode: " << d_map[static_cast<int>(upsilons[0].get_tag_d_mode())] << endl;
  cout << "\t\t\ttag Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_tag_dstar_mode())] << endl;
  cout << "\t\t\tsig D mode: " << d_map[static_cast<int>(upsilons[0].get_sig_d_mode())] << endl;
  cout << "\t\t\tsig Dstar mode: " << dstar_map[static_cast<int>(upsilons[0].get_sig_dstar_mode())] << endl;
  cout << "\t\t\tsig tau mode: " << tau_map[static_cast<int>(upsilons[0].get_sig_tau_mode())] << endl;
  cout << endl;

  reco_output.open("sp998r1.1.reco.gv");
  reader->print_reco_graph(reco_output);
  reco_output.close();

  mc_output.open("sp998r1.1.mc.gv");
  reader->print_mc_graph(mc_output);
  mc_output.close();

  delete reader;

  return 0;
}
