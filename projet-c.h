#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	int JourIns;
	int MoisIns;
	int AnnIns;
}Date;
typedef struct {
	char Nom[20];
	int codeJeux;
	Date d; //date a laquelle location se termine
	bool emprunte; //si un jeux est emprunte ou non
}Jeux;
typedef struct {
	char Nom[20];
	int numAp;
	Date d;
	int nbPlaces;
}ApTh;

typedef struct {
	int numAd;
	char Nom[20];
	Jeux JeuxEmpruntes[3]; //3 parce qu'on commence a 1 et pas à 0
	int nbJeux;
	Date d;
	ApTh ApMidiIns[50];
	int nbApMidi;
}Adherent;

typedef struct maillonAd {
	Adherent ad;
	struct maillonAd *suiv;
}MaillonAd, *ListeAdherent;

typedef struct maillonJeux {
	Jeux jeux;
	struct maillonJeux *suiv;
}MaillonJeux, *ListeJeux;

typedef struct maillonAp {
	ApTh ap;
	struct maillonAp *suiv;
}MaillonAp, *ListeAp;

int main();
void affichMenu();
void menu();

Date getDate();
int compDates(Date d1, Date d2);
void affichDate(Date d);
Date ajouterUnMois(Date d);

Adherent inscriptionAd();
ListeAdherent ensVideAd();
ListeAdherent insEnTeteAd(ListeAdherent l, Adherent ad);
ListeAdherent insAd(ListeAdherent l, Adherent ad);
ListeAdherent supEnTeteAd(ListeAdherent l);
ListeAdherent supAd(ListeAdherent l, int NumAd);
int listTailleAd(ListeAdherent head);
bool peutEmprunt(ListeAdherent l, ListeJeux lJeux, int numAd, int numJeux);
ListeJeux empruntJeuxJ(ListeJeux jeux, int numJeux);
ListeAdherent empruntJeuxAd(ListeAdherent ad, ListeJeux jeux, int numAd, int numJeux);
bool peutInscrApth(ListeAdherent l, ListeAp ap, int numAd, int numAp);
ListeAdherent inscripApthAd(ListeAdherent ad, ListeAp ap, int numAd, int numAp);
ListeAp inscripApthAp(ListeAp ap, int numAp);
void inscritApTh(ListeAdherent l);
void inscritApthPrecise(ListeAdherent l, int numAp);
void empruntRet(ListeAdherent l, ListeJeux j);

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
