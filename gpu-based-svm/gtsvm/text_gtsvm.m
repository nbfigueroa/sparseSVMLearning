%%%%%%%%%%%%%%%%%
%  Test GTSVM   %
%%%%%%%%%%%%%%%%%
%% Load Dataset
clear all;close all;clc
% load('./adult/adult.mat')
load('./cov/cov1.mat')

% Convert training data for LibSVM
y_train = double(training_labels);
y_train(y_train==0) = -1;
X_train = full(training_vectors);

% Convert testing data for LibSVM
X_test = full(testing_vectors);
y_test = double(testing_labels);
y_test(y_test==0) = -1;


%% Run GTSVM Optimize
C     = 3;
gamma = 1;
epsilon = 0.001;
Niter = 10000000;

context = gtsvm;
context.initialize( X_train, y_train, false, C, 'gaussian', ...
    gamma, 0, 0, true);
tic;
[primal, dual] = context.optimize(epsilon, Niter);
toc;

dual_gap = primal - dual
alphas = context.get_alphas;
NSVs = sum(alphas > 0)

%% Test GTSVM Model Performance
tic;
decision_values = context.classify( X_train );
toc;
%%
y_est_gt = sign(decision_values);
[test_stats] = class_performance(y_train,y_est_gt);
fprintf('*Classifier Performance on Train set (%d points)* \n Acc: %1.5f, F-1: %1.5f, FPR: %1.5f, TPR: %1.5f \n', ...
    length(y_train), test_stats.ACC, test_stats.F1, test_stats.FPR, test_stats.TPR)

%% Test C-SVM on Data (Assuming you ran CV first)
clear options
% Optimal Values from CV on Xk dataset
options.svm_type    = 0;      % 0: C-SVM, 1: nu-SVM
options.C           = 1;      % Misclassification Penalty
options.sigma       = 3.1623; % radial basis function: exp(-gamma*|u-v|^2), gamma = 1/(2*sigma^2)

% Train SVM Classifier
tic;
[y_est, model] = svm_classifier(X_train, y_train, options, []);
toc;

%% Test GTSVM Model Performance
[test_stats] = class_performance(y_train,y_est);
fprintf('*Classifier Performance on Train set (%d points)* \n Acc: %1.5f, F-1: %1.5f, FPR: %1.5f, TPR: %1.5f \n', ...
    length(y_train), test_stats.ACC, test_stats.F1, test_stats.FPR, test_stats.TPR)