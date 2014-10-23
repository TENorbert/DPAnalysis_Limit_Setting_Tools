//root -b -q -l upperLimit.C+
#include "TMath.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TText.h"
#include "TLatex.h"
#include "TAxis.h"

#include "tdrstyle.C"
//#include "../acceptance/acceptance_db.C"

#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <bits/stl_vector.h>
#include "stdio.h"

struct XYZ
{
    int x, y;
    float z;

    bool operator ==(const XYZ & v) const
    {
        return(x == v.x) && (y == v.y);
    }
} ;

static const double lumi2010 = 0;
static const double lumi2011 = 4971.0;
static const double lumi2012 = 19100.0;

//AcceptanceDB* gldb, *db, *db2011;

int fprintf_sigFig(FILE * f, double t, int n);

class compareByNu
{
public:

    bool operator() (const XYZ& a, const XYZ& b)
    {
        return a.y > b.y;
    }
} ;

class compareByNu2
{
public:

    bool operator() (const XYZ& a, const XYZ& b)
    {
        return a.y < b.y;
    }
} ;

std::vector<XYZ> getSortedVector(int iwr, std::vector<XYZ> a)
{

    // First create a vector that contains only the points we want
    std::vector<XYZ> vec;
    for(unsigned int i = 0; i < a.size(); i++)
        if(a.at(i).x == iwr) vec.push_back(a.at(i));
    std::sort(vec.begin(), vec.end(), compareByNu());
    return vec;
}

std::vector<XYZ> getSortedVector(int iwr, std::vector<XYZ> a, std::vector<XYZ> b)
{

    std::vector<XYZ> vec;
    for(unsigned int i = 0; i < a.size(); i++)
        if(a.at(i).x == iwr) vec.push_back(a.at(i));
    for(unsigned int i = 0; i < b.size(); i++)
        if(b.at(i).x == iwr) vec.push_back(b.at(i));
    return getSortedVector(iwr, vec);
}


