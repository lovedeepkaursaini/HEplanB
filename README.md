# HEplanB

class to go into dataformats


Depth SEg: Geometry/HcalCommonData/data/PhaseI/hcalRecNumbering.xml

source ~/setuafenv
cmsrel CMSSW_6_2_0_SLHC25_patch3
cd CMSSW_6_2_0_SLHC25_patch3/src/
cmsenv
git cms-merge-topic -u bsunanda:Run2-hcx09
git cms-merge-topic kpedro88:SLHC-HERadDamJets
git clone git@github.com:kpedro88/HERadDamJets
git clone git@github.com:kpedro88/JetMETAnalysis --branch RadDam1
git clone git@github.com:kpedro88/Analysis
scram b -j 6
git cms-addpkg DataFormats


