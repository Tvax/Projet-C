#include "projet-c.h"
#include <time.h>

void affichMenu(){
	printf("\n\t---\n\n");
	printf("1. Inscription nouvel adherent\n");
	printf("2. Supprimer adherent\n");
	printf("3. Enregistrer un emprunt\n");
	printf("4. Enregistrer un apres-midi thematique\n");
	printf("5. Enregistrer l'inscription d'un adherent a une apres-midi thematique\n");
	printf("6. Adherents inscrits a une apres-midi thematique\n");
	printf("7. Emprunteur en retard\n");
	printf("8. Afficher tous les adherents (ordre alphabetique)\n");
	printf("9. Afficher tous les jeux (ordre alphabetique)\n");
	printf("10. Afficher toutes les apres-midis (ordre alphabetique)\n");
	printf("0. Quitter\n");
}

void menu(){
	int i, numAd, numJeux, numAp;
	ApTh ap;

	ListeAdherent listeAd = chargAdherents("adherents.bin");
	ListeJeux listeJeux = chargJeux("jeux.bin");
	ListeAp listeAp = chargApTh("apTh.bin");

	affichListes(listeAd, listeJeux, listeAp);

	while(true){
		affichMenu();
		printf("\nChoix: ");
		scanf("%d", &i);

		switch(i){
		case 0:
			printf("Enregistrement\n");
			saveBin("adherents.bin", "jeux.bin", "apTh.bin", listeAd, listeJeux, listeAp);
			printf("Fermeture");
			exit(0);
			break;
		case 1:
			listeAd = insAd(listeAd, inscriptionAd(listTailleAd(listeAd)));
			printf("Adherent cree !\n");
			affichListeAd(listeAd);
			break;
		case 2:
			printf("Num adherent a supprimer : ");
			scanf("%d", &numAd);
			listeAd = supAd(listeAd, numAd);
			printf("Adherent supprime !\n");
			break;
		case 3:
			printf("Enregistrer un emprunt\n");
			printf("Num adherent : ");
			scanf("%d", &numAd);
			printf("Num jeux : ");
			scanf("%d", &numJeux);

			if(peutEmprunt(listeAd, listeJeux, numAd, numJeux)){
				listeAd = empruntJeuxAd(listeAd, listeJeux, numAd, numJeux);
				listeJeux = empruntJeuxJ(listeJeux, numJeux);
			}
			break;
		case 4:
			printf("Ajout d'une apres midi thematique\n");

			printf("Date : ");
			scanf("%d%d%d", &ap.d.JourIns, &ap.d.MoisIns, &ap.d.AnnIns);
			if(compDates(getDate(), ap.d) < 0){
				printf("Date depassee ! \n");
				break;
			}

			printf("Nom : ");
			fflush(stdin);
			fgets(ap.Nom, sizeof(ap.Nom), stdin);
			ap.Nom[strlen(ap.Nom) - 1] = '\0';

			printf("Nombre de places : ");
			scanf("%d", &ap.nbPlaces);
			ap.numAp++;
			listeAp = insAp(listeAp, ap);
			break;
		case 5:
			printf("Inscription a une apres-midi thematique\n");
			printf("Num adherent : ");
			scanf("%d", &numAd);
			printf("Num apres-midi : ");
			scanf("%d", &numAp);

			if(peutInscrApth(listeAd, listeAp, numAd, numAp)){
				listeAd = inscripApthAd(listeAd, listeAp, numAd, numAp);
				listeAp = inscripApthAp(listeAp, numAp);
			}
			break;
		case 6:
			printf("Liste Inscrits a une apres-midi\n");
			printf("Numero apres-midi: ");
			scanf("%d", &numAp);
			inscritApthPrecise(listeAd, numAp);
			break;
		case 7:
			break;
		case 8:
			affichListeAd(listeAd);
			break;
		case 9:
			affichListeJeux(listeJeux);
			break;
		case 10:
			affichListeAp(listeAp);
			break;
		default:
			printf("Choix invalide!\n");
		}
	}
}

