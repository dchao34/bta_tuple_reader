#include <iostream> 
#include <chrono>

#include <bdtaunu_tuple_analyzer/BDtaunuMcReader.h>

using namespace std;

int main() {

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();

  BDtaunuMcReader reader("/Users/dchao/bdtaunu/v4/data/root/signal/aug_12_2014/A/sp11444r1.root");
  int nevents = 0;
  while (reader.next_record() != RootReader::Status::kEOF) {
    nevents += 1;
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  cout << "processed " << nevents << " events in " << elapsed_seconds.count() << " seconds." << endl;

  return 0;
}
