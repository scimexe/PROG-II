#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "sortedSetADT.h"

typedef struct treeNode TreeNode, *TreeNodePtr;


void stampaSet_rec(TreeNodePtr cur, void (*stampaelem)(void*)) {
    #ifdef DEBUG
    printf("( ");
    #endif
    if (cur) {
        stampaSet_rec(cur->left, stampaelem);
        (*stampaelem)(cur->elem);
        stampaSet_rec(cur->right, stampaelem);
    }
    #ifdef DEBUG
    printf(") ");
    #endif
}

// stampa i contenuti dell'insieme, data la funzione di stampa di un elemento
void stampaSet(SortedSetADTptr ss, void (*stampaelem)(void*)) {
    if(!ss) printf("Insieme non esiste\n");
    else if(sset_size(ss) == 0) printf("Insieme vuoto\n");
    else {
        printf("Insieme: (size %d) ",ss->size);
        stampaSet_rec(ss->root, stampaelem);
        printf("\n");
    }
}

// restituisce un insieme vuoto impostando funzione di confronto, NULL se errore
SortedSetADTptr mkSSet(int (*compare)(void*, void*)) {
    SortedSetADTptr newTree = malloc(sizeof(struct sortedSetADT));
    if(newTree != NULL){
        newTree->root = NULL;
        newTree->compare = compare;
        newTree->size = 0;
    }
    return newTree;
}

void destroyTree_rec(TreeNodePtr cur) {
    if (cur != NULL) {
        destroyTree_rec(cur->left);  // Visita il sottoalbero sinistro
        destroyTree_rec(cur->right); // Visita il sottoalbero destro
        free(cur);                   // Dealloca il nodo corrente
    }
}

// Funzione per distruggere completamente l'insieme
_Bool dsSSet(SortedSetADTptr* ssptr) {
    if (ssptr != NULL && *ssptr != NULL) {
        destroyTree_rec((*ssptr)->root); // Dealloca tutti i nodi dell'albero
        free(*ssptr);                    // Dealloca la struttura dell'insieme
        *ssptr = NULL;                   // Imposta il puntatore a NULL per indicare che l'insieme è stato distrutto
        return true;
    }
    return false;
}

// aggiunge un elemento all'insieme 
_Bool sset_add(SortedSetADTptr ss, void* elem) {
    if (ss == NULL) {
        return false;
    }

    TreeNodePtr *current = &ss->root;
    while (*current != NULL) {
        if (elem < (*current)->elem) {
            current = &(*current)->left;
        } else if (elem > (*current)->elem) {
            current = &(*current)->right;
        } else {
            // Element already in the set
            return false;
        }
    }

    // Allocate new node
    TreeNodePtr newNode = (TreeNodePtr)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        return false;
    }
    newNode->elem = elem;
    newNode->left = NULL;
    newNode->right = NULL;

    // Attach new node
    ss->size++;
    *current = newNode;
    return true;
}

void sset_extractMin_rec(TreeNodePtr* cur, void**ptr, int (*compare)(void*, void*));
void sset_extractMax_rec(TreeNodePtr* cur, void**ptr, int (*compare)(void*, void*));

// funzione ausiliaria che toglie un elemento da un sottoalbero
_Bool sset_remove_rec(TreeNodePtr* cur, void* elem, int (*compare)(void*, void*)) {
    if (*cur == NULL) return false;
    int r = compare(elem, (*cur)->elem);
    if (r < 0) return sset_remove_rec(&((*cur)->left), elem, compare);
    if (r > 0) return sset_remove_rec(&((*cur)->right), elem, compare);
    void* ptr;
    if ((*cur)->left == NULL) {
        sset_extractMin_rec(cur, &ptr, compare);
    } else if ((*cur)->right == NULL) {
        sset_extractMax_rec(cur, &ptr, compare);
    } else {
        sset_extractMax_rec(&((*cur)->left), &ptr, compare);
        (*cur)->elem = ptr;
    }
    return true;
}

// toglie un elemento all'insieme
_Bool sset_remove(SortedSetADTptr ss, void* elem) {
    if (ss && sset_remove_rec(&(ss->root), elem, ss->compare)) {
        ss->size--;
        return true;
    }
    return false;

}

// controlla se un elemento appartiene all'insieme
int sset_member(const SortedSetADT* ss, void* elem) {
    if(ss != NULL){
        TreeNodePtr cur = ss->root;
        while (cur != NULL)
        {
            if(elem < cur->elem){
                cur = cur->left;
            }else if(elem > cur->elem){
                cur = cur->right;
            }else{
                return 1;
            }
        }
        
        return 0;

    }else{
        return -1;
    }
}

// cerca un elemento nell'insieme che si compara uguale a quello dato, NULL se non trovato
void* sset_search(const SortedSetADT* ss, void* elem) {
    if (ss != NULL) {
        TreeNodePtr cur = ss->root;
        while (cur != NULL) {
            if (elem < cur->elem) {
                cur = cur->left;
            } else if (elem > cur->elem) {
                cur = cur->right;
            } else {
                return cur->elem;  // Restituisce l'elemento trovato
            }
        }
        return NULL;  // Elemento non trovato
    } else {
        return NULL;  // SortedSetADT è NULL
    }
}

