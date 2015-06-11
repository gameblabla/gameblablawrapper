// GestionTextes

#ifndef H_TEXTES
#define H_TEXTES 1

/* Inclusions des bibliotheques C */
// Necessaire pour la manipulation des chaines de caracteres
#include <string.h>
// Necessaire pour l'utilisation de fonctions mathematiques courantes
#include <math.h>
/* ------------------------------ */


// Necessaire pour la manipulation des cellules
#include <celutils.h>
// Necessaire pour la gestion des écrans, des fonds et des effets de fondu
#include <displayutils.h>
// Necessaire pour la gestion de memoire
#include <mem.h>
#include <umemory.h>
// Necessaire pour la gestion des listes
#include <list.h>
// Necessaire pour la gestion du texte
#include <textlib.h>
// Necessaire pour la gestion des noeuds
#include <nodes.h>
// Necessaire pour la gestion des polices
#include <fontlib.h>
/* ------------------------------ */

// Declaration du dossier ou sont stockes les polices
#define	DossierPolices "Fonts/"

// Structure du texte
typedef struct Texte{
	// Noeud dans la liste d'affichage
	Node *noeud;
	// Police
	FontDescriptor *fontDesc;
	// Cellule dans laquelle est place le texte
	TextCel *tCel;
	// Largeur et hauteur de la cellule
	int32 largeur;
	int32 hauteur;
	// Position X/Y de chaque coin du sprite
	Point coin1;
	Point coin2;
	Point coin3;
	Point coin4;
	// Marges du texte
	int32 margeGauche;
	int32 margeHaut;
} Texte;

/* Charge une police */
FontDescriptor *policeChargement(char *pointeurFichier, FontDescriptor *police);
/* Supprime une police */
void policeSuppression(FontDescriptor *police);
/* Charge un texte et effectue des configurations de base */
struct Texte *texteChargement(FontDescriptor *police, char *pointeurTexteContenu, int32 positionX, int32 positionY, int32 celluleLargeur, int32 celluleHauteur, int32 margeGauche, int32 margeHaut, int32 couleurFond, int32 couleurTexte, int32 alignement, bool visible);
/* Modifie le Texte */
bool texteModification(struct Texte *pointeurTexte, char *pointeurTexteContenu, bool remplacement);
/* Modifie le placage de la cellule du texte en fonction des valeurs X/Y de chacun des coins */
void texteModificationPlacage(struct Texte *pointeurTexte, int32 coin1X, int32 coin1Y, int32 coin2X, int32 coin2Y, int32 coin3X, int32 coin3Y, int32 coin4X, int32 coin4Y);
/* Modifie la position de la cellule du Texte */
void texteModificationPosition(struct Texte *pointeurTexte, int32 valeurX, int32 valeurY, bool relative);
/* Modifie les marges du Texte */
void texteModificationMarges(struct Texte *pointeurTexte, int32 margeGauche, int32 margeHaut);
/* Modifie les couleurs du Texte */
void texteModificationCouleur(struct Texte *pointeurTexte, int32 couleurFond, int32 couleurTexte);

#endif