///
//**************************************
//Simple Script to plot  Mass, ctau and 2D_ upper Limits
// run as: doPlot CTAU
//***************************************
//**                         ************
//** Designed by TEN@UMN.EDU ************
//**                         ************
//***************************************
//
#include "TColor.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGaxis.h"
#include "TPaveText.h"
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
//#include "CMS_lumi.h"
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
void XsecVsMassDraw(std::string, std::string); // Xsec Vs Mass

//void XsecVsMassDraw(std::string); // Xsec Vs Mass
void rootlogon();

void XsecVsCtauDraw(const std:: string LambdaValue,const std::string filename, const int nModel); // xsec Vs ctau

// Label Limits fxns
std::string get_sqrtText();

TPaveText* get_labelSqrt( int legendQuadrant );

TPaveText* get_labelCMS( int legendQuadrant , std::string year, bool sim, std::string run);

void MassVsCtau_exclusion() ; // make 2D_Limits: Mass/Lambda Vs Ctau


//***************************************************************//
//
//***     MAIN FXN    *******************************************//
//
//
// **************************************************************//
#ifndef __CINT__
int main(int argc, char* argv[] )  
{
   // Make sure at Least two arguments are given
  
     if( argc < 4 ) { 
                   std::cout <<"Oops! Run Program as: >>  ./doplots CTAU limitfilename NumberOfDifferentCTAU  LambdaValue" << std::endl; exit(1);
		  } 
  		  
   std::string ctau = argv[1]; // Given Ctau u want shown on plots
   const std::string limitfilename = argv[2];
   const int  nModels = (int)atoi(argv[3]);
   const std::string LambdaValue = argv[4];
   //Run CMS ploting Style:
   rootlogon();
   //Draw Xsec*BR Vs Mass/Lambda
   XsecVsMassDraw(ctau, LambdaValue);
  // Draw Xsec*BR Vs ctau Limit
   XsecVsCtauDraw( LambdaValue, limitfilename, nModels);
  
  //** Mass Vs Ctau Limit **//
   MassVsCtau_exclusion() ; 
}
#endif

//***********************************************************************///
//                                                                      *///
//      Draw_Limit() Xsec Vs ctau                             *///
//                                                                      */// 
//***********************************************************************///
void XsecVsCtauDraw( const std::string LambdaValue, const std::string fileName, const int Nmod) {
  
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
  // gAllp->GetXaxis()->SetLimits( Ctau[0], Ctau[NumModels-1] ) ;
   //gAllp->GetXaxis()->SetLimits( Ctau[0], 6500);//Ctau[NumModels-1] ) ;
   gAllp->GetXaxis()->SetLimits(50, 2500);//Ctau[NumModels-1] ) ;
   gAllp->SetMaximum(100.0);
   gAllp->SetMinimum(2e-3);
   gAllp->GetXaxis()->SetTitleOffset(1.);
   gAllp->GetYaxis()->SetTitleOffset(1.);
   gAllp->GetXaxis()->SetTitleFont(42);
   gAllp->GetYaxis()->SetTitleFont(42);
   gAllp->GetXaxis()->SetTitleSize(0.05);
   gAllp->GetYaxis()->SetTitleSize(0.07);
   gAllp->GetXaxis()->SetTitle("Neutralino Proper Decay Length[mm]"); //c#tau_{#chi^{0}_{1}}[mm]" ) ;
   //gAllp->GetYaxis()->SetTitle("#sigma_{UL} (pb) @ 95% CL_{s}") ;
   gAllp->GetYaxis()->SetTitle("#sigma (#tilde{#chi^{0}_{1}} #rightarrow #tilde{G}#gamma) #times BR [pb]");

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
   TString gPlotname = LambdaValue+"TeV_Neutralino_CrossSecTimesBR_Uplimit" ; 
   //TString gPlotname = "XsecTimesBR_Uplimit.pdf";
   c1a->Print( gPlotname+ ".pdf",".pdf") ;
   c1a->Print( gPlotname+ ".png",".png") ;
   c1a->Print( gPlotname+ ".eps",".eps") ;
   c1a->Draw();
}

