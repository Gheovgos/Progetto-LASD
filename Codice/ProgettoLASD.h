#include <liste.h>
#include <string.h>
#include <stdlib.h>

/*FILE HEADER DI DEFINIZIONE UTENTE, STRUTTURE DATI E FUNZIONI*/
/*GIORGIO LONGOBARDO N86003571*/

//Definizione di struct Utente
typedef struct User{
	char username[50];    //Campo stringa che identifica l'username di un utente
	char password[50];   //Campo stringa che identifica la password di un utente
	float saldo;        //Campo float che memorizza il saldo corrente di un utente. Il saldo viene automaticamente impostato a 1000 se viene creato un nuovo utente. Inoltre il saldo viene caricato e/o salvato nel file di testo "utenti.txt"
	int isSU;          //indica se è un Super User (1 se admin, 0 altrimenti)
}utente;

//Definizione di una struct prodotto
typedef struct Prodotto{
	char nomeProdotto[200];   //Campo stringa che identifica il nome del prodotto
	float costo;             //Campo float che identifica il costo del prodotto
	int taglia;             //Campo intero che identifica la taglia specifica del prodotto
	int isAv;              //Campo booleano che indica se un determinato prodotto sia disponibile (0 se non disponibile, 1 altrimenti)
	utente inAttesa[50]; //vettore utenti che attendono un determinato prodotto
}prodotto;


//Definizione di carrello (Lista SL)
typedef struct carr{
	prodotto Prodotto;          //Campo struct prodotto
	struct carr *next;          //Campo puntatore al prossimo nodo del carrello
}Carrello;


//Definizione di un ABR di prodotti
typedef struct ABR{
	prodotto prodottoN;          //Campo struct prodotto
	struct ABR* dx;             //Puntatore al figlio destro dell'albero
	struct ABR* sx;            //Puntatore al figlio sinistro dell'albero
}ABRprodotti;



/*        PROTOTIPI FUNZIONE        */
/*          FUNZIONI CONDIVISE      */

utente login(); //Funzione di accesso di un Utente
utente signup(); //funzione di registrazione di un utente

/*          FUNZIONI ADMIN          */

void menuAdmin(utente User); //funzione che mostra il menu dell'amministratore

void aggiungiProdotto(ABRprodotti **ABR, float c, char *nomeFile); //funzione che aggiunge un prodotto (nodo) nell'albero dei prodotti

void mostraProdotti(ABRprodotti *ABR); //visita in preordine dell'albero binario

ABRprodotti* caricaProdotti(ABRprodotti *ABR, char *nomeFile); //carica in memoria i nodi di un ABR da un file di testo

void salvaProdotti(ABRprodotti *ABR, FILE *f); //salva in memoria i nodi di un ABR in un file di testo

void supportoCaricaProdotti(ABRprodotti **ABR, float prezzo, char *nomeProdotto, int taglia, int isAv); //funzione di supporto a caricaProdotti (aggiunge un nodo all'albero con dati prelevati da un file di testo)

void modificaProdotti(ABRprodotti *ABR, char *nomeFile); //modifica un determinato prodotto in un albero
      
float ricercaPrezzoMinimo(ABRprodotti *ABR); //trova il prezzo minimo in un albero

int vuoto(ABRprodotti *ABR);                 //Indica se l'albero è vuoto

ABRprodotti* eliminaProdotto(ABRprodotti *rad, float c);      //elimina un nodo dell'albero dato il prezzo

ABRprodotti* popolaAlberoRandom(); // crea un albero di prodotti casuale (per fase di testing!)


/*     FUNZIONI UTENTE        */
void menuUtente(utente User); //funzione che mostra le opzioni possibili per un utente

void aggiornaUtenti(utente *User);        //funzione che aggiorna "utenti.txt" una volta modificato il saldo di un utente

Carrello* acquistaProdotto(ABRprodotti *ABR, char *nomeProdotto, float costo, utente *User, Carrello* carr); //funzione che mette in un carrello un prodotto che si vuole acquistare
 
