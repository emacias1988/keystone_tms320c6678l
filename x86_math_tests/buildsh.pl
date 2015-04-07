#!/usr/bin/perl
#******************************************************************************
#
# Script to build gcc 
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

my $cmd = 'gcc -o math_tests.o';
$cmd .= ' main.c';

$cmd .= ' atan.c';
$cmd .= ' atan2.c';
$cmd .= ' cmp.c';
$cmd .= ' cos.c';
$cmd .= ' div.c';
$cmd .= ' exp.c';
$cmd .= ' exp2.c';
$cmd .= ' exp10.c';
$cmd .= ' log.c';
$cmd .= ' log2.c';
$cmd .= ' log10.c';
$cmd .= ' pow.c';
$cmd .= ' recip.c';
$cmd .= ' rsqrt.c';
$cmd .= ' sin.c';
$cmd .= ' sqrt.c';

$cmd .= ' -lm';	#Math Link

$cmd .= ' -lrt';
system($cmd);