//***********************************************************************///
//                                                                      *///
//      PLot Xsec Vs Mass Limit here!                                                *///
//                                                                      */// 
//***********************************************************************///
void XsecVsMassDraw(std::string ctau, std::string LambdaValue)
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

        float mTh[5] = { 100*theoFac, 120*theoFac, 140*theoFac, 160*theoFac, 180*theoFac};

        float xsTh[5] = { 0.2357, 0.0860, 0.0368, 0.0181, 0.0092 };  //Production Cross section Lambda 100 - 180 TeV
       // float xsTh[4] = { 0.368, 0.133, 0.0574, 0.0145 };  //Production Cross section Lambda 100 - 180 TeV @ 8 TeV Collision

        float x_pdf[10] = { 100*theoFac, 120*theoFac, 140*theoFac, 160*theoFac, 180*theoFac, 180*theoFac, 160*theoFac, 140*theoFac, 120*theoFac,100*theoFac };

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
   float y_pdf_1sig[10] = { upperlimit_Lambda100[1], upperlimit_Lambda120[1], upperlimit_Lambda140[1], upperlimit_Lambda160[1], upperlimit_Lambda180[1], upperlimit_Lambda180[3], upperlimit_Lambda160[3], upperlimit_Lambda140[3], upperlimit_Lambda120[3], upperlimit_Lambda100[3] };


   float y_pdf_2sig[10] = { upperlimit_Lambda100[0], upperlimit_Lambda120[0], upperlimit_Lambda140[0], upperlimit_Lambda160[0], upperlimit_Lambda180[0], upperlimit_Lambda180[4], upperlimit_Lambda160[4], upperlimit_Lambda140[4], upperlimit_Lambda120[4], upperlimit_Lambda100[4] };

/* 
   float y_pdf_1sig[10] = {0.03, 0.034, 0.026, 0.022, 0.018, 0.066, 0.07, 0.066, 0.058, 0.046 };

   float y_pdf_2sig[10] = {0.0022, 0.0026, 0.0018, 0.0018, 0.0014, 0.09, 0.094, 0.086, 0.086,0.066 };
*/

// Now Do Plotting
   TCanvas* c0 = new TCanvas("Mass exclusion limit", "Mass exclusion limit", 900, 900);
   c0->cd();
   c0->SetLogy();
   // c0->SetGridx();
   //c0->SetGridy();

// region excluded by Tevatron limits
   float x_shaded[8] = {130,135,140,146,146,140,135,130 };

   float y_shaded[8] = {0.0007,0.0007,0.0007,0.0007,5, 5, 5, 5 };
 
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
   gAllp->GetXaxis()->SetTitle("Neutralino Mass [GeV/c^{2}]");
   //gAllp->GetXaxis()->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
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
   s_lumi = "4.86";
   CoMEr = "#sqrt{(S)} = 7 TeV";
   //std::string lint = "#int Ldt= "+s_lumi+" fb^{-1}";
   std::string lint = s_lumi + "fb^{-1}"+ CoMEr;
   TLatex lg;
   lg.SetTextAlign(12);
   lg.SetTextSize(0.038);
   lg.SetTextFont(22);
   lg.SetTextAngle(0);
   lg.SetTextColor(kBlack);    
   lg.SetNDC();
  // lg.DrawLatex(0.155, 0.967, "CMS Preliminary");
  // lg.DrawLatex(0.43, 0.967, CoMEr.c_str());
  // lg.DrawLatex(0.75, 0.96, lint.c_str());
   lg.DrawLatex(0.21, 0.922, "CMS");
   lg.DrawLatex(0.21, 0.885, "Preliminary");
   lg.DrawLatex(0.70, 0.967, lint.c_str());

   // Theoretical Cross Section 
   TGraph *xsTh_vs_m = new TGraph(4, mTh, xsTh);
   xsTh_vs_m->SetLineWidth(4);
   xsTh_vs_m->SetLineColor(kBlue);
   xsTh_vs_m->SetMarkerSize(1.);
   xsTh_vs_m->SetMarkerStyle(22);
   xsTh_vs_m->SetMarkerColor(kRed);
   xsTh_vs_m->SetTitle("Neutralino Mass [GeV/c^{2}]");
   //xsTh_vs_m->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
   xsTh_vs_m->GetXaxis()->SetTitleOffset(1.5);
   xsTh_vs_m->SetTitle("#sigma (pb) ");
   xsTh_vs_m->Draw("Csame");

   // Legend
   TLegend* leg = new TLegend(0.47,0.55,0.91,0.93);
   leg->SetFillStyle(0); leg->SetBorderSize(0); 
   leg->SetFillColor(0);
 
   leg->SetHeader(("c#tau(#tilde{#chi}^{0}_{1})  = "+ctau+" mm").c_str());
   leg->SetTextFont(22);
   leg->SetTextSize(0.030) ;
   leg->AddEntry(pl,"CDF exclusion (2.6 fb^{-1})","f");
   leg->AddEntry(xsTh_vs_m,"Theoretical LO cross-section","l");
   leg->AddEntry(ul_lim_graph, "Observed  95% CL upper limit", "L");
   leg->AddEntry(exp_lim_graph, "Expected 95% CL upper limit", "L");
   leg->AddEntry(Onesig_graph, "#pm 1 #sigma Expected", "F");
   leg->AddEntry(Twosig_graph, "#pm 2 #sigma Expected", "F");
   leg->Draw("same");
   //c0->SaveAs(("exclusion_limit_"+ctau+".eps").c_str());
   c0->SaveAs(( LambdaValue+ "TeV_Neutralino_XsecVsMAss_Exclusion_limit_"+ctau+".png").c_str());
   //c0->SaveAs((LambdaValue+ "TeV_Neutralino_XsecVsMass_Exclusion_limit_"+ctau+".pdf").c_str());
   //c0->SaveAs(("LambdaValue+ "TeV_Neutralino_XsecVsMass_Exclusion_limit_"+ctau+".eps").c_str());
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

