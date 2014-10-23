///
//
//Simple Script to plot Limits
// run as: doPlot CTAU
//
#include "TColor.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TAttFill.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooGenericPdf.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooDataHist.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooPlot.h"
#include "RooHistPdf.h"
#include "RooHist.h"
#include "RooNLLVar.h"
#include "RooBinning.h"
#include "TStyle.h"
#include "TLatex.h"
//#include "TPad.h"
#include "TPolyLine.h"
#include "RooGlobalFunc.h"
#include "TROOT.h"
#include "RooFitResult.h"
#include "TFrame.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "Riostream.h"
#include "TGraphAsymmErrors.h"
#include "Riostream.h"
#include "CMS_lumi.h"
#include "tdrstyle.C"

using namespace RooFit;
using namespace std;

struct FInputLine{
 float Obs_limit, mexp, OneSigUp, TwoSigUp, OneSigD, TwoSigD, eff, sgXsec, ctau, lambda, Bratio, lumi;
 int linenumber;
};

// Use Read in operator
istream& operator>>(istream& in, FInputLine& fline) {
    ++fline.linenumber;

    return in >> fline.Obs_limit
              >> fline.TwoSigD
	      >> fline.OneSigD
	      >> fline.mexp
	      >> fline.OneSigUp
	      >> fline.TwoSigUp
	      >> fline.eff
	      >> fline.sgXsec
	      >> fline.ctau
	      >> fline.lambda
	      >> fline.Bratio
	      >> fline.lumi;
}
//Use read Out Operator
ostream& operator<<(ostream& out, const FInputLine& fline) {
  return out << "LineNumber: " << fline.linenumber 
             << " Observe Limit: " << fline.Obs_limit 
             << " TwoSigmaDown: " << fline.TwoSigD
             << " OneSigmaDown: " << fline.OneSigD
             << " Median Expected: " << fline.mexp
             << " OneSigmaUP: " << fline.OneSigUp
             << " TwoSigmaUP: " << fline.TwoSigUp
             << " Efficiency: " << fline.eff
             << " CrossSection: " << fline.sgXsec
             << " CTAU: " << fline.ctau
             << " Lambda: " << fline.lambda
             << " Braching Ratio: " << fline.Bratio;
}


//Declare fxns
void Plot_limit(std::string); // Xsec Vs Mass
void rootlogon();
void Limit_Draw(const std::string filename, const int nModel); // xsec Vs ctau

// Main function to Run limiti
#ifndef __CINT__
int main(int argc, char* argv[] )  
{
   // Make sure at Least two arguments are given
  
     if( argc < 3 ) { 
                   std::cout <<"Oops! Run Program as: >>  ./doplots CTAU limitfilename NumberOfDifferentCTAU " << std::endl; exit(1);
		  } 
  		  
   std::string ctau = argv[1]; // Given Ctau u want shown on plots
   const std::string limitfilename = argv[2];
   const int  nModels = (int)atoi(argv[3]);
   //Run CMS ploting Style:
   rootlogon();
   // Plot Limit
   Plot_limit(ctau);
  // Draw Xsec*BR Vs ctau Limit
   Limit_Draw(limitfilename, nModels);
    
}
#endif

