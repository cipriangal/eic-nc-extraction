#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include"TROOT.h"
#include "TH1.h"
#include "TMath.h"
#include "TString.h"
#include "TH1F.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TMatrixD.h"
using namespace std;

int main(int argc, char **argv){
  if(argc!=4){
    cout<<"Usage: ./Get_unfolding_matrix_new energy_configuration i(entry) j(N_y index)"<<endl;
    return 1;
  }

  //	cout<<"here..............."<<endl;
  TString energy_configuration=argv[1];
  int i=atoi(argv[2]);// entry
  int j=atoi(argv[3]); // N_y
  //##################cut list here #################################


  //################################################################
  //_____________________constant used _______________________________________________
  double lumi=100000.0;  //10^5 inverse pbarn =  100 inverse fbarn

  double GF=1.166389E-5;
  double alpha=1.0/137.0359895;
  double GF_2_sqrt2_pi_alpha=GF/2.0/sqrt(2.0)/3.14159265/alpha;

  double f_Q2=0;
  //___________________________________________________________________________________
	
  // output matrix to txt file
  TString name_unfolding_matrix="./matrix_"+energy_configuration+"/unfolding_matrix_" + argv[2] + "_" + argv[3] + ".txt";
  ofstream OUTPUT_unfolding_matrix;
  OUTPUT_unfolding_matrix.open(name_unfolding_matrix);
  if(!OUTPUT_unfolding_matrix){
    cout<<"error !!! can't open output file"<<endl;
    return 1;
  }

  //Final_energy_configuration.root
  TString name_final_rootfile="../Final_" + energy_configuration + ".root";
  TChain *T=new TChain("T","T");
  T->AddFile(name_final_rootfile);

  int N_y;   //how many y bins in this (Q2,x) data bin 
  double lg_Q2_bin_low=0, lg_Q2_bin_high=0;
  double lg_x_bin_low=0, lg_x_bin_high=0;
  double y_bin_low[100]={0}, y_bin_high[100]={0};   //to store y binning info
  int rootfile_ID;

  T->SetBranchAddress("rootfile_ID",&rootfile_ID);	
  T->SetBranchAddress("N_y",&N_y);
  T->SetBranchAddress("y_bin_low",y_bin_low);
  T->SetBranchAddress("y_bin_high",y_bin_high);
  T->SetBranchAddress("lg_Q2_bin_low",&lg_Q2_bin_low);
  T->SetBranchAddress("lg_Q2_bin_high",&lg_Q2_bin_high);
  T->SetBranchAddress("lg_x_bin_low",&lg_x_bin_low);
  T->SetBranchAddress("lg_x_bin_high",&lg_x_bin_high);
	
  int N_entries=T->GetEntries();
  //cout<<"here....."<<endl;	
  // Chain skimmed rootfiles
  TChain *skimT = new TChain("skimT","skimT");
	
  TString name_INPUT_filelist="filelist_"+energy_configuration;
  ifstream INPUT_filelist;
  INPUT_filelist.open(name_INPUT_filelist);
  if(!INPUT_filelist){
    cout<<"error!!! can't open input_filelist"<<endl;
    return 1;
  }

  TString current_file_name;
  while(INPUT_filelist>>current_file_name){
    //cout<<"chaining....."<<current_file_name<<endl;
    skimT->AddFile(current_file_name);
  }

  double x_det, y_det, Q2_det;
  double weight;
  double x_scaled, y_scaled, Q2_scaled;
	
  double electron_px=0, electron_py=0, electron_pz=0;
	
  skimT->SetBranchAddress("x_det",&x_det);
  skimT->SetBranchAddress("y_det",&y_det);
  skimT->SetBranchAddress("Q2_det",&Q2_det);
  skimT->SetBranchAddress("weight",&weight);
  skimT->SetBranchAddress("x_scaled",&x_scaled);
  skimT->SetBranchAddress("y_scaled",&y_scaled);
  skimT->SetBranchAddress("Q2_scaled",&Q2_scaled);
			
  skimT->SetBranchAddress("electron_px",&electron_px);
  skimT->SetBranchAddress("electron_py",&electron_py);
  skimT->SetBranchAddress("electron_pz",&electron_pz);
	
  int N_skimT=skimT->GetEntries();		
  //cout<<N_skimT<<endl;	
  int element_num=0;
  //loop binning in Final_energy_configuration.root 
	
	
  //	for(int i=0;i<N_entries;i++){
  T->GetEntry(i);
  double cut_lg_x_bin_low=lg_x_bin_low;
  double cut_lg_x_bin_high=lg_x_bin_high;
  double cut_lg_Q2_bin_low=lg_Q2_bin_low;
  double cut_lg_Q2_bin_high=lg_Q2_bin_high;
  int  N_y_i=N_y;

  double cut_y_bin_low=0;
  double cut_y_bin_high=0;
  cut_y_bin_low=y_bin_low[j];
  cut_y_bin_high=y_bin_high[j];

  //loop Final_energy_configuration.root again
  //to calculate R(observed in bin | true value in bin j)
  element_num=0;
  double R_matrix[1000][100]={{0}};

  for(int ii=0;ii<N_entries;ii++){
    T->GetEntry(ii);

    double sum_true=0;
    double sum_obs=0;

    double current_lg_x_bin_low=lg_x_bin_low;
    double current_lg_x_bin_high=lg_x_bin_high;
    double current_lg_Q2_bin_low=lg_Q2_bin_low;
    double current_lg_Q2_bin_high=lg_Q2_bin_high;
    int N_y_ii=N_y;

    double current_y_bin_low=0;
    double current_y_bin_high=0;
    for(int jj=0;jj<N_y_ii;jj++){
      //					cout<<"###########"<<ii<<"	"<<jj<<endl;
      current_y_bin_low=y_bin_low[jj];
      current_y_bin_high=y_bin_high[jj];

      element_num++;
      sum_true=0;
      sum_obs=0;

      //loop skimT to check each event
      for(int k=0;k<N_skimT;k++){
	skimT->GetEntry(k);
						
	//f_Q2=GF_2_sqrt2_pi_alpha*Q2_scaled;
	//true Q2,x,y in current bin
	// physics cuts here...............................................................................................................
	if(1.0>0.0
	   &&log10(Q2_scaled)>=current_lg_Q2_bin_low && log10(Q2_scaled)<=current_lg_Q2_bin_high
	   && log10(x_scaled)>=current_lg_x_bin_low && log10(x_scaled)<=current_lg_x_bin_high
	   && y_scaled>=current_y_bin_low && y_scaled<=current_y_bin_high){
							
	  f_Q2=GF_2_sqrt2_pi_alpha*Q2_scaled;
	  //true kinematics in this bin
	  sum_true += f_Q2*f_Q2*weight*lumi;

	  if(1.0>0.0
	     &&log10(Q2_det)>=cut_lg_Q2_bin_low && log10(Q2_det)<=cut_lg_Q2_bin_high
	     &&log10(x_det)>=cut_lg_x_bin_low && log10(x_det)<=cut_lg_x_bin_high
	     &&y_det>=cut_y_bin_low && y_det<=cut_y_bin_high){

	    //observed kinematics in the considered bin
                                                               
	    f_Q2=GF_2_sqrt2_pi_alpha*Q2_det;
	    sum_obs += f_Q2*f_Q2*weight*lumi;
	  }
	}
      }// scan over all events in skimT
			
      // if the event sample is too large, one or two y bins will not have true data, set it to 
      // small number in order reverse matrix can success
      if(sum_true!=0){
	R_matrix[ii][jj]=sum_obs/sum_true;
      }else{
	R_matrix[ii][jj]=0.00000001;
      }

    }//jj
  }//ii

  //output for one considered bin i,j
  //			if(i==0&&j==0){//output dimension of the matrix
  OUTPUT_unfolding_matrix<<element_num<<endl;
  //			}

  OUTPUT_unfolding_matrix<<i<<"	"<<j<<endl;	
  //small trick: if survivability is low enough, then set this element to a small value
  double check_sum=0;
  for(int check_i=0;check_i<N_entries;check_i++){
    T->GetEntry(check_i);

    int N_y_check_i=N_y;

    for(int check_j=0;check_j<N_y_check_i;check_j++){
      check_sum += R_matrix[check_i][check_j];
    }
  }
			
  if(check_sum==0){
    R_matrix[i][j]=0.000001;
  }
  // trick finished


  for(int out_i=0;out_i<N_entries;out_i++){ //Q2, x bin
    T->GetEntry(out_i);

    int N_y_out_i=N_y;
				
    for(int out_j=0;out_j<N_y_out_i;out_j++){

      OUTPUT_unfolding_matrix<<R_matrix[out_i][out_j]<<"	";
    }
  }
  OUTPUT_unfolding_matrix<<endl;
  //		}//j
  //	}//i


}