ListeAdherent empruntJeuxAd(ListeAdherent ad, ListeJeux jeux, int numAd, int numJeux){
	while(jeux != NULL){
		if(jeux->jeux.codeJeux == numJeux)
			while(ad != NULL){
				if(ad->ad.numAd == numAd){
					ad->ad.nbJeux++;
					ad->ad.JeuxEmpruntes[ad->ad.nbJeux] = jeux->jeux;
					break;
				}
				ad = ad->suiv;
			}
		jeux = jeux->suiv;
	}
	return ad;
}
ListeJeux empruntJeuxJ(ListeJeux jeux, int numJeux){
	while(jeux != NULL){
		if(jeux->jeux.codeJeux == numJeux){
			jeux->jeux.emprunte = true;
			jeux->jeux.d = getDate();
			jeux->jeux.d = ajouterUnMois(jeux->jeux.d);
			break;
		}
		jeux = jeux->suiv;
	}
	return jeux;
}
bool peutEmprunt(ListeAdherent l, ListeJeux lJeux, int numAd, int numJeux){
	Date tmp;

	while(l != NULL){
		if(l->ad.numAd == numAd){
			if(l->ad.nbJeux == 3){
				printf("L'adherent emprunte le maximum de jeux autorise\n");
				return false;
			}
			if(l->ad.nbJeux == 1 || l->ad.nbJeux == 2){ //regarde si il y a pas un jeu emprunte plus de 3 semaines
				int i = 0;
				for(i; i < 2; i++)
					if(compDates(getDate(), l->ad.JeuxEmpruntes[i].d) < 0){
						printf("%s emprunte depuis plus de trois semaines\n", l->ad.JeuxEmpruntes[i].Nom);
						return false;
					}
			}
			tmp = l->ad.d; //pour voir si une annee apres inscription pas depasse
			tmp.AnnIns++;
			if(compDates(getDate(), tmp) < 0){
				printf("Linscription est terminee\n");
				return false;
			}
		}

		while(lJeux != NULL){
			if(lJeux->jeux.codeJeux == numJeux){
				if(lJeux->jeux.emprunte){
					printf("Jeux deja emprunte");
					return false;
				}
				return true;
			}
			lJeux = lJeux->suiv;
		}
		l = l->suiv;
	}
	printf("Num inconnu\n"); //du jeux
	return false;
}

ListeAdherent inscripApthAd(ListeAdherent ad, ListeAp ap, int numAd, int numAp){
	ApTh tmp[50];
	while(ap != NULL){
		if(ap->ap.numAp == numAp)
			while(ad != NULL){
				if(ad->ad.numAd == numAd){
					ad->ad.nbApMidi++;
					if(ad->ad.nbApMidi > 50){
						printf("Nombre maximum d'apres-midi thematique depasse !\n");
						return ad;
					}
					ad->ad.ApMidiIns[ad->ad.nbApMidi] = ap->ap;
					break;
				}
				ad = ad->suiv;
			}
		ap = ap->suiv;
	}
	return ad;
}
ListeAp inscripApthAp(ListeAp ap, int numAp){
	while(ap != NULL){
		if(ap->ap.numAp == numAp){
			ap->ap.nbPlaces--;
			break;
		}
		ap = ap->suiv;
	}
	return ap;
}
bool peutInscrApth(ListeAdherent l, ListeAp ap, int numAd, int numAp){
	int i;
	Date tmp;
	while(l != NULL){
		if(l->ad.numAd == numAd){
			if(l->ad.nbApMidi != 0)
				for(i = 0; i < l->ad.nbApMidi; i++)
					if(l->ad.ApMidiIns[i].numAp == numAp){
						printf("Adherent deja inscrit a cette apres-midi\n");
						return false;
					}
			tmp = l->ad.d; //pour voir si une annee apres inscription pas depasse
			tmp.AnnIns++;
			if(compDates(getDate(), tmp) < 0){
				printf("Linscription est terminee\n");
				return false;
			}
		}

		while(ap != NULL){
			if(ap->ap.numAp == numAp){
				if(ap->ap.nbPlaces == 0){
					printf("Plus de places disponibles");
					return false;
				}
				return true;
			}
			ap = ap->suiv;
		}
		l = l->suiv;
	}
	printf("Num inconnu\n"); //du jeux
	return false;
}

