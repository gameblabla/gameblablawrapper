// GestionTextes
#include "GestionTextes.h"

/* Charge une police */
FontDescriptor *policeChargement(char *pointeurFichier, FontDescriptor *police){
	// Initialise le chemin avec le dossier de base (voir GestionTextes.h)
	char fichierChemin[128]=DossierPolices;
	// Concatene le nom du fichier avec le chemin
	strcat(fichierChemin, pointeurFichier);
	// Charge la police
	police=LoadFont(fichierChemin, MEMTYPE_ANY);
	// Si la police n'a pas ete chargee
	if(police==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Retourne la police
	return police;
}

/* Supprime une police */
void policeSuppression(FontDescriptor *police){
	// Supprime la police
	// Note : Toutes les cellules de texte qui font reference a cette police doivent etre supprimees avant
	UnloadFont(police);
}

/* Charge un texte et effectue des configurations de base */
struct Texte *texteChargement(FontDescriptor *police, char *pointeurTexteContenu, int32 positionX, int32 positionY, int32 largeur, int32 hauteur, int32 margeGauche, int32 margeHaut, int32 couleurFond, int32 couleurTexte, int32 alignement, bool visible){
	// Cree un texte
	struct Texte *pointeurTexteTemporaire=NULL;
	// Si la police n'a pas ete chargee en memoire
	if(police==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Alloue de la memoire pour le texte
	pointeurTexteTemporaire=(Texte*)AllocMem(sizeof(Texte), MEMTYPE_ANY);
	// Si l'allocation a echoue
	if(pointeurTexteTemporaire==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Alloue de la memoire pour le noeud
	pointeurTexteTemporaire->noeud=(Node*)AllocMem(sizeof(Node), MEMTYPE_ANY);
	// Si l'allocation a echoue
	if(pointeurTexteTemporaire->noeud==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Indique que le noeud est de type Texte
	pointeurTexteTemporaire->noeud->n_Type=2;
	// Copie l'adresse de la police chargee en memoire
	pointeurTexteTemporaire->fontDesc=police;
	// Stocke la taille de la cellule du texte
	// Si la largeur et la hauteur sont a 0, la cellule fait une taille de 1 x 1 pixel par defaut
	// Dans ce cas, lorsque du texte est ajoute, la taille de la cellule s'adapte
	pointeurTexteTemporaire->largeur=largeur;
	pointeurTexteTemporaire->hauteur=hauteur;
	// Cree le texte en utilisant la police chargee
	// Le second parametre sert a specifier l'alignement
	// 0 : a gauche
	// 1 : a droite
	// 2 : centre
	// 8 : retour a la ligne automatique
	pointeurTexteTemporaire->tCel=CreateTextCel(pointeurTexteTemporaire->fontDesc, alignement, pointeurTexteTemporaire->largeur, pointeurTexteTemporaire->hauteur);
	// Modifie les marges
	texteModificationMarges(pointeurTexteTemporaire, margeGauche, margeHaut);
	// Modifie les couleurs
	texteModificationCouleur(pointeurTexteTemporaire, couleurFond, couleurTexte);
	// Modifie le texte
	texteModification(pointeurTexteTemporaire, pointeurTexteContenu, TRUE);
	// Initialise le placage de la cellule
	texteModificationPlacage(pointeurTexteTemporaire, positionX, positionY, (positionX+pointeurTexteTemporaire->largeur), positionY, (positionX+pointeurTexteTemporaire->largeur), (positionY+pointeurTexteTemporaire->hauteur), positionX, (positionY+pointeurTexteTemporaire->hauteur));
	// Retourne le texte
	return pointeurTexteTemporaire;
}

/* Modifie le Texte */
bool texteModification(struct Texte *pointeurTexte, char *pointeurTexteContenu, bool remplacement){
	// Retour interne
	int32 retour=0;
	// Modifie le texte
	retour=UpdateTextInCel(pointeurTexte->tCel, remplacement, pointeurTexteContenu);
	// Si il y a un probleme
	if(retour<0){
		// Retourne une erreur
		return FALSE;
	}
	// Retourne un succes
	return TRUE;
}
/* Modifie le placage de la cellule du texte en fonction des valeurs X/Y de chacun des coins */
void texteModificationPlacage(struct Texte *pointeurTexte, int32 coin1X, int32 coin1Y, int32 coin2X, int32 coin2Y, int32 coin3X, int32 coin3Y, int32 coin4X, int32 coin4Y){
	// Modifie le placage de la cellule du texte en partant du coin en haut a gauche et dans le sens des aiguilles d'une montre
	// Permet d'etirer la cellule pour simuler une pespective
	pointeurTexte->coin1.pt_X=coin1X;
	pointeurTexte->coin1.pt_Y=coin1Y;
	pointeurTexte->coin2.pt_X=coin2X;
	pointeurTexte->coin2.pt_Y=coin2Y;
	pointeurTexte->coin3.pt_X=coin3X;
	pointeurTexte->coin3.pt_Y=coin3Y;
	pointeurTexte->coin4.pt_X=coin4X;
	pointeurTexte->coin4.pt_Y=coin4Y;
}

/* Modifie la position de la cellule du Texte */
void texteModificationPosition(struct Texte *pointeurTexte, int32 valeurX, int32 valeurY, bool relative){
	// Si la modification est relative
	if(relative){
		// Modifie la position du sprite en affectant toutes les valeurs X/Y
		// Pour deplacer le sprite de 30 pixels vers la droite mettre valeurX a 30
		// Par exemple : spriteModificationPosition(pointeurTexte, 120, 120, FALSE);
		// Puis : spriteModificationPosition(pointeurTexte, 30, 0, TRUE);
		pointeurTexte->coin1.pt_X=pointeurTexte->coin1.pt_X+valeurX;
		pointeurTexte->coin1.pt_Y=pointeurTexte->coin1.pt_Y+valeurY;
		pointeurTexte->coin2.pt_X=pointeurTexte->coin2.pt_X+valeurX;
		pointeurTexte->coin2.pt_Y=pointeurTexte->coin2.pt_Y+valeurY;
		pointeurTexte->coin3.pt_X=pointeurTexte->coin3.pt_X+valeurX;
		pointeurTexte->coin3.pt_Y=pointeurTexte->coin3.pt_Y+valeurY;
		pointeurTexte->coin4.pt_X=pointeurTexte->coin4.pt_X+valeurX;
		pointeurTexte->coin4.pt_Y=pointeurTexte->coin4.pt_Y+valeurY;
	// Sinon la modification est absolue
	}else{
		// Modifie la position du sprite en affectant uniquement les valeurs X/Y du point d'origine
		// Pour deplacer le sprite de 30 pixels vers la droite recuperer prealablement la valeur de position1.pt_X et lui ajouter 30
		// Par exemple : spriteModificationPosition(pointeurTexte, 120, 120, FALSE);
		// Puis : spriteModificationPosition(pointeurTexte, 150, 120, FALSE);
		pointeurTexte->coin1.pt_X=valeurX;
		pointeurTexte->coin1.pt_Y=valeurY;
		pointeurTexte->coin2.pt_X=valeurX+pointeurTexte->largeur;
		pointeurTexte->coin2.pt_Y=valeurY;
		pointeurTexte->coin3.pt_X=valeurX+pointeurTexte->largeur;
		pointeurTexte->coin3.pt_Y=valeurY+pointeurTexte->hauteur;
		pointeurTexte->coin4.pt_X=valeurX;
		pointeurTexte->coin4.pt_Y=valeurY+pointeurTexte->hauteur;
	}
}

/* Modifie les marges du Texte */
void texteModificationMarges(struct Texte *pointeurTexte, int32 margeGauche, int32 margeHaut){
	// Stocke les valeurs
	pointeurTexte->margeGauche=margeGauche;
	pointeurTexte->margeHaut=margeHaut;
	// Modifie les marges
	SetTextCelMargins(pointeurTexte->tCel, margeGauche, margeHaut);
}

/* Modifie les couleurs du Texte */
void texteModificationCouleur(struct Texte *pointeurTexte, int32 couleurFond, int32 couleurTexte){
	// Modifie les couleurs
	// Chaque couleur est determinee avec une valeur numerique obtenue a partir de MakeRGB15(R,G,B)
	// R, G et B vont de 0 a 255
	// La transparence est obtenue avec MakeRGB15(0,0,0)
	// Le noir pur est obtenu avec MakeRGB15(0,0,1)
	SetTextCelColor(pointeurTexte->tCel, couleurFond, couleurTexte);
}