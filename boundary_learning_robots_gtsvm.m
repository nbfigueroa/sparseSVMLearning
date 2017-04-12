%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load COLLISION/NON-COLLISION REGION DATA (20 deg resolution) %
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all; close all; clc;
load('Fender-Collision-Avoidance-Dataset.mat')
 
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Partition Dataset into Train+Validation/Test %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tt_ratio = 0.005;
[ X_train, y_train, X_valid, y_valid ] = split_data(X', y', tt_ratio );

X_train = X_train'; y_train = y_train';
X_valid  = X_valid';  y_valid  = y_valid';

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Find suitable range for rbf kernel %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%% Compute Element-wise pairwise distances %%%%%%
%%% Throughout ALL training points %%%
tic;
D = pdist(X_train, 'euclidean');
toc;
mean_D = mean(D(:))

%%% Within Positive Class training points %%%
tic;
D_wpos = pdist(X_train(y_train==1,:), 'euclidean');
toc;
mean_Dwpos = mean(D_wpos(:))
sum(D_wpos(:))

%%% Within Negative Class training points %%%
tic;
D_wneg = pdist(X_train(y_train==-1,:), 'euclidean');
toc;
mean_Dwneg = mean(D_wneg(:))
sum(D_wneg(:))

%%% Between Class training points %%%
tic;
D_betw = pdist2(X_train(y_train==-1,:), X_train(y_train==1,:), 'euclidean');
toc;
mean_Dbetw = mean(D_betw(:))

%% Visualize pairwise distances as Histogram
D_v = D(:);
hist_distances = 10;
tot_distances  = length(D_v);
figure('Color',[1 1 1])
histfit(D_v(1:hist_distances:end,:))
title('Collision Avoidance Features Pairwise Distances')
xlabel('L_2 Norm')
grid on 
axis tight

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%   Validate Range with "Optimal" RBF width size
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Supposed Analytical Equation to find an optimal Sigma
% Optimum can only be found if B_bar > W_bar
weight_btw_sep = 0.95;
[optSigma B_bar W_bar] = sigmaSelection(X_train,y_train','Analytical', weight_btw_sep)

% Not really good estimate! But can help with defining the ranges

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%     Learn Optimal C - SUPPORT VECTOR MACHINE with GTSVM %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Initialize Parameters/Prepare Data for GTSVM
% Hyper-parameters
C    = 2222
sigma = 0.733;
gamma = 1/(2*(sigma^2))
epsilon = 0.001;
Niter = 10000000;

% Convert Data to GTSVM Friendly
y_train_gt = y_train;
y_train_gt(y_train_gt == -1) = 0;
X_train_gt = sparse(X_train);

% Initialize GTSVM Model
context = gtsvm;
context.initialize( X_train_gt, y_train_gt, false, C, 'gaussian', ...
    gamma, 0, 0, true);

%% Learn SVM Model through GTSVM 
tic;
[primal, dual] = context.optimize(epsilon, Niter);
toc;
dual_gap = primal - dual
alphas = context.get_alphas;
NSVs = sum(alphas > 0)

%% Predict Labels for Training Data
tic;
decision_values = context.classify( X_train_gt );
toc;
y_est_gt = sign(decision_values);
[test_stats] = class_performance(y_train, y_est_gt);
fprintf('*Classifier Performance on Train set (%d points)* \n Acc: %1.5f, F-1: %1.5f, FPR: %1.5f, TPR: %1.5f \n', ...
    length(y_train), test_stats.ACC, test_stats.F1, test_stats.FPR, test_stats.TPR)


%% Train SVM Classifier learned from LIBSVM
tic;
[y_est] = svm_classifier(X_train, y_train,  [], model);
toc;

%% Model Stats
totSV = model.totalSV;
ratioSV = totSV/length(y_train);
posSV = model.nSV(1)/totSV;
negSV = model.nSV(2)/totSV;
boundSV = sum(abs(model.sv_coef) == options.C)/totSV;

fprintf('*SVM Model Statistic*\n Total SVs: %d, SV/M: %1.4f \n +1 SV : %1.4f, -1 SVs: %1.4f, Bounded SVs: %1.4f \n', ...
    totSV, ratioSV, posSV, negSV,  boundSV);

[train_stats] = class_performance(y_train,y_est);
fprintf('*Classifier Performance on Train set (%d points)* \n Acc: %1.5f, F-1: %1.5f, FPR: %1.5f, TPR: %1.5f \n', ...
    length(y_train), train_stats.ACC, train_stats.F1, train_stats.FPR, train_stats.TPR)

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%     Evaluate SVM performance on Testing Set   %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Extract a random testing points 
numSamples = 1000;
y_test_   = zeros(1,numSamples);
y_est_    = zeros(1,numSamples);

% Evaluate on Testing Points ==> TODO: Compute dec funct and grad
for i=1:numSamples
    idx_rand  = randperm(numSamples);
    
    X_test_    = X_test(idx_rand(i),:);
    y_test_(i) = y_test(idx_rand(i),:);
    
    % Test Learnt Model
    [y_est_(i)] = svm_classifier(X_test_, y_test_(i), [], model);
end

% Compute Classifier Test Stats
[test_stats] = class_performance(y_test_,y_est_); 
fprintf('*Classifier Performance on Test Set (%d points)* \n Acc: %1.5f, F-1: %1.5f, FPR: %1.5f, TPR: %1.5f \n', ...
    length(y_test_), test_stats.ACC, test_stats.F1, test_stats.FPR, test_stats.TPR)

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%   Grid-search on CV to find 'optimal' hyper-parameters for C-SVM with RBF %
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Set options for SVM Grid Search and Execute
clear options
options.svm_type   = 0;             % SVM Type (0:C-SVM, 1:nu-SVM)
options.limits_C   = [10^-1, 10^4]; % Limits of penalty C
options.limits_w   = [0.1, 2];      % Limits of kernel width \sigma
options.steps      = 10;            % Step of parameter grid 
options.K          = 10;            % K-fold CV parameter

%% Do Grid Search
[ ctest , ctrain , cranges ] = ml_grid_search_class( X_train, y_train, options );

%% Get CV statistics

% Extract parameter ranges
range_C  = cranges(1,:);
range_w  = cranges(2,:);

% Extract parameter ranges
stats = ml_get_cv_grid_states(ctest,ctrain);

%% Visualize Grid-Search Heatmap
cv_plot_options              = [];
cv_plot_options.title        = strcat('36-D, 13k (Fender) --Joint Positions f(q)-- C-SVM :: Grid Search with RBF');
cv_plot_options.param_names  = {'C', '\sigma'};
cv_plot_options.param_ranges = [range_C ; range_w];

if exist('hcv','var') && isvalid(hcv), delete(hcv);end
hcv = ml_plot_cv_grid_states(stats,cv_plot_options);

% Find 'optimal hyper-parameters'
[max_acc,ind] = max(stats.train.acc.mean(:));
[C_max, w_max] = ind2sub(size(stats.train.acc.mean),ind);
C_opt = range_C(C_max);
w_opt = range_w(w_max);
