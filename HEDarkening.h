#ifndef HcalCalibObjects_HEDarkening_h
#define HcalCalibObjects_HEDarkening_h

class HEDarkening {
 public:
  HEDarkening(unsigned int scenario);
  ~HEDarkening();
  double degradation(int firstYear, int currYear, int ieta, int layer) ;
  double standAloneResponse(int year, int ieta, int layer);
  double getLumi(int year);
  double getLumiRate(int year);
  double getFlukaDose(int eta, int layer);
  int getYearsForLumi(double iL);
  double degradation(double iLumi, int iEta, int iLayer);
  static const char* scenarioDescription (unsigned int scenario);
 // (1) number of HE ieta bins for darkening
  static const unsigned int nEtaBins = 14;
 // // (2) max. number of HE scint. layers
  static const unsigned int nScintLayers = 19;
 
 private:
  int ieta_shift;
  //Note: following arrays are parallel.
  float    aConst ;//= 2.4;
  float    bConst ;//= 0.6;
  //int    year[9];
  //double lumi[9];
  //double doserate[9];
 // static const unsigned int Nieta = 14;
 // static const unsigned int Nlayer = 18;
  double doseMap [nEtaBins][nScintLayers];
  int getYearIndex(int y);
  
  int year[9];//={2011,2012,2015,2016,2017, 2018, 2021, 2022, 2023};
  float ilumi[9];//=   {5.6, 28.9,33.0,63.0,128.0,193.0,293.0,393.0,493.0};
  float lumi[9];//={5.6, 23.3,4.1, 30,  65,  65,  100, 100, 100};
  float doserate[9];//={0.005,0.013,0.009,0.026,0.043,0.043,0.05,0.05,0.05};
};
#endif // HEDarkening_h