//
//
//** Get Text fxn ****//
//

std::string get_sqrtText() {
   
   char label_sqrt_text[150];
  
    sprintf( label_sqrt_text, "#sqrt{s} = 8 TeV");
    std::string returnString(label_sqrt_text);

  return returnString;
}
//
//***                   *****///
//*** Adding CMS Labels *****///
//***                   *****///
//

TPaveText* get_labelCMS( int legendQuadrant = 0 , std::string year="2012", bool sim=false, std::string run = "ALL") {

  if( legendQuadrant!=0 && legendQuadrant!=1 && legendQuadrant!=2 && legendQuadrant!=3 ) {
    std::cout << "WARNING! Legend quadrant '" << legendQuadrant << "' not yet implemented for CMS label. Using 2." << std::endl;
    legendQuadrant = 2;
  }

  float x1, y1, x2, y2;
  if( legendQuadrant==1 ) {
    x1 = 0.63;
    y1 = 0.83;
    x2 = 0.8;
    y2 = 0.87;
  } else if( legendQuadrant==2 ) {
    x1 =  0.25;
    y1 = 0.83;
    x2 =  0.42;
    y2 = 0.87;
  } else if( legendQuadrant==3 ) {
    x1 = 0.25;
    y1 = 0.2;
    x2 = 0.42;
    y2 = 0.24;
  } else if( legendQuadrant==0 ) {
    x1 = 0.175;
    y1 = 0.953;
    x2 = 0.6;
    y2 = 0.975;
  }

  
  TPaveText* cmslabel = new TPaveText( x1, y1, x2, y2, "brNDC" );
  cmslabel->SetFillColor(kWhite);
  cmslabel->SetTextSize(0.038);
  if( legendQuadrant==0 ) cmslabel->SetTextAlign(11);
  cmslabel->SetTextSize(0.038);
  cmslabel->SetTextFont(22);
 
    std::string leftText;
   
    if(year == "2010")  leftText = "CMS Preliminary 2010, 34 pb^{-1}";
    if(year == "2012")  leftText = "CMS Preliminary 2012, 19.1 fb^{-1}";
    if (sim)  leftText = "CMS Simulation 2011";
    else {
      if(year == "2011" && run == "RUN2011A")  leftText = "CMS Preliminary RUN2011A 2.034 fb^{-1}";
      if(year == "2011" && run == "RUN2011B")  leftText = "CMS Preliminary 2011, 2.516 fb^{-1}";
      if(year == "2011" && run == "ALL")  leftText = "CMS Preliminary 2011, 4.86 fb^{-1}";
      //if(year == "2011" && run == "ALL")  leftText = "CMS Preliminary 2011, 4.86 fb^{-1}";
      if(year == "2011" && run == "ALL")  leftText = "CMS";
      if(year == "May2011")leftText = "CMS Preliminary 2011, 858.4 pb^{-1}";

    }
    cmslabel->AddText(leftText.c_str());
    return cmslabel;

}

