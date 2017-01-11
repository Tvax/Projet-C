#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	char* Nom;
	int codeJeux;
	bool emprunte; //si un jeux est emprunte ou non;
}Jeux;

typedef struct{
	char *Nom;
	int JourIns;
	int MoisIns;
	int AnnIns;
	int nbPlaces;
}ApTh;

typedef struct{
	int numAd;
	char Nom[20];
	Jeux JeuxEmpruntes[2];
	int nbJeux;
	struct tm *dateIns;
	ApTh** ApMidiIns;
	int nbApMidi;
}Adherent;

typedef struct maillonAd{
	Adherent ad;
	struct maillonAd *suiv;
}MaillonAd, *ListeAdherent;

typedef struct maillonJeux{
	Jeux jeux;
	struct maillonJeux *suiv;
}MaillonJeux, *ListeJeux;

typedef struct maillonAp{
	ApTh ap;
	struct maillonAp *suiv;
}MaillonAp, *ListeAp;

int main();
void affichMenu();
void menu();

Adherent inscriptionAd();
ListeAdherent ensVideAd();
ListeAdherent insEnTeteAd(ListeAdherent l, Adherent ad);
ListeAdherent insAd(ListeAdherent l, Adherent ad);
ListeAdherent supEnTeteAd(ListeAdherent l);
ListeAdherent supAd(ListeAdherent l, int NumAd);
int listTailleAd(ListeAdherent head);
bool peutEmprunt(ListeAdherent l, ListeJeux lJeux, int numAd, int numJeux);

ListeJeux ensVideJeux();
ListeJeux insEnTeteJeux(ListeJeux l, Jeux jeux);
ListeJeux insJeux(ListeJeux l, Jeux jeux);

ListeAp ensVideAp();
ListeAp insEnTeteAp(ListeAp l, ApTh ap);
ListeAp insAp(ListeAp l, ApTh ap);

ListeAdherent chargAdherents(char *nomFic);
ListeJeux chargJeux(char *nomFic);
ListeAp chargApTh(char *nomFic);

void saveBin(char *adFic, char *jeuxFic, char *apthFic, ListeAdherent ad, ListeJeux jeux, ListeAp ap);

void affichListes(ListeAdherent ad, ListeJeux jeux, ListeAp ap);
void affichListeAd(ListeAdherent ad);
void affichListeJeux(ListeJeux jeux);
void affichListeAp(ListeAp ap);