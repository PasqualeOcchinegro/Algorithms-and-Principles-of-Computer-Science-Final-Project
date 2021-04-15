#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define L 100                                               //lunghezza delle stringhe

typedef struct tree                                         //struttura che conterrà tutte le entità
{
    struct log *root;                                       //puntatore alla radice della struttura
} Tree;

typedef struct log                                          //nodo dell'albero
{
    char name[L];                                           //nome dell'entità
    struct log *p;                                          //padre del nodo
    struct log *left;                                       //figlio sinistro del nodo
    struct log *right;                                      //figlio destro del nodo
    struct branch *b_head;                                  //puntatore al primo tipo di relazione presente nel rack
} Log;

typedef struct branch                                       //nodo della lista di adiacenza, indica il tipo di relazione
{
    char relation_name [L];                                 //nome del tipo di relazione
    struct branch *b_next;                                  //puntatore al prossimo tipo di relazione
    int popularity;                                         //counter di quante persone hanno questa entità come destinatario della relazione
    struct fruit *f_head;                                   //puntatore al primo elemento della lista di persone che hanno questa entità come destinatario della relazione
} Branch;

typedef struct fruit                                        //nodo che indica chi ha stretto una specifica relazione con questa entità
{
    Log *relation_giver;                                    //puntatore alla persona che ha questa entità come destinatario della relazione
    struct fruit *f_next;                                   //puntatore al prossimo nodo di questa lista
} Fruit;

typedef struct woodshed                                     //struttura che salva in ordine alfabetico i tipi di relazione il max corrente per ogni tipo
{
    int max;                                                //counter dell'attuale massimo raggiunto dal tipo di relazione
    char relation_name [L];                                 //nome del tipo di relazione
    struct woodshed *next;                                  //puntatore al prossimo nodo della struttura
} Woodshed;

Tree *structure;                                            //creazione della struttura
Woodshed *rack;                                             //creazione dell'elenco dei tipi di relazione

Woodshed *create_node_woodshed (char *relation_name)        //crea un nuovo nodo woodshed
{
    Woodshed *node = malloc(sizeof(Woodshed));              //alloca memoria per la creazione del nodo
    node->next = NULL;                                      //imposta a NULL il prossimo woodshed
    strcpy (node->relation_name, relation_name);            //inserisce il nome della relazione nel nodo
    node->max = 0;                                          //imposta a 0 il counter del massimo
    return node;
}

Fruit *create_node_fruit (Log *relation_giver)              //crea un nuovo nodo fruit
{
    Fruit *node = malloc(sizeof(Fruit));                    //alloca memoria per la creazione del nodo
    node->relation_giver = relation_giver;                  //imposta la persona da cui parte la relazione
    node->f_next = NULL;                                    //imposta a NULL il prossimo nodo
    return node;
}

Branch *create_node_branch(char *relation_name)            //crea un nuovo nodo branch
{
    Branch *node = malloc(sizeof(Branch));                  //alloca memoria per la creazione del nodo
    strcpy(node->relation_name, relation_name);             //inserisce il nome della relazione nel nodo
    node->f_head = NULL;                                    //imposta a NULL il prossimo fruit
    node->b_next = NULL;                                    //imposta a NULL il prossimo branch
    node->popularity = 0;                                   //inizializza la popolarità a 0
    return node;
}

Log *create_node_log(char *name)                            //crea un nuovo nodo log
{
	Log *node = malloc(sizeof(Log));                        //alloca memoria per la creazione del nodo
	strcpy(node->name, name);                               //inserisce il nome nel nodo
	node->p = NULL;                                         //imposta a NULL il padre
	node->left = NULL;                                      //imposta a NULL il figlio sinistro
	node->right = NULL;                                     //imposta a NULL il figlio destro
    node->b_head = NULL;                                    //imposta a NULL il prossimo nodo branch
	return node;
}

int find_new_max (Log *node, int max, int i)                //sostituisce il massimo nel caso delrel modifichi il suo valore
{
    if (node != NULL)                                       //controlla quando finisce l'albero
    {
        Branch *cursor;                                     //puntatore al branch dell'attuale node
        int j;                                              //j salverà il valore di i e verrà utilizzato per contare i passaggi senza alterare la chiamata ricorsiva di i
        int dx;                                             //int che salva il risultato della ricorsione di destra
        int sx;                                             //int che salva il risultato della ricorsione di sinistra
        int current;                                        //salva il massimo trovato
        cursor = node->b_head;                              //posizioniamoci all'inizio del branch
        j = i;                                              //inizializziamo j al valore di i
        while (j != 0)                                      //scorriamo il branch fino a trovare il tipo di relazione presa in considerazione
        {
            cursor = cursor->b_next;                        //cursor passa al successivo fino a trovare il tipo desiderato
            j--;                                            //diminuiamo il counter degli spostamenti
        }
        sx = find_new_max (node->left, max, i);             //ripete l'operazione ricorsivamente, partendo dall'elemento più a sinistra
        dx = find_new_max (node->right, max, i);            //ripete l'operazione ricorsivamente, arrivando all'elemento pià a destra
        current = max-1;                                    //poniamo il current a max-1, se troviamo il max all'interno dell'albero lo cambiamo
        if (cursor->popularity==max)                        //se troviamo max significa che possiamo non cambiare il massimo nel woodshed, quindi restituiremo max
        {
            current = max;                                  //current diventa max per restituirlo al delrel
        }
        if (dx == max || sx == max || current == max)       //se almeno una volta current è diventato max continueremo a fare il suo return
        {
            return max;
        }
        else                                                //se una volta passati per tutto l'albero non abbiamo mai sostituito current il max non esisterà più
        {
            return max-1;
        }
    }
}

