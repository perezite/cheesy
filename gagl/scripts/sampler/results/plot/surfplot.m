function [  ] = surfplot( nlevels, filename, tr )

% load data
z = load(filename);
maxfit = max(max(z))
%fitness = fitness/maxfit;
z = min(z, tr);         % floor all the z-values to tr

% plot the fitness landscape
steps = 30;
xmax = 0.16;
ymax= 0.047;
x=linspace(xmax/steps, xmax, steps);
y=linspace(ymax/steps, ymax, steps);
surf(y,x,z); colorbar;

end


