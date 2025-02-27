%
% Clear all variables and close all graphs
%

clear all
close all

%
% Get max_gflops from /proc/cpuinfo by reading the parameters
% set in file proc_parameters.m
%

proc_parameters

max_gflops = nflops_per_cycle * nprocessors * GHz_of_processor;

%
% Read in the first data set and plot it.
%

%%%%%%%%%%%%%%%%%%%%%%% old %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
output_old

version_old = version;

plot( MY_MMult( :,1 ), MY_MMult( :,2 ), 'bo-.;OLD;' );
last = size( MY_MMult, 1 );

hold on

axis( [ 0 MY_MMult( last,1 ) 0 max_gflops ] );

xlabel( 'm = n = k' );
ylabel( 'GFLOPS/sec.' );

%%%%%%%%%%%%%%%%%%%%%%% prev %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
output_prev

version_prev = version

plot( MY_MMult( :,1 ), MY_MMult( :,2 ), 'g<-.;PREV;' );

hold on

%%%%%%%%%%%%%%%%%%%%%%% new %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
output_new

version_new = version

plot( MY_MMult( :,1 ), MY_MMult( :,2 ), 'r-*;NEW;' );

title_string = sprintf(
    "OLD = %s, PREV = %s, NEW = %s", 
    version_old, version_prev, version_new);
title( title_string );

filename = sprintf( "compare_%s_%s", version_prev, version_new );

print( filename, '-dpng' );