void mettiInCoda(ABRprodotti *ABR, utente *User);     //funzione che mette un utente nel campo "utente inAttesa[]" l'utente che vuole un prodotto non ancora disponibile

Carrello* creaNodo(prodotto Prodotto);           //funzione che crea un nodo del carrello

void mostraCarrello(Carrello *carr);            //funzione che mostra il carrello di un utente

void calcolaSommaCarrello(Carrello *carrello, float *f);         //funzione che calcola la somma totale dei prezzi di tutti i prodotti in un carrello

Carrello* inserisciInTesta(Carrello *carr, prodotto Prodotto);  //funzione che mette in testa un nuovo prodotto in un carrello

Carrello* svuotaCarrello(Carrello *carr);                       //funzione che svuota un carrello (dopo l'acquisto dei prodotti)



/*          DEFINIZIONE FUNZIONI           */

utente login() {
	utente User;
	User.saldo = -1;     //Il saldo viene impostato a -1 per convenzione: se l'utente non viene trovato nel file di testo, allora l'utente di ritorno avrà -1 come saldo e quindi indica al main che non è stato trovato con quelle credenziali
	FILE *f;
	char stringa[100], usernm[50], psswd[50];
	int c = 0, su;
	float saldo;
	
	printf("Inserire username: ");
	fflush(stdin);
	gets(usernm);
	
	printf("Inserire password: ");
	fflush(stdin);
	gets(psswd);
	
	for(int i = 0; usernm[i] != '\0'; i++) {                        
		stringa[c] = usernm[i];
		c++;
	}
	for(int i = 0; psswd[i] != '\0'; i++) {
		stringa[c] = psswd[i];
		c++;
	}
	c = 0;
	
	f = fopen("utenti.txt", "r");
	if(f == NULL) {
		printf("File di testo non trovato!");
		return User;
	}
	
	while(!feof(f)) {
		char tmp[100];
		fscanf(f, "%s\n", &tmp);
		fscanf(f, "%d\n", &su);
		fscanf(f, "%f\n", &saldo);
		
		if(!strcmp(tmp, stringa)) {
			
			strcpy(User.username, usernm);
			strcpy(User.password, psswd);
			User.saldo = saldo;
			User.isSU = su;
			fclose(f);
			printf("Login effettuato con successo!\n");
			return User;
			
		}	
	}
	
	printf("Utente non trovato!\n");
	fclose(f);
	return User;
}

utente signup() {
	utente User;
	FILE *f, *fp;
	char stringa[100], usernm[50], psswd[50];
	int c = 0, su;
	float saldo = 1000;
	
	printf("Inserire username: ");
	fflush(stdin);
	gets(usernm);
	
	printf("Inserire password: ");
	fflush(stdin);
	gets(psswd);
	
	for(int i = 0; usernm[i] != '\0'; i++) {
		stringa[c] = usernm[i];
		c++;
	}
	for(int i = 0; psswd[i] != '\0'; i++) {
		stringa[c] = psswd[i];
		c++;
	}
	c = 0;
	
	
	f = fopen("utenti.txt", "a");
	fp = fopen("utenti.txt", "r");
	if(f == NULL) {
		printf("File di testo non trovato!");
		return User;
	}
			
	printf("\nSei un amministratore?\n\n1 - Si'\n\n0 - No\n\nInserisci valore: ");
	scanf("%d", &su);
	
	while(su != 0 && su != 1) {
		printf("\nNumero non valido. Inserire scelta: ");
		scanf("%d", &su);
	}
	
	fprintf(f, "\n%s", stringa);
	fprintf(f, "\n%d", su);
	fprintf(f, "\n%f", saldo);
	fclose(f);
	
	strcpy(User.username, usernm);
	strcpy(User.password, psswd);
	
	User.saldo = saldo;
	User.isSU = su;

	return User;
}