// controlla se l'insieme e' vuoto
int isEmptySSet(const SortedSetADT* ss) {
    if(ss == NULL){
        return -1;
    }
    else if(ss->size == 0){
        return -1;
    }
    else{
        return 0;
    }
} 

// restituisce il numero di elementi presenti nell'insieme
int sset_size(const SortedSetADT* ss) {
    if(ss != NULL){
        int ret  = ss->size;
        return ret;
    }
    else{
        return -1;
    }
} 

// toglie e restituisce un elemento a caso dall'insieme
_Bool sset_extract(SortedSetADTptr ss, void**ptr) {
    if(ss == NULL || ss->size == 0){
        return false;
    }

    int index = rand() % ss->size;
    TreeNodePtr cur = ss->root;

    //Troviamo elemento radom 
    for(int i=0; i<index; i++){
        if(cur->left != NULL){
            cur = cur->left;
        }else if(cur->right != NULL){
            cur = cur->right;
        }else{
            break;
        }
    }

    *ptr = cur->elem;

    sset_remove(ss,*ptr);

    return true;
} 

// controlla se due insiemi sono uguali
int sset_equals(const SortedSetADT* s1, const SortedSetADT* s2) { 
    if(s1 == NULL && s2==NULL){
        return 1;
    }else if((s1 == NULL ) ^ (s2 == NULL)){
        return -1;
    }else{
        if(s1->size != s2->size){
            return 0;
        }else{
            TreeNodePtr current1 = s1->root;
            TreeNodePtr current2 = s2->root;
            bool ret = -1;
            for(int i=0; i<s1->size; i++){
                if(current1->elem != current2->elem){
                    return ret;
                }

                if(current1->left != NULL){
                    current1 = current1->left;
                    current2 = current2->left;
                }else if(current1->right != NULL){
                    current1 = current1->right;
                    current2 = current2->right;
                }else{
                    ret = 1;
                }
            }

            return ret;

        }
    }
}
//funzione ausiliaria per fare la ricorsione su gli alberi 
int isSubseteq(TreeNodePtr node1, TreeNodePtr node2) {
    if (node1 == NULL) {
        return 1;  // Un insieme vuoto è sempre un sottoinsieme
    }
    if (node2 == NULL) {
        return -1;  // Se node1 non è NULL e node2 è NULL, node1 non può essere un sottoinsieme
    }

    if (node1->elem < node2->elem) {
        return isSubseteq(node1, node2->left);
    } else if (node1->elem > node2->elem) {
        return isSubseteq(node1, node2->right);
    } else {
        return isSubseteq(node1->left, node2->left) && isSubseteq(node1->right, node2->right);
    }
}

 // controlla se due insiemi sono uguali
int sset_subseteq(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;  // O entrambi NULL, o almeno uno è NULL
    }
    if (s1->size > s2->size) {
        return 0;  // Se s1 è più grande di s2, non può essere un sottoinsieme
    }
    return isSubseteq(s1->root, s2->root);
}

//funzione ausiliaria per fare la ricorsione su gli alberi 
int isSubset(TreeNodePtr node1, TreeNodePtr node2) {
    if (node1 == NULL) {
        return 1;  // Un insieme vuoto è sempre un sottoinsieme
    }
    if (node2 == NULL) {
        return -1;  // Se node1 non è NULL e node2 è NULL, node1 non può essere un sottoinsieme
    }

    if (node1->elem < node2->elem) {
        return isSubset(node1, node2->left);
    } else if (node1->elem > node2->elem) {
        return isSubset(node1, node2->right);
    } else {
        return isSubset(node1->left, node2->left) && isSubset(node1->right, node2->right);
    }
}

// controlla se il primo insieme e' incluso strettamente nel secondo
int sset_subset(const SortedSetADT* s1, const SortedSetADT* s2) {
    if (s1 == NULL || s2 == NULL) {
        return -1;  // O entrambi NULL, o almeno uno è NULL
    }
    if (s1->size >= s2->size) {
        return 0;  // Se s1 è più grande di s2, non può essere un sottoinsieme
    }
    return isSubset(s1->root, s2->root);
} 

// restituisce la sottrazione primo insieme meno il secondo, NULL se errore
SortedSetADTptr sset_subtraction(const SortedSetADT* s1, const SortedSetADT* s2) {
    if(s1 == NULL || s2 == NULL){
        return NULL;
    }

    SortedSetADTptr setSub = mkSSet(s1->compare);
    TreeNodePtr cur1 = s1->root;
        while (cur1 != NULL){
            sset_add(setSub,cur1->elem);
            if(cur1->left != NULL){
                cur1 = cur1->left;
            }else if(cur1->right != NULL){
                cur1 = cur1->right;
            }else{
                break;
            }
        }
    
    TreeNodePtr cur2 = s2->root;
        while (cur2 != NULL){
            if(sset_member(setSub,cur2->elem)){
                sset_remove(setSub,cur2->elem);
            }
            
            if(cur2->left != NULL){
                cur2 = cur2->left;
            }else if(cur2->right != NULL){
                cur2 = cur2->right;
            }else{
                break;
            }
        }
    return setSub;        
} 

