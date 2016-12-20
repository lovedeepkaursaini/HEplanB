# Setup for HE radiation damage jet study, modified for new geometry and new raddam model by LPCPlanB group
```
source ~/setuafenv
cmsrel CMSSW_6_2_0_SLHC25_patch3
cd CMSSW_6_2_0_SLHC25_patch3/src/
cmsenv
git cms-merge-topic -u bsunanda:Run2-hcx09
git cms-merge-topic kpedro88:SLHC-HERadDamJets
git clone git@github.com:kpedro88/HERadDamJets
git clone git@github.com:kpedro88/JetMETAnalysis --branch RadDam1
git clone git@github.com:kpedro88/Analysis
git cms-addpkg DataFormats/HcalCalibObjects
git cms-addpkg CalibCalorimetry/HcalPlugins
git cms-addpkg SimGeneral/DataMixingModule
git cms-addpkg SimG4CMS/Calo
git cms-addpkg SimG4CMS/HcalTestBeam
git cms-addpkg SimCalorimetry/HcalSimProducers
git cms-addpkg Validation/HcalHits
git clone https://github.com/lovedeepkaursaini/HEplanB/
cp HEplanB/HEDarkening.cc DataFormats/HcalCalibObjects/src/
cp HEplanB/HEDarkening.h DataFormats/HcalCalibObjects/interface/
scram b -j 6
```

## to get rid off error from class version, Increment ClassVersion to 1 and assign it to checksum ------
```
scram build updateclassversion
vi DataFormats/HcalCalibObjects/src/classes_def.xml
---------------------
you will see something:
<class name="HEDarkening" ClassVersion="0">
   <version ClassVersion="1" checksum="535394469"/>
---------------------
cd HERadDamJets/FullSim/test/
```

##a temporary interactive run to check setup
```
./FStemp.sh DiJet_GEN_SIM . . 19 30 0 10 1 run 
```
##for running jobs on condor
- remeber to edit path names
- My output for latest rad-dam model is here:
/store/user/lovedeep/newestraddam

- previous radam model
/store/user/lovedeep/newraddam


###To digitize and reconstruct QCD DiJet events:
```
./FSsub_digireco.sh (edit path)
```
###To compute L2L3 jet energy corrections:
```
./FSsub_JRA.sh
./run_jec.sh
```
###To run the jet analyzer:
```
./FSsub_tree.sh
```
###To make plots:
```
./hadd_tree_jet.sh
root -b -l -q 'fs_jet_comp.C+(-1,true)'
```
####A file edited to make plots mentioned by Marek in Aug https://github.com/lovedeepkaursaini/HEplanB/blob/master/reso_fit_vs_eta__pt30_PF_2019__lumi0__lumi150__lumi300__lumi500__lumi500.png

####get my macro from here:
```
wget https://github.com/lovedeepkaursaini/HEplanB/blob/master/fs_jet_comp.C
root -b -l -q 'fs_jet_comp.C+(1,true)'
```
### NOTES
- class to go into dataformats i.e. Depth SEg: Geometry/HcalCommonData/data/PhaseI/hcalRecNumbering.xml

