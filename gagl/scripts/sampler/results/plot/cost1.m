function [  ] = cost1(  )
% plot the fitness landscape for varying first and
% second parameter. 3rd and 4th parameter are set
% to reference values

dist = load('distfit.txt');
time = load('timefit.txt');
num = load('numfit.txt');
z = dist + time + num.^3;

steps = 30;
xmax = 0.16;
ymax= 0.047;
x=linspace(xmax/steps, xmax, steps);
y=linspace(ymax/steps, ymax, steps);
contourf(y,x,z,20); colorbar;
xlabel('ys');
ylabel('xs'); 

end

