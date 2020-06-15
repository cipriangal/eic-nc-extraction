#include<iostream>
#include<fstream>
#include"TROOT.h"
#include"TString.h"
#include"TFile.h"
#include"TTree.h"
#include"TChain.h"
#include"TMath.h"


using namespace std;

int main(int argc, char **argv){
  if(argc!=2){
    cout<<"usage:  ./skim_tree 10_100 "<<endl;
    return 1;
  }
  TString file_index=argv[1];
  TString filename="Q2_x_binning_information_" + file_index + ".txt";
  cout<<"##############################################################"<<endl;
  cout<<"#             doing       "<<filename<<endl;
  cout<<"##############################################################"<<endl;


  ofstream OUTPUT_final_binning;
  TString output_final_name="Effective_Q2_x_binning_information_" + file_index + ".txt";
  OUTPUT_final_binning.open(output_final_name);

  double lg_Q2_low, lg_Q2_high, lg_x_low, lg_x_high, lg_Q2_mean, lg_x_mean, Q2_mean, x_mean, cross_section;
  ifstream INPUT_file;
  INPUT_file.open(filename);
  if(!INPUT_file){
    cout<<"error!!! can't open input file!"<<endl;
    return 1;
  }


  double F1G=0, F1GZ=0, F1Z=0, F3GZ=0, F3Z=0;
  double G1G=0, G1GZ=0, G1Z=0, G5GZ=0, G5Z=0;
  double F1CC=0, F3CC=0;
  double G1CC=0, G5CC=0;

  double F1GT=0, F1GZT=0, F1ZT=0, F3GZT=0, F3ZT=0;
  double G1GT=0, G1GZT=0, G1ZT=0, G5GZT=0, G5ZT=0;
  double F1CCT=0, F3CCT=0;
  double G1CCT=0, G5CCT=0;

  double F1GLQ=0, F1GZLQ=0, F1ZLQ=0, F3GZLQ=0, F3ZLQ=0;
  double G1GLQ=0, G1GZLQ=0, G1ZLQ=0, G5GZLQ=0, G5ZLQ=0;
  double F1CCLQ=0, F3CCLQ=0;
  double G1CCLQ=0, G5CCLQ=0;

  double x_scaled=0, y_scaled=0, Q2_scaled=0, W2_scaled=0;
  double electron_px=0, electron_py=0, electron_pz=0;
	
  double Q2, x, y;
  double x_det=0, y_det=0, Q2_det=0, W2_det=0;
  double px_det=0, py_det=0, pz_det=0, p_det=0;

	
  double sum_u=0, sum_d=0, sum_s=0, sum_c=0;
  double sum_u_t=0, sum_d_t=0, sum_s_t=0, sum_c_t=0;

  long int ntot;
  long int N_entries;
  double totxs;

  double weight;
	
  double bin_lg_x_low=0, bin_lg_x_high=0, bin_lg_Q2_low=0, bin_lg_Q2_high=0;

  int i=0;
  while(INPUT_file>>lg_Q2_low>>lg_Q2_high>>lg_x_low>>lg_x_high>>lg_Q2_mean>>lg_x_mean>>Q2_mean>>x_mean>>cross_section){
    if(cross_section>0){  //have data
      i++;
			
      OUTPUT_final_binning<<i<<"	"<<lg_Q2_low<<"	"<<lg_Q2_high<<"	"<<lg_x_low<<"	"<<lg_x_high<<"	"<<lg_Q2_mean<<"	"<<lg_x_mean<<"	"<<Q2_mean<<"	"<<x_mean<<"	"<<cross_section<<endl;
			
			
      bin_lg_x_low=lg_x_low;
      bin_lg_x_high=lg_x_high;
      bin_lg_Q2_low=lg_Q2_low;
      bin_lg_Q2_high=lg_Q2_high;

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
	//cout<<"chaining...."<<name_current_root_file<<endl;
	generated_num_rootfile++;
      }
	
	
      ntot=generated_num_rootfile*1000000;
	


      T->SetBranchAddress("tot_xs",&totxs);
      T->SetBranchAddress("x",&x);
      T->SetBranchAddress("y",&y);
      T->SetBranchAddress("Q2",&Q2);
			
      T->SetBranchAddress("x_det",&x_det);
      T->SetBranchAddress("y_det",&y_det);
      T->SetBranchAddress("Q2_det",&Q2_det);
      T->SetBranchAddress("W2_det",&W2_det);


      T->SetBranchAddress("px_det",&px_det);
      T->SetBranchAddress("py_det",&py_det);
      T->SetBranchAddress("pz_det",&pz_det);
      T->SetBranchAddress("p_det",&p_det);


      T->SetBranchAddress("F1G",&F1G);
      T->SetBranchAddress("F1GZ",&F1GZ);
      T->SetBranchAddress("F1Z",&F1Z);
      T->SetBranchAddress("F3GZ",&F3GZ);
      T->SetBranchAddress("F3Z",&F3Z);

      T->SetBranchAddress("G1G",&G1G);
      T->SetBranchAddress("G1GZ",&G1GZ);
      T->SetBranchAddress("G1Z",&G1Z);
      T->SetBranchAddress("G5GZ",&G5GZ);
      T->SetBranchAddress("G5Z",&G5Z);

      T->SetBranchAddress("F1CC",&F1CC);
      T->SetBranchAddress("F3CC",&F3CC);
      T->SetBranchAddress("G1CC",&G1CC);
      T->SetBranchAddress("G5CC",&G5CC);
			
      T->SetBranchAddress("F1GLQ",&F1GLQ);
      T->SetBranchAddress("F1GZLQ",&F1GZLQ);
      T->SetBranchAddress("F1ZLQ",&F1ZLQ);
      T->SetBranchAddress("F3GZLQ",&F3GZLQ);
      T->SetBranchAddress("F3ZLQ",&F3ZLQ);

      T->SetBranchAddress("G1GLQ",&G1GLQ);
      T->SetBranchAddress("G1GZLQ",&G1GZLQ);
      T->SetBranchAddress("G1ZLQ",&G1ZLQ);
      T->SetBranchAddress("G5GZLQ",&G5GZLQ);
      T->SetBranchAddress("G5ZLQ",&G5ZLQ);

      T->SetBranchAddress("F1CCLQ",&F1CCLQ);
      T->SetBranchAddress("F3CCLQ",&F3CCLQ);
      T->SetBranchAddress("G1CCLQ",&G1CCLQ);
      T->SetBranchAddress("G5CCLQ",&G5CCLQ);

      T->SetBranchAddress("F1GT",&F1GT);
      T->SetBranchAddress("F1GZT",&F1GZT);
      T->SetBranchAddress("F1ZT",&F1ZT);
      T->SetBranchAddress("F3GZT",&F3GZT);
      T->SetBranchAddress("F3ZT",&F3ZT);

      T->SetBranchAddress("G1GT",&G1GT);
      T->SetBranchAddress("G1GZT",&G1GZT);
      T->SetBranchAddress("G1ZT",&G1ZT);
      T->SetBranchAddress("G5GZT",&G5GZT);
      T->SetBranchAddress("G5ZT",&G5ZT);

      T->SetBranchAddress("F1CCT",&F1CCT);
      T->SetBranchAddress("F3CCT",&F3CCT);
      T->SetBranchAddress("G1CCT",&G1CCT);
      T->SetBranchAddress("G5CCT",&G5CCT);

      T->SetBranchAddress("x_scaled",&x_scaled);
      T->SetBranchAddress("y_scaled",&y_scaled);
      T->SetBranchAddress("Q2_scaled",&Q2_scaled);
      T->SetBranchAddress("W2_scaled",&W2_scaled);
			
      T->SetBranchAddress("electron_px",&electron_px);
      T->SetBranchAddress("electron_py",&electron_py);
      T->SetBranchAddress("electron_pz",&electron_pz);
			
      T->SetBranchAddress("SUM_U",&sum_u);
      T->SetBranchAddress("SUM_D",&sum_d);
      T->SetBranchAddress("SUM_S",&sum_s);
      T->SetBranchAddress("SUM_C",&sum_c);
			
      T->SetBranchAddress("SUM_U_T",&sum_u_t);
      T->SetBranchAddress("SUM_D_T",&sum_d_t);
      T->SetBranchAddress("SUM_S_T",&sum_s_t);
      T->SetBranchAddress("SUM_C_T",&sum_c_t);
			

			
			
      N_entries=T->GetEntries();
			
      //T->GetEntry(0);
      //cout<<ntot<<"   events are generated!"<<endl;
      //cout<<totxs<<" pbarn is the total cross section!"<<endl;
			
      TString newfilename=Form("./skim_data/file_bin_%d.root",i);
      cout<<"doing file: "<<newfilename<<endl;

      TFile *new_file=new TFile(newfilename,"RECREATE");
      TTree *skimT=new TTree("skimT","skimT");
			
      skimT->Branch("bin_lg_x_low",&bin_lg_x_low,"bin_lg_x_low/D");
      skimT->Branch("bin_lg_x_high",&bin_lg_x_high,"bin_lg_x_high/D");
      skimT->Branch("bin_lg_Q2_low",&bin_lg_Q2_low,"bin_lg_Q2_low/D");
      skimT->Branch("bin_lg_Q2_high",&bin_lg_Q2_high,"bin_lg_Q2_high/D");

      skimT->Branch("x",&x,"x/D");
      skimT->Branch("y",&y,"y/D");
      skimT->Branch("Q2",&Q2,"Q2/D");
			
      skimT->Branch("weight",&weight,"weight/D");

      skimT->Branch("x_det",&x_det,"x_det/D");
      skimT->Branch("y_det",&y_det,"y_det/D");
      skimT->Branch("Q2_det",&Q2_det,"Q2_det/D");
      skimT->Branch("W2_det",&W2_det,"W2_det/D");

      skimT->Branch("px_det",&px_det,"px_det/D");
      skimT->Branch("py_det",&py_det,"py_det/D");
      skimT->Branch("pz_det",&pz_det,"pz_det/D");
      skimT->Branch("p_det",&p_det,"p_det/D");

      skimT->Branch("F1G",&F1G,"F1G/D");
      skimT->Branch("F1GZ",&F1GZ,"F1GZ/D");
      skimT->Branch("F1Z",&F1Z,"F1Z/D");
      skimT->Branch("F3GZ",&F3GZ,"F3GZ/D");
      skimT->Branch("F3Z",&F3Z,"F3Z/D");

      skimT->Branch("G1G",&G1G,"G1G/D");
      skimT->Branch("G1GZ",&G1GZ,"G1GZ/D");
      skimT->Branch("G1Z",&G1Z,"G1Z/D");
      skimT->Branch("G5GZ",&G5GZ,"G5GZ/D");
      skimT->Branch("G5Z",&G5Z,"G5Z/D");

      skimT->Branch("F1CC",&F1CC,"F1CC/D");
      skimT->Branch("F3CC",&F3CC,"F3CC/D");
      skimT->Branch("G1CC",&G1CC,"G1CC/D");
      skimT->Branch("G5CC",&G5CC,"G5CC/D");

      skimT->Branch("F1GLQ",&F1GLQ,"F1GLQ/D");
      skimT->Branch("F1GZLQ",&F1GZLQ,"F1GZLQ/D");
      skimT->Branch("F1ZLQ",&F1ZLQ,"F1ZLQ/D");
      skimT->Branch("F3GZLQ",&F3GZLQ,"F3GZLQ/D");
      skimT->Branch("F3ZLQ",&F3ZLQ,"F3ZLQ/D");

      skimT->Branch("G1GLQ",&G1GLQ,"G1GLQ/D");
      skimT->Branch("G1GZLQ",&G1GZLQ,"G1GZLQ/D");
      skimT->Branch("G1ZLQ",&G1ZLQ,"G1ZLQ/D");
      skimT->Branch("G5GZLQ",&G5GZLQ,"G5GZLQ/D");
      skimT->Branch("G5ZLQ",&G5ZLQ,"G5ZLQ/D");

      skimT->Branch("F1CCLQ",&F1CCLQ,"F1CCLQ/D");
      skimT->Branch("F3CCLQ",&F3CCLQ,"F3CCLQ/D");
      skimT->Branch("G1CCLQ",&G1CCLQ,"G1CCLQ/D");
      skimT->Branch("G5CCLQ",&G5CCLQ,"G5CCLQ/D");


      skimT->Branch("F1GT",&F1GT,"F1GT/D");
      skimT->Branch("F1GZT",&F1GZT,"F1GZT/D");
      skimT->Branch("F1ZT",&F1ZT,"F1ZT/D");
      skimT->Branch("F3GZT",&F3GZT,"F3GZT/D");
      skimT->Branch("F3ZT",&F3ZT,"F3ZT/D");

      skimT->Branch("G1GT",&G1GT,"G1GT/D");
      skimT->Branch("G1GZT",&G1GZT,"G1GZT/D");
      skimT->Branch("G1ZT",&G1ZT,"G1ZT/D");
      skimT->Branch("G5GZT",&G5GZT,"G5GZT/D");
      skimT->Branch("G5ZT",&G5ZT,"G5ZT/D");

      skimT->Branch("F1CCT",&F1CCT,"F1CCT/D");
      skimT->Branch("F3CCT",&F3CCT,"F3CCT/D");
      skimT->Branch("G1CCT",&G1CCT,"G1CCT/D");
      skimT->Branch("G5CCT",&G5CCT,"G5CCT/D");


      skimT->Branch("x_scaled",&x_scaled,"x_scaled/D");
      skimT->Branch("y_scaled",&y_scaled,"y_scaled/D");
      skimT->Branch("Q2_scaled",&Q2_scaled,"Q2_scaled/D");
      skimT->Branch("W2_scaled",&W2_scaled,"W2_scaled/D");
			
      skimT->Branch("electron_px",&electron_px,"electron_px/D");
      skimT->Branch("electron_py",&electron_py,"electron_py/D");
      skimT->Branch("electron_pz",&electron_pz,"electron_pz/D");
			
      skimT->Branch("SUM_U",&sum_u,"SUM_U/D");
      skimT->Branch("SUM_D",&sum_d,"SUM_D/D");
      skimT->Branch("SUM_S",&sum_s,"SUM_S/D");
      skimT->Branch("SUM_C",&sum_c,"SUM_C/D");
			
      skimT->Branch("SUM_U_T",&sum_u_t,"SUM_U_T/D");
      skimT->Branch("SUM_D_T",&sum_d_t,"SUM_D_T/D");
      skimT->Branch("SUM_S_T",&sum_s_t,"SUM_S_T/D");
      skimT->Branch("SUM_C_T",&sum_c_t,"SUM_C_T/D");
			



      for(int j=0;j<N_entries;j++){
	T->GetEntry(j);
	//base cut!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//pass cut then fill new tree base cut: Q2>1 && W2_hadron>4
	if(Q2_det>1.0 && W2_det>4.0 
	   && log10(x_det)>lg_x_low && log10(x_det)<lg_x_high 
	   && log10(Q2_det)>lg_Q2_low && log10(Q2_det)<lg_Q2_high
	   ){
	  weight=totxs/ntot;
					
	  skimT->Fill();
	}

      }

      new_file->Write();
      new_file->Save();
			
      delete skimT;
			
      new_file->Close();
      delete new_file;

      delete T;
			
      //	file->Close();
      //	delete file;



    }
  }
}







	
