// Supponete che qualcun altro abbia implementato un modulo "stoub.c" con questa specifica: 

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
// Il vostro scopo è quello di trovare possibili bachi nell'implementazione proposta (di cui non vedete il codice)
// tramite una serie di test. Per questo, supponete che ogni test sia descritto da una struttura di questo tipo:

// struct Test {
//    char *s;
//    unsigned short b;
//    int res;
//    int num;
// };

// dove 's' è un puntatore alla stringa da fornire in argomento a stoub, e 'b' è la base richiesta.
// Il risultato atteso è dato dalla coppia di interi res, num: res è il risultato restituito dalla chiamata, e num è il numero convertito.
// Quindi dato un test t descritto dalla struttura qui sopra, l'esecuzione del test corrisponde al blocco di codice (i e j sono due interi):

// i = stoub(t.s, t.b, &j);
// if (i != t.res || (i == 1 && j != t.num)) printf("TEST FAILED! expecting (%d,%d), got (%d,%d)\n",t.res,t.num,i,j);
// else printf("TEST PASSED!\n");

// Supponete ora di avere a disposizione spazio per codificare fino a 1000 test, da specificare nel vettore (già allocato): 

// struct Test tests[1000];

// Scrivere una serie di test il più possibile completa per mettere alla prova varie versioni del modulo stoub.c.
// "Dietro le quinte" ci sono 12 versioni del modulo stoub.c, con bachi via via più subdoli.

// Ad esempio, il seguente frammento definisce un test di esempio in posizione 0:

// tests[0].s = "25";
// tests[0].b = 10;
// tests[0].res = 1;
// tests[0].num = 25;

// Scrivere una serie di test simili a quello qui sopra, per verificare che le 12 implementazioni dietro le quinte
// sono in effetti bacate. La soluzione è corretta se tutti i "nostri test della vostra suite di test" diventano verdi,
// dove il nostro n-esimo test verifica se avete trovato che l'implementazione n-esima è bacata (n=1..12).
// Nota 1: superare tutti e 12 i nostri test è difficile. Ai fini dell'ammissione all'esame, il progetto 1a è da considerarsi svolto con successo se almeno 8 test risultano verdi. Non accanirsi a cercare di ottenere 12 test verdi.
// Nota 2: viene eseguito all'inizio anche un "sanity check" (check(0)) usando una implementazione (sperabilmente) non bacata.
// Opzionale: ipotizzare quale (o quali) bachi sono presenti in ciascuna versione.

/* scrivere qui la vostra suite di test */

/* ad esempio: */
tests[0].s = "0";
tests[0].b = 10;
tests[0].res = 1;
tests[0].num = 0;

// 1
tests[1].s = "";
tests[1].b = 0;
tests[1].res = 0;
tests[1].num = 0;

// 2
tests[2].s = "111";
tests[2].b = 40;
tests[2].res = 0;
tests[2].num = 0;

// 3
tests[3].s = " A";
tests[3].b = 16;
tests[3].res = 1;
tests[3].num = 10;

// 4
tests[4].s = "-1";
tests[4].b = 10;
tests[4].res = 1;
tests[4].num = -1;

// 5
tests[5].s = "+1";
tests[5].b = 10;
tests[5].res = 1;
tests[5].num = 1;

// 7
tests[8].s = "\n11";
tests[8].b = 2;
tests[8].res = 1;
tests[8].num = 3;

// 8
tests[9].s = "a";
tests[9].b = 10;
tests[9].res = 0;
tests[9].num = 0;

// 9,10
tests[10].s = "Z";
tests[10].b = 36;
tests[10].res = 1;
tests[10].num = 35;

// 11
tests[11].s = "10";
tests[11].b = 16;
tests[11].res = 1;
tests[11].num = 16;

// 12
tests[12].s = "01102";
tests[12].b = 2;
tests[12].res = 1;
tests[12].num = 6;
