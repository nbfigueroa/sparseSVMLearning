%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Sparse modeling of Robot Collision Data %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Number of Subsets to Reduce
K       = 1000;

% Size of each K-th Subsets
K_size  = round(length(y)/K);

% Hyper-parameters for dissimlarity algorithm
dissimilarityType = 'Euc'; % type of pairwise dissimilarities
p = inf;                   % norm used for L1/Lp optimization in DS3
regularized = true;        % true: regularized version, false: constrained version
alpha       = 0.01;         % regularizer coefficient
verbose     = true;        % true/false: show/hide optimization steps

% Optimization options
options.mu = 1 * 10^-1;
options.maxIter = 1000;
options.errThr = 1 * 10^-7;

X_train_diss = [];
y_train_diss = [];

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
    
    %%% Find the Subset representatives for positive class %%
    % compute Dissimilarity Matrix
    tic;
    D = computeDissimilarity(dissimilarityType, Pos_class);
    D = D ./ max(max(D));
    CFD = ones(size(D,1),1);
    [rho_min, rho_max] = computeRegularizer(D,p);
    options.verbose = verbose;
    options.rho = alpha * rho_max; % regularization parameter
    options.cfd = CFD;
    Z = ds3solver_regularized(D,p,options);        
    % find representatives and clustering
    Pos_repInd = findRepresentatives(Z);
    toc;

    %%% Find the Subset representatives for negative class %%
    % compute Dissimilarity Matrix
    tic;
    D = computeDissimilarity(dissimilarityType, Neg_class);
    D = D ./ max(max(D));
    CFD = ones(size(D,1),1);
    [rho_min, rho_max] = computeRegularizer(D,p);
    options.verbose = verbose;
    options.rho = alpha * rho_max; % regularization parameter
    options.cfd = CFD;
    Z = ds3solver_regularized(D,p,options);        
    % find representatives and clustering
    Neg_repInd = findRepresentatives(Z);
    toc;

    % New Reduced Dataset for K-th Subset with SMRS 
    X_train_red = [Pos_class(:,Pos_repInd) Neg_class(:,Neg_repInd)]';
    y_train_red = [y_pos(Pos_repInd) ; y_neg(Neg_repInd)];        
    
    % Append to final Reduced Dataset
    X_train_diss = [X_train_diss; X_train_red];
    y_train_diss = [y_train_diss; y_train_red];
    
    fprintf('--- Current Subset Size: %d ---\n', length(y_train_diss))
end


%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%     Learn Optimal C - SUPPORT VECTOR MACHINE  %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Test C-SVM on Data (Assuming you ran CV first)
clear options
% Optimal Values from CV on Xk dataset
options.svm_type    = 0;      % 0: C-SVM, 1: nu-SVM
options.C           = 1000;    % Misclassification Penalty
options.sigma       = 0.5036; % radial basis function: exp(-gamma*|u-v|^2), gamma = 1/(2*sigma^2)

% Train SVM Classifier (12k+3D pts = 8s,....)
tic;
[y_est, model] = svm_classifier(X_train_smrs, y_train_smrs, options, []);
toc;


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
    
    X_test_    = X_train(idx_rand(i),:);
    y_test_(i) = y_train(idx_rand(i),:);
    
    % Test Learnt Model
    [y_est_(i)] = svm_classifier(X_test_, y_test_(i), [], model);
end

% Compute Classifier Test Stats
[test_stats] = class_performance(y_test_,y_est_); 
fprintf('*Classifier Performance on Test Set (%d points)* \n Acc: %1.5f, F-1: %1.5f, FPR: %1.5f, TPR: %1.5f \n', ...
    length(y_test_), test_stats.ACC, test_stats.F1, test_stats.FPR, test_stats.TPR)
