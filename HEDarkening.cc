#include "DataFormats/HcalCalibObjects/interface/HEDarkening.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
//#define DebugLog

HEDarkening::HEDarkening(unsigned int scenario) :
  year {2011,2012,2015,2016,2017, 2018, 2021, 2022, 2023},
  ilumi   {5.6, 28.9,33.0,63.0,128.0,193.0,293.0,393.0,493.0},
  lumi {5.6, 23.3,4.1, 30,  65,  65,  100, 100, 100},
  doserate {0.005,0.013,0.009,0.026,0.043,0.043,0.05,0.05,0.05}

{
  ieta_shift = 16;
  
  aConst = 2.4;                                                                                                                                                     
  bConst = 0.6;                                                                                                                                                     
  
  //Hardcode the filename for the time being.
  const double _doseMap [Nieta][Nlayer] = {
    {2.500000E+00, 0.000000E+00, 0.000000E+00, 3.116667E-06, 3.133333E-06, 2.050000E-06, 9.166667E-07, 4.000000E-07, 4.000000E-07, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00},
    {2.075000E-05, 2.805000E-05, 1.775000E-05, 2.068333E-05, 7.566667E-06, 5.883333E-06, 3.216667E-06, 1.883333E-06, 6.000000E-07, 4.333333E-07, 3.333333E-07, 2.000000E-07, 3.333333E-08, 1.333333E-07, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00},
    {9.990000E-05, 6.360000E-05, 4.546667E-05, 2.083333E-05, 1.463333E-05, 1.200000E-05, 7.750000E-06, 5.366667E-06, 4.466667E-06, 1.400000E-06, 8.500000E-07, 1.666667E-07, 2.166667E-07, 1.833333E-07, 6.666667E-08, 1.666667E-08, 0.000000E+00, 0.000000E+00},
    {1.857667E-04, 1.059833E-04, 6.970000E-05, 5.113333E-05, 2.800000E-05, 1.875000E-05, 1.120000E-05, 8.316667E-06, 6.900000E-06, 2.183333E-06, 2.766667E-06, 1.033333E-06, 8.833333E-07, 1.150000E-06, 2.000000E-07, 1.666667E-08, 2.000000E-07, 1.666667E-07},
    {2.759833E-04, 1.728167E-04, 1.175500E-04, 8.326667E-05, 4.255000E-05, 3.216667E-05, 2.383333E-05, 1.541667E-05, 8.483333E-06, 4.950000E-06, 2.350000E-06, 1.783333E-06, 1.050000E-06, 8.333333E-07, 1.050000E-06, 1.833333E-07, 1.333333E-07, 1.333333E-07},
    {3.588167E-04, 2.211167E-04, 1.559667E-04, 1.053500E-04, 9.741667E-05, 5.316667E-05, 2.840000E-05, 1.745000E-05, 1.195000E-05, 1.366667E-05, 7.483333E-06, 2.966667E-06, 2.066667E-06, 2.350000E-06, 7.166667E-07, 8.333333E-07, 3.500000E-07, 1.250000E-06},
    {4.883833E-04, 3.191333E-04, 2.472167E-04, 1.547667E-04, 1.066167E-04, 7.736667E-05, 4.993333E-05, 2.571667E-05, 1.958333E-05, 1.820000E-05, 1.316667E-05, 6.466667E-06, 5.183333E-06, 2.500000E-06, 2.250000E-06, 2.316667E-06, 5.000000E-07, 9.000000E-07},
    {6.569000E-04, 4.815000E-04, 3.872333E-04, 2.520833E-04, 1.690833E-04, 1.121000E-04, 6.795000E-05, 5.610000E-05, 3.575000E-05, 2.610000E-05, 1.266667E-05, 7.083333E-06, 7.483333E-06, 5.183333E-06, 3.016667E-06, 1.166667E-06, 2.183333E-06, 1.116667E-06},
    {9.552500E-04, 7.336000E-04, 5.263833E-04, 3.781167E-04, 2.509500E-04, 1.474167E-04, 1.114000E-04, 7.310000E-05, 7.055000E-05, 3.200000E-05, 2.676667E-05, 1.811667E-05, 1.670000E-05, 1.136667E-05, 8.666667E-06, 3.700000E-06, 3.550000E-06, 1.583333E-06},
    {1.409033E-03, 1.122883E-03, 9.155667E-04, 6.086500E-04, 4.302833E-04, 2.925667E-04, 1.871167E-04, 1.292667E-04, 8.776667E-05, 7.793333E-05, 4.665000E-05, 2.490000E-05, 2.615000E-05, 1.446667E-05, 7.766667E-06, 7.283333E-06, 9.183333E-06, 6.883333E-06},
    {2.163600E-03, 1.866083E-03, 1.303450E-03, 1.016433E-03, 7.006000E-04, 5.508667E-04, 3.320833E-04, 2.387500E-04, 1.766667E-04, 1.311833E-04, 1.014167E-04, 5.851667E-05, 4.396667E-05, 3.153333E-05, 1.618333E-05, 2.053333E-05, 1.295000E-05, 1.780000E-05},
    {3.098283E-03, 2.984033E-03, 2.155650E-03, 1.645467E-03, 1.141617E-03, 8.015667E-04, 5.699167E-04, 4.089667E-04, 3.060500E-04, 2.327667E-04, 1.609000E-04, 1.076667E-04, 7.761667E-05, 7.725000E-05, 8.660000E-05, 5.126667E-05, 4.308333E-05, 3.891667E-05},
    {4.599117E-03, 4.781817E-03, 3.099200E-03, 2.252433E-03, 1.714900E-03, 2.053367E-03, 1.671167E-03, 1.459333E-03, 1.238833E-03, 1.041000E-03, 8.670167E-04, 8.372667E-04, 7.145667E-04, 5.851333E-04, 5.941833E-04, 5.394167E-04, 5.035667E-04, 4.553667E-04},
    {1.082867E-02, 1.072262E-02, 6.020917E-03, 4.557333E-03, 3.753517E-03, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}
    
  };

  for(unsigned int iR=0;iR<Nieta;iR++){
    for(unsigned int iC=0;iC<Nlayer;iC++){
         doseMap[iR][iC] = _doseMap[iR][iC];
    }
  }
 
}