//***********************************************************************///
//                                                                      *///
//      PLot Limit here!                                                *///
//                                                                      */// 
//***********************************************************************///
void Plot_limit(std::string ctau)
{
      
      /*float upperlimit_Lambda100[5] = {0.018,0.022,0.0438671,0.054,0.074};
        float upperlimit_Lambda120[5] = {0.01,0.018,0.035048,0.046,0.062};
        float upperlimit_Lambda140[5] = {0.006,0.01,0.0240775,0.03,0.042};
        float upperlimit_Lambda160[5] = {0.002,0.006,0.0177448,0.022,0.03};
        float upperlimit_Lambda180[5] = {0.002,0.006,0.0131662,0.014,0.018};
       */
        //float upperlimit1_Lambda100[5] = {0.,0.,0.,0.,0.};
        float upperlimit_Lambda100[5] = {0.0230419,0.0250243,0.0288547,0.0319195,0.0393824};
        float upperlimit_Lambda120[5] = {0.0471724,0.0522344,0.0600325,0.0670999,0.0778567};
        float upperlimit_Lambda140[5] = {0.006,0.01,0.0255327,0.03,0.038};
        float upperlimit_Lambda160[5] = {0.006,0.006,0.0187741,0.022,0.03};
        float upperlimit_Lambda180[5] = {0.002,0.006,0.0138085,0.014,0.018};
        //float upperlimit500_Lambda100[5] = {0.0248624,0.0274653,0.0306991,0.0333265,0.0433285};
        //float upperlimit1000_Lambda100[5] = {0.0314197,0.034027,0.0367808,0.0392051,0.0422995};
        //float upperlimit2000_Lambda100[5] = {0.0462303,0.0491389,0.052771,0.0559793,0.0681559};
        //float upperlimit4000_Lambda100[5] = {0.0745266,0.0841812,0.0891725,0.0960313,0.100699};
        //float upperlimit6000_Lambda100[5] = {0.10191,0.110137,0.119908,0.132735,0.154897};

        float obs_lim[5]= {0.};
        float exp_lim[5]= {0.};



       /*obs_lim[0] =0.0612122; 
	obs_lim[1] =0.0460931;
	obs_lim[2] =0.0379412; 
	obs_lim[3] =0.0307229; 
 	obs_lim[4] =0.0241807; 
        */

        obs_lim[0] =0.0549363;
        obs_lim[1] =0.0445393;
        obs_lim[2] =0.0332381;
        obs_lim[3] =0.0283376;
        obs_lim[4] =0.0249918;


        exp_lim[0] = upperlimit_Lambda100[2]; 
        exp_lim[1] = upperlimit_Lambda120[2]; 
        exp_lim[2] = upperlimit_Lambda140[2]; 
        exp_lim[3] = upperlimit_Lambda160[2]; 
        exp_lim[4] = upperlimit_Lambda180[2]; 


        float theoFac = 1.39;

        float mTh[5] = {  100*theoFac,
                        120*theoFac,
                        140*theoFac,
                        160*theoFac,
                        180*theoFac};

        float xsTh[5] = {   0.2357,
                        0.0860,
                        0.0368,
                        0.0181,
                        0.0092 };

        float x_pdf[10] = { 100*theoFac,
                        120*theoFac,
                        140*theoFac,
                        160*theoFac,
                        180*theoFac,
                        180*theoFac,
                        160*theoFac,
                        140*theoFac,
                        120*theoFac,
                        100*theoFac };


         float y_pdf[10];


 for(int i = 0; i<5; i++){
   y_pdf[i] = 0.07*xsTh[i]+xsTh[i];
 }

 int j = 3;
 for(int i = 5; i<10; i++){
   y_pdf[i] = -0.07*xsTh[j]+xsTh[j];
   j--;
 }


 float y_pdf_exp[10];

 for(int ii = 0; ii<5; ii++){
   y_pdf_exp[ii] = 0.2*obs_lim[ii]+obs_lim[ii];
 }
 
 int jj = 3;

 for(int ii = 5; ii<10; ii++){
   y_pdf_exp[ii] = -0.2*obs_lim[jj]+obs_lim[jj];
   jj--;
 }


  // Green and yellow error bands 
   float y_pdf_1sig[10] = {  upperlimit_Lambda100[1],
				upperlimit_Lambda120[1],
				upperlimit_Lambda140[1],
				upperlimit_Lambda160[1],
				upperlimit_Lambda180[1],
				upperlimit_Lambda180[3],
				upperlimit_Lambda160[3],
				upperlimit_Lambda140[3],
				upperlimit_Lambda120[3],
				upperlimit_Lambda100[3] 
				};


   float y_pdf_2sig[10] = {  upperlimit_Lambda100[0],
				upperlimit_Lambda120[0],
				upperlimit_Lambda140[0],
				upperlimit_Lambda160[0],
				upperlimit_Lambda180[0],
				upperlimit_Lambda180[4],
				upperlimit_Lambda160[4],
				upperlimit_Lambda140[4],
				upperlimit_Lambda120[4],
				upperlimit_Lambda100[4] 
				};

/* 
   float y_pdf_1sig[10] = {	0.03, 
				0.034,
				0.026, 
				0.022, 
				0.018, 
				0.066, 
				0.07, 
				0.066, 
				0.058, 
				0.046 };

   float y_pdf_2sig[10] = {	0.0022, 
				0.0026, 
				0.0018, 
				0.0018, 
				0.0014, 
				0.09, 
				0.094, 
				0.086, 
				0.086, 
				0.066 };

*/


// Now Do Plotting
  
   TCanvas* c0 = new TCanvas("exclusion limit", "exclusion limit", 900, 900);
   c0->cd();
   c0->SetLogy();
   // c0->SetGridx();
   //c0->SetGridy();
  

// region excluded by Tevatron limits
   float x_shaded[8] = {	130,
				135,
				140,
				146,
				146, 
				140, 
				135, 
				130 };


   float y_shaded[8] = {	0.0007,
				0.0007,
				0.0007,
				0.0007,
				5, 
				5, 
				5, 
				5 };

 
   TGraph* Onesig_graph;
   Onesig_graph = new TGraph(10, x_pdf, y_pdf_1sig );
   Onesig_graph->SetFillStyle(1001);
   Onesig_graph->SetFillColor(kGreen);
   Onesig_graph->SetLineColor(kGreen);

   TGraph* Twosig_graph;
   Twosig_graph = new TGraph(10, x_pdf, y_pdf_2sig );
   Twosig_graph->SetFillStyle(1001);
   Twosig_graph->SetFillColor(kYellow);
   Twosig_graph->SetLineColor(kYellow);

   TMultiGraph* gAllp = new TMultiGraph;
   gAllp->Add(Twosig_graph);
   gAllp->Add(Onesig_graph);
   //gAllp->Add(exp_lim_graph);
   //gAllp->Add(ul_lim_graph);
   //Onesig_graph->Draw("FE3same");
   //Twosig_graph->Draw("FE3same");
   gAllp->Draw("E3F same");
   gAllp->GetYaxis()->SetRangeUser(1e-3, 10.);
   gAllp->GetXaxis()->SetRangeUser(140., 500);
   gAllp->GetXaxis()->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
   gAllp->GetYaxis()->SetTitle("#sigma ( #tilde{#chi^{0}_{1}} #rightarrow #tilde{G}#gamma) #times BR [pb]");
   
   TGraph* exp_lim_graph;
   exp_lim_graph  = new TGraph(5, mTh, exp_lim);
   exp_lim_graph->SetLineWidth(3);
   exp_lim_graph->SetLineStyle(3);
   exp_lim_graph->Draw("LPsame");

   TGraph* ul_lim_graph;
   ul_lim_graph  = new TGraph(5, mTh, obs_lim);
   ul_lim_graph->SetLineColor(kBlack);
   ul_lim_graph->SetLineWidth(3);
   ul_lim_graph->Draw("Csame");
  
  // Do Exclusion Limits
   TGraph *pl = new TGraph(8,x_shaded,y_shaded);
   pl->SetFillColor(kGray);
   pl->Draw("Fsame");
   //CMS Preliminary Infor
   std::string s_lumi;
   std::string CoMEr;
   s_lumi = "4.5";
   CoMEr = "#sqrt{(S)} = 7 TeV";
   std::string lint = "#intLdt= "+s_lumi+" fb^{-1}";
   TLatex l1;
   l1.SetTextAlign(12);
   l1.SetTextSize(0.035);
   l1.SetTextFont(22);
   l1.SetNDC();
   l1.DrawLatex(0.155, 0.967, "CMS Preliminary,");
   l1.DrawLatex(0.43, 0.967, CoMEr.c_str());
   l1.DrawLatex(0.75, 0.96, lint.c_str());

   // Theoretical Cross Section 
   TGraph *xsTh_vs_m = new TGraph(5, mTh, xsTh);
   xsTh_vs_m->SetLineWidth(2);
   xsTh_vs_m->SetLineColor(kRed);
   xsTh_vs_m->SetMarkerSize(1.);
   xsTh_vs_m->SetMarkerStyle(22);
   xsTh_vs_m->SetMarkerColor(kRed);
   xsTh_vs_m->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
   xsTh_vs_m->GetXaxis()->SetTitleOffset(1.5);
   xsTh_vs_m->SetTitle("#sigma (pb) ");
   xsTh_vs_m->Draw("Csame");

   // Legend
   TLegend* leg = new TLegend(0.5,0.6,0.95,0.9);
   leg->SetFillStyle(0); leg->SetBorderSize(0); 
   leg->SetFillColor(0);
 
   leg->SetHeader(("c#tau(#tilde{#chi}^{0}_{1})  = "+ctau+" mm").c_str());
   leg ->SetTextFont(22);
   leg->AddEntry(pl,"CDF exclusion (2.6 fb^{-1})","f");
   leg->AddEntry(xsTh_vs_m,"Theoretical LO cross-section","l");
   leg->AddEntry(ul_lim_graph, "Observed  95% CL upper limit", "L");
   leg->AddEntry(exp_lim_graph, "Expected 95% CL upper limit", "L");
   leg->AddEntry(Onesig_graph, "#pm 1 #sigma Expected", "F");
   leg->AddEntry(Twosig_graph, "#pm 2 #sigma Expected", "F");
   leg->Draw("same");
   //c0->SaveAs(("exclusion_limit_"+ctau+".eps").c_str());
   c0->SaveAs(("Exclusion_limit_"+ctau+".png").c_str());
   //c0->SaveAs(("exclusion_limit_"+ctau+".pdf").c_str());
   //c0->SaveAs(("exclusion_limit_"+ctau+".eps").c_str());
}

