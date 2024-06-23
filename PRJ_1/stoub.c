// Seguendo l'approccio Test Driven Development, dopo aver realizzato i test del laboratorio 1a, potete passare a implementare la vostra versione del modulo "stoub.c": 

// /**
//  * @brief Converte una stringa in intero (data la base).
//  *
//  * Calcola il numero intero rappresentato dalla stringa in argomento nella base
//  * (la base deve essere un numero intero nel range 2..36) in argomento:
//  * − saltando gli eventuali spazi bianchi iniziali,
//  * − riconoscendo l'eventuale (singolo) carattere del segno,
//  * − fermandosi al raggiungimento del primo carattere non cifra nella base data (le cifre per la base 36 sono: 0,..,9,A,..,Z).
//  *
//  * @param s: Una stringa da convertire.
//  * @param b: La base.
//  * @param r: L'indirizzo dell'intero dove memorizzare il risultato.
//  * @return Restituisce 0 se 'b' non è nell’intervallo ammesso, oppure se non trova nessuna cifra valida
//  *            (e in tal caso il valore all'indirizzo r non è significativo).
//  *            Altrimenti restituisce 1, e scrive all'indirizzo r il numero intero rappresentato nella stringa nella base data.
//  *            Non si accorge dell'eventuale overflow (restituendo in questo caso un risultato scorretto).
//  */
// int stoub(char *s, unsigned short b, int*r);

// L'implementazione proposta verrà sottoposta ai nostri test in modo simile al LAB1A, ma con ruoli invertiti. 

#include <ctype.h>
#include <stdlib.h>

/**
 * @brief Converte una stringa in intero (data la base).
 *
 * Calcola il numero intero rappresentato dalla stringa in argomento nella base
 * (la base deve essere un numero intero nel range 2..36) in argomento:
 * − saltando gli eventuali spazi bianchi iniziali,
 * − riconoscendo l'eventuale (singolo) carattere del segno,
 * − fermandosi al raggiungimento del primo carattere non cifra nella base data (le cifre per la base 36 sono: 0,..,9,A,..,Z).
 *
 * @param s: Una stringa da convertire.
 * @param b: La base.
 * @param r: L'indirizzo dell'intero dove memorizzare il risultato.
 * @return   Restituisce 0 se 'b' non è nell’intervallo ammesso, oppure se non trova nessuna cifra valida
 *           (e in tal caso il valore all'indirizzo r non è significativo).
 *           Altrimenti restituisce 1, e scrive all'indirizzo r il numero intero rappresentato nella stringa nella base data.
 *           Non si accorge dell'eventuale overflow (restituendo in questo caso un risultato scorretto).
 */

int stoub(char *s, unsigned short b, int *r) {
    if (b < 2 || b > 36) {
        return 0;
    }

    // Saltare spazi bianchi iniziali
    while (isspace(*s)) {
        s++;
    }

    // Gestione del segno
    int segno = 1;
    if (*s == '-') {
        segno = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    int risultato = 0;
    int found_digit = 0;

    while (*s != '\0') {
        int cifra;
        if (isdigit(*s)) {
            cifra = *s - '0';
        } else if (isalpha(*s)) {
            cifra = toupper(*s) - 'A' + 10;
        } else {
            break;
        }

        if (cifra >= b) {
            break;
        }

        risultato = risultato * b + cifra;
        found_digit = 1;
        s++;
    }

    if (!found_digit) {
        return 0;
    }

    *r = segno * risultato;
    return 1;
}