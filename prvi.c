#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX 2000

int main()
{
	int tmp = 0;
	char lista[MAX+1] = {0};
	scanf("%1000s", lista);
	int n = strlen(lista);

	// printf("Prvi korak:\n");
	for(int i = 0; i < n; i++) {
		lista[i]-=10;
		// printf("%c", lista[i]);
	}
	// printf("\n");

	char updatedLista[MAX*8+1] = {0};
	
	int j = 0;
	// printf("Drugi korak:\n");
	for(int i = 0; i < n; i++) {
		if (lista[i] >= 'A' && lista[i] <= 'Z') {
			int start = j;
			for (j = j; j < start + lista[i]-'A'+2; j++) {
				updatedLista[j] = '0';
				// printf("%c", updatedLista[j]);
			}
		} else {
			updatedLista[j] = lista[i];
			// printf("%c", updatedLista[j]);
			j++;
		}
	}
	// printf("\n");
	int len = strlen(updatedLista);

	// printf("Treci korak:\n");

	for(int i = 0; i < len; i++) {
		if (updatedLista[i] == '0') {
			updatedLista[i] = '1';
			// printf("%c", updatedLista[i]);
			continue;
		} 
		if (updatedLista[i] == '1') {
			updatedLista[i] = '0';
			// printf("%c", updatedLista[i]);
			continue;
		}
		// printf("%c", updatedLista[i]);
	}
	len = strlen(updatedLista);

	// printf("\n");

	char copy[MAX*8+1] = {0};
	strcpy(copy, updatedLista);
	// printf("Cetvrti korak:\n");
	j = 0;
	for(int i = 0; i < len; i++) {
		if (copy[i] >= '2' && copy[i] <= '9') {
			int numZeroes = copy[i]-'0';
			int end = j+numZeroes;
			while(j<end) {
				updatedLista[j] = '0';
				// printf("%c", updatedLista[j]);
				j++;
			}
			continue;
		}
		updatedLista[j] = copy[i];
		// printf("%c", updatedLista[j]);
		j++;
	}
	// printf("\n");
	len = strlen(updatedLista);

	// printf("Peti korak:\n");
	char final[MAX] = {0};
	for(int i=0; i < len; i+=8) {
    int c = 0;
    for(j=i; j<i+8;j++) {
      if (updatedLista[j] == '1') c+=(int)pow(2, 7-j%8);
    }
    final[i/8] = (char)c;
		// printf("%c", final[i/8]);
	}

	// printf("\n");

	printf("%s", final);

	return 0;

}
