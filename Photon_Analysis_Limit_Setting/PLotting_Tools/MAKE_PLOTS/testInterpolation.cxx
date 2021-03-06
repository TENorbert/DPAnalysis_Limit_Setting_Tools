//Example of the various methods of interpolation provided by the 
// ROOT::Math::Interpolator class
// 
//Example can also be run in ROOT by doing : 
// 
// root> .x testInterpolation.cxx
//
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "Math/Interpolator.h"
#include <iostream>
#include <string>
#include "TROOT.h"
#include <cmath>

TGraph *grorig = 0;

void interpolate( const  ROOT::Math::Interpolator & itp ) { 

   std::string type = itp.Type(); 
   std::cout << "\n" << type << "  interpolation:" << std::endl;

   std::cout << "x[i]     y[i]     deriv[i]     deriv2[i]    integral[i] \n" << std::endl; 
   // print result of interpolation
   const Int_t n = 51;
   Int_t i = 0;
   Float_t xcoord[n], ycoord[n];
   for ( double xi = 0; xi < 10; xi += 0.2) { 
      xcoord[i] = xi;
      ycoord[i] = itp.Eval(xi);
      double dyi = itp.Deriv(xi); 
      double dy2i = itp.Deriv2(xi);
      double igyi = itp.Integ(0, xi); 
      std::cout << xcoord[i]  << "  " << ycoord[i] << "  " << dyi << "  " << dy2i << "  " <<  igyi << std::endl;
      i++; 
   }

   TGraph *gr = new TGraph(n,xcoord,ycoord);
   gr->SetMarkerColor(kBlue);
   gr->SetMarkerStyle(7);
   gr->Draw("CP");
   TLegend * l = new TLegend(0.1,0.7,0.4,0.9); 
   l->AddEntry(grorig,"original data"); 
   l->AddEntry(gr,type.c_str());
   l->Draw();

   return;
}


void testInterpolation() {

   // Create data
   int n = 10; 
   Float_t xorig[10], yorig[10];
   std::vector<double> x(n); 
   std::vector<double> y(n); 
   for (int i = 0; i < n; ++i) {  
      x[i] = i + 0.5 * std::sin(i+0.0); 
      y[i] = i + std::cos(i * i+0.0); 
      xorig[i] = x[i];
      yorig[i] = y[i];
   } 

   TCanvas *c1 = new TCanvas("c1","Original (red), Linear (upper left), Polynomial (upper right), Spline , Spline periodic, Akima (lower left) and Akima Periodic (lower right) Interpolation",10,10,1000,800);
   c1->Divide(2,3);
   c1->cd(1);

   grorig = new TGraph(n,xorig,yorig);
   grorig->SetMarkerColor(kRed);
   grorig->SetMarkerStyle(20);
   grorig->GetYaxis()->SetRange(0,40);
   grorig->Draw("AP"); 

   //ROOT::Math::Interpolator itp1(x, y, ROOT::Math::Interpolation::LINEAR); 
   ROOT::Math::Interpolator itp1(x.size(), ROOT::Math::Interpolation::LINEAR); 
   itp1.SetData(x,y);
   interpolate(itp1);


   c1->cd(2);
   grorig->Draw("AP"); 

   ROOT::Math::Interpolator itp2(x, y, ROOT::Math::Interpolation::POLYNOMIAL); 
   interpolate(itp2);


   c1->cd(3);
   grorig->Draw("AP"); 

   //std::cout << "Cubic Spline Interpolation: " << std::endl;
   ROOT::Math::Interpolator itp3( 2*x.size(), ROOT::Math::Interpolation::CSPLINE); 
   itp3.SetData(x.size(), &x[0], &y[0]);
   interpolate(itp3);

   c1->cd(4);
   grorig->Draw("AP"); 

   //std::cout << "Akima  Interpolation: " << std::endl;
   ROOT::Math::Interpolator itp4(x, y, ROOT::Math::Interpolation::AKIMA); 
   interpolate(itp4);

   c1->cd(5);
   grorig->Draw("AP"); 

   //std::cout << "Cubic Spline Periodic Interpolation: " << std::endl;
   ROOT::Math::Interpolator itp5(x, y, ROOT::Math::Interpolation::CSPLINE_PERIODIC); 
   interpolate(itp5);



   c1->cd(6);
   grorig->Draw("AP"); 

   //std::cout << "Akima Periodic Interpolation: " << std::endl;
   ROOT::Math::Interpolator itp6(x, y, ROOT::Math::Interpolation::AKIMA_PERIODIC); 
   interpolate(itp6);

   std::cout << "\n***********************************" << std::endl;
   std::cout << "Using default Interpolation type: " << std::endl;

   ROOT::Math::Interpolator itp7; 
   itp7.SetData(x,y);
   interpolate(itp7);


}

#ifndef __CINT__
int main(int argc, char **argv)
{
   if (argc > 1) { 
      TApplication theApp("App",&argc,argv);
      testInterpolation();
      theApp.Run();
   } 
   else 
      testInterpolation();

   return 0;
}
#endif
