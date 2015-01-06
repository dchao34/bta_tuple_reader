# Configurations
# --------------

# compiler
CXX ?= g++
CXXFLAGS = -Wall -std=c++11
CXXFLAGS += -fPIC

# Contents
# --------

BINARIES = mcreader_test1 mcreader_test2 mcreader_test3 truthmatch_test1 truthmatch_test2 truthmatch_test3

# Dependencies
# ------------

# tuple_reader 
TUPLE_READER_LIBNAME = -lTupleReader
TUPLE_READER_INC_PATH = ../../
TUPLE_READER_LIB_PATH = ../../lib
INCFLAGS += -I $(TUPLE_READER_INC_PATH)
LDFLAGS += -L $(TUPLE_READER_LIB_PATH) $(TUPLE_READER_LIBNAME)

# custom cpp utilities
CUSTOM_CPP_UTIL_ROOT = /Users/dchao/bdtaunu/v4/custom_cpp_utilities
INCFLAGS += -I$(CUSTOM_CPP_UTIL_ROOT)

# cern root
INCFLAGS += $(shell root-config --cflags)
LDFLAGS += $(shell root-config --libs)

# boost
INCFLAGS += -I$(BOOST_ROOT)

# Build Rules
# -----------

.PHONY: all debug pdf pdf_clean clean 

$@ : $@.cc
	$(CXX) $(CXXFLAGS) $< -o $%

all : CXXFLAGS += -O3
all : $(BINARIES)

debug : CXX += -DDEBUG -g
debug : $(BINARIES)

$(BINARIES) : % : %.cc
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS) -Wl,-rpath,$(TUPLE_READER_LIB_PATH) -o $@ $<

pdf:
	printf "%s\n" *.gv | sed s'/^\(.*\).gv/ -Tpdf \1.gv -o \1.pdf/' | xargs -L 1 dot
	pdftk *.pdf cat output merged.pdf

pdf_clean:
	rm -f *.gv *.pdf

clean:
	rm -f *~ *.o *.gv *.pdf `find . -perm +100 -type f`
