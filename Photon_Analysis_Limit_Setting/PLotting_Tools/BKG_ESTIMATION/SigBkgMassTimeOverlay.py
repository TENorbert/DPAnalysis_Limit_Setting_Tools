# This Simple Code Overlays Histograms Plots  from Different  Jet Multiplicity Files
# Cannot DO IT with C++ because It root chokes especially when Number bins are 
# not the same.
## Aurthor Tambe E. Norbert a.k.a TEN (norbert@physics.umn.edu)

#!/usr/bin/env python
""" OverLay Myhist.png from several  Directories identical structure!"""
__version__ = "1.0"


import sys
import optparse
import shutil
import os
import re
from ROOT import *
from ROOT import TFile, TH1D, TLegend, TCanvas, TLatex
from array import array

# ROOT general options
#gROOT.SetBatch(kTRUE)
#gROOT.SetStyle("Plain")


gStyle.SetCanvasColor(kWhite)  #   // background is no longer mouse-dropping white
gStyle.SetPalette(1) #           // blue to red false color palette. Use 9 for b/w
gStyle.SetCanvasBorderMode(0) #     // turn off canvas borders
gStyle.SetPadBorderMode(0)
gStyle.SetPaintTextFormat("5.2f") #  // What precision to put numbers if plotted with "TEXT"

#  // For publishing:
gStyle.SetLineWidth(2)
gStyle.SetTextSize(1.1)
gStyle.SetLabelSize(0.06,"xy")
gStyle.SetTitleSize(0.06,"xy")
gStyle.SetTitleOffset(1.2,"x")
gStyle.SetTitleOffset(1.05,"y")
#gStyle.SetPadTopMargin(0.1)
#gStyle.SetPadRightMargin(0.1)
gStyle.SetPadBottomMargin(0.16)
gStyle.SetPadLeftMargin(0.12)
gStyle.SetOptStat(0)
gStyle.SetPadGridX(1);
gStyle.SetPadGridY(1);

# Get Files!
#------------------------------------------
fbg =  TFile("BACKGROUND_TimePerf-plots.root")
fsg =  TFile("SIGNAL_TimePerf-plots.root")

# Mass
hZmass = fbg.Get("EBEB/mass")
hZmassSg = fsg.Get("EBEB/NZmass")
hZmassBg = fbg.Get("EBEB/NZmass") 

#Electron Candidate Time
hZtime = fbg.Get("EBEB/seed time")
hZtimeSg = fsg.Get("EBEB/NZMass seed time")
hZtimeBg = fbg.Get("EBEB/NZMass seed time")

### For Mass
# Choose Line Colors
hZmass.SetLineColor(kBlack)
hZmassSg.SetLineColor(kBlue)
hZmassBg.SetLineColor(kRed)

# Choose Line Style
hZmass.SetLineStyle(1)
hZmassSg.SetLineStyle(7)
hZmassBg.SetLineStyle(7)


#Choose LineWidth
hZmass.SetLineWidth(5)
hZmassSg.SetLineWidth(5)
hZmassBg.SetLineWidth(5)

#Choose Marker Style!
#hZmass.SetMarkerStyle(2)
#hZmassSg.SetMarkerStyle(3)
#hZmassBg.SetMarkerStyle(4)

# Scale Histograms
#if hZmass.Integral()!=0:
#                   hZmass.Scale(1/hZmass.Integral())

#if hZmassSg.Integral()!=0:
#                   hZmassSg.Scale(1/hZmassSg.Integral())

#if hZmassBg.Integral()!=0:
#                   hZmassBg.Scale(1/hZmassBg.Integral())


# Label Axis
hZmass.GetXaxis().SetTitle("Mass_{#gamma^{1},#gamma^{2}}[GeV/c^{2}]")
hZmass.GetYaxis().SetTitle("Events")


 ### For Candidate Time
# Choose Line Colors
hZtime.SetLineColor(kBlack)
hZtimeSg.SetLineColor(kBlue)
hZtimeBg.SetLineColor(kRed)

# Choose Line Style
hZtime.SetLineStyle(1)
hZtimeSg.SetLineStyle(7)
hZtimeBg.SetLineStyle(7)


#Choose LineWidth
hZtime.SetLineWidth(5)
hZtimeSg.SetLineWidth(5)
hZtimeBg.SetLineWidth(5)


#Choose Marker Style!
#hZtime.SetMarkerStyle(2)
#hZtimeSg.SetMarkerStyle(3)
#hZtimeBg.SetMarkerStyle(4)

# Scale Histograms
#if hZtime.Integral()!=0:
#                   hZtime.Scale(1/hZtime.Integral())

#if hZtimeSg.Integral()!=0:
#                   hZtimeSg.Scale(1/hZtimeSg.Integral())

#if hZtimeBg.Integral()!=0:
#                   hZtimeBg.Scale(1/hZtimeBg.Integral())


# Label Axis
hZtime.GetXaxis().SetTitle("ECAL Time(seed)[ns]")
hZtime.GetYaxis().SetTitle("Events")


