#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definizione Strutture Dati
typedef struct {
    int b;
    int exp;
} Monomio;

typedef struct {
    Monomio* Monomi;
    int nM;
} Polinomio;

// Prototipi Funzioni ausiliarie
Polinomio* readFromFile(int* );
Monomio* readLine(char*, int*);
int compareM(const void* a, const void* b);
void printP(Polinomio p);
Polinomio sum(Polinomio p1, Polinomio p2); 

// Main
int main(){
    int n = 0;
    Polinomio* P = readFromFile(&n);

    srand(time(NULL));
    int rP1 = rand() % n;
    int rP2 = rand() % n;

    printf("Ho letto %d polinomi, calcolo la somma dei polinomi #%d e #%d\n", n, rP1, rP2);
    printf("P1: ");
    printP(P[rP1]);
    printf("P2: ");
    printP(P[rP2]);

    Polinomio S = sum(P[rP1], P[rP2]);
    printf("Somma: ");
    printP(S);

    free(S.Monomi);
    for(int i=0; i<n; i++) free(P[i].Monomi);
    free(P);

    return 0;
}

// Funzione di lettura del file
Polinomio* readFromFile(int* nP){
    FILE* fp = fopen("polinomi.txt", "r");
    if(!fp){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    Polinomio* P = NULL;               
    char line[1024];   
    *nP = 0;        

    while (fgets(line, sizeof(line), fp) != NULL) {
        (*nP)++;
        Polinomio* temp = realloc(P, *nP * sizeof(Polinomio));
        P = temp;
        P[*nP-1].Monomi = readLine(line, &P[*nP-1].nM);
    }
    
    fclose(fp);
    return P;
}

// Funzione di appoggio qsort
int compareM(const void* a, const void* b) {
    Monomio* m1 = (Monomio*)a;
    Monomio* m2 = (Monomio*)b;

    return m2->exp - m1->exp; 
}

// Funzione readline
Monomio* readLine(char* line, int* nM){
    Monomio* M = NULL;
    *nM = 0;

    char* token = strtok(line, " \n\r"); 
    while (token != NULL) {
        int tmpB = atoi(token);

        token = strtok(NULL, " \n\r"); 
        int tmpExp = atoi(token);

        Monomio* temp = realloc(M, (*nM + 1) * sizeof(Monomio));
        
        M = temp;
        M[*nM].b = tmpB;
        M[*nM].exp = tmpExp;
        (*nM)++;

        token = strtok(NULL, " \n\r");
    }

    if (*nM > 1) {
        qsort(M, *nM, sizeof(Monomio), compareM);
    }
    return M;
}

// Funzione di stampa
void printP(Polinomio p) {
    if (p.nM == 0) {
        printf("0\n");
        return;
    }

    int ft = 1;

    for(int i = 0; i < p.nM; i++) {
        int coeff = p.Monomi[i].b;
        int exp = p.Monomi[i].exp;

        if(coeff == 0){
            continue;
        }

        int abs = (coeff < 0) ? -coeff : coeff;
        if(!ft){
            if (coeff >= 0) {
            printf(" + ");
            } else {
                printf(" - ");
            }
        } else{
            ft = 0;
        }
        

        if(exp == 0){
            printf("%d", abs);
        } else if(exp == 1) {
            if(abs == 1) {
                printf("x"); 
            }else {
                printf("%dx", abs);
            }
        } else {
            if(abs == 1) {
                printf("x^%d", exp); 
            } else {
                printf("%dx^%d", abs, exp);
            }
        }
    }
    printf("\n");
}

// Funzione di Somma
Polinomio sum(Polinomio p1, Polinomio p2) {
    Polinomio res;
    res.Monomi = NULL;
    res.nM = 0;

    int i = 0;
    int j = 0;

    while (i < p1.nM && j < p2.nM) {
        res.Monomi = realloc(res.Monomi, (res.nM + 1) * sizeof(Monomio));
        
        if (p1.Monomi[i].exp > p2.Monomi[j].exp) {
            res.Monomi[res.nM] = p1.Monomi[i];
            i++;
            res.nM++;
        } else if (p2.Monomi[j].exp > p1.Monomi[i].exp) {
            res.Monomi[res.nM] = p2.Monomi[j];
            j++;
            res.nM++;
        } else {
            int sommaCoeff = p1.Monomi[i].b + p2.Monomi[j].b;
            if (sommaCoeff != 0) {
                res.Monomi[res.nM].b = sommaCoeff;
                res.Monomi[res.nM].exp = p1.Monomi[i].exp;
                res.nM++;
            }
            i++;
            j++;
        }
    }

    while (i < p1.nM) {
        res.Monomi = realloc(res.Monomi, (res.nM + 1) * sizeof(Monomio));
        res.Monomi[res.nM] = p1.Monomi[i];
        i++;
        res.nM++;
    }
    while (j < p2.nM) {
        res.Monomi = realloc(res.Monomi, (res.nM + 1) * sizeof(Monomio));
        res.Monomi[res.nM] = p2.Monomi[j];
        j++;
        res.nM++;
    }
    return res;
}

