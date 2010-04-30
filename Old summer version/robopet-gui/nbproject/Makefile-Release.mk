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
	${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/mainWindow.o \
	${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/sock.o \
	${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/server.o \
	${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/displaySettings.o \
	${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/interface.o \
	${OBJECTDIR}/main.o

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
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/robopet-gui

dist/Release/GNU-Linux-x86/robopet-gui: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/robopet-gui ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/mainWindow.o: nbproject/Makefile-${CND_CONF}.mk /grupos/RoboPET/robopet/robopet-gui/source/mainWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/mainWindow.o /grupos/RoboPET/robopet/robopet-gui/source/mainWindow.cpp

${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/sock.o: nbproject/Makefile-${CND_CONF}.mk /grupos/RoboPET/robopet/robopet-gui/source/sock.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/sock.o /grupos/RoboPET/robopet/robopet-gui/source/sock.cpp

${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/server.o: nbproject/Makefile-${CND_CONF}.mk /grupos/RoboPET/robopet/robopet-gui/source/server.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/server.o /grupos/RoboPET/robopet/robopet-gui/source/server.cpp

${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/displaySettings.o: nbproject/Makefile-${CND_CONF}.mk /grupos/RoboPET/robopet/robopet-gui/source/displaySettings.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/displaySettings.o /grupos/RoboPET/robopet/robopet-gui/source/displaySettings.cpp

${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/interface.o: nbproject/Makefile-${CND_CONF}.mk /grupos/RoboPET/robopet/robopet-gui/source/interface.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/grupos/RoboPET/robopet/robopet-gui/source/interface.o /grupos/RoboPET/robopet/robopet-gui/source/interface.cpp

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/robopet-gui

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