void inscritApthPrecise(ListeAdherent l, int numAp){
	int i;

	while(l != NULL){
		for(i = 0; i <= l->ad.nbApMidi; i++)
			if(l->ad.ApMidiIns[i].numAp == numAp)
				printf("%s", l->ad.Nom);
		l = l->suiv;
	}
}
void empruntRet(ListeAdherent l, ListeJeux j){
	int i;

	while(l != NULL){
		for(i = 0; i < l->ad.nbJeux; i++)
			while(j != NULL){
				if(l->ad.JeuxEmpruntes[i].codeJeux == j->jeux.codeJeux)
					if(compDates(getDate(), j->jeux.d) < 0)
					   printf("%s a depasse le delai de location pour %s", l->ad.Nom, j->jeux.Nom);

				j = j->suiv;
			}
		l = l->suiv;
	}
}

Adherent inscriptionAd(int tailleListe){
	Adherent ad;
	ad.d = getDate();

	printf("\n\t---Inscription Adherent---\n");
	printf("Mois : %d\n", ad.d.MoisIns);
	printf("Jour : %d\n", ad.d.JourIns);
	printf("Annee : %d\n", ad.d.AnnIns);

	printf("Nom : ");
	fflush(stdin);
	fgets(ad.Nom, sizeof(ad.Nom), stdin);
	ad.Nom[strlen(ad.Nom) - 1] = '\0';

	ad.nbJeux = 0;
	ad.nbApMidi = 0;
	ad.numAd = tailleListe + 1;
	return ad;
}

ListeAdherent ensVideAd(){
	return NULL;
}
ListeAdherent insEnTeteAd(ListeAdherent l, Adherent ad){

	MaillonAd *m;
	m = (MaillonAd *)malloc(sizeof(MaillonAd));
	if(m == NULL)
		exit(1);

	m->ad = ad;
	m->suiv = l;

	return m;
}
void inscritApTh(ListeAdherent l){
	while(l != NULL){
		if(l->ad.nbApMidi > 0)
			l = insAd(l, l->ad);
		l = l->suiv;
	}
	affichListeAd(l);
}
ListeAdherent insAd(ListeAdherent l, Adherent ad){

	if(l == NULL)
		return insEnTeteAd(l, ad);
	if(strcmp(ad.Nom, l->ad.Nom) < 0)
		return insEnTeteAd(l, ad);
	if(strcmp(ad.Nom, l->ad.Nom) == 0)
		return l;
	l->suiv = insAd(l->suiv, ad);
	return l;
}
ListeAdherent supEnTeteAd(ListeAdherent l){
	MaillonAd *m;

	if(l == NULL)
		exit(1);
	m = l;
	l = l->suiv;
	free(m);
	return l;
}
ListeAdherent supAd(ListeAdherent l, int numAd){
	if(l == NULL)
		return l;
	if(numAd < l->ad.numAd)
		return l;
	if(numAd == l->ad.numAd)
		return supEnTeteAd(l);

	l->suiv = supAd(l->suiv, numAd);
	return l;
}
int listTailleAd(ListeAdherent head){
	int len = 0;
	while(head != NULL){
		len++;
		head = head->suiv;
	}
	return len;
}


ListeJeux ensVideJeux(){
	return NULL;
}
ListeJeux insEnTeteJeux(ListeJeux l, Jeux jeux){

	MaillonJeux *m;
	m = (MaillonJeux *)malloc(sizeof(MaillonJeux));
	if(m == NULL)
		exit(1);

	m->jeux = jeux;
	m->suiv = l;

	return m;

}
ListeJeux insJeux(ListeJeux l, Jeux jeux){

	if(l == NULL)
		return insEnTeteJeux(l, jeux);
	if(strcmp(jeux.Nom, l->jeux.Nom) < 0)
		return insEnTeteJeux(l, jeux);
	if(strcmp(jeux.Nom, l->jeux.Nom) == 0)
		return l;
	l->suiv = insJeux(l->suiv, jeux);
	return l;
}

