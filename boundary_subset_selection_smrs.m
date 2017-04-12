%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Sparse modeling of Robot Collision Data %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Number of Subsets to Reduce
K       = 1000;

% Size of each K-th Subsets
K_size  = round(length(y)/K);

% input the regularization parameter
alpha = 100; % typically alpha in [2,50]

% if desired to reduce data dimension by PCA enter the projection
% dimension r, else r = 0 for using the data without any projections
r = 0;

% report information about iterations
verbose = true;

X_train_smrs = [];
y_train_smrs = [];

for i=1:K
    
    % Extracting Subset Datapoints
    subset_start = (i-1)*K_size + 1;
    subset_end   = i*K_size;
    X_train_ = X(subset_start:subset_end,:);
    y_train_ = y(subset_start:subset_end,:);   
    fprintf('Data point indices: %d - %d\n', subset_start, subset_end);
    
    % Subset for positive and negative class
    Pos_class = X_train_(y_train_==1,:)'; Neg_class = X_train_(y_train_==-1,:)';    
    y_pos = y_train_(y_train_==1); y_neg = y_train_(y_train_==-1);
    
    % Find the Subset representatives via sparse modelling for positive class
    tic;
    [Pos_repInd,C] = smrs(Pos_class,alpha,r,verbose);
    toc;

    % Find the Subset representatives via sparse modelling for negative class
    tic;
    [Neg_repInd,C] = smrs(Neg_class,alpha,r,verbose);
    toc;

    % New Reduced Dataset for K-th Subset with SMRS 
    X_train_red = [Pos_class(:,Pos_repInd) Neg_class(:,Neg_repInd)]';
    y_train_red = [y_pos(Pos_repInd) ; y_neg(Neg_repInd)];        
    
    % Append to final Reduced Dataset
    X_train_smrs = [X_train_smrs; X_train_red];
    y_train_smrs = [y_train_smrs; y_train_red];
    
    fprintf('--- Current Subset Size: %d ---\n', length(y_train_smrs))
end

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Estimate optimal kernel width value      %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Compute Element-wise pairwise distances %%%%%%

randidx       = randperm(length(y_train_smrs));
train_samples = round(length(y_train_smrs)/2);
% train_samples = 6000;
X_train_smrs_ = X_train_smrs(randidx(1:train_samples),:); y_train_smrs_ = y_train_smrs(randidx(1:train_samples),:);


%% % Throughout ALL training points %%%
tic;
D = pdist(X_train_smrs_(1:2:end,:), 'euclidean');
toc;
mean_D = mean(D(:))

% Visualize pairwise distances as Histogram
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
[optSigma B_bar W_bar] = sigmaSelection(X_train_smrs_,y_train_smrs_','Analytical', weight_btw_sep)

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%     Learn Optimal C - SUPPORT VECTOR MACHINE  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Test C-SVM on Data (Assuming you ran CV first)
clear options
% Optimal Values from CV on Xk dataset
options.svm_type    = 0;      % 0: C-SVM, 1: nu-SVM
options.C           = 100;    % Misclassification Penalty
options.sigma       = 2; % radial basis function: exp(-gamma*|u-v|^2), gamma = 1/(2*sigma^2)

% Train SVM Classifier (12k+3D pts = 8s,....)
tic;
[y_est, model] = svm_classifier(X_train_smrs_, y_train_smrs_, options, []);
toc;

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%     Evaluate SVM performance on Testing Set   %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Extract a random testing points 
numSamples = 1000;
y_test_   = zeros(1,numSamples);
y_est_    = zeros(1,numSamples);

% Evaluate on Testing Points
for i=1:numSamples
    idx_rand  = randperm(numSamples);
    
    X_test_    = X(idx_rand(i),:);
    y_test_(i) = y(idx_rand(i),:);
    
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
options.svm_type   = 0;            % SVM Type (0:C-SVM, 1:nu-SVM)
options.limits_C   = [10^-1, 10^4]; % Limits of penalty C
options.limits_w   = [0.1, 2];     % Limits of kernel width \sigma
options.steps      = 10;           % Step of parameter grid 
options.K          = 5;            % K-fold CV parameter

%% Do Grid Search
[ ctest , ctrain , cranges ] = ml_grid_search_class( X_train_smrs_, y_train_smrs_, options );

%% Get CV statistics

% Extract parameter ranges
range_C  = cranges(1,:);
range_w  = cranges(2,:);

% Extract parameter ranges
stats = ml_get_cv_grid_states(ctest,ctrain);

%% Visualize Grid-Search Heatmap
cv_plot_options              = [];
cv_plot_options.title        = strcat('36-D, 12k --Joint Positions f(q)-- C-SVM :: Grid Search with RBF');
cv_plot_options.param_names  = {'C', '\sigma'};
cv_plot_options.param_ranges = [range_C ; range_w];

if exist('hcv','var') && isvalid(hcv), delete(hcv);end
hcv = ml_plot_cv_grid_states(stats,cv_plot_options);

% Find 'optimal hyper-parameters'
[max_acc,ind] = max(stats.train.acc.mean(:));
[C_max, w_max] = ind2sub(size(stats.train.acc.mean),ind);
C_opt = range_C(C_max);
w_opt = range_w(w_max);
