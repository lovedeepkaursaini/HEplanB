#include "HEDarkening.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
#include "TROOT.h"
#include "TH2F.h"
#include "TFile.h"
//#define DebugLog

HEDarkening::HEDarkening(unsigned int scenario) :
  year    {2010,2011,2012,  2013, 2014, 2015,  2016,  2017, 2018,  2019, 2020, 2021, 2022, 2023}, //2010,13,14,19,20 dummy, set as 0 for int. lumi or lumi
  ilumi   {0,   5.6, 28.9,  0., 0.,     33.0,  63.0,  128.0,193.0, 0.,   0.,   293.0,393.0,493.0},
  lumi    {0,   5.6, 23.3,  0., 0.,     4.1,   30,    65,   65,    0.,   0.,   100,  100,  100},
  doserate{0,   0.005,0.013,0., 0.,     0.009, 0.026, 0.043,0.043, 0.,   0.,   0.05, 0.05, 0.05}

{
  ieta_shift = 16;
  
  aConst = 1.57;                                                                                                                                                     
  bConst = 0.538;                                                                                                                                                     
  //aug15,2016
  //The “4TeV” map applies to 2011 and 2012 runs, and the “7TeV” map to 2015 and later.

  
  //Hardcode fluka dose map for the time being.
  //layers count start from -1 and dose for it set as same as for layer 0
  const double _doseMap_7TeV [nEtaBins][nScintLayers] = {
    {0, 0, 0, 0, 1.04169e-05, 7.0851e-06, 3.45277e-06, 1.97179e-06, 1.29751e-06, 8.12643e-07, 5.21069e-07, 4.39539e-07, 0, 0, 0, 0, 0, 0, 0 },
    {6.41055e-05, 6.41055e-05, 3.77665e-05, 2.48917e-05, 1.55142e-05, 1.01644e-05, 5.98729e-06, 4.47106e-06, 2.57327e-06, 1.65583e-06, 1.18412e-06, 7.62716e-07, 4.11486e-07, 2.75138e-07, 2.2617e-07, 0, 0, 0, 0},
    {0.000106981, 0.000106981, 6.75986e-05, 4.39621e-05, 2.8876e-05, 1.92157e-05, 1.20511e-05, 8.21195e-06, 5.58752e-06, 3.8293e-06, 2.42651e-06, 1.43422e-06, 1.07732e-06, 7.57427e-07, 4.00963e-07, 3.85561e-07, 2.10408e-07, 2.06579e-07, 1.65067e-07},
    {0.000186173, 0.000186173, 0.000113704, 8.0048e-05, 5.35935e-05, 3.60206e-05, 2.41339e-05, 1.66962e-05, 1.05307e-05, 7.16253e-06, 3.99943e-06, 2.81461e-06, 2.09285e-06, 1.22785e-06, 1.12699e-06, 6.95595e-07, 3.44666e-07, 2.4328e-07, 4.53805e-07 },
    {0.000259151, 0.000259151, 0.000167139, 0.000116368, 8.06327e-05, 5.54382e-05, 3.69251e-05, 2.44325e-05, 1.80451e-05, 1.06238e-05, 7.54182e-06, 4.74617e-06, 3.49052e-06, 2.08107e-06, 1.35571e-06, 1.11385e-06, 6.06968e-07, 5.35681e-07, 4.64625e-07}, 
    {0.000338283, 0.000338283, 0.000232531, 0.000167013, 0.000119032, 8.02514e-05, 5.40997e-05, 3.71662e-05, 2.57748e-05, 1.62543e-05, 1.11141e-05, 7.31538e-06, 5.21127e-06, 3.16344e-06, 2.74217e-06, 2.25833e-06, 1.09619e-06, 7.69602e-07, 7.48441e-07}, 
    {0.000458306, 0.000458306, 0.000329581, 0.000242296, 0.00017689, 0.000119708, 7.9648e-05, 5.53821e-05, 3.92938e-05, 2.65461e-05, 1.72514e-05, 1.28871e-05, 8.68546e-06, 5.32072e-06, 5.09479e-06, 2.82754e-06, 2.15726e-06, 1.46387e-06, 1.41249e-06}, 
    {0.000641789, 0.000641789, 0.000496637, 0.000358111, 0.000257986, 0.000182171, 0.000123549, 8.63902e-05, 5.99267e-05, 3.83196e-05, 2.89796e-05, 2.01147e-05, 1.39902e-05, 1.04614e-05, 6.69897e-06, 4.92296e-06, 3.20188e-06, 2.8376e-06, 1.8316e-06}, 
    {0.000901178, 0.000901178, 0.000730147, 0.000548701, 0.000388954, 0.000279802, 0.000198241, 0.000137335, 9.87215e-05, 6.84092e-05, 4.84553e-05, 3.19036e-05, 2.32472e-05, 1.65942e-05, 1.2378e-05, 8.88643e-06, 5.72947e-06, 4.92052e-06, 3.08615e-06}, 
    {0.00133804, 0.00133804, 0.00115398, 0.000870244, 0.000621144, 0.000462982, 0.000332914, 0.000230478, 0.000162129, 0.000118771, 8.03863e-05, 5.65533e-05, 4.36531e-05, 2.94137e-05, 2.24036e-05, 1.7869e-05, 1.17343e-05, 9.49663e-06, 7.15003e-06}, 
    {0.00204948, 0.00204948, 0.00189352, 0.00143658, 0.00104873, 0.000772498, 0.000550745, 0.000396343, 0.000290628, 0.000213597, 0.00015653, 0.000108865, 8.48056e-05, 6.13595e-05, 4.68353e-05, 3.92003e-05, 2.86936e-05, 2.38083e-05, 1.87514e-05}, 
    {0.00298876, 0.00298876, 0.00296063, 0.00225681, 0.00166062, 0.00123318, 0.000908371, 0.000671423, 0.000483246, 0.000365001, 0.000280074, 0.000201985, 0.000159967, 0.000128292, 0.000101657, 8.35071e-05, 7.50964e-05, 5.89237e-05, 5.03306e-05}, 
    {0.00443296, 0.00443296, 0.00481976, 0.00349831, 0.00261662, 0.00195546, 0.00211829, 0.00170831, 0.00139417, 0.00116364, 0.000982658, 0.000853974, 0.00074493, 0.000690484, 0.000592507, 0.000533603, 0.000481528, 0.000444415, 0.000417535}, 
    {0.0101209, 0.0101209, 0.0120926, 0.00695758, 0.00487591, 0.003866, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} 


  };
  
  const double _doseMap_4TeV [nEtaBins][nScintLayers] = {
    {0, 0, 0, 0, 7.13881e-06, 4.36795e-06, 3.38345e-06, 1.3488e-06, 1.09062e-06, 6.95122e-07, 1.02265e-06, 2.08338e-07, 0, 0, 0, 0, 0, 0, 0},
    {4.90809e-05, 4.90809e-05, 2.98646e-05, 2.01822e-05, 1.16861e-05, 8.74001e-06, 4.73107e-06, 2.96299e-06, 1.91818e-06, 1.29249e-06, 9.17852e-07, 5.28536e-07, 4.04551e-07, 1.86693e-07, 1.68977e-07, 0, 0, 0, 0 },
    {8.18763e-05, 8.18763e-05, 5.30274e-05, 3.39795e-05, 2.17837e-05, 1.32876e-05, 8.77963e-06, 5.70792e-06, 3.89239e-06, 2.48827e-06, 2.24001e-06, 8.61296e-07, 6.84139e-07, 4.07136e-07, 2.24442e-07, 6.06341e-07, 2.17533e-07, 1.28277e-07, 1.95167e-07 },
    {0.000150183, 0.000150183, 9.33663e-05, 6.1219e-05, 4.12833e-05, 2.68744e-05, 1.83797e-05, 1.1456e-05, 7.064e-06, 4.97222e-06, 3.16566e-06, 2.45888e-06, 1.63586e-06, 1.04798e-06, 6.12064e-07, 3.77065e-07, 3.03202e-07, 2.03016e-07, 1.76588e-07 },
    {0.000197159, 0.000197159, 0.000131559, 9.20889e-05, 6.20364e-05, 4.32983e-05, 2.86049e-05, 1.85217e-05, 1.33494e-05, 8.96186e-06, 6.1301e-06, 3.46284e-06, 2.25568e-06, 1.81668e-06, 1.24443e-06, 7.83383e-07, 6.44456e-07, 6.02248e-07, 2.65023e-07 },
    {0.000277126, 0.000277126, 0.000192471, 0.000132399, 9.78929e-05, 6.24279e-05, 4.29971e-05, 3.13023e-05, 2.10283e-05, 1.30427e-05, 9.14407e-06, 6.22866e-06, 3.91093e-06, 2.85732e-06, 2.20171e-06, 1.81617e-06, 1.38505e-06, 1.26317e-06, 6.01188e-07}, 
    {0.00037762, 0.00037762, 0.00026336, 0.000187376, 0.000132562, 9.40212e-05, 6.41114e-05, 4.40068e-05, 2.83479e-05, 1.90327e-05, 1.36684e-05, 9.06711e-06, 6.56536e-06, 4.74323e-06, 3.27319e-06, 2.12624e-06, 1.62384e-06, 1.01539e-06, 9.40781e-07 },
    {0.000518705, 0.000518705, 0.000392791, 0.00028284, 0.000208286, 0.000141732, 9.5607e-05, 6.71323e-05, 4.69692e-05, 2.96149e-05, 1.98559e-05, 1.48252e-05, 1.15107e-05, 7.31413e-06, 4.52753e-06, 2.94406e-06, 2.89987e-06, 1.66177e-06, 1.57329e-06}, 
    {0.000742, 0.000742, 0.000597508, 0.000432557, 0.000308914, 0.000215585, 0.000159913, 0.000112334, 7.37817e-05, 5.95234e-05, 3.32454e-05, 2.55701e-05, 1.99843e-05, 1.38419e-05, 9.14924e-06, 7.22134e-06, 4.76832e-06, 3.67155e-06, 2.57449e-06}, 
    {0.00111619, 0.00111619, 0.000939271, 0.000702459, 0.000508214, 0.000362243, 0.000265904, 0.000183685, 0.000134428, 9.22114e-05, 6.42866e-05, 4.54858e-05, 3.31561e-05, 2.35836e-05, 1.87642e-05, 1.40304e-05, 9.60822e-06, 6.8192e-06, 6.03673e-06}, 
    {0.00170819, 0.00170819, 0.0015618, 0.00116921, 0.000867079, 0.000642795, 0.000447157, 0.000329993, 0.000230796, 0.000169917, 0.000122783, 9.0673e-05, 6.84071e-05, 5.02039e-05, 3.84228e-05, 3.00036e-05, 2.44637e-05, 1.84054e-05, 1.58614e-05}, 
    {0.00248414, 0.00248414, 0.00247299, 0.0018046, 0.00136495, 0.000986899, 0.000753882, 0.000535137, 0.000397247, 0.000302302, 0.000220461, 0.000179427, 0.000131723, 0.000104264, 8.66996e-05, 7.02749e-05, 5.95469e-05, 4.73914e-05, 4.22979e-05}, 
    {0.00395864, 0.00395864, 0.00397347, 0.00284622, 0.00217326, 0.001595, 0.00179279, 0.00146422, 0.00120687, 0.001019, 0.000882959, 0.000754963, 0.000660849, 0.000593096, 0.000535623, 0.00048851, 0.000433377, 0.000399182, 0.00037423 },
    {0.00889222, 0.00889222, 0.00992053, 0.00579247, 0.00409976, 0.00327175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
    
    
  };
  
  for(unsigned int iR=0;iR<nEtaBins;iR++){
    for(unsigned int iC=0;iC<nScintLayers;iC++){
      doseMap_4TeV[iR][iC] = _doseMap_4TeV[iR][iC];
      doseMap_7TeV[iR][iC] = _doseMap_7TeV[iR][iC];
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

double HEDarkening::getFlukaDose_4TeV(int ieta, int ilayer){
   double dose = doseMap_4TeV[ieta][ilayer];
   if (dose==0)return 1.;
   else return dose;
}
double HEDarkening::getFlukaDose_7TeV(int ieta, int ilayer){
   double dose = doseMap_7TeV[ieta][ilayer];
   if (dose==0)return 1.;
   else return dose;
}


double HEDarkening::standAloneResponse(int year, int ieta, int layer){
  if (year<=2010) return 1.; 
 double lumi = getLumi(year);
  if(lumi <=0) return 1;
  double lumiRate = getLumiRate(year);
  double dose  = 1;
  if(year < 2013) dose = getFlukaDose_4TeV(ieta,layer);
  else dose = getFlukaDose_7TeV(ieta,layer);
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

  //shift ieta tower index
  ieta -= ieta_shift;
  //if outside eta range, no darkening
  if(ieta < 0 || ieta >= (int)nEtaBins) {cout<<"tower # "<<ieta<<" > "<<nEtaBins<<" known? "<<std::endl;return 1.;}

 //shift layer index by 1 to act as array index
  ilayer += 1;
 
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
    double dose = 1;
    if(currYear < 2013 )dose = getFlukaDose_4TeV(ieta,ilayer);
    else dose = getFlukaDose_7TeV(ieta,ilayer);
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


double roundTo100(double floatValue)
{
  return double(int(floatValue * 1000 + 0.5)) / 1000; 
}

void testHEDarkening(){
  
  TFile *f = new TFile("kieDoseMap.root","RECREATE");
  TH2F *hResp_153 = new TH2F("hResp_153","Response",18,0,18,14,16,30);
  
  HEDarkening h(1);
  // for (int t=29; t<30; t++){
  //  for (int i=0; i<18; i++){
  //      std::cout<< h.degradation(2011,2023,29,0)<<std::endl;
  std::cout<<h.getYearsForLumi(500)<<std::endl;
  std::cout<<h.degradation(153,29,0)<<std::endl;
  std::cout<<h.degradation(500,29,0)<<std::endl;
  //std::cout<<h.degradation(2011,2023,29,0)<<std::endl;
  // }
  // }
  //Initialize the response histogram.
  for (int Y=0; Y<14; Y++){
    for(int X=0; X<18; X++){
      double degn = h.degradation(153,Y+16,X);
      cout<<roundTo100(degn)<<endl;
      hResp_153->SetBinContent(X+1,Y+1,roundTo100(degn));
    }
  }
  hResp_153->Write();
  f->Write();
  f->Close();
}