//
//**** Adding CMassEr ***//
//
TPaveText* get_labelSqrt( int legendQuadrant ) {

  if( legendQuadrant!=0 && legendQuadrant!=1 && legendQuadrant!=2 && legendQuadrant!=3 ) {
    std::cout << "WARNING! Legend quadrant '" << legendQuadrant << "' not yet implemented for Sqrt label. Using 2." << std::endl;
    legendQuadrant = 2;
  }


  float x1, y1, x2, y2;
  if( legendQuadrant==1 ) {
    x1 = 0.63;
    y1 = 0.78;
    x2 = 0.8;
    y2 = 0.82;
  } else if( legendQuadrant==2 ) {
    x1 = 0.25;
    y1 = 0.78;
    x2 = 0.42;
    y2 = 0.82;
  } else if( legendQuadrant==3 ) {
    x1 = 0.25;
    y1 = 0.16;
    x2 = 0.42;
    y2 = 0.2;
  } else if( legendQuadrant==0 ) {
    x1 = 0.67;
    y1 = 0.953;
    x2 = 0.86;
    y2 = 0.975;
  }


  TPaveText* label_sqrt = new TPaveText(x1,y1,x2,y2, "brNDC");
  label_sqrt->SetFillColor(kWhite);
  label_sqrt->SetTextSize(0.038);
  label_sqrt->SetTextFont(22);
  label_sqrt->SetTextAlign(31); // align right
  label_sqrt->AddText("4.86 fb^{-1}(7 TeV)");
  //label_sqrt->AddText("#sqrt{s} = 7 TeV");
  return label_sqrt;
}

