// GestionAffichage
#include "GestionAffichage.h"

// Format de l'ecran par defaut (NTSC)
int32 ecranFormat=0;
int32 *pointeurEcranFormat=&ecranFormat;
// Taille de l'image par defaut (NTSC)
int32 ecranLargeur=320;
int32 *pointeurEcranLargeur=&ecranLargeur;
int32 ecranHauteur=240;
int32 *pointeurEcranHauteur=&ecranHauteur;
// Images par secondes par defaut (NTSC)
int32 ecranFrequence=60;
int32 *pointeurEcranFrequence=&ecranFrequence;
// Requete d'entree/sortie sur la memoire video (VRAM)
Item requeteVRAMIO=-1;
// Ecran
ScreenContext *pointeurEcranContexte=NULL;
// Affichage
Item affichage=-1;
// Ecran courant
int32 ecranCourant=0;
// Image de fond
ubyte *pointeurImageFond=NULL;
// Si il y a une gestion des sprites
#if H_SPRITES
// Initialise la liste d'affichage
List *listeAffichageSprites=NULL;
#endif
// Si il y a une gestion des textes
#if H_TEXTES
// Initialise la liste d'affichage
List *listeAffichageTextes=NULL;
#endif

/* Initialise l'affichage */
bool affichageInitialisation(){
	// Retour
	int32 retourLocal=TRUE;	
	// Standard d'affichae utilise
	int32 affichageStandard=0;	
	// Demarre la gestion video de la 3DO
	retourLocal=OpenGraphicsFolio();
	// Si le demarrage a echoue
	if(retourLocal<0){
		// Retourne une erreur
		return FALSE;
	}
	// Cree une requete d'entree/sortie sur la memoire video (VRAM)
	requeteVRAMIO=CreateVRAMIOReq();
	// Si la creation de la requete a echoue
	if(requeteVRAMIO<0){
		// Retourne une erreur
		return FALSE;
	}
	// Alloue de la memoire pour l'affichage d'un ecran
	pointeurEcranContexte=(ScreenContext *)AllocMem(sizeof(ScreenContext), MEMTYPE_ANY);
	//Si l'allocation de memoire n'a pas eu lieu
	if(pointeurEcranContexte==NULL){
		// Retourne une erreur
		return FALSE;
	}
	// Recupere le format de l'ecran			
	affichageStandard=GetDisplayType();
	// Si la recuperation a echoue
	if(affichageStandard<0){
		// Retourne une erreur
		return FALSE;
	}
	// Si l'affichage est en PAL1
	if(affichageStandard==DI_TYPE_PAL1){
		// Format
		*pointeurEcranFormat=1;
		// Taille de l'image
		*pointeurEcranLargeur=320;
		*pointeurEcranHauteur=288;
		// Images par secondes
		*pointeurEcranFrequence=50;	
	// Si l'affichage est en PAL2
	}else if(affichageStandard==DI_TYPE_PAL2){
		// Format
		*pointeurEcranFormat=2;
		// Taille de l'image
		*pointeurEcranLargeur=384;
		*pointeurEcranHauteur=288;
		// Images par secondes
		*pointeurEcranFrequence=50;	
	// L'affichage est en NTSC
	}else{
		// Format
		*pointeurEcranFormat=0;
		// Taille de l'image
		*pointeurEcranLargeur=320;
		*pointeurEcranHauteur=240;
		// Images par secondes
		*pointeurEcranFrequence=60;
	}	
	// Cree l'affichage sur deux ecrans (ou deux zones tampon)
	// Ainsi, pendant que le premier ecran est affiche, le second est genere
	// A chaque fois que la fonction affichageRendu est appelee, l'affichage passe d'un ecran a l'autre
	affichage=CreateBasicDisplay(pointeurEcranContexte, affichageStandard, Ecrans);
	// Si la creation de l'affichage a echoue
	if(affichage<0){
		// Retourne une erreur
		return FALSE;
	}
	// Si il y a une gestion des sprites
	#if H_SPRITES
	// Alloue de la memoire pour la liste d'affichage
	listeAffichageSprites=(List*)AllocMem(sizeof(List), MEMTYPE_ANY);
	// Initialise la liste d'affichage
	InitList(listeAffichageSprites, "ListeAffichageSprites");
	#endif
	// Si il y a une gestion des textes
	#if H_TEXTES
	// Alloue de la memoire pour la liste d'affichage
	listeAffichageTextes=(List*)AllocMem(sizeof(List), MEMTYPE_ANY);
	// Initialise la liste d'affichage
	InitList(listeAffichageTextes, "ListeAffichageTextes");
	#endif
	// Retourne un succes	
	return TRUE;
}

