function [  ] = contourplot2_2( generation, nbest, style )
%CONTOUR Summary of this function goes here
%   Detailed explanation goes here

% config
popfile = sprintf('population%d.txt',generation);

% load data
pop = load(popfile);
load landscape2_2.txt;

% plot the fitness landscape
steps = 30;
xmax = 4.0;
ymax= 1.5;
x=linspace(xmax/steps, xmax, steps);
y=linspace(ymax/steps, ymax, steps);
z = landscape2_2;
contourf(y,x,z);

% overplot the citizens
hold on;
citx = pop(1:nbest,3)
city = pop(1:nbest,4)
plot(city, citx, style);
hold off

end

