//#include <ProgettoLASD.h>
#include <stdio.h>


int main() {
	//utente User[50];
	int scelta;
	
	printf("***************** PROGETTO LASD *****************\n            GIORGIO LONGOBARDO N86003571                 \n");
	printf("0 - Sign In \n\n1 - Sign Up \n\n\nInserire scelta: ");
	scanf("%d", &scelta);
	
	while(scelta != 0 && scelta != 1) {
		printf("\nNumero non valido. Inserire scelta: ");
		scanf("%d", &scelta);
	}
}
