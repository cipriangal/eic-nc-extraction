#include<iostream>
#include<fstream>
using namespace std;

/*
  This script is to get the dp/p resolution as a function of p

*/



const double p_cut=5;   //5 GeV particles

void yield_dp_p_resolution_eta(){
  double eta_min=-1.1;
  double eta_max=1.1;
  int N_bin_eta=10;
	
  double step_eta=(eta_max-eta_min)/N_bin_eta;
	
  //	TFile *myfile=new TFile("erhic-nc-yesradiation_ep_10_100_run_0.root");
  //	TTree *T=myfile->Get("T");
	
	
  ifstream INPUT_filelist;
  INPUT_filelist.open("filelist.txt");
  if(!INPUT_filelist){
    cout<<"error, can't open filelist to be analyzed!"<<endl;
    return 1;
  }
	
  TChain *T=new TChain("T","T");
  TString name_current_file;
  while(INPUT_filelist>>name_current_file){
    T->AddFile(name_current_file);
    cout<<"adding....."<<name_current_file<<endl;
  }



  double x_scaled, y_scaled, Q2_scaled, W2_scaled;
  double x_det, y_det, Q2_det, W2_det;
  double x, y, Q2, W2;

  double px_det, py_det, pz_det, p_det;
  double electron_px, electron_py, electron_pz;

  T->SetBranchAddress("x_scaled", &x_scaled);
  T->SetBranchAddress("y_scaled", &y_scaled);
  T->SetBranchAddress("Q2_scaled", &Q2_scaled);
  T->SetBranchAddress("W2_scaled", &W2_scaled);
	
  T->SetBranchAddress("x_det", &x_det);
  T->SetBranchAddress("y_det", &y_det);
  T->SetBranchAddress("Q2_det", &Q2_det);
  T->SetBranchAddress("W2_det", &W2_det);
	
  T->SetBranchAddress("x", &x);
  T->SetBranchAddress("y", &y);
  T->SetBranchAddress("Q2", &Q2);

  T->SetBranchAddress("px_det",&px_det);
  T->SetBranchAddress("py_det",&py_det);
  T->SetBranchAddress("pz_det",&pz_det);
  T->SetBranchAddress("p_det",&p_det);

  T->SetBranchAddress("electron_px",&electron_px);
  T->SetBranchAddress("electron_py",&electron_py);
  T->SetBranchAddress("electron_pz",&electron_pz);

  long int N_entries=T->GetEntries();
  cout<<" doing ...."<<N_entries<<" events "<<endl;
	
  TH1F *h_eta=new TH1F("h_eta","eta",100,-10,10);
	
  TH1F *h_fit=new TH1F("h_fit", "fit histogram", 1000, -0.035, 0.035);
  TF1 *fit_fun=new TF1("fit_fun","gaus");

  double array_eta[100]={0};
  double array_resolution[100]={0};
  double array_resolution_err[100]={0};
	
  double array_mean[100]={0};
  double array_mean_err[100]={0};

  TCanvas *c1=new TCanvas("c1","c1");
  c1->Draw();
	
  for(int bin_i=0;bin_i<N_bin_eta;bin_i++){
    double eta_bin_low=eta_min + step_eta*bin_i;
    double eta_bin_high=eta_bin_low + step_eta;
    cout<<eta_bin_low<<"	"<<eta_bin_high<<endl;

    h_eta->Reset();
    h_fit->Reset();

    for(long int i=0;i<N_entries;i++){
      T->GetEntry(i);	
			
      double electron_p=sqrt(electron_px*electron_px + electron_py*electron_py + electron_pz*electron_pz);
      double theta=acos(electron_pz/electron_p);
			
      double tmp_eta=-1.0*log(tan(theta/2.0));

      if(tmp_eta>=eta_bin_low && tmp_eta<eta_bin_high && electron_p > (p_cut-0.1) && electron_p < (p_cut+0.1) ){ //eta in range, p in range
	h_eta->Fill(tmp_eta);
	h_fit->Fill((p_det-electron_p)/electron_p);
      }
    } //end for
		
    c1->cd();
    h_fit->Draw();
    h_fit->Fit(fit_fun);
    c1->Update();
		
    array_eta[bin_i]=h_eta->GetMean();
    double tmp_par[5]={0};
    fit_fun->GetParameters(tmp_par);
    array_resolution[bin_i]=tmp_par[2];
    array_resolution_err[bin_i]=fit_fun->GetParError(2);

    array_mean[bin_i]=tmp_par[1];
    array_mean_err[bin_i]=fit_fun->GetParError(1);
  }

  TGraphErrors *g_resolution=new TGraphErrors(N_bin_eta,array_eta, array_resolution,0,array_resolution_err);
  TGraphErrors *g_mean=new TGraphErrors(N_bin_eta, array_eta, array_mean,0,array_mean_err);

  TCanvas *can=new TCanvas("can","can");
  can->Divide(2,1);
  can->cd(1);
  g_resolution->SetMarkerStyle(20);
  g_resolution->SetMarkerSize(2);
  g_resolution->Draw("AP");
  can->cd(2);
  g_mean->SetMarkerStyle(20);
  g_mean->SetMarkerSize(2);
  g_mean->Draw("AP");
	
  can->Print("dp_p_as_eta.png");



}

	

