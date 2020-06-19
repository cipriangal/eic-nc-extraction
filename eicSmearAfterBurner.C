#include "eicsmear/erhic/EventDjangoh.h"
R__LOAD_LIBRARY(libeicsmear.so)

TFile *fin,*fout;
TTree *tin,*tout;
erhic::EventDjangoh *gen= nullptr;

// const double pi=acos(-1);
// const double gf = 1.166379e-5; //GeV^{-2}
// const double alpha = 1/137;
// const double mz = 91.1876; //GeV

double totxs(0);
double Q2(0), x(0), y(0);
double x_det=0, y_det=0, Q2_det=0, W2_det=0;
double px_det=0, py_det=0, pz_det=0, p_det=0;

double F1G=0, F1GZ=0, F1Z=0, F3GZ=0, F3Z=0;
double G1G=0, G1GZ=0, G1Z=0, G5GZ=0, G5Z=0;
double F1CC=0, F3CC=0;
double G1CC=0, G5CC=0;

double F1GLQ=0, F1GZLQ=0, F1ZLQ=0, F3GZLQ=0, F3ZLQ=0;
double G1GLQ=0, G1GZLQ=0, G1ZLQ=0, G5GZLQ=0, G5ZLQ=0;
double F1CCLQ=0, F3CCLQ=0;
double G1CCLQ=0, G5CCLQ=0;

double F1GT=0, F1GZT=0, F1ZT=0, F3GZT=0, F3ZT=0;
double G1GT=0, G1GZT=0, G1ZT=0, G5GZT=0, G5ZT=0;
double F1CCT=0, F3CCT=0;
double G1CCT=0, G5CCT=0;

double x_scaled=0, y_scaled=0, Q2_scaled=0, W2_scaled=0;
double electron_px=0, electron_py=0, electron_pz=0;

double sum_u=0, sum_d=0, sum_s=0, sum_c=0;
double sum_u_t=0, sum_d_t=0, sum_s_t=0, sum_c_t=0;

int readIn(string finNm);
void setOutTree(string finNm);
void initVars();
void printIn();

void eicSmearAfterBurner(string finNm="/phenix/spin/phnxsp01/ciprian/eic/djangohOut/o_dj_epI_Rad1_NLO_FL_FRAG_NC_20x250_evt.500000event.root"){
  gSystem->Load("libeicsmear.so");

  if(readIn(finNm)){
    cout<<"can't open "<<finNm<<" quitting!"<<endl;
    return;
  }

  setOutTree(finNm);
  
  const long nEntries = tin->GetEntries();
  //for(long i =0; i<nEntries;i++){
  for(long i =0; i<3;i++){
    //tin->Show(i);
    tin->GetEntry(i);
    //printIn();
    //std::cin.ignore();
    initVars();
    tout->Fill();
  }

  fout->cd();
  tout->Write();
  fout->Close();
  fin->Close();
}

int readIn(string fnm){
  fin=TFile::Open(fnm.c_str(),"READ");
  if(!fin->IsOpen()) return 1;

  tin=(TTree*)fin->Get("EICTree");
  tin->SetBranchAddress("event", &gen);
  return 0;
}

