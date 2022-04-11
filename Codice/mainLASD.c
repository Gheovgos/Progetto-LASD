#include <ProgettoLASD.h>
#include <stdio.h>


int main() {
	utente User;
	int scelta;
	
	printf("***************** PROGETTO LASD *****************\n            GIORGIO LONGOBARDO N86003571                 \n");
	printf("0 - Sign In \n\n1 - Sign Up \n\n\nInserire scelta: ");
	scanf("%d", &scelta);
	
	while(scelta != 0 && scelta != 1) {
		printf("\nNumero non valido. Inserire scelta: ");
		scanf("%d", &scelta);
	}

	if(scelta == 1) User = signup();
	
	else if(scelta == 0) User = login();	
	
	
	printf("Ciao, %s\n\n", User.username);
	if(User.isSU) {
		menuAdmin(User);
	}
}
