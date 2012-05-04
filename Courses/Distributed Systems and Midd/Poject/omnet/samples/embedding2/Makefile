#
# OMNeT++/OMNEST Makefile for embedding2
#
# This file was generated with the command:
#  opp_makemake -f --deep --nolink
#

# C++ include paths (with -I)
INCLUDE_PATH = -I. -Imodel

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/main.o $O/model/Host.o $O/model/nedfiles.o $O/model/Server.o

# Message files
MSGFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# inserted from file 'makefrag':
#
# Customize Makefile to link an executable with the simulation kernel,
# but without the user interface libs (oppenvir, oppcmdenv, opptkenv)
#

TARGET = embedding2$(EXE_SUFFIX)

OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(KERNEL_LIBS) $(SYS_LIBS)

all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	ln -s -f $O/$(TARGET) .

$O/$(TARGET): $(OBJS) Makefile
	@mkdir -p $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(OMNETPP_LIBS)

# <<<
#------------------------------------------------------------------------------

# Main target

all: $(OBJS)  Makefile
	@# Do nothing

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f embedding2 embedding2.exe libembedding2.so libembedding2.a libembedding2.dll libembedding2.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f model/*_m.cc model/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc model/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/main.o: main.cc
$O/model/Host.o: model/Host.cc \
  model/Host.h
$O/model/Server.o: model/Server.cc \
  model/Server.h
$O/model/nedfiles.o: model/nedfiles.cc
