//** My simple Macro to Fit hists ***/
//Run AS 
//root[0] .L Hist_Fit.C
//root[1] .X Hist_Fit.C
// Designed by 10Sr @2013 // norbe072@umn.edu
#include "TObject.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TFrame.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "Riostream.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TString.h"
#include "TStyle.h"
#include "TSystem.h"
#include  "TMath.h"
#include  "TROOT.h"
#include  <iostream>
#include  <string>
#include  <cmath>

using namespace std;

//gROOT->Reset();
//gROOT->Clear();

double gauss2D(double *x, double *par) {
	double z1 = 0;
	if(par[2] != 0) z1 = double((x[0]-par[1])/par[2]);
	double z2 = 0;
	if(par[4] != 0 ) z2 = double((x[1]-par[3])/par[4]);          
	    return par[0]*exp(-0.5*(z1*z1+z2*z2));
} 

// private Defined  gaus fxn
//
double mygaus(double *t, double *par){
	double x = t[0];
	double arg0 = par[0];
	double arg1 = 0;
	if(par[2] != 0) arg1 =double( ( x - par[1])/par[2] );
	double  f   = arg0*TMath::Exp(-0.5*arg1*arg1);
	return f;
}


//doing it the D**way
 double low0 = 0.0;
 double high0 = 5.0;
 double low1 = 5.0;
 double high1 = 7.50;
 double low2 = 7.50;
 double high2 = 9.50;
 double low3 = 9.50;
 double high3 = 9.50;
 double low4 = 12.0;
 double high4 = 14.50;
 double low5 = 14.50;
 double high5 = 17.0;
 double low6 = 17.0;
 double high6 = 19.50;
 double low7 = 19.0;
 double high7 = 22.50;
 double low8 = 22.50;
 double high8 = 26.0;


 TF1 *fitfunc  = 0;
 double par[24];
//Generating Fit Function to Use//

 TF1* g0 = new TF1("g0","gaus", low0, high0);
 TF1* g1 = new TF1("g1","gaus", low1, high1);
 TF1* g2 = new TF1("g2","gaus", low2, high2);
 TF1* g3 = new TF1("g3","gaus", low3, high3);
 TF1* g4 = new TF1("g4","gaus", low4, high4);
 TF1* g5 = new TF1("g5","gaus", low5, high5);
 TF1* g6 = new TF1("g6","gaus", low6, high6);
 TF1* g7 = new TF1("g7","gaus", low7, high7);


//double fitf(double *t, double *par){
TF1* fitf = new TF1("Totalfitfunc", "gaus(0) + gaus(3) + gaus(6) + gaus(9) + gaus(12) + gaus(15) + gaus(18) + gaus(21) + gaus(24)", 0.0 , 25.0); 
//        double fitfunc += gaus(0) + gaus(3) + gaus(6) + gaus(9) + gaus(12) + gaus(15) + gaus(18) + gaus(21) + gaus(24);
//	return fitfunc;
//}

TF1 *fitFcn  = fitf;
TH1F *IhistEEM = 0;
TH1F *IhistEEP = 0;


//*** Main Fitting Fxn *****//
void Hist_Fit(char *Ifile){
	
        TFile *ifile  = new TFile(Ifile);
	IhistEEM = (TH1F*)ifile->Get("BkgEst/my.ALL_EEM_pho_time"); // use EE- to get fit fxn and try in on EE+
	IhistEEP = (TH1F*)ifile->Get("BkgEst/my.ALL_EEP_pho_time"); 
//	gDirectory->GetObject("BkgEst/my.ALL_EEM_pho_time;1", IhistEEM);
//	gDirectory->GetObject("BkgEst/my.ALL_EEP_pho_time;1", IhistEEP);
	 

	if(IhistEEM == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
	if(IhistEEP == 0){ std::cout  <<"!! Histogram Does not exist!!" << std::endl; throw 1;}
        
//        fitFcn = new TF1("Totalfitfunc", "gaus(0) + gaus(3) + gaus(6) + gaus(9) + gaus(12) + gaus(15) + gaus(18) + gaus(21) + gaus(24)", -10.0 , 30.0,24);

//        TF1* fitFcn = new TF1("fitFcn", fitf, -10.0, 30.0, 24);
	fitFcn->SetNpx(500);
	fitFcn->SetLineWidth(4);
	fitFcn->SetLineColor(kMagenta);

	// Fit Each fxn and Add it to the List
	IhistEEM->Fit(g0, "R");	
	IhistEEM->Fit(g1, "R+");	
	IhistEEM->Fit(g2, "R+");	
	IhistEEM->Fit(g3, "R+");	
	IhistEEM->Fit(g4, "R+");	
	IhistEEM->Fit(g5, "R+");	
	IhistEEM->Fit(g6, "R+");	
	IhistEEM->Fit(g7, "R+");	

 	// GetParameters from each fit first
	g0->GetParameters(&par[0]);
	g1->GetParameters(&par[3]);
	g2->GetParameters(&par[6]);
	g3->GetParameters(&par[9]);
	g4->GetParameters(&par[12]);
	g5->GetParameters(&par[15]);
	g6->GetParameters(&par[18]);
	g7->GetParameters(&par[24]);

	//set parms as parms of Fit Fxn
	fitFcn->SetParameters(par);
	// Now Do the Final Fit to see How it looks on the Histogram
	IhistEEM->Fit(fitFcn, "R+");
	std::cout << "Printing Fit Parameters For EE- ......   " << std::endl;
        fitFcn->GetParameters(par);
        // Gets integral of function between fit limits
        printf("Integral of function in EE-  = %g\n",fitFcn->Integral(-2,2));
 
	IhistEEP->Fit(fitFcn, "R+"); // Try Fit on EE+	
	std::cout << "Printing Fit Parameters For EE- ......   " << std::endl;
        fitFcn->GetParameters(par);
        printf("Integral of function in EE+  = %g\n",fitFcn->Integral(-2,2));
	
	TCanvas *c1 = new TCanvas("c1_fit","Fit Canvas",200,10,800,900);
     	c1->SetGridx();
     	c1->SetGridy();
	c1->GetFrame()->SetFillColor(21);
	c1->GetFrame()->SetBorderMode(-1);
	c1->GetFrame()->SetBorderSize(5);
        c1->Divide(2,1);
	
	c1->cd(1);
	IhistEEM->Draw();
	fitFcn->Draw("sames");

	//  draw the legend
    	TLegend *leg = new TLegend(0.6,0.65,0.88,0.85);
       	leg->SetTextFont(72);
        leg->SetTextSize(0.04);
        leg->AddEntry(IhistEEM,"Data","lpe");
        leg->AddEntry(fitFcn,"Global Fit","l");
        leg->Draw();
                     
        c1->cd(2);
	IhistEEP->Draw();
	fitFcn->Draw("sames");

    	TLegend *leg1 = new TLegend(0.6,0.65,0.88,0.85);
       	leg1->SetTextFont(72);
        leg1->SetTextSize(0.04);
        leg1->AddEntry(IhistEEP,"Data","lpe");
        leg1->AddEntry(fitFcn,"Global Fit","l");
        leg1->Draw();

}
