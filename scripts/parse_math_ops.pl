#!/usr/bin/perl
#******************************************************************************
#
# parse_math_ops.pl - Script to parse the math operation benchmarks.
#
# Copyright (c) 2015 Texas Instruments Incorporated.  All rights reserved.
# TI Information - Selective Disclosure
#
#******************************************************************************

use strict;

use POSIX;
use File::Basename;
use File::Path;
use File::Spec;
use Cwd;
use Getopt::Std;

my $basedir = dirname(${0});

our($opt_d, $opt_f, $opt_g, $opt_h);

#
# set default config file name
#
$opt_f = "config.in";

#
# parse command line switches
#
getopts("f:dgh");

if(defined($opt_h))
{
    print "Usage: parse_math_ops.pl [-g]\n";
    print "Generate parsed output for math operation benchmarks.\n";
    print "\n";
	print "  -d  Print debug information while parsing\n";
    print "  -g  Generate all parsed output for math operations\n";
    print "  -f  Specify configuration file name (default config.in)\n";
    print "  -h  Print the help menu text\n";
    print "\n";
    exit 1;
}

#******************************************************************************
#
# Math functions Hash 
#   Organization:
#	Math_Function @ Size @ Cycles
#
#******************************************************************************
my %math_func_db;

if(defined($opt_g))
{
	if(defined($opt_d))
	{
		print "Generating results...\n";
	}
	
	open(FILE_HANDLER, "< $opt_f") || die "Failed opening $opt_f";
	
	if(defined($opt_d))
	{
		print "Reading file $opt_f.\n";
	}
	
	while(<FILE_HANDLER>)
	{
		next if /^\s*($|#)/;
		
		#
        # Get rid of whatever line end characters the input file has (DOS or
        # UNIX style).
        #
        s/[\n\r]+$//;
		
		#
		# Check for MATHLIB_TESTS and *_i 
		#
		if(/^MATHLIB_TEST/ && /_i/)
		{
			my @data_values;
			
			#
			# Split the line based on space
			#
			@data_values = split(' ', $_);
			
			my $size_transfer;
			
			#Store in $1 the value between parentheses
			$data_values[2] =~ /\((.*?)\)/g;
			
			$size_transfer = $1;
			
			#
			# Create Hash
			#
			$math_func_db{$data_values[1]}{$size_transfer}{$data_values[5]} = 1;
		}
		
		#print "$_\n";
	}
	
	close(FILE_HANDLER);
	
	#
	#	Print Table Header
	#
	printf "%10s\t","Function";
	foreach my $func_name (sort keys %math_func_db) 
	{
		foreach my $data_size (sort {$a <=> $b} keys %{ $math_func_db{$func_name}})
		{
			printf "%15s\t",$data_size." words";
		}	
		print("\n");
		last;
	}
	
	#
	#	Print Table Information
	#
	foreach my $func_name (sort keys %math_func_db) 
	{
		printf "%10s\t",$func_name;
		foreach my $data_size (sort {$a <=> $b} keys %{ $math_func_db{$func_name} })
		{
			foreach my $latency (keys %{ $math_func_db{$func_name}{$data_size} })
			{
				printf "%15s\t",$latency;
			}
		}	
		print "\n";
	}
}
