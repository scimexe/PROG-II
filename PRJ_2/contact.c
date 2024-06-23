// Sviluppare un modulo contact.c che implementi la specifica:

// contact.h

// In cui si definisce il seguente tipo di dato Contact: 

// /**  Un tipo di dato per contatti telefonici e cyberspaziali */

// typedef struct contact {

//    char* name;   

//    char* surname;

//    char* mobile;

//    char* url;

// } Contact, *ContactPtr;

// e alcune funzioni di utilità che operano su di esso. 
// Applicare la metodologia TDD, procedendo in modo iterativo e incrementale.

#include "contact.h"
#include <strings.h>
#include <stdlib.h>


_Bool contactEq(Contact c1, Contact c2){
    if(strcasecmp(c1.name, c2.name) == 0 && strcasecmp(c1.surname, c2.surname) == 0){
        return 1;
    }
    return 0;
}

_Bool contactEqEff(const Contact const *pc1, const Contact const *pc2){
    if(pc1 == NULL || pc2==NULL){
        return 0;
    }
    return contactEq(*pc1, *pc2);
}

int contactCmp(Contact c1, Contact c2){
    
    int compareSurname = strcasecmp(c1.surname, c2.surname);
    int compareName    = strcasecmp(c1.name, c2.name);
    
    if (compareSurname==0){
        
        if(compareName<0){
            return -1;
        }
        else if(compareName>0){
            return 1;
        }
        else {
            return 0;
        }
    }
    else if (compareSurname>0){
        return 1;
    }
    else {
        return -1;
    }
}

int contactCmpEff(const Contact const *pc1, const Contact const *pc2){
    return contactCmp(*pc1, *pc2);
}