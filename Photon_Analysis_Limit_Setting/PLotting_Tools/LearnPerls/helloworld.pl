#!/usr/bin perl

use strict;
use warnings; 
#print "Hello World ";

#my $undef = 0;
#print $undef;

#my $num= 400.458;
#print $num; 

# Matching strings
#my $string = "colourless green ideas sleep furiously";
#my @matches = $string =~ m/(\w+)\s+((\w+)\s+(\w+))\s+(\w+)\s+(\w+)/;
#print join ", ", map { "'".$_."'" } @matches;
# prints "'colourless', 'green ideas', 'green', 'ideas', 'sleep', 'fur

# How to change word in string
#my $string = "Good morning world";
#$string =~ s/world/Vietnam/;
#$string =~ s/world//;
#print $string; # "Good morning Vietnam"

my $string = "a tonne of feathers or a tonne of bricks";
while($string =~ m/(\w+)/g) {
  print "'".$1."'\n";
  }

#my @matches = $string =~ m/(\w+)/g;
#print join ", ", map { "'".$_."'" } @matches;

# Try once without /g.
 $string =~ s/[aeiou]/r/g;
 print $string; # "r tonne of feathers or a tonne of bricks"
#


#my $str1 = "5";
#my $str2 = "4";

#print $str1 . $str2;
#print $str1 + $str2;
#print $str1 eq $str2; 
#print $str1 ne $str2;

#my @array = (
# "print",
# "these",
# "strings",
# "out",
# "for",
# "me", 
# "Norbert",# trailing comma is okay
#);


#print $array[0]; # "print"
#print $array[1]; # "these"
#print $array[2]; # "strings"
#print $array[3]; # "out"
#print $array[4]; # "for"
#print $array[5]; # "me"
#print $array[6]; # re


#print $array[-1]; # "me"
#print $array[-2]; # "for"
#print $array[-3]; # "out"
#print $array[-4]; # "strings"
#print $array[-5]; # "these"
#print $array[-6]; # "print"
#print $array[-7]; # returns undef, prints "" and raises a warning
