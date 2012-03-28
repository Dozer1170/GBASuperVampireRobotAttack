##########################################################################################
# Makefile to be used with the HAM SDK (www.ngine.de)
#
# Created using VisualHAM (www.console-dev.de)
#
##########################################################################################
include $(HAMDIR)/system/master.mak

#
# Set the name of your desired GBA image name here
#
PROGNAME=FinalGPAProj

#
# Set a list of files you want to compile
#
OFILES +=levelone.map.o	\
	levelone.pal.o	\
	levelone.raw.o	\
	levelonehitmap.map.o	\
	levelonelandscape.map.o	\
	main.o	\
	sinlut.o

#
# Set a list of libraries you want to link
#
ADD_LIBS +=

#
# Compile using Krawall software (set to yes or no) ?
# Also specify if Krawall is registered (yes or no)
#
USE_KRAWALL=no
KRAWALL_IS_REGISTERED=no
KRAWALL_FILES=

##########################################################################################
# Standard Makefile targets start here
##########################################################################################
all : $(PROGNAME).$(EXT)

#
# Most Makefile targets are predefined for you, suchas
# vba, clean ... in the following file
#
include $(HAMDIR)/system/standard-targets.mak

##########################################################################################
# Add custom makefile targets here
##########################################################################################
