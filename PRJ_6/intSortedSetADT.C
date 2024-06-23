#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "intSortedSetADT.h"
#include "intLinkedListSortedSetADT.h"


IntSortedSetADT mkSSet() {
    IntSortedSetADT newSet = (IntSortedSetADT)malloc(sizeof(struct intSortedSet));
    if(newSet == NULL)
        return NULL;
    newSet->first = NULL;
    newSet->last  = NULL;
    newSet->size  = 0;
    return newSet;
}

_Bool dsSSet(IntSortedSetADT *ssptr) {
    if(ssptr == NULL || (*ssptr)==NULL){
        return false;
    }
    ListNodePtr currentNode = (*ssptr)->first;
    while(currentNode != NULL){
        ListNodePtr temp = currentNode;
        currentNode = currentNode->next;
        free(temp);
    }
    
    free((*ssptr));
    (*ssptr) = NULL;
    ssptr = NULL;
    return true;
}

_Bool sset_add(IntSortedSetADT ss, const int elem) {
    if(ss == NULL || sset_member(ss, elem)){
        return false;
    }
    
    ListNodePtr newNode = (ListNodePtr)malloc(sizeof(ListNode));
    if(newNode == NULL){
        return false;
    }
    newNode->elem = elem;
    newNode->next = NULL;
    
    if(isEmptySSet(ss)){            //Set vuoto, il nuovo elemento è sia il primo che l'ultimo
        ss->first = newNode;
        ss->last  = newNode;
        ss->size++;
        return true;
    }
    
    ListNodePtr temp = ss->first;
    ListNodePtr prec = NULL;
    
    while(temp!=NULL && temp->elem < elem){  //ciclo fino ad arrivare alla posizione corretta
        prec = temp;
        temp = temp->next;
    }
    
    if(prec==NULL){     //va inserito per primo
        newNode->next = ss->first;
        ss->first = newNode;
    }
    else if(temp == NULL){
        ss->last->next = newNode;
        ss->last = newNode;
    }
    else{
        newNode->next = temp;
        prec->next = newNode;
    }
    ss->size++;
    return true;
}

_Bool sset_remove(const IntSortedSetADT ss, const int elem) {
    if(ss==NULL || ss->first==NULL || (!sset_member(ss, elem))){
        return false;
    }
    ListNodePtr current = ss->first;
    ListNodePtr prev = NULL;
    while(current!=NULL && current->elem != elem){
        prev = current;
        current = current->next;
    }
    
    if(prev==NULL){
        ss->first = ss->first->next;
        if(ss->first == NULL){
            ss->last = NULL;
        }
        free(current);
    }
    else{
        prev->next = current->next;
        if (current->next == NULL) {
            ss->last = prev;  // Se rimuoviamo l'ultimo nodo, aggiorniamo last
        }
        free(current);
    }
    ss->size--;
    return true;
}

