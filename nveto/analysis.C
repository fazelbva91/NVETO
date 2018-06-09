
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TF1.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TGaxis.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "string.h"
#include <sstream>

using namespace std;

gROOT->Reset();

void analysis()
{	
	//----------------------------------input--------------------------------  	  	 	 	
  	//open a simulation result file 
    TFile* file = TFile::Open("output.root");
    
    Double_t NbinE = 200;
    Double_t Emin = 0.; 	
    Double_t Emax = 2.;	//MeV
    
	Int_t particle = 0; //0->gamma, 1->electron, 2->proton
    //-----------------------------------------------------------------------
    
    //particle cut
    Char_t partCut[128];
    sprintf(partCut,"part == %d", particle);
    
    Char_t title[128];
    sprintf(title,"Energy Deposition in LAB from ");
    switch (particle) {
    	case 0:
    		strcat(title, "gammas");
    		break;
    	case 1:
    		strcat(title, "electrons");
    		break;
    	case 2:
    		strcat(title, "protons");
    		break;
    }
    
    //Energy Deposition 	
  	TH1D* EnergyDep = new TH1D("EnergyDep", title, NbinE, Emin, Emax);
  	
  	nVeto->Project("EnergyDep", "Edep", partCut); 		 
  	
  	EnergyDep->SetLineColor(kRed); 
  	EnergyDep->SetLineWidth(2);
  	EnergyDep->SetXTitle("E (MeV)");
  	EnergyDep->GetXaxis()->CenterTitle();
  	EnergyDep->GetXaxis()->SetTitleOffset(1.1); 
  	EnergyDep->SetYTitle("Counts");
  	EnergyDep->GetYaxis()->CenterTitle();
  	EnergyDep->GetYaxis()->SetTitleOffset(1.6); 
  	TCanvas* c1 = new TCanvas("c1","",1000,1100); 	
  	c1->cd(); 						
  	gPad->SetLogy();				
  	EnergyDep->Draw();
   	
}
