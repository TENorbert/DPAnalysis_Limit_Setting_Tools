#!/usr/bin/perl
####
# How to Run this script
# ./UseGridLimit.plx DP-Limit-DataCards/limit_100_1700 Output_limitL100-Ct1700.root FOutput_limitL100-Ct1700.log
#
# Orig by J Mans; Adjusted by TEN@UMN.EDU
####
$dfile=$ARGV[0];
$ofile=$ARGV[1];
$log=$ARGV[2];
$stub=$dfile;
$stub=~s/.*\///;
$parallelism=12;
$points=24;
$nominalXsecFB=1;
$username = getpwuid( $< );
#$wd="/export/scratch/user/${username}/${stub}";
$wd="/afs/cern.ch/user/t/${username}/MyArea/${stub}";
$stub=~/[a-z]+_([0-9]+)_([0-9]+)/;
$Lambda=$1;
$Ctau=$2;
print "$wd\n";
system("rm -rf $wd");
system("mkdir -p $wd");
#chdir($wd);
# Seed with the ProfileLikelihood
$cmd="combine -v0 -n DPp -m $Lambda -M ProfileLikelihood -t 20 $dfile|";
open(LOG,">${log}");
print LOG "===== $cmd \n";
open(SEED,$cmd);
while (<SEED>) {
print LOG;
if (/median expected limit: r < ([0-9.]+)/) {
$seed_median=$1;
}
if (/68% expected band : ([0-9.]+) < r < ([0-9.]+)/) {
$seed_68l=$1;
$seed_68h=$2;
}
if (/95% expected band : ([0-9.]+) < r < ([0-9.]+)/) {
$seed_95l=$1;
$seed_95h=$2;
}
}
close(SEED);
print LOG "$seed_median $seed_68l $seed_68h\n";
$lp=log($seed_95l*0.75);
$hp=log($seed_95h*1.5);
$cmd="combine -v0 -n DPphoObs -m $Lambda -M HybridNew -H ProfileLikelihood --rMin 0.01 --rMax 10.0 --rAbsAcc 0.01 --frequentist --testStat LHC --fork 4 $dfile|";
print LOG "===== $cmd \n";
open(OSUM,"$cmd");
while (<OSUM>) {
print LOG;
if (/Limit: r < ([0-9.]+) +/) { # +\/- ([0-9.]+)) {
$obs_med=$1;
}
}
close(OSUM);
print LOG "$obs_med\n";
$running=0;
$files="";
for ($i=0; $i<$points; $i++) {
$rv=$lp+$i*($hp-$lp)/$points;
$rv=exp($rv);
$seed=$i+248100;
$cmd="combine -v0 -n DPp -m $Lambda -M HybridNew --clsAcc 0 -t 10 --saveToys --saveHybridResult --singlePoint $rv -s $seed --frequentist --testStat LHC $dfile > ${i}.log 2>&1 ";
print LOG "===== $cmd \n";
$files.=sprintf("higgsCombineDPp.HybridNew.mH%d.%d.root ",$Lambda,$seed);
if (fork()==0) {
system($cmd);
exit(0);
}
$running++;
while ($running>=$parallelism) {
# print "Waiting\n";
$av=wait();
$running-- if ($av!=-1);
break if ($av==-1);
}
}
while ($running>0) {
$av=wait();
$running-- if ($av!=-1);
break if ($av==-1);
}
close(LOG);
unlink($ofile);
system("hadd $ofile $files >> $log 2>&1 ");
open(LOG,">>${log}");
$summary=$log;
$summary=~s/\.log/-summary.log/;
open(SUM,">$summary");
print SUM "LAMBDA=${Lambda}\n";
print SUM "CTAU=${Ctau}\n";
# now, we extract the limits
limitFromGrid(-1,"obs");
limitFromGrid(0.5,"exp");
limitFromGrid(0.84,"exp_p1s");
limitFromGrid(0.16,"exp_m1s");
limitFromGrid(0.975,"exp_p2s");
limitFromGrid(0.025,"exp_m2s");
close(SUM);
close(LOG);
system("xz ${log}");
sub limitFromGrid() {
my($pt,$label)=@_;
if ($pt>0) {
$cmd="combine ${dfile} -M HybridNew --grid=${ofile} --expectedFromGrid ${pt} -m ${Lambda}";
print LOG "===== $cmd \n";
open(CMD,"$cmd |");
while (<CMD>) {
print LOG;
if (/Limit: r < ([0-9.]+)/) {
$rv=$1;
}
}
close(CMD);
print SUM "${label} = $rv\n";
} else {
#$cmd="combine ${dfile} -M HybridNew --grid=${ofile} -m ${Lambda}";
print SUM "obs = $obs_med\n";
}
}
