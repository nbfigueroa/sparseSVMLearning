% This function augments the dissimilarity with weights that reflect the 
% likelihood of elements in target set being outliers
% D: dissimilarity matrix
% beta: scaling of outlier weights
% sigmaV: parameter for setting outlier weights
%--------------------------------------------------------------------------
% Copyright @ Ehsan Elhamifar, 2015
%--------------------------------------------------------------------------

function D = augmentD(D,beta,sigmaV)

if (nargin < 3)
    sigmaV = 1;
end

dmax = max(max(D)) + 1;
w = beta .* exp(-min(D + dmax.*(D == 0)) ./ sigmaV);
D = [D;w];