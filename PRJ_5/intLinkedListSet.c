// Procedendo in modo iterativo e incrementale, sviluppare un modulo
// intLinkedListSetADT.c che implementa un insieme di interi (int)
// tramite una lista linkata.

// In particolare, il modulo deve soddisfare la specifica:

// intSetADT.h

// e verrà sottoposto ad una serie di test e di benchmarks per valutare
// l'efficienza della vostra implementazione.

// Per cominciare la vostra implementazione, potete basarvi su questi file di scheletro:

// intLinkedListSet.h
// intLinkedListSetADT_empty.c

// Potete anche verificare il vostro codice in locale usando questa suite di test:

// test_intLinkedListSetADT.c

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "intSetADT.h"

typedef struct listNode ListNode, *ListNodePtr;

struct listNode {
   int data;
   ListNodePtr next;
};

struct intSet {
    ListNodePtr front; /* Punta al primo nodo dell'insieme, se l'insieme e' vuoto vale NULL */
    int size; /* Numero di elementi presenti nell'insieme */
};
/*
    gcc -o test_intLinkedListSetADT test_intLinkedListSetADT.c  intLinkedListSetADT_empty.c
    .\test_intLinkedListSetADT.exe
*/

IntSetADT mkSet() {     //Creazione e inizializzazione di un Set vuoto
    IntSetADT newSet = (IntSetADT)malloc(sizeof(struct intSet));
    if(newSet==NULL){
        return NULL;
    }
    newSet->front = NULL;
    newSet->size = 0;
    return newSet;
}

_Bool dsSet(IntSetADT *sp) { //Distruzione del set, non prima di aver deallocato tutti gli elementi di esso
    if(sp==NULL || *sp == NULL){
        return 0;
    }
    ListNodePtr temp = (*sp)->front;
    while (temp != NULL){
        ListNodePtr currElementTMP = temp;
        free(currElementTMP);
        temp = temp->next;
        currElementTMP = NULL;
        temp = temp->next;
    }
    free(*sp);
    (*sp)->size = 0;
    *sp = NULL;

    return true;
}

ListNodePtr creaNodo(const int elem){       //funzione ausiliaria per creare nodi
    ListNodePtr newNode = (ListNodePtr)malloc(sizeof(ListNode));
    newNode->data = elem;
    newNode->next = NULL;
    if(newNode==NULL){
        return NULL;
    }
    return newNode;
}

_Bool set_add(IntSetADT set, const int elem) {  //Aggiunta di un elemento nel Set
    if(set == NULL){
        return false;
    }
    if(set_member(set, elem)){                  //Se già presente, non lo aggiungiamo
        return false;
    }
    ListNodePtr newNode = creaNodo(elem);
    if(newNode == NULL){
        return false;
    }
    newNode->next = set->front;                 //Aggiungiamo il nuovo nodo in testa e trasliamo
    set->front = newNode;

    set->size++;                                //Aumentiamo il riferimento alla size del Set
    return true;
}

_Bool set_remove(IntSetADT set, const int elem) {
    if(set == NULL || set->front == NULL) {     //Il Set esiste? Ha almeno il front?
        return false;
    }

    ListNodePtr currentTemp = set->front;
    ListNodePtr prev = NULL; // Puntatore al nodo precedente. NULL all'inizio dato che non c'è un precedente al set->front.
    while (currentTemp != NULL) {
        if (currentTemp->data == elem) {
            if (prev == NULL) { // Se il nodo da eliminare è il primo nella lista
                set->front = currentTemp->next; //Trasliamo la lista partendo dal front
            } else {
                prev->next = currentTemp->next; //Trasliamo la lista dall'elemento trovato
            }
            free(currentTemp);                  //Deallocazione
            set->size--;                        //Riduzione della size dopo aver rimosso l'elemento
            return true; // Elemento trovato ed eliminato   
        }
        prev = currentTemp;                     //Andiamo avanti con la lettura della lista
        currentTemp = currentTemp->next;
    }
    
    return false;                               //Non trovato
}

_Bool set_member(const IntSetADT set, const int elem) {
    if(set == NULL || set->front == NULL){ //Il Set esiste? Ha almeno il front?
        return false;
    }
    ListNodePtr currentNode = set->front;
    while(currentNode != NULL){         //Cicliamo sul set. Usciamo se lo troviamo
        if(currentNode->data == elem){
            return true;
        }
        currentNode = currentNode->next;
    }
    return false;                       //Non presente nel set
}

_Bool isEmptySet(const IntSetADT set) {
    if(set == NULL){        //Il Set esiste?
        return false;
    }
    if(set->size == 0){     
        return true;
    }
    return false;
}

int set_size(const IntSetADT set) {
    if(set == NULL){        //Il Set non esiste? -1 
        return -1;
    }
    
    return set->size;       //Ritorno la dimensione, da 0 a n
}

