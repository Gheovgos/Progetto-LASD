#include <liste.h>

/*FILE HEADER DI DEFINIZIONE UTENTE, STRUTTURE DATI E FUNZIONI*/

//Definizione di Utente
typedef struct User{
	char username[50]; 
	char password[50]; 
	float saldo;
	int isSU; //indica se è un Super User (Admin)
	//nodiAlbero carrello;
}utente;

//Definizione di un prodotto
typedef struct Prodotto{
	char nomeProdotto[50]; 
	float costo;
	int taglia;
	utente inAttesa[]; //allocazione dinamica di un vettore utenti che attendono un determinato prodotto di una determinata taglia
}prodotto;

//Definizione di un ABR di prodotti
typedef struct ABR{
	prodotto prodottoN;
	struct ABR* dx;
	struct ABR* sx;
}ABRprodotti;


int login(utente *User, char *usernm, char *psswd, int SU); //Funzione di accesso di un Utente
void singup(); //funzione di registrazione di un utente



