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

  BDtaunuMcReader *reader;

  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11444r1.root");

  reader->next_record();
  reco_output.open("1.reco.gv");
  mc_output.open("1.mc.gv");
  mc_contracted_output.open("1.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("2.reco.gv");
  mc_output.open("2.mc.gv");
  mc_contracted_output.open("2.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("3.reco.gv");
  mc_output.open("3.mc.gv");
  mc_contracted_output.open("3.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("4.reco.gv");
  mc_output.open("4.mc.gv");
  mc_contracted_output.open("4.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  delete reader;

  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11445r1.root");

  reader->next_record();
  reco_output.open("5.reco.gv");
  mc_output.open("5.mc.gv");
  mc_contracted_output.open("5.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("6.reco.gv");
  mc_output.open("6.mc.gv");
  mc_contracted_output.open("6.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("7.reco.gv");
  mc_output.open("7.mc.gv");
  mc_contracted_output.open("7.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("8.reco.gv");
  mc_output.open("8.mc.gv");
  mc_contracted_output.open("8.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  delete reader;

  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11446r1.root");

  reader->next_record();
  reco_output.open("9.reco.gv");
  mc_output.open("9.mc.gv");
  mc_contracted_output.open("9.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("10.reco.gv");
  mc_output.open("10.mc.gv");
  mc_contracted_output.open("10.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("11.reco.gv");
  mc_output.open("11.mc.gv");
  mc_contracted_output.open("11.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("12.reco.gv");
  mc_output.open("12.mc.gv");
  mc_contracted_output.open("12.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  delete reader;

  reader = new BDtaunuMcReader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11447r1.root");

  reader->next_record();
  reco_output.open("13.reco.gv");
  mc_output.open("13.mc.gv");
  mc_contracted_output.open("13.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("14.reco.gv");
  mc_output.open("14.mc.gv");
  mc_contracted_output.open("14.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("15.reco.gv");
  mc_output.open("15.mc.gv");
  mc_contracted_output.open("15.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  reader->next_record();
  reco_output.open("16.reco.gv");
  mc_output.open("16.mc.gv");
  mc_contracted_output.open("16.mc_contracted.gv");
  reader->print_mc_graph(mc_output);
  reader->print_contracted_mc_graph(mc_contracted_output);
  reader->print_truthmatch_reco_graph(reco_output);
  reco_output.close();
  mc_output.close();
  mc_contracted_output.close();

  delete reader;

  return 0;
}
