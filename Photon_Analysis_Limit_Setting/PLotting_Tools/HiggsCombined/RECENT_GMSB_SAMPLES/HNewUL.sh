#!/bin/bash   
# Simple scripts: can be used with Condor or
# run alone calling Higgs combined tool
# to calculate upper limits
# And expectations!
# Designed by TEN@UMN.EDU 
# Summer 2014

echo Hi there!, I am soucing cmssw environment
. /local/grid/cmssoft/cms/cmsset_default.sh
cd /data/whybee1a/user/norbert/GMSBAnalysis/2014-GMSB-GGM-DPAnalysis/CMSSW_6_1_1/src/GMSB_HEBEAMHALOSTUDY/Photon_Analysis_Limit_Setting/PLotting_Tools/HiggsCombined/RECENT_GMSB_SAMPLES/
echo Now sourcing cms commands
eval `scramv1 runtime -sh`

#datacard="$1"
#Outputfile="$2"
#ctau="$3"

datacard6000="$1"
Outputfile="$2"

## Asymptotic Limit Setting
## local Limit setting
#echo "Upper Limit for "$datacard250" ====>" >$Outputfile
#combine -M Asymptotic $datacard250  -S0 -t1000 -m 250  >>$Outputfile
#echo "Upper Limit for "$datacard500" ====>" >> $Outputfile
#combine -M Asymptotic $datacard500  -S0 -t1000 -m 500  >>$Outputfile
#echo "Upper Limit for "$datacard1000" ====>" >>$Outputfile
#combine -M Asymptotic $datacard1000 -S0 -t1000 -m 1000 >>$Outputfile
#echo "Upper Limit for "$datacard2000" ====>" >>$Outputfile
#combine -M Asymptotic $datacard2000 -S0 -t1000 -m 2000 >>$Outputfile
#echo "Upper Limit for "$datacard3000" ====>" >> $Outputfile
#combine -M Asymptotic $datacard3000 -S0 -t1000 -m 3000 >>$Outputfile
#echo "Upper Limit for "$datacard4000" ====>" >>$Outputfile
#combine -M Asymptotic $datacard4000 -S0 -t1000 -m 4000 >>$Outputfile
#echo "Upper Limit for "$datacard6000" ====>" >>$Outputfile
#combine -M Asymptotic $datacard6000 -S0 -t1000 -m 6000 >>$Outputfile

#### HybridNew Limit Calc #############

echo "Upper Limit for "$datacard6000" ====>" >>$Outputfile
combine -M HybridNew -t100 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood  -m 6000 >>$Outputfile 
combine -M HybridNew -t100 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.025  -m 6000 >>$Outputfile 
combine -M HybridNew -t100 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.16   -m 6000 >>$Outputfile 
combine -M HybridNew -t100 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.5  -m 6000 >>$Outputfile 
combine -M HybridNew -t100 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.84  -m 6000 >>$Outputfile 
combine -M HybridNew -t100 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.98  -m 6000 >>$Outputfile 
