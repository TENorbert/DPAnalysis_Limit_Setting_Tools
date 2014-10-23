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
#include "TF1.h"
#include "TGaxis.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TPolyLine.h"
#include "TROOT.h"
//#include "tdrstyle.C"
#include "TGraph2D.h"
#include "TAxis.h"
#include "TList.h"
#include "TGraph.h"
#include "TLine.h"
#include "TBox.h"
#include "TH2.h"
#include <vector>
#include <algorithm>
#include "Riostream.h"
#include "TGraphAsymmErrors.h"


// User Define Struct
struct XYZ {
double x, y;
float z;
bool operator == (const XYZ& v) const
{
return (x == v.x) && (y == v.y);
}
};

/*
struct LimitsResults {
TGraph2D* limit_gr;
TGraph2D* xs_gr;
TGraph2D* diff_gr;
TGraph* cont0;
};
*/



void rootlogon();

void rootlogon() {
   gROOT->SetStyle("Plain");
   gStyle->SetPalette(1);
   gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
   gStyle->SetOptFit(111110);
   gStyle->SetOptFile(1);
   gStyle->SetMarkerStyle(20);
   gStyle->SetMarkerSize(2.);
   gStyle->SetMarkerColor(1);
   gStyle->SetTitleOffset(1.20,"Y");

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
     // setTDRStyle();

}


std::string get_sqrtText() {

   char label_sqrt_text[150];
  
    sprintf( label_sqrt_text, "#sqrt{s} = 8 TeV");
    std::string returnString(label_sqrt_text);

  return returnString;

}


TPaveText* get_labelCMS( int legendQuadrant = 0 , std::string year="2011", bool sim=false, std::string run = "RUN2011A") {

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
  cmslabel->SetTextFont(42);
 
    std::string leftText;
   
    if(year == "2010")  leftText = "CMS Preliminary 2010, 34 pb^{-1}";
    if (sim)  leftText = "CMS Simulation 2011";
    else {
      if(year == "2011" && run == "RUN2011A")  leftText = "CMS Preliminary RUN2011A 2.034 fb^{-1}";
      if(year == "2011" && run == "RUN2011B")  leftText = "CMS Preliminary 2011, 2.516 fb^{-1}";
      if(year == "2011" && run == "ALL")  leftText = "CMS Preliminary 2011, 4.86 fb^{-1}";
      if(year == "May2011")leftText = "CMS Preliminary 2011, 858.4 pb^{-1}";

    }
    cmslabel->AddText(leftText.c_str());
    return cmslabel;

}

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
  label_sqrt->SetTextFont(42);
  label_sqrt->SetTextAlign(31); // align right
  label_sqrt->AddText("#sqrt{s} = 8 TeV");
  return label_sqrt;
}


