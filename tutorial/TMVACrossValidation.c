#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TMVA/DataLoader.h"
#include "TMVA/CrossValidation.h"
#include "TMVA/Tools.h"

void TMVACrossValidation()
{
   // This loads the library
   TMVA::Tools::Instance();
   // Load data
   TString fname = "./tmva_class_example.root";
   if (gSystem->AccessPathName(fname))
      gSystem->Exec("curl -O http://root.cern.ch/files/tmva_class_example.root");
   TFile *input = TFile::Open(fname);
   TTree* signalTree = (TTree*)input->Get("TreeS");
   TTree* background = (TTree*)input->Get("TreeB");
   // Setup dataloader
   TMVA::DataLoader* dataloader = new TMVA::DataLoader("dataset");
   dataloader->AddSignalTree(signalTree);
   dataloader->AddBackgroundTree(background);
   dataloader->AddVariable("var1");
   dataloader->AddVariable("var2");
   dataloader->AddVariable("var3");
   dataloader->AddVariable("var4");
   dataloader->PrepareTrainingAndTestTree("", "SplitMode=Random:NormMode=NumEvents:!V");
   // Setup cross-validation with Fisher method
   TMVA::CrossValidation cv(dataloader);
   cv.BookMethod(TMVA::Types::kFisher, "Fisher", "!H:!V:Fisher");
   // Run cross-validation and print results
   cv.Evaluate();
   TMVA::CrossValidationResult results = cv.GetResults();
   results.Print();
}
int main()
{
   TMVACrossValidation();
}
