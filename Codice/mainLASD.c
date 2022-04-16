#include <ProgettoLASD.h>
#include <stdio.h>
/*                PROGETTO LASD 2021/2022              */
/*             GIORGIO LONGOBARDO N86003571           */


/*        _____,    _..-=-=-=-=-====--,
     _.'a   /  .-',___,..=--=--==-'`
    ( _     \ /  //___/-=---=----'
     ` `\    /  //---/--==----=-'
  ,-.    | / \_//-_.'==-==---='
 (.-.`\  | |'../-'=-=-=-=--'
  (' `\`\| //_|-\.`;-~````~,        _
       \ | \_,_,_\.'        \     .'_`\
        `\            ,    , \    || `\\
          \    /   _.--\    \ '._.'/  / |
          /  /`---'   \ \   |`'---'   \/
         / /'          \ ;-. \
      __/ /           __) \ ) `|
    ((='--;)         (,___/(,_/*/


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
	
	while(User.saldo == -1) {
		printf("0 - Crea nuovo utente\n1 - Inserisci credenziali\n\nInserisci valore: ");
		fflush(stdin);
		scanf("%d", &scelta);
		while(scelta != 0 && scelta != 1) {
			printf("\nNumero non valido. Inserire scelta: ");
			scanf("%d", &scelta);
		}	
		if(scelta) User = login();
		else User = signup();		
	}
	
	printf("Ciao, %s\n\n", User.username);
	if(User.isSU) {
		menuAdmin(User);
	}
	else {
		menuUtente(User);
	}

}
