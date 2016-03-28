// GestionAffichage

#define	Ecrans 2
//#define	DossierFondsPAL1 "PAL/Graphics/" // 320x288 pixels - 50 images/sec
//#define	DossierFondsPAL2 "PAL/Graphics/" // 384x288 pixels - 50 images/sec.
//#define	DossierFondsNTSC "NTSC/Graphics/" // 320x240 pixels - 60 images/sec.
#define	DossierFondsPAL1 "" // 320x288 pixels - 50 images/sec
#define	DossierFondsPAL2 "" // 384x288 pixels - 50 images/sec.
#define	DossierFondsNTSC "" // 320x240 pixels - 60 images/sec.

/* Inclusions des bibliotheques C */
// Necessaire pour la manipulation des chaines de caracteres
#include <string.h>
/* ------------------------------ */
// Necessaire pour la gestion de l'affichage
#include <graphics.h>
// Necessaire pour controler le format de restitution d'image (NTSC, PAL1 ou PAL2)
#include <getvideoinfo.h>
// Necessaire pour la gestion des écrans, des fonds et des effets de fondu
#include <displayutils.h>
// Necessaire pour la gestion de memoire
#include <mem.h>
#include <umemory.h>
// Necessaire pour la gestion des listes
#include <list.h>
// Necessaire pour la capture des evenements et les ecouteurs
#include <event.h>
/* ------------------------------ */

/* Inclusions des API personnelles */
// Gestion des sprites
#include "GestionSprites.h"
// Gestion des textes
#include "GestionTextes.h"
/* ------------------------------ */

/* Initialise l'affichage */
bool affichageInitialisation(void);
/* Cree le rendu de l'affichage en fonction des multiples elements */
void affichageRendu(void);
/* Cree un ecran neutre */
void affichageCouleurUnie(int32 couleur);
/* Met a jour l'affichage */
void affichageMiseAJour(void);
/* Supprime l'affichage */
void affichageSuppression(int32 fondu);
/* Charge une image de fond */
bool imageFondChargement(char *pointeurFichier);
/* Ajoute un element a la liste d'affichage */
void listeAffichageAjout(Node *pointeurElement, bool sens, int32 type, int32 index);
/* Modifie la priorite d'un element dans la liste d'affichage */
void listeAffichagePriorite(Node *pointeurElement, int32 index);
/* Supprime un element a la liste d'affichage */
void listeAffichageSuppression(Node *pointeurElement);
/* Vide la liste d'affichage */
void listeAffichagePurge(int32 type);
/* Nombre d'elements dans la liste d'affichage */
int32 listeAffichageElements(int32 type);
/* Si la 3do est NTSC ou PAL1 ou PAL2 */
bool Screen_format();

void setpixel(int x, int y, int RED, int GREEN, int BLUE);
void fill_rectangle(int x, int y, int size_rect, int RED, int GREEN, int BLUE);
void fill_rectangle_norgb(int x, int y, int size_rect, int couleur);

/*
void fill_rectangle(unsigned short x, unsigned short y, unsigned short size_rect, unsigned char R, unsigned char G, unsigned char B);
*/

