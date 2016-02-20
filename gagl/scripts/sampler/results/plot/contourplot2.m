function [  ] = contourplot2( nlevels, filename )
%CONTOUR Summary of this function goes here
%   Detailed explanation goes here

% config
%popfile = sprintf('population%d.txt',generation);

% load data
%pop = load(popfile);
%load distfit.txt;
%load timefit.txt;
%load numfit.txt;
%load typefit.txt;
z = load(filename);

% plot the fitness landscape
steps = 30;
xmax = 10.5;
ymax= 5.0;
x=linspace(xmax/steps, xmax, steps);
y=linspace(ymax/steps, ymax, steps);
%subplot(2,2,1);contourf(y,x,distfit, nlevels); colorbar;
%subplot(2,2,2);contourf(y,x,timefit, nlevels); colorbar;
%subplot(2,2,3);contourf(y,x,numfit, nlevels); colorbar;
%subplot(2,2,4);contourf(y,x,typefit, nlevels); colorbar;
contourf(y,x,z, nlevels); colorbar;


% overplot the citizens
%hold on;
%citx = pop(1:nbest,3)
%city = pop(1:nbest,4)
%plot(city, citx, style);
%hold off

end

