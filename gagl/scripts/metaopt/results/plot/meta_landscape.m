function [  ] = meta_landscape(  )
% Plot the combined, normalized meta landscape

minfit = load('fitmat_min.dat');
callsfit = load('fitmat_calls.dat');
max_min = max(max(minfit));
max_calls = max(max(callsfit));
%contourf(minfit/max_min + callsfit/max_calls);
contourf(minfit/max_min);
%contourf(callsfit/max_calls);

end

