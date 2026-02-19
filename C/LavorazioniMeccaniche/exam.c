#include <stdio.h>
#include <stdlib.h>

// Prototipi Funzioni Ausiliarie
int parseCircle(int, int);
void parseRect(int, int, char, int, int*, int*);

int main(){
    // Cerco il path del file in input
    char filePath[1000];
    printf("Insert file\n");
    scanf("%s", filePath);

    // Provo ad aprire il file
    FILE* fp = fopen(filePath, "r");
    if(!fp){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Dichiaro variabili
    int x, y, z;
    char c, k;

    // Scorro il file finche' trovo 3 elementi continuo a leggere il file
    while(fscanf(fp, "%d%d %c", &x, &y, &c) == 3){
        // Switch per gestire i due casi possibili, Cerchio e Rettangolo, avrei potuto usare un if-else
        switch(c){
            case 'C': {
                int r = parseCircle(x, y);
                printf("Dimensioni Lastra: %d x %d, massimo raggio cerchio: %d\n", x, y, r);
                break;
            }
            case 'R': {
                fscanf(fp, " %c%d", &k, &z);

                int a = -1;
                int b = -1;

                parseRect(x, y, k, z, &a, &b);
                if(a == -1 && b == -1){
                  printf("Dimensioni Lastra: %d x %d, dimensioni rettangolo non determinabili con i dati in ingresso\n", x, y);  
                } else{
                    if(k == 'A'){ 
                        printf("Dimensioni Lastra: %d x %d, dimensioni rettangolo con Area massima: %d x %d\n", x, y, a, b);
                    } else if(k == 'P'){
                        printf("Dimensioni Lastra: %d x %d, dimensioni rettangolo con Perimetro massimo: %d x %d\n", x, y, a, b);
                    }
                }
                break;
            }
            default: {
                fclose(fp);
                perror("SHAPE NOT FOUND");
                exit(EXIT_FAILURE);  
            }
        }
    }
    fclose(fp);
    return 0;
}

/* 
   Funzione Ausiliaria del calcolo del raggio massimo per lastra in input, 
   calcolo il lato minore ne sottraggo la tolleranza richiesta di 2mm e 
   divido per 2
*/ 
int parseCircle(int a, int b){
    int min = (a < b)? a : b;
    return (min - 2)/2;
}

/* 
   Funzione Ausiliaria del calcolo dei lati massimi per lastra in input, 
   con le condizioni richieste. Verifico le due condizioni partendo dalla dimensione 
   massima possibile e tornando indietro a ritroso.
*/ 
void parseRect(int a, int b, char c, int d, int* e, int* f){
    int max = 0;
    int temp;

    for(int i = (a - 2); i >= 1; --i){
        for(int j = (b - 2); j >= 1; --j){
            // Prima condizione: Se la somma fra Area e due lati = k 
            if(i*j + i+j != d){ 
                continue;
            }
            // Seconda condizione: In base al processo di lavorazione richiesto
            if(c == 'A'){ 
                temp = i*j;
            } else if(c == 'P'){
                temp = i+j;
            } else{
                perror("OPERATION NOT FOUND");
                exit(EXIT_FAILURE);  
            }
             // Se ho trovato una nuova dimensione massima la aggiorno
            if(temp > max){
                max = temp;
                *e = i;
                *f = j;
            }
        }
    }
}