/* Cree le rendu de l'affichage en fonction des multiples elements */
void affichageRendu(void){
	// Pointeur CCB
	CCB	*pointeurCCB=NULL;
	// Cree un tableau de 4 valeurs X/Y definissant les 4 coins d'un quadrilatere servant au placage du sprite sur l'ecran
	Point quadrilatere[4];
	// Si il y a une gestion des sprites
	#if H_SPRITES
	struct Sprite *spriteATraiter;
	#endif
	// Si il y a une gestion des textes
	#if H_TEXTES
	struct Texte *texteATraiter;
	#endif
	// Copie l'image de fond stockee en VRAM et l'applique a l'ecran courant
	CopyVRAMPages(requeteVRAMIO, pointeurEcranContexte->sc_Bitmaps[ecranCourant]->bm_Buffer, pointeurImageFond, pointeurEcranContexte->sc_nFrameBufferPages, 0xFFFFFFFF);
	// Affiche l'image de fond
	// Si il y a une gestion des sprites
	#if H_SPRITES
	// Si la liste d'elements a afficher n'est pas vide
	if(!IsListEmpty(listeAffichageSprites)){
		// Parcourt la liste de la tete a la queue
		ScanList(listeAffichageSprites, spriteATraiter, Sprite){
		// Parcourt la liste de la queue a la tete
		//ScanListB(listeAffichageSprites, spriteATraiter, Sprite){
			// Si le sprite est un clone
			if(spriteATraiter->pointeurClone!=NULL){
				// Recupere le contenu du sprite
				//pointeurCCB=GetAnimCel(spriteATraiter->pointeurClone->pointeurSpriteContenu, 0);
			// Sinon
			}else{
				// Si le sprite est anime automatiquement
				if(spriteATraiter->animationCyclique){
					// Incremente le compteur
					spriteATraiter->animationCyclesCompteur++;
					// Si le nombre de cycles est atteint
					if(spriteATraiter->animationCyclesCompteur>=spriteATraiter->animationCycles){
						// Si c'est le mode par defaut
						if(!spriteATraiter->animationCycliqueMode){
							// Passe a l'image suivante de l'animation
							spriteATraiter->imageIndex++;
							// Si l'animation cyclique depasse la fin de l'intervalle
							if(spriteATraiter->imageIndex>spriteATraiter->animationCycliqueIntervalleFin){
								// Retourne au debut de l'intervalle
								spriteATraiter->imageIndex=spriteATraiter->animationCycliqueIntervalleDebut;
							}
						// Sinon, si c'est le mode inverse
						}else if(spriteATraiter->animationCycliqueMode==1){
							// Passe a l'image precedente de l'animation
							spriteATraiter->imageIndex--;
							// Si l'animation cyclique depasse le debut de l'intervalle
							if(spriteATraiter->imageIndex<spriteATraiter->animationCycliqueIntervalleDebut){
								// Retourne a la fin de l'animation cyclique
								spriteATraiter->imageIndex=spriteATraiter->animationCycliqueIntervalleFin;
							}
						// Sinon, c'est un aller
						}else if(spriteATraiter->animationCycliqueMode==2){
							// Passe a l'image suivante de l'animation
							spriteATraiter->imageIndex++;
							// Si l'animation cyclique depasse la fin de l'intervalle
							if(spriteATraiter->imageIndex>=spriteATraiter->animationCycliqueIntervalleFin){
								// Passe en mode retour
								spriteATraiter->animationCycliqueMode=3;
							}
						// Sinon, c'est un retour
						}else if(spriteATraiter->animationCycliqueMode==3){
							// Passe a l'image precedente de l'animation
							spriteATraiter->imageIndex--;
							// Si l'animation cyclique depasse le debut de l'intervalle
							if(spriteATraiter->imageIndex<=spriteATraiter->animationCycliqueIntervalleDebut){
								// Passe en mode aller
								spriteATraiter->animationCycliqueMode=2;
							}
						}
						// Initialise le nombre de cycles ecoules
						spriteATraiter->animationCyclesCompteur=0;
					}
				}
				// Modifie l'image affichee du sprite
				spriteATraiter->pointeurSpriteContenu->cur_Frame=Convert32_F16(spriteATraiter->imageIndex);
				// Recupere le contenu du sprite
				pointeurCCB=GetAnimCel(spriteATraiter->pointeurSpriteContenu, 0);
			// Defini les coordonnees du placage en indiquant la position des 4 coins (de 0 à 3)
			quadrilatere[0].pt_X=spriteATraiter->coin1.pt_X;
			quadrilatere[0].pt_Y=spriteATraiter->coin1.pt_Y;
			quadrilatere[1].pt_X=spriteATraiter->coin2.pt_X;
			quadrilatere[1].pt_Y=spriteATraiter->coin2.pt_Y;
			quadrilatere[2].pt_X=spriteATraiter->coin3.pt_X;
			quadrilatere[2].pt_Y=spriteATraiter->coin3.pt_Y;
			quadrilatere[3].pt_X=spriteATraiter->coin4.pt_X;
			quadrilatere[3].pt_Y=spriteATraiter->coin4.pt_Y;
			// Applique le sprite
			MapCel(pointeurCCB, quadrilatere);
			// Aucune cellule liee a la precedente
			LAST_CEL(pointeurCCB);
			// Dessine le sprite sur l'ecran
			DrawCels(pointeurEcranContexte->sc_BitmapItems[ecranCourant], pointeurCCB);	
			}
		}
	}
	#endif
	// Si il y a une gestion des textes
	#if H_TEXTES
	// Si la liste d'elements a afficher n'est pas vide
	if(!IsListEmpty(listeAffichageTextes)){
		// Parcourt la liste de la tete a la queue
		ScanList(listeAffichageTextes, texteATraiter, Texte){
		// Parcourt la liste de la queue a la tete
		//ScanListB(listeAffichageTextes, texteATraiter, Texte){
			pointeurCCB=texteATraiter->tCel->tc_CCB;
			// Defini les coordonnees du sprite
			quadrilatere[0].pt_X=texteATraiter->coin1.pt_X;
			quadrilatere[0].pt_Y=texteATraiter->coin1.pt_Y;
			quadrilatere[1].pt_X=texteATraiter->coin2.pt_X;
			quadrilatere[1].pt_Y=texteATraiter->coin2.pt_Y;
			quadrilatere[2].pt_X=texteATraiter->coin3.pt_X;
			quadrilatere[2].pt_Y=texteATraiter->coin3.pt_Y;
			quadrilatere[3].pt_X=texteATraiter->coin4.pt_X;
			quadrilatere[3].pt_Y=texteATraiter->coin4.pt_Y;
			// Applique le sprite
			MapCel(pointeurCCB, quadrilatere);
			// Aucune cellule liee a la precedente
			LAST_CEL(pointeurCCB);
			// Dessine le sprite sur l'ecran
			DrawCels(pointeurEcranContexte->sc_BitmapItems[ecranCourant], pointeurCCB);
		}
	}
	#endif
	// Met a jour l'affichage
	affichageMiseAJour();
}

/* Cree un ecran neutre */
void affichageCouleurUnie(int32 couleur){
	// Contexte graphique
	GrafCon contexteGraphique;
	// Rectangle
	Rect rectangle;
	// Cree la couleur de fond qui est determinee avec une valeur numerique obtenue a partir de MakeRGB15(R,G,B)
	// R, G et B vont de 0 a 255
	// Le noir pur est obtenu avec MakeRGB15(0,0,0)
	SetFGPen(&contexteGraphique, couleur);
	// Defini les coordonnees du rectangle selon le format de l'ecran
	rectangle.rect_XLeft=0;
	rectangle.rect_YTop=0;
	rectangle.rect_XRight=*pointeurEcranLargeur;
	rectangle.rect_YBottom=*pointeurEcranHauteur;
	// Copie l'image de fond stockee en VRAM et l'applique a l'ecran courant
	CopyVRAMPages(requeteVRAMIO, pointeurEcranContexte->sc_Bitmaps[ecranCourant]->bm_Buffer, pointeurImageFond, pointeurEcranContexte->sc_nFrameBufferPages, 0xFFFFFFFF);
	// Applique le rectangle sur l'image de l'ecran courant
	FillRect(pointeurEcranContexte->sc_BitmapItems[ecranCourant], &contexteGraphique, &rectangle);
	// Met a jour l'affichage
	affichageMiseAJour();
}

/* Met a jour l'affichage */
void affichageMiseAJour(void){
	// Affiche l'ecran courant
	// Le second parametre est utilise pour un affichage stereoscopique ou entrelace
	// Dans le cas contraire, le premier et le second parametre doivent etre identiques ou le second a 0
	DisplayScreen(pointeurEcranContexte->sc_Screens[ecranCourant], 0);
	// Passe a l'écran suivant
	ecranCourant++;
	// Si l'écran courant est superieur ou egal au nombre d'écrans utilises
	if(ecranCourant>=Ecrans){
		// Passe au premier ecran
		ecranCourant=0;
	}
}

/* Supprime l'affichage */
void affichageSuppression(int32 fondu){
	// Si il y a un ecran et qu'un fondu est demande
	if((pointeurEcranContexte!=NULL)&&fondu){
		// Fais un fondu vers le noir sur un nombre de nuances determinees
		// fondu : nombre d'images utilisees
		// Note : A la suite d'un fondu, il n'est pas possible d'afficher d'autres elements a l'ecran
		FadeToBlack(pointeurEcranContexte, fondu);
	}
	// Force l'affichage d'un ecran neutre
	// Si il n'y a pas eu de fondu, affiche la couleur en parametre sur tout l'ecran
	// Sinon, la couleur n'est pas affichee mais cela force un rafraichissement d'image pour la derniere image du fondu
	affichageCouleurUnie(MakeRGB15(0,0,0));
	// Si une image de fond existe
	if(pointeurImageFond!=NULL){
		// Libere la memoire utilisee par l'image de fond en utilisant la taille utilisee pour son stockage en memoire
		FreeMem(pointeurImageFond, pointeurEcranContexte->sc_nFrameByteCount);
		// Indique qu'il n'y a plus d'image de fond
		pointeurImageFond=NULL;
	}
	// Si il y a un affichage
	if(affichage>=0){
		// Supprime l'affichage
		DeleteBasicDisplay(pointeurEcranContexte);
		// Indique que l'affichage n'existe plus
		affichage=-1;
	}
	// Si il y a un ecran
	if(pointeurEcranContexte!=NULL){
		// Libere la memoire utilisee pour l'ecran
		FreeMem(pointeurEcranContexte, sizeof(ScreenContext));
		// Indique que l'ecran n'existe plus
		pointeurEcranContexte=NULL;
	}		
	// Si il y a une requete d'entree/sortie sur la memoire video (VRAM)
	if(requeteVRAMIO>=0){
		// Supprime la requete
		DeleteVRAMIOReq(requeteVRAMIO);
		// Indique que la requete n'existe plus
		requeteVRAMIO=-1;
	}
	// Ferme la gestion video de la 3DO
	CloseGraphicsFolio();
}

/* Charge une image de fond */
bool imageFondChargement(char *pointeurFichier){
	// Initialise le chemin du fichier
	char fichierChemin[128];
	// Si l'affichage est en PAL1
	if(*pointeurEcranFormat==1){
		// Utilise le dossier correspondant (DossierFondsPAL1 : voir GestionAffichage.h)
		strcpy(fichierChemin, DossierFondsPAL1);
	// Si l'affichage est en PAL2
	}else if(*pointeurEcranFormat==2){
		// Utilise le dossier correspondant (DossierFondsPAL2 : voir GestionAffichage.h)
		strcpy(fichierChemin, DossierFondsPAL2);
	// L'affichage est en NTSC
	}else{
		// Utilise le dossier correspondant (DossierFondsNTSC : voir GestionAffichage.h)
		strcpy(fichierChemin, DossierFondsNTSC);
	}	
	// Concatene le chemin du fichier et le nom du fichier
	strcat(fichierChemin, pointeurFichier);
	// Alloue de la memoire pour l'image de fond et stocke l'adresse dans le pointeur dedie
	pointeurImageFond=(ubyte *)AllocMem((int)(pointeurEcranContexte->sc_nFrameByteCount), MEMTYPE_STARTPAGE | MEMTYPE_VRAM | MEMTYPE_CEL);
	// Si l'allocation ne s'est pas deroulee correctement
	if(pointeurImageFond==NULL){
		// Retourne une erreur
		return FALSE;
	}
	// Applique aux pages de VRAM le contenu de la memoire allouee au pointeur
	SetVRAMPages(requeteVRAMIO, pointeurImageFond, 0, pointeurEcranContexte->sc_nFrameBufferPages, -1);
	// Charge l'image dans le contenu de la memoire allouee au pointeur
	if(!LoadImage(fichierChemin, pointeurImageFond, (VdlChunk **)NULL, pointeurEcranContexte)){
		// En cas de probleme, retourne une erreur
		return FALSE;
	}
	// Retourne un succes
	return TRUE;
}


/* Ajoute un element a la liste d'affichage */
void listeAffichageAjout(Node *pointeurElement, bool sens, int32 type, int32 index){
	// Si c'est un sprite
	if(type==1){
		// Si l'ajout doit etre en fin de liste
		if(sens){
			// Insert l'element dans la liste d'affichage
			InsertNodeFromTail(listeAffichageSprites, (Node *) pointeurElement);
		// Sinon
		}else{
			// Insert l'element dans la liste d'affichage
			InsertNodeFromHead(listeAffichageSprites, (Node *) pointeurElement);	
		}
	// Sinon, si c'est un texte
	}else if(type==2){
		// Si l'ajout doit etre en fin de liste
		if(sens){
			// Insert l'element dans la liste d'affichage
			InsertNodeFromTail(listeAffichageTextes, (Node *) pointeurElement);
		// Sinon
		}else{
			// Insert l'element dans la liste d'affichage
			InsertNodeFromHead(listeAffichageTextes, (Node *) pointeurElement);	
		}
	}
	// Modifie la priorite d'un element
	listeAffichagePriorite((Node *) pointeurElement, index);
}

/* Modifie la priorite d'un element dans la liste d'affichage */
void listeAffichagePriorite(Node *pointeurElement, int32 index){
	// Modifie la priorite de l'element 
	// La valeur peut aller de 0 a 255
	// Lorsqu'une valeur est modifiee, l'integralite de la liste est ordonnee en fonction du changement
	// Si ScanList (voir fonction : affichageRendu) est utilise, plus la valeur est faible, plus l'element est affiche en priorite, donc susceptible d'etre recouvert par d'autres
	// Si ScanListB (voir fonction : affichageRendu) est utilise, plus la valeur est haute, plus l'element est affiche en priorite, donc susceptible d'etre recouvert par d'autres
	SetNodePri((Node *) pointeurElement, index);
}

/* Supprime un element a la liste d'affichage */
void listeAffichageSuppression(Node *pointeurElement){
	// Supprime l'element
	RemNode((Node *) pointeurElement);
}

/* Vide la liste d'affichage */
void listeAffichagePurge(int32 type){
	// Noeud a traiter dans la liste d'affichage
	struct Node *noeudATraiter=NULL;
	// Si c'est la liste des sprites
	if(type==1){
		// Si la liste d'elements a afficher n'est pas vide
		if(!IsListEmpty(listeAffichageSprites)){
			// Parcourt la liste de la tete a la queue
			ScanList(listeAffichageSprites, noeudATraiter, Node){
				// Supprime l'element
				RemNode(noeudATraiter);
			}
		}
	// Sinon, c'est la liste des textes
	}else if(type==2){
		// Si la liste d'elements a afficher n'est pas vide
		if(!IsListEmpty(listeAffichageTextes)){
			// Parcourt la liste de la tete a la queue
			ScanList(listeAffichageTextes, noeudATraiter, Node){
				// Supprime l'element
				RemNode(noeudATraiter);
			}
		}
	}
}

/* Nombre d'elements dans la liste d'affichage */
int32 listeAffichageElements(int32 type){
	// Si c'est un sprite
	if(type==1){
		// Retourne le nombre d'elements
		return GetNodeCount(listeAffichageSprites);
	// Sinon, si c'est un texte
	}else if(type==2){
		// Retourne le nombre d'elements
		return GetNodeCount(listeAffichageTextes);
	}
	// Le cas echeant retourne 0
	return 0;
	
}

void setpixel(int x, int y, int RED, int GREEN, int BLUE)
{
	GrafCon contexteGraphique;
	int32 couleur;
	
	couleur = MakeRGB15(RED,GREEN,BLUE);
	SetFGPen(&contexteGraphique, couleur);
	
	WritePixel(pointeurEcranContexte->sc_BitmapItems[ecranCourant], &contexteGraphique, x, y);
}


void fill_rectangle(int x, int y, int size_rect, int RED, int GREEN, int BLUE)
{	
	GrafCon contexteGraphique;
	Rect rectangle;
	int32 couleur;
	
	couleur = MakeRGB15(RED,GREEN,BLUE);
	SetFGPen(&contexteGraphique, couleur);

	rectangle.rect_XLeft=x;
	rectangle.rect_YTop=y;
	rectangle.rect_XRight=x+size_rect-1;
	rectangle.rect_YBottom=y+size_rect-1;
	
	FillRect(pointeurEcranContexte->sc_BitmapItems[ecranCourant], &contexteGraphique, &rectangle);
}

bool Screen_format()
{
	if (*pointeurEcranFormat==1)
	{
		return	1;
	}
	else if (*pointeurEcranFormat==2)
	{
		return	2;
	}
	else
	{
		return	0;
	}	
	
	return true;
}