//void make2D(){
void Exclusion_Limit(){

  TPaveText* label_cms = get_labelCMS(0,"2012", false, "ALL");
  TPaveText* label_sqrt = get_labelSqrt(0);

// Make vectors for each graph to be plot
 vector<XYZ> obs_exclusion_cms, exp_exclusion_cms, twosigD_exclusion_cms, onesigD_exclusion_cms, onesigU_exclusion_cms, twosigU_exclusion_cms, theo_xsec ;

   // Method 2 : read file old way!
   //FILE* fitlog = fopen(fileName.c_str(),"r+");
   FILE* fitlog = fopen("ctauVsMass_Asymp_Limit.txt","r+");
   int read ;
   float  Obs_limit, TwoSigD, OneSigD, mexp, OneSigUp, TwoSigUp, eff, sgXsec, ctau, lambda, Bratio, lumi;
   //float ep;
   for ( int i=0; i< 23 ; i++ ) {

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
      
            // read from fill and fill vectors
        const XYZ pobs = {lambda, ctau, Obs_limit*sgXsec/eff };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(obs_exclusion_cms.begin(), obs_exclusion_cms.end(), pobs) == obs_exclusion_cms.end())
            obs_exclusion_cms.push_back(pobs);

        const XYZ pexp = {lambda, ctau, mexp*sgXsec/eff };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(exp_exclusion_cms.begin(), exp_exclusion_cms.end(), pexp) == exp_exclusion_cms.end())
            exp_exclusion_cms.push_back(pexp);

        const XYZ p2sD = {lambda, ctau, TwoSigD*sgXsec/eff };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(twosigD_exclusion_cms.begin(), twosigD_exclusion_cms.end(), p2sD) == twosigD_exclusion_cms.end())
            twosigD_exclusion_cms.push_back(p2sD);
        
	const XYZ p1sD = {lambda, ctau, OneSigD*sgXsec/eff };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(onesigD_exclusion_cms.begin(), onesigD_exclusion_cms.end(), p1sD) == onesigD_exclusion_cms.end())
            onesigD_exclusion_cms.push_back(p1sD);

        const XYZ p1sU = {lambda, ctau, OneSigUp*sgXsec/eff };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(onesigU_exclusion_cms.begin(), onesigU_exclusion_cms.end(), p1sU) == onesigU_exclusion_cms.end())
            onesigU_exclusion_cms.push_back(p1sU);
       
        const XYZ p2sU = {lambda, ctau, TwoSigUp*sgXsec/eff };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(twosigU_exclusion_cms.begin(), twosigU_exclusion_cms.end(), p2sU) == twosigU_exclusion_cms.end())
           twosigU_exclusion_cms.push_back(p2sU);
       
        const XYZ pXs = {lambda, ctau, sgXsec };  // {sig7[i].MW, sig7[i].MNu, sig7[i].CS};
        if (find(theo_xsec.begin(), theo_xsec.end(), pXs) == theo_xsec.end())
            theo_xsec.push_back(pXs);
       
       if ( read != 1 ) cout<<" reading error "<<endl ;

    } 

   
   /// Now set points graphs! 
  //TGraph2D* obs_cms_gr = new TGraph2D(obs_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  TGraph* obs_cms_gr = new TGraph(obs_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < obs_exclusion_cms.size(); ii++) { 
       //obs_cms_gr->SetPoint(ii, obs_exclusion_cms[ii].x, obs_exclusion_cms[ii].y, obs_exclusion_cms[ii].z );
       obs_cms_gr->SetPoint(ii, obs_exclusion_cms[ii].x, obs_exclusion_cms[ii].y); //, obs_exclusion_cms[ii].z );
       }
  obs_cms_gr->SetFillColor(kCyan);
  obs_cms_gr->SetLineColor(kCyan);
  obs_cms_gr->SetLineWidth(4);
  obs_cms_gr->SetNameTitle("limit", "limit; #Lambda; C#tau; #sigma");

  //TGraph2D* exp_cms_gr = new TGraph2D(exp_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  TGraph* exp_cms_gr = new TGraph(exp_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < exp_exclusion_cms.size(); ii++) { 
       //exp_cms_gr->SetPoint(ii, exp_exclusion_cms[ii].x, exp_exclusion_cms[ii].y, exp_exclusion_cms[ii].z );
       exp_cms_gr->SetPoint(ii, exp_exclusion_cms[ii].x, exp_exclusion_cms[ii].y); //, exp_exclusion_cms[ii].z );
       }
  exp_cms_gr->SetFillColor(kRed-4);
  exp_cms_gr->SetLineColor(kRed-4);
  exp_cms_gr->SetLineWidth(5);
  exp_cms_gr->SetLineStyle(2);
  exp_cms_gr->SetNameTitle("Exp_limit", "Exp_limit; #Lambda; C#tau; #sigma");

  TGraph2D* sig2D_cms_gr = new TGraph2D(twosigD_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < twosigD_exclusion_cms.size(); ii++) { 
       sig2D_cms_gr->SetPoint(ii, twosigD_exclusion_cms[ii].x, twosigD_exclusion_cms[ii].y, twosigD_exclusion_cms[ii].z );
       }
  sig2D_cms_gr->SetFillColor(kYellow);
  sig2D_cms_gr->SetLineColor(kYellow);
  sig2D_cms_gr->SetLineWidth(3);
  sig2D_cms_gr->SetNameTitle(" 2#sigma_down_limit", "2#sigma_down_limit; #Lambda; C#tau; #sigma");

  TGraph2D* sig1D_cms_gr = new TGraph2D(onesigD_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < onesigD_exclusion_cms.size(); ii++) { 
       sig1D_cms_gr->SetPoint(ii, onesigD_exclusion_cms[ii].x, onesigD_exclusion_cms[ii].y, onesigD_exclusion_cms[ii].z );
       }
  sig1D_cms_gr->SetFillColor(kGreen);
  sig1D_cms_gr->SetLineColor(kGreen);
  sig1D_cms_gr->SetLineWidth(3);
  sig1D_cms_gr->SetNameTitle("1#sigma_down_limit", "1#sigma_down_limit; #Lambda; C#tau; #sigma");

  TGraph2D* sig1U_cms_gr = new TGraph2D(onesigU_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < onesigU_exclusion_cms.size(); ii++) { 
       sig1U_cms_gr->SetPoint(ii, onesigU_exclusion_cms[ii].x, onesigU_exclusion_cms[ii].y, onesigU_exclusion_cms[ii].z );
       }
  sig1U_cms_gr->SetFillColor(kGreen);
  sig1U_cms_gr->SetLineColor(kGreen);
  sig1U_cms_gr->SetLineWidth(3);
  sig1U_cms_gr->SetNameTitle("1#sigma_up_limit", "1#sigma_up_limit; #Lambda; C#tau; #sigma");

  TGraph2D* sig2U_cms_gr = new TGraph2D(twosigU_exclusion_cms.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < twosigD_exclusion_cms.size(); ii++) { 
       sig2U_cms_gr->SetPoint(ii, twosigU_exclusion_cms[ii].x, twosigU_exclusion_cms[ii].y, twosigU_exclusion_cms[ii].z );
       }
  sig2U_cms_gr->SetFillColor(kYellow);
  sig2U_cms_gr->SetLineColor(kYellow);
  sig2U_cms_gr->SetLineWidth(3);
  sig2U_cms_gr->SetNameTitle(" 2#sigma_up_limit", "2#sigma_up_limit; #Lambda; C#tau; #sigma");


  //TGraph2D* Xsec_gr = new TGraph2D(theo_xsec.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  TGraph* Xsec_gr = new TGraph(theo_xsec.size()); // = new TGraph2(16, obs_lim_cms,ctau);
  for (size_t ii= 0;  ii < theo_xsec.size(); ii++) { 
       //Xsec_gr->SetPoint(ii, theo_xsec[ii].x, theo_xsec[ii].y, theo_xsec[ii].z );
       Xsec_gr->SetPoint(ii, theo_xsec[ii].x, theo_xsec[ii].y); // theo_xsec[ii].z );
       }
  Xsec_gr->SetFillColor(kBlue-8);
  Xsec_gr->SetLineColor(kBlue-8);
  Xsec_gr->SetLineWidth(2);
  Xsec_gr->SetNameTitle("Xsec_Theory", "Xsec_Theory; #Lambda; C#tau; #sigma");

  //**************************observed cms*******************************//
  Double_t obs_lim_cms[16]={100., 145., 157., 179., 192., 216., 221., 218., 218., 221., 216., 192., 179., 157., 145., 100.};
  Double_t CTAU[16]={0., 0., 0., 0., 0., 0., 0., 0., 1., 250., 500., 1000., 2000., 4000., 6000., 6000.};

  TGraph* obs_exclusion_cms_7;
  obs_exclusion_cms_7 = new TGraph(16, obs_lim_cms,CTAU);
  obs_exclusion_cms_7->SetFillColor(kYellow);
  obs_exclusion_cms_7->SetLineColor(kYellow);
  obs_exclusion_cms_7->SetLineWidth(4);

  //**************************expecteded cms*******************************//
  Double_t exp_lim_cms[16]={100., 148., 160., 182., 199., 218., 221., 221., 221., 221., 218., 199., 182., 160., 148., 100.};

  //exp_exclusion_cms_7 = new TGraph(16, x-axis, y-axis);
  TGraph* exp_exclusion_cms_7 = new TGraph(16, exp_lim_cms,CTAU );