// fxn plots limits
void plotLimits( int minctauval = -1, int maxctauval = -1, float xmini = 100.0, float xmaxi = 500.0, float ymaxi = 2400.0, std::string limitFile = "", std::string csFile = "cs.txt", std::string csFile2 = "xsecs.txt")
{
    if(minctauval < 0) minctauval = 100;
    if(maxctauval < 0) maxctauval = 12500;

    std::string file("Limit_Input_File.txt");
    if(limitFile.length() == 0)
    {
      cout << "Sorry, Errrrooor! limit file is empty, give another! plz!" << endl;
    }else {
            limitFile = file;
    }

    const int M_STEP = 50, CTAU_STEP = 100;
    const float xmin = xmini, xmax = xmaxi, ymax = ymaxi;

    // Inizialization
    //FILE *Ptr;
    char *str1 = new char[128];
    FILE *inLimit = fopen(limitFile.c_str(), "r");
    FILE *inCS = fopen(csFile.c_str(), "r");
    FILE *inCS2 = fopen(csFile2.c_str(), "r");

    //std::string outputFileName = "obsexpSummary.txt";
    //Ptr = fopen(outputFileName.c_str(), "a");

    std::vector<XYZ> obsLimit;
    std::vector<XYZ> expLimit;
    std::vector<XYZ> exp68loLimit;
    std::vector<XYZ> exp68hiLimit;
    std::vector<XYZ> exp95loLimit;
    std::vector<XYZ> exp95hiLimit;

    std::map<std::pair<int, int>, float > ps;
    std::map<std::pair<int, int>, float > csforsf;

    // Read This for 8 TeV data
    char buff[4096];
    char *c;
    while(!feof(inCS) && (c = fgets(buff, 4096, inCS)) != NULL)
    {
        int energy, tmp_mass, tmp_ctau;
        float tmp_cs, tmp_sf;
        for(char* k = strchr(buff, ','); k != 0; k = strchr(buff, ',')) *k = ' ';
        if(sscanf(buff, "%d %d %d %f %f\n", &energy, &tmp_mass, &tmp_ctau, &tmp_cs, &tmp_sf) == 5 && energy == 8)
        {
            csforsf[std::make_pair(tmp_mass, tmp_ctau)] = tmp_cs * tmp_sf;
        }
    }
    fclose(inCS);

    while(!feof(inCS2) && (c = fgets(buff, 4096, inCS2)) != NULL)
    {
        int energy, tmp_mass, tmp_ctau;
        float tmp_cs;
        for(char* k = strchr(buff, ','); k != 0; k = strchr(buff, ',')) *k = ' ';
        if(sscanf(buff, "%d %d %d %f\n", &energy, &tmp_mass, &tmp_ctau, &tmp_cs) == 4 && energy == 8)
        {
            if(!(tmp_mass % M_STEP))
            {
                ps[std::make_pair(tmp_mass, tmp_ctau)] = tmp_cs * 1.0; // 1000000000.0;  //think about the units!
                //std::cout << tmp_mWR << "\t" << tmp_mNu << "\t" << ps[std::make_pair(tmp_mWR, tmp_mNu)] << std::endl;
            }
        }
    }
    fclose(inCS2);
    
    // Build up cross-section as a function of W_R and N mass
    TGraph2D* grCS = new TGraph2D(ps.size());
    grCS->SetNameTitle("xs", "xs;Neutralino Mass; c#tau; #sigma");
    int ibin = 0;
    for(std::map<std::pair<int, int>, float >::const_iterator it = ps.begin(); it != ps.end(); ++it)
    {
        //std::pair<int, int> mid = std::make_pair(it->first.first, it->first.first / 2);
        //double midNLOcs = csforsf[mid], midLOcs = ps[mid];
        grCS->SetPoint(ibin++, it->first.first, it->first.second, it->second ); //* midNLOcs / midLOcs);
        //grCS->SetPoint(ibin++, it->first.first, it->first.second, (it->second * nFlavorScale / midLOcs));
    }

    double jscale = 1.0;

    while(!feof(inLimit) && (c = fgets(buff, 4096, inLimit)) != NULL)
    {
        float tmp_Mass, tmp_Ctau, obsl, expl, sig2d, sig1d , sig2u, sig1u;
        for(char* k = strchr(buff, ','); k != 0; k = strchr(buff, ',')) *k = ' ';
        if(sscanf(buff, "%f %f %f %f %f %f %f %f\n", &tmp_Mass, &tmp_Ctau, &obsl, &sig2d, &sig1d, &expl, &sig1u, &sig2u) == 8)
        {
            //printf("%f %f %f %f %f %f %f %f\n", tmp_mwr, tmp_mnu, f3, f4, f5, f6, f7, f8);
            double accRatio = 1.0;// / accRatioGetter.getAccRatio(tmp_mwr, tmp_mnu, mode);
            const XYZ p = {tmp_Mass, tmp_Ctau, obsl * accRatio / jscale};
            const XYZ q = {tmp_Mass, tmp_Ctau, expl * accRatio / jscale};
            const XYZ h68 = {tmp_Mass, tmp_Ctau, sig1u * accRatio / jscale};
            const XYZ l68 = {tmp_Mass, tmp_Ctau, sig1d * accRatio / jscale};
            const XYZ h95 = {tmp_Mass, tmp_Ctau, sig2u * accRatio / jscale};
            const XYZ l95 = {tmp_Mass, tmp_Ctau, sig2d * accRatio / jscale};
            if(find(obsLimit.begin(), obsLimit.end(), p) == obsLimit.end()) obsLimit.push_back(p);
            if(find(expLimit.begin(), expLimit.end(), q) == expLimit.end()) expLimit.push_back(q);
            if(find(exp68hiLimit.begin(), exp68hiLimit.end(), h68) == exp68hiLimit.end()) exp68hiLimit.push_back(h68);
            if(find(exp68loLimit.begin(), exp68loLimit.end(), l68) == exp68loLimit.end()) exp68loLimit.push_back(l68);
            if(find(exp95hiLimit.begin(), exp95hiLimit.end(), h95) == exp95hiLimit.end()) exp95hiLimit.push_back(h95);
            if(find(exp95loLimit.begin(), exp95loLimit.end(), l95) == exp95loLimit.end()) exp95loLimit.push_back(l95);
        }
    }
    fclose(inLimit);
    
    
    //return;
    //
    // 1D plots
    //
    for(int ictau = minctauval; ictau <= maxctauval; ictau += CTAU_STEP)
    {

        TGraph* h_exp = new TGraph();
        TGraph* h_obs = new TGraph();
        TGraph* h_theoryL = new TGraph();
        TGraphAsymmErrors* h_theory = new TGraphAsymmErrors();
        TGraphAsymmErrors* h_exp1sig = new TGraphAsymmErrors();
        TGraphAsymmErrors* h_exp2sig = new TGraphAsymmErrors();
        
	/*
        double eff_68lo = getEfficiency(iwr, exp68loLimit, mode);
        double eff_68hi = getEfficiency(iwr, exp68hiLimit, mode);
        double eff_95lo = getEfficiency(iwr, exp95loLimit, mode);
        double eff_95hi = getEfficiency(iwr, exp95hiLimit, mode);
        */
        unsigned int c0 = 0, c1 = 0, c2 = 0;
        std::map<std::pair<int, int>, double >::const_iterator imp;
        for(int inu = 70; inu < iwr - 50; inu += MNU_STEP)
        {
            std::pair<int,  int> i(mass, inu);
            if((imp = rawExpPts.find(i)) != rawExpPts.end())
            {
                double acc = getWgtAcceptance(imass, inu, mode);
                if(acc < 0) continue;
                double d68lo = imp->second - eff_68lo / acc;
                double d68hi = eff_68hi / acc - imp->second;
                double d95lo = imp->second - eff_95lo / acc;
                double d95hi = eff_95hi / acc  - imp->second;

                h_exp->SetPoint(c1, (double)inu, imp->second);
                h_exp1sig->SetPoint(c1, (double)inu, imp->second);
                h_exp2sig->SetPoint(c1, (double)inu, imp->second);

                h_exp1sig->SetPointError(c1, 0., 0., d68lo, d68hi);
                h_exp2sig->SetPointError(c1++, 0., 0., d95lo, d95hi);
            }
            if((imp = rawObsPts.find(std::make_pair(imass, inu))) != rawObsPts.end())
                h_obs->SetPoint(c2++, (double)inu, imp->second);

            double sigmaBR = grCS->Interpolate(imass, inu);
            // if ( sigmaBR < 0.000001 ) continue ;
            if(sigmaBR > 1e-5 && sigmaBR < 1000.0 && inu > 10)
            {
                h_theoryL->SetPoint(c0, inu, sigmaBR);
                h_theory->SetPoint(c0, inu, sigmaBR);
                h_theory->SetPointError(c0++, 0., 0., sigmaBR * 0.1, sigmaBR * 0.1);
            }
        }

        // Style for lines
        h_exp->SetLineWidth(3);
        h_exp->SetLineStyle(2);
        h_obs->SetLineWidth(3);
        h_obs->SetLineColor(1);
        h_obs->SetLineStyle(1);
        h_exp1sig->SetFillColor(kBlue - 7);
        h_exp2sig->SetFillColor(kYellow);
        h_theoryL->SetLineColor(kRed);
        h_theoryL->SetLineWidth(3);
        h_theory->SetFillColor(kGreen);

        sprintf(str1, "csUL%i", imass);

        TCanvas* csUL = new TCanvas(str1, "c1", 750, 750);
        setTDRStyle();
        fixOverlay();
        csUL->SetLeftMargin(0.19);
        csUL->SetRightMargin(0.06);
        csUL->SetTopMargin(0.06);
        csUL->SetLogy(1);

        TMultiGraph* mg = new TMultiGraph();
        mg->SetName("");
        mg->SetTitle(";M_{#tilde{#chi}_{1}^{0}} [GeV];Arbitrary scale");

        mg->Add(h_exp2sig, "E3");
        mg->Add(h_exp1sig, "E3");
        //mg->Add(h_theory, "E3");
        mg->Add(h_theoryL, "L");
        mg->Add(h_exp, "L");
        mg->Add(h_obs, "L");
        // add reco curves here for comapirson of selected points

        char leglabel[128];
        sprintf(leglabel, "{M_{#tilde{#chi}_{1}^{0}} = %d GeV}", imass);
        TLegend *leg = new TLegend(0.50, 0.68, 0.94, 0.94, leglabel, "brNDC");
        leg->SetTextSize(0.030);
        leg->SetTextFont(42);
        leg->SetBorderSize(1);
        leg->SetFillColor(10);
        leg->SetLineWidth(1);
        leg->SetNColumns(1);
        leg->AddEntry(h_obs, "95% C.L. Observed Limit", "L");
        leg->AddEntry(h_exp, "95% C.L. Expected Limit", "L");
        leg->AddEntry(h_exp1sig, "#pm1#sigma Expected Limit", "F");
        leg->AddEntry(h_exp2sig, "#pm2#sigma Expected Limit", "F");
        leg->AddEntry(h_theoryL, "NLO Signal cross section", "L");
        //leg->AddEntry(h_theory, "#pm1#sigma PDF+scale unc.", "F");


        TH1D* dummy = new TH1D("dummy", "dummy", 100, 0, double(imass));
        dummy->GetXaxis()->SetTitle("M_{#tilde{#chi}_{1}^{0}} [GeV]");
        dummy->GetYaxis()->SetTitle("#sigma(pp #rightarrow #tilde{g}#tilde{g}) #times Br(#tilde{#chi}_{1}^{0} #rightarrow #gamma + #tilde{G}) [pb]");
        dummy->GetYaxis()->SetTitleOffset(1.45);
        // dummy->SetMinimum(minyval) ; 
        // dummy->SetMaximum(maxyval) ; 

        mg->Draw("AH");
        mg->GetXaxis()->SetRangeUser(0., double(imass));
        // mg->GetYaxis()->SetRangeUser(0,ymax) ; 
        mg->GetXaxis()->SetNdivisions(7, 5, 0, true);
        mg->GetXaxis()->SetLabelSize(0.05);
        dummy->Draw();
        mg->Draw("A");
        dummy->Delete();

        TLatex* mark = new TLatex(0.20, 0.95, "CMS (Preliminary)");// Preliminary");
        // TLatex* mark2 = new TLatex(0.6,0.65, "#int Ldt = 227 pb^{-1} at 7 TeV") ;
        TLatex* mark2 = new TLatex(0.70, 0.95, "19.1 fb^{-1} (8 TeV)");
        mark->SetNDC();
        mark->SetTextAlign(31);
        mark->SetTextSize(0.04 * 1.1);
        mark->SetTextFont(42);
        mark->DrawLatex(1 - csUL->GetRightMargin(), 0.9575, "19.1 fb^{-1} (8 TeV)");
        mark->SetTextAlign(13);
        mark->SetTextSize(0.04 * 1.1 * 1.25);
        mark->SetTextFont(61);
        //mark->DrawLatex(twoDlimits->GetLeftMargin() + 0.025, 1 - (twoDlimits->GetTopMargin() + 0.025), "CMS #scale[0.8]{#it{Preliminary}}");
        mark->DrawLatex(csUL->GetLeftMargin() + 0.055, 1 - (csUL->GetTopMargin() + 0.025), "CMS");
        mark->SetTextSize(0.04 * 1.1);
        mark->SetTextFont(51);
        mark->DrawLatex(csUL->GetLeftMargin() + 0.055, 1 - (csUL->GetTopMargin() + 0.075), "preliminary");
        //mark->SetNDC(kTRUE);
        //mark->SetTextSize(0.04 * 1.1 * 0.85);
        //mark->SetTextFont(42);
        //mark->SetTextAlign(31);
        //mark->DrawLatex(0.94, 0.9575, "19.7 fb^{-1} (8 TeV)");
        //mark->SetTextAlign(13);
        //mark->SetTextSize(0.04 * 1.1 * 1.25*0.85);
        //mark->DrawLatex(0.18 + 0.025, 1 - (0.06 + 0.025), "CMS #scale[0.8]{#it{Preliminary}}");
        //switch(mode)
        //{
        //    case 0:
        //    case 1:
        //    case 2:
        //    case 3:
        //        mark2 = new TLatex(0.70, 0.95, "19.7 fb^{-1} at 8 TeV");
        //        break;
        //}

        //mark->SetNDC(kTRUE);
        //mark->SetTextSize(0.035);
        //mark->SetTextFont(42);
        //mark2->SetNDC(kTRUE);
        //mark2->SetTextSize(0.035);
        //mark2->SetTextFont(42);

        //TLatex* mark  = new TLatex(0.0*mg->GetXaxis()->GetXmax(), 1.03*mg->GetYaxis()->GetXmax(), "CMS Preliminary") ;
        //mark->SetTextSize(0.04);

        //TLatex* mark2 = new TLatex(0.62*mg->GetXaxis()->GetXmax(),1.03*mg->GetYaxis()->GetXmax(), "227 pb^{-1} at 7 TeV") ;
        //mark2->SetTextSize(0.04);

        leg->Draw();

        //mark->Draw();
        //mark2->Draw();

        char cn[128], png[128], eps[128];
                sprintf(cn, "Mu_Limit_1D_mWR_%d.pdf", iwr);
                sprintf(png, "Mu_Limit_1D_mWR_%d.png", iwr);
                sprintf(eps, "Mu_Limit_1D_mWR_%d.eps", iwr);
                
		sprintf(cn, "Elec_Limit_1D_mWR_%d.pdf", iwr);
                sprintf(png, "Elec_Limit_1D_mWR_%d.png", iwr);
                sprintf(eps, "Elec_Limit_1D_mWR_%d.eps", iwr);
                
		sprintf(cn, "El-Mu_Limit_1D_mWR_%d.pdf", iwr);
                sprintf(png, "El-Mu_Limit_1D_mWR_%d.png", iwr);
                sprintf(eps, "El-Mu_Limit_1D_mWR_%d.eps", iwr);
                
		sprintf(cn, "Mu_2011-2012_Limit_1D_mWR_%d.pdf", iwr);
                sprintf(png, "Mu_2011-2012_Limit_1D_mWR_%d.png", iwr);
                sprintf(eps, "Mu_2011-2012_Limit_1D_mWR_%d.eps", iwr);
        
	csUL->Print(cn);
        csUL->Print(png);
        csUL->Print(eps);
    }



    //
    // Now, create a set of points and make the 2D exclusion plot
    //

    std::vector<XYZ> expLowPts;
    std::vector<XYZ> expHighPts;
    std::vector<XYZ> obsLowPts;
    std::vector<XYZ> obsHighPts;
    std::vector<XYZ> obs_right, exp_right;
    for(int iwr = minval; iwr <= maxval; iwr += MWR_STEP)
    {
        int nuExpLo = 0;
        int nuExpHi = iwr;
        int nuObsLo = 0;
        int nuObsHi = iwr;
        int expExclPts = 0;
        int obsExclPts = 0;
        for(int inu = 10; inu < iwr; inu += MNU_STEP)
        {
            std::pair<int, int> masspoint(iwr, inu);
            double sigmaBR = grCS->Interpolate(iwr, inu);
            if(sigmaBR <= 0) continue;

            double expDiff = sigmaBR - rawExpPts[masspoint];
            if(expDiff < 0)
            {
                if(inu <= (iwr / 2) && (inu > nuExpLo)) nuExpLo = inu;
                if(inu > (iwr / 2) && (inu < nuExpHi)) nuExpHi = inu;
            }
            else
            {
                expExclPts++;
            }
            if(iwr > TRANSITION_MWR && expDiff > 0 && inu < iwr - 50)
            {
                std::pair<int, int> nmasspoint(iwr + MWR_STEP, inu);
                double nextSigmaBR = grCS->Interpolate(iwr + MWR_STEP, inu);
                double nextExpDiff = nextSigmaBR - rawExpPts[nmasspoint];
                if(nextExpDiff < 0)
                {
                    double x1 = iwr;
                    double y1 = expDiff;
                    double x2 = iwr + MWR_STEP;
                    double y2 = nextExpDiff;
                    double interpol = -y1 * (x2 - x1) / (y2 - y1) + x1; // notice possible divide-by-zero!
                    XYZ p = {interpol, inu, 0};
                    if(interpol > x1 && interpol < x2)
                        exp_right.push_back(p);
                }
            }

            double obsDiff = sigmaBR - rawObsPts[masspoint];
            if(obsDiff < 0)
            {
                if(inu <= (iwr / 2) && (inu > nuObsLo)) nuObsLo = inu;
                if(inu > (iwr / 2) && (inu < nuObsHi)) nuObsHi = inu;
            }
            else
            {
                obsExclPts++;
            }
            if(iwr > TRANSITION_MWR && obsDiff > 0 && inu < iwr - 50)
            {
                std::pair<int, int> nmasspoint(iwr + MWR_STEP, inu);
                double nextSigmaBR = grCS->Interpolate(iwr + MWR_STEP, inu);
                double nextObsDiff = nextSigmaBR - rawObsPts[nmasspoint];
                if(nextObsDiff < 0)
                {
                    double x1 = iwr;
                    double y1 = obsDiff;
                    double x2 = iwr + MWR_STEP;
                    double y2 = nextObsDiff;
                    double interpol = -y1 * (x2 - x1) / (y2 - y1) + x1; // notice possible divide-by-zero!
                    XYZ p = {interpol, inu, 0};
                    if(interpol > x1 && interpol < x2)
                        obs_right.push_back(p);
                }
            }
        }
        if(iwr <= TRANSITION_MWR)
        {
            if(nuObsLo > 0 && obsExclPts > 0)
            {
                XYZ p = {iwr, nuObsLo, 0.};
                obsLowPts.push_back(p);
            }
            if(nuObsHi < iwr && obsExclPts > 0)
            {
                XYZ p = {iwr, nuObsHi, 0.};
                obsHighPts.push_back(p);
            }
            if(nuExpLo > 0 && expExclPts > 0)
            {
                XYZ p = {iwr, nuExpLo, 0.};
                expLowPts.push_back(p);
            }
            if(nuExpHi < iwr && expExclPts > 0)
            {
                XYZ p = {iwr, nuExpHi, 0.};
                expHighPts.push_back(p);
            }
        }
    }

    std::sort(obs_right.begin(), obs_right.end(), compareByNu2());
    std::sort(exp_right.begin(), exp_right.end(), compareByNu2());

    TGraph* observedPlot = new TGraph();
    TGraph* expectedPlot = new TGraph();

    unsigned int istart = 0;
    if(minval > 700)
    {
        observedPlot->SetPoint(istart, 700/1000.0, 50/1000.0);
        expectedPlot->SetPoint(istart, 700/1000.0, 50/1000.0);
        istart++;
    }
    if(minval > 800)
    {
        observedPlot->SetPoint(istart, 800/1000.0, 54/1000.0);
        expectedPlot->SetPoint(istart, 800/1000.0, 52/1000.0);
        istart++;
    }
    if(minval > 900)
    {
        observedPlot->SetPoint(istart, 900/1000.0, 56/1000.0);
        expectedPlot->SetPoint(istart, 900/1000.0, 59/1000.0);
        istart++;
    }

    /*    int ctr2d = istart;
    for(unsigned int i = 0; i < obsLowPts.size(); i++)
    {
        if(obsLowPts.at(i).x % 100 == 0)
        {
            if(i == 0) observedPlot->SetPoint(ctr2d++, obsLowPts.at(i).x, (obsLowPts.at(i).y + 0.5 * obsLowPts.at(i + 1).y) / 1.5);
            if(i == obsLowPts.size() - 1) observedPlot->SetPoint(ctr2d++, obsLowPts.at(i).x, (obsLowPts.at(i).y + 0.5 * obsLowPts.at(i - 1).y) / 1.5);
            else observedPlot->SetPoint(ctr2d++, obsLowPts.at(i).x, (obsLowPts.at(i).y + 0.5 * obsLowPts.at(i + 1).y + 0.5 * obsLowPts.at(i - 1).y) / 2.0);
        }
    }
    for(unsigned int i = 0; i < obs_right.size(); i++) observedPlot->SetPoint(ctr2d++, obs_right.at(i).x, obs_right.at(i).y);
    for(int i = obsHighPts.size() - 1; i < obsHighPts.size(); i--)
    {
        if(obsHighPts.at(i).x % 100 == 0)
        {
            //std::cout << "HERE: " << i << "\t" << obsHighPts.at(i).x << std::endl;
            if(i == 0) observedPlot->SetPoint(ctr2d++, obsHighPts.at(i).x, (obsHighPts.at(i).y + 0.5 * obsHighPts.at(i + 1).y) / 1.5);
            if(i == obsHighPts.size() - 1) observedPlot->SetPoint(ctr2d++, obsHighPts.at(i).x, (obsHighPts.at(i).y + 0.5 * obsHighPts.at(i - 1).y) / 1.5);
            else observedPlot->SetPoint(ctr2d++, obsHighPts.at(i).x, (obsHighPts.at(i).y + 0.5 * obsHighPts.at(i + 1).y + 0.5 * obsHighPts.at(i - 1).y) / 2.0);
        }
    }
    ctr2d = istart;
    for(unsigned int i = 0; i < expLowPts.size(); i++) expectedPlot->SetPoint(ctr2d++, expLowPts.at(i).x, expLowPts.at(i).y);
    for(unsigned int i = 0; i < exp_right.size(); i++) expectedPlot->SetPoint(ctr2d++, exp_right.at(i).x, exp_right.at(i).y);
    for(unsigned int i = expHighPts.size(); i > 0; i--) expectedPlot->SetPoint(ctr2d++, expHighPts.at(i - 1).x, expHighPts.at(i - 1).y);*/

    int ctr2d = istart;
    for(unsigned int i = 0; i < obsLowPts.size(); i++) observedPlot->SetPoint(ctr2d++, obsLowPts.at(i).x/1000.0, obsLowPts.at(i).y/1000.0);
    for(unsigned int i = 0; i < obs_right.size(); i++) observedPlot->SetPoint(ctr2d++, obs_right.at(i).x/1000.0, obs_right.at(i).y/1000.0);
    for(unsigned int i = obsHighPts.size(); i > 0; i--) observedPlot->SetPoint(ctr2d++, obsHighPts.at(i - 1).x/1000.0, obsHighPts.at(i - 1).y/1000.0);
    ctr2d = istart;
    for(unsigned int i = 0; i < expLowPts.size(); i++) expectedPlot->SetPoint(ctr2d++, expLowPts.at(i).x/1000.0, expLowPts.at(i).y/1000.0);
    for(unsigned int i = 0; i < exp_right.size(); i++) expectedPlot->SetPoint(ctr2d++, exp_right.at(i).x/1000.0, exp_right.at(i).y/1000.0);
    for(unsigned int i = expHighPts.size(); i > 0; i--) expectedPlot->SetPoint(ctr2d++, expHighPts.at(i - 1).x/1000.0, expHighPts.at(i - 1).y/1000.0);

    if(minval > 900)
    {
        observedPlot->SetPoint(observedPlot->GetN(), 900/1000.0, 819/1000.0);
        expectedPlot->SetPoint(expectedPlot->GetN(), 900/1000.0, 810/1000.0);
    }
    if(minval > 800)
    {
        observedPlot->SetPoint(observedPlot->GetN(), 800/1000.0, 722/1000.0);
        expectedPlot->SetPoint(expectedPlot->GetN(), 800/1000.0, 726/1000.0);
    }
    if(minval > 700)
    {
        observedPlot->SetPoint(observedPlot->GetN(), 700/1000.0, 645/1000.0);
        expectedPlot->SetPoint(expectedPlot->GetN(), 700/1000.0, 641/1000.0);
    }

    double xx, yy;
    std::cout << "OBSERVED" << std::endl;
    for(int i = 0; i < observedPlot->GetN(); i++)
    {
        observedPlot->GetPoint(i, xx, yy);
        std::cout << xx << "\t" << yy << "\t" << xx / yy << std::endl;
    }
    std::cout << "EXPECTED" << std::endl;
    for(int i = 0; i < expectedPlot->GetN(); i++)
    {
        expectedPlot->GetPoint(i, xx, yy);
        std::cout << xx << "\t" << yy << "\t" << xx / yy << std::endl;
    }

    TCanvas *twoDlimits = new TCanvas("twoDlimits", "twoDlimits", 800, 800);
    twoDlimits->SetRightMargin(0.03);
    twoDlimits->SetLeftMargin(0.17);
    setTDRStyle();
    fixOverlay();

    TH2D* grid = new TH2D("grid", "grid", 100, xmin/1000.0, xmax/1000.0, 100, 0.001/1000.0, ymax/1000.0); //1000) ;
    grid->GetXaxis()->SetTitle("M_{W_{R}} [TeV]");
    grid->GetXaxis()->SetNdivisions(6, 5, 0);
    grid->GetYaxis()->SetTitleOffset(1.22);
    grid->GetXaxis()->SetNdivisions(6, 5, 0);

    switch(mode)
    {
        case 0:
            grid->GetYaxis()->SetTitle("M_{#scale[1.25]{N_{#scale[1.5]{#mu}}}} [TeV]");
            expectedPlot->SetLineColor(kBlue);
            break;
        case 1:
            grid->GetYaxis()->SetTitle("M_{#scale[1.25]{N_{#scale[1.5]{e}}}} [TeV]");
            expectedPlot->SetLineColor(kRed);
            observedPlot->SetLineColor(kRed);
            break;
        case 2:
            grid->GetYaxis()->SetTitle("M_{#scale[1.25]{N_{#scale[1.5]{e,#mu}}}} [TeV]");
            expectedPlot->SetLineColor(kMagenta + 2);
            break;
        case 3:
            grid->GetYaxis()->SetTitle("M_{N_{#mu}} [TeV]");
            expectedPlot->SetLineColor(kBlue);
            break;
    }
    grid->Draw();


    observedPlot->SetLineColor(kBlack);
    observedPlot->SetFillStyle(3005);
    observedPlot->SetLineWidth(5);
    observedPlot->Draw("F SAME");
    observedPlot->Draw("L SAME");

    expectedPlot->SetLineStyle(2);
    expectedPlot->SetLineWidth(5);
    expectedPlot->Draw("L SAME");

    TLegend *leg = new TLegend(.78, .83, .95, .93);
    leg->SetTextFont(42);
    leg->SetTextSize(0.032);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetLineWidth(1);
    leg->SetNColumns(1);
    leg->AddEntry(observedPlot, "Observed", "L");
    leg->AddEntry(expectedPlot, "Expected", "L");

    // ============== plot LEP limit region:
    float x[] = {1.0, 4.0,  4.0, 1.0};
    float y[] = {  0,   0, 0.09, 0.09};
    TGraph* LEP = new TGraph(4, x, y);
    LEP->SetFillColor(kGray + 1);
    LEP->SetLineColor(kGray);
    
    // ============== plot WR->tb limit region:
    float x3[] = {  0, 2.03, 2.14, 2.14,   0};
    float y3[] = {  0,    0, 2.14, 4.0 , 4.0};
    TGraph* wrtotb = new TGraph(5, x3, y3);
    wrtotb->SetFillColor(kTeal + 1);
    wrtotb->SetLineColor(kTeal);

    // ============== plot region M_nuR > M_WR:
    float x2[] = {xmin/1000.0, ymax/1000.0, xmin/1000.0};
    float y2[] = {ymax/1000.0, ymax/1000.0, xmin/1000.0};
    TGraph* wrnu = new TGraph(3, x2, y2);
    wrnu->SetLineWidth(3);
    wrnu->SetLineColor(kYellow - 4);
    wrnu->SetFillColor(kYellow);

    // ============= plot labels:
    TLatex text;
    text.SetTextSize(0.04);
    text.SetTextFont(42);

    TLatex text2;
    text2.SetTextSize(0.035);
    text2.SetTextAngle(90);
    text2.SetTextFont(42);

    TLatex* mark = new TLatex(1300/1000.0, 1.015 * ymax/1000.0, "CMS Preliminary");
    mark->SetNDC();
    mark->SetTextSize(0.04 * 1.1);
    mark->SetTextFont(42);


    //TLatex* mark2 = new TLatex(1300, 0.93 * ymax, "4.7 fb^{-1} at 7 TeV");
    //mark2->SetTextSize(0.04);
    //mark2->SetTextFont(42);


    //Tevatron->Draw("F SAME");
    //LEP->Draw("same F");
    wrnu->Draw("F SAME");
    //wrtotb->Draw("same F");
    mark->SetTextAlign(21);
    text.SetTextAngle(45);
    switch(mode)
    {
        case 0:
            text.DrawLatex((xmin + 350)/1000.0, .625 * ymax/1000.0, "M_{#scale[1.25]{N_{#scale[1.5]{#mu}}}} > M_{#scale[1.25]{W_{#scale[1.5]{R}}}}");
            //mark->DrawLatex(twoDlimits->GetLeftMargin()+0.5*(1 - twoDlimits->GetRightMargin() - twoDlimits->GetLeftMargin()), 0.9575, "CMS    #sqrt{s} = 8 TeV    L = 19.7 fb^{-1}");
            //mark->DrawLatex(0.71, 0.96, "12.1 fb^{-1} at 8 TeV");
            break;
        case 1:
            text.DrawLatex((xmin + 350)/1000.0, .625 * ymax/1000.0, "M_{#scale[1.25]{N_{#scale[1.5]{e}}}} > M_{#scale[1.25]{W_{#scale[1.5]{R}}}}");
            //mark->DrawLatex(twoDlimits->GetLeftMargin()+0.5*(1 - twoDlimits->GetRightMargin() - twoDlimits->GetLeftMargin()), 0.9575, "CMS    #sqrt{s} = 8 TeV    L = 19.7 fb^{-1}");
            //mark->DrawLatex(0.71, 0.96, "12.3 fb^{-1} at 8 TeV");
            break;
        case 2:
            text.DrawLatex((xmin + 350)/1000.0, .625 * ymax/1000.0, "M_{#scale[1.25]{N_{#scale[1.5]{e,#mu}}}} > M_{#scale[1.25]{W_{#scale[1.5]{R}}}}");
            //mark->DrawLatex(twoDlimits->GetLeftMargin()+0.5*(1 - twoDlimits->GetRightMargin() - twoDlimits->GetLeftMargin()), 0.9575, "CMS    #sqrt{s} = 8 TeV    L = 19.7 fb^{-1}");
            //mark->DrawLatex(0.71, 0.96, "3.6 fb^{-1} at 8 TeV");
            break;
        case 3:
            text.DrawLatex((xmin + 350)/1000.0, .625 * ymax/1000.0, "M_{N_{#mu}} > M_{W_{R}}");
            //mark->DrawLatex(0.51, 0.96, "5.0 fb^{-1} (7 TeV) + 3.6 fb^{-1} (8 TeV)");
            break;

    }
    mark->SetTextAlign(31);
    mark->DrawLatex(1 - twoDlimits->GetRightMargin(), 0.9575, "19.7 fb^{-1} (8 TeV)");
    mark->SetTextAlign(13);
    mark->SetTextSize(0.04 * 1.1 * 1.25);
    mark->SetTextFont(61);
    //mark->DrawLatex(twoDlimits->GetLeftMargin() + 0.025, 1 - (twoDlimits->GetTopMargin() + 0.025), "CMS #scale[0.8]{#it{Preliminary}}");
    mark->DrawLatex(twoDlimits->GetLeftMargin() + 0.025, 1 - (twoDlimits->GetTopMargin() + 0.025), "CMS");
    //text2.DrawLatex(880, 20, "Excluded by Tevatron");
    //mark->DrawLatex(0.18, 0.96, "CMS Preliminary");

    fixOverlay();

    leg->Draw("same");

    switch(mode)
    {
        case 0:
            twoDlimits->Print("Mu_Lim2D.pdf");
            twoDlimits->Print("Mu_Lim2D.png");
            twoDlimits->Print("Mu_Lim2D.eps");
            break;
        case 1:
            twoDlimits->Print("Elec_Lim2D.pdf");
            twoDlimits->Print("Elec_Lim2D.png");
            twoDlimits->Print("Elec_Lim2D.eps");
            break;
        case 2:
            twoDlimits->Print("El-Mu_Lim2D.pdf");
            twoDlimits->Print("El-Mu_Lim2D.png");
            twoDlimits->Print("El-Mu_Lim2D.eps");
            break;
        case 3:
            twoDlimits->Print("Mu_2011-2012_Lim2D.pdf");
            twoDlimits->Print("Mu_2011-2012_Lim2D.png");
            twoDlimits->Print("Mu_2011-2012_Lim2D.eps");
            break;
    }

    //delete[] db;

}

int fprintf_sigFig(FILE * f, double t, int n)
{
    int i;
    for(i = n; pow(10,i) > t; i--);
    i = n - i - 1;
    if(i < 0) i = 0;
    char format[16];
    sprintf(format, "%%0.%dlf", i);
    fprintf(f, format, t);
    
    return 0;
}