void menuAdmin(utente User) {
	printf("****** MENU AMMINISTRATORE ******\n\n\n");
	
	int n, loop = 1, flag, counter = 0, s;
	ABRprodotti *ABR = NULL;
	float c;
	char stringa[100], tmp[100], str[100];
	FILE *f;
	
	while(loop) {
	printf("\nQuale operazione vuole svolgere?\n\n0 - Carica prodotti\n1 - Aggiungi prodotto\n2 - Mostra prodotti\n3 - Aggiorna prodotti\n4 - Crea albero casuale\n10 - Logout\n\n\nInserire valore: ");
	scanf("%d", &n);
	
	while(n != 0 && n != 1 && n != 2 && n != 3 && n != 4 && n != 10) {
		printf("\nNumero non valido. Inserire scelta: ");
		fflush(stdin);
		scanf("%d", &n);
	}
		
	switch(n) {
		
		case 0:
			printf("Hai selezionato carica prodotti");
		
			printf("\nInserire il nome del file dal quale si devono caricare i prodotti: ");
			fflush(stdin);
			gets(stringa);
			float d;
			while(ABR != NULL) {                                                     //svuoto l'albero nel caso in cui non sia vuoto
				d = ricercaPrezzoMinimo(ABR);
				ABR = eliminaProdotto(ABR, d);
			}
			ABR = caricaProdotti(ABR, stringa);	

			break;
		case 1:
			
			printf("Hai selezionato aggiungi prodotto");
							
			printf("\nInserire il nome del file sul quale si devono salvare i prodotti: ");
			fflush(stdin);
			gets(stringa);
			
			
			f = fopen("listaprodotti.txt", "r+");
			
			while(!feof(f)) {
				fscanf(f, "%s", &tmp);
				if(!strcmp(tmp, stringa))
					counter++;	
			}
			fclose(f);
			
			if(counter == 0) {
				f = fopen("listaprodotti.txt", "a+");
				fprintf(f, "\n%s", stringa);
				fclose(f);
			}
			
			do {
			
			printf("\nInserire il costo: ");
				fflush(stdin);
				scanf("%f", &c);
				while(c < 0) {
					printf("\nIl costo non puo' essere negativo!\nInserire un costo valido: ");
					fflush(stdin);
					scanf("%f", &c);		
			}
			
			aggiungiProdotto(&ABR, c, stringa);
			
			printf("Vuoi aggiungere un altro prodotto?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
			fflush(stdin);
			scanf("%d", &flag);	
			while(flag != 0 && flag != 1) {
					printf("\nInserire un costo valido: ");
					fflush(stdin);
					scanf("%d", &flag);		
				}
			} while(flag);
			
		
			break;
			
		case 2:
			printf("Hai selezionato mostra prodotti ordinati per prezzo crescente\n");
			mostraProdotti(ABR);		
			break;
			
		case 3:
			printf("\nInserire il nome del file dal quale si devono modificare i prodotti: ");
			fflush(stdin);
			gets(stringa);
			
			f = fopen(stringa, "r+");
			while(f == NULL) {
				printf("Il file inserito non esiste!\nInserire un file esistente: ");
				fflush(stdin);
				gets(stringa);
			}			
			while(ABR != NULL) {
				d = ricercaPrezzoMinimo(ABR);
				ABR = eliminaProdotto(ABR, d);
			}
						
			ABR = caricaProdotti(ABR, stringa);
			mostraProdotti(ABR);	
			fclose(f);
			
			do {				
				printf("\nScrivere il nome prodotto che si vuole modificare: ");
				fflush(stdin);
				gets(str);
				
					
				printf("Inserire il vecchio prezzo: ");
				fflush(stdin);
				scanf("%f", &d);
					
				ABR = eliminaProdotto(ABR, d);
				
				printf("Vuoi eliminare definitivament il prodotto?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
				fflush(stdin);
				scanf("%d", &s);
				while(s != 0 && s != 1) {
					printf("Valore non valido!\nInserisci un valore valido: ");
					fflush(stdin);
					scanf("%d", &s);
				}
				if(s) {
					FILE *fp = fopen(stringa, "w+");
					salvaProdotti(ABR, fp);
					fclose(fp);
					printf("\nVuoi modificare un altro prodotto?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
					fflush(stdin);
					
					scanf("%d", &counter);
					while(counter != 0 && counter != 1) {
						printf("Valore inserito non valido! Inserire un valore valido: ");
						fflush(stdin);
						scanf("%d", &counter);
					}
				}
				else {
					printf("\nInserire il nuovo prezzo: ");
					fflush(stdin);
					scanf("%f", &c);
					
					aggiungiProdotto(&ABR, c, stringa);
					FILE *fp = fopen(stringa, "w+");
					salvaProdotti(ABR, fp);
					fclose(fp);
					
					printf("\nVuoi modificare un altro prodotto?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
					fflush(stdin);
					
					scanf("%d", &counter);
					while(counter != 0 && counter != 1) {
						printf("Valore inserito non valido! Inserire un valore valido: ");
						fflush(stdin);
						scanf("%d", &counter);
					}	
			      }		
				}while(counter);
			
			break;
		
		case 4:
			printf("\nAdesso creo un albero casuale");
			ABR = popolaAlberoRandom();
			printf("\nEcco cosa ho generato: \n");
			mostraProdotti(ABR);
			break;
			
		case 10:
			printf("Arrivederci!");
			loop = 0;
			system(EXIT_SUCCESS);
		}
	}
}

void aggiungiProdotto(ABRprodotti **ABR, float c, char *nomeFile) {
	ABRprodotti *aux;
	if(*ABR == NULL) {
		aux = (ABRprodotti*)malloc(sizeof(ABRprodotti));
		
		if(aux) {
		
			FILE* f = fopen(nomeFile, "a+");
			int loop = 1, s;
			
						
			aux->prodottoN.costo = c;
			fprintf(f, "%.2f\n", c);
			printf("\nAggiungi il nome del prodotto: ");
			fflush(stdin);
			gets(aux->prodottoN.nomeProdotto);
			fprintf(f, "%s\n", aux->prodottoN.nomeProdotto);
		
		
			printf("\nInserire la taglia: ");
			fflush(stdin);
			scanf("%d", &aux->prodottoN.taglia);
			fprintf(f, "%d\n", aux->prodottoN.taglia);
			
			
			printf("Il prodotto e' gia' disponibile?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
			scanf("%d", &aux->prodottoN.isAv);
			fprintf(f, "%d\n", aux->prodottoN.isAv);
			while(aux->prodottoN.isAv != 0 && aux->prodottoN.isAv != 1) {
					printf("\nInserire un valore valido: ");
					fflush(stdin);
					scanf("%d", &aux->prodottoN.isAv);	
					fprintf(f, "%d\n", aux->prodottoN.isAv);	
				}		
			aux->sx=NULL;
			aux->dx=NULL;
			*ABR=aux;	
		}	
	}
	else if((*ABR)->prodottoN.costo > c) 	aggiungiProdotto(&(*ABR)->sx, c, nomeFile);
	else if((*ABR)->prodottoN.costo < c) 	aggiungiProdotto(&(*ABR)->dx, c, nomeFile);	
}

void mostraProdotti(ABRprodotti *ABR) {
	if(ABR) {
		mostraProdotti(ABR->sx);
		printf("ARTICOLO: %s PREZZO: %.2f TAGLIA: %d  \n", ABR->prodottoN.nomeProdotto, ABR->prodottoN.costo, ABR->prodottoN.taglia);	
		mostraProdotti(ABR->dx);
	}	
}

ABRprodotti* caricaProdotti(ABRprodotti *ABR, char *nomeFile) {
	
	FILE *f;
	int taglia, isAv;

	float prezzo;
	char nomeProdotto[100];

	f = fopen(nomeFile, "r+");
	
	while(!feof(f)) {

		fscanf(f, "%f\n %s\n %d\n %d\n", &prezzo, &nomeProdotto, &taglia, &isAv);
		
		supportoCaricaProdotti(&ABR, prezzo, nomeProdotto, taglia, isAv);	
	}

	fclose(f);
	return ABR;
}
	
void supportoCaricaProdotti(ABRprodotti **ABR, float prezzo, char *nomeProdotto, int taglia, int isAv) {
	ABRprodotti *aux;
	if(*ABR == NULL) {
		
		aux = (ABRprodotti*)malloc(sizeof(ABRprodotti));
		if(aux) {		
			aux->prodottoN.costo = prezzo;
			
			strcpy(aux->prodottoN.nomeProdotto, nomeProdotto);
			
			aux->prodottoN.taglia = taglia;
			aux->prodottoN.isAv = isAv;
			aux->sx=NULL;
			aux->dx=NULL;
			*ABR=aux;	
		}
		
	}
	else if((*ABR)->prodottoN.costo > prezzo) 	supportoCaricaProdotti(&(*ABR)->sx, prezzo, nomeProdotto, taglia, isAv);
	else if((*ABR)->prodottoN.costo < prezzo) 	supportoCaricaProdotti(&(*ABR)->dx, prezzo, nomeProdotto, taglia, isAv);
}

int vuoto(ABRprodotti *ABR) {
	if(ABR) return 0;
	else return 1;
}

float ricercaPrezzoMinimo(ABRprodotti *ABR) {
	float min = 0;
	if(!vuoto(ABR)) {
		if(ABR->sx == NULL) min = ABR->prodottoN.costo;
		else min = ricercaPrezzoMinimo(ABR->sx); }
	return min;
}

ABRprodotti* ricercaNodoMinimo(ABRprodotti *ABR) {
	float min = 0;
	if(!vuoto(ABR)) {
		if(ABR->sx == NULL) min = ABR->prodottoN.costo;
		else min = ricercaPrezzoMinimo(ABR->sx); }
	return ABR;
}

ABRprodotti* eliminaProdotto(ABRprodotti *rad, float c) {
	
	if(!rad) return rad;
	if(rad->prodottoN.costo < c) rad->dx = eliminaProdotto(rad->dx, c);
	else if(rad->prodottoN.costo > c) rad->sx = eliminaProdotto(rad->sx, c);
	
	else if(rad->prodottoN.costo == c) {
		if(!rad->sx) {
			ABRprodotti* tmp = rad->dx;
			free(rad);
			return tmp;
		}
		else if(!rad->dx) {
			ABRprodotti* tmp = rad->sx;
			free(rad);
			return tmp;
		}
		ABRprodotti* tmp = ricercaNodoMinimo(rad->dx);
		rad->prodottoN.costo = tmp->prodottoN.costo;
		rad->dx = eliminaProdotto(rad->dx, c);
	}
	return rad;
}

ABRprodotti* popolaAlberoRandom() {
	ABRprodotti *ABR = (ABRprodotti*)malloc(sizeof(ABRprodotti));
	ABR = NULL;
	int counter;
	int cont = 0;
	counter = rand() % 24;
	float c = (float)rand()/(float)(RAND_MAX/500);
	int ia = rand() % 2;
	char nome[20];
	
	
	for(int i = 0; i < counter; i++) {
		int random = rand()%51;
		cont++;
		ia = rand() % 3;
		c = (float)rand()/(float)(RAND_MAX/500);

		strcpy(nome, "prodotto ");
		nome[8] =  c + 64;
		
		supportoCaricaProdotti(&ABR, c, nome, random , ia-1);
	}
	
	return ABR;
		
}

void salvaProdotti(ABRprodotti *ABR, FILE* f) {		
	if(ABR) {
		salvaProdotti(ABR->sx, f);
		fprintf(f, "\n%f\n%s\n%d\n%d\n", ABR->prodottoN.costo, ABR->prodottoN.nomeProdotto, ABR->prodottoN.taglia, ABR->prodottoN.isAv);
		salvaProdotti(ABR->dx, f);
	}
}

void menuUtente(utente User) {
	printf("******** MENU UTENTE ********\n\n\n");
	
	int n, loop = 1, flag, counter = 0;
	ABRprodotti *ABR = NULL;
	Carrello *carrello = NULL;
	float c;
	char stringa[100], tmp[100], str[100];
	FILE *f;
	
	while(loop) {
		printf("\nSaldo attuale: %.2f", User.saldo);
		printf("\nQuale operazione vuole svolgere?\n\n0 - Acquista prodotti\n1 - Mostra prodotti\n2 - Ricarica saldo\n3 - Vedi carrello\n4 - Scarica denaro\n10 - Logout\n\n\nInserire valore: ");
		fflush(stdin);
		scanf("%d", &n);
	
		while(n != 0 && n != 1 && n != 2 && n != 3 && n != 4 && n != 10) {
			printf("\nNumero non valido. Inserire scelta: ");
			fflush(stdin);
			scanf("%d", &n);
		}
		
	switch(n) {
		
		case 0:
			printf("Hai selezionato acquista prodotti\n");
			if(ABR == NULL) {
					f = fopen("listaprodotti.txt", "r");
					
					while(!feof(f)) {
						fscanf(f, "%s", &stringa);
						ABR = caricaProdotti(ABR, stringa);		
					}	
					fclose(f);
			}
			
			mostraProdotti(ABR);
			
			printf("Inserire nome del prodotto che si vuole acquistare: ");
			fflush(stdin);
			gets(stringa);
			
			printf("Inserire prezzo del prodotto che si vuole acquistare: ");
			fflush(stdin);
			scanf("%f", &c);
			
			carrello = acquistaProdotto(ABR, stringa, c, &User, carrello);

			break;
			
		case 1:
			
			printf("Hai selezionato mostra prodotti");
			printf("\nEcco tutti i prodotti disponibili: \n");
			
			if(ABR == NULL) {
				f = fopen("listaprodotti.txt", "r+");

				while(!feof(f)) {
					fscanf(f, "%s", &stringa);
					ABR = caricaProdotti(ABR, stringa);
				}
				fclose(f);	
			}
			
			mostraProdotti(ABR);
								
			break;
		
		case 2:
			printf("Inserisci l'importo da caricare al saldo attuale: ");
			fflush(stdin);
			scanf("%f", &c);
			User.saldo += c;
			aggiornaUtenti(&User);
			break;
			
		case 3:
			if(carrello == NULL) printf("\nIl tuo carrello e' vuoto!");
			
			else {
				printf("\nQuesto e' il tuo carrello: \n");
				mostraCarrello(carrello);
				printf("\nVuoi acquistare?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
				fflush(stdin);
				scanf("%d", &counter);
				while(counter != 0 && counter != 1) {
					printf("Inserisci un valore valido!\nInserisci il valore: ");
					fflush(stdin);
					scanf("%d", &counter);
				}	
				if(counter) {
					c = 0;
					calcolaSommaCarrello(carrello, &c);
					printf("Somma totale del carrello: %.2f", c);
					if(User.saldo < c) printf("\nSaldo insufficiente!\n");
					else {
						User.saldo -= c;
						aggiornaUtenti(&User);
						carrello = svuotaCarrello(carrello);
					}
				}	
			}
			break;
			
		case 4:
			printf("\nInserisci l'importo da scaricare dal saldo attuale: ");
			fflush(stdin);
			scanf("%f", &c);
			User.saldo -= c;
			aggiornaUtenti(&User);
			break;
			
		case 10:
			printf("Arrivederci!");
			loop = 0;
			system(EXIT_SUCCESS);
		}
	}
}

void aggiornaUtenti(utente *User) {
	FILE *f;
	int i = 0, isAv[100];
	char usernamepassword[100][100], tmp[200];
	float saldo[100];
		
	strcpy(tmp, User->username);
	strcat(tmp, User->password);

	f = fopen("utenti.txt", "r+");
	while(!feof(f)) {
		fscanf(f, "%s\n%d\n%f", &usernamepassword[i], &isAv[i], &saldo[i]);
			
		if(!strcmp(usernamepassword[i], tmp)) saldo[i] = User->saldo;
		 
		i++;
	}
	
	i--;
	fclose(f);
	
	f = fopen("utenti.txt", "w+");
	for(int k = 0; k < i; k++) fprintf(f, "\n%s\n %d\n %f\n", usernamepassword[k], isAv[k], saldo[k]);
	fclose(f);
}

Carrello* acquistaProdotto(ABRprodotti *ABR, char *nomeProdotto, float costo, utente *User, Carrello *carr) {
	if(ABR) {
		if(!strcmp(nomeProdotto, ABR->prodottoN.nomeProdotto) && ABR->prodottoN.costo == costo) {
			if(ABR->prodottoN.costo > User->saldo) {
				printf("\nSaldo insufficiente!");
				return NULL;
			}
			else if(!ABR->prodottoN.isAv){
				int s;
				
				printf("Il prodotto non e' ancora disponibile. Vuoi metterti in coda per l'acquisto non appena il prodotto sara' disponibile?\n\n1 - Si\n0 - No\n\nInserisci valore: ");
				fflush(stdin);
				scanf("%d", &s);
				
				while(s != 1 && s != 0) {
					printf("\nValore non valido! Inserire nuovamente il valore: ");
					fflush(stdin);
					scanf("%d", &s);
				}
				if(s) mettiInCoda(ABR, User);
				
			}
			else {
				carr = inserisciInTesta(carr, ABR->prodottoN);
				printf("\nProdotto messo nel carrello!\nEcco il carrello: \n");
				mostraCarrello(carr);
				
				return carr;
			}
			
		}
		else if(ABR->prodottoN.costo < costo) carr = acquistaProdotto(ABR->dx, nomeProdotto, costo, User, carr);
		else if(ABR->prodottoN.costo > costo) carr = acquistaProdotto(ABR->sx, nomeProdotto, costo, User, carr);	
	}
}

void mettiInCoda(ABRprodotti *ABR, utente *User) {
	int i;
	for(i = 0; ABR->prodottoN.inAttesa[i].username[i] != '\0'; i++);
	
	ABR->prodottoN.inAttesa[i] = *User;
	
	printf("\nUtenti in attesa per questo prodotto: %d\n", i+1);
	
}

Carrello* creaNodo(prodotto Prodotto) {
	Carrello *tmp = (Carrello*)malloc(sizeof(Carrello));
	tmp->Prodotto = Prodotto;
	tmp->next = NULL;
	return tmp;
}

void mostraCarrello(Carrello *carr) {
	if(carr) {
		printf("\nNome prodotto: %s, prezzo: %.2f", carr->Prodotto.nomeProdotto, carr->Prodotto.costo);
		mostraCarrello(carr->next);	
	}	 	
}

Carrello* inserisciInTesta(Carrello *carr, prodotto Prodotto) {
	if(carr == NULL) {
		carr = creaNodo(Prodotto);
		return carr;
	}
	else {
		Carrello *tmp = creaNodo(Prodotto);
		tmp->next = carr;
		return tmp;
	}
}

void calcolaSommaCarrello(Carrello *carrello, float *f)  {
	if(carrello == NULL) {
		return;
	}	
	calcolaSommaCarrello(carrello->next, f);	
	*f += carrello->Prodotto.costo;
}

Carrello* svuotaCarrello(Carrello *carr) {
	if(carr->next == NULL) {
		free(carr);
		return NULL;
	}
	else {
		carr->next = svuotaCarrello(carr->next);
		free(carr);	
		return NULL;
	}
}

