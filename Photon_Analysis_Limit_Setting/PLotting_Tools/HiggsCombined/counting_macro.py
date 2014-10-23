import sys
import os
import ROOT
from ROOT import RooFit,RooStats
ROOT.gSystem.SetIncludePath('-I$ROOFITSYS/include')
if os.access('RooPowerFunction.cxx', os.R_OK):
    ROOT.gROOT.ProcessLine('.L RooPowerFunction.cxx+')


#create workspace
pWs = ROOT.RooWorkspace("myWS")

def MakeWorkspace(outfilename):
    # use this area to implement your model for a counting experiment
    print "building counting model..."
    #observable
    pWs.factory("n[0]") #this create a literal, real-valued container called n
    #signal yield
    #pWs.factory("nsig[0,0,100]") # name[value,min bound,max bound]
    #signal yield
    pWs.factory("lumi[20000]") #luminosity
    pWs.factory("xsec[0,0,0.1]") #cross-section (the new POI)
    pWs.factory("eff[0.1]") #efficiency
    pWs.factory("prod::nsig(lumi,xsec,eff)") #new definition of nsig
    #background yield
    pWs.factory("nbkg[10,0,100]")
    #full yield
    pWs.factory("sum::yield(nsig,nbkg)") # creates a function nsig+nbkg
    # define Bayesian prior PDF for POI
    pWs.factory( "Uniform::prior(xsec)" )
    #save the workspace to a file
    pWs.SaveAs(outfilename)
    #print the contents of the workspace
    pWs.Print()

    return 0
################################################################################
if __name__ == "__main__":
    MakeWorkspace("workspace.root")

