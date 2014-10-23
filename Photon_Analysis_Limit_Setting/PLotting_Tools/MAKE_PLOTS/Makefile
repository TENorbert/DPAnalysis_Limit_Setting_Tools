# My Make file for DoPlots April 23/2014 Tambe E.. Norbert @ UMN.EDU
CC=g++ -g -fPIC -Wall -O -ansi -D_GNU_SOURCE -g -O2 -m64
ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags`
LDFLAGS = `${ROOTSYS}/bin/root-config  --ldflags --glibs` -lHistFactory -lXMLParser -lRooStats -lRooFit -lRooFitCore -lThread -lMinuit -lFoam -lHtml -lMathMore 

RFIDIR=/local/cms/sw/slc5_amd64_gcc462/lcg/roofit/5.32.00-cms6/include

RFLIB=/local/cms/sw/slc5_amd64_gcc462/lcg/roofit/5.32.00-cms6/lib

RFFLAGS=-I$(RFIDIR)

LIBROOT=`${ROOTSYS}/bin/root-config --libs` 
LIBROOFIT=-L$(RFLIB) 

#OBJECTS=AnaInput.o hDraw.o StatsLib.o StatsTool.o 
OBJECTS=PLot_Limit.o

all:doplots.exe

PLot_Limit.o : PLot_Limit.cc
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIBROOT) $(LIBROOFIT) $(RFFLAGS) $(LDFLAGS) 

CMS_lumi.o : CMS_lumi.C CMS_lumi.h
	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIBROOT) $(LIBROOFIT) $(RFFLAGS)
#hDraw.o : hDraw.cc hDraw.h
#	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIBROOT) $(LIBROOFIT) $(RFFLAGS)

#StatsLib.o : StatsLib.cc StatsLib.h
#	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIBROOFIT) $(LIBROOFIT) $(RFFLAGS)

#StatsTool.o : StatsTool.cc StatsTool.h
#	$(CC) -c -o $@ $< $(ROOTFLAG) $(LIBROOT) $(LIBROOFIT) $(RFFLAGS) $(LDFLAGS) 

doplots.exe : $(OBJECTS)  
	$(CC) -o $@ $ < $(OBJECTS) $(ROOTFLAG) $(LIBROOT) $(LIBROOFIT) $(RFFLAGS) $(LDFLAGS)

clean : 
	rm -rf *.o doplots.exe
