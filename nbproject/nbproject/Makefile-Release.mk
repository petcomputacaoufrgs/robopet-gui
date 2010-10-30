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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/812168374/mainWindow.o \
	${OBJECTDIR}/_ext/812168374/interface.o \
	${OBJECTDIR}/_ext/812168374/main.o \
	${OBJECTDIR}/_ext/812168374/communication.o \
	${OBJECTDIR}/_ext/812168374/displaySettings.o \
	${OBJECTDIR}/_ext/812168374/game.o \
	${OBJECTDIR}/_ext/812168374/guiPathplan.o \
	${OBJECTDIR}/_ext/812168374/drawing.o


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
LDLIBSOPTIONS=../../communication/communication.a ../../lib/robopet.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk ../GUI

../GUI: ../../communication/communication.a

../GUI: ../../lib/robopet.a

../GUI: ${OBJECTFILES}
	${MKDIR} -p ..
	${LINK.cc} `pkg-config --libs gtkglext-1.0` `pkg-config --libs protobuf` -lglut -o ../GUI  ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/812168374/mainWindow.o: ../source/mainWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/mainWindow.o ../source/mainWindow.cpp

${OBJECTDIR}/_ext/812168374/interface.o: ../source/interface.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/interface.o ../source/interface.cpp

${OBJECTDIR}/_ext/812168374/main.o: ../source/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/main.o ../source/main.cpp

${OBJECTDIR}/_ext/812168374/communication.o: ../source/communication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/communication.o ../source/communication.cpp

${OBJECTDIR}/_ext/812168374/displaySettings.o: ../source/displaySettings.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/displaySettings.o ../source/displaySettings.cpp

${OBJECTDIR}/_ext/812168374/game.o: ../source/game.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/game.o ../source/game.cpp

${OBJECTDIR}/_ext/812168374/guiPathplan.o: ../source/guiPathplan.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/guiPathplan.o ../source/guiPathplan.cpp

${OBJECTDIR}/_ext/812168374/drawing.o: ../source/drawing.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/812168374
	${RM} $@.d
	$(COMPILE.cc) -g -I../header -I../../communication/packets -I../../communication/proto -I../../communication/socket -I../../communication -I../../lib -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/812168374/drawing.o ../source/drawing.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} ../GUI

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
