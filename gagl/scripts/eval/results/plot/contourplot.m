function [  ] = contourplot( generation, nbest, style )
%CONTOUR Summary of this function goes here
%   Detailed explanation goes here

% config
popfile = sprintf('population%d.txt',generation);

% load data
pop = load(popfile);
load landscape.txt;

% plot the fitness landscape
steps = 30;
xmax = 0.16;
ymax= 0.047;
x=linspace(xmax/steps, xmax, steps);
y=linspace(ymax/steps, ymax, steps);
z = landscape;
contourf(x,y,z);

% overplot the citizens
hold on;
citx = pop(1:nbest,1);
city = pop(1:nbest,2);
plot(citx, city, style);
hold off

end

