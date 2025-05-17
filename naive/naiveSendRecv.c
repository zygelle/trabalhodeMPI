#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

int primo (long int n) { /* mpi_primos.c  */
	int i;

	for (i = 3; i < (int)(sqrt(n) + 1); i+=2) {
		if(n%i == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	double t_inicial, t_final;
	int cont = 0, total = 0;
	long int i, n;
	int meu_ranque, num_procs, inicio, salto;

	if (argc < 2) {
        printf("Valor inválido! Entre com um valor do maior inteiro\n");
		return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
		if (n < 2) {
			printf("Valor inválido! Entre com um valor maior que 1\n");
			return 0;
		}
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);	
    t_inicial = MPI_Wtime();
    inicio = 3 + meu_ranque*2;
    salto = num_procs*2;

	for (i = inicio; i <= n; i += salto) 
	{	
		if(primo(i) == 1) cont++;
	}
		
	if(num_procs > 1) {
		if(meu_ranque != 0) {
			MPI_Send(&cont, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		} else {
			total = cont;
			for(int origem = 1; origem < num_procs; origem++) {
				MPI_Recv(&cont, 1, MPI_INT, origem, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				total += cont;
			}
		}
	} else {
		total = cont;
	}
	
	t_final = MPI_Wtime();

	if (meu_ranque == 0) {
        total += 1;    /* Acrescenta o dois, que também é primo */
		printf("Quant. de primos entre 1 e n: %d \n", total);
		printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);	 
	}
	MPI_Finalize();
	return(0);
}