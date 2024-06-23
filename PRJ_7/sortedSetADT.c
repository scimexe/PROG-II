#include "sortedSetADT.h"
#include <time.h>
typedef struct listNode ListNode, *ListNodePtr;

struct listNode {
   void* elem;
   ListNodePtr next;
};

struct sortedSetADT {
    ListNodePtr first; /* Punta al primo nodo dell'insieme, se l'insieme e' vuoto vale NULL */
    ListNodePtr last; /* Punta all'ultimo nodo dell'insieme, se l'insieme e' vuoto vale NULL */
    int (*compare)(void*, void*); /* confronto tra due elementi: -1,0,1 se primo precede, uguale o segue il secondo */
    int size; /* Numero di elementi presenti nell'insieme */
};


// restituisce un insieme vuoto impostando funzione di confronto, NULL se errore
SortedSetADTptr mkSSet(int (*compare)(void*, void*)) {
    if (compare == NULL) {
        return NULL;
    }

    SortedSetADTptr newSet = (SortedSetADTptr)malloc(sizeof(SortedSetADT));
    if (newSet == NULL) {
        return NULL;
    }

    newSet->first = NULL;
    newSet->last = NULL;
    newSet->compare = compare;
    newSet->size = 0;

    return newSet;  
}

// distrugge l'insieme, recuperando la memoria
_Bool dsSSet(SortedSetADTptr* ssptr) {
    if (ssptr == NULL || *ssptr == NULL) {
        return false;
    }

    ListNodePtr current = (*ssptr)->first;
    while (current != NULL) {
        ListNodePtr temp = current;
        current = current->next;
        free(temp);
    }

    free(*ssptr);
    *ssptr = NULL;
    return true;
}

// aggiunge un elemento all'insieme 
_Bool sset_add(SortedSetADTptr ss, void* elem) { 
    if (ss == NULL || elem == NULL) {
        return false;
    }

    ListNodePtr newNode = (ListNodePtr)malloc(sizeof(ListNode));
    if (newNode == NULL) {
        return false;
    }

    newNode->elem = elem;
    newNode->next = NULL;

    if (ss->first == NULL) { // L'insieme è vuoto
        ss->first = newNode;
        ss->last = newNode;
    } else {
        ListNodePtr prev = NULL, current = ss->first;
        while (current != NULL && ss->compare(current->elem, elem) < 0) {
            prev = current;
            current = current->next;
        }

        if (current != NULL && ss->compare(current->elem, elem) == 0) {
            free(newNode);
            return false; // Elemento già presente
        }

        if (prev == NULL) { // Inserimento in testa
            newNode->next = ss->first;
            ss->first = newNode;
        } else {
            newNode->next = current;
            prev->next = newNode;
            if (current == NULL) { // Inserimento in coda
                ss->last = newNode;
            }
        }
    }

    ss->size++;
    return true;
}  

// toglie un elemento all'insieme 
_Bool sset_remove(SortedSetADTptr ss, void* elem) {
    if (ss == NULL || elem == NULL) {
        return false;
    }

    ListNodePtr prev = NULL, current = ss->first;
    while (current != NULL && ss->compare(current->elem, elem) < 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL || ss->compare(current->elem, elem) != 0) {
        return false; // Elemento non presente
    }

    if (prev == NULL) {
        ss->first = current->next;
        if (ss->first == NULL) {
            ss->last = NULL;
        }
    } else {
        prev->next = current->next;
        if (current->next == NULL) {
            ss->last = prev;
        }
    }

    free(current);
    ss->size--;
    return true;
}

// controlla se un elemento appartiene all'insieme
int sset_member(const SortedSetADT* ss, void* elem) {
    if (ss == NULL || elem == NULL) {
        return -1;
    }

    ListNodePtr current = ss->first;
    while (current != NULL) {
        int cmp = ss->compare(current->elem, elem);
        if (cmp == 0) {
            return 1; // Elemento trovato
        }
        if (cmp > 0) {
            break; // Elemento non trovato. Non può esserci in quanto il set è ordinato
        }
        current = current->next;
    }
    return 0;
}
    
// controlla se l'insieme e' vuoto    
int isEmptySSet(const SortedSetADT* ss) {
    if (ss == NULL) {
        return -1;
    }
    return ss->size == 0;
} 

// restituisce il numero di elementi presenti nell'insieme
int sset_size(const SortedSetADT* ss) {
    if (ss == NULL) {
        return -1;
    }
    return ss->size;
} 

// toglie e restituisce un elemento a caso dall'insieme
_Bool sset_extract(SortedSetADTptr ss, void**ptr) {
    if (ss == NULL || ss->first == NULL || ptr == NULL) {
        return false;
    }
    srand(time(NULL));
    // Genera un indice casuale tra 0 e size - 1
    int randomIndex = rand() % ss->size;

    ListNodePtr prev = NULL;
    ListNodePtr current = ss->first;

    // Trova il nodo all'indice casuale
    for (int i = 0; i < randomIndex; i++) {
        prev = current;
        current = current->next;
    }

    // Estrae l'elemento
    *ptr = current->elem;

    // Rimuove il nodo dall'insieme
    if (prev == NULL) {
        // Il nodo da rimuovere è il primo nodo
        ss->first = current->next;
    } else {
        prev->next = current->next;
    }

    if (current == ss->last) {
        // Il nodo da rimuovere è l'ultimo nodo
        ss->last = prev;
    }

    free(current);
    ss->size--;

    return true;
} 

