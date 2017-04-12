/*
	Copyright (C) 2011  Andrew Cotter

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
	\file gtsvm_mex.c
*/




#include <mex.h>
#include <matrix.h>

#include <gtsvm.h>

#include <string.h>




/*============================================================================
	Windows workaround for strcasecmp/stricmp
============================================================================*/


#ifdef WINDOWS
#define strcasecmp stricmp
#endif    /* WINDOWS */




/*============================================================================
	GetType function
============================================================================*/


static GTSVM_Type const GetType( mxArray const* array ) {

	GTSVM_Type result = GTSVM_TYPE_UNKNOWN;

	if ( mxIsLogical( array ) )
		result = GTSVM_TYPE_BOOL;
	else if ( mxIsNumeric( array ) && ( ! mxIsComplex( array ) ) ) {

		if ( mxIsDouble( array ) )
			result = GTSVM_TYPE_DOUBLE;
		else if ( mxIsSingle( array ) )
			result = GTSVM_TYPE_FLOAT;
		else if ( mxIsInt8( array ) )
			result = GTSVM_TYPE_INT8;
		else if ( mxIsInt16( array ) )
			result = GTSVM_TYPE_INT16;
		else if ( mxIsInt32( array ) )
			result = GTSVM_TYPE_INT32;
		else if ( mxIsInt64( array ) )
			result = GTSVM_TYPE_INT64;
		else if ( mxIsUint8( array ) )
			result = GTSVM_TYPE_UINT8;
		else if ( mxIsUint16( array ) )
			result = GTSVM_TYPE_UINT16;
		else if ( mxIsUint32( array ) )
			result = GTSVM_TYPE_UINT32;
		else if ( mxIsUint64( array ) )
			result = GTSVM_TYPE_UINT64;
	}

	return result;
}




/*============================================================================
	GetDouble function
============================================================================*/


