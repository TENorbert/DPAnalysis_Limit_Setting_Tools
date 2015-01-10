#!/usr/bin/perl
use v5.10;
use warnings;
# Calcualate scale limit from given limit
# Inputs: Input_limit_log_file, Nii, Njj(get Nii, Njj from dataCard)
# Output: Output_limit_log_file,
# 
# Run as: ./CalcScaleLimits.plx Input_limit_log_file  Nii, Njj, Output_limit_log_file
#
# Designed and implemented by TEN@UMN.EDU
# Jan 10, 2015
#
$fInp = $ARGV[0];
$Nii  = $ARGV[1]; #Signal events with limits
$Njj  = $ARGV[2]; #Signal events to calculated limits
$fOut = $ARGV[3]; # Output limit log file
$fh = $fOut;
$fh =~/[a-z]+_([0-9]+)_([0-9]+)/;

$LAMBDA = $1;
$CTAU   = $2;

#$rObsii; $rExpii; $r68lii; $r68hii; $r95hii; $r95lii; 
open (IFILE, "<$fInp") or die "Sorry, Can't open $fInp: $!\n";

while (<IFILE>) {

#print "Got it $_\n";
#print IFILE;
if (/obs = ([0-9.]+)/) {
$rObsii = $1;
}
if (/exp_m2s = ([0-9.]+)/ ) {
$r95lii = $1;
}
if (/exp_m1s = ([0-9.]+)/ ) {
$r68lii = $1;
}   
if (/exp = ([0-9.]+)/ ) {
$rExpii = $1;
}
if (/exp_p1s = ([0-9.]+)/ ) {
$r68hii = $1;
}
if (/exp_p2s = ([0-9.]+)/ ) {
$r95hii = $1;
}
 
}

#Now calculate and right limits in Output file
open (OFILE, ">$fOut");

print OFILE "LAMBDA = $LAMBDA\n";
print OFILE "CTAU   = $CTAU\n";

# For each r call subroutine
GetLimits( $rObsii, $Nii, $Njj, "Obs" );
GetLimits( $r95lii, $Nii, $Njj, "exp_m2s");
GetLimits( $r68lii, $Nii, $Njj, "exp_m1s" );
GetLimits( $rExpii, $Nii, $Njj, "exp" );
GetLimits( $r68hii, $Nii, $Njj, "exp_p1s" );
GetLimits( $r95hii, $Nii, $Njj, "exp_p2s" );

close(OFILE);

close(IFILE);
# Defined Subroutine
sub GetLimits() {
    my($ri, $Nsii, $Nsjj, $label) = @_;
    #$Num = scalar(@_);
    #print " Items passed \$Num = $Num\n"; 
    #print "values = $ri\t , $Nsii\t, $Nsjj\t, $label\n";   
    #$rj = $ri*($Nsjj/$Nsii); #Reverse of what I am doing to Not correct
    $rj = $ri*($Nsii/$Nsjj);
    print OFILE "${label} = $rj\n";
}
