/* 
 * usage: 
 * -at the end of the present file, specify input trees, luminosity and m4l window
 * -run with:
 *   root -q -l -b prepareYields.C++
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TLegend.h"
#include "TMath.h"
#include "TPaveStats.h"
#include "TSpline.h"
#include "TSystem.h"
#include "TTree.h"

//#include "Djetefficiency.C"
#include "../Plotter/tdrstyle.C"
#include "../Plotter/plotUtils.C"
#include "ZZAnalysis/AnalysisStep/src/kFactors.C"
#include <ZZAnalysis/AnalysisStep/src/Category.cc>

using namespace std;

#define DEBUG 0
#define MERGE2E2MU 1

#define APPLYKFACTORS 1
#define USEPUWEIGHT 1

#define USEDJETEFFICIENCYFORGGZZ 1
#define GGZZDJETEFFICIENCY 0.02572353

#define DOZPLUSXFROMRUN2COMBINEDSHAPE 1
#define ZPLUSXDJETEFFICIENCY 0.02803343

#define JUST125 0



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////// global variables /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

enum Process {ggH=0, qqH=1, WH=2, ZH=3, ttH=4, qqZZ=5, ggZZ=6};
const int nProcesses = 7;
string sProcess[nProcesses] = {"ggH", "qqH", "WH", "ZH", "ttH", "qqZZ", "ggZZ"};
bool isSignal[nProcesses] = {1,1,1,1,1,0,0,};

/*
const int nMHPoints = 35;
string sMHPoint[nMHPoints] = {"","115","120","124","125","126","130","135","140","145","150","155","160","165","170","175","180","190","200","210","230","250","270","300","350","400","450","500","550","600","700","750","800","900","1000",};
Float_t fMHPoint[nMHPoints] = {0., 115., 120., 124., 125., 126., 130., 135., 140., 145., 150., 155., 160., 165., 170., 175., 180., 190., 200., 210., 230., 250., 270., 300., 350., 400., 450., 500., 550., 600., 700., 750., 800., 900., 1000.};
int indexOf125 = 4;
Int_t nMHPointsProcess[nProcesses] = {34,34,25,25,18,0,0};
bool hasMHPoint[nProcesses][nMHPoints] = {
  //{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},//ggH
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,},//ggH
  //{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},//qqH
  {0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,},//qqH
  //{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,},//WH
  {0,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,},//WH
  //{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,},//ZH
  {0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,},//ZH
  //{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},//ttH
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},//ttH
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},//qqZZ
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},//ggZZ
};
const int orderOfPolynomial[nProcesses] = {8,8,8,8,8,0,0};
//*/

/*
const int nMHPoints = 6;
string  sMHPoint[nMHPoints] = {"","120","124","125","126","130",};
Float_t fMHPoint[nMHPoints] = {0., 120., 124., 125., 126., 130.,};
int indexOf125 = 3;
//Int_t nMHPointsProcess[nProcesses] = {5,5,5,5,5,0,0};
Int_t nMHPointsProcess[nProcesses] = {5,5,4,5,5,0,0}; //FIXME: some mass points are still missing
bool hasMHPoint[nProcesses][nMHPoints] = {
  {0,1,1,1,1,1,},//ggH
  {0,1,1,1,1,1,},//qqH
  //{0,1,1,1,1,1,},//WH
  {0,1,1,1,0,1,},//WH
  {0,1,1,1,1,1,},//ZH
  {0,1,1,1,1,1,},//ttH
  {1,0,0,0,0,0,},//qqZZ
  {1,0,0,0,0,0,},//ggZZ
};
const int orderOfPolynomial[nProcesses] = {2,2,2,2,2,0,0};
//*/

//*
const int nMHPoints = 7;
string  sMHPoint[nMHPoints] = {"","115","120","124","125","126","130",};
Float_t fMHPoint[nMHPoints] = {0., 115., 120., 124., 125., 126., 130.,};
int indexOf125 = 4;
//Int_t nMHPointsProcess[nProcesses] = {6,6,6,6,6,0,0};
Int_t nMHPointsProcess[nProcesses] = {6,6,5,5,6,0,0}; //FIXME: some mass points are still missing
bool hasMHPoint[nProcesses][nMHPoints] = {
  {0,1,1,1,1,1,1,},//ggH
  {0,1,1,1,1,1,1,},//qqH
  //{0,1,1,1,1,1,1,},//WH
  {0,1,1,1,1,0,1,},//WH
  //{0,1,1,1,1,1,1,},//ZH
  {0,0,1,1,1,1,1,},//ZH
  {0,1,1,1,1,1,1,},//ttH
  {1,0,0,0,0,0,0,},//qqZZ
  {1,0,0,0,0,0,0,},//ggZZ
};
const int orderOfPolynomial[nProcesses] = {2,2,2,2,2,0,0};
//*/

enum FinalState {fs4mu=0, fs4e=1, fs2e2mu=2, fs2mu2e=3};
const int nFinalStates = 4;
string sFinalState[nFinalStates+1] = {"4mu", "4e", "2e2mu", "2mu2e", "4l"};
Int_t fsMarkerStyle[nFinalStates+1] = {20,22,21,33,29};


/* ---------- full RunII categorization 
const int nCategories = 6;
string sCategory[nCategories+1] = {
  "UnTagged",
  "OneJetTagged",
  "VBFTagged", 
  "VHLeptTagged",
  "VHHadrTagged",
  "ttHTagged",
  "inclusive",
};
Color_t catColor[nCategories+1] = {kBlue-9, kCyan-6, kGreen-6, kRed-7, kOrange+6, kMagenta-6, kBlack, };
//*/

//* ---------- Moriond 2016 categorization 
const int nCategories = 2;
string sCategory[nCategories+1] = {
  "UnTagged",
  "VBFTagged", 
  "inclusive", 
};
Color_t catColor[nCategories+1] = {kBlue-9, kGreen-6, kBlack, };
//*/

enum ResonantStatus {resonant=0, nonresonant=1};
const int nResStatuses = 2;
string sResonantStatus[nResStatuses+1] = {"resonant", "nonresonant", "allres"};

