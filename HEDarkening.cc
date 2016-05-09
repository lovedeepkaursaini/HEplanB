#include "HEDarkening.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
//#define DebugLog

HEDarkening::HEDarkening(unsigned int scenario) :
  year    {2010,2011,2012,  2013, 2014, 2015,  2016,  2017, 2018,  2019, 2020, 2021, 2022, 2023}, //2010,13,14,19,20 dummy, set as 0 for int. lumi or lumi
  ilumi   {0,   5.6, 28.9,  0., 0.,     33.0,  63.0,  128.0,193.0, 0.,   0.,   293.0,393.0,493.0},
  lumi    {0,   5.6, 23.3,  0., 0.,     4.1,   30,    65,   65,    0.,   0.,   100,  100,  100},
  doserate{0,   0.005,0.013,0., 0.,     0.009, 0.026, 0.043,0.043, 0.,   0.,   0.05, 0.05, 0.05}

{
  ieta_shift = 16;
  
  aConst = 2.4;                                                                                                                                                     
  bConst = 0.6;                                                                                                                                                     
  
  //Hardcode fluka dose map for the time being.
  //layers count start from -1 and dose for it set as same as for layer 0
  const double _doseMap [nEtaBins][nScintLayers] = {
    {2.500000E+00, 2.500000E+00, 0.000000E+00, 0.000000E+00, 3.116667E-06, 3.133333E-06, 2.050000E-06, 9.166667E-07, 4.000000E-07, 4.000000E-07, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00},
    {2.075000E-05, 2.075000E-05, 2.805000E-05, 1.775000E-05, 2.068333E-05, 7.566667E-06, 5.883333E-06, 3.216667E-06, 1.883333E-06, 6.000000E-07, 4.333333E-07, 3.333333E-07, 2.000000E-07, 3.333333E-08, 1.333333E-07, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00},
    {9.990000E-05, 9.990000E-05, 6.360000E-05, 4.546667E-05, 2.083333E-05, 1.463333E-05, 1.200000E-05, 7.750000E-06, 5.366667E-06, 4.466667E-06, 1.400000E-06, 8.500000E-07, 1.666667E-07, 2.166667E-07, 1.833333E-07, 6.666667E-08, 1.666667E-08, 0.000000E+00, 0.000000E+00},
    {1.857667E-04, 1.857667E-04, 1.059833E-04, 6.970000E-05, 5.113333E-05, 2.800000E-05, 1.875000E-05, 1.120000E-05, 8.316667E-06, 6.900000E-06, 2.183333E-06, 2.766667E-06, 1.033333E-06, 8.833333E-07, 1.150000E-06, 2.000000E-07, 1.666667E-08, 2.000000E-07, 1.666667E-07},
    {2.759833E-04, 2.759833E-04, 1.728167E-04, 1.175500E-04, 8.326667E-05, 4.255000E-05, 3.216667E-05, 2.383333E-05, 1.541667E-05, 8.483333E-06, 4.950000E-06, 2.350000E-06, 1.783333E-06, 1.050000E-06, 8.333333E-07, 1.050000E-06, 1.833333E-07, 1.333333E-07, 1.333333E-07},
    {3.588167E-04, 3.588167E-04, 2.211167E-04, 1.559667E-04, 1.053500E-04, 9.741667E-05, 5.316667E-05, 2.840000E-05, 1.745000E-05, 1.195000E-05, 1.366667E-05, 7.483333E-06, 2.966667E-06, 2.066667E-06, 2.350000E-06, 7.166667E-07, 8.333333E-07, 3.500000E-07, 1.250000E-06},
    {4.883833E-04, 4.883833E-04, 3.191333E-04, 2.472167E-04, 1.547667E-04, 1.066167E-04, 7.736667E-05, 4.993333E-05, 2.571667E-05, 1.958333E-05, 1.820000E-05, 1.316667E-05, 6.466667E-06, 5.183333E-06, 2.500000E-06, 2.250000E-06, 2.316667E-06, 5.000000E-07, 9.000000E-07},
    {6.569000E-04, 6.569000E-04, 4.815000E-04, 3.872333E-04, 2.520833E-04, 1.690833E-04, 1.121000E-04, 6.795000E-05, 5.610000E-05, 3.575000E-05, 2.610000E-05, 1.266667E-05, 7.083333E-06, 7.483333E-06, 5.183333E-06, 3.016667E-06, 1.166667E-06, 2.183333E-06, 1.116667E-06},
    {9.552500E-04, 9.552500E-04, 7.336000E-04, 5.263833E-04, 3.781167E-04, 2.509500E-04, 1.474167E-04, 1.114000E-04, 7.310000E-05, 7.055000E-05, 3.200000E-05, 2.676667E-05, 1.811667E-05, 1.670000E-05, 1.136667E-05, 8.666667E-06, 3.700000E-06, 3.550000E-06, 1.583333E-06},
    {1.409033E-03, 1.409033E-03, 1.122883E-03, 9.155667E-04, 6.086500E-04, 4.302833E-04, 2.925667E-04, 1.871167E-04, 1.292667E-04, 8.776667E-05, 7.793333E-05, 4.665000E-05, 2.490000E-05, 2.615000E-05, 1.446667E-05, 7.766667E-06, 7.283333E-06, 9.183333E-06, 6.883333E-06},
    {2.163600E-03, 2.163600E-03, 1.866083E-03, 1.303450E-03, 1.016433E-03, 7.006000E-04, 5.508667E-04, 3.320833E-04, 2.387500E-04, 1.766667E-04, 1.311833E-04, 1.014167E-04, 5.851667E-05, 4.396667E-05, 3.153333E-05, 1.618333E-05, 2.053333E-05, 1.295000E-05, 1.780000E-05},
    {3.098283E-03, 3.098283E-03, 2.984033E-03, 2.155650E-03, 1.645467E-03, 1.141617E-03, 8.015667E-04, 5.699167E-04, 4.089667E-04, 3.060500E-04, 2.327667E-04, 1.609000E-04, 1.076667E-04, 7.761667E-05, 7.725000E-05, 8.660000E-05, 5.126667E-05, 4.308333E-05, 3.891667E-05},
    {4.599117E-03, 4.599117E-03, 4.781817E-03, 3.099200E-03, 2.252433E-03, 1.714900E-03, 2.053367E-03, 1.671167E-03, 1.459333E-03, 1.238833E-03, 1.041000E-03, 8.670167E-04, 8.372667E-04, 7.145667E-04, 5.851333E-04, 5.941833E-04, 5.394167E-04, 5.035667E-04, 4.553667E-04},
    {1.082867E-02, 1.082867E-02, 1.072262E-02, 6.020917E-03, 4.557333E-03, 3.753517E-03, 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}
    
  };

  for(unsigned int iR=0;iR<nEtaBins;iR++){
    for(unsigned int iC=0;iC<nScintLayers;iC++){
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
  if (year<=2010) return 1.; 
 double lumi = getLumi(year);
  if(lumi <=0) return 1;
  double lumiRate = getLumiRate(year);
  double dose  = getFlukaDose(ieta,layer);
  //Calculate decay constant
  double decayConst = aConst*std::pow((1000*dose*lumiRate),bConst);
  //Calculate accumulated dose
  double accumDose = dose*lumi;

  //Calculate response
  double response = exp(-(accumDose/decayConst));
  //  cout<< "I m in SAR for year: "<<year<<", lumi: "<< lumi <<", dose: "<<dose<<", accumDose: "<<accumDose<<
  //  ", response: "<<response<<endl;

  //return the value
  return response;
}

int HEDarkening::getYearsForLumi(double iL){
  // How many years have passed by
  int i = 0;
  while(iL>=ilumi[i]){
    i++;
    if (i>13)break;  
  }
  i=i-1;
  int yr = year[i];
  //If the year is 2023 simply return 0 months.
  if (yr==2023) return 100*yr;
  
  //Now let us find the months over that.
  double furk = iL-ilumi[i];
  //  int months = furk*(12./(ilumi[i+1]-ilumi[i]));
  int months = furk*12./lumi[i+1];
  //cout<<"i: "<<i<<", furk: "<<furk<<", months: "<<months<<", 100*yr+months: "<<100*yr+months<<endl;
  return 100*yr+months;
}

double HEDarkening::degradation(double iLumi, int ieta, int ilayer){
  int firstYear = 2010;
  int yearMonth = getYearsForLumi(iLumi);
  //Extract year and month
  int currYear = (int)yearMonth/100;
  int months = yearMonth-currYear*100;

  //shift ieta tower index
  ieta -= ieta_shift;
  //if outside eta range, no darkening
  if(ieta < 0 || ieta >= (int)nEtaBins) {cout<<"tower # "<<ieta<<" > "<<nEtaBins<<" known? "<<std::endl;return 1.;}
  if(ilayer < 0 || ilayer >= (int)nScintLayers){cout<<"layer # "<<ilayer<<" > "<<nScintLayers<<" known? "<<std::endl; return 1.;}

  //Calculate darkening factor: accumulated over years.
  double response = 1.;
  
  //... damage through the past years.
  for (int yr=firstYear; yr<currYear+1; yr++)
    response = response*standAloneResponse(yr,ieta,ilayer);
  
  //damage in left over months
  int i = 0; //index of year passed year.
  while(! (year[i]==currYear)) i++;
  double lumiSoFar = iLumi - ilumi[i];

  if (iLumi>493) return response;
  if(lumiSoFar<=0) return response;
  else {
    
    double dR = doserate[i+1];
    double dose = getFlukaDose(ieta,ilayer);
    double decayConst =  aConst*std::pow((1000*dose*dR),bConst);
    response = response*exp(-(lumiSoFar*dose)/decayConst);
  }
  return response;
  
}

int HEDarkening::getYearIndex(const int y) {
  int i = 0;
  while (1){
    if (year[i]==y)return i;
    else if (i==13)return -1;
    else i++;
  }
}

const char* HEDarkening::scenarioDescription (unsigned int scenario) {
 if (scenario == 0) return "full replacement of HE scintillators, no darkening";
 else if (scenario == 1) return "no replacement, full stage darkening";
 else if (scenario == 2) return "only replace scintillators with expected light yield < 20% for 500 fb-1";
 else if (scenario == 3) return "replace complete megatiles only in the front 4 layers";
 return "undefined scenario: assume no replacement, full stage darkening";
}

int main(){

  HEDarkening h(1);
 
  double test[29] = {0.,1.0,1.5,2.0,2.5,3.0,4.0,4.3,4.6,4.9,5.1,5.4,5.7,6.1,6.5,7.0,7.5,18.0,50,150,200,250,300,350,400,450,492,493,500};
  for(int i=0; i!=29; i++){
    std::cout<<test[i]<<"\t"<<h.getYearsForLumi(test[i])<<"    ";
    std::cout<<h.degradation(test[i],29,0)<<endl;
  }
}

