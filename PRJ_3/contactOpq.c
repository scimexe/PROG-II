// Sviluppare un modulo contactOpq.c che implementi la specifica:

// contactOpq.h

// In cui si dichiara la struttura opaca: 

// typedef struct contact Contact, *ContactPtr;

// la cui definizione (non accessibile dal client) va posta nel file contactOpq.c:

// struct contact {
//    char* name; 
//    char* surname;
//    char* mobile;
//    char* url;
// };

// assieme alla implementazione di varie funzioni di utilità.  
// Applicare la metodologia TDD, procedendo in modo iterativo e incrementale.

#include "contactOpq.h"
#include <strings.h>

struct contact {
   char* name; 
   char* surname;
   char* mobile;
   char* url;
};

ContactPtr mkContact(char* name, char* surname, char* mobile, char* url){
    
    ContactPtr concactPtr = malloc(sizeof(Contact));
    if(name == NULL || surname == NULL)
        return NULL;
        
    concactPtr->name = name;
    concactPtr->surname = surname;
    concactPtr->mobile = mobile;
    concactPtr->url = url;
    
    return concactPtr;
}

void dsContact(ContactPtr* cntptr){
    free(*cntptr);
    *cntptr = NULL;
    
}

void updateMobile(ContactPtr cnt, char* newMobile){
    cnt -> mobile = newMobile;
}

void updateUrl(ContactPtr cnt, char* newUrl){
    cnt -> url = newUrl;
}

char* getName(const ContactPtr cnt){
    char* str = cnt -> name;
    return str;
}

char* getSurname(const ContactPtr cnt){
    char* str = cnt -> surname;
    return str;
}

char* getMobile(const ContactPtr cnt){
    char* str = cnt -> mobile;
    return str;
}

char* getUrl(const ContactPtr cnt){
    char* str = cnt -> url;
    return str;
}

_Bool equalsContact(const ContactPtr cnt1, const ContactPtr cnt2){
    if(strcasecmp(cnt1->name, cnt2->name)==0 && strcasecmp(cnt1->surname, cnt2->surname)==0)
        return 1;
    return 0;
}

int cmpContact(const ContactPtr cnt1, const ContactPtr cnt2){
    if(strcasecmp(cnt1->surname, cnt2->surname)==0){
        
        if(strcasecmp(cnt1->name, cnt2->name)==0){
            return 0;
        } else if(strcasecmp(cnt1->name, cnt2->name)>0){
            return 1;
        }
        else{
            return -1;
        }
    } else if(strcasecmp(cnt1->surname, cnt2->surname)>0){
        return 1;
    }
    return -1;
}