s_lumi = "19.1"
CoMEr = "(8 TeV)"
#CoMEr = "#sqrt{(S)} = 8 TeV";
#std::string lint = "#int Ldt= "+s_lumi+" fb^{-1}";
#lint = s_lumi + "fb^{-1}"+ CoMEr;
lint = CoMEr


# Draw Plots now
# ************************************************************************************
# Signal Mass
c1 = TCanvas("c1", "SignalOverLay", 900, 900)
c1.cd()
cmstex = TLatex()
#cmstex = ROOT.TLatex()
cmstex.SetTextAlign(12)
cmstex.SetTextSize(0.035)
cmstex.SetTextFont(22)
cmstex.SetTextAngle(0)
cmstex.SetTextColor(kBlack)
cmstex.SetNDC()
cmstex.DrawLatex(0.20, 0.865, "CMS")
cmstex.DrawLatex(0.20, 0.832, "Preliminary")
cmstex.DrawLatex(0.72, 0.920, lint)

hZmass.SetTitle("Z Mass")
hZmass.Draw()
hZmassSg.Draw("sames")
hZmass.GetXaxis().SetRangeUser(50.0, 150.0)
#Add Legend
lsg = TLegend( 0.60, 0.70, 0.94, 0.90)
lsg.SetBorderSize(1)
lsg.SetFillColor(19)
lsg.AddEntry(hZmass,"Total Z Candidates","l")
lsg.AddEntry(hZmassSg,"76GeV/c^{2} < Mass < 100 GeV/c^{2}","l")
lsg.SetTextSize(0.02)
lsg.SetHeader("CMS Preliminary (8 TeV)") # L=8.7 fb^{-1}")
lsg.Draw()

# Z Sig Time
c2 = TCanvas("c3", "SignalTimeOverLay", 900, 900)
c2.cd()

hZtime.SetTitle("ECAL Time")
hZtime.Draw()
hZtimeSg.Draw("sames")
hZtime.GetXaxis().SetRangeUser(-10.0, 10.0)
#Add Legend
lsgt = TLegend( 0.60, 0.70, 0.94, 0.90)
lsgt.SetBorderSize(1)
lsgt.SetFillColor(19)
lsgt.AddEntry(hZtime,"Total Z Candidates","l")
lsgt.AddEntry(hZtimeSg,"76GeV/c^{2} < Mass < 100 GeV/c^{2}","l")
lsgt.SetTextSize(0.02)
lsgt.SetHeader("CMS Preliminary (8 TeV)") # L=8.7 fb^{-1}")
lsgt.Draw()


# Bkg Mass
c3 = TCanvas("c2", "BackGroundOverLay", 900, 900)
c3.cd()
hZmass.SetTitle("Z Mass")
hZmass.Draw()
hZmassBg.Draw("sames")
hZmass.GetXaxis().SetRangeUser(50.0, 150.0)

#Add Legend
lbg = TLegend( 0.60, 0.70, 0.94, 0.90)
lbg.SetBorderSize(1)
lbg.SetFillColor(19)
lbg.AddEntry(hZmass,"Total Z Candidates","l")
lbg.AddEntry(hZmassBg,"Mass < 76 & Mass > 100 GeV/c^{2}","l")
lbg.SetTextSize(0.02)
lbg.SetHeader("CMS Preliminary (8 TeV)") # #sqrt{s}=8 TeV") # L=8.7 fb^{-1}")
lbg.Draw()


# Bkg Time
c4 = TCanvas("c4", "BackgroundTimeOverLay", 900, 900)
c4.cd()
hZtime.SetTitle("ECAL Time")
hZtime.Draw()
hZtimeBg.Draw("sames")
hZtime.GetXaxis().SetRangeUser(-10.0, 10.0)

#Add Legend
lbgt = TLegend( 0.60, 0.70, 0.94, 0.90)
lbgt.SetBorderSize(1)
lbgt.SetFillColor(19)
lbgt.AddEntry(hZtime,"Total Z Candidates","l")
lbgt.AddEntry(hZtimeBg,"Mass < 76 & Mass > 100 GeV/c^{2}","l")
lbgt.SetTextSize(0.02)
lbgt.SetHeader("CMS Preliminary (8 TeV)") # L=8.7 fb^{-1}")
lbgt.Draw()


#_______________________ Save Plots_______________________#

c1.SetLogy(1)
c1.SaveAs("Z-CandidateOverLay-SignalMass.png")
c1.SaveAs("Z-CandidateOverLay-SignalMass.pdf")

c2.SetLogy(1)
c2.SaveAs("Z-CandidateOverLay-SignalTime.png")
c2.SaveAs("Z-CandidateOverLay-SignalTime.pdf")

c3.SetLogy(1)
c3.SaveAs("Z-CandidateOverLay-BackgroundMass.png")
c3.SaveAs("Z-CandidateOverLay-BackgroundMass.pdf")

c4.SetLogy(1)
c4.SaveAs("Z-CandidateOverLay-BackgroundTime.png")
c4.SaveAs("Z-CandidateOverLay-BackgroundTime.pdf")