HEDarkening::~HEDarkening() {}

double HEDarkening::getLumi(int year){
  int yIndex = getYearIndex(year);
  return lumi[yIndex];
}

double HEDarkening::getLumiRate(int year){
  int yIndex = getYearIndex(year);
  return doserate[yIndex];
}

double HEDarkening::getFlukaDose(int ieta, int ilayer){
   double dose = doseMap[ieta][ilayer];
   if (dose==0)return 1.;
   else return dose;
}


double HEDarkening::standAloneResponse(int year, int ieta, int layer){
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

int HEDarkening::getYearsForLumi(double iL){
  //  int index=-999;
  int past = 0;
  for(int i = 0; i!=9; i++){
    if (iL>= past && iL<ilumi[i]){
      float left = iL-past;
      float right = ilumi[i]-iL;
      if (left<right) return year[i-1];
      else return year[i];
   }
    else {
      past = ilumi[i];
    }
  }
 
    return year[8];
 
}


double HEDarkening::degradation(double iLumi, int iEta, int iLayer){
  int firstYear = 2011;
  int currYear = getYearsForLumi(iLumi);
  return degradation(firstYear,currYear,iEta,iLayer);
}

double HEDarkening::degradation(int firstYear, int currYear, int ieta, int layer){
  std::cout<<currYear<<std::endl;
  // if (currYear<2011 || currYear == 2013 || currYear ==14 || currYear == 2019 || currYear ==20 || currYear > 2023 )return 1; //No darkening
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
  
 // cout<< "I m in degradation for year: "<< currYear <<", tower # "<<ieta+ieta_shift<<", layer: "<<layer<<", resp: "<<response<<endl;
 // cout<<"degradation: .... "<<endl;
  return response;
  //return (exp(-intlumi/lumiscale[ieta][lay]));
}

int HEDarkening::getYearIndex(const int y) {
  int i = 0;
  while (1){
    if (year[i]==y)return i;
    else if (i==8)return -1;
    else i++;
  }
}