// controlla se due insiemi sono uguali
int sset_equals(const SortedSetADT* s1, const SortedSetADT* s2) { 
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    if (s1->size != s2->size) {
        return 0;
    }

    ListNodePtr current1 = s1->first;
    ListNodePtr current2 = s2->first;

    while (current1 != NULL && current2 != NULL) {
        if (s1->compare(current1->elem, current2->elem) != 0) {
            return 0;
        }
        current1 = current1->next;
        current2 = current2->next;
    }

    return 1;
}

// controlla se il primo insieme e' incluso nel secondo
int sset_subseteq(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    ListNodePtr current1 = s1->first;
    ListNodePtr current2 = s2->first;

    while (current1 != NULL) {
        while (current2 != NULL && s2->compare(current2->elem, current1->elem) < 0) {
            current2 = current2->next;
        }

        if (current2 == NULL || s2->compare(current2->elem, current1->elem) != 0) {
            return 0;
        }

        current1 = current1->next;
    }

    return 1;
}

// controlla se il primo insieme e' incluso strettamente nel secondo
int sset_subset(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;
    }

    if (s1->size >= s2->size) {
        return 0;
    }

    return sset_subseteq(s1, s2);
} 

// restituisce la sottrazione primo insieme meno il secondo, NULL se errore
SortedSetADTptr sset_subtraction(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    SortedSetADTptr result = mkSSet(s1->compare);
    if (result == NULL) {
        return NULL;
    }

    ListNodePtr current1 = s1->first;
    ListNodePtr current2 = s2->first;

    while (current1 != NULL) {
        while (current2 != NULL && s2->compare(current2->elem, current1->elem) < 0) {
            current2 = current2->next;
        }

        if (current2 == NULL || s2->compare(current2->elem, current1->elem) != 0) {
            sset_add(result, current1->elem);
        }

        current1 = current1->next;
    }

    return result; 
} 

// restituisce l'unione di due insiemi, NULL se errore
SortedSetADTptr sset_union(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    SortedSetADTptr result = mkSSet(s1->compare);
    if (result == NULL) {
        return NULL;
    }

    ListNodePtr current1 = s1->first;
    ListNodePtr current2 = s2->first;

    while (current1 != NULL && current2 != NULL) {
        int cmp = s1->compare(current1->elem, current2->elem);
        if (cmp < 0) {
            sset_add(result, current1->elem);
            current1 = current1->next;
        } else if (cmp > 0) {
            sset_add(result, current2->elem);
            current2 = current2->next;
        } else {
            sset_add(result, current1->elem);
            current1 = current1->next;
            current2 = current2->next;
        }
    }

    while (current1 != NULL) {
        sset_add(result, current1->elem);
        current1 = current1->next;
    }

    while (current2 != NULL) {
        sset_add(result, current2->elem);
        current2 = current2->next;
    }

    return result; 
} 

// restituisce l'intersezione di due insiemi, NULL se errore
SortedSetADTptr sset_intersection(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return NULL;
    }

    SortedSetADTptr result = mkSSet(s1->compare);
    if (result == NULL) {
        return NULL;
    }

    ListNodePtr current1 = s1->first;
    ListNodePtr current2 = s2->first;

    while (current1 != NULL && current2 != NULL) {
        int cmp = s1->compare(current1->elem, current2->elem);
        if (cmp < 0) {
            current1 = current1->next;
        } else if (cmp > 0) {
            current2 = current2->next;
        } else {
            sset_add(result, current1->elem);
            current1 = current1->next;
            current2 = current2->next;
        }
    }

    return result;
}

// restituisce il primo elemento 
_Bool sset_min(const SortedSetADT* ss, void**ptr) {
    if (ss == NULL || ss->first == NULL || ptr == NULL) {
        return false;
    }

    *ptr = ss->first->elem;
    return true;
}

// restituisce l'ultimo elemento 
_Bool sset_max(const SortedSetADT* ss, void**ptr) {
    if (ss == NULL || ss->last == NULL || ptr == NULL) {
        return false;
    }

    *ptr = ss->last->elem;
    return true;
}

// toglie e restituisce il primo elemento 
_Bool sset_extractMin(SortedSetADTptr ss, void**ptr) {
    if (ss == NULL || ss->first == NULL || ptr == NULL) {
        return false;
    }

    ListNodePtr nodeToRemove = ss->first;
    *ptr = nodeToRemove->elem;

    ss->first = nodeToRemove->next;
    if (ss->first == NULL) {
        ss->last = NULL;
    }

    free(nodeToRemove);
    ss->size--;
    return true;       
}

// toglie e restituisce l'ultimo elemento
_Bool sset_extractMax(SortedSetADTptr ss, void**ptr) {
    if (ss == NULL || ss->last == NULL || ptr == NULL) {
        return false;
    }

    ListNodePtr prev = NULL, current = ss->first;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    *ptr = current->elem;

    if (prev == NULL) {
        ss->first = NULL;
        ss->last = NULL;
    } else {
        prev->next = NULL;
        ss->last = prev;
    }

    free(current);
    ss->size--;
    return true;      
}