ListeAp ensVideAp(){
	return NULL;
}
ListeAp insEnTeteAp(ListeAp l, ApTh ap){

	MaillonAp *m;
	m = (MaillonAp *)malloc(sizeof(MaillonAp));
	if(m == NULL)
		exit(1);

	m->ap = ap;
	m->suiv = l;

	return m;

}
ListeAp insAp(ListeAp l, ApTh ap){

	if(l == NULL)
		return insEnTeteAp(l, ap);
	if(strcmp(ap.Nom, l->ap.Nom) < 0)
		return insEnTeteAp(l, ap);
	if(strcmp(ap.Nom, l->ap.Nom) == 0)
		return l;
	l->suiv = insAp(l->suiv, ap);
	return l;
}

ListeAdherent chargAdherents(char *nomFic){
	FILE *fe;
	fe = fopen(nomFic, "rb");
	ListeAdherent ad = ensVideAd();
	Adherent adherent;

	if(fe == NULL){
		printf("Erreur ouverture %s", nomFic);
		exit(1);
	}

	fread(&adherent, sizeof(Adherent), 1, fe);
	ad = insAd(ad, adherent);

	while(!feof(fe)){
		fread(&adherent, sizeof(Adherent), 1, fe);
		ad = insAd(ad, adherent);
	}
	fclose(fe);
	return ad;
}
ListeJeux chargJeux(char *nomFic){
	FILE *fe;
	fe = fopen(nomFic, "rb");
	ListeJeux listeJeux = ensVideJeux();
	Jeux jeux;

	if(fe == NULL){
		printf("Erreur ouverture %s", nomFic);
		exit(1);
	}

	fread(&jeux, sizeof(Jeux), 1, fe);
	listeJeux = insJeux(listeJeux, jeux);

	while(!feof(fe)){
		fread(&jeux, sizeof(Jeux), 1, fe);
		listeJeux = insJeux(listeJeux, jeux);
	}
	fclose(fe);
	return listeJeux;
}
ListeAp chargApTh(char *nomFic){
	FILE *fe;
	fe = fopen(nomFic, "rb");
	ListeAp listeAp = ensVideAp();
	ApTh ap;

	if(fe == NULL){
		printf("Erreur ouverture %s", nomFic);
		exit(1);
	}

	fread(&ap, sizeof(ApTh), 1, fe);
	listeAp = insAp(listeAp, ap);

	while(!feof(fe)){
		fread(&ap, sizeof(Jeux), 1, fe);
		listeAp = insAp(listeAp, ap);
	}
	fclose(fe);
	return listeAp;
}

void saveBin(char *adFic, char *jeuxFic, char *apthFic, ListeAdherent ad, ListeJeux jeux, ListeAp ap){
	FILE *fe;
	fe = fopen(adFic, "wb");
	if(fe == NULL){
		printf("Erreur ouverture %s", adFic);
		exit(1);
	}
	while(ad != NULL){
		fwrite(&ad->ad, sizeof(Adherent), 1, fe);
		ad = ad->suiv;
	}
	fclose(fe);

	printf("Sauvegarde 1/3\n");


	fe = fopen(jeuxFic, "wb");
	if(fe == NULL){
		printf("Erreur ouverture %s", jeuxFic);
		exit(1);
	}
	while(jeux != NULL){
		fwrite(&jeux->jeux, sizeof(Jeux), 1, fe);
		jeux = jeux->suiv;
	}
	fclose(fe);

	printf("Sauvegarde 2/3\n");

	fe = fopen(apthFic, "wb");
	if(fe == NULL){
		printf("Erreur ouverture %s", apthFic);
		exit(1);
	}
	while(ap != NULL){
		fwrite(&ap->ap, sizeof(ApTh), 1, fe);
		ap = ap->suiv;
	}
	fclose(fe);

	printf("Sauvegarde 3/3\n");
}