//void make2D(){
void MassVsCtau_exclusion(){

  TPaveText* label_cms = get_labelCMS(0,"2011", false, "ALL");
  TPaveText* label_sqrt = get_labelSqrt(0);

  //**************************observed cms*******************************//
  Double_t obs_lim_cms[16]={100., 145., 157., 179., 192., 216., 221., 218., 218., 221., 216., 192., 179., 157., 145., 100.};

  Double_t ctau[16]={0., 0., 0., 0., 0., 0., 0., 0., 1., 250., 500., 1000., 2000., 4000., 6000., 6000.};

  TGraph* obs_exclusion_cms;
  obs_exclusion_cms = new TGraph(16, obs_lim_cms,ctau);
  obs_exclusion_cms->SetFillColor(5);
  obs_exclusion_cms->SetLineColor(0);
  obs_exclusion_cms->SetLineWidth(0);

//**************************expecteded cms*******************************//
  Double_t exp_lim_cms[16]={100., 148., 160., 182., 199., 218., 221., 221., 221., 221., 218., 199., 182., 160., 148., 100.};

  TGraph* exp_exclusion_cms;
  exp_exclusion_cms = new TGraph(16, exp_lim_cms,ctau );
//  exp_exclusion_cms->SetFillStyle(3022);
//  exp_exclusion_cms->SetFillColor(kOrange+7);
//  exp_exclusion_cms->SetLineColor(kOrange+7);
  exp_exclusion_cms->SetLineWidth(1);
  exp_exclusion_cms->SetLineStyle(2);

//**************************observed cdf*******************************//
  Double_t obs_lim_cdf[6]={100., 146., 149.5, 149.5, 146., 100.};
  Double_t lifetime_cdf[6]={0., 0., 0., 300., 600., 600.};


  TGraph* obs_exclusion_cdf;
  obs_exclusion_cdf = new TGraph(6, obs_lim_cdf,lifetime_cdf );
  obs_exclusion_cdf->SetFillColor(29);
  obs_exclusion_cdf->SetFillStyle(3144);
  obs_exclusion_cdf->SetLineColor(0);
  obs_exclusion_cdf->SetLineWidth(0);

//**************************observed D0*******************************//
  Double_t obs_lim_d0[4]={100., 175., 175., 100.};
  Double_t lifetime_d0[4]={0., 0., 0.9, 0.9};

 

  TGraph* obs_exclusion_d0;
  obs_exclusion_d0 = new TGraph(4, obs_lim_d0,lifetime_d0 );
  obs_exclusion_d0->SetFillColor(9);
  obs_exclusion_d0->SetLineColor(0);
  obs_exclusion_d0->SetLineWidth(0);
//  obs_exclusion_d0->SetFillStyle(3144);



//**************************observed conversion*******************************//
  Double_t obs_lim_conv[4]={100., 140., 140., 100.};
  Double_t lifetime_conv[4]={0., 0., 30., 30.};

 

  TGraph* obs_exclusion_conv;
  obs_exclusion_conv = new TGraph(4, obs_lim_conv,lifetime_conv );
  obs_exclusion_conv->SetFillColor(46);
  obs_exclusion_conv->SetLineColor(0);
  obs_exclusion_conv->SetLineWidth(0);
  obs_exclusion_conv->SetFillStyle(3001);

  // Now Draw all Limits same Canvas
  TCanvas* c0 = new TCanvas("2D_exclusion limit", "2D_exclusion limit", 1);
  c0->cd();
  // c0->SetLogy();
  //c0->SetGridx();
  //c0->SetGridy();


    TPad * pad2 = new TPad("pad2", "pad2",0.01,0.0001,0.99,0.99);

    pad2->Draw("AH");
    //pad2->SetBorderSize(0.1);
    pad2->cd();
    /* 
    TH2F *h3 = new TH2F("h3","Axes",100,100,300,100,0,100);
    h3->SetLineColor(kWhite);   
    h3->SetLineWidth(0.);   
    h3->Draw("L");
    */
    TPad * pad1 = new TPad("pad1", "pad1",0.01,0.002,0.99,0.99);
    
    pad1->SetLogy();
    pad1->Draw();
    pad1->cd();
    
    TH2F *h2 = new TH2F("h","Axes",100,100,250,100,0.7,1E8);
    //h2->GetXaxis()->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
    //h2->GetYaxis()->SetTitle("c#tau_{#tilde{#chi^{0}_{1}}} [mm]");
    h2->GetXaxis()->SetTitle("Neutralino Mass [GeV/c^{2}]");
    h2->GetYaxis()->SetTitle("Neutralino Proper Decay Length [mm]");
    h2->GetXaxis()->SetTitleSize(0.048);
    h2->GetXaxis()->SetTitleOffset(1.24);
    h2->GetYaxis()->SetTitleSize(0.048);
    h2->GetYaxis()->SetTitleOffset(1.6);
    
    h2->Draw("");
    obs_exclusion_cms->Draw("LF");
    obs_exclusion_cdf->Draw("LFsame");
    obs_exclusion_conv->Draw("LFsame");
    obs_exclusion_d0->Draw("LFsame");
    exp_exclusion_cms->Draw("Lsame");
    h2->Draw("sameaxis");

   // Labelling SPS8 mGMSB Model
   TPaveText *pt = new TPaveText(0.45,0.26,0.65,0.60,"NDC");
   pt = new TPaveText(0.4551193,0.2499166,0.6555865,0.3643191,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(5);
   pt->SetTextAlign(12);
   pt->SetTextFont(22);
   pt->SetTextSize(0.030) ;
   //pt->SetTextColor(kGray+2);
   pt->SetTextColor(kBlack);
   pt->AddText("#font[42]{SPS8}"); 
   pt->AddText("#font[42]{GMSB #tilde{#chi}^{0}_{1}#rightarrow #gamma #tilde{G}}");
   pt->AddText("#font[42]{M_{m} = 2#Lambda, tan(#beta)=15}");
   pt->AddText("#font[42]{N_{m} = 1, #mu > 0}");

   TLegend* leg = new TLegend(0.24, 0.54, 0.90, 0.90);
   leg->SetFillStyle(0); leg->SetBorderSize(0); 
   leg->SetFillColor(0);
   leg->SetTextSize(0.030) ;
    
   label_cms->Draw("same");
   label_sqrt->Draw("same");
   pt->Draw("same");

 
   leg ->SetTextFont(22);
   leg->AddEntry(exp_exclusion_cms, "CMS expected with #gamma + Jets (4.86 fb^{-1})", "l");
   leg->AddEntry(obs_exclusion_cms, "CMS observed with #gamma + Jets (4.86 fb^{-1})", "f");
   leg->AddEntry(obs_exclusion_conv,"CMS with #gamma (conversions) + #slash{E}_{T} + Jets (2.1 fb^{-1})","f");
   leg->AddEntry(obs_exclusion_cdf,"CDF with #gamma #gamma + #slash{E}_{T} + Jets (2.6 fb^{-1})","f"); 
   leg->AddEntry(obs_exclusion_d0,"D#slash{O} with prompt #gamma #gamma + #slash{E}_{T} (6.3 fb^{-1})","f");
  
   leg->Draw("same");

   c0->SaveAs("Neutralino_2D_exclusion.png");
   c0->SaveAs("Neutralino_2D_exclusion.pdf");
   c0->SaveAs("Neutralino_2D_exclusion.eps");

}
