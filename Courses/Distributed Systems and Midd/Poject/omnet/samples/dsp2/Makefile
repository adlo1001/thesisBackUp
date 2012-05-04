#
# OMNeT++/OMNEST Makefile for dsp2
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out
#

# Name of target to be created (-o option)
TARGET = dsp2$(EXE_SUFFIX)

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
    $O/appBroker.o \
    $O/appClient.o \
    $O/causalConsistency.o \
    $O/internet.o \
    $O/netFault.o \
    $O/TimstampedPktRef.o \
    $O/User.o \
    $O/appPacket_m.o \
    $O/consistencyCtrlPacket_m.o \
    $O/internetPacket_m.o \
    $O/selfMessage_m.o \
    $O/sysReply_m.o \
    $O/userCommand_m.o

# Message files
MSGFILES = \
    appPacket.msg \
    consistencyCtrlPacket.msg \
    internetPacket.msg \
    selfMessage.msg \
    sysReply.msg \
    userCommand.msg

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
	-rm -f dsp2 dsp2.exe libdsp2.so libdsp2.a libdsp2.dll libdsp2.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f results/*_m.cc results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/selfMessage_m.o: selfMessage_m.cc \
	selfMessage_m.h
$O/userCommand_m.o: userCommand_m.cc \
	userCommand_m.h
$O/internet.o: internet.cc \
	TimstampedPktRef.h \
	selfMessage_m.h \
	internet.h \
	internetPacket_m.h
$O/sysReply_m.o: sysReply_m.cc \
	sysReply_m.h
$O/TimstampedPktRef.o: TimstampedPktRef.cc \
	TimstampedPktRef.h
$O/consistencyCtrlPacket_m.o: consistencyCtrlPacket_m.cc \
	consistencyCtrlPacket_m.h
$O/appClient.o: appClient.cc \
	userCommand_m.h \
	sysReply_m.h \
	appClient.h \
	appPacket_m.h
$O/causalConsistency.o: causalConsistency.cc \
	causalConsistency.h \
	selfMessage_m.h \
	appPacket_m.h \
	consistencyCtrlPacket_m.h
$O/User.o: User.cc \
	User.h \
	userCommand_m.h \
	selfMessage_m.h \
	sysReply_m.h
$O/appBroker.o: appBroker.cc \
	userCommand_m.h \
	appBroker.h \
	selfMessage_m.h \
	appPacket_m.h
$O/appPacket_m.o: appPacket_m.cc \
	appPacket_m.h
$O/netFault.o: netFault.cc \
	netFault.h \
	selfMessage_m.h \
	internetPacket_m.h \
	consistencyCtrlPacket_m.h
$O/internetPacket_m.o: internetPacket_m.cc \
	internetPacket_m.h

