#
# OMNeT++/OMNEST Makefile for libqueueinglib
#
# This file was generated with the command:
#  opp_makemake -f --make-so
#

# Name of target to be created (-o option)
TARGET = libqueueinglib$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = -I.

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
    $O/Allocate.o \
    $O/Classifier.o \
    $O/Clone.o \
    $O/Deallocate.o \
    $O/Delay.o \
    $O/Fork.o \
    $O/Job.o \
    $O/JobList.o \
    $O/Join.o \
    $O/Merge.o \
    $O/PassiveQueue.o \
    $O/Queue.o \
    $O/ResourceBasedQueue.o \
    $O/ResourcePool.o \
    $O/Router.o \
    $O/SelectionStrategies.o \
    $O/Server.o \
    $O/Sink.o \
    $O/Source.o \
    $O/Job_m.o

# Message files
MSGFILES = \
    Job.msg

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

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
	$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(LIBS) $(OMNETPP_LIBS) $(LDFLAGS)
	$(SHLIB_POSTPROCESS) $O/$(TARGET)

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
	-rm -f queueinglib queueinglib.exe libqueueinglib.so libqueueinglib.a libqueueinglib.dll libqueueinglib.dylib
	-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Allocate.o: Allocate.cc \
  ./IResourcePool.h \
  ./QueueingDefs.h \
  ./ResourcePool.h \
  ./Job.h \
  ./Job_m.h \
  ./Allocate.h
$O/Classifier.o: Classifier.cc \
  ./Classifier.h \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h
$O/Clone.o: Clone.cc \
  ./Clone.h \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h
$O/Deallocate.o: Deallocate.cc \
  ./IResourcePool.h \
  ./QueueingDefs.h \
  ./ResourcePool.h \
  ./Deallocate.h
$O/Delay.o: Delay.cc \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h \
  ./Delay.h
$O/Fork.o: Fork.cc \
  ./Fork.h \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h
$O/Job.o: Job.cc \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h \
  ./JobList.h
$O/JobList.o: JobList.cc \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h \
  ./JobList.h
$O/Job_m.o: Job_m.cc \
  ./QueueingDefs.h \
  ./Job_m.h
$O/Join.o: Join.cc \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h \
  ./Join.h
$O/Merge.o: Merge.cc \
  ./QueueingDefs.h \
  ./Merge.h
$O/PassiveQueue.o: PassiveQueue.cc \
  ./QueueingDefs.h \
  ./IPassiveQueue.h \
  ./SelectionStrategies.h \
  ./Job.h \
  ./Job_m.h \
  ./PassiveQueue.h
$O/Queue.o: Queue.cc \
  ./QueueingDefs.h \
  ./Queue.h \
  ./Job.h \
  ./Job_m.h
$O/ResourceBasedQueue.o: ResourceBasedQueue.cc \
  ./IResourcePool.h \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h \
  ./ResourceBasedQueue.h
$O/ResourcePool.o: ResourcePool.cc \
  ./IResourcePool.h \
  ./QueueingDefs.h \
  ./ResourcePool.h
$O/Router.o: Router.cc \
  ./QueueingDefs.h \
  ./Router.h
$O/SelectionStrategies.o: SelectionStrategies.cc \
  ./IServer.h \
  ./Server.h \
  ./QueueingDefs.h \
  ./IPassiveQueue.h \
  ./SelectionStrategies.h \
  ./PassiveQueue.h
$O/Server.o: Server.cc \
  ./IServer.h \
  ./Server.h \
  ./QueueingDefs.h \
  ./IPassiveQueue.h \
  ./SelectionStrategies.h \
  ./Job.h \
  ./Job_m.h
$O/Sink.o: Sink.cc \
  ./Sink.h \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h
$O/Source.o: Source.cc \
  ./QueueingDefs.h \
  ./Job.h \
  ./Job_m.h \
  ./Source.h