static double const GetDouble( mxArray const* array ) {

	double result = 0;

	if ( mxGetNumberOfElements( array ) != 1 )
		mexErrMsgTxt( "Expected only one element" );

	switch( GetType( array ) ) {
		case GTSVM_TYPE_BOOL:   result = ( *( bool const* )mxGetLogicals( array ) ? 1.0f : 0.0f ); break;
		case GTSVM_TYPE_FLOAT:  result = ( double )( *( float const*    )mxGetPr( array ) ); break;
		case GTSVM_TYPE_DOUBLE: result = ( double )( *( double const*   )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT8:   result = ( double )( *( INT8_T const*   )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT16:  result = ( double )( *( INT16_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT32:  result = ( double )( *( INT32_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT64:  result = ( double )( *( INT64_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT8:  result = ( double )( *( UINT8_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT16: result = ( double )( *( UINT16_T const* )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT32: result = ( double )( *( UINT32_T const* )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT64: result = ( double )( *( UINT64_T const* )mxGetPr( array ) ); break;
		default: mexErrMsgTxt( "Unknown type" );
	}

	return result;
}




/*============================================================================
	GetInt function
============================================================================*/


static int const GetInt( mxArray const* array ) {

	int result = 0;

	if ( mxGetNumberOfElements( array ) != 1 )
		mexErrMsgTxt( "Expected only one element" );

	switch( GetType( array ) ) {
		case GTSVM_TYPE_BOOL:   result = ( *( bool const* )mxGetLogicals( array ) ? 1 : 0 ); break;
		case GTSVM_TYPE_FLOAT:  result = ( int )( *( float const*    )mxGetPr( array ) ); break;
		case GTSVM_TYPE_DOUBLE: result = ( int )( *( double const*   )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT8:   result = ( int )( *( INT8_T const*   )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT16:  result = ( int )( *( INT16_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT32:  result = ( int )( *( INT32_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT64:  result = ( int )( *( INT64_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT8:  result = ( int )( *( UINT8_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT16: result = ( int )( *( UINT16_T const* )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT32: result = ( int )( *( UINT32_T const* )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT64: result = ( int )( *( UINT64_T const* )mxGetPr( array ) ); break;
		default: mexErrMsgTxt( "Unknown type" );
	}

	return result;
}




/*============================================================================
	GetBool function
============================================================================*/


static bool const GetBool( mxArray const* array ) {

	bool result = 0;

	if ( mxGetNumberOfElements( array ) != 1 )
		mexErrMsgTxt( "Expected only one element" );

	switch( GetType( array ) ) {
		case GTSVM_TYPE_BOOL:   result = *( bool const* )mxGetLogicals( array ); break;
		case GTSVM_TYPE_FLOAT:  result = ( *( float const*    )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_DOUBLE: result = ( *( double const*   )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_INT8:   result = ( *( INT8_T const*   )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_INT16:  result = ( *( INT16_T const*  )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_INT32:  result = ( *( INT32_T const*  )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_INT64:  result = ( *( INT64_T const*  )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_UINT8:  result = ( *( UINT8_T const*  )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_UINT16: result = ( *( UINT16_T const* )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_UINT32: result = ( *( UINT32_T const* )mxGetPr( array ) > 0 ); break;
		case GTSVM_TYPE_UINT64: result = ( *( UINT64_T const* )mxGetPr( array ) > 0 ); break;
		default: mexErrMsgTxt( "Unknown type" );
	}

	return result;
}




/*============================================================================
	GetContext function
============================================================================*/


static GTSVM_Context const GetContext( mxArray const* array ) {

	GTSVM_Context result = 0;

	if ( mxGetNumberOfElements( array ) != 1 )
		mexErrMsgTxt( "Expected only one element" );

	switch( GetType( array ) ) {
		case GTSVM_TYPE_BOOL:   result = ( GTSVM_Context )( *( bool const* )mxGetLogicals( array ) ? 1 : 0 ); break;
		case GTSVM_TYPE_FLOAT:  result = ( GTSVM_Context )( *( float const*    )mxGetPr( array ) ); break;
		case GTSVM_TYPE_DOUBLE: result = ( GTSVM_Context )( *( double const*   )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT8:   result = ( GTSVM_Context )( *( INT8_T const*   )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT16:  result = ( GTSVM_Context )( *( INT16_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT32:  result = ( GTSVM_Context )( *( INT32_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_INT64:  result = ( GTSVM_Context )( *( INT64_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT8:  result = ( GTSVM_Context )( *( UINT8_T const*  )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT16: result = ( GTSVM_Context )( *( UINT16_T const* )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT32: result = ( GTSVM_Context )( *( UINT32_T const* )mxGetPr( array ) ); break;
		case GTSVM_TYPE_UINT64: result = ( GTSVM_Context )( *( UINT64_T const* )mxGetPr( array ) ); break;
		default: mexErrMsgTxt( "Unknown type" );
	}

	return result;
}




/*============================================================================
	GetStringCopy function
============================================================================*/


static char* const GetStringCopy( mxArray const* array ) {

	char* result = NULL;

	if ( ! mxIsChar( array ) )
		mexErrMsgTxt( "Expected a string" );

	{	unsigned int const size = mxGetNumberOfElements( array );
		mxChar const* const string = mxGetChars( array );

		mxChar const* ii = string;
		mxChar const* iiEnd = ii + size;

		char* jj = NULL;

		result = mxCalloc( size + 1, sizeof( char ) );

		jj = result;
		for ( ; ii != iiEnd; ++ii, ++jj )
			*jj = ( char )*ii;
		*jj = '\0';
	}

	return result;
}




/*============================================================================
	mexFunction function
============================================================================*/


void mexFunction( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {

	if ( nrhs < 1 )
		mexErrMsgTxt( "Function not specified" );

	switch( GetInt( prhs[ 0 ] ) ) {

		case 0: {

			if ( nrhs != 1 )
				mexErrMsgTxt( "create takes no parameters" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "create returns one result ( context )" );

			{	GTSVM_Context context;
				if ( GTSVM_Create( &context ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxUINT32_CLASS, false );    /* **TODO **FIXME **HACK: GTSVM_Context is supposed to be opaque (although it is true that it is a 32-bit unsigned int) */
				*( GTSVM_Context* )mxGetPr( plhs[ 0 ] ) = context;
			}

			break;
		}

		case 1: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "destroy takes one parameter ( context )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "destroy returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				if ( GTSVM_Destroy( context ) )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 2: {

			if ( ( nrhs < 10 ) || ( nrhs > 13 ) )
				mexErrMsgTxt( "initialize takes 9 to 12 parameters ( context, vectors, labels, multiclass, regularization, kernel, parameter1, parameter2, parameter3, biased = false, small_clusters = false, active_clusters = 64 )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "initialize returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int const rows    = mxGetM( prhs[ 2 ] );
				unsigned int const columns = mxGetN( prhs[ 2 ] );

				bool const multiclass = GetBool( prhs[ 4 ] );
				float const regularization = ( float )GetDouble( prhs[ 5 ] );

				GTSVM_Kernel kernel = GTSVM_KERNEL_UNKNOWN;
				char* const string = GetStringCopy( prhs[ 6 ] );

				float const kernelParameter1 = ( float )GetDouble( prhs[ 7 ] );
				float const kernelParameter2 = ( float )GetDouble( prhs[ 8 ] );
				float const kernelParameter3 = ( float )GetDouble( prhs[ 9 ] );

				bool biased = false;
				bool smallClusters = false;
				int activeClusters = 64;

				if ( ( mxGetM( prhs[ 3 ] ) != rows ) || ( mxGetN( prhs[ 3 ] ) != 1 ) )
					mexErrMsgTxt( "Expected vectors to be MxN, and labels to be Mx1" );

				if ( strcasecmp( string, "gaussian" ) == 0 )
					kernel = GTSVM_KERNEL_GAUSSIAN;
				else if ( strcasecmp( string, "polynomial" ) == 0 )
					kernel = GTSVM_KERNEL_POLYNOMIAL;
				else if ( strcasecmp( string, "sigmoid" ) == 0 )
					kernel = GTSVM_KERNEL_SIGMOID;
				mxFree( string );
				if ( kernel == GTSVM_KERNEL_UNKNOWN )
					mexErrMsgTxt( "Expected kernel to be one of \"gaussian\", \"polynomial\" or \"sigmoid\"" );

				if ( nrhs >= 11 )
					biased = GetBool( prhs[ 10 ] );
				if ( nrhs >= 12 )
					smallClusters = GetBool( prhs[ 11 ] );
				if ( nrhs >= 13 )
					activeClusters = GetInt( prhs[ 12 ] );
				if ( activeClusters <= 0 )
					mexErrMsgTxt( "Expected active_clusters to be strictly positive" );

				if ( mxIsSparse( prhs[ 3 ] ) )
					mexErrMsgTxt( "Expected labels to be dense" );
				if ( mxIsSparse( prhs[ 2 ] ) ) {

					if (
						GTSVM_InitializeSparse(
							context,
							mxGetPr( prhs[ 2 ] ),
							mxGetIr( prhs[ 2 ] ),    /* indices of nonzero elements */
							mxGetJc( prhs[ 2 ] ),    /* indices of starting/ending elements of each row */
							GetType( prhs[ 2 ] ),
							mxGetPr( prhs[ 3 ] ),
							GetType( prhs[ 3 ] ),
							rows,
							columns,
							true,    /* column-major order */
							multiclass,
							regularization,
							kernel,
							kernelParameter1,
							kernelParameter2,
							kernelParameter3,
							biased,
							smallClusters,
							activeClusters
						)
					)
					{
						mexErrMsgTxt( GTSVM_Error() );
					}
				}
				else {

					if (
						GTSVM_InitializeDense(
							context,
							mxGetPr( prhs[ 2 ] ),
							GetType( prhs[ 2 ] ),
							mxGetPr( prhs[ 3 ] ),
							GetType( prhs[ 3 ] ),
							rows,
							columns,
							true,    /* column-major order */
							multiclass,
							regularization,
							kernel,
							kernelParameter1,
							kernelParameter2,
							kernelParameter3,
							biased,
							smallClusters,
							activeClusters
						)
					)
					{
						mexErrMsgTxt( GTSVM_Error() );
					}
				}
			}

			break;
		}

		case 3: {

			if ( ( nrhs < 3 ) || ( nrhs > 5 ) )
				mexErrMsgTxt( "load takes 2 to 4 parameters ( context, filename, small_clusters = false, active_clusters = 64 )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "load returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				bool error = false;

				char* const string = GetStringCopy( prhs[ 2 ] );
				bool smallClusters = false;
				int activeClusters = 64;

				if ( nrhs >= 4 )
					smallClusters = GetBool( prhs[ 3 ] );
				if ( nrhs >= 5 )
					activeClusters = GetInt( prhs[ 4 ] );
				if ( activeClusters <= 0 )
					mexErrMsgTxt( "Expected active_clusters to be strictly positive" );

				error = GTSVM_Load(
					context,
					string,
					smallClusters,
					activeClusters
				);
				mxFree( string );

				if ( error )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 4: {

			if ( nrhs != 3 )
				mexErrMsgTxt( "save takes two parameters ( context, filename )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "save returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				bool error = false;

				char* const string = GetStringCopy( prhs[ 2 ] );

				error = GTSVM_Save( context, string );
				mxFree( string );

				if ( error )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 5: {

			if ( ( nrhs < 2 ) || ( nrhs > 4 ) )
				mexErrMsgTxt( "shrink takes 1 to 3 parameters ( context, small_clusters = false, active_clusters = 64 )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "shrink returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				bool error = false;

				bool smallClusters = false;
				int activeClusters = 64;

				if ( nrhs >= 3 )
					smallClusters = GetBool( prhs[ 2 ] );
				if ( nrhs >= 4 )
					activeClusters = GetInt( prhs[ 3 ] );
				if ( activeClusters <= 0 )
					mexErrMsgTxt( "Expected active_clusters to be strictly positive" );

				error = GTSVM_Shrink(
					context,
					smallClusters,
					activeClusters
				);

				if ( error )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 6: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "deinitialize_device takes one parameter ( context )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "deinitialize_device returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				if ( GTSVM_DeinitializeDevice( context ) )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 7: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "deinitialize takes one parameter ( context )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "deinitialize returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				if ( GTSVM_Deinitialize( context ) )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 8: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_regularization takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_regularization returns one result ( regularization )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				float regularization;
				if ( GTSVM_GetRegularization( context, &regularization ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxSINGLE_CLASS, false );
				*( float* )mxGetPr( plhs[ 0 ] ) = regularization;
			}

			break;
		}

		case 9: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_kernel takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_kernel returns one result ( kernel )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				GTSVM_Kernel kernel;
				if ( GTSVM_GetKernel( context, &kernel ) )
					mexErrMsgTxt( GTSVM_Error() );

				switch( kernel ) {
					case GTSVM_KERNEL_GAUSSIAN:   { plhs[ 0 ] = mxCreateString( "gaussian"   ); break; }
					case GTSVM_KERNEL_POLYNOMIAL: { plhs[ 0 ] = mxCreateString( "polynomial" ); break; }
					case GTSVM_KERNEL_SIGMOID:    { plhs[ 0 ] = mxCreateString( "sigmoid"    ); break; }
					default: mexErrMsgTxt( "Unknown kernel" );
				}
			}

			break;
		}

		case 10: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_kernel_parameter1 takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_kernel_parameter1 returns one result ( parameter1 )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				float kernelParameter1;
				if ( GTSVM_GetKernelParameter1( context, &kernelParameter1 ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxSINGLE_CLASS, false );
				*( float* )mxGetPr( plhs[ 0 ] ) = kernelParameter1;
			}

			break;
		}

		case 11: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_kernel_parameter2 takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_kernel_parameter2 returns one result ( parameter2 )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				float kernelParameter2;
				if ( GTSVM_GetKernelParameter2( context, &kernelParameter2 ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxSINGLE_CLASS, false );
				*( float* )mxGetPr( plhs[ 0 ] ) = kernelParameter2;
			}

			break;
		}

		case 12: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_kernel_parameter3 takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_kernel_parameter3 returns one result ( parameter3 )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				float kernelParameter3;
				if ( GTSVM_GetKernelParameter3( context, &kernelParameter3 ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxSINGLE_CLASS, false );
				*( float* )mxGetPr( plhs[ 0 ] ) = kernelParameter3;
			}

			break;
		}

		case 13: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_biased takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_biased returns one result ( biased )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				bool biased;
				if ( GTSVM_GetBiased( context, &biased ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateLogicalMatrix( 1, 1 );
				*( bool* )mxGetPr( plhs[ 0 ] ) = biased;
			}

			break;
		}

		case 14: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_bias takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_bias returns one result ( bias )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				double bias;
				if ( GTSVM_GetBias( context, &bias ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxDOUBLE_CLASS, false );
				*( double* )mxGetPr( plhs[ 0 ] ) = bias;
			}

			break;
		}

		case 15: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_training_vectors takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_training_vectors returns one result ( vectors )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int rows;
				unsigned int columns;
				unsigned int nonzeros;

				if ( GTSVM_GetRows( context, &rows ) )
					mexErrMsgTxt( GTSVM_Error() );
				if ( GTSVM_GetColumns( context, &columns ) )
					mexErrMsgTxt( GTSVM_Error() );
				if ( GTSVM_GetNonzeros( context, &nonzeros ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateSparse( rows, columns, nonzeros, mxREAL );    /* **TODO: why not mSINGLE_CLASS? */
				if (
					GTSVM_GetTrainingVectorsSparse(
						context,
						mxGetPr( plhs[ 0 ] ),
						mxGetIr( plhs[ 0 ] ),    /* indices of nonzero elements */
						mxGetJc( plhs[ 0 ] ),    /* indices of starting/ending elements of each row */
						GetType( plhs[ 0 ] ),
						true    /* column-major order */
					)
				)
				{
					mexErrMsgTxt( GTSVM_Error() );
				}
			}

			break;
		}

		case 16: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_training_labels takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_training_labels returns one result ( labels )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int rows;
				if ( GTSVM_GetRows( context, &rows ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( rows, 1, mxINT32_CLASS, false );
				if (
					GTSVM_GetTrainingLabels(
						context,
						mxGetPr( plhs[ 0 ] ),
						GetType( plhs[ 0 ] )
					)
				)
				{
					mexErrMsgTxt( GTSVM_Error() );
				}
			}

			break;
		}

		case 17: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_training_responses takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_training_responses returns one result ( responses )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int rows;
				unsigned int classes;

				if ( GTSVM_GetRows( context, &rows ) )
					mexErrMsgTxt( GTSVM_Error() );
				if ( GTSVM_GetClasses( context, &classes ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( rows, classes, mxDOUBLE_CLASS, false );
				if (
					GTSVM_GetTrainingResponses(
						context,
						mxGetPr( plhs[ 0 ] ),
						GetType( plhs[ 0 ] ),
						true    /* column-major order */
					)
				)
				{
					mexErrMsgTxt( GTSVM_Error() );
				}
			}

			break;
		}

		case 18: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "get_alphas takes one parameter ( context )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "get_alphas returns one result ( alphas )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int rows;
				unsigned int classes;

				if ( GTSVM_GetRows( context, &rows ) )
					mexErrMsgTxt( GTSVM_Error() );
				if ( GTSVM_GetClasses( context, &classes ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( rows, classes, mxSINGLE_CLASS, false );
				if (
					GTSVM_GetAlphas(
						context,
						mxGetPr( plhs[ 0 ] ),
						GetType( plhs[ 0 ] ),
						true    /* column-major order */
					)
				)
				{
					mexErrMsgTxt( GTSVM_Error() );
				}
			}

			break;
		}

		case 19: {

			if ( nrhs != 3 )
				mexErrMsgTxt( "set_alphas takes 2 parameters ( context, alphas )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "set_alphas returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int rows;
				unsigned int classes;

				if ( GTSVM_GetRows( context, &rows ) )
					mexErrMsgTxt( GTSVM_Error() );
				if ( GTSVM_GetClasses( context, &classes ) )
					mexErrMsgTxt( GTSVM_Error() );

				if ( rows != mxGetM( prhs[ 2 ] ) )
					mexErrMsgTxt( "expected alphas to have same number of rows as training data" );
				if ( classes != mxGetN( prhs[ 2 ] ) )
					mexErrMsgTxt( "expected alphas to have same number of classes as training data" );

				if (
					GTSVM_SetAlphas(
						context,
						mxGetPr( prhs[ 2 ] ),
						GetType( prhs[ 2 ] ),
						true    /* column-major order */
					)
				)
				{
					mexErrMsgTxt( GTSVM_Error() );
				}
			}

			break;
		}

		case 20: {

			if ( nrhs != 2 )
				mexErrMsgTxt( "recalculate takes one parameter ( context )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "recalculate returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );
				if ( GTSVM_Recalculate( context ) )
					mexErrMsgTxt( GTSVM_Error() );
			}

			break;
		}

		case 21: {

			if ( ( nrhs < 7 ) || ( nrhs > 8 ) )
				mexErrMsgTxt( "restart takes 6 or 7 parameters ( context, regularization, kernel, parameter1, parameter2, parameter3, biased = false )" );
			if ( nlhs != 0 )
				mexErrMsgTxt( "restart returns no results" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				float const regularization = ( float )GetDouble( prhs[ 2 ] );

				GTSVM_Kernel kernel = GTSVM_KERNEL_UNKNOWN;
				char* const string = GetStringCopy( prhs[ 3 ] );

				float const kernelParameter1 = ( float )GetDouble( prhs[ 4 ] );
				float const kernelParameter2 = ( float )GetDouble( prhs[ 5 ] );
				float const kernelParameter3 = ( float )GetDouble( prhs[ 6 ] );

				bool biased = false;

				if ( strcasecmp( string, "gaussian" ) == 0 )
					kernel = GTSVM_KERNEL_GAUSSIAN;
				else if ( strcasecmp( string, "polynomial" ) == 0 )
					kernel = GTSVM_KERNEL_POLYNOMIAL;
				else if ( strcasecmp( string, "sigmoid" ) == 0 )
					kernel = GTSVM_KERNEL_SIGMOID;
				mxFree( string );
				if ( kernel == GTSVM_KERNEL_UNKNOWN )
					mexErrMsgTxt( "Expected kernel to be one of \"gaussian\", \"polynomial\" or \"sigmoid\"" );

				if ( nrhs >= 8 )
					biased = GetBool( prhs[ 7 ] );

				if (
					GTSVM_Restart(
						context,
						regularization,
						kernel,
						kernelParameter1,
						kernelParameter2,
						kernelParameter3,
						biased
					)
				)
				{
					mexErrMsgTxt( GTSVM_Error() );
				}
			}

			break;
		}

		case 22: {

			if ( nrhs != 3 )
				mexErrMsgTxt( "optimize takes two parameters ( context, iterations )" );
			if ( ( nlhs < 0 ) || ( nlhs > 2 ) )
				mexErrMsgTxt( "optimize returns two optional results ( primal, dual )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int const iterations = GetInt( prhs[ 2 ] );

				double primal, dual;
				if ( GTSVM_Optimize( context, &primal, &dual, iterations ) )
					mexErrMsgTxt( GTSVM_Error() );

				if ( nlhs > 0 ) {

					plhs[ 0 ] = mxCreateNumericMatrix( 1, 1, mxDOUBLE_CLASS, false );
					*( double* )mxGetPr( plhs[ 0 ] ) = primal;
				}
				if ( nlhs > 1 ) {

					plhs[ 1 ] = mxCreateNumericMatrix( 1, 1, mxDOUBLE_CLASS, false );
					*( double* )mxGetPr( plhs[ 1 ] ) = dual;
				}
			}

			break;
		}

		case 23: {

			if ( nrhs != 3 )
				mexErrMsgTxt( "classify takes two parameters ( context, vectors )" );
			if ( nlhs != 1 )
				mexErrMsgTxt( "classify return one result ( classifiers )" );

			{	GTSVM_Context const context = GetContext( prhs[ 1 ] );

				unsigned int const rows    = mxGetM( prhs[ 2 ] );
				unsigned int const columns = mxGetN( prhs[ 2 ] );

				unsigned int classes;
				if ( GTSVM_GetClasses( context, &classes ) )
					mexErrMsgTxt( GTSVM_Error() );

				plhs[ 0 ] = mxCreateNumericMatrix( rows, classes, mxDOUBLE_CLASS, false );

				if ( mxIsSparse( prhs[ 2 ] ) ) {

					if (
						GTSVM_ClassifySparse(
							context,
							mxGetPr( plhs[ 0 ] ),
							GetType( plhs[ 0 ] ),
							mxGetPr( prhs[ 2 ] ),
							mxGetIr( prhs[ 2 ] ),    /* indices of nonzero elements */
							mxGetJc( prhs[ 2 ] ),    /* indices of starting/ending elements of each row */
							GetType( prhs[ 2 ] ),
							rows,
							columns,
							true    /* column-major order */
						)
					)
					{
						mexErrMsgTxt( GTSVM_Error() );
					}
				}
				else {

					if (
						GTSVM_ClassifyDense(
							context,
							mxGetPr( plhs[ 0 ] ),
							GetType( plhs[ 0 ] ),
							mxGetPr( prhs[ 2 ] ),
							GetType( prhs[ 2 ] ),
							rows,
							columns,
							true    /* column-major order */
						)
					)
					{
						mexErrMsgTxt( GTSVM_Error() );
					}
				}
			}

			break;
		}

		default: mexErrMsgTxt( "Unknown function" );
	}
}
