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
CND_CONF=Release
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
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/nbproject

dist/Release/GNU-Linux-x86/nbproject: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nbproject ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/source/displaySettings.o: nbproject/Makefile-${CND_CONF}.mk ../source/displaySettings.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/displaySettings.o ../source/displaySettings.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/pathplan.o: nbproject/Makefile-${CND_CONF}.mk ../source/pathplan.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/pathplan.o ../source/pathplan.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/main.o: nbproject/Makefile-${CND_CONF}.mk ../source/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/main.o ../source/main.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/mainWindow.o: nbproject/Makefile-${CND_CONF}.mk ../source/mainWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/mainWindow.o ../source/mainWindow.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/drawing.o: nbproject/Makefile-${CND_CONF}.mk ../source/drawing.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/drawing.o ../source/drawing.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/communication.o: nbproject/Makefile-${CND_CONF}.mk ../source/communication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/communication.o ../source/communication.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/game.o: nbproject/Makefile-${CND_CONF}.mk ../source/game.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/game.o ../source/game.cpp

${OBJECTDIR}/_ext/_DOTDOT/source/interface.o: nbproject/Makefile-${CND_CONF}.mk ../source/interface.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/source/interface.o ../source/interface.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/nbproject

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
