% Copyright (C) 2011  Andrew Cotter
% 
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.




classdef gtsvm < handle

	properties( GetAccess = private )
		context
	end

	methods

		function obj = gtsvm
			obj.context = gtsvm_mex( 0 );
		end

		function delete( obj )
			gtsvm_mex( 1, obj.context );
		end

		% kernel = 'gaussian'    ==>  K( x, y ) = exp( -p1 * || x - y ||^2 )
		% kernel = 'polynomial'  ==>  K( x, y ) = ( p1 * <x,y> + p2 )^p3
		% kernel = 'sigmoid'     ==>  K( x, y ) = tanh( p1 * <x,y> + p2 )
		function initialize( obj, vectors, labels, multiclass, regularization, kernel, parameter1, parameter2, parameter3, biased, small_clusters, active_clusters )

			if ( nargin < 10 )
				gtsvm_mex( 2, obj.context, vectors, labels, multiclass, regularization, kernel, parameter1, parameter2, parameter3 );
			elseif ( nargin < 11 )
				gtsvm_mex( 2, obj.context, vectors, labels, multiclass, regularization, kernel, parameter1, parameter2, parameter3, biased );
			elseif ( nargin < 12 )
				gtsvm_mex( 2, obj.context, vectors, labels, multiclass, regularization, kernel, parameter1, parameter2, parameter3, biased, small_clusters );
			else
				gtsvm_mex( 2, obj.context, vectors, labels, multiclass, regularization, kernel, parameter1, parameter2, parameter3, biased, small_clusters, active_clusters );
			end
		end

		function load( obj, filename, small_clusters, active_clusters )

			if ( nargin < 3 )
				gtsvm_mex( 3, obj.context, filename );
			elseif ( nargin < 4 )
				gtsvm_mex( 3, obj.context, filename, small_clusters );
			else
				gtsvm_mex( 3, obj.context, filename, small_clusters, active_clusters );
			end
		end

		function save( obj, filename )
			gtsvm_mex( 4, obj.context, filename );
		end

		function shrink( obj, small_clusters, active_clusters )

			if ( nargin < 2 )
				gtsvm_mex( 5, obj.context );
			elseif ( nargin < 3 )
				gtsvm_mex( 5, obj.context, small_clusters );
			else
				gtsvm_mex( 5, obj.context, small_clusters, active_clusters );
			end
		end

		function deinitialize_device( obj )
			gtsvm_mex( 6, obj.context );
		end

		function deinitialize( obj )
			gtsvm_mex( 7, obj.context );
		end

		function regularization = get_regularization( obj )
			regularization = gtsvm_mex( 8, obj.context );
		end

		function kernel = get_kernel( obj )
			kernel = gtsvm_mex( 9, obj.context );
		end

		function parameter1 = get_kernel_parameter1( obj )
			parameter1 = gtsvm_mex( 10, obj.context );
		end

		function parameter2 = get_kernel_parameter2( obj )
			parameter2 = gtsvm_mex( 11, obj.context );
		end

		function parameter3 = get_kernel_parameter3( obj )
			parameter3 = gtsvm_mex( 12, obj.context );
		end

		function biased = get_biased( obj )
			biased = gtsvm_mex( 13, obj.context );
		end

		function bias = get_bias( obj )
			bias = gtsvm_mex( 14, obj.context );
		end

		function training_vectors = get_training_vectors( obj )
			training_vectors = gtsvm_mex( 15, obj.context );
		end

		function training_labels = get_training_labels( obj )
			training_labels = gtsvm_mex( 16, obj.context );
		end

		function training_responses = get_training_responses( obj )
			training_responses = gtsvm_mex( 17, obj.context );
		end

		function alphas = get_alphas( obj )
			alphas = gtsvm_mex( 18, obj.context );
		end

		function set_alphas( obj, alphas )
			gtsvm_mex( 19, obj.context, alphas );
		end

		function recalculate( obj )
			gtsvm_mex( 20, obj.context );
		end

		% kernel = 'gaussian'    ==>  K( x, y ) = exp( -p1 * || x - y ||^2 )
		% kernel = 'polynomial'  ==>  K( x, y ) = ( p1 * <x,y> + p2 )^p3
		% kernel = 'sigmoid'     ==>  K( x, y ) = tanh( p1 * <x,y> + p2 )
		function restart( obj, regularization, kernel, parameter1, parameter2, parameter3, biased )

			if ( nargin < 7 )
				gtsvm_mex( 21, obj.context, regularization, kernel, parameter1, parameter2, parameter3 );
			else
				gtsvm_mex( 21, obj.context, regularization, kernel, parameter1, parameter2, parameter3, biased );
			end
		end

		function [ primal, dual ] = optimize( obj, epsilon, iterations, repetitions )

			if ( nargin < 4 )
				repetitions = 256;
			end
			if ( ( repetitions <= 0 ) || ( mod( repetitions, 16 ) ~= 0 ) )
				error( 'repetitions must be a positive multiple of 16' );
			end

			primal = Inf;
			dual = -Inf;

            fprintf( 'Optimizing.. \n') ;
			for ( ii = 1:repetitions:iterations )

				[ primal, dual ] = gtsvm_mex( 22, obj.context, repetitions );
				fprintf( 'Iteration %d/%d, primal = %f, dual = %f\n', ii, iterations, primal, dual ) ;
%                 fprintf( '. \n') ;
				if ( 2 * ( primal - dual ) < epsilon * ( primal + dual ) ); break; end

            end

			if ~( isfinite( primal ) && isfinite( dual ) )
				[ primal, dual ] = gtsvm_mex( 22, obj.context, 0 );
			end
		end

		function [ primal, dual ] = get_objectives( obj )
			[ primal, dual ] = gtsvm_mex( 22, obj.context, 0 );
		end

		function classifiers = classify( obj, vectors )
			classifiers = gtsvm_mex( 23, obj.context, vectors );
		end

	end

end
