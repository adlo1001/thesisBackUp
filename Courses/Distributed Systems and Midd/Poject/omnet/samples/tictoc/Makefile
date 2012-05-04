#
# OMNeT++/OMNEST Makefile for tictoc
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out
#

# Name of target to be created (-o option)
TARGET = tictoc$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -Iresults

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/txc1.o \
    $O/txc10.o \
    $O/txc11.o \
    $O/txc12.o \
    $O/txc13.o \
    $O/txc14.o \
    $O/txc15.o \
    $O/txc16.o \
    $O/txc2.o \
    $O/txc3.o \
    $O/txc4.o \
    $O/txc5.o \
    $O/txc6.o \
    $O/txc7.o \
    $O/txc8.o \
    $O/txc9.o \
    $O/tictoc13_m.o \
    $O/tictoc14_m.o \
    $O/tictoc15_m.o \
    $O/tictoc16_m.o

# Message files
MSGFILES = \
    tictoc13.msg \
    tictoc14.msg \
    tictoc15.msg \
    tictoc16.msg

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

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

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
	-rm -f tictoc tictoc.exe libtictoc.so libtictoc.a libtictoc.dll libtictoc.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f results/*_m.cc results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/txc11.o: txc11.cc
$O/txc16.o: txc16.cc \
	tictoc16_m.h
$O/txc1.o: txc1.cc
$O/txc13.o: txc13.cc \
	tictoc13_m.h
$O/txc8.o: txc8.cc
$O/tictoc16_m.o: tictoc16_m.cc \
	tictoc16_m.h
$O/tictoc13_m.o: tictoc13_m.cc \
	tictoc13_m.h
$O/txc15.o: txc15.cc \
	tictoc15_m.h
$O/txc14.o: txc14.cc \
	tictoc14_m.h
$O/txc3.o: txc3.cc
$O/txc12.o: txc12.cc
$O/txc4.o: txc4.cc
$O/txc9.o: txc9.cc
$O/tictoc14_m.o: tictoc14_m.cc \
	tictoc14_m.h
$O/txc6.o: txc6.cc
$O/txc5.o: txc5.cc
$O/txc2.o: txc2.cc
$O/txc10.o: txc10.cc
$O/tictoc15_m.o: tictoc15_m.cc \
	tictoc15_m.h
$O/txc7.o: txc7.cc

