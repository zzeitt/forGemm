%
% Clear all variables and close all graphs
%

clear all
close all
set(groot, 'defaultLineMarkerSize',7);
set(0, 'defaultLineLineWidth', 1.8);
%%%%%%%%%%%%%%%%%%%%%%% old %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
output_MMult0
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'square:;MMult0;' );
hold on;

output_MMult1
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '+:;MMult1;' );
hold on;

output_MMult2
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '*:;MMult2;' );
hold on;

output_MMult_1x4_3
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '.:;MMult_1x4_3;' );
hold on;

output_MMult_1x4_4
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'x:;MMult_1x4_4;' );
hold on;

output_MMult_1x4_5
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'o:;MMult_1x4_5;' );
hold on;

output_MMult_1x4_6
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'square:;MMult_1x4_6;' );
hold on;

output_MMult_1x4_7
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '^:;MMult_1x4_7;' );
hold on;

output_MMult_1x4_8
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'pentagram:;MMult_1x4_8;' );
hold on;

output_MMult_1x4_9
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '<:;MMult_1x4_9;' );
hold on;

output_MMult_4x4_9
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '>-;MMult_4x4_9;' );
hold on;

output_MMult_4x4_10
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '*-;MMult_4x4_10;' );
hold on;

output_MMult_4x4_11
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'square-;MMult_4x4_11;' );
hold on;

output_MMult_4x4_12
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'x-;MMult_4x4_12;' );
hold on;

output_MMult_4x4_13
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), 'o-;MMult_4x4_13;' );
hold on;

output_MMult_4x4_14
plot( MY_MMult( :,1 ), MY_MMult( :,3 ), '^-;MMult_4x4_14;' );
hold on;

title( 'MMult Optimization' );
xlabel( 'm = n = k' );
ylabel( 'ABS' );
legend('Location', 'bestoutside');
copied_legend = findobj(gcf(),"type","axes","Tag","legend");
set(copied_legend, "FontSize", 8);
print( 'compare_all_diffs', '-dpng' );
