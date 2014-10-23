{
   TFile* file = new TFile("Mass_OfZ_Within_75And100GeV_TimePerf-plots.root","READ");;
   TH1F* hs    = (TH1F*)file->Get("EBEB/seed time");

   Float_t ecalt; 

   //TNtuple * ntuple = new TNtuple("ntuple","ECAL Time", "ecalt");
   TTree * ntuple = new TTree("ntuple","ECAL Time");
   TBranch* br = ntuple->Branch("ecalt",&ecalt, "ecalt/F");
   Int_t Nentries = hs->GetEntries();
   Int_t Nbins = hs->GetNbinsX();

   //for( Int_t b = 0; b < Nbins; b++ ) {
   for( Int_t b = 0; b < Nentries; b++ ) {

     //ecalt = hs->GetBinContent(b);
    // ecalt = hs->GetBinContent();
     //ntuple->Fill(ecalt);
     ntuple->Fill();
     
     }

  ntuple->Print();

  TFile *f = new TFile("myfile.root", "RECREATE");
  TCanvas* c = new TCanvas("c","c", 800, 700 );
  c->cd();
  ntuple->Draw();
 f->cd();
 ntuple->Write("",TObject::kOverwrite); // save new version only

 //ntuple->Write();
 f->Close();
// f->Write()
}
