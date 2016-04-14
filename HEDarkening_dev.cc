#include "DataFormats/HcalCalibObjects/interface/HEDarkening_dev.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
//#define DebugLog
HEDarkening_dev::HEDarkening_dev() {
  ieta_shift = 16;

  ifstream in;
  //Hardcode the filename for the time being.
  in.open("fluka_Mrad_fbinv.txt");
  for(unsigned int iR=0;iR<Nieta;iR++){
    for(unsigned int iC=0;iC<Nlayer;iC++){
      in >> doseMap[iR][iC];
      if (!in.good()) break;
    }
  }
  in.close();
 
}

HEDarkening_dev::~HEDarkening_dev() {}

double HEDarkening_dev::getLumi(int year){
  int yIndex = getYearIndex(year);
  return lumi[yIndex];
}

double HEDarkening_dev::getLumiRate(int year){
  int yIndex = getYearIndex(year);
  return doserate[yIndex];
}

double HEDarkening_dev::getFlukaDose(int ieta, int ilayer){
   double dose = doseMap[ieta][ilayer];
   if (dose==0)return 1.;
   else return dose;
}


double HEDarkening_dev::standAloneResponse(int year, int ieta, int layer){
  double lumi = getLumi(year);
  double lumiRate = getLumiRate(year);
  double dose  = getFlukaDose(ieta,layer);
  //Calculate decay constant
  double decayConst = aConst*std::pow((1000*dose*lumiRate),bConst);
  //Calculate accumulated dose
  double accumDose = dose*lumi;
  //cout<< "I m in SAR for year: "<<year<<", lumi: "<< lumi <<", dose: "<<dose<<", accumDose: "<<accumDose<<endl;

  //Calculate response
  double response = exp(-(accumDose/decayConst));

  //return the value
  return response;
}

double HEDarkening_dev::degradation(int firstYear, int currYear, int ieta, 
				int layer){
  std::cout<<currYear<<std::endl;
  if (currYear<2011 || currYear == 2013 || currYear ==14 || currYear == 2019 || currYear ==20 || currYear > 2023 )return 1; //No darkening
  //shift ieta tower index
  ieta -= ieta_shift;
  //if outside eta range, no darkening
  if(ieta < 0 || ieta >= (int)Nieta) {cout<<"tower # "<<ieta<<" > "<<Nieta<<" known? "<<std::endl;return 1.;}
  if(layer < 0 || layer >= (int)Nlayer){cout<<"layer # "<<layer<<" > "<<Nlayer<<" known? "<<std::endl; return 1.;}
  
  //  if (lumiscale[ieta][lay] == 0) return 1.;
  //Calculate darkening factor: accumulated over years.
  double response = 1;
  for (int yr = firstYear; yr<currYear+1; yr++){
    //std::cout<<"response: "<<response<<std::endl;
    response = response*standAloneResponse(yr,ieta,layer);
  }
  
  cout<< "I m in degradation for year: "<< currYear <<", tower # "<<ieta+ieta_shift<<", layer: "<<layer<<", resp: "<<response<<endl;
  cout<<"degradation: .... "<<endl;
  return response;
  //return (exp(-intlumi/lumiscale[ieta][lay]));
}

int HEDarkening_dev::getYearIndex(const int y) {
  int i = 0;
  while (1){
    if (year[i]==y)return i;
    else if (i==8)return -1;
    else i++;
  }
}

int main(){

  HEDarkening_dev h;
  // for (int t=29; t<30; t++){
  //  for (int i=0; i<18; i++){
      std::cout<< h.degradation(2011,2023,29,0)<<std::endl;
      // }
      // }
}