void setOutTree(string fnm){
  string foutNm = fnm.substr(fnm.find_last_of("/")+1,fnm.find_last_of(".")-fnm.find_last_of("/")-1) + string("_AfterBurner.root");
  fout = new TFile(foutNm.c_str(),"RECREATE");

  tout=new TTree("T","T");
  tout->Branch("tot_xs",&totxs,"tot_xs/D");
  tout->Branch("x",&x,"x/D");
  tout->Branch("y",&y,"y/D");
  tout->Branch("Q2",&Q2,"Q2/D");
  tout->Branch("x_det",&x_det,"x_det/D");
  tout->Branch("y_det",&y_det,"y_det/D");
  tout->Branch("Q2_det",&Q2_det,"Q2_det/D");
  tout->Branch("W2_det",&W2_det,"W2_det/D");
  tout->Branch("px_det",&px_det,"px_det/D");
  tout->Branch("py_det",&py_det,"py_det/D");
  tout->Branch("pz_det",&pz_det,"pz_det/D");
  tout->Branch("p_det",&p_det,"p_det/D");
  tout->Branch("F1G",&F1G,"F1G/D");
  tout->Branch("F1GZ",&F1GZ,"F1GZ/D");
  tout->Branch("F1Z",&F1Z,"F1Z/D");
  tout->Branch("F3GZ",&F3GZ,"F3GZ/D");
  tout->Branch("F3Z",&F3Z,"F3Z/D");
  tout->Branch("G1G",&G1G,"G1G/D");
  tout->Branch("G1GZ",&G1GZ,"G1GZ/D");
  tout->Branch("G1Z",&G1Z,"G1Z/D");
  tout->Branch("G5GZ",&G5GZ,"G5GZ/D");
  tout->Branch("G5Z",&G5Z,"G5Z/D");
  tout->Branch("F1CC",&F1CC,"F1CC/D");
  tout->Branch("F3CC",&F3CC,"F3CC/D");
  tout->Branch("G1CC",&G1CC,"G1CC/D");
  tout->Branch("G5CC",&G5CC,"G5CC/D");
  tout->Branch("F1GLQ",&F1GLQ,"F1GLQ/D");
  tout->Branch("F1GZLQ",&F1GZLQ,"F1GZLQ/D");
  tout->Branch("F1ZLQ",&F1ZLQ,"F1ZLQ/D");
  tout->Branch("F3GZLQ",&F3GZLQ,"F3GZLQ/D");
  tout->Branch("F3ZLQ",&F3ZLQ,"F3ZLQ/D");
  tout->Branch("G1GLQ",&G1GLQ,"G1GLQ/D");
  tout->Branch("G1GZLQ",&G1GZLQ,"G1GZLQ/D");
  tout->Branch("G1ZLQ",&G1ZLQ,"G1ZLQ/D");
  tout->Branch("G5GZLQ",&G5GZLQ,"G5GZLQ/D");
  tout->Branch("G5ZLQ",&G5ZLQ,"G5ZLQ/D");
  tout->Branch("F1CCLQ",&F1CCLQ,"F1CCLQ/D");
  tout->Branch("F3CCLQ",&F3CCLQ,"F3CCLQ/D");
  tout->Branch("G1CCLQ",&G1CCLQ,"G1CCLQ/D");
  tout->Branch("G5CCLQ",&G5CCLQ,"G5CCLQ/D");
  tout->Branch("F1GT",&F1GT,"F1GT/D");
  tout->Branch("F1GZT",&F1GZT,"F1GZT/D");
  tout->Branch("F1ZT",&F1ZT,"F1ZT/D");
  tout->Branch("F3GZT",&F3GZT,"F3GZT/D");
  tout->Branch("F3ZT",&F3ZT,"F3ZT/D");
  tout->Branch("G1GT",&G1GT,"G1GT/D");
  tout->Branch("G1GZT",&G1GZT,"G1GZT/D");
  tout->Branch("G1ZT",&G1ZT,"G1ZT/D");
  tout->Branch("G5GZT",&G5GZT,"G5GZT/D");
  tout->Branch("G5ZT",&G5ZT,"G5ZT/D");
  tout->Branch("F1CCT",&F1CCT,"F1CCT/D");
  tout->Branch("F3CCT",&F3CCT,"F3CCT/D");
  tout->Branch("G1CCT",&G1CCT,"G1CCT/D");
  tout->Branch("G5CCT",&G5CCT,"G5CCT/D");
  tout->Branch("x_scaled",&x_scaled,"x_scaled/D");
  tout->Branch("y_scaled",&y_scaled,"y_scaled/D");
  tout->Branch("Q2_scaled",&Q2_scaled,"Q2_scaled/D");
  tout->Branch("W2_scaled",&W2_scaled,"W2_scaled/D");
  tout->Branch("electron_px",&electron_px,"electron_px/D");
  tout->Branch("electron_py",&electron_py,"electron_py/D");
  tout->Branch("electron_pz",&electron_pz,"electron_pz/D");
  tout->Branch("SUM_U",&sum_u,"SUM_U/D");
  tout->Branch("SUM_D",&sum_d,"SUM_D/D");
  tout->Branch("SUM_S",&sum_s,"SUM_S/D");
  tout->Branch("SUM_C",&sum_c,"SUM_C/D");
  tout->Branch("SUM_U_T",&sum_u_t,"SUM_U_T/D");
  tout->Branch("SUM_D_T",&sum_d_t,"SUM_D_T/D");
  tout->Branch("SUM_S_T",&sum_s_t,"SUM_S_T/D");
  tout->Branch("SUM_C_T",&sum_c_t,"SUM_C_T/D");			
}

void initVars(){
  totxs = gen->sigTot;
  x = gen->dtrueX;
  y = gen->dtrueY;
  Q2 = gen->dtrueQ2;
  
  x_det = gen->dX;
  y_det = gen->dY;
  Q2_det = gen->dQ2;
  W2_det = gen->dW2;

  /*FIXME implement MCParctile read to get electron info*/
  // px_det = 0;
  // py_det = 0;
  // pz_det = 0;
  // p_det = 0;

  // F1NC = gen->F1NC;
  // F3NC = gen->F3NC;
  // G1NC = gen->G1NC;
  // G3NC = getn->G3NC;
  //double etaGZ = gf * mz*mz*Q2 / (2*sqrt(2)*pi*alpha * (mz*mz + Q2) );

  F1G = gen->F1G;
  //F3G = gen->F3G;
  G1G = gen->G1G;
  //G3G = gen->G3G;

  F1GZ = gen->F1GZ;
  F3GZ = gen->F3GZ;
  G1GZ = gen->G1GZ;
  G5GZ = gen->G3GZ;

  //F1Z = gen->F1Z;
  F3Z = gen->F3Z;
  G1Z = gen->G1Z;
  G5Z = gen->G3Z;

}

void printIn(){
  cout<< gen->dX  <<"\t"<<
    gen->dNu <<"\t"<<
    gen->dQ2 <<"\t"<<
    gen->dY  <<"\t"<<
    
    gen->F1NC <<"\t"<<
    gen->F3NC <<"\t"<<
    gen->G1NC <<"\t"<<
    gen->G3NC <<"\t"<<
    
    gen->F1CC <<"\t"<<
    gen->F3CC <<"\t"<<
    gen->G1CC <<"\t"<<
    gen->G5CC <<"\t"<<
    
    gen->sigTot <<"\t"<<
    gen->sigTotErr <<"\t"<<
    
    gen->dstruckparton <<"\t"<<endl;
}