// restituisce l'unione di due insiemi, NULL se errore
SortedSetADTptr sset_union(const SortedSetADT* s1, const SortedSetADT* s2) {
    if(s1 == NULL || s2 == NULL){
        return NULL;
    }

    SortedSetADTptr unionSet = mkSSet(s1->compare);
    TreeNodePtr cur1 = s1->root;
        while (cur1 != NULL){
            sset_add(unionSet,cur1->elem);
            if(cur1->left != NULL){
                cur1 = cur1->left;
            }else if(cur1->right != NULL){
                cur1 = cur1->right;
            }else{
                break;
            }
        }
    
    TreeNodePtr cur2 = s2->root;
        while (cur2 != NULL){
            sset_add(unionSet,cur2->elem);
            if(cur2->left != NULL){
                cur2 = cur2->left;
            }else if(cur2->right != NULL){
                cur2 = cur2->right;
            }else{
                break;
            }
        }
    return unionSet;   
} 

// restituisce l'intersezione di due insiemi, NULL se errore
SortedSetADTptr sset_intersection(const SortedSetADT* s1, const SortedSetADT* s2) {
    if(s1 == NULL || s2 == NULL){
        return NULL;
    }

    SortedSetADTptr setIn = mkSSet(s1->compare);
    TreeNodePtr cur = s1->root;
        while (cur != NULL){

            if(sset_member(s2,cur->elem)){
                sset_add(setIn,cur->elem);
            }

            if(cur->left != NULL){
                cur = cur->left;
            }else if(cur->right != NULL){
                cur = cur->right;
            }else{
                break;
            }
        }
    return setIn;
}

// restituisce il primo elemento 
_Bool sset_min(const SortedSetADT* ss, void**ptr) {
    if(ss == NULL || ss->size == 0){
        return 0;
    }

    TreeNodePtr cur = ss->root;
    while (cur->left !=  NULL){
        cur = cur->left;
    }

    *ptr = cur->elem;
    return 1;
}

// restituisce l'ultimo elemento 
_Bool sset_max(const SortedSetADT* ss, void**ptr) {
    if(ss == NULL || ss->size == 0){
        return 0;
    }

    TreeNodePtr cur = ss->root;
    while (cur->right !=  NULL){
        cur = cur->right;
    }

    *ptr = cur->elem;
    return 1;
}


void sset_extractMin_rec(TreeNodePtr* cur, void**ptr, int (*compare)(void*, void*)) {
    if ((*cur)->left) sset_extractMin_rec(&((*cur)->left), ptr, compare);
    else {
        (*ptr) = (*cur)->elem;
        if ((*cur)->right) {
            TreeNodePtr tmp = *cur;
            (*cur) = (*cur)->right;
            free(tmp);
        } else {
            free(*cur);
            *cur = NULL;
        }
    }
}

// toglie e restituisce il primo elemento 
_Bool sset_extractMin(SortedSetADTptr ss, void**ptr) {
    if(ss == NULL || ss->size == 0){
        return 0;
    }

    TreeNodePtr cur = ss->root;
    while (cur->left !=  NULL){
        cur = cur->left;
    }

    *ptr = cur->elem;
    sset_remove(ss,*ptr);
    return 1;  
}

void sset_extractMax_rec(TreeNodePtr* cur, void**ptr, int (*compare)(void*, void*)) {
    if ((*cur)->right) sset_extractMax_rec(&((*cur)->right), ptr, compare);
    else {
        (*ptr) = (*cur)->elem;
        if ((*cur)->left) {
            TreeNodePtr tmp = *cur;
            (*cur) = (*cur)->left;
            free(tmp);
        } else {
            free(*cur);
            *cur = NULL;
        }
    }
}

// toglie e restituisce l'ultimo elemento (0 se lista vuota, -1 se errore, 1 se restituisce elemento)
_Bool sset_extractMax(SortedSetADTptr ss, void**ptr) {
    if(ss == NULL || ss->size == 0){
        return 0;
    }

    TreeNodePtr cur = ss->root;
    while (cur->right !=  NULL){
        cur = cur->right;
    }

    *ptr = cur->elem;
    sset_remove(ss,*ptr);
    return 1;      
}

void countNodes(const TreeNode* node, int* count) {
    if (node == NULL) return;

    (*count)++;
    countNodes(node->left, count);
    countNodes(node->right, count);
}

void preorder(const TreeNode* node, void** array, int* index) {
    if (node == NULL) return;

    array[*index] = node->elem;  
    (*index)++;                  

    preorder(node->left, array, index);  
    preorder(node->right, array, index); 
}


void** sset_toArray(const SortedSetADT* ss) {
    if (ss == NULL || ss->root == NULL) {
        return NULL;
    }


    int count = 0;
    countNodes(ss->root, &count); 

    void** array = (void**)malloc(count * sizeof(void*));
    if (array == NULL) {
        return NULL; 
    }

  
    int index = 0;
    preorder(ss->root, array, &index);

    return array;
}

