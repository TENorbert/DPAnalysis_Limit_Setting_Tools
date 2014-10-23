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

datacard93="$1"
datacard185="$2"
datacard250="$3"
datacard368="$4"
datacard500="$5"
datacard733="$6"
datacard1000="$7"
datacard1076="$8"
datacard1444="$9"
datacard2000="$10"
datacard2161="$11"
datacard3000="$12"
#datacard4000="$13"
datacard6000="$13"
Outputfile="$14"

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

# condor_submit <condor_file.txt>
#echo "Upper Limit for "$datacard" ====>" >$Outputfile
#combine -M HybridNew -t1000 -S0 --frequentist --testStat LHC $datacard -H ProfileLikelihood  --fork 4 -m $datacard >>$Outputfile 
#combine -M HybridNew -t1000 -S0 --frequentist --testStat LHC $datacard -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m $ctau -n $datacard >>$Outputfile 
#combine -M HybridNew -t1000 -S0 --frequentist --testStat LHC $datacard -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m $ctau -n $datacard >>$Outputfile 
#combine -M HybridNew -t1000 -S0 --frequentist --testStat LHC $datacard -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m $ctau -n $datacard >>$Outputfile 
#combine -M HybridNew -t1000 -S0 --frequentist --testStat LHC $datacard -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m $ctau -n $datacard >>$Outputfile 
#combine -M HybridNew -t1000 -S0 --frequentist --testStat LHC $datacard -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m $ctau -n $datacard >>$Outputfile 

##### Local Running
echo "Upper Limit for "$datacard93" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard93 -H ProfileLikelihood  --fork 4 -m 93 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard93 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 93 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard93 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 93 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard93 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 93 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard93 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 93 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard93 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 93 >>$Outputfile 

echo "Upper Limit for "$datacard185" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard185 -H ProfileLikelihood  --fork 4 -m 250 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard185 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 185 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard185 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 185 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard185 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 185 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard185 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 185 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard185 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 185 >>$Outputfile 

echo "Upper Limit for "$datacard250" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard250 -H ProfileLikelihood  --fork 4 -m 250 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard250 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 250 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard250 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 250 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard250 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 250 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard250 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 250 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard250 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 250 >>$Outputfile 

echo "Upper Limit for "$datacard368" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard368 -H ProfileLikelihood  --fork 4 -m 368 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard368 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 368 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard368 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 368 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard368 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 368 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard368 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 368 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard368 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 368 >>$Outputfile 

echo "Upper Limit for "$datacard500" ====>" >>$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard500 -H ProfileLikelihood  --fork 4 -m 500 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard500 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 500 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard500 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 500 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard500 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 500 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard500 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 500 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard500 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 500 >>$Outputfile 

echo "Upper Limit for "$datacard733" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard733 -H ProfileLikelihood  --fork 4 -m 733 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard733 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 733 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard733 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 733 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard733 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 733 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard733 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 733 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard733 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 733 >>$Outputfile 

echo "Upper Limit for "$datacard1000" ====>" >>$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1000 -H ProfileLikelihood  --fork 4 -m 1000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1000 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 1000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1000 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 1000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1000 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 1000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1000 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 1000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1000 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 1000 >>$Outputfile 

echo "Upper Limit for "$datacard1076" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1076 -H ProfileLikelihood  --fork 4 -m 1076 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1076 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 1076 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1076 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 1076 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1076 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 1076 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1076 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 1076 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1076 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 1076 >>$Outputfile 

echo "Upper Limit for "$datacard1444" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1444 -H ProfileLikelihood  --fork 4 -m 1444 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1444 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 1444 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1444 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 1444 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1444 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 1444 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1444 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 1444 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard1444 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 1444 >>$Outputfile 

echo "Upper Limit for "$datacard2000" ====>" >>$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2000 -H ProfileLikelihood  --fork 4 -m 2000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2000 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 2000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2000 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 2000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2000 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 2000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2000 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 2000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2000 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 2000 >>$Outputfile 

echo "Upper Limit for "$datacard2161" ====>" >$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2161 -H ProfileLikelihood  --fork 4 -m 2161 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2161 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 2161 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2161 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 2161 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2161 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 2161 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2161 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 2161 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard2161 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 2161 >>$Outputfile 



echo "Upper Limit for "$datacard3000" ====>" >>$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard3000 -H ProfileLikelihood  --fork 4 -m 3000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard3000 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 3000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard3000 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 3000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard3000 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 3000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard3000 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 3000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard3000 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 3000 >>$Outputfile 

#echo "Upper Limit for "$datacard4000" ====>" >>$Outputfile
#combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard4000 -H ProfileLikelihood  --fork 4 -m 4000 >>$Outputfile 
#combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard4000 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 4000 >>$Outputfile 
#combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard4000 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 4000 >>$Outputfile 
#combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard4000 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 4000 >>$Outputfile 
#combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard4000 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 4000 >>$Outputfile 
#combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard4000 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 4000 >>$Outputfile 

echo "Upper Limit for "$datacard6000" ====>" >>$Outputfile
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood  --fork 4 -m 6000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.025 --fork 4 -m 6000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.16 --fork 4 -m 6000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.5 --fork 4 -m 6000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.84 --fork 4 -m 6000 >>$Outputfile 
combine -M HybridNew -t5000 -S1 --frequentist --testStat LHC $datacard6000 -H ProfileLikelihood --expectedFromGrid 0.98 --fork 4 -m 6000 >>$Outputfile 
