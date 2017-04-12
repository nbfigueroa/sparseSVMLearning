setenv( 'LIB', getenv( 'CUDA_LIB_PATH' ) );
mex -O -v -largeArrayDims -I../lib -L../Solution/Release -DWINDOWS gtsvm_mex.c -lgtsvm -lcudart
