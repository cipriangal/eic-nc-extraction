#include<iostream>
#include<fstream>
#include"TROOT.h"
#include"TH1.h"
#include"TH1F.h"
#include"TFile.h"
#include"TTree.h"
#include"TChain.h"
#include"TString.h"
#include"TMath.h"

using namespace std;

//hard coded parameters:   lg_x_min  lg_x_max  lg_Q2_min  lg_Q2_max  N_binning_x  N_binning_Q2
//cut
//for sin2thw analysis, the x should be cut at x>0.2 so that only valence quarks dominate the asymmetry
//and the u and d quarks cancel in ratio

int main(int argc, char **argv){
	
  int N_binning_x=5;
  int N_binning_Q2=20;

  //         10x100
  //  log(x) -8.3 <---> -0.2
  //  log(Q2) 0 <---> 8.0
  //         10x250
  //  log(x) -9.21 <---> -0.2
  //  log(Q2) 0 <---> 8.5
  //         10x100
  //  log(x) -10.18 <---> -0.2
  //  log(Q2) 0 <---> 9.5

  if(argc!=2){
    cout<<"usage:  ./Get_Q2_x_binning_info 10_100 "<<endl;
    return 1;
  }
	
  TString file_index=argv[1];

  double lg_x_min=0.0;
  double lg_x_max=0.0;
  double lg_Q2_min=0.0;
  double lg_Q2_max=0.0;

  // 10 x 100   100->1500
  // 10 x 250   200->3500
  // 15 x 100   100->2000
  // 15 x 250   200->4000
	
  if(file_index=="10_100"){ //10x100
		
    lg_x_min=log10(0.2);
    lg_x_max=0.0;
    lg_Q2_min=log10(100.0);       //100->1500
    lg_Q2_max=log10(1500.0);
	
  }else if(file_index=="10_250"){ //10x250
		
    lg_x_min=log10(0.2);
    lg_x_max=0.0;
    lg_Q2_min=log10(200.0);  //200->3500
    lg_Q2_max=log10(3500);
	
  }else if(file_index=="20_325"){ //20x325
		
    lg_x_min=log10(0.2);
    lg_x_max=0.0;
    lg_Q2_min=log10(1000.0);        // 1000->6400
    lg_Q2_max=log10(6400.0);
	
  }else if(file_index=="20_250"){ //20x250
		
    lg_x_min=log10(0.2);
    lg_x_max=0.0;
    lg_Q2_min=log10(1000.0);        //1000->6400
    lg_Q2_max=log10(6400.0);
	
  }else if(file_index=="15_100"){ //15x100
		
    lg_x_min=log10(0.2);
    lg_x_max=0.0;
    lg_Q2_min=log10(100.0);        //100->2000
    lg_Q2_max=log10(2000.0);
	
  }else if(file_index=="15_250"){ //15x250
		
    lg_x_min=log10(0.2);
    lg_x_max=0.0;
    lg_Q2_min=log10(200.0);        //200->4000
    lg_Q2_max=log10(4000.0);
	
  }else{
    cout<<"error, unkown beam and target energy!!!"<<endl;
    return 1;
  }
	
	
  double step_lg_x=(lg_x_max-lg_x_min)/N_binning_x;
  double step_lg_Q2=(lg_Q2_max-lg_Q2_min)/N_binning_Q2;

  int ntot;
  double totxs;
  double x_det,Q2_det,W2_det;
  int N_entries;

  //	TString rootfile_name="../NC_yesradiation_" + file_index +"_e_p_unpol_unpol.root";
  //	TFile *file=new TFile(rootfile_name); 
  //	cout<<"#########################################################"<<endl;
  //	cout<<"doing "<<rootfile_name<<endl;
  //	cout<<"#########################################################"<<endl;
	
  //	TTree *T=(TTree*)file->Get("T");
	
  TString name_ROOTFILE_LIST="ROOTFILE_LIST_"+file_index;
  ifstream INPUT_ROOTFILE_LIST;
  INPUT_ROOTFILE_LIST.open(name_ROOTFILE_LIST);
  if(!INPUT_ROOTFILE_LIST){
    cout<<"error !!! can't open ROOTFILE_LIST!"<<endl;
    return 1;
  }
	
  TChain *T=new TChain("T","T");
	
  int generated_num_rootfile=0;
  TString name_current_root_file;
  while(INPUT_ROOTFILE_LIST>>name_current_root_file){
    T->AddFile(name_current_root_file);
    cout<<"chaining...."<<name_current_root_file<<endl;
    generated_num_rootfile++;
  }


  ntot=generated_num_rootfile*1000000;
	
	
  //	T->SetBranchAddress("NTOT",&ntot);
  T->SetBranchAddress("tot_xs",&totxs);
  T->SetBranchAddress("x_det",&x_det);
  T->SetBranchAddress("Q2_det",&Q2_det);
  T->SetBranchAddress("W2_det",&W2_det);
	
  N_entries=T->GetEntries();
  cout<<"total entries......"<<N_entries<<endl;
	
  //	T->GetEntry(0);
  //	cout<<ntot<<"   events are generated!"<<endl;
  //	cout<<totxs<<" pbarn is the total cross section!"<<endl;
	
  TString txtfile_name="Q2_x_binning_information_" + file_index + ".txt";
  cout<<"OUTPUT FILE:  "<<txtfile_name<<endl;

  ofstream OUTPUT_file;
  OUTPUT_file.open(txtfile_name);
  if(!OUTPUT_file){
    cout<<"can't open output file!!!"<<endl;
    return 1;
  }

  //______format lg_Q2_low, lg_Q2_high, lg_x_low, lg_x_high, lg_Q2_mean, lg_x_mean, Q2_mean, x_mean, cross-section

  TH1F *h_x=new TH1F("h_x","x",1000,0,1);
  TH1F *h_lg_x=new TH1F("h_lg_x","lg(x)",1000,log10(0.0000001),log10(1.0));

  TH1F *h_Q2=new TH1F("h_Q2","Q2",1000,1,100000);
  TH1F *h_lg_Q2=new TH1F("h_lg_Q2","lg(Q2)",1000,log10(1.0),log10(100000.0));

  //	TCanvas *can=new TCanvas("can");


  for(int i=0;i<N_binning_Q2;i++){//Q2 loop	
    double lg_Q2_low=lg_Q2_min + step_lg_Q2*i;
    double lg_Q2_high=lg_Q2_low + step_lg_Q2;
		
    for(int j=0;j<N_binning_x;j++){//x loop
      double lg_x_low=lg_x_min + step_lg_x*j;
      double lg_x_high=lg_x_low + step_lg_x;

      h_x->Reset();
      h_lg_x->Reset();
      h_Q2->Reset();
      h_lg_Q2->Reset();


      //base cut!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //___________ base cut Q2>1.0 W2> 4
      for(int k=0;k<N_entries;k++){
	T->GetEntry(k);
	// cut : Q2>1.0, W2>4.0
	if(Q2_det>1.0 && W2_det>4.0 
	   && log10(x_det)>lg_x_low && log10(x_det)<lg_x_high 
	   && log10(Q2_det)>lg_Q2_low && log10(Q2_det)<lg_Q2_high
	   ){
	  h_x->Fill(x_det,totxs/ntot);
	  h_lg_x->Fill(log10(x_det),totxs/ntot);
	  h_Q2->Fill(Q2_det,totxs/ntot);
	  h_lg_Q2->Fill(log10(Q2_det),totxs/ntot);
	}
      }
      //			can->cd();
      //			h_lg_x->Draw();
      //			can->Update();
			
      double x_mean=h_x->GetMean();
      double cross_section=h_x->Integral(1,1000);
      double lg_x_mean=h_lg_x->GetMean();
      double Q2_mean=h_Q2->GetMean();
      double lg_Q2_mean=h_lg_Q2->GetMean();

      OUTPUT_file<<lg_Q2_low<<"	"<<lg_Q2_high<<"	"<<lg_x_low<<"	"<<lg_x_high<<"	"<<lg_Q2_mean<<"	"<<lg_x_mean<<"	"<<Q2_mean<<"	"<<x_mean<<"	"<<cross_section<<endl;
    }
  }
}

