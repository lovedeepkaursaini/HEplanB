#ifndef HcalCalibObjects_HEDarkening_dev_h
#define HcalCalibObjects_HEDarkening_dev_h

class HEDarkening_dev {
 public:
  HEDarkening_dev();
  ~HEDarkening_dev();
  double degradation(int firstYear, int currYear, int ieta, int layer) ;
  double standAloneResponse(int year, int ieta, int layer);
  double getLumi(int year);
  double getLumiRate(int year);
  double getFlukaDose(int eta, int layer);
  
 private:
  int ieta_shift;
  //Note: following arrays are parallel.
  float    aConst = 2.4;
  float    bConst = 0.6;
  //int    year[9];
  //double lumi[9];
  //double doserate[9];
  static const unsigned int Nieta = 14;
  static const unsigned int Nlayer = 18;
  double doseMap [Nieta][Nlayer];
  int getYearIndex(int y);

  const int year[9]={2011,2012,2015,2016,2017,2018,2021,2022,2023};
  float lumi[9]={5.6, 23.3,4.1, 30,  65,  65,  100, 100, 100};
  float doserate[9]={0.005,0.013,0.009,0.026,0.043,0.043,0.05,0.05,0.05};
};
#endif // HEDarkening_dev_h
