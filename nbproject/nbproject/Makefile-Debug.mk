#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/source/displaySettings.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/pathplan.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/main.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/mainWindow.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/drawing.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/communication.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/game.o \
	${OBJECTDIR}/_ext/_DOTDOT/source/interface.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`pkg-config --cflags gtkglext-1.0` `pkg-config --cflags protobuf` 
CXXFLAGS=`pkg-config --cflags gtkglext-1.0` `pkg-config --cflags protobuf` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../robopet-communication/packets -L../../robopet-communication/proto -L../../robopet-communication/socket -L../../robopet-ai/lib -L../../robopet-communication

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/nbproject

dist/Debug/GNU-Linux-x86/nbproject: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} ../../robopet-ai/lib/point.o ../../robopet-ai/lib/movingObject.o ../../robopet-ai/lib/rrt.o ../../robopet-ai/lib/player.o ../../robopet-ai/lib/vector.o ../../robopet-communication/ssl_client.o ../../robopet-communication/ssl_server.o ../../robopet-communication/message_gui_to_ai.pb.o ../../robopet-communication/message_ai_to_gui.pb.o ../../robopet-communication/messages_robocup_ssl_wrapper.pb.o ../../robopet-communication/message_ai_to_radio.pb.o ../../robopet-communication/message_ai_to_sim.pb.o ../../robopet-communication/message_ai_to_tracker.pb.o ../../robopet-communication/message_radio_to_tracker.pb.o ../../robopet-communication/message_sim_to_tracker.pb.o ../../robopet-communication/messages_robocup_ssl_detection.pb.o ../../robopet-communication/messages_robocup_ssl_geometry.pb.o ../../robopet-communication/messages_robocup_ssl_refbox_log.pb.o ../../robopet-communication/message_tracker_to_ai.pb.o ../../robopet-communication/udp_netraw.o `pkg-config --libs gtkglext-1.0` `pkg-config --libs protobuf` -lglut -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nbproject  ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/source/displaySettings.o: nbproject/Makefile-${CND_CONF}.mk ../source/displaySettings.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/displaySettings.o ../source/displaySettings.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/pathplan.o: nbproject/Makefile-${CND_CONF}.mk ../source/pathplan.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/pathplan.o ../source/pathplan.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/main.o: nbproject/Makefile-${CND_CONF}.mk ../source/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/main.o ../source/main.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/mainWindow.o: nbproject/Makefile-${CND_CONF}.mk ../source/mainWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/mainWindow.o ../source/mainWindow.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/drawing.o: nbproject/Makefile-${CND_CONF}.mk ../source/drawing.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/drawing.o ../source/drawing.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/communication.o: nbproject/Makefile-${CND_CONF}.mk ../source/communication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/communication.o ../source/communication.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/game.o: nbproject/Makefile-${CND_CONF}.mk ../source/game.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/game.o ../source/game.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/interface.o: nbproject/Makefile-${CND_CONF}.mk ../source/interface.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../robopet-ai/lib -I../../robopet-communication/packets -I../../robopet-communication/proto -I../../robopet-communication/socket -I../../robopet-communication -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/interface.o ../source/interface.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/nbproject

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
