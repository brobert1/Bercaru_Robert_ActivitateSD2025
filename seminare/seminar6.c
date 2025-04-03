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

typedef struct Nod Nod;

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

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
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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

void afisareListaMasini(Nod* lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}

}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow
	nou->next = NULL;

	if (*lista) {
		Nod* p = *lista; // Correct initialization
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*lista = nou;
	}

}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));
	ht.dim = dimensiune;
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(char key, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	int hash;
	if (dimensiune > 0) {
		hash = key % dimensiune;
	}
	else {
		return 0;
	}
	
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int poz = calculeazaHash(masina.serie, hash.dim);
	if (poz < hash.dim) {
		//avem coliziune
		if (hash.vector[poz] != NULL) {
			adaugaMasinaInLista(&(hash.vector[poz]), masina);
		}
		else {
			hash.vector[poz] = (Nod*)malloc(sizeof(Nod));
			hash.vector[poz]->info = masina;
			hash.vector[poz]->next = NULL;
		}
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(dimensiune);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, masina);
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor 
	for (int i = 0; i < ht.dim; i++) {
		printf("\nCluster %d\n", i);
		afisareListaMasini(ht.vector[i]);
	}

}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, char serie) {
	Masina m;
	m.id = -1;
	int pozitie = calculeazaHash(serie, ht.dim);
	if (ht.vector[pozitie] != NULL) {
		Nod* p = ht.vector[pozitie];
		while (p != NULL && p->info.serie != serie) {
			p = p->next;
		}
		if (p) {
			m = p->info;
			m.model = (char*)malloc(strlen(p->info.model) + 1);
			strcpy(m.model, p->info.model);

			m.numeSofer = (char*)malloc(strlen(p->info.numeSofer) + 1);
			strcpy(m.numeSofer, p->info.numeSofer);
		}
	}
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt", 7);
	afisareTabelaDeMasini(ht);
	Masina m;

	printf("\nMasina cu seria A\n");
	m = getMasinaDupaCheie(ht, 'A');
	afisareMasina(m);
	free(m.model);
	free(m.numeSofer);
	return 0;
}