_Bool set_extract(IntSetADT set, int *datap) {
    if(set == NULL || isEmptySet(set) || datap == NULL){  //Il set esiste? è vuoto? Il puntatore passato è valido?
        return false;
    }
    srand(time(NULL));                                    //Inizializzazione seme
    int indice_random = rand() % set->size;               //rand che va da 0 a set->size-1

    ListNodePtr temp = set->front;
    ListNodePtr listaDaLinkare = NULL;

    for (int i = 0; i < indice_random; i++)               //Ciclo fino all'indice da estrarre. Uso un for
    {                                                     //in quanto avendo un range da 0 a n-1 elementi, 
        listaDaLinkare = temp;                            //NON DOVREI (se Dio vuole) leggere puntatori a NULL
        temp = temp->next;
    }

    if(temp == set->front){                               //Il nodo estratto il primo?
        set->front = temp->next;                          //Estrai il primo, slida
    }
    else{
        if(listaDaLinkare != NULL){ 
            listaDaLinkare->next = temp->next;            //aggiorna il puntatore del nodo precedente
        }
    }
//Copia del dato, deallocazione e riduzione della dimensione del set
    *datap = temp->data;   
    free(temp); 
    set->size--;
    
    return true;
}

_Bool set_equals(const IntSetADT set1, const IntSetADT set2) {
    if(set1 == NULL && set2 == NULL){   //Entrambi set nulli
        return true;    
    }
    if(set1 == NULL || set2 == NULL) {  // Uno dei set è nullo e l'altro no. 
        return false; 
    }
    
    if(isEmptySet(set1) && isEmptySet(set2)){ //Entrambi set vuoti
        return true;
    }
    if(isEmptySet(set1) || isEmptySet(set2)) { // Uno dei set è vuoto e l'altro no
        return false; 
    }
    
    if(set1->size != set2->size) { // I set hanno dimensioni diverse, sono diversi
        return false; 
    }
    // Funziona in caso di SET ORDINATI
    // ListNodePtr temp1 = set1->front;
    // ListNodePtr temp2 = set2->front;
    // while(temp1 != NULL  || temp2 != NULL){
    //     if(temp1->data != temp2->data){
    //     return false;
    //     }
    //     temp1 = temp1->next;
    //     temp2 = temp2->next;
    // }
    ListNodePtr temp1 = set1->front;
    while(temp1!=NULL){                         //per ogni iterazione, controlliamo che l'elemento sia presente nel set1/2(sotto fatto lo stesso ciclo). 
        if(!set_member(set2, temp1->data)){     //L'elemento corrente non è presente nel set? 
            return false;                       //set diversi
        }
        temp1 = temp1->next;
    }
    
    ListNodePtr temp2 = set2->front;
    while(temp2!=NULL){
        if(!set_member(set1, temp2->data)){
            return false;
        }
        temp2 = temp2->next;
    }
    
    return true;
}

_Bool subseteq(const IntSetADT set1, const IntSetADT set2) {
    if(set1 == NULL || set2 == NULL){
        return false;
    }
    if(set_equals(set1, set2) || subset(set1, set2)){
        return true;
    }
    return false;
}

_Bool subset(const IntSetADT set1, const IntSetADT set2) {
    if(set1 == NULL || set2 == NULL){
        return false;
    }
    ListNodePtr temp1 = set1->front;

    while (temp1 != NULL)
    {
        if(!set_member(set2, temp1->data)){
            return false;
        }
        temp1 = temp1->next;
    }
    if(set1->size>=set2->size){
        return false;
    }
    return true;    
}

IntSetADT set_union(const IntSetADT set1, const IntSetADT set2) {
    if(set1 == NULL || set2 == NULL){
        return NULL;
    }
    IntSetADT unionSet = mkSet();
    if(unionSet==NULL){
        return NULL;
    }
    ListNodePtr temp1 = set1->front;
    ListNodePtr temp2 = set2->front;
    
    while(temp1 != NULL){
        set_add(unionSet, temp1->data);
        temp1 = temp1->next;
    }
    
    while (temp2!=NULL)
    {
        set_add(unionSet,temp2->data);
        temp2 = temp2->next;
    }
    
    return unionSet;
}

IntSetADT set_intersection(const IntSetADT set1, const IntSetADT set2) {
    if(set1 == NULL || set2 == NULL){
        return NULL;
    }
    IntSetADT intersectionSet = mkSet();
    if(intersectionSet==NULL){
        return NULL;
    }
    ListNodePtr temp1 = set1->front;
    while(temp1 != NULL){
        if(set_member(set2, temp1->data)){
            set_add(intersectionSet, temp1->data);
        }

        temp1 = temp1->next;
    }

    return intersectionSet;
}

IntSetADT set_subtraction(const IntSetADT set1, const IntSetADT set2) {
    if(set1 == NULL || set2 == NULL){
        return NULL;
    }
    IntSetADT subtractionSet = mkSet();
    if(subtractionSet==NULL){
        return NULL;
    }
    ListNodePtr temp1 = set1->front;
    while(temp1 != NULL){
        if(!set_member(set2, temp1->data)){
            set_add(subtractionSet, temp1->data);
        }

        temp1 = temp1->next;
    }
    return subtractionSet;
}