int find_new_max_deluxe (Log *node, int i, Log *name)       //sostituisce il massimo nel caso delent modifichi il suo valore
{
    if (node != NULL)                                       //controlla quando finisce l'albero
    {
        Branch *node_b;                                     //puntatore al branch dell'attuale root
        int dx;                                             //int che salva il risultato della ricorsione di destra
        int sx;                                             //int che salva il risultato della ricorsione di sinistra
        int j;                                              //j salverà il valore di i e verrà utilizzato per contare i passaggi senza alterare la chiamata ricorsiva di i
        j = i;                                              //inizializziamo j al valore di i
        node_b = node->b_head;                              //posizioniamoci all'inizio del branch
        sx = find_new_max_deluxe (node->left, i, name);     //ripete l'operazione ricorsivamente, partendo dall'elemento più a sinistra
        dx = find_new_max_deluxe (node->right, i, name);    //ripete l'operazione ricorsivamente, arrivando all'elemento pià a destra
        if (node == name)                                   //se il nodo analizzato è uguale al nodo da eliminare non serve controllarne il massimo
        {
            if (dx > sx)                                    //returna il valore più alto tra dx e sx
            {
                return dx;
            }
            else
            {
                return sx;
            }
        }
        while (j != 0)                                      //scorriamo il branch fino a raggiungere il tipo di relazione desiderato
        {
            node_b = node_b->b_next;                        //node_b passa all'elemento successivo
            j--;                                            //diminuiamo il counter degli spostamenti
        }
        if (node_b->popularity>dx && node_b->popularity>sx) //se la popularity del nodo attuale è maggiore delle popularity fino ad ora osservate returna il suo valore
        {
            return node_b->popularity;
        }
        else if (dx>sx)                                     //altrimenti returna il valore più alto tra dx e sx
        {
            return dx;
        }
        else
        {
            return sx;
        }
    }
    return 0;
}

int rack_insert (char *relation)                            //inserisce un nuovo tipo di relazione nella lista rack
{
    Woodshed *w_prev;                                       //puntatore al nodo precedente al posto dove inseriremo il nostro nuovo tipo di relazione
    Woodshed *new_w;                                        //puntatore al nuovo nodo
    Woodshed *cursor;                                       //puntatore al primo nodo della lista
    int i;                                                  //counter degli spostamenti
    cursor = rack;                                          //poniamoci all'inizio della lista
    i = 0;                                                  //il counter degli spostamenti per ora è 0
    w_prev = NULL;                                          //inizializziamo il nodo w_prev a NULL
    new_w = create_node_woodshed (relation);                //allochiamo memoria per la creazione del nuovo nodo
    if (cursor == NULL)                                     //se node è NULL non è ancora presente nessun nodo nella lista, quindi inseriremo il nuovo nodo in testa
    {
        rack = new_w;                                       //il nodo appena creato sarà il primo della lista
        return i;                                           //ritorneremo 0 in quanto non ci siamo spostati
    }
    while (cursor != NULL && strcmp (cursor->relation_name, relation) < 0)  //se il nodo viene alfabeticamente dopo e ci sono ancora nodi da controllare scorriamo la lista
    {
        w_prev = cursor;                                    //w_prev diventa il nodo controllato in questo momento
        cursor = cursor->next;                              //controlliamo il prossimo elemento della lista
        i++;                                                //aumentiamo il counter degli spostamenti
    }
    if (w_prev == NULL)                                     //se dopo il while w_prev è ancora NULL il nodo creato dovrà essere inserito in testa
    {
        new_w->next = rack;                                 //il nuovo nodo punterà al precedente primo nodo della lista
        rack = new_w;                                       //il nodo creato sarà il nuovo primo elemento
        return i;                                           //anche in questo caso i sarà 0 in quanto stiamo inserendo in testa
    }
    w_prev->next = new_w;                                   //w_prev è il nodo precedente a quello da inserire, quindi il suo next dovrà puntare al nodo appena creato
    new_w->next = cursor;                                   //new_w è il nodo che stiamo inserendo, e dovrà puntare al nodo precedentemente puntato da w_prev
    return i;                                               //ritorneremo il numero di spostamenti effettuati all'interno del rack
}

void relation_insert (Log *node, char *relation, int i)     //scorre l'albero e aggiunge una relazione
{
    if (node != NULL)                                       //controlla quando finisce l'albero
    {
        relation_insert (node->left, relation, i);          //ripete l'operazione ricorsivamente, partendo dall'elemento più a sinistra
        Branch *b_prev;                                     //puntatore al nodo precedente al posto dove inseriremo il nostro nuovo tipo di relazione
        Branch *new_b;                                      //puntatore al nuovo nodo
        Branch *cursor;                                     //puntatore al primo nodo del branch, lo useremo per scorrere
        int j;                                              //j salverà il valore di i e verrà utilizzato per contare i passaggi senza alterare la chiamata ricorsiva di i
        cursor = node->b_head;                              //poniamoci all'inizio del branch
        b_prev = NULL;                                      //inizializziamo il puntatore b_prev a NULL
        j = i;                                              //inizializziamo j al valore di i
        new_b = create_node_branch (relation);              //allochiamo memoria per la creazione del nodo
        if (j == 0)                                         //se j = 0 non dobbiamo scorrere la lista del branch, il nodo dovrà essere aggiunto in testa
        {
            new_b->b_next = node->b_head;                   //il nodo appena creato punterà al precedente primo nodo della lista
            node->b_head = new_b;                           //il log associato punterà al nodo creato, ora nuova testa della lista
        }
        else
        {
            while (j != 0)                                  //se j != 0 dobbiamo scorrere la lista del branch di "i" posizioni
            {
                b_prev = cursor;                            //b_prev diventa il nodo controllato in questo momento
                cursor = cursor->b_next;                    //controlliamo il prossimo elemento della lista
                j--;                                        //diminuiamo il counter del numero di spostamenti
            }
            b_prev->b_next = new_b;                         //b_prev è il nodo precedente a quello da inserire, quindi il suo next dovrà puntare al nodo appena creato
            new_b->b_next = cursor;                         //new_b è il nodo che stiamo inserendo, e dovrà puntare al nodo precedentemente puntato da b_prev
        }
        relation_insert (node->right, relation, i);         //ripete l'operazione ricorsivamente, arrivando all'elemento più a destra
    }
    return;
}

void popularity_contest (Log *node, char *relation, int i)  //stampa i nomi più popolari di un determinato tipo di relazione
{
    if (node != NULL)                                       //controlla quando finisce l'albero
    {
        Branch *current_relation;                           //puntatore che scorrerà il branch fino a trovare il tipo di relazione di cui vogliamo l'output
        Woodshed *cursor;                                   //puntatore che scorrerà la rack fino a trovare il tipo di relazione di cui vogliamo l'output
        int j;                                              //j salverà il valore di i e verrà utilizzato per contare i passaggi senza alterare la chiamata ricorsiva di i
        current_relation = node->b_head;                    //posizioniamoci all'inizio del branch
        cursor = rack;                                      //posizioniamoci all'inizio della rack
        j = i;                                              //inizializziamo j al valore di i
        popularity_contest (node->left, relation, i);       //ripete l'operazione ricorsivamente, partendo dall'elemento più a sinistra
        while (j != 0)                                      //scorriamo il branch finchè non troviamo la relazione presa in considerazione
        {
            current_relation = current_relation->b_next;    //current_relation scorrerà il branch passando all'elemento successivo
            j--;                                            //diminuiamo il counter degli spostamenti
        }
        j = i;                                              //salviamo nuovamente il valore di i per riutilizzarlo nel rack
        while (j != 0)                                      //scorriamo la rack finchè non troviamo la relazione presa in considerazione
        {
            cursor = cursor->next;                          //cursor scorrerà la rack passando all'elemento successivo
            j--;                                            //diminuiamo il counter degli spostamenti
        }
        if (current_relation->popularity == cursor->max)    //confrontiamo la popolarità del log associato al current_relation e il massimo della relazione di cursor
        {
            fputc (34, stdout);                             //stampa le virgolette
            fputs (node->name, stdout);                     //stampiamo il nome del log nel caso si verificasse l'if
            fputc (34, stdout);                             //stampa le virgolette
            fputc (32, stdout);                             //stampa uno spazio
        }
        popularity_contest (node->right, relation, i);      //ripete l'operazione ricorsivamente, arrivando all'elemento più a destra
    }
    return;
}

void harvest_fruit (Log *root, Log *node)                   //elimina tutte le relazioni uscenti da un nodo
{
    if (root != NULL)                                       //scorriamo ricorsivamente l'intero albero
    {
        Branch *node_b;                                     //puntatore al branch del nodo attuale
        Fruit *node_f;                                      //puntatore ai vari fruit del branch
        Fruit *node_f_prev;                                 //puntatore al nodo del fruit precedente a node_f
        Woodshed *cursor;                                   //puntatore alla rack
        int i;                                              //counter degli spostamenti
        int j;                                              //j salverà il valore di i e verrà utilizzato per contare i passaggi senza alterare la chiamata ricorsiva di i
        node_b = root->b_head;                              //posizioniamoci all'inizio del branch
        i = 0;                                              //inizializziamo i a 0
        harvest_fruit (root->left, node);                   //ripetiamo l'operazione ricorsivamente, partendo dall'elemento più a sinistra
        while (node_b != NULL)                              //scorriamo l'intero branch del nodo attuale
        {
            node_f = node_b->f_head;                        //posizioniamoci all'inizio del fruit
            if (node_f != NULL)                             //controlliamo l'esistenza di fruit
            {
                if (node_f->relation_giver == node)         //se il primo fruit è una relazione con il nodo da eliminare va tolta
                {
                    node_b->f_head = node_f->f_next;        //il branch punterà al secondo nodo dei fruit
                    free (node_f);                          //liberiamo il fruit
                    node_b->popularity--;                   //diminuiamo la popularity del corrispettivo branch
                    cursor = rack;                          //posizioniamoci all'inizio del rack
                    j = i;                                  //inizializziamo j al valore di i
                    while (j != 0)                          //scorriamo il rack fino a trovare il tipo di relazione del branch di cui abbiamo liberato il fruit
                    {
                        cursor = cursor->next;              //cursor passa all'elemento successivo
                        j--;                                //diminuiamo il counter degli spostamenti
                    }
                    if (cursor->max == node_b->popularity+1)    //nel caso la popularity del branch fosse stata il massimo del tipo di relazione dobbiamo trovare il nuovo massimo
                    {
                        cursor->max = find_new_max (structure->root, cursor->max, i);   //troviamo il nuovo massimo e sostituiamolo nel cursor max
                    }
                }
                else                                        //entriamo in questo else se il primo elemento del fruit non corrisponde ad una relazione con il nodo da eliminare
                {
                    node_f_prev = node_f;                   //node_f_prev diventerà il nodo appena controllato
                    node_f = node_f->f_next;                //node_f passa al successivo
                    while (node_f != NULL)                  //scorriamo il while finchè esistono fruit
                    {
                        if (node_f->relation_giver == node) //se troviamo il fruit contenente una relazione al nodo da eliminare la ricerca termina, sicuramente non ce ne saranno altri
                        {
                            node_f_prev->f_next = node_f->f_next;   //node_f_prev punterà al nodo successivo di node_f
                            free (node_f);                  //eliminiamo node_f
                            node_f = node_f_prev->f_next;   //node_f torna ad essere il successivo di node_f_prev
                            node_b->popularity--;           //diminuiamo la popularity del corrispettivo branch
                            cursor = rack;                  //posizioniamoci all'inizio del rack
                            j = i;                          //inizializziamo j al valore di i
                            while (j != 0)                  //scorriamo il rack fino a trovare il tipo di relazione del branch di cui abbiamo liberato il fruit
                            {
                                cursor = cursor->next;      //cursor passa all'elemento successivo
                                j--;                        //diminuiamo il counter degli spostamenti
                            }
                            if (cursor->max == node_b->popularity+1)    //nel caso la popularity del branch fosse stata il massimo del tipo di relazione dobbiamo trovare il nuovo massimo
                            {
                                cursor->max = find_new_max (structure->root, cursor->max, i);   //troviamo il nuovo massimo e sostituiamolo nel cursor max
                            }
                        }
                        else                                //se il fruit attuale non ha al suo interno la relazione con il nodo da eliminare scorriamo i fruit
                        {
                            node_f_prev = node_f;           //node_f_prev diventa node_f
                            node_f = node_f->f_next;        //node_f passa al successivo
                        }
                    }
                }
            }
            node_b = node_b->b_next;                        //node_b passa al nodo successivo del branch
            i++;                                            //aumentiamo il counter degli spostamenti

        }
        harvest_fruit (root->right, node);                  //ripetiamo l'operazione ricorsivamente, arrivando all'elemento più a destra
    }
    return;
}

void prune_branch (Log *node)                               //elimina l'intero branch di un nodo
{
    Branch *node_b;                                         //puntatore al branch del nodo
    Fruit *node_f;                                          //puntatore ai vari frutti del branch
    Woodshed *cursor;                                       //puntatore al rack
    int i;                                                  //int per contare gli spostamenti
    int j;                                                  //salva il numero degli spostamenti e viene sfruttato per spostarsi nel rack e nel branch lo stesso numero di volte
    node_b = node->b_head;                                  //posizioniamoci all'inizio del branch
    i = 0;                                                  //inizializziamo i a 0
    while (node->b_head != NULL)                            //questo while continuerà finchè ci saranno nodi del branch
    {
        cursor = rack;                                      //posizioniamoci nuovamente all'inizio del rack
        if (node_b->f_head == NULL)                         //se l'attuale nodo del branch non ha frutti possiamo eliminarlo
        {
            node->b_head = node_b->b_next;                  //il nodo dell'albero punterà al secondo nodo del branch
            free (node_b);                                  //eliminiamo il primo nodo del branch
            i++;                                            //aumentiamo il counter degli spostamenti
            node_b = node->b_head;                          //node_b punterà al prossimo (ora primo) nodo del branch
        }
        else                                                //se entriamo nell'else l'attuale nodo del branch avrà dei frutti da eliminare
        {
            while (node_b->f_head != NULL)                  //quessto while continuerà finchè ci saranno fruit nel nodo del branch
            {
                node_f = node_b->f_head;                    //posizioniamoci all'inizio dei fruit
                node_b->f_head = node_f->f_next;            //il branch punterà al secondo fruit della lista
                free (node_f);                              //liberiamo il primo nodo
            }
            j = i;                                          //inizializziamo j al valore di i
            while (j != 0)                                  //scorriamo il rack un numero di volte pari al numero di nodi del branch passati
            {
                cursor = cursor->next;                      //cursor passa al successivo
                j--;                                        //diminuiamo il counter degli spostamenti
            }
            if (node_b->popularity == cursor->max)          //se l'entità da eliminare era un massimo dobbiamo trovarne uno nuovo
            {
                cursor->max = find_new_max_deluxe (structure->root, i, node); //troviamo un nuovo massimo per il tipo di relazione corrente
            }
            node->b_head = node_b->b_next;                  //eliminati i frutti il nodo punterà al prossimo branch
            free (node_b);                                  //eliminiamo il branch attuale
            i++;                                            //aumentiamo il counter degli spostamenti
            node_b = node->b_head;                          //andiamo al prossimo nodo
        }
    }
    return;
}

Log *tree_minimum (Log *node)                               //trova il nodo più piccolo partendo da quello presente
{
    while (node->left != NULL)                              //scorre l'albero finchè non ci sono nodi a sinistra di quello corrente
    {
        node = node->left;                                  //il nodo passa al suo figlio di sinistr, quindi minore
    }
    return node;
}

Log *tree_successor (Log *node)                             //trova il nodo successivo a quello presente
{
    Log *node_p;                                            //puntatore al padre del nodo corrente
    if (node->right != NULL)                                //se il nodo ha figli destri prenderemo il più piccolo nodo maggiore tramite la funzione tree minimum
    {
        return tree_minimum(node->right);
    }
    node_p = node->p;                                       //inizializziamo node_p al padre del nodo
    while (node_p != NULL && node == node_p->right)         //scorriamo l'albero al contrario fino ad incontrare un padre avente figlio destro
    {
        node = node_p;                                      //node diventa node_p
        node_p = node_p->p;                                 //node_p passa al padre di node
    }
    return node_p;
}

Log *tree_delete (Tree *structure, Log *node)               //elimina un nodo dalla structure
{
    Log *x;                                                 //puntatore provvisorio ai nodi dell'albero
    Log *y;                                                 //puntatore provvisorio ai nodi dell'albero
    if (node->left == NULL || node->right == NULL)          //se il nodo da eliminare non ha uno dei due figli entriamo in questo if
    {
        y = node;                                           //y diventa il nodo
    }
    else
    {
        y = tree_successor (node);                          //y diventa il successor del nodo
    }
    if (y->left != NULL)                                    //se y ha un figlio sinistro entriamo in questo if
    {
        x = y->left;                                        //x diventa il figlio sinistro di y
    }
    else
    {
        x = y->right;                                       //x diventa il figlio destro di y
    }
    if (x != NULL)                                          //se y ha almeno un figlio entriamo in questo if
    {
        x->p = y->p;                                        //il padre di y ddiventa il padre di x
    }
    if (y->p == NULL)                                       //se il padre di y non esiste y è la radice dell'albero
    {
        structure->root = x;                                //la nuova radice diventa x
    }
    else if (y == y->p->left)                               //se il padre di y esiste ed y è il suo figlio sinistro entriamo in questo if
    {
        y->p->left = x;                                     //il figlio sinistro del padre di y diventa x
    }
    else
    {
        y->p->right = x;                                    //il figlio destro del padre di y diventa x
    }
    if (y != node)                                          //se y non è il nodo da eliminare
    {
        y->p = node->p;                                     //il padre di node diventa il padre di y
        if (node->p == NULL)                                //se il nodo da eliminare era la radice entriamo in questo if
        {
            structure->root = y;                            //y diventa la nuova radice
        }
        else
        {
            if (node->p->left == node)                      //se node è il figlio sinistro di suo padre entriamo in questo if
            {
                node->p->left = y;                          //il figlio sinistro del padre di node diventa y
            }
            else
            {
                node->p->right = y;                         //il figlio destro del padre di node diventa y
            }
        }
        y->left = node->left;                               //il figlio sinistro di node diventa il figlio sinistro di y
        y->right = node->right;                             //il figlio destro di node diventa il figlio destro di y
        if (node->left != NULL)                             //se esiste il figlio sinistro di node entriamo in questo if
        {
            node->left->p = y;                              //y diventa il padre del figlio sinistro di node
        }
        if (node->right != NULL)                            //se esite il figlio destro di node entriamo in questo if
        {
            node->right->p = y;                             //y diventa il padre del figlio destro di node
        }
    }
    return node;                                            //restituiamo il nodo da eliminare
}

Log *tree_search (Log *node, char *name)                    //controlla se un nodo è già presente nell'albero
{
    int i;                                                  //int per salvare il risultato del strcmp
    if (node == NULL || strcmp(node->name, name) == 0)      //se l'albero è vuoto o il nome è già nella radice conclude qui la funzione
    {
        return node;                                        //esce dal programma restituendo node
    }
    i = (strcmp(node->name, name));                         //confronta il nome cercato con quello corrente per vedere dove proseguire la ricerca
    if (i<0)                                                //il nome viene alfabeticamente prima del nodo attuale
    {
        return tree_search(node->right, name);               //continua la ricerca proseguendo a sinistra nell'albero
    }
    else                                                    //il nome viene alfabeticamente dopo il nodo attuale
    {
        return tree_search(node->left, name);              //continua la ricerca proseguendo a destra nell'albero
    }
}

Branch *relation_search (Branch *b_node, char *relation)    //controlla se un tipo di relazione è già presente
{
    int i;                                                  //int per conservare il risultato del strcmp
    while (b_node != NULL)                                  //in questo while scorriamo la lista dei tipi di relazione finchè non troviamo quello cercato (se presente)
    {
        i = strcmp(b_node->relation_name, relation);        //confrontiamo il nome dei tipi di relazione
        if (i == 0)                                         //se coincidono abbiamo trovato il nostro tipo di relazione
        {
            return b_node;                                  //finita la funzione avremo il puntatore al nodo del branch in cui è presente il tipo di relazione
        }
        else if (i<0)                                       //se i<0 il nostro tipo di funzione viene alfabeticamente dopo quello attuale, continuiamo la ricerca
        {
            b_node = b_node->b_next;                        //prendiamo in considerazione il prossimo nodo della lista
        }
        else                                                //se i>0 il nostro tipo di funzione viene alfabeticamente prima di quello attuale, ergo non è presente
        {
            return NULL;
        }
    }
    return NULL;
}

void addent (char *name1)                                   //comando addent
{
    int i;                                                  //int per salvare il risultato del strcmp
    Log *prov;                                              //puntatore per controllare i figli dell'albero
    Log *node_p;                                            //puntatore provvisorio per mantenere il padre
    Log *node;                                              //puntatore al nodo da aggiungere
    Branch *node_b;                                         //puntatore ai nodi del branch da aggiungere
    Branch *node_b_prev;                                    //puntatore ai nodi del branch precedenti a quelli da aggiungere
    Woodshed *cursor;                                       //puntatore alla testa della lista di rack
    cursor = rack;                                          //posizioniamo cursor in cima alla lista
    node = create_node_log (name1);                         //allochiamo memoria per la creazione del nuovo nodo
    prov = structure->root;                                 //partiamo dal controllare l'albero in entrata
    node_p = NULL;                                          //se node_p sarà ancora NULL dopo il while l'albero è vuoto
    while (prov != NULL)                                    //controlliamo i nodi dell'albero
    {
        node_p = prov;                                      //salviamo la posizione di prov
        i = strcmp(node->name, prov->name);                 //controlliamo se il nome è presente nell'albero e se non è così controlliamo l'ordine alfabetico
        if (i == 0)                                         //il nome è presente nell'albero
        {
            return;                                         //non serve aggiungere nulla
        }
        else if (i<0)                                       //il nome viene alfabeticamente prima del nodo attuale
        {
            prov = prov->left;                              //proseguiamo la ricerca andando a sinistra
        }
        else                                                //il nome viene alfabeticamente dopo il nodo attuale
        {
            prov = prov->right;                             //proseguiamo la ricerca andando a sinistra
        }
    }    
    node->p = node_p;                                       //node_p diventa il padre del nodo che stiamo inserendo
    if (node_p == NULL)                                     //l'albero è vuoto
    {
        structure->root = node;                             //il nodo diventa la radice del nuovo albero
    }
    else
    {
        i = strcmp(node->name, node_p->name);               //controlliamo se il nome è alfabeticamente successivo o precedente a suo padre
        if (i<0)                                            //il nome viene alfabeticamente prima del padre
        {
            node_p->left = node;                            //inseriamo il nodo alla sua sinistra
        }
        else                                                //il nome viene alfabeticamente dopo il padre
        {
            node_p->right = node;                           //inseriamo il nodo alla sua destra
        }
    }
    if (cursor != NULL)                                     //controlliamo che il woodshed non sia vuoto, nel caso inseriamo il primo elemento nel branch
    {
        node_b = create_node_branch (cursor->relation_name);//allochiamo memoria per il primo nodo del branch
        node->b_head = node_b;                              //inseriamo il primo nodo del branch in testa
        cursor = cursor->next;                              //passiamo all'elemento successivo del woodshed
    }
    while (cursor != NULL)                                  //proseguiamo inserendo tutti i successivi nodi del woodshed nel branch
    {
        node_b_prev = node_b;                               //salviamo la posizione dell'attuale nodo del branch
        node_b = create_node_branch (cursor->relation_name);//allochiamo memoria per il prossimo nodo del branch
        node_b_prev->b_next = node_b;                       //il nuovo nodo verrà collegato al precedente
        cursor = cursor->next;                              //scorriamo il woodshed
    }
    return;
}

void delent (char *name)                                    //comando delent
{
    Log *node;                                              //puntatore al nodo contenente il nome da eliminare
    node = tree_search (structure->root, name);             //cerchiamo il nome nell'albero
    if (node == NULL)                                       //il nome non è presente
    {
        return;                                             //usciamo dalla delent
    }
    else                                                    //il nome è presente
    {
        harvest_fruit (structure->root, node);              //questa funzione elimina tutte le relazioni che partono dal nodo da eliminare
        if (node->b_head != NULL)                           //se il nodo ha un branch bisogna eliminare prima quest'ultimo del nodo vero e proprio
        {
            prune_branch (node);                            //questa funzione elimina tutti i nodi dell'eventuale branch
        }
        node = tree_delete (structure, node);               //isoliamo il nodo dalla structure
        free (node);                                        //eliminiamo il nodo
    }
    return;
}

void addrel (char *name1, char *name2, char *relation)      //comando addrel
{
    Log *node_name1;                                        //salva un puntatore temporaneo per la ricerca dei nomi
    Log *node_name2;                                        //salva un puntatore temporaneo per la ricerca dei nomi
    Branch *node_branch;                                    //salva un puntatore al primo branch del log
    Fruit *node_fruit;                                      //salva un puntatore al nodo del fruit
    Woodshed *cursor;                                       //salva un puntatore al primo nodo del woodshed
    int i;                                                  //counter degli spostamenti
    node_name1 = tree_search (structure->root, name1);      //controlla che nome1 sia presente nell'albero
    node_name2 = tree_search (structure->root, name2);      //controlla che nome2 sia presente nell'albero
    if (node_name1 == NULL || node_name2 == NULL)           //se uno dei due nomi è assente conclude il comando
    {
        return;
    }
    node_branch = relation_search (node_name2->b_head, relation);   //cerchiamo il nome della relazione nel branch del secondo nodo
    if (node_branch != NULL)                                //se la lista non è vuota dobbiamo controllare che la relazione tra le due entità non esista già
    {
        node_fruit = node_branch->f_head;                   //posizioniamoci all'inizio del fruit del tipo di relazione del secondo nome
        while (node_fruit != NULL)                          //scorriamo l'intera lista
        {
            if (strcmp (node_fruit->relation_giver->name, node_name1->name) == 0)   //se entriamo in questo if la relazione è già presente, quindi usciamo dall'addrel
            {
                return;
            }
            node_fruit = node_fruit->f_next;                //procediamo controllando il prossimo elemento della lista
        }
    }
    else                                                    //il branch era vuoto oppure il tipo di relazione non esiste, in questo caso dobbiamo aggiungerlo
    {
        i = rack_insert (relation);                         //inseriamo la relazione anche nel woodshed
        relation_insert (structure->root, relation, i);     //la funzione scorre l'intero albero e inserisce il nuovo tipo nel branch di ogni log
        node_branch = node_name2->b_head;                   //una volta inserito il nuovo tipo di relazione ci riposizioniamo in testa al branch del secondo nome   
        node_branch = relation_search (node_branch, relation);  //scorriamo nuovamente la lista, questa volta certi di trovare il tipo di relazione
    }
    node_branch->popularity++;                              //il counter del secondo nome verrà aumentato come conseguenza di essere destinatario della relazione
    cursor = rack;                                          //poniamoci all'inizio del woodshed
    while (strcmp (cursor->relation_name, relation) != 0)   //scorriamo il woodshed fino a trovare la nostra relazione
    {
        cursor = cursor->next;                              //se non è stata trovata nel nodo corrente procediamo al prossimo
    }
    if (node_branch->popularity > cursor->max)              //se il counter popularity di un determinato log ha superato il max per quel tipo di relazione lo sostituisce
    {
        cursor->max = node_branch->popularity;              //max diventa il counter di questo nodo
    }
    node_fruit = create_node_fruit (node_name1);            //creiamo un nodo per il fruit del secondo nome che conterrà un puntatore al nodo del primo nome
    node_fruit->f_next = node_branch->f_head;               //inseriamo in testa il nodo appena creato
    node_branch->f_head = node_fruit;                       //ora che la testa della precedente lista fruit è puntata dal nuovo nodo il branch dovrà puntare quest'ultimo
    return;
}

void delrel (char *name1, char *name2, char *relation)      //comando delrel
{
    int i;
    Log *node_name1;                                        //salva un puntatore temporaneo per la ricerca dei nomi
    Log *node_name2;                                        //salva un puntatore temporaneo per la ricerca dei nomi
    Branch *node_branch;                                    //salva un puntatore al primo branch del log
    Fruit *node_fruit;                                      //salva un puntatore al nodo della relazione da cancellare nel fruit
    Fruit *node_fruit_prev;                                 //salva un puntatore al nodo precedente alla relazione da cancellare nel fruit
    Woodshed *cursor;                                       //salva un puntatore al primo nodo del woodshed
    i = 0;                                                  //inizializziamo il counter a 0
    cursor = rack;                                          //posizioniamoci all'inizio del woodshed
    node_fruit_prev = NULL;                                 //inizializziamo x a NULL
    node_name1 = tree_search (structure->root, name1);      //controlla che nome1 sia presente nell'albero
    node_name2 = tree_search (structure->root, name2);      //controlla che nome2 sia presente nell'albero
    if (node_name1 == NULL || node_name2 == NULL)           //se uno dei due nomi è assente conclude il comando
    {
        return;
    }
    node_branch = relation_search (node_name2->b_head, relation);   //cerchiamo il nome della relazione nel branch del secondo nodo
    if (node_branch == NULL)                                //se la relazione non è presente non può esistere tra le due entità
    {
        return;
    }
    else                                                    //controlliamo che la relazione tra le entità esista
    {
        node_fruit = node_branch->f_head;                   //posizioniamoci all'inizio del fruit del tipo di relazione del secondo nome
        while (node_fruit != NULL && strcmp (node_fruit->relation_giver->name, node_name1->name) != 0)   //scorriamo l'intera lista
        {
            node_fruit_prev = node_fruit;                   //node_fruit_prev diventa il nodo appena controllato
            node_fruit = node_fruit->f_next;                //procediamo controllando il prossimo elemento della lista
        }
        if (node_fruit == NULL)                             //se il nodo è vuoto la relazione non è stata trovata
        {
            return;
        }
        else                                                //entriamo in questo else solo se effettivamente dobbiamo effettuare la delrel
        {
            if (node_fruit_prev == NULL)                    //se node_fruit_prev è uguale a NULL stiamo cancellando il primo elemento della lista
            {
                node_branch->f_head = node_fruit->f_next;   //colleghiamo il nodo del branch al secondo nodo della lista
            }
            else
            {
                node_fruit_prev->f_next = node_fruit->f_next;   //il nodo prima di quello da eliminare punterà a quello successivo
            }
            free (node_fruit);                              //eliminiamo il nodo
            node_branch->popularity--;                      //abbassiamo il counter di popolarità
        }
        cursor = rack;                                      //posizioniamoci all'inizio del woodshed
        while (strcmp(cursor->relation_name, node_branch->relation_name) != 0)  //scorriamo il woodshed fino a trovare il tipo di relazione desiderata
        {
            cursor = cursor->next;                          //cursor passa all'elemento successivo
            i++;                                            //aumentiamo il counter degli spostamenti
        }
        if (cursor->max == node_branch->popularity+1)       //se il massimo era precedentemente istituito da questo log dobbiamo trovarne il nuovo
        {
            cursor->max = find_new_max (structure->root, cursor->max, i); //la funzione confronta il massimo con la popolarità di tutti i nodi
        }
    }
    return;
}

void report ()                                              //comando report
{
    Woodshed *cursor;                                       //puntatore ai vari tipi di relazione presenti nella rack
    int i;                                                  //counter degli spostamenti
    cursor = rack;                                          //posizioniamoci all'inizio della rack
    i = 0;                                                  //inizializziamo il counter a 0
    if (structure->root == NULL)                            //se non esistono entità non ci sono relazioni
    {
        fputs ("none\n", stdout);                           //senza relazioni il report stampa none
        return;
    }
    if (cursor == NULL)                                     //se la lista delle relazioni nella rack è vuota non ci sono relazioni
    {
        fputs ("none\n", stdout);                           //senza relazioni il report stampa none
        return;
    }
    else
    {
        while (cursor != NULL)                              //scorriamo tutti i tipi di relazione presenti nel rack
        {
            if (cursor->max != 0)                           //se il massimo di un tipo di relazione è 0 non va stampato
            {
                fputc (34, stdout);                         //stampa le virgolette
                fputs (cursor->relation_name, stdout);      //stampa il tipo di relazione
                fputc (34, stdout);                         //stampa le virgolette
                fputc (32, stdout);                         //stampa le virgolette
                popularity_contest (structure->root, cursor->relation_name, i); //questa funzione ci permette di stampare i massimi della relazione corrente
                printf ("%d; ", cursor->max);               //stampiamo il massimo, trovato all'interno del nodo del woodshed corrispondente
            }
            cursor = cursor->next;                          //cursor passa al successivo fino a trovare la relazione
            i++;                                            //aumentiamo il counter degli spostamenti
        }
    }
    fputs ("\n", stdout);                                   //stampa un a capo
}