_Bool sset_member(const IntSortedSetADT ss, const int elem) {
    if(ss == NULL || ss->first == NULL){
        return false;
    }
    ListNodePtr temp = ss->first;
    while(temp != NULL){
        if(temp->elem == elem){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

_Bool isEmptySSet(const IntSortedSetADT ss) {
    if(ss == NULL)
        return false;

    if(ss->size == 0){
        return true;
    }
    return false;
}

int sset_size(const IntSortedSetADT ss) {
    if (ss == NULL){
        return -1;
    }
    return ss->size;
}

_Bool sset_extract(IntSortedSetADT ss, int *ptr) {
    if(ss == NULL ||ss->first==NULL || ptr==NULL){
        return false;
    }
    srand(time(NULL));
    
    size_t indice_random = rand() % ss->size;
    ListNodePtr temp = ss->first;
    
    for(size_t i=0; i<indice_random; i++){
        temp = temp->next;
    }
    *ptr = temp->elem;
    if(sset_remove(ss, temp->elem)){
        return true;
    }
    return false;
}

_Bool sset_equals(const IntSortedSetADT s1, const IntSortedSetADT s2) {
    
    if(s1==NULL || s2==NULL){
        return false;
    }
    if(isEmptySSet(s1) && isEmptySSet(s2)){
        return true;
    }
    if(isEmptySSet(s1) || isEmptySSet(s2)){
        return false;
    }
    if(s1->size != s2->size){
        return false;
    }
    
    ListNodePtr temp1 = s1->first;
    ListNodePtr temp2 = s2->first;
    
    while(temp1 && temp2){
        if(temp1->elem != temp2->elem){
            return false;
        }
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    return true;
}

_Bool sset_subseteq(const IntSortedSetADT s1, const IntSortedSetADT s2) {
    if(s1==NULL || s2==NULL){
        return false;
    }
    if(sset_equals(s1, s2) || sset_subset(s1,s2)){
        return true;
    }
    return false;
}

_Bool sset_subset(const IntSortedSetADT s1, const IntSortedSetADT s2) {
    if(s1==NULL || s2==NULL){
        return false;
    }
    if(s1->size >= s2->size){
        return false;
    }
    ListNodePtr temp1 = s1->first;

    while (temp1 != NULL)
    {
        if(!sset_member(s2, temp1->elem)){
            return false;
        }
        temp1 = temp1->next;
    }
    return true;
}

IntSortedSetADT sset_union(const IntSortedSetADT s1, const IntSortedSetADT s2) {
    if(s1 == NULL || s2 == NULL){
        return NULL;
    }
    IntSortedSetADT unionSet = mkSSet();
    if(unionSet==NULL){
        return NULL;
    }
    ListNodePtr temp1 = s1->first;
    ListNodePtr temp2 = s2->first;
    
    while(temp1 != NULL){
        sset_add(unionSet, temp1->elem);
        temp1 = temp1->next;
    }
    
    while (temp2!=NULL)
    {
        sset_add(unionSet,temp2->elem);
        temp2 = temp2->next;
    }
    
    return unionSet;
}

IntSortedSetADT sset_intersection(const IntSortedSetADT s1, const IntSortedSetADT s2) {
    if(s1 == NULL || s2 == NULL){
        return NULL;
    }
    IntSortedSetADT intersectionSet = mkSSet();
    if(intersectionSet==NULL){
        return NULL;
    }
    ListNodePtr temp1 = s1->first;
    while(temp1 != NULL){
        if(sset_member(s2, temp1->elem)){
            sset_add(intersectionSet, temp1->elem);
        }

        temp1 = temp1->next;
    }

    return intersectionSet;
}

IntSortedSetADT sset_subtraction(const IntSortedSetADT s1, const IntSortedSetADT s2) {
   if(s1 == NULL || s2 == NULL){
        return NULL;
    }
    IntSortedSetADT subtractionSet = mkSSet();
    if(subtractionSet==NULL){
        return NULL;
    }
    ListNodePtr temp1 = s1->first;
    while(temp1 != NULL){
        if(!sset_member(s2, temp1->elem)){
            sset_add(subtractionSet, temp1->elem);
        }

        temp1 = temp1->next;
    }

    return subtractionSet;  
}

_Bool sset_min(const IntSortedSetADT ss, int *ptr) {
    if(ss==NULL || isEmptySSet(ss) || ptr == NULL){
        return false;
    }
    *ptr = ss->first->elem;
    return true;
}

_Bool sset_max(const IntSortedSetADT ss, int *ptr) {
    if(ss==NULL || isEmptySSet(ss) || ptr == NULL){
        return false;
    }
    *ptr = ss->last->elem;
    return true;
}

_Bool sset_extractMin(IntSortedSetADT ss, int *ptr) {
    if(ss==NULL || isEmptySSet(ss) || ptr == NULL){
        return false;
    }
    ListNodePtr temp = ss->first;
    *ptr = ss->first->elem; 
    ss->first = ss->first->next;
    free(temp);
    
    return true;
}

_Bool sset_extractMax(IntSortedSetADT ss, int *ptr) {
    if(ss==NULL || isEmptySSet(ss) || ptr == NULL){
        return false;
    }
    *ptr = ss->last->elem;  
        
    ListNodePtr temp = ss->first;
    ListNodePtr prec = NULL;
    
    while(temp->next != NULL){
        prec = temp;
        temp = temp->next;
    }

    if (prec == NULL) {
        ss->first = NULL;
    } else {
        prec->next = NULL;
    }
    
    if (ss->last == temp) {
        ss->last = prec;
    }
    
    free(temp);
    return true;
}
