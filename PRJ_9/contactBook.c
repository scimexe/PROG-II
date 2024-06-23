#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "contactBook.h"
#include "sortedSetADT.h"
#include "contactOpq.h"
#include "binarySearchTreeSortedSetADT.h"

#define MAX 100

/**
 Un tipo di dato per una rubrica di contatti
*/

struct contactBookADT {
    SortedSetADTptr contacts; // Tutto quello che serve è già in questa struttura
};

// ----------------------------------------------------------------------------------------------------

void stampaContact(void* elem) {
    ContactPtr contact = (ContactPtr)elem;
    printf("%s %s: %s (%s)", getName(contact), getSurname(contact), getMobile(contact), getUrl(contact));
}

int contact_cmp(void* c1, void* c2) {
    return cmpContact((ContactPtr)c1, (ContactPtr)c2);
}

// ------------------------------------------------------------------------------------------------------

// restituisce una rubrica vuota, NULL se errore
ContactBookADTptr mkCBook() {
    ContactBookADTptr book = (ContactBookADTptr)malloc(sizeof(ContactBookADT));
    if (book == NULL) return NULL;

    book->contacts = mkSSet(contact_cmp);
    return book;
}

// distrugge la rubrica, recuperando la memoria, false se errore
_Bool dsCBook(ContactBookADTptr* book) {
    if (book == NULL || *book == NULL) return false;

    dsSSet(&((*book)->contacts));
    *book = NULL;
    return true;
}

// controlla se la rubrica e' vuota, -1 se NULL
int isEmptyCBook(const ContactBookADT* book) {
    if (book == NULL) return -1;

    return isEmptySSet(book->contacts);
}

// restituisce il numero di contatti presenti nella rubrica, -1 se NULL
int cbook_size(const ContactBookADT* book) {
    if (book == NULL) return -1;

    return sset_size(book->contacts);
}

// restituisce un puntatore al contatto con dato nome e cognome (può essere usata per accedere o modificare il numero e url del contatto), NULL se non presente
ContactPtr cbook_search(const ContactBookADT* book, char* name, char* surname) {
    if (book == NULL || name == NULL || surname == NULL) return NULL;

    ContactPtr tmp = mkContact(name, surname, NULL, NULL);

    void* cnt = sset_search(book->contacts, tmp);
    return (ContactPtr)cnt;
}

// aggiunge un contatto alla rubrica (restituisce false se l'elemento era gia' presente, true altrimenti)
_Bool cbook_add(ContactBookADTptr book, ContactPtr cnt) {
    if (book == NULL) return false;

    return sset_add(book->contacts, cnt);
}

// toglie un elemento all'insieme (restituisce false se l'elemento non era presente, true altrimenti)
_Bool cbook_remove(ContactBookADTptr book, char* name, char* surname) {
    if (book == NULL) return false;

    ContactPtr cnt = mkContact(name, surname, NULL, NULL);
    return sset_remove(book->contacts, cnt);
}

// carica una rubrica da file, NULL se errore
ContactBookADTptr cbook_load(FILE* fin) {
    if(!fin) return NULL;

    ContactBookADTptr book = mkCBook();
    if(!book) return NULL;

    int retn, rets, retm, retu;
    while(fscanf(fin, "\n") == 0 && !feof(fin)){
        char* name = malloc(sizeof(char)*100);
        char* surname = malloc(sizeof(char)*100);
        char* mobile = malloc(sizeof(char)*100);
        char* url = malloc(sizeof(char)*100);

        retn = fscanf(fin, "%100[^,],", name);
        rets = fscanf(fin, "%100[^,],", surname);
        retm = fscanf(fin, "%100[^,],", mobile);
        retu = fscanf(fin, "%100[^\n]", url);

        ContactPtr cnt = mkContact((retn > 0) ? name : NULL, (rets > 0) ? surname : NULL, (retm > 0) ? mobile : "", (retu > 0) ? url : "");
        if(cnt)
            cbook_add(book, cnt);
        else 
            return NULL;
    }

    return book;
}

// salva una rubrica su file, false se errore
_Bool cbook_dump(const ContactBookADT* book, FILE* fout) {
    if(!book || !fout)
        return false;

    ContactPtr* arrayBook=(ContactPtr*)sset_toArray(book->contacts);
    if(arrayBook) {
        int size = sset_size(book->contacts);

        for(int i = 0; i < size; i++){
            //stampaContact(arrayBook[i]);
            fprintf(fout, "%s,", getName(arrayBook[i]));
            fprintf(fout, "%s,", getSurname(arrayBook[i]));
            fprintf(fout, "%s,", getMobile(arrayBook[i]));
            fprintf(fout, "%s\n", getUrl(arrayBook[i]));
        }
        free(arrayBook);
        return true;
    }
    
    return false;
}