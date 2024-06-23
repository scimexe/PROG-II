// Procedendo in modo iterativo e incrementale, sviluppare un modulo
// charLinkedListQueueADT.c che implementa una coda di caratteri (char)
// tramite una lista linkata.

// In particolare, il modulo deve soddisfare la specifica charQueueADT.h:

// charQueueADT.h

// e verrà sottoposto ad una serie di test e di benchmarks per valutare
// l'efficienza della vostra implementazione.
#include "charQueueADT.h"

typedef struct listNode ListNode, *ListNodePtr;
struct listNode {
   char data;
   ListNodePtr next;
};

struct charQueue {
    ListNodePtr front;
    ListNodePtr rear; 
    int size;
};

/* @brief Restituisce una coda vuota, se non trova memoria restituisce NULL */
CharQueueADT mkQueue() {
    CharQueueADT newQueue = (CharQueueADT)malloc(sizeof(struct charQueue));
    if(newQueue == NULL)
        return NULL;
        
    newQueue->front = NULL;
    newQueue->rear = NULL;
    newQueue->size = 0;
    return newQueue;
}

/* @brief Distrugge la coda, recuperando la memoria */
void dsQueue(CharQueueADT *pq) {
    free(*pq);
    *pq = NULL;
    return; 
}

/* @brief Aggiunge un elemento in fondo alla coda */
_Bool enqueue(CharQueueADT q, const char e) {
    if (q == NULL) {
        return 0;
    }

    ListNodePtr newElement = (ListNodePtr)malloc(sizeof(struct listNode));
    if (newElement == NULL) {
        return 0;
    }

    newElement->data = e;
    newElement->next = NULL;

    if (q->front == NULL) {
        q->front = newElement;
        q->rear  = newElement;
    } else {
        q->rear->next = newElement;
        q->rear = newElement;
    }

    q->size = q->size+1;

    return 1;
}

/* @brief Toglie e restituisce l'elemento in testa alla coda */
_Bool dequeue(CharQueueADT q, char* res) {
    
    if (q == NULL || q->front == NULL ){
        return 0;
    }
    
    *res = q->front->data;
    
    ListNodePtr temp = q->front;
    
    if (q->front == q->rear) {
        q->front = NULL;
        q->rear = NULL;
    }
    else {                
        q->front = q->front->next;    
    }
    
    free(temp);
    q->size = q->size-1;

    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    return 1;
}

/* @brief Controlla se la coda è vuota */
_Bool isEmpty(CharQueueADT q) {
    return (q == NULL) || (q->front == NULL);
}

/* @brief Restituisce il numero degli elementi presenti nella coda */
int size(CharQueueADT q) {
    return q->size;
}   

/* @brief Restituisce l'elemento nella posizione data (senza toglierlo) */
_Bool peek(CharQueueADT q, int position, char *res) {
    if(q == NULL || q->front == NULL){
        // puts("Esplodo");
        return 0;
    }
    ListNodePtr current = q->front;

    size_t currentPos = 0;
    while(current!= NULL){
        if(currentPos == position){
            *res = current->data;
            return 1;
        }
        current = current->next;
        currentPos++;
    }
    return 0;
}


