#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
};

typedef struct Nod N;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(N* nod) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (nod) {
		afisareMasina(nod->info);
		nod = nod->next;
	}
}

void adaugaMasinaInLista(N** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	N* nodNou;
	nodNou = (N*)malloc(sizeof(N));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;

	if ((*lista) == NULL) {
		(*lista) = nodNou;
	}
	else {
		N* aux = (*lista);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nodNou;
	}
}

void adaugaLaInceputInLista(/*lista de masini*/ Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}


//EX 2
void adaugaMasinaInListaSortataDupaPret(N** lista, Masina masinaNoua) {
	N* nou = (N*)malloc(sizeof(N));
	nou->info = masinaNoua;
	nou->next = NULL;
	if (*lista == NULL || (*lista)->info.pret > masinaNoua.pret) {
		nou->next = *lista;
		*lista = nou;
		return;
	}
	N* poz = *lista;
	while (poz->next != NULL && poz->next->info.pret < masinaNoua.pret) {
		poz = poz->next;
	}
	nou->next = poz->next;
	poz->next = nou;
}


//EX 3
Masina* adaugaMasiniFiltrateInVector(N* lista, float prag, int* dim) {
	int contor = 0;
	N* aux = lista;

	while (aux) {
		if (aux->info.pret > prag) {
			contor++;
		}
		aux = aux->next;
	}

	*dim = contor;

	if (contor == 0)
		return NULL;

	Masina* vector = (Masina*)malloc(contor * sizeof(Masina));
	aux = lista;
	int index = 0;
	while (aux) {
		if (aux->info.pret > prag) {
			vector[index].id = aux->info.id;
			vector[index].nrUsi = aux->info.nrUsi;
			vector[index].pret = aux->info.pret;

			vector[index].model = malloc(strlen(aux->info.model) + 1);
			strcpy(vector[index].model, aux->info.model);

			vector[index].numeSofer = malloc(strlen(aux->info.numeSofer) + 1);
			strcpy(vector[index].numeSofer, aux->info.numeSofer);

			vector[index].serie = aux->info.serie;

			index++;
		}
		aux = aux->next;
	}
	return vector;
}


// EX4
void schimbareNoduriInLista(N** lista, int poz1, int poz2){
    if(lista == NULL || poz1 == poz2){
        return ;
    }
    if(poz1 > poz2){
        int aux = poz1;
        poz1 = poz2;
        poz2 = aux;
    }

    N* prev1 = NULL, *prev2 = NULL;
    N* nod1 = *lista, *nod2 = *lista;

    for(int i = 0; i < poz1 && nod1 != NULL; i++){
        prev1 = nod1;
        nod1 = nod1->next;
    }

    for(int i = 0; i < poz2 && nod2 != NULL; i++){
        prev2 = nod2;
        nod2 = nod2->next;
    }

    if(prev1){
        prev1->next = nod2;
    } else {
        *lista = nod2;
    }

    if(prev2){
        prev2->next = nod1;
    } else {
        *lista = nod1;
    }

    N* aux = nod1->next;
    nod1->next = nod2->next;
    nod2->next = aux;
}

N* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f;
	f = fopen(numeFisier, "r");
	N* lista = NULL;

	while (!feof(f)) {
		Masina m;
		m = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&lista, m);
	}

	fclose(f);
	return lista;
}


void dezalocareListaMasini(N** lista) {
	//sunt dezalocate toate masinile si lista de elemente

	while ((*lista)) {
		N* p = (*lista);
		(*lista) = (*lista)->next;

		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

float calculeazaPretMediu(N* lista) {
	//calculeaza pretul mediu al masinilor din lista.

	float suma = 0;
	int contor = 0;

	while (lista) {
		suma += lista->info.pret;
		contor++;
		lista = lista->next;
	}

	if (contor == 0) {
		return 0;
	}
	return suma / contor;
}

void stergeMasiniDinSeria(/*lista masini*/ char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

float calculeazaPretulMasinilorUnuiSofer(N* lista, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.

	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	return suma;
}


//EX 1
void stergeMasinaLaIndex(N** nod, int pozitie) {
	if (*nod == NULL || pozitie < 0) {
		return;
	}

	N* aux = *nod;
	if (pozitie == 0) {
		*nod = aux->next;
		if (aux->info.model) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		free(aux);
		return;
	}

	for (int i = 0; aux != NULL && i < pozitie - 1; i++) {
		aux = aux->next;
	}

	if (aux == NULL || aux->next == NULL) {
		return;
	}

	N* nodDeSters = aux->next;
	aux->next = nodDeSters->next;

	free(nodDeSters->info.model);
	free(nodDeSters->info.numeSofer);
	free(nodDeSters);
}

int getNrUsiMasinaScumpa(N* lista) {
	if (lista) {
		float pretMaxim = lista->info.pret;
		int nrUsi = lista->info.nrUsi;
		lista = lista->next;
		while (lista) {
			if (lista->info.pret > pretMaxim) {
				nrUsi = lista->info.nrUsi;
				pretMaxim = lista->info.pret;
			}
			lista = lista->next;
		}
		return nrUsi;
	}
	return 0;  // Return default value if lista is NULL
}

int main() {
	N* nod;
	nod = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(nod);
	float medie = calculeazaPretMediu(nod);
	// printf("Media este : %.2f", medie);
	// float sumaSofer = calculeazaPretulMasinilorUnuiSofer(nod, "Ionescu");
	// printf("\nSuma soferului este : %.2f", sumaSofer);
	// int nrUsi = getNrUsiMasinaScumpa(nod);
	// printf("\nNumarul de usi ale celei mai scumpe masini este: %i", nrUsi);


	// printf("\n\nMasini filtrate: \n");

	// int dim = 0;
	// float prag = 10000;
	// Masina* masini = adaugaMasiniFiltrateInVector(nod, prag, &dim);
	// for (int i = 0; i < dim; i++) {
	// 	afisareMasina(masini[i]);
	// }
	// for (int i = 0; i < dim;i++) {
	// 	free(masini[i].model);
	// 	free(masini[i].numeSofer);
	// }
	// free(masini);
    

    printf("\n\n");
    printf("Lista dupa schimbare: \n");
    schimbareNoduriInLista(&nod, 0, 3);
    afisareListaMasini(nod);
	return 0;
}