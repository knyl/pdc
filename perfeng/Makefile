all: opt

gnu:
	gcc -O3 -std=c99 main.c -o gscale -lm -DGNU
nopt:
	icc -O3 -std=c99 main.c -o scale_slow
opt:
	icc -O3 -std=c99 -fast -openmp -fno-alias -funroll-all-loops main.c -o matmul_romanvg_knyl
prof:
	icc -O3 -std=c99 -opt-report -openmp -fast main.c -o scale
blas:
	icc call_blas.c -O3 -I/pdc/vol/i-compilers/11.1/icc/mkl/include -L/pdc/vol/i-compilers/11.1/icc/mkl/lib/em64t -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -Wl,--rpath,/pdc/vol/i-compilers/11.1/icc/mkl/lib/em64t -o call_blas
blaspar:
	mpicc call_blas.c -O3 -I/pdc/vol/i-compilers/11.1/icc/mkl/include -L/pdc/vol/i-compilers/11.1/icc/mkl/lib/em64t -lmkl_scalapack_lp64 -lmkl_blacs_openmpi_lp64 -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -Wl,--rpath,/pdc/vol/i-compilers/11.1/icc/mkl/lib/em64t -o call_blas
correct:
	icc main.c -std=c99 -O3 -openmp -I/pdc/vol/i-compilers/11.1/icc/mkl/include -L/pdc/vol/i-compilers/11.1/icc/mkl/lib/em64t -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -Wl,--rpath,/pdc/vol/i-compilers/11.1/icc/mkl/lib/em64t -DUSE_BLAS -DTEST_CORRECTNESS -o scale_correct

run:
	./gscale 100
