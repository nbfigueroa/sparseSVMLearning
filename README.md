# sparseSVMLearning
Multiple implementations for fast/efficient/sparse SVM learning. Also some data sparse coding toolboxes. 

TODO: Fill-in description.

### Soft-Margin SVM learning
To learn a standard SVM with the typical SMO-like optimization algorithm from [libSVM](https://www.csie.ntu.edu.tw/~cjlin/libsvm/) and do grid search with k-fold cross-validation, please download the [ML_toolbox](https://github.com/epfl-lasa/ML_toolbox) and add it to your MATLAB search path. The scripts to test this are:
```
boundary_learning_2d.m
boundary_learning_robots.m
```
contains the necessary code snippets to find the optimal sets of hyper-parameters for a 2D toy problem and for the real Big-Data self-collision avoidance dataset, which have to be downloaded found [here](https://www.dropbox.com/s/y0xi36i4ezwoqfo/data_mat.zip?dl=0)

This implementation solves the dual QP problem with a type of SMO optimization, which yields accurate results but may take really long for large number of datapoints. The optimization is implemented in C++ but it has MATLAB mex interface to run them from Matlab. In my experience, I tried learning an SVM from 270k points and it took a couple of days to solve on a (not so old) PC.

### GPU-Accelerated Soft-Margin SVM learning
This fast implementation of soft-margin SVM learning relies on exploiting the computational power of GPU's. Given the vcorrect NVidia card, the algorithm is extremely fast! The scripts to test this are:
```
boundary_learning_gtsvm.m
```
NOTE: In order to achieve this efficiency the algorithm does some data clustering to reduce the number of sample points. If your data is completely overlapping, this might not given the same performance as the standard Soft-Margin SVM learning algorithm, so be careful! For the 2D dataset example it works very nicely, however, for our self-collision avoidance it just cannot reach the expected performance (in terms of classification rates). In other words, it's fast but less accurate.

### References and Toolboxes
- The standard library for SVM learning is libSVM: https://www.csie.ntu.edu.tw/~cjlin/libsvm/
- [ML_toolbox](https://github.com/epfl-lasa/ML_toolbox) Matlab toolbox used for teaching machine learning techniques at EPFL by N. Figueroa among others.
