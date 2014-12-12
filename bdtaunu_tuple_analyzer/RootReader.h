#ifndef __ROOTREADER_H__
#define __ROOTREADER_H__

#include <TFile.h>
#include <TTree.h>

//! Abstract base class that opens a TFile and gets a TTree. 
/*! This class is responsible for opening and closing a TFile, and it
 * also owns the pointer to the TTree from which we would like to read
 * from. 
 *
 * It supports single pass iteration of events in the TTree. */
class RootReader {

  public:

    //! Reader status codes
    enum class Status {
      kReadSucceeded = 0,
      kEOF = 1,
      kMaxRecoCandExceeded = 2,
      kMaxMcParticlesExceeded = 3,
    };

    //! Default constructor. 
    RootReader() = default;

    //! Constructor with specified root file name and TTree name. 
    RootReader(const char *root_fname, const char *root_trname = "ntp1");
    virtual ~RootReader();

    //! Read in the next event from the TTree. 
    virtual Status next_record();

  private:
    TFile *tfile = nullptr;

  protected: 
    TTree *tr = nullptr;

  private: 
    int record_index = 0;
    int total_records = 0;

    void PrepareTreeFile(const char *root_fname, const char *root_trname);
};

#endif