//  exp_exclusion_cms_7->SetFillStyle(3022);
//  exp_exclusion_cms_7->SetFillColor(kOrange+7);
//  exp_exclusion_cms_7->SetLineColor(kOrange+7);
  exp_exclusion_cms_7->SetLineWidth(3);
  exp_exclusion_cms_7->SetLineStyle(2);


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
  

  TCanvas* c0 = new TCanvas("exclusion limit", "exclusion limit", 1);
  c0->cd();
  // c0->SetLogy();
  //c0->SetGridx();
  //c0->SetGridy();
    TPad * pad2 = new TPad("pad2", "pad2",0.01,0.0001,0.99,0.99);
    pad2->Draw("AH");
    pad2->SetBorderSize(0.);
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
    
    //TH2F *h2 = new TH2F("h","Axes",100,100,250,100,0.7,1E8);
    // h2->GetXaxis()->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
    //h2->GetXaxis()->SetTitle("M_{#tilde{#chi^{0}_{1}}} [GeV/c^{2}]");
  /*
   TMultiGraph *gAllp = new TMultiGraph();
   gAllp->SetTitle( " 95% CLs Limit " );
   //gAllp->Draw("LPsame") ;
   gAllp->Add( obs_cms_gr ) ;
   gAllp->Add( exp_cms_gr ) ;
   gAllp->Add( Xsec_gr );
   gAllp->Add( obs_exclusion_cdf) ;
   gAllp->Add(obs_exclusion_d0) ;
   gAllp->Draw("");
    obs_cms_gr->Draw("LF");
    obs_exclusion_cdf->Draw("LFsame");
    obs_exclusion_d0->Draw("LFsame");
    exp_cms_gr->Draw("Lsame");
 // obs_exclusion_conv->Draw("LFsame");
    Xsec_gr->Draw("LCsame");
    gAllp->Draw("sameaxis") ;
    gAllp->GetXaxis()->SetTitle("#Lambda [TeV]");
    gAllp->GetYaxis()->SetTitle("c#tau_{#tilde{#chi^{0}_{1}}} [mm]");
    gAllp->GetXaxis()->SetTitleSize(0.048);
    gAllp->GetXaxis()->SetTitleOffset(1.24);
    gAllp->GetYaxis()->SetTitleSize(0.048);
    gAllp->GetYaxis()->SetTitleOffset(1.6);
   */
    
     
    TH2F *h2 = new TH2F("h","Axes",100,100,250,100,0.7,1E8);
    h2->GetXaxis()->SetTitle("#Lambda [TeV]");
    h2->GetYaxis()->SetTitle("c#tau_{#tilde{#chi^{0}_{1}}} [mm]");
    h2->GetXaxis()->SetTitleSize(0.048);
    h2->GetXaxis()->SetTitleOffset(1.24);
    h2->GetYaxis()->SetTitleSize(0.048);
    h2->GetYaxis()->SetTitleOffset(1.6);
    h2->SetTitle("");
    gStyle->SetOptStat("");
    h2->Draw("");

    obs_cms_gr->Draw("LF");
    obs_exclusion_cms_7->Draw("LF");
    obs_exclusion_cdf->Draw("LFsame");
    obs_exclusion_conv->Draw("LFsame");
    obs_exclusion_d0->Draw("LFsame");
    exp_cms_gr->Draw("Lsame");
    exp_exclusion_cms_7->Draw("Lsame");
    Xsec_gr->Draw("LCsame");
    h2->Draw("sameaxis");
   


    TPaveText *pt = new TPaveText(0.45,0.3,0.65,0.55,"NDC");
   pt = new TPaveText(0.4551193,0.2499166,0.6555865,0.3643191,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(5);
   pt->SetTextAlign(12);
   pt->SetTextFont(42);
   pt->SetTextColor(kGray+2);
    pt->AddText("#font[42]{SPS8}"); 
    pt->AddText("#font[42]{GMSB #tilde{#chi}^{0}_{1}#rightarrow #gamma #tilde{G}}");
   pt->AddText("#font[42]{M_{m} = 2#Lambda, tan(#beta)=15}");
   pt->AddText("#font[42]{N_{m} = 1, #mu > 0}");

    TLegend* leg = new TLegend(0.25,0.50,0.8,0.90);
    leg->SetFillStyle(0); leg->SetBorderSize(0); 
    leg->SetFillColor(0);
    
    label_cms->Draw("same");
    label_sqrt->Draw("same");
    pt->Draw("same");
 
    leg ->SetTextFont(42);
    leg->AddEntry(exp_cms_gr, "CMS Expected with #gamma + Jets 19.1 fb^{-1}(8 TeV)", "l");
    leg->AddEntry(obs_cms_gr, "CMS Observed with #gamma + Jets 19.1 fb^{-1}(8 TeV)", "f");
    leg->AddEntry(exp_exclusion_cms_7, "CMS Expected with #gamma + Jets 4.86 fb^{-1}(7 TeV)", "l");
    leg->AddEntry(obs_exclusion_cms_7, "CMS Observed with #gamma + Jets 4.86 fb^{-1}(7 TeV)", "f");
    leg->AddEntry(obs_exclusion_conv,"CMS with #gamma (conversions) + #slash{E}_{T} + Jets (2.1 fb^{-1})","f");
    leg->AddEntry(obs_exclusion_cdf,"CDF with #gamma #gamma + #slash{E}_{T} + Jets (2.6 fb^{-1})","f"); 
    leg->AddEntry(obs_exclusion_d0,"D#slash{O} with prompt #gamma #gamma + #slash{E}_{T} (6.3 fb^{-1})","f");
  
    leg->Draw("same");
   c0->SaveAs("Neutralino_Ctau-Vs-Lambda_2D_exclusion.png");
   c0->SaveAs("Neutralino_Ctau-Vs-Lambda_2D_exclusion.pdf");

}