Double_t deltaR(Double_t e1, Double_t p1, Double_t e2, Double_t p2) {
  Double_t deltaPhi = acos(cos(p1-p2));
  return TMath::Sqrt((e1-e2)*(e1-e2) + deltaPhi*deltaPhi);
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// compute and save yields /////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void computeYields(string inputFilePathSignal, string inputFilePathqqZZ, string inputFilePathggZZ, double lumi, double sqrts, double m4lMin, double m4lMax)
{

  TH1::SetDefaultSumw2(true);

  TFile* ggZZKFactorFile = TFile::Open("../../data/kfactors/Kfactor_Collected_ggHZZ_2l2l_NNLO_NNPDF_NarrowWidth_13TeV.root");
  TSpline3* sp = (TSpline3*)ggZZKFactorFile->Get("sp_kfactor_Nominal");

  const int nDatasets = 13;
  string datasets[nDatasets] = {
    "ggH",
    "VBFH",
    "WplusH",
    "WminusH",
    "ZH",
    "ttH",
    "ZZTo4l",
    "ggZZ4e",
    "ggZZ4mu",
    "ggZZ4tau",
    "ggZZ2e2mu",
    "ggZZ2e2tau",
    "ggZZ2mu2tau",
  };

  TFile* inputFile[nDatasets];
  TTree* inputTree[nDatasets];
  TH1F* hCounters[nDatasets];
  Long64_t NGenEvt[nDatasets];
  Double_t gen_sumWeights[nDatasets];
  Float_t partialSampleWeight[nDatasets];

  Int_t nRun;
  Long64_t nEvent;
  Int_t nLumi;
  Short_t Nvtx;
  Short_t NObsInt;
  Float_t NTrueInt;
  Float_t genHEPMCweight;
  Float_t PUWeight;
  Float_t dataMCWeight;
  Float_t overallEventWeight;
  Float_t KFactorggZZ;
  Float_t KFactorEWKqqZZ;
  Float_t KFactorQCDqqZZ_dPhi;
  Float_t KFactorQCDqqZZ_M;
  Float_t KFactorQCDqqZZ_Pt;
  Float_t xsec;
  Short_t ZZsel;
  Float_t ZZMass;
  Float_t ZZPt;
  Short_t Z1Flav;
  Short_t Z2Flav;
  Float_t DiJetFisher;
  Float_t pvbf_VAJHU_old;
  Float_t phjj_VAJHU_old;
  vector<Float_t> *CandLepEta = 0;
  vector<Float_t> *CandLepPhi = 0;
  Short_t nExtraLep;
  vector<Float_t> *ExtraLepEta = 0;
  vector<Float_t> *ExtraLepPhi = 0;
  Short_t nJets;
  Short_t nJetsBTagged;
  vector<Float_t> *JetPt = 0;
  vector<Float_t> *JetEta = 0;
  vector<Float_t> *JetPhi = 0;
  vector<Float_t> *JetMass = 0;
  Float_t jetPt[99];
  Float_t jetEta[99];
  Float_t jetPhi[99];
  Float_t jetMass[99];
  Float_t GenHMass;
  Float_t GenZ1Phi;
  Float_t GenZ2Phi;
  Float_t GenZ1Flav;
  Float_t GenZ2Flav;
  Float_t GenLep1Eta;
  Float_t GenLep1Phi;
  Short_t GenLep1Id;
  Float_t GenLep2Eta;
  Float_t GenLep2Phi;
  Short_t GenLep2Id;
  Float_t GenLep3Eta;
  Float_t GenLep3Phi;
  Short_t GenLep3Id;
  Float_t GenLep4Eta;
  Float_t GenLep4Phi;
  Short_t GenLep4Id;

  TH1F* hYield[nProcesses][nMHPoints][nFinalStates+1][nCategories+1][nResStatuses+1];
  for(int pr=0; pr<nProcesses; pr++)
    for(int mp=0; mp<nMHPoints; mp++)
      if(hasMHPoint[pr][mp])
	for(int fs=0; fs<nFinalStates+1; fs++)
	  for(int cat=0; cat<nCategories+1; cat++)
	    for(int rs=0; rs<nResStatuses+1; rs++){
	      hYield[pr][mp][fs][cat][rs] = new TH1F(Form("hYield_%s%s_%s_%s_%s",sProcess[pr].c_str(),sMHPoint[mp].c_str(),sFinalState[fs].c_str(),sCategory[cat].c_str(),sResonantStatus[rs].c_str()),"",1,0,1);
	      hYield[pr][mp][fs][cat][rs]->Sumw2(true);
	    }
  
  int currentProcess;
  int currentFinalState;
  int currentCategory;
  int currentResStatus;


  //---------- Will loop over all datasets

  for(int d=0; d<nDatasets; d++){

    //----- assign dataset to correct process
    currentProcess = -1;
    if(datasets[d]=="ggH") currentProcess = ggH;
    if(datasets[d]=="VBFH") currentProcess = qqH;
    if(datasets[d]=="WplusH") currentProcess = WH;
    if(datasets[d]=="WminusH") currentProcess = WH;
    if(datasets[d]=="ZH") currentProcess = ZH;
    if(datasets[d]=="ttH") currentProcess = ttH;
    if(datasets[d]=="ZZTo4l"||
       datasets[d]=="ZZTo4lamcatnlo") 
      currentProcess = qqZZ;
    if(datasets[d]=="ggZZ4e"||
       datasets[d]=="ggZZ4mu"||
       datasets[d]=="ggZZ4tau"||
       datasets[d]=="ggZZ2e2mu"||
       datasets[d]=="ggZZ2e2tau"||
       datasets[d]=="ggZZ2mu2tau") 
      currentProcess = ggZZ;
    
    for(int mp=0; mp<nMHPoints; mp++){

      if(!hasMHPoint[currentProcess][mp] || (JUST125&&sMHPoint[mp]!="125"&&sMHPoint[mp]!="")) continue;
      
      string inputFileName = string(Form("%s%s%s/ZZ4lAnalysis.root",(currentProcess==ggZZ)?inputFilePathggZZ.c_str():(currentProcess==qqZZ)?inputFilePathqqZZ.c_str():inputFilePathSignal.c_str(),datasets[d].c_str(),sMHPoint[mp].c_str()));
      inputFile[d] = TFile::Open(inputFileName.c_str());
      
      hCounters[d] = (TH1F*)inputFile[d]->Get("ZZTree/Counters");
      NGenEvt[d] = (Long64_t)hCounters[d]->GetBinContent(1);
      gen_sumWeights[d] = (Long64_t)hCounters[d]->GetBinContent(USEPUWEIGHT?40:41);
      partialSampleWeight[d] = lumi * 1000 / gen_sumWeights[d] ;
      
      inputTree[d] = (TTree*)inputFile[d]->Get("ZZTree/candTree");
      inputTree[d]->SetBranchAddress("RunNumber", &nRun);
      inputTree[d]->SetBranchAddress("EventNumber", &nEvent);
      inputTree[d]->SetBranchAddress("LumiNumber", &nLumi);
      inputTree[d]->SetBranchAddress("Nvtx", &Nvtx);
      inputTree[d]->SetBranchAddress("NObsInt", &NObsInt);
      inputTree[d]->SetBranchAddress("NTrueInt", &NTrueInt);
      inputTree[d]->SetBranchAddress("genHEPMCweight",&genHEPMCweight);
      inputTree[d]->SetBranchAddress("PUWeight",&PUWeight);
      inputTree[d]->SetBranchAddress("dataMCWeight",&dataMCWeight);
      inputTree[d]->SetBranchAddress("overallEventWeight", &overallEventWeight);
      inputTree[d]->SetBranchAddress("KFactorggZZ", &KFactorggZZ);
      inputTree[d]->SetBranchAddress("KFactorEWKqqZZ", &KFactorEWKqqZZ);
      inputTree[d]->SetBranchAddress("KFactorQCDqqZZ_dPhi", &KFactorQCDqqZZ_dPhi);
      inputTree[d]->SetBranchAddress("KFactorQCDqqZZ_M", &KFactorQCDqqZZ_M);
      inputTree[d]->SetBranchAddress("KFactorQCDqqZZ_Pt", &KFactorQCDqqZZ_Pt);
      inputTree[d]->SetBranchAddress("xsec", &xsec);
      inputTree[d]->SetBranchAddress("ZZsel", &ZZsel);
      inputTree[d]->SetBranchAddress("ZZMass", &ZZMass);
      inputTree[d]->SetBranchAddress("ZZPt", &ZZPt);
      inputTree[d]->SetBranchAddress("Z1Flav", &Z1Flav);
      inputTree[d]->SetBranchAddress("Z2Flav", &Z2Flav);
      inputTree[d]->SetBranchAddress("LepEta", &CandLepEta);
      inputTree[d]->SetBranchAddress("LepPhi", &CandLepPhi);
      inputTree[d]->SetBranchAddress("nExtraLep", &nExtraLep);
      inputTree[d]->SetBranchAddress("ExtraLepEta", &ExtraLepEta);
      inputTree[d]->SetBranchAddress("ExtraLepPhi", &ExtraLepPhi);
      inputTree[d]->SetBranchAddress("nCleanedJetsPt30", &nJets);
      inputTree[d]->SetBranchAddress("nCleanedJetsPt30BTagged", &nJetsBTagged);
      inputTree[d]->SetBranchAddress("JetPt", &JetPt);
      inputTree[d]->SetBranchAddress("JetEta", &JetEta);
      inputTree[d]->SetBranchAddress("JetPhi", &JetPhi);
      inputTree[d]->SetBranchAddress("JetMass", &JetMass);
      inputTree[d]->SetBranchAddress("DiJetFisher", &DiJetFisher);
      inputTree[d]->SetBranchAddress("pvbf_VAJHU_old", &pvbf_VAJHU_old);
      inputTree[d]->SetBranchAddress("phjj_VAJHU_old", &phjj_VAJHU_old);
      inputTree[d]->SetBranchAddress("GenHMass", &GenHMass);
      inputTree[d]->SetBranchAddress("GenZ1Phi", &GenZ1Phi);
      inputTree[d]->SetBranchAddress("GenZ2Phi", &GenZ2Phi);
      inputTree[d]->SetBranchAddress("GenZ1Flav", &GenZ1Flav);
      inputTree[d]->SetBranchAddress("GenZ2Flav", &GenZ2Flav);
      inputTree[d]->SetBranchAddress("GenLep1Eta", &GenLep1Eta);
      inputTree[d]->SetBranchAddress("GenLep1Phi", &GenLep1Phi);
      inputTree[d]->SetBranchAddress("GenLep1Id", &GenLep1Id);
      inputTree[d]->SetBranchAddress("GenLep2Eta", &GenLep2Eta);
      inputTree[d]->SetBranchAddress("GenLep2Phi", &GenLep2Phi);
      inputTree[d]->SetBranchAddress("GenLep2Id", &GenLep2Id);
      inputTree[d]->SetBranchAddress("GenLep3Eta", &GenLep3Eta);
      inputTree[d]->SetBranchAddress("GenLep3Phi", &GenLep3Phi);
      inputTree[d]->SetBranchAddress("GenLep3Id", &GenLep3Id);
      inputTree[d]->SetBranchAddress("GenLep4Eta", &GenLep4Eta);
      inputTree[d]->SetBranchAddress("GenLep4Phi", &GenLep4Phi);
      inputTree[d]->SetBranchAddress("GenLep4Id", &GenLep4Id);


      //---------- Process tree

      Long64_t entries = inputTree[d]->GetEntries();
      cout<<"Processing dataset "<<datasets[d]<<sMHPoint[mp]<<" ("<<entries<<" entries) ..."<<endl;

      for (Long64_t z=0; z<entries; ++z){

	if(DEBUG && z>1000) break;

	inputTree[d]->GetEntry(z);
      
	if( !(ZZsel>=90) ) continue;
	if(ZZMass<m4lMin || ZZMass>m4lMax) continue;

	Float_t kfactor = 1.;
	if(APPLYKFACTORS){
	
	  if(currentProcess==qqZZ){
	  
	    //kfactor = 1.065;
	  
	    //kfactor = (GenZ1Flav==GenZ2Flav) ? 1.09 : 1.11 ;
	  
	    //kfactor = 1.1; // Jamboree
	  
	    kfactor = KFactorEWKqqZZ * KFactorQCDqqZZ_M ; // Moriond2016 

	  }else if(currentProcess==ggZZ){
	  
	    //kfactor = 2.;
	  
	    //kfactor = 1.7; // Jamboree
	  
	    kfactor = (float)sp->Eval(GenHMass); // Moriond2016
	    //kfactor = KFactorggZZ; // Moriond2016

	  }

	}

	Double_t eventWeight = partialSampleWeight[d] * xsec * kfactor * (USEPUWEIGHT ? overallEventWeight : genHEPMCweight*dataMCWeight) ;


	//----- find final state

	currentFinalState = -1;
	if(Z1Flav==-121){
	  if(Z2Flav==-121)
	    currentFinalState = fs4e;
	  else if(Z2Flav==-169)
	    currentFinalState = fs2e2mu;
	  else
	    cerr<<"error in event "<<nRun<<":"<<nLumi<<":"<<nEvent<<", Z2Flav="<<Z2Flav<<endl;
	}else if(Z1Flav==-169){
	  if(Z2Flav==-121)
	    currentFinalState = fs2mu2e;
	  else if(Z2Flav==-169)
	    currentFinalState = fs4mu;
	  else
	    cerr<<"error in event "<<nRun<<":"<<nLumi<<":"<<nEvent<<", Z2Flav="<<Z2Flav<<endl;
	}else{
	  cerr<<"error in event "<<nRun<<":"<<nLumi<<":"<<nEvent<<", Z1Flav="<<Z1Flav<<endl;
	}
	if(MERGE2E2MU && currentFinalState==fs2mu2e) currentFinalState = fs2e2mu;


	//----- find category

	for(int j=0; j<nJets; j++){
	  jetPt[j] = JetPt->at(j);
	  jetEta[j] = JetEta->at(j);
	  jetPhi[j] = JetPhi->at(j);
	  jetMass[j] = JetMass->at(j);
	}

	/* ---------- full RunII categorization 
	currentCategory = category(
	   nExtraLep,
	   ZZPt,
	   ZZMass,
	   nJets, 
	   nJetsBTagged,
	   jetPt,
	   jetEta,
	   jetPhi,
	   jetMass,
	   DiJetFisher
	   );
	//*/
	//* ---------- Moriond 2016 categorization 
	currentCategory = categoryMor16(
	   nJets,
           pvbf_VAJHU_old,
           phjj_VAJHU_old
           );
	//*/


	//----- check if this is resonant signal (ie. H->4l with correct lepton choice)

	Short_t GenHLepId[4] = {GenLep1Id,GenLep2Id,GenLep3Id,GenLep4Id};
	Float_t GenHLepEta[4] = {GenLep1Eta,GenLep2Eta,GenLep3Eta,GenLep4Eta};
	Float_t GenHLepPhi[4] = {GenLep1Phi,GenLep2Phi,GenLep3Phi,GenLep4Phi};

	Int_t nGenHLep = 0;
	Int_t nRecoLepMatchedToGenHLep[4] = {0,0,0,0};
	Int_t nGenHLepMatchedToCandLep[4] = {0,0,0,0};
	for(Int_t iGenHLep=0; iGenHLep<4; iGenHLep++){
	  if(abs(GenHLepId[iGenHLep])==11 || abs(GenHLepId[iGenHLep])==13){
	    nGenHLep++;
	    for(Int_t iCandLep=0; iCandLep<4; iCandLep++){
	      if(deltaR(GenHLepEta[iGenHLep],GenHLepPhi[iGenHLep],CandLepEta->at(iCandLep),CandLepPhi->at(iCandLep)) < 0.1){
		nRecoLepMatchedToGenHLep[iGenHLep]++;
		nGenHLepMatchedToCandLep[iCandLep]++;
	      }
	    }
	    for(Int_t iExtraLep=0; iExtraLep<nExtraLep; iExtraLep++){
	      if(deltaR(GenHLepEta[iGenHLep],GenHLepPhi[iGenHLep],ExtraLepEta->at(iExtraLep),ExtraLepPhi->at(iExtraLep)) < 0.1){
		nRecoLepMatchedToGenHLep[iGenHLep]++;
	      }
	    }
	  }
	}
	Bool_t foundMatchingAmbiguity = false;
	for(Int_t iGenHLep=0; iGenHLep<4; iGenHLep++) if(nRecoLepMatchedToGenHLep[iGenHLep]>1){ foundMatchingAmbiguity = true; break; }
	for(Int_t iCandLep=0; iCandLep<4; iCandLep++) if(nGenHLepMatchedToCandLep[iCandLep]>1){ foundMatchingAmbiguity = true; break; }
	Int_t nMatches = 0;
	for(Int_t iGenHLep=0; iGenHLep<4; iGenHLep++) if(nRecoLepMatchedToGenHLep[iGenHLep]==1) nMatches++;

	if(nGenHLep==4 && !foundMatchingAmbiguity && nMatches==4) 
	  currentResStatus = resonant;
	else 
	  currentResStatus = nonresonant;


	//----- fill yield histogram

	hYield[currentProcess][mp][currentFinalState][currentCategory][currentResStatus]->Fill(0.5,eventWeight);


      } // end for entries

    } // end for mHPoints

  } // end for datasets


  //---------- Fill 'inclusive' counters
  for(int pr=0; pr<nProcesses; pr++)
    for(int mp=0; mp<nMHPoints; mp++)
      if(hasMHPoint[pr][mp])
	for(int fs=0; fs<nFinalStates; fs++)
	  for(int cat=0; cat<nCategories; cat++)
	    for(int rs=0; rs<nResStatuses; rs++)
	      hYield[pr][mp][nFinalStates][cat][rs]->Add(hYield[pr][mp][fs][cat][rs]);
  for(int pr=0; pr<nProcesses; pr++)
    for(int mp=0; mp<nMHPoints; mp++)
      if(hasMHPoint[pr][mp])
	for(int fs=0; fs<nFinalStates+1; fs++)
	  for(int cat=0; cat<nCategories; cat++)
	    for(int rs=0; rs<nResStatuses; rs++)
	      hYield[pr][mp][fs][nCategories][rs]->Add(hYield[pr][mp][fs][cat][rs]);
  for(int pr=0; pr<nProcesses; pr++)
    for(int mp=0; mp<nMHPoints; mp++)
      if(hasMHPoint[pr][mp])
	for(int fs=0; fs<nFinalStates+1; fs++)
	  for(int cat=0; cat<nCategories+1; cat++)
	    for(int rs=0; rs<nResStatuses; rs++)
	      hYield[pr][mp][fs][cat][nResStatuses]->Add(hYield[pr][mp][fs][cat][rs]);
  
  TH1F* hYieldSignal125[nFinalStates+1][nCategories+1][nResStatuses+1];
  for(int fs=0; fs<nFinalStates+1; fs++)
    for(int cat=0; cat<nCategories+1; cat++)
      for(int rs=0; rs<nResStatuses+1; rs++){
	hYieldSignal125[fs][cat][rs] = new TH1F(Form("hYieldSignal125_%s_%s_%s",sFinalState[fs].c_str(),sCategory[cat].c_str(),sResonantStatus[rs].c_str()),"",1,0,1);
	for(int pr=0; pr<nProcesses; pr++)
	  if(isSignal[pr])
	    hYieldSignal125[fs][cat][rs]->Add(hYield[pr][indexOf125][fs][cat][rs]);
      }

  //---------- Write yield arrays to a ROOT file

  TFile* fOutYields = new TFile(Form("yields_%iTeV_m4l%.1f-%.1f_%.3ffb-1%s.root",(int)sqrts,m4lMin,m4lMax,lumi,(MERGE2E2MU?"_m":"")),"recreate");
  fOutYields->cd();
  for(int pr=0; pr<nProcesses; pr++){
    for(int mp=0; mp<nMHPoints; mp++){
      if(!hasMHPoint[pr][mp] || (JUST125&&sMHPoint[mp]!="125"&&sMHPoint[mp]!="")) continue;
      for(int fs=0; fs<nFinalStates+1; fs++){
	if(MERGE2E2MU && fs==fs2mu2e) continue;
	for(int cat=0; cat<nCategories+1; cat++){
	  for(int rs=0; rs<nResStatuses+1; rs++){
	    hYield[pr][mp][fs][cat][rs]->Write(hYield[pr][mp][fs][cat][rs]->GetName());
	    delete hYield[pr][mp][fs][cat][rs];
	  }
	}
      }
    }
  }
  for(int fs=0; fs<nFinalStates+1; fs++){
    if(MERGE2E2MU && fs==fs2mu2e) continue;
    for(int cat=0; cat<nCategories+1; cat++){
      for(int rs=0; rs<nResStatuses+1; rs++){
	hYieldSignal125[fs][cat][rs]->Write(hYieldSignal125[fs][cat][rs]->GetName());
	delete hYieldSignal125[fs][cat][rs];
      }
    }
  }
  fOutYields->Close();
  delete fOutYields; 

}


void getZPlusXYields_Run2CombinedShape(Float_t* yieldZPX, Int_t m4lMin, Int_t m4lMax) {

  //----- take the Z+X shapes sent by Pedja on March 1st

  TF1 *f4eComb = new TF1("f4eComb", "landau(0)*(1 + exp( pol1(3))) + [5]*(TMath::Landau(x, [6], [7]))", 70, 1000);
  TF1 *f4muComb = new TF1("f4muComb","landau(0)",70,1000);
  TF1 *f2e2muComb = new TF1("f2e2muComb","landau(0)",70,1000);

  f4eComb->SetParameters(4.404e-05,151.2,36.6,7.06,-0.00497,0.01446,157.3,26.00);
  f4muComb->SetParameters(0.04276,134.6,24.4);
  f2e2muComb->SetParameters(0.04130,144.5,25.3);

  //----- take the normalization sent by Pedja on March 1st

  Float_t normFullRange4e    = 2.2;
  Float_t normFullRange4mu   = 2.1;
  Float_t normFullRange2e2mu = 3.2;

  //----- compute normalization of the subrange of interest

  yieldZPX[fs4mu  ] = normFullRange4mu   * f4muComb  ->Integral(m4lMin,m4lMax) / f4muComb  ->Integral(70,1000);
  yieldZPX[fs4e   ] = normFullRange4e    * f4eComb   ->Integral(m4lMin,m4lMax) / f4eComb   ->Integral(70,1000);
  yieldZPX[fs2e2mu] = normFullRange2e2mu * f2e2muComb->Integral(m4lMin,m4lMax) / f2e2muComb->Integral(70,1000);
  yieldZPX[nFinalStates] = yieldZPX[fs4mu] + yieldZPX[fs4e] + yieldZPX[fs2e2mu];

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/////////////////////////// fit signal yields vs. mH ///////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void DrawYieldFits(string outputDirectory, TGraphErrors* g[nProcesses][nFinalStates][nCategories+1], TF1* f[nProcesses][nFinalStates][nCategories+1]){

  setTDRStyle();

  string outputPath = string(Form("%s/CanvasesSignalFits/",outputDirectory.c_str()));
  gSystem->Exec(("mkdir -p "+outputPath).c_str());
  TCanvas* cYield[nProcesses][nCategories];
  TLegend* lgd[nProcesses][nCategories];
  bool first;

  for(int pr=0; pr<nProcesses; pr++){
    if(!isSignal[pr]) continue;
    
    for(int cat=0; cat<nCategories; cat++){
      
      string canvasName = string(Form("cFits_%s_%s",sProcess[pr].c_str(),sCategory[cat].c_str()));
      cYield[pr][cat] = new TCanvas(canvasName.c_str(),canvasName.c_str(),500,500);
      lgd[pr][cat] = new TLegend(0.2,0.73,0.4,0.88);
      lgd[pr][cat]->SetFillStyle(0);
      //lgd[pr][cat]->SetBorderSize(0);
      first = true;
      
      for(int fs=0; fs<nFinalStates; fs++){
	if(MERGE2E2MU && fs==fs2mu2e) continue;

	g[pr][fs][cat]->GetXaxis()->SetTitle("generated m_{H}");
	g[pr][fs][cat]->GetYaxis()->SetTitle("expected yield");
	g[pr][fs][cat]->GetXaxis()->SetTitleOffset(1.2);
	g[pr][fs][cat]->GetYaxis()->SetTitleOffset(1.6);
	g[pr][fs][cat]->GetXaxis()->SetLabelFont(42);
	g[pr][fs][cat]->GetYaxis()->SetLabelFont(42);
	g[pr][fs][cat]->GetXaxis()->SetLabelSize(0.04);
	g[pr][fs][cat]->GetYaxis()->SetLabelSize(0.04);
	g[pr][fs][cat]->GetXaxis()->SetTitleFont(42);
	g[pr][fs][cat]->GetYaxis()->SetTitleFont(42);
	g[pr][fs][cat]->GetXaxis()->SetTitleSize(0.04);
	g[pr][fs][cat]->GetYaxis()->SetTitleSize(0.04);
	g[pr][fs][cat]->SetMarkerStyle(fsMarkerStyle[fs]);
	g[pr][fs][cat]->SetMarkerColor(catColor[cat]);
	g[pr][fs][cat]->SetMinimum(0);
	g[pr][fs][cat]->GetXaxis()->SetLimits(fMHPoint[1]-5.,fMHPoint[nMHPoints-1]+5.);

	f[pr][fs][cat]->SetLineColor(catColor[cat]);
	f[pr][fs][cat]->SetLineWidth(1);

	lgd[pr][cat]->AddEntry(g[pr][fs][cat],sFinalState[fs].c_str(),"p");
      }

      for(int fs=nFinalStates-1; fs>=0; fs--){
	if(MERGE2E2MU && fs==fs2mu2e) continue;
	g[pr][fs][cat]->Draw(first?"AP":"P");
	f[pr][fs][cat]->Draw("SAME");
	first = false;
      }

      //lgd[pr][cat]->AddEntry(f[pr][0][cat],sCategory[cat].c_str(),"l");
      lgd[pr][cat]->Draw();

      TPaveText* pav = new TPaveText(0.13,0.93,0.8,1.,"brNDC");
      pav->SetFillStyle(0);
      pav->SetBorderSize(0);
      pav->SetTextAlign(11);
      pav->SetTextSize(0.04);
      pav->AddText((sProcess[pr]+", "+sCategory[cat]).c_str());
      pav->Draw();

      SaveCanvas(outputPath,cYield[pr][cat]);

    }
  }

}

void fitSignalYields(string outputDirectory, double lumi, double sqrts, double m4lMin, double m4lMax)
{

  //---------- Retrieve yields from the ROOT file

  TFile* fInYields = TFile::Open(Form("yields_%iTeV_m4l%.1f-%.1f_%.3ffb-1%s.root",(int)sqrts,m4lMin,m4lMax,lumi,(MERGE2E2MU?"_m":"")));
  TH1F* hTemp;
  Float_t yield[nProcesses][nMHPoints][nFinalStates][nCategories+1];
  Float_t yieldStatError[nProcesses][nMHPoints][nFinalStates][nCategories+1];
  for(int pr=0; pr<nProcesses; pr++){
    if(!isSignal[pr]) continue;
    for(int mp=0; mp<nMHPoints; mp++){
      if(hasMHPoint[pr][mp] && isSignal[pr]){
	for(int fs=0; fs<nFinalStates; fs++){
	  if(MERGE2E2MU && fs==fs2mu2e) continue;
	  for(int cat=0; cat<nCategories+1; cat++){
	    hTemp = (TH1F*)fInYields->Get(Form("hYield_%s%s_%s_%s_%s",sProcess[pr].c_str(),sMHPoint[mp].c_str(),sFinalState[fs].c_str(),sCategory[cat].c_str(),sResonantStatus[nResStatuses].c_str()));
	    yield[pr][mp][fs][cat] = hTemp->GetBinContent(1);
	    yieldStatError[pr][mp][fs][cat] = hTemp->GetBinError(1);//0.;//
	  }
	}
      }
    }
  }

  //---------- Fit yield vs. mH

  TFile* fOutYieldFunctions = new TFile(Form("yieldFunctions_%iTeV_m4l%.1f-%.1f_%.3ffb-1%s.root",(int)sqrts,m4lMin,m4lMax,lumi,(MERGE2E2MU?"_m":"")),"recreate");
  TGraphErrors* gYield[nProcesses][nFinalStates][nCategories+1];
  TF1* fYield[nProcesses][nFinalStates][nCategories+1];

  for(int pr=0; pr<nProcesses; pr++){
    if(!isSignal[pr]) continue;

    for(int fs=0; fs<nFinalStates; fs++){
      if(MERGE2E2MU && fs==fs2mu2e) continue;

      for(int cat=0; cat<nCategories+1; cat++){

	gYield[pr][fs][cat] = new TGraphErrors(nMHPointsProcess[pr]);

	int iPoint = 0;
	for(int mp=0; mp<nMHPoints; mp++){
	  if(hasMHPoint[pr][mp]){
	    gYield[pr][fs][cat]->SetPoint(iPoint,fMHPoint[mp],yield[pr][mp][fs][cat]);
	    gYield[pr][fs][cat]->SetPointError(iPoint,0.,yieldStatError[pr][mp][fs][cat]);
	    iPoint++;
	  }
	}

	string fName = (string)Form("f_%s_%s_%s",sProcess[pr].c_str(),sFinalState[fs].c_str(),sCategory[cat].c_str());
	//int order = ((pr==WH || pr==ZH) && cat==VBFTaggedMor16) ? 1 : orderOfPolynomial[pr] ;
	int order = (pr==ZH && cat==VBFTaggedMor16) ? 1 : orderOfPolynomial[pr] ;
	fYield[pr][fs][cat] = new TF1(fName.c_str(),Form("pol%i",order),fMHPoint[1],fMHPoint[nMHPoints-1]); 
	//fYield[pr][fs][cat] = new TF1(fName.c_str(),"[0]+[1]*x+[2]*x*x",fMHPoint[1],fMHPoint[nMHPoints-1]); // the fit doesn't work with this
	
	cout<<endl<<sProcess[pr]<<", "<<sFinalState[fs]<<", "<<sCategory[cat]<<endl;
	gYield[pr][fs][cat]->Fit(fYield[pr][fs][cat],"N S");

	//cout<<fYield[pr][fs][cat]->GetExpFormula("P")<<endl;
	fYield[pr][fs][cat]->Write();
	
      }
    }
  }

  DrawYieldFits(outputDirectory,gYield,fYield);

  fOutYieldFunctions->Close();
  delete fOutYieldFunctions; 

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////// prepare fragments ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void generateFragments(string outputDirectory, double lumi, double sqrts, double m4lMin, double m4lMax, string mHoption)
{

  //---------- Retrieve yields and functions from the ROOT file

  TFile* fInYields = TFile::Open(Form("yields_%iTeV_m4l%.1f-%.1f_%.3ffb-1%s.root",(int)sqrts,m4lMin,m4lMax,lumi,(MERGE2E2MU?"_m":"")));
  TH1F* hTemp;
  Float_t yield[nProcesses][nFinalStates+1][nCategories+1];
  Float_t yieldStatError[nProcesses][nFinalStates+1][nCategories+1];
  for(int pr=0; pr<nProcesses; pr++){
    for(int fs=0; fs<nFinalStates+1; fs++){
      if(MERGE2E2MU && fs==fs2mu2e) continue;
      for(int cat=0; cat<nCategories+1; cat++){
	hTemp = (TH1F*)fInYields->Get(Form("hYield_%s%s_%s_%s_%s",sProcess[pr].c_str(),(isSignal[pr]?(mHoption!="param"?mHoption.c_str():"125"):""),sFinalState[fs].c_str(),sCategory[cat].c_str(),sResonantStatus[nResStatuses].c_str()));
	yield[pr][fs][cat] = hTemp->GetBinContent(1);
	yieldStatError[pr][fs][cat] = hTemp->GetBinError(1);
      }
    }
  }

  // for Moriond16 categorization, use Heshy's parameterization of Djet efficiency:
  if(USEDJETEFFICIENCYFORGGZZ){
    for(int fs=0; fs<nFinalStates+1; fs++){
      if(MERGE2E2MU && fs==fs2mu2e) continue;
      yield[ggZZ][fs][UntaggedMor16] = (1.-GGZZDJETEFFICIENCY) * yield[ggZZ][fs][nCategories];
      yieldStatError[ggZZ][fs][UntaggedMor16] = (1.-GGZZDJETEFFICIENCY) * yieldStatError[ggZZ][fs][nCategories];
      yield[ggZZ][fs][VBFTaggedMor16] = GGZZDJETEFFICIENCY * yield[ggZZ][fs][nCategories];
      yieldStatError[ggZZ][fs][VBFTaggedMor16] = GGZZDJETEFFICIENCY * yieldStatError[ggZZ][fs][nCategories];
    }
  }

  Float_t yieldSignal125[nFinalStates+1][nCategories+1];
  Float_t yieldSignal125StatError[nFinalStates+1][nCategories+1];
  for(int fs=0; fs<nFinalStates+1; fs++){
    if(MERGE2E2MU && fs==fs2mu2e) continue;
    for(int cat=0; cat<nCategories+1; cat++){
      hTemp = (TH1F*)fInYields->Get(Form("hYieldSignal125_%s_%s_%s",sFinalState[fs].c_str(),sCategory[cat].c_str(),sResonantStatus[nResStatuses].c_str()));
      yieldSignal125[fs][cat] = hTemp->GetBinContent(1);
      yieldSignal125StatError[fs][cat] = hTemp->GetBinError(1);
    }
  }

  TF1* fYield[nProcesses][nFinalStates][nCategories+1];
  if(mHoption=="param"){
    TFile* fInYieldFunctions = TFile::Open(Form("yieldFunctions_%iTeV_m4l%.1f-%.1f_%.3ffb-1%s.root",(int)sqrts,m4lMin,m4lMax,lumi,(MERGE2E2MU?"_m":"")));
    for(int pr=0; pr<nProcesses; pr++){
      if(!isSignal[pr]) continue;
      for(int fs=0; fs<nFinalStates; fs++){
	if(MERGE2E2MU && fs==fs2mu2e) continue;
	for(int cat=0; cat<nCategories+1; cat++)
	  fYield[pr][fs][cat] = (TF1*)fInYieldFunctions->Get(Form("f_%s_%s_%s",sProcess[pr].c_str(),sFinalState[fs].c_str(),sCategory[cat].c_str()));	  
      }
    }
  }

  //Z+X yield for Moriond16
  Float_t yieldZPlusX[nFinalStates+1][nCategories+1];
  if(DOZPLUSXFROMRUN2COMBINEDSHAPE && MERGE2E2MU){
    Float_t yieldZPX[nFinalStates+1];
    getZPlusXYields_Run2CombinedShape(yieldZPX,m4lMin,m4lMax);
    for(int fs=0; fs<nFinalStates+1; fs++){
      if(MERGE2E2MU && fs==fs2mu2e) continue;
      yieldZPlusX[fs][nCategories] = yieldZPX[fs];
      yieldZPlusX[fs][UntaggedMor16] = (1.-ZPLUSXDJETEFFICIENCY) * yieldZPX[fs];
      yieldZPlusX[fs][VBFTaggedMor16] = ZPLUSXDJETEFFICIENCY * yieldZPX[fs];
    }
  }


  //---------- Prepare the yaml fragments
  float totalYieldSgnl[nFinalStates+1];
  float totalYieldBkgd[nFinalStates+1];
  float totalYield[nFinalStates+1];
  totalYieldSgnl[nFinalStates] = 0.;
  totalYieldBkgd[nFinalStates] = 0.;
  totalYield[nFinalStates] = 0.;

  string outputFileName[nFinalStates];
  ofstream outFile[nFinalStates];
  for(int fs=0; fs<nFinalStates; fs++){
    if(MERGE2E2MU && fs==fs2mu2e) continue;
    
    outputFileName[fs] = string(Form("%s/yields_per_tag_category_%iTeV_%s.yaml",outputDirectory.c_str(),(int)sqrts,sFinalState[fs].c_str())); // (not specifying MH and mass window in the name here)
    outFile[fs].open(outputFileName[fs]);
    
    outFile[fs]<<"---"<<endl;
    outFile[fs]<<"# sqrt(s) = "<<sqrts<<" TeV"<<endl;
    outFile[fs]<<"# integrated luminosity = "<<lumi<<" fb-1"<<endl;
    outFile[fs]<<endl;
    outFile[fs]<<"mass_range: '"<<m4lMin<<", "<<m4lMax<<"'"<<endl;
    outFile[fs]<<"kd_range: '0, 1'"<<endl;
    outFile[fs]<<endl;

    // outFile[fs]<<"# Category numbering convention:"<<endl;
    // for(int cat=0; cat<nCategories; cat++){
    //   outFile[fs]<<"# "<<cat<<" "<<sCategory[cat]<<endl;
    // }
    // outFile[fs]<<endl;

    totalYieldBkgd[fs] = 0.;
    totalYieldSgnl[fs] = 0.;
    totalYield[fs] = 0.;

    for(int cat=0; cat<nCategories; cat++){
      outFile[fs]<<sCategory[cat]<<": "<<endl;

      for(int pr=0; pr<nProcesses; pr++){

	if(isSignal[pr] && mHoption=="param"){
	  outFile[fs]<<"    "<<sProcess[pr]<<": ";
	  for(int ord=0; ord<=orderOfPolynomial[pr]; ord++){
	    outFile[fs]<<"("<<fYield[pr][fs][cat]->GetParameter(ord);
	    for(int ord2=0; ord2<=ord-1; ord2++) outFile[fs]<<"*@0";
	    outFile[fs]<<")";
	    if(ord<orderOfPolynomial[pr]) outFile[fs]<<"+";
	  }
	  outFile[fs]<<endl;
	}else{
	  outFile[fs]<<"    "<<sProcess[pr]<<": '"<<yield[pr][fs][cat]<<"'"<<endl;
	}

	if(isSignal[pr]) totalYieldSgnl[fs] += yield[pr][fs][cat];
	else totalYieldBkgd[fs] += yield[pr][fs][cat];
	totalYield[fs] += yield[pr][fs][cat];
      }

      if(DOZPLUSXFROMRUN2COMBINEDSHAPE && MERGE2E2MU){
	outFile[fs]<<"    zjets: '"<<yieldZPlusX[fs][cat]<<"'"<<endl;
	totalYieldBkgd[fs] += yieldZPlusX[fs][cat];
	totalYield[fs] += yieldZPlusX[fs][cat];
      }

      outFile[fs]<<endl;
    }

    totalYieldBkgd[nFinalStates] += totalYieldBkgd[fs];
    totalYieldSgnl[nFinalStates] += totalYieldSgnl[fs];
    totalYield[nFinalStates] += totalYield[fs];

    outFile[fs].close();

  }


  //---------- Print the yields

  cout<<"In mass window ["<<m4lMin<<","<<m4lMax<<"] : "<<endl;

  cout<<" "<<"Total signal @ 125 GeV"<<":        "<<totalYieldSgnl[nFinalStates]<<endl;
  cout<<" "<<"Total signal @ 125 GeV"<<", 4e   : "<<totalYieldSgnl[fs4e        ]<<endl;
  cout<<" "<<"Total signal @ 125 GeV"<<", 4mu  : "<<totalYieldSgnl[fs4mu       ]<<endl;
  cout<<" "<<"Total signal @ 125 GeV"<<", 2e2mu: "<<totalYieldSgnl[fs2e2mu     ]<<endl;
  if(!MERGE2E2MU)
    cout<<" "<<"Total signal @ 125 GeV"<<", 2mu2e: "<<totalYieldSgnl[fs2mu2e     ]<<endl;

  cout<<" "<<"Total background"<<":        "<<totalYieldBkgd[nFinalStates]<<endl;
  cout<<" "<<"Total background"<<", 4e   : "<<totalYieldBkgd[fs4e        ]<<endl;
  cout<<" "<<"Total background"<<", 4mu  : "<<totalYieldBkgd[fs4mu       ]<<endl;
  cout<<" "<<"Total background"<<", 2e2mu: "<<totalYieldBkgd[fs2e2mu     ]<<endl;
  if(!MERGE2E2MU)
    cout<<" "<<"Total background"<<", 2mu2e: "<<totalYieldBkgd[fs2mu2e     ]<<endl;

  cout<<" "<<"Total signal+background"<<":        "<<totalYield[nFinalStates]<<endl;
  cout<<" "<<"Total signal+background"<<", 4e   : "<<totalYield[fs4e        ]<<endl;
  cout<<" "<<"Total signal+background"<<", 4mu  : "<<totalYield[fs4mu       ]<<endl;
  cout<<" "<<"Total signal+background"<<", 2e2mu: "<<totalYield[fs2e2mu     ]<<endl;
  if(!MERGE2E2MU)
    cout<<" "<<"Total signal+background"<<", 2mu2e: "<<totalYield[fs2mu2e     ]<<endl;

  for(int pr=0; pr<nProcesses; pr++){
    cout<<"  "<<sProcess[pr]<<":        "<<yield[pr][nFinalStates][nCategories]<<endl;
    cout<<"  "<<sProcess[pr]<<", 4e   : "<<yield[pr][fs4e        ][nCategories]<<endl;
    cout<<"  "<<sProcess[pr]<<", 4mu  : "<<yield[pr][fs4mu       ][nCategories]<<endl;
    cout<<"  "<<sProcess[pr]<<", 2e2mu: "<<yield[pr][fs2e2mu     ][nCategories]<<endl;
    if(!MERGE2E2MU)
      cout<<"  "<<sProcess[pr]<<", 2mu2e: "<<yield[pr][fs2mu2e     ][nCategories]<<endl;
  }
  if(DOZPLUSXFROMRUN2COMBINEDSHAPE && MERGE2E2MU){
    cout<<"  Z+X:        "<<yieldZPlusX[nFinalStates][nCategories]<<endl;
    cout<<"  Z+X, 4e   : "<<yieldZPlusX[fs4e        ][nCategories]<<endl;
    cout<<"  Z+X, 4mu  : "<<yieldZPlusX[fs4mu       ][nCategories]<<endl;
    cout<<"  Z+X, 2e2mu: "<<yieldZPlusX[fs2e2mu     ][nCategories]<<endl;
  }

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// main function ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void prepareYields(bool recomputeYields = true) {


  // --------------- definitions ---------------

  // Define input/output location
  string inputPathSignal = "";
  string inputPathqqZZ = "";
  string inputPathggZZ = "";
  string outputPath = "YieldFiles";

  // Define c.o.m. energy (13TeV only for the moment)
  float sqrts = 13.;

  // Define the luminosity
  float lumi = 2.762;

  // m4l window
  /*
  float m4l_min = 105.;
  float m4l_max = 140.;
  //*/
  /*
  float m4l_min = 70.;
  float m4l_max = 886.;
  //*/
  //*
  float m4l_min = 70.;
  float m4l_max = 3000.;
  //*/
  /*
  float m4l_min = 118.;
  float m4l_max = 130.;
  //*/
  /*
  float m4l_min = 110.;
  float m4l_max = 150.;
  //*/


  // --------------- processing ---------------

  gSystem->Exec(("mkdir -p "+outputPath).c_str());

  // Compute the yields for all available processes and mH values, and store them in a ROOT file 
  // (to be done only once, it can take a few minutes)
  if(recomputeYields)
    computeYields(inputPathSignal, inputPathqqZZ, inputPathggZZ, lumi, sqrts, m4l_min, m4l_max);

  // Parameterize signal yields as a function of mH
  if(!JUST125) 
    fitSignalYields(outputPath, lumi, sqrts, m4l_min, m4l_max);

  // Prepare the yaml files
  if(JUST125)
    generateFragments(outputPath, lumi, sqrts, m4l_min, m4l_max, "125"); // This takes mH=125 for the signal yield.
  else
    generateFragments(outputPath, lumi, sqrts, m4l_min, m4l_max, "param"); // This puts the yield(mH) expression when possible.


}