void find_function (char *string)                           //smista la prossia riga di input per eseguire il giusto comando
{
    char name1[L];                                          //stringa per salvare il nome di addent e delent, o il primo nome di addrel e delrel
    char name2[L];                                          //stringa per salvare il secondo nome di addrel e delrel
    char relation[L];                                       //stringa per salvare la relazione addrel e delrel
    int i;                                                  //int per spostarsi nella string
    int j;                                                  //int per spostarsi nella string
    i = 8;                                                  //impostare i ad 8 permette di posizionarsi all'inizio del primo nome di addent, delent, addrel e delrel
    j = 0;                                                  //iniziamo a leggere la string da 0

    if (strncmp (string, "addent", 6) == 0)                 //se la prima parola è addent il programma inizierà la corrispettiva funzione
    {
        while (string[i] != '\"')                           //legge il nome successivo all'addent
        {
            name1[j] = string[i];                           //salva in name1 il nome successivo all'addent
            i++;                                            //scorre la string
            j++;                                            //scorre name1
        }
        name1[j] = '\0';                                    //mette fine al nome
        addent (name1);                                     //attua il comando addent
        return;
    }
    else if (strncmp (string, "delent", 6) == 0)            //se la prima parola è delent il programma inizierà la corrispettiva funzione
    {
        while (string[i] != '\"')                           //legge il nome successivo all'addent
        {
            name1[j] = string[i];                           //salva in name1 il nome successivo all'addent
            i++;                                            //scorre la string
            j++;                                            //scorre name1
        }
        name1[j] = '\0';                                    //mette fine al nome
        delent (name1);                                     //attua il comando delent
        return;
    }
    else if (strncmp (string, "addrel", 6) == 0)            //se la prima parola è addrel il programma inizierà la corrispettiva funzione
    {
        while (string[i] != '\"')                           //legge il nome successivo all'addrel
        {
            name1[j] = string[i];                           //salva in name1 il primo nome
            i++;                                            //scorre string
            j++;                                            //scorre name1
        }
        name1[j] = '\0';                                    //mette fine al primo nome
        i=i+3;                                              //posiziona il puntatore di string all'inizio del secondo nome
        j=0;                                                //posiziona il puntatore di name2 all'inizio della stringa
        while (string[i] != '\"')                           //legge il nome successivo al primo
        {
            name2[j] = string[i];                           //salva in name2 il secondo nome
            i++;                                            //scorre string
            j++;                                            //scorre name2
        }
        name2[j] = '\0';                                    //mette fine al secondo nome
        i=i+3;                                              //posiziona il puntatore di string all'inizio della relazion
        j=0;                                                //posiziona il puntatore di relation all'inizio della stringa
        while (string[i] != '\"')                           //legge la relazione
        {
            relation[j] = string[i];                        //salva in relation il nome della relazione
            i++;                                            //scorre string
            j++;                                            //scorre relation
        }      
        relation[j] = '\0';                                 //mette fine alla relation     
        addrel (name1, name2, relation);                    //attua il comando addrel
        return;
    }
    else if (strncmp (string, "delrel", 6) == 0)            //se la prima parola è delrel il programma inizierà la corrispettiva funzione
    {
        while (string[i] != '\"')                           //legge il nome successivo all'addrel
        {
            name1[j] = string[i];                           //salva in name1 il primo nome
            i++;                                            //scorre string
            j++;                                            //scorre name1
        }
        name1[j] = '\0';                                    //mette fine al primo nome
        i=i+3;                                              //posiziona il puntatore di string all'inizio del secondo nome
        j=0;                                                //posiziona il puntatore di name2 all'inizio della stringa
        while (string[i] != '\"')                           //legge il nome successivo al primo
        {
            name2[j] = string[i];                           //salva in name2 il secondo nome
            i++;                                            //scorre string
            j++;                                            //scorre name2
        }
        name2[j] = '\0';                                    //mette fine al secondo nome
        i=i+3;                                              //posiziona il puntatore di string all'inizio della relazion
        j=0;                                                //posiziona il puntatore di relation all'inizio della stringa
        while (string[i] != '\"')                           //legge la relazione
        {
            relation[j] = string[i];                        //salva in relation il nome della relazione
            i++;                                            //scorre string
            j++;                                            //scorre relation
        }      
        relation[j] = '\0';                                 //mette fine alla relation     
        delrel (name1, name2, relation);                    //attua il comando delrel
        return;
    }
    else if (strncmp (string, "report", 6) == 0)            //se la prima parola è report il programma inizierà la corrispettiva funzione
    {
        report ();                                          //attua il comando report
        return;
    }
}

int main ()                                                 //inizio programma quello serio
{
    char string[4*L];                                       //stringa per salvare riga per riga il file d'ingresso

    structure = malloc(sizeof(Tree));                       //dedichiamo memoria all'albero
    structure->root = NULL;                                 //impostiamo la radice

    do                                                      //tutto il programma girerà all'interno di questo do while finchè non arriverà all'end
    {
        fgets(string, 4*L, stdin);                          //prende in ingresso la prossima riga del file
        find_function (string);                             //funzione che indirizza verso il prossimo comando e lo attua
    } while (strncmp (string, "end", 3 != 0));
    return 0;
}