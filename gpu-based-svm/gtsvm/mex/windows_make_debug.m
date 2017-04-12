setenv( 'LIB', getenv( 'CUDA_LIB_PATH' ) );
mex -g -v -largeArrayDims -I../lib -L../Solution/Debug -DWINDOWS gtsvm_mex.c -lgtsvm -lcudart
