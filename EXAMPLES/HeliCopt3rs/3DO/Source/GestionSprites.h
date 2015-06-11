// SHMUP8

#ifndef H_SPRITES
#define H_SPRITES 1

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
// Necessaire pour gerer plusieurs images d'un fichier ANIM
#include <animutils.h>
/* ------------------------------ */

// Declaration du dossier ou sont stockes les sprites
//#define	DossierSprites "Sprites/"
#define	DossierSprites ""

// Structure du sprite
typedef struct Sprite{
	// Noeud dans la liste d'affichage
	Node *noeud;
	// Indique le type de noeud pour utiliser la bonne methode de rendu
	// Ici, elle doit etre initialisee avec la valeur 1 et ne doit pas entre modifiee
	int32 noeudType;
	// CCB (Cell Control Block) du sprite
	// Structure de donnees contenant les informations qui controlent la projection d'une cellule.
	// La structure contient :
	// - la position de la cellule dans le tampon (frame buffer)
	// - le quadrilatere de projection
	// - un pointeur vers la source de donnees de la cellule
	// - les valeurs pour regler la luminosite
	// - les valeurs pour regler la position des coins (x/y)
	// - les liens vers d'autres cellules
	CCB *pointeurCCB;
	// Contenu graphique du sprite
	ANIM *pointeurSpriteContenu;
	// Largeur et hauteur du sprite
	// Ces valeurs ne sont pas destinees a etre modifiees
	// Elles permettent de conserver les valeurs d'origines
	int32 largeurOriginale;
	int32 hauteurOriginale;
	// Largeur et hauteur du sprite
	// Ces valeurs correspondent a celles utilisees pour l'affichage
	int32 largeur;
	int32 hauteur;
	// Position X/Y de chaque coin du sprite
	// Ces valeurs ne sont pas destinees a etre modifiees
	// Elles permettent de conserver les valeurs d'origines
	Point coinOriginal1;
	Point coinOriginal2;
	Point coinOriginal3;
	Point coinOriginal4;
	// Position X/Y de chaque coin du sprite
	Point coin1;
	Point coin2;
	Point coin3;
	Point coin4;
	// Position X/Y du centre du sprite a partir du point coin1
	Point centre;
	// Index de l'image en cours
	int32 imageIndex;
	// Indique que le sprite est anime automatiquement selon son nombre de frames
	// Des que cette variable passe a vraie, l'animation demarre en prenant pour depart l'image affiche a l'aide de imageIndex
	bool animationCyclique;
	// Indique le mode d'animation cyclique utilise
	// 0 : les images sont affichees successivement en partant de l'index le plus faible au plus eleve
	// 1 : les images sont affichees successivement en partant de l'index le plus eleve au plus faible
	// 2 : les images sont affichees successivement en partant de l'index le plus faible au plus eleve, puis du plus eleve au plus faible en commencant par le debut
	// 3 : les images sont affichees successivement en partant de l'index le plus faible au plus eleve, puis du plus eleve au plus faible en commencant par la fin
	int32 animationCycliqueMode;
	// Indique l'intervalle de l'animation cyclique
	// animationCycliqueIntervalleFin doit etre superieur a animationCycliqueIntervalleDebut
	// animationCycliqueIntervalleFin doit etre inferieur a pointeurSpriteContenu->num_Frames
	int32 animationCycliqueIntervalleDebut;
	int32 animationCycliqueIntervalleFin;
	// Indique le nombre de cycles necessaire au changement de frame
	int32 animationCycles;
	// Indique le nombre de cycles ecoules
	int32 animationCyclesCompteur;
	// Pointeur vers le sprite clone
	struct Sprite *pointeurClone;
} Sprite;

// Set common CCB
CCB *SetCommonCCB(ANIM* pointeurSpriteContenu);

/* Parcourt les sprites de la liste et les affiche sur l'ecran */
bool spritesListeAffichage(ScreenContext *pointeurEcranContexte, int32 ecranCourant);
/* Charge un sprite et effectue des configurations de base */
struct Sprite *spriteChargement(char *pointeurFichier, int32 positionX, int32 positionY, int32 centreX, int32 centreY, int32 imageIndex, bool animationCyclique, int32 animationCycliqueMode, int32 animationCycliqueIntervalleDebut, int32 animationCycliqueIntervalleFin, int32 animationCycles);
/* Clone un sprite dans un autre */
struct Sprite *spriteClone(struct Sprite *pointeurSprite);
/* Copie un sprite dans un autre */
struct Sprite *spriteCopie(struct Sprite *pointeurSprite);
/* Modifie le fichier du sprite */
void spriteModificationFichier(struct Sprite *pointeurSprite, char *pointeurFichier);
/* Modifie le placage du sprite en fonction des valeurs X/Y de chacun des coins */
void spriteModificationPlacage(struct Sprite *pointeurSprite, int32 coin1X, int32 coin1Y, int32 coin2X, int32 coin2Y, int32 coin3X, int32 coin3Y, int32 coin4X, int32 coin4Y);
/* Modifie la position du sprite */
void spriteModificationPosition(struct Sprite *pointeurSprite, int32 valeurX, int32 valeurY, bool relative);
/* Modifie le centre du sprite */
void spriteModificationCentre(struct Sprite *pointeurSprite, int32 centreX, int32 centreY);
/* Modifie la taille du sprite en fonction de la position de son origine */
void spriteModificationTaille(struct Sprite *pointeurSprite, float valeurX, float valeurY, bool relative);
/* Modifie la rotation du sprite en fonction d'un angle dans le sens des aiguilles d'une montre */
void spriteModificationRotation(struct Sprite *pointeurSprite, int32 angle);
/* Configure la frame affichee du sprite */
void spriteConfigurationFrame(struct Sprite *pointeurSprite, int32 imageIndex);
/* Configure l'animation cyclique du sprite */
void spriteConfigurationAnimationCyclique(struct Sprite *pointeurSprite, bool animationCyclique, int32 animationCycliqueMode, int32 animationCycliqueIntervalleDebut, int32 animationCycliqueIntervalleFin, int32 animationCycles);

#endif