%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  A projection onto the L1,infinity Ball
%
%  Authors: Ariadna Quattoni, Xavier Carreras, 
%           Michael Collins, Trevor Darrell
%
%  First release: July 2009
%
%  Revised September 2011: fixes important "bug" related to efficiency,
%   the projection was running innecessarily in time O(dm log dm + ddm) 
%   instead of O(dm log dm + dm) 
%

This code is a C implementation of a projection onto the L1,infinity
balls, proposed in the following paper:

Ariadna Quattoni, Xavier Carreras, Michael Collins, and Trevor
Darrell. An Efficient Projection for L1,Infinity Regularization, , In
Proceedings of ICML-2009.

If you use this code in your research, please cite the above paper. 

The projection itself is implemented in the file
projL1Inf.c. Additionally, we provide an interface for using the
projection in Matlab.

We also provide a test program to run the projection, both in C and in
Matlab. This test program randomly generates a matrix of a given
dimensionality, and projects the matrix into balls of smaller norms,
for several norm factors.

To run the C program, compile it with the following command:

$ gcc -O4 runprojection.c -o runprojection

Then you can execute it supplying the dimensions of the input matrix:

$ ./runprojection 5000 1000
Input Matrix A: norm=4994.835887
Projected Matrix B: factor=0.010000  norm=49.948359  time = 2.650058
Projected Matrix B: factor=0.050000  norm=249.741795  time = 2.602246
Projected Matrix B: factor=0.100000  norm=499.483589  time = 2.537135
Projected Matrix B: factor=0.200000  norm=998.967178  time = 2.484860
Projected Matrix B: factor=0.300000  norm=1498.450766  time = 2.478568
Projected Matrix B: factor=0.400000  norm=1997.934355  time = 2.471872
Projected Matrix B: factor=0.500000  norm=2497.417944  time = 2.462840

To run the projection in Matlab, you first need to compile the mexfile:

$ mex -output projL1Inf mex-projL1Inf.c 

Then, from Matlab, test the projection by running the script "runprojection", e.g.:

>> runprojection
Input Matrix A: norm=17155.049471
Projected Matrix B: factor=0.001000 norm=17.155050 time=2.535697
Projected Matrix B: factor=0.005000 norm=85.775247 time=2.553278
Projected Matrix B: factor=0.100000 norm=1715.504947 time=2.534807
Projected Matrix B: factor=0.200000 norm=3431.009894 time=2.560677
Projected Matrix B: factor=0.300000 norm=5146.514841 time=2.449561
Projected Matrix B: factor=0.400000 norm=6862.019788 time=2.454681
Projected Matrix B: factor=0.500000 norm=8577.524736 time=2.423409
