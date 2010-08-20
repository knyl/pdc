#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <malloc.h>

#define BLOCK 16
#define min(a, b) ((a < b) ? a : b)
#define MUL_OUTPUT 1
//TODO: sse, profile value of BLOCK (bruteforce)

//#ifdef GNU
	#include <xmmintrin.h>
//#else
//Intel Compiler
	#include <emmintrin.h>
	#include <mmintrin.h>
//#endif

// example prototype for your matmul function
void mul(double* dest, const double* a, const double* b, int N);

inline double gettime(void) 
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec + 1e-6*tv.tv_usec;
}

inline void fill_mat(double* m, int M, double val)
{
    int i, j;
    for (i=0; i<M; i++)
	for (j=0; j<M; j++)
	    m[M*i + j] = val;
}

inline void mul(double* dest, const double* a, const double* b, int M){
    int i, j, k, jj, kk;
    for (jj=0; jj<M; jj+=BLOCK) 
	for (kk=0; kk<M; kk+=BLOCK)
	    for (i=0; i<M; ++i)
		for (j=jj; j<min(jj+BLOCK, M); ++j){
		    double sum = 0.0;
		    for (k=kk; k<min(kk+BLOCK, M); ++k)
			sum += a[M*i + k] * b[M*k + j];
		    dest[M*i + j] += sum;
		}
}


void mul_sse(double* dest, const double* a, const double* b, int M){
    int i, j, k, jj, kk;
    double dummy[2];

    __m128d ae, be, res, sum;

    //sum = _mm_xor_pd(sum, sum);
    sum = _mm_setzero_pd();
    for (jj=0; jj<M; jj+=BLOCK)
	for (kk=0; kk<M; kk+=BLOCK)
	    for (i=0; i<M; i+=2) {
		for (j=jj; j<min(jj+BLOCK, M); j+=2) {
		    res = _mm_setzero_pd();
		    for (k=kk; k<min(kk+BLOCK, M); k+=2){
			// Loading values into __m128d
			ae = _mm_load_pd(&(a[M*i+k]));
			be = _mm_load_pd(&(b[M*k+j]));
			// Performing multiplication and add (sum += a * b)
			sum = _mm_add_pd(sum, _mm_mul_pd(ae, be));
			
			_mm_store_pd(dummy, sum);
			printf("dummy: %lf %lf\n", dummy[0], dummy[0]);
		    }
		    res = _mm_load_pd(&(dest[M*i+j]));
		    // Add result
		    res = _mm_add_pd(res, sum);
		    _mm_store_pd(&(dest[M*i+j]), res);
			_mm_store_pd(dummy, res);
			printf("dummy: %lf %lf\n", dummy[0], dummy[1]);
			printf("dest: %lf %lf\n", dest[0], dest[1]);
		}
	    }
}

int main(int args, char* argv[])
{
    int i, j;
    const int M = atoi(argv[1]);

    double* A = (double*) _mm_malloc(M*M*sizeof(double), 16);
    double* B = (double*) _mm_malloc(M*M*sizeof(double), 16);
    double* C = (double*) _mm_malloc(M*M*sizeof(double), 16);

    fill_mat(A,M,1.0);
    fill_mat(B,M,2.0);
    fill_mat(C,M,0.0);

    double t = gettime();
    //mul(C,A,B,M);
    mul_sse(C,A,B,M);
    t = gettime()-t;

    printf("%d\t%f\t%E\n",M,t,2*pow(M,3)/t);
    if (MUL_OUTPUT) {    
	for (i=0; i<M; i++, printf("\n"))
	    for (j=0; j<M; j++, printf(" "))
		printf("%lf", C[M*i + j]);
    }
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    return 0;
}