//***********************************************************************///
//                                                                      *///
//      Draw_Limit() Xsec Vs ctau                             *///
//                                                                      */// 
//***********************************************************************///
void Limit_Draw(const std::string fileName, const int Nmod) {
  
  bool writeExtraText = true;       // if extra text
  std::string extraText  = "Preliminary";  // default extra text is "Preliminary"
  std::string lumi_8TeV  = "19.1 fb^{-1}"; // default is "19.7 fb^{-1}"
  std::string lumi_7TeV  = "4.9 fb^{-1}";  // default is "5.1 fb^{-1}"
  int iPeriod = 2;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV 
  int iPos=11; 
  // second parameter in example_plot is iPos, which drives the position of the CMS logo in the plot
  //iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally : 

   const int NumModels = Nmod ;
   cout<<"Using limitlogfile from HiggsCombine:" << fileName << "  with " << NumModels << " models "<< endl ;
   //Input->GetParameters("TestModel", &testModel ) ;
 
   //vector<double> ctauModel ; // Each ctau is a GMSB model
   //vector<double> lambdaModel; 

   float MedianExp[NumModels];
   float Up1sigma[NumModels];
   float Up2sigma[NumModels];
   float Down1sigma[NumModels];
   float Down2sigma[NumModels];
   float Observed[NumModels];
   float sigXsec[NumModels];
   float Ctau[NumModels];
   float Lambda[NumModels];
   float CtauError[NumModels] ;
   
/*   //Method 1 to Read File
   std::ifstream fitlog(fileName, ios::in);
   if(!fitlog.good()) cout <<"Error Opening File.. try another" << endl;
   vector<float> MedE, Up1, Up2, D1, D2, OBS, SIGX, cTAU, LMDA, cTAUEr, Eff, BR, LUMI;
   FInputLine fline;
   while (fitlog.good()) 
   { 
     fitlog >> fline;
     OBS.push_back(fline.Obs_limit); 
     D2.push_back(fline.TwoSigD);
     D1.push_back(fline.OneSigD);
     MedE.push_back(fline.mexp); 
     Up1.push_back(fline.OneSigUp);
     Up2.push_back(fline.TwoSigUp);
     Eff.push_back(fline.eff);
     SIGX.push_back(fline.sgXsec);
     cTAU.push_back(fline.ctau);
     LMDA.push_back(fline.lambda);
     BR.push_back(fline.Bratio);
     LUMI.push_back(fline.lumi);
     cTAUEr.push_back(0.0);     
   }
   // Now Loop Through Number of Models 
  for ( int i=0; i< NumModels ; i++ ) {

       CtauError[i] = cTAUEr[i];
       Ctau[i] = cTAU[i]; //ctauModel[i] ;
       Lambda[i] = LMDA[i]; //lambdaModel[i] ;
       Up2sigma[i] = Up2[i] / (Eff[i]*LUMI[i]) ;
       Up1sigma[i] = Up1[i] / (Eff[i]*LUMI[i]) ;
       MedianExp[i] = MedE[i] / (Eff[i]*LUMI[i]) ;
       Down1sigma[i] = D1[i] / (Eff[i]*LUMI[i]) ;
       Down2sigma[i] = D2[i] / (Eff[i]*LUMI[i]) ;
       Observed[i] = OBS[i] / (Eff[i]*LUMI[i]) ;
       sigXsec[i] = SIGX[i] ;
       
       printf(" mexp: %f, eff: %f , MedianExp: %f \n", MedE[i], Eff[i], MedianExp[i] ) ;
       printf(" %f, %f, %f, %f, %f \n", D2[i], D1[i], MedE[i], Up1[i], Up2[i] ) ;
     }  
  */
   // Method 2 : read file old way!
   FILE* fitlog = fopen(fileName.c_str(),"r+");
   int read ;
   float  Obs_limit, TwoSigD, OneSigD, mexp, OneSigUp, TwoSigUp, eff, sgXsec, ctau, lambda, Bratio, lumi;
   //float ep;
   for ( int i=0; i< NumModels ; i++ ) {

       read = fscanf(fitlog, "%f", &Obs_limit );
       read = fscanf(fitlog, "%f", &OneSigD );  // Reading From file is two sig Down
       read = fscanf(fitlog, "%f", &TwoSigD );  // Reading from file is 1 Sig Down
       read = fscanf(fitlog, "%f", &mexp );
       read = fscanf(fitlog, "%f", &OneSigUp );
       read = fscanf(fitlog, "%f", &TwoSigUp );
       read = fscanf(fitlog, "%f", &eff );
       read = fscanf(fitlog, "%f", &sgXsec);
       read = fscanf(fitlog, "%f", &ctau);
       read = fscanf(fitlog, "%f", &lambda);
       read = fscanf(fitlog, "%f", &Bratio);
       read = fscanf(fitlog, "%f", &lumi);
       //read = fscanf(fitlog, "%f", &ep );
       
       CtauError[i] = 0. ;
       Ctau[i] = ctau; //ctauModel[i] ;
       Lambda[i] = lambda; //lambdaModel[i] ;
       Up2sigma[i] = TwoSigUp*sgXsec/eff; //(eff*lumi) ;
       Up1sigma[i] = OneSigUp*sgXsec/eff; //(eff*lumi) ;
       MedianExp[i] =mexp*sgXsec/eff; //(eff*lumi) ;
       Down2sigma[i] =TwoSigD*sgXsec/eff; //(eff*lumi) ;
       Down1sigma[i] =OneSigD*sgXsec/eff; //(eff*lumi) ;
       Observed[i] = Obs_limit*sgXsec/eff; //(eff*lumi) ;
       //aep[i] = ep / (eff*lumi) ;
       sigXsec[i] = sgXsec ;
       //printf(" mexp: %f, eff: %f , MedianExp: %f \n", mexp, eff, MedianExp[i] ) ;
       //printf(" %f, %f, %f, %f, %f \n", TwoSigD, OneSigD, mexp, OneSigUp, TwoSigUp ) ;
       if ( read != 1 ) cout<<" reading error "<<endl ;
   }
  

   //TCanvas *c1a = new TCanvas("c1a","c1a",200,10,600,400);
   TCanvas *c1a = new TCanvas("c1a","c1a",200,100,900,900);
   c1a->SetLogy() ;

   TGraphAsymmErrors* g2sig = new TGraphAsymmErrors(NumModels, Ctau, MedianExp, CtauError, CtauError, Down2sigma, Up2sigma);
   g2sig->SetFillStyle(1001);
   //g2sig->SetFillColor(kSpring);
   g2sig->SetFillColor(kYellow);
   g2sig->SetLineColor(kYellow);
   //g2sig->GetXaxis()->SetRangeUser(1.,5.) ;
   //g2sig->GetXaxis()->SetLimits(1.,5.) ;
   
   TGraphAsymmErrors* g1sig = new TGraphAsymmErrors(NumModels, Ctau, MedianExp, CtauError, CtauError, Down1sigma, Up1sigma);
   g1sig->SetFillStyle(1001);
   g1sig->SetFillColor(kGreen);
   g1sig->SetLineColor(kGreen) ;
   //g2sig->GetXaxis()->SetLimits(1.,5.) ;
   
   TMultiGraph *gAllp = new TMultiGraph();
   gAllp->SetTitle( " 95% CLs Limit " );
   //gAllp->Draw("LPsame") ;
   gAllp->Add( g2sig ) ;
   gAllp->Add( g1sig ) ;
   g2sig->Draw("E3");
   g1sig->Draw("E3");
   gAllp->Draw("LPE3 sames") ;
   gPad->Modified();
   //gAllp->GetXaxis()->SetLimits( Ctau[0], Ctau[NumModels-1] ) ;
   //gAllp->GetXaxis()->SetLimits( Ctau[0], 6500);//Ctau[NumModels-1] ) ;
   gAllp->GetXaxis()->SetLimits( Ctau[0], 13000);//Ctau[NumModels-1] ) ;
   gAllp->SetMaximum(1.5);
   gAllp->SetMinimum(1e-3);
   gAllp->GetXaxis()->SetTitleOffset(1.);
   gAllp->GetYaxis()->SetTitleOffset(1.);
   gAllp->GetXaxis()->SetTitleFont(42);
   gAllp->GetYaxis()->SetTitleFont(42);
   gAllp->GetXaxis()->SetTitleSize(0.05);
   gAllp->GetYaxis()->SetTitleSize(0.07);
   gAllp->GetXaxis()->SetTitle("Neutralino Proper Decay Length[mm]"); //c#tau_{#chi^{0}_{1}}[mm]" ) ;
   gAllp->GetYaxis()->SetTitle("#sigma_{UL} (pb) @ 95% CL_{s}") ;

   // expected
   TGraph* gMExp = new TGraph(NumModels, Ctau, MedianExp ) ;
   gMExp->SetLineColor(kRed) ;
   gMExp->SetLineWidth(3) ;
   gMExp->SetLineStyle(7) ;
   //gMExp->GetXaxis()->SetLimits(1.,5.) ;

   gMExp->Draw("LPsame") ;
   // observation
   TGraph* gObs = new TGraph(NumModels, Ctau, Observed ) ;
   gObs->SetLineColor(kBlack) ;
   gObs->SetLineWidth(2) ;
   gObs->SetLineStyle(1) ;
   //gObs->GetXaxis()->SetLimits(1.,5.) ;
   //gObs->SetMarkerStyle(20) ;
  gObs->Draw("LPsame");




   std::string s_lumi;
   std::string CoMEr;
   s_lumi = "19.1";
   //CoMEr = "#sqrt{(S)} = 8 TeV";
   CoMEr = "(8 TeV)";
   //std::string lint = "#int Ldt= "+s_lumi+" fb^{-1}";
   std::string lint = s_lumi + "fb^{-1}"+ CoMEr;
   TLatex l1;
   l1.SetTextAlign(12);
   l1.SetTextSize(0.035);
   l1.SetTextFont(22);
   l1.SetTextAngle(0);
   l1.SetTextColor(kBlack);    
   l1.SetNDC();
  // l1.DrawLatex(0.155, 0.967, "CMS Preliminary");
  // l1.DrawLatex(0.43, 0.967, CoMEr.c_str());
  // l1.DrawLatex(0.75, 0.96, lint.c_str());
   
   l1.DrawLatex(0.20, 0.922, "CMS");
   l1.DrawLatex(0.20, 0.885, "Preliminary");
   l1.DrawLatex(0.75, 0.967, lint.c_str());
    int n_ = 2;
    float x1_l = 0.92;
    float y1_l = 0.60;

    float dx_l = 0.30;
    float dy_l = 0.18;
    float x0_l = x1_l-dx_l;
    float y0_l = y1_l-dy_l;

    TPad* pad_0 = new TPad("pad_0","pad_0",x0_l,y0_l,x1_l, y1_l );
    pad_0->SetFillColor( kGray );
    // CMS_lumi(pad_0, iPeriod, iPos );
    c1a->Update() ;

   // theoratical suggestion
   TGraph* gth = new TGraph(NumModels, Ctau, sigXsec ) ;
   gth->SetLineColor(kBlue) ;
   gth->SetLineWidth(3) ;
   gth->SetLineStyle(1) ;
   gth->GetXaxis()->SetLimits( Ctau[0],6500);// Ctau[NumModels-1] ) ;
   
   // Draw Graphs
   gth->Draw("Csame") ;
   // Legend
   std::string Tctau ="6000";
   TLegend* leg1 = new TLegend(.37, .57, .95, .85 );
   leg1->SetHeader("#tilde{#chi}^{0}_{1} #rightarrow #gamma #tilde{G}"); // m_{#tilde{#chi}^{0}_{1}}= 256 GeV");
   leg1->SetFillStyle(0); 
   leg1->SetBorderSize(0);
   leg1->SetFillColor(0);
   leg1 ->SetTextFont(22);
  // leg1->SetFillColor(10) ;
   //leg1->SetTextSize(0.030) ;
   leg1->AddEntry( gth, "Theory LO cross-section" , "L");
   leg1->AddEntry( gMExp,"Expected 95% CL Upper Limit", "L");
   leg1->AddEntry( gObs, "Observed  95% CL Upper Limit", "L");
   leg1->AddEntry( g1sig, "#pm 1#sigma Expected" , "F");
   leg1->AddEntry( g2sig, "#pm 2#sigma Expected" , "F");
   leg1->Draw("same") ;
   
   c1a->RedrawAxis();
   c1a->GetFrame()->Draw();
   TString gPlotname = "Neutralino_CrossSecTimesBR_Uplimit" ; 
   //TString gPlotname = "XsecTimesBR_Uplimit.pdf";
   c1a->Print( gPlotname+ ".pdf",".pdf") ;
   c1a->Print( gPlotname+ ".png",".png") ;
   c1a->Print( gPlotname+ ".eps",".eps") ;
   c1a->Draw();
}



//Use cms style
void rootlogon()
{
   gROOT->SetStyle("Plain");
   gStyle->SetPalette(1);
   gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
   gStyle->SetOptFit(111110);
   gStyle->SetOptFile(1);
   gStyle->SetMarkerStyle(20);
   gStyle->SetMarkerSize(2.);
   gStyle->SetMarkerColor(1);
   gStyle->SetTitleOffset(1.9,"Y");
   gStyle->SetTitleOffset(1.9,"X");
   gStyle->SetTitleColor(1, "XYZ");
   gStyle->SetTitleFont(42, "XYZ");
   gStyle->SetTitleSize(0.08, "XYZ");
   //gStyle->SetTitleXOffset(0.9);
   //gStyle->SetTitleYOffset(1.20);
    //define high def color palette
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    cout << "loading TDR style and setting as default" << endl;
    gROOT->ProcessLine(".L tdrstyle.C");
    setTDRStyle();
}