void affichListes(ListeAdherent ad, ListeJeux jeux, ListeAp ap){

	affichListeAd(ad);
	affichListeJeux(jeux);
	affichListeAp(ap);
}
void affichListeAd(ListeAdherent ad){
	int i;

	printf("\t---Adherent---\n");
	if(ad == NULL){
		printf("Aucun adherent");
		return;
	}

	while(ad != NULL){
		char buffer[100];
		printf("Numero : %d\n", ad->ad.numAd);
		printf("Nom : %s\n", ad->ad.Nom);
		printf("Date Inscription : ");
		affichDate(ad->ad.d);

		if(ad->ad.nbJeux != 0)
			for(i = 1; i <= ad->ad.nbJeux; i++)
				printf("Jeux num %d : %s\n", i, ad->ad.JeuxEmpruntes[i].Nom);
		else
			printf("Aucun jeux emprunte !\n");

		if(ad->ad.nbApMidi != 0)
			for(i = 1; i != ad->ad.nbApMidi; i++)
				printf("Apres-midi num %d : %s\n", i, ad->ad.ApMidiIns[i].Nom);
		else
			printf("Aucune apres-midi reservee !\n");

		ad = ad->suiv;
		printf("\n");
	}
}
void affichListeJeux(ListeJeux jeux){
	printf("\t---Jeux---\n");
	while(jeux != NULL){
		printf("Jeux : %s\n", jeux->jeux.Nom);
		printf("Numero : %d\n", jeux->jeux.codeJeux);
		if(jeux->jeux.emprunte)
			printf("Emprunte\n");
		else
			printf("Libre\n");
		jeux = jeux->suiv;
		printf("\n");
	}
}
void affichListeAp(ListeAp ap){
	printf("\t---Apres-midi---\n");
	while(ap != NULL){
		printf("Numero : %d\n", ap->ap.numAp);
		printf("Apres-midi %s\n", ap->ap.Nom);
		printf("Date inscription : ");
		affichDate(ap->ap.d);
		printf("Place(s) restante(s) : %d", ap->ap.nbPlaces);
		ap = ap->suiv;
		printf("\n\n");
	}
}

void creerAd(){
	Adherent ad;
	strcpy(ad.Nom, "moi");

	ad.d = getDate();

	ad.nbApMidi = 0;
	ad.nbJeux = 0;
	ad.numAd = 1;

	FILE *fe;
	fe = fopen("adherents.bin", "wb");
	if(fe == NULL){
		printf("Erreur ouverture %s", "adherents.bin");
		exit(1);
	}
	fwrite(&ad, sizeof(Adherent), 1, fe);
	fclose(fe);

	Jeux j;
	j.codeJeux = 1;
	strcpy(j.Nom, "jeux 1");
	j.emprunte = false;

	fe = fopen("jeux.bin", "wb");
	if(fe == NULL){
		printf("Erreur ouverture %s", "jeux.bin");
		exit(1);
	}
	fwrite(&j, sizeof(Jeux), 1, fe);
	fclose(fe);

	ApTh a;
	strcpy(a.Nom, "Apth");
	a.nbPlaces = 2;
	a.d = getDate();
	a.d.JourIns++;
	a.numAp = 1;


	fe = fopen("apTh.bin", "wb");
	if(fe == NULL){
		printf("Erreur ouverture %s", "apTh.bin");
		exit(1);
	}
	fwrite(&a, sizeof(ApTh), 1, fe);
	fclose(fe);

	exit(0);
}

void affichDate(Date d){
	printf("%d/%d/%d\n", d.JourIns, d.MoisIns, d.AnnIns);
}
Date getDate(){
	Date d;
	char jour[10];
	system("echo %DATE% > date.don");

	FILE *fe;
	fe = fopen("date.don", "r");
	fscanf(fe, "%s%d/%d/%d", jour, &d.JourIns, &d.MoisIns, &d.AnnIns);
	fclose(fe);
	return d;
}
//d1 = dateNow d2 = dateAComparer 
//si le return < 0 ça fait plus de 3 semaines.
int compDates(Date d1, Date d2){
	int cmp;
	cmp = d2.AnnIns - d1.AnnIns;
	if(cmp == 0){
		cmp = d2.MoisIns - d1.MoisIns;
		if(cmp == 0)
			cmp = d2.JourIns - d1.JourIns;
	}
	return cmp;
}
Date ajouterUnMois(Date d){
	if(d.MoisIns + 1 > 12){
		d.MoisIns = 1;
		d.AnnIns++;
		d.JourIns = 31 - d.JourIns;
	} else
		d.MoisIns++;
	return  d;
}

int main(){
	//creerAd();
	menu();
	return 0;
}