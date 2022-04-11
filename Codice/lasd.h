#include <stdio.h>
#include <stdlib.h>

typedef struct Lista_Dp {
	int val;
	struct Lista_Dp* next;
	struct Lista_Dp* prev;
}ListaDp;

static ListaDp* NuovoEl(int dato) {
	ListaDp* tmp;
	tmp = (ListaDp*)malloc(sizeof(ListaDp));
	tmp->val = dato;
	tmp->next = NULL;
	tmp->prev = NULL;
	return tmp;
}

ListaDp* Inserisci(ListaDp* testa, int dato) {
	if (testa == NULL)
	{
		testa = NuovoEl(dato);
		return testa;
	}
	else
	{
		if (testa->val < dato)
		{
			if (testa->next == NULL)
			{
				ListaDp* tmp = (ListaDp*)malloc(sizeof(ListaDp));
				tmp = Inserisci(testa->next, dato);
				tmp->prev = testa;
				testa->next = tmp;
				return testa;
			}
			else
			{
				testa->next = Inserisci(testa->next, dato);
				return testa;
			}
		}
		else
		{
			ListaDp* tmp;
			tmp = NuovoEl(dato);
			if (testa->prev == NULL)
			{
				tmp->next = testa;
				testa->prev = tmp;
				tmp->prev = NULL;
				return tmp;
			}
			else
			{
				tmp->next = testa;
				tmp->prev = testa->prev;
				testa->prev->next = tmp;
				testa->prev = tmp;
				return tmp;
			}
		}
	}
}

static void StampaListaDpDecrescente(ListaDp* testa) {
	if (testa->next == NULL) {
		printf("//\n");
	}
	else {
		printf("-> %d", testa->val);
		StampaListaDpDecrescente(testa->next);
	}
}
static void StampaListaDpCrescente(ListaDp* testa) {
	if (testa->next != NULL)
	{
		StampaListaDpCrescente(testa->next);
	}
	if (testa->next != NULL)
	{
			printf("-> %d", testa->val);
	}
}
void SelezionaStampaDp(ListaDp* testa) {
	int scelta;
	printf("\nseleziona 1 se vuoi la lista in ordine crescente\nseleziona 0 se vuoi la lista in ordine decrescente: ->");
	do {
		scanf("%d", &scelta);
		if (scelta == 1)
		{
			StampaListaDpCrescente(testa);
		}
		if (scelta == 0)
		{
			StampaListaDpDecrescente(testa);
		}
		else
		{
			("\nPuoi inserire solo 0 o 1");
		}
	} while (scelta != 1 && scelta != 0);
}

int sizeDp(ListaDp* testa) {
	int c = 0;
	if(testa->next == NULL)
		return 0;
	else c = 1 + sizeDp(testa->next);

	return c;	
}

ListaDp* eliminaElemDp(ListaDp* testa, int n) {	
	if(testa == NULL) {
		printf("\nException!\n");
		return NULL;
	}
	else {
		ListaDp* nuovaTesta;
		if(testa->val == n && testa->prev == NULL) { //è una eliminazione dalla testa
			nuovaTesta = testa->next;
			nuovaTesta->prev = NULL;
		
			free(testa);
			return nuovaTesta;
    	}
    	if(testa->val == n && testa->next == NULL) { //è una eliminazione dalla coda
			nuovaTesta = testa->next;
			testa->prev->next = NULL;

			free(testa);
			return nuovaTesta;
    	}
    	if(testa->val == n && testa->next != NULL && testa->prev != NULL) { //è una eliminazione in mezzo
			
			nuovaTesta = testa->next;
			testa->next->prev = testa->prev;
			testa->prev->next = testa->next;
			
			free(testa);
			return nuovaTesta;
    	}
    		
		else {
			
			testa->next = eliminaElemDp(testa->next, n);
		
	    }	
	}
	
}

void stampaListaDp(ListaDp* testa) {
	if(testa == NULL) {
		printf("NULL\n");
		return;
	}
	else {
		printf("%d -> ", testa->val);
		stampaListaDp(testa->next);
	}
	
}
