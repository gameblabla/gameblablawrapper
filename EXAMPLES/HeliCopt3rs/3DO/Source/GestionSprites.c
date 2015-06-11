// Gestion des sprites
#include "GestionSprites.h"

// Tableaux de cosinus et sinus declares en avance pour economiser du temps de calcul
const int32 tableauCos[360]={256.0, 255.96, 255.84, 255.64, 255.37, 255.02, 254.59, 254.09, 253.50, 252.84, 252.11, 251.29, 250.40, 249.43, 248.39, 247.27, 246.08, 244.81, 243.47, 242.05, 240.56, 238.99, 237.35, 235.64, 233.86, 232.01, 230.09, 228.09, 226.03, 223.90, 221.70, 219.43, 217.10, 214.69, 212.23, 209.70, 207.10, 204.45, 201.73, 198.94, 196.10, 193.20, 190.24, 187.22, 184.15, 181.01, 177.83, 174.59, 171.29, 167.95, 164.55, 161.10, 157.60, 154.06, 150.47, 146.83, 143.15, 139.42, 135.65, 131.84, 127.99, 124.11, 120.18, 116.22, 112.22, 108.19, 104.12, 100.02, 95.899, 91.742, 87.557, 83.345, 79.108, 74.847, 70.563, 66.257, 61.932, 57.587, 53.225, 48.847, 44.453, 40.047, 35.628, 31.198, 26.759, 22.311, 17.857, 13.398, 8.9342, 4.4678, 1.5675, -4.4678, -8.9342, -13.397, -17.857, -22.311, -26.759, -31.198, -35.628, -40.047, -44.453, -48.847, -53.225, -57.587, -61.932, -66.257, -70.563, -74.847, -79.108, -83.345, -87.557, -91.742, -95.899, -100.02, -104.12, -108.19, -112.22, -116.22, -120.18, -124.11, -128.00, -131.84, -135.65, -139.42, -143.15, -146.83, -150.47, -154.06, -157.60, -161.10, -164.55, -167.95, -171.29, -174.59, -177.83, -181.01, -184.15, -187.22, -190.24, -193.20, -196.10, -198.94, -201.73, -204.45, -207.10, -209.70, -212.23, -214.69, -217.10, -219.43, -221.70, -223.90, -226.03, -228.09, -230.09, -232.01, -233.86, -235.64, -237.35, -238.99, -240.56, -242.05, -243.47, -244.81, -246.08, -247.27, -248.39, -249.43, -250.40, -251.29, -252.11, -252.84, -253.50, -254.09, -254.59, -255.02, -255.37, -255.64, -255.84, -255.96, -256.0, -255.96, -255.84, -255.64, -255.37, -255.02, -254.59, -254.09, -253.50, -252.84, -252.11, -251.29, -250.40, -249.43, -248.39, -247.27, -246.08, -244.81, -243.47, -242.05, -240.56, -238.99, -237.35, -235.64, -233.86, -232.01, -230.09, -228.09, -226.03, -223.90, -221.70, -219.43, -217.10, -214.69, -212.23, -209.70, -207.10, -204.45, -201.73, -198.94, -196.10, -193.20, -190.24, -187.22, -184.15, -181.01, -177.83, -174.59, -171.29, -167.95, -164.55, -161.10, -157.60, -154.06, -150.47, -146.83, -143.15, -139.42, -135.65, -131.84, -127.99, -124.11, -120.18, -116.22, -112.22, -108.19, -104.12, -100.02, -95.899, -91.742, -87.557, -83.345, -79.108, -74.847, -70.563, -66.257, -61.931, -57.587, -53.225, -48.847, -44.453, -40.047, -35.628, -31.198, -26.759, -22.311, -17.857, -13.397, -8.9342, -4.4677, -4.7026, 4.4677, 8.9342, 13.397, 17.857, 22.311, 26.759, 31.198, 35.628, 40.047, 44.453, 48.847, 53.225, 57.587, 61.931, 66.257, 70.563, 74.847, 79.108, 83.345, 87.557, 91.742, 95.899, 100.02, 104.12, 108.19, 112.22, 116.22, 120.18, 124.11, 127.99, 131.84, 135.65, 139.42, 143.15, 146.83, 150.47, 154.06, 157.60, 161.10, 164.55, 167.95, 171.29, 174.59, 177.83, 181.01, 184.15, 187.22, 190.24, 193.20, 196.10, 198.94, 201.73, 204.45, 207.10, 209.70, 212.23, 214.69, 217.10, 219.43, 221.70, 223.90, 226.03, 228.09, 230.09, 232.01, 233.86, 235.64, 237.35, 238.99, 240.56, 242.05, 243.47, 244.81, 246.08, 247.27, 248.39, 249.43, 250.40, 251.29, 252.11, 252.84, 253.50, 254.09, 254.59, 255.02, 255.37, 255.64, 255.84, 255.96};
const int32 tableauSin[360]={0.0, 4.4678, 8.9342, 13.398, 17.857, 22.311, 26.759, 31.198, 35.628, 40.047, 44.453, 48.847, 53.225, 57.587, 61.932, 66.257, 70.563, 74.847, 79.108, 83.345, 87.557, 91.742, 95.899, 100.02, 104.12, 108.19, 112.22, 116.22, 120.18, 124.11, 128.00, 131.84, 135.65, 139.42, 143.15, 146.83, 150.47, 154.06, 157.60, 161.10, 164.55, 167.95, 171.29, 174.59, 177.83, 181.01, 184.15, 187.22, 190.24, 193.20, 196.10, 198.94, 201.73, 204.45, 207.10, 209.70, 212.23, 214.69, 217.10, 219.43, 221.70, 223.90, 226.03, 228.09, 230.09, 232.01, 233.86, 235.64, 237.35, 238.99, 240.56, 242.05, 243.47, 244.81, 246.08, 247.27, 248.39, 249.43, 250.40, 251.29, 252.11, 252.84, 253.50, 254.09, 254.59, 255.02, 255.37, 255.64, 255.84, 255.96, 256.0, 255.96, 255.84, 255.64, 255.37, 255.02, 254.59, 254.09, 253.50, 252.84, 252.11, 251.29, 250.40, 249.43, 248.39, 247.27, 246.08, 244.81, 243.47, 242.05, 240.56, 238.99, 237.35, 235.64, 233.86, 232.01, 230.09, 228.09, 226.03, 223.90, 221.70, 219.43, 217.10, 214.69, 212.23, 209.70, 207.10, 204.45, 201.73, 198.94, 196.10, 193.20, 190.24, 187.22, 184.15, 181.01, 177.83, 174.59, 171.29, 167.95, 164.55, 161.10, 157.60, 154.06, 150.47, 146.83, 143.15, 139.42, 135.65, 131.84, 128.00, 124.11, 120.18, 116.22, 112.22, 108.19, 104.12, 100.02, 95.899, 91.742, 87.557, 83.345, 79.108, 74.847, 70.563, 66.257, 61.932, 57.587, 53.225, 48.847, 44.453, 40.047, 35.628, 31.198, 26.759, 22.311, 17.857, 13.397, 8.9342, 4.4678, 3.1350, -4.4677, -8.9342, -13.397, -17.857, -22.311, -26.759, -31.198, -35.628, -40.047, -44.453, -48.847, -53.225, -57.587, -61.931, -66.257, -70.563, -74.847, -79.108, -83.345, -87.557, -91.742, -95.899, -100.02, -104.12, -108.19, -112.22, -116.22, -120.18, -124.11, -127.99, -131.84, -135.65, -139.42, -143.15, -146.83, -150.47, -154.06, -157.60, -161.10, -164.55, -167.95, -171.29, -174.59, -177.83, -181.01, -184.15, -187.22, -190.24, -193.20, -196.10, -198.94, -201.73, -204.45, -207.10, -209.70, -212.23, -214.69, -217.10, -219.43, -221.70, -223.90, -226.03, -228.09, -230.09, -232.01, -233.86, -235.64, -237.35, -238.99, -240.56, -242.05, -243.47, -244.81, -246.08, -247.27, -248.39, -249.43, -250.40, -251.29, -252.11, -252.84, -253.50, -254.09, -254.59, -255.02, -255.37, -255.64, -255.84, -255.96, -256.0, -255.96, -255.84, -255.64, -255.37, -255.02, -254.59, -254.09, -253.50, -252.84, -252.11, -251.29, -250.40, -249.43, -248.39, -247.27, -246.08, -244.81, -243.47, -242.05, -240.56, -238.99, -237.35, -235.64, -233.86, -232.01, -230.09, -228.09, -226.03, -223.90, -221.70, -219.43, -217.10, -214.69, -212.23, -209.70, -207.10, -204.45, -201.73, -198.94, -196.10, -193.20, -190.24, -187.22, -184.15, -181.01, -177.83, -174.59, -171.29, -167.95, -164.55, -161.10, -157.60, -154.06, -150.47, -146.83, -143.15, -139.42, -135.65, -131.84, -127.99, -124.11, -120.18, -116.22, -112.22, -108.19, -104.12, -100.02, -95.899, -91.742, -87.557, -83.345, -79.108, -74.847, -70.563, -66.257, -61.931, -57.587, -53.225, -48.847, -44.453, -40.047, -35.628, -31.198, -26.759, -22.311, -17.857, -13.397, -8.9342, -4.4677};

CCB *SetCommonCCB(ANIM* pointeurSpriteContenu){
	// Initialise l'index
	int32 index=0;
	CCB* pointeurCCB=pointeurSpriteContenu->pentries[0].af_CCB;
	// Pour chaque frame du sprite
	for(index=0;index<pointeurSpriteContenu->num_Frames;index++){
	
		pointeurSpriteContenu->pentries[index].af_CCB=pointeurCCB;
	}

	return	pointeurCCB;
}

/* Charge un sprite et effectue des configurations de base */
struct Sprite *spriteChargement(char *pointeurFichier, int32 positionX, int32 positionY, int32 centreX, int32 centreY, int32 imageIndex, bool animationCyclique, int32 animationCycliqueMode, int32 animationCycliqueIntervalleDebut, int32 animationCycliqueIntervalleFin, int32 animationCycles){
	// Initialise le chemin avec le dossier de base (voir GestionSprites.h)
	char fichierChemin[128]=DossierSprites;	
	// Cree un sprite
	struct Sprite *pointeurSpriteTemporaire=NULL;
	// Alloue de la memoire pour le sprite
	pointeurSpriteTemporaire=(Sprite*)AllocMem(sizeof(Sprite), MEMTYPE_ANY);
	// Si l'allocation a echoue
	if(pointeurSpriteTemporaire==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Concatene le nom du fichier avec le chemin
	strcat(fichierChemin, pointeurFichier);
	// Charge le contenu du sprite
	pointeurSpriteTemporaire->pointeurSpriteContenu=LoadAnim(fichierChemin, MEMTYPE_CEL);
	// Si le contenu n'a pas ete charge
	if(pointeurSpriteTemporaire->pointeurSpriteContenu==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Cree un CCB
	pointeurSpriteTemporaire->pointeurCCB=SetCommonCCB(pointeurSpriteTemporaire->pointeurSpriteContenu);
	// Indique la largeur du sprite
	pointeurSpriteTemporaire->largeurOriginale=pointeurSpriteTemporaire->pointeurCCB->ccb_Width;
	pointeurSpriteTemporaire->hauteurOriginale=pointeurSpriteTemporaire->pointeurCCB->ccb_Height;
	// Duplique la taille du sprite
	pointeurSpriteTemporaire->largeur=pointeurSpriteTemporaire->largeurOriginale;
	pointeurSpriteTemporaire->hauteur=pointeurSpriteTemporaire->hauteurOriginale;
	// Indique les coins du sprite
	pointeurSpriteTemporaire->coinOriginal1.pt_X=0;
	pointeurSpriteTemporaire->coinOriginal1.pt_Y=0;
	pointeurSpriteTemporaire->coinOriginal2.pt_X=pointeurSpriteTemporaire->largeurOriginale;
	pointeurSpriteTemporaire->coinOriginal2.pt_Y=0;
	pointeurSpriteTemporaire->coinOriginal3.pt_X=pointeurSpriteTemporaire->largeurOriginale;
	pointeurSpriteTemporaire->coinOriginal3.pt_Y=pointeurSpriteTemporaire->hauteurOriginale;
	pointeurSpriteTemporaire->coinOriginal4.pt_X=0;
	pointeurSpriteTemporaire->coinOriginal4.pt_Y=pointeurSpriteTemporaire->hauteurOriginale;
	// Initialise le centre du sprite
	spriteModificationCentre(pointeurSpriteTemporaire, pointeurSpriteTemporaire->largeurOriginale/2, pointeurSpriteTemporaire->hauteurOriginale/2);
	// Initialise la frame affichee du sprite
	spriteConfigurationFrame(pointeurSpriteTemporaire, imageIndex);
	// Initialise le placage du sprite
	spriteModificationPlacage(pointeurSpriteTemporaire, positionX, positionY, (positionX+pointeurSpriteTemporaire->largeurOriginale), positionY, (positionX+pointeurSpriteTemporaire->largeurOriginale), (positionY+pointeurSpriteTemporaire->hauteurOriginale), positionX, (positionY+pointeurSpriteTemporaire->hauteurOriginale));
	// Configure l'animation cyclique du sprite
	spriteConfigurationAnimationCyclique(pointeurSpriteTemporaire, animationCyclique, animationCycliqueMode, animationCycliqueIntervalleDebut, animationCycliqueIntervalleFin, animationCycles);
	// Indique ce n'est pas un clone
	pointeurSpriteTemporaire->pointeurClone=NULL;
	// Retourne le sprite
	return pointeurSpriteTemporaire;
}

/* Clone un sprite dans un autre */
struct Sprite *spriteClone(struct Sprite *pointeurSprite){
	// Cree un sprite
	struct Sprite *pointeurSpriteTemporaire=NULL;
	// Alloue de la memoire pour le sprite
	pointeurSpriteTemporaire=(Sprite*)AllocMem(sizeof(Sprite), MEMTYPE_ANY);
	// Si l'allocation a echoue
	if(pointeurSpriteTemporaire==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Stocke l'adresse du clone
	pointeurSpriteTemporaire->pointeurClone=pointeurSprite;
	// Si le sprite n'a pas ete clone
	if(pointeurSpriteTemporaire->pointeurClone==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Retourne le sprite
	return pointeurSpriteTemporaire;
}

/* Copie un sprite dans un autre */
struct Sprite *spriteCopie(struct Sprite *pointeurSprite){
	// Cree un sprite
	struct Sprite *pointeurSpriteTemporaire=NULL;
	// Alloue de la memoire pour le sprite
	pointeurSpriteTemporaire=(Sprite*)AllocMem(sizeof(Sprite), MEMTYPE_ANY);
	// Si l'allocation a echoue
	if(pointeurSpriteTemporaire==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Copie le contenu du sprite
	pointeurSpriteTemporaire->pointeurSpriteContenu=pointeurSprite->pointeurSpriteContenu;
	// Si le contenu n'a pas ete charge
	if(pointeurSpriteTemporaire->pointeurSpriteContenu==NULL){
		// Retourne une erreur
		return NULL;
	}
	// Cree un CCB
	pointeurSpriteTemporaire->pointeurCCB=SetCommonCCB(pointeurSpriteTemporaire->pointeurSpriteContenu);
	// Indique la taille du sprite
	pointeurSpriteTemporaire->largeurOriginale=pointeurSprite->largeurOriginale;
	pointeurSpriteTemporaire->hauteurOriginale=pointeurSprite->hauteurOriginale;
	// Duplique la taille du sprite
	pointeurSpriteTemporaire->largeur=pointeurSprite->largeur;
	pointeurSpriteTemporaire->hauteur=pointeurSprite->hauteur;
	// Initialise le centre du sprite
	spriteModificationCentre(pointeurSpriteTemporaire, pointeurSprite->centre.pt_X, pointeurSprite->centre.pt_Y);
	// Initialise la frame affichee du sprite
	spriteConfigurationFrame(pointeurSpriteTemporaire, pointeurSprite->imageIndex);
	// Initialise le placage du sprite
	spriteModificationPlacage(pointeurSpriteTemporaire, pointeurSprite->coin1.pt_X, pointeurSprite->coin1.pt_Y, pointeurSprite->coin2.pt_X, pointeurSprite->coin2.pt_Y, pointeurSprite->coin3.pt_X, pointeurSprite->coin3.pt_Y, pointeurSprite->coin4.pt_X, pointeurSprite->coin4.pt_Y);
	// Configure l'animation cyclique du sprite
	spriteConfigurationAnimationCyclique(pointeurSpriteTemporaire, pointeurSprite->animationCyclique, pointeurSprite->animationCycliqueMode, pointeurSprite->animationCycliqueIntervalleDebut, pointeurSprite->animationCycliqueIntervalleFin, pointeurSprite->animationCycles);
	// Indique ce n'est pas un clone
	pointeurSpriteTemporaire->pointeurClone=NULL;
	// Retourne le sprite
	return pointeurSpriteTemporaire;
}

/* Modifie le fichier du sprite */
void spriteModificationFichier(struct Sprite *pointeurSprite, char *pointeurFichier){
	// Initialise le chemin avec le dossier de base (voir GestionAffichage.h)
	char fichierChemin[128]=DossierSprites;
	// Concatene le nom du fichier avec le chemin
	strcat(fichierChemin, pointeurFichier);
	// Charge le contenu du sprite
	pointeurSprite->pointeurSpriteContenu=LoadAnim(fichierChemin, MEMTYPE_CEL);
	// Si le contenu n'a pas ete charge
	if(pointeurSprite->pointeurSpriteContenu==NULL){
		// Retourne une erreur
		//return FALSE;
	}
	// Cree un CCB
	pointeurSprite->pointeurCCB=SetCommonCCB(pointeurSprite->pointeurSpriteContenu);

}

/* Modifie le placage du sprite en fonction des valeurs X/Y de chacun des coins */
void spriteModificationPlacage(struct Sprite *pointeurSprite, int32 coin1X, int32 coin1Y, int32 coin2X, int32 coin2Y, int32 coin3X, int32 coin3Y, int32 coin4X, int32 coin4Y){
	// Modifie le placage du sprite en partant du coin en haut a gauche et dans le sens des aiguilles d'une montre
	// Permet d'etirer le sprite pour simuler une pespective
	pointeurSprite->coin1.pt_X=coin1X;
	pointeurSprite->coin1.pt_Y=coin1Y;
	pointeurSprite->coin2.pt_X=coin2X;
	pointeurSprite->coin2.pt_Y=coin2Y;
	pointeurSprite->coin3.pt_X=coin3X;
	pointeurSprite->coin3.pt_Y=coin3Y;
	pointeurSprite->coin4.pt_X=coin4X;
	pointeurSprite->coin4.pt_Y=coin4Y;
}

/* Modifie la position du sprite */
void spriteModificationPosition(struct Sprite *pointeurSprite, int32 valeurX, int32 valeurY, bool relative){
	// Si la modification est relative
	if(relative){
		// Modifie la position du sprite en affectant toutes les valeurs X/Y
		// Pour deplacer le sprite de 30 pixels vers la droite mettre valeurX a 30
		// Par exemple : spriteModificationPosition(pointeurSprite, 120, 120, FALSE);
		// Puis : spriteModificationPosition(pointeurSprite, 30, 0, TRUE);
		pointeurSprite->coin1.pt_X=pointeurSprite->coin1.pt_X+valeurX;
		pointeurSprite->coin1.pt_Y=pointeurSprite->coin1.pt_Y+valeurY;
		pointeurSprite->coin2.pt_X=pointeurSprite->coin2.pt_X+valeurX;
		pointeurSprite->coin2.pt_Y=pointeurSprite->coin2.pt_Y+valeurY;
		pointeurSprite->coin3.pt_X=pointeurSprite->coin3.pt_X+valeurX;
		pointeurSprite->coin3.pt_Y=pointeurSprite->coin3.pt_Y+valeurY;
		pointeurSprite->coin4.pt_X=pointeurSprite->coin4.pt_X+valeurX;
		pointeurSprite->coin4.pt_Y=pointeurSprite->coin4.pt_Y+valeurY;
	// Sinon la modification est absolue
	}else{
		// Modifie la position du sprite en affectant uniquement les valeurs X/Y du point d'origine
		// Pour deplacer le sprite de 30 pixels vers la droite recuperer prealablement la valeur de coin1.pt_X et lui ajouter 30
		// Par exemple : spriteModificationPosition(pointeurSprite, 120, 120, FALSE);
		// Puis : spriteModificationPosition(pointeurSprite, 150, 120, FALSE);
		pointeurSprite->coin1.pt_X=valeurX;
		pointeurSprite->coin1.pt_Y=valeurY;
		pointeurSprite->coin2.pt_X=valeurX+pointeurSprite->largeur;
		pointeurSprite->coin2.pt_Y=valeurY;
		pointeurSprite->coin3.pt_X=valeurX+pointeurSprite->largeur;
		pointeurSprite->coin3.pt_Y=valeurY+pointeurSprite->hauteur;
		pointeurSprite->coin4.pt_X=valeurX;
		pointeurSprite->coin4.pt_Y=valeurY+pointeurSprite->hauteur;
	}
}

/* Modifie le centre du sprite */
void spriteModificationCentre(struct Sprite *pointeurSprite, int32 centreX, int32 centreY){
	// Configure le centre du sprite a partir du point coin1
	pointeurSprite->centre.pt_X=centreX;
	pointeurSprite->centre.pt_Y=centreY;
}

/* Modifie la taille du sprite en fonction de la position de son origine */
void spriteModificationTaille(struct Sprite *pointeurSprite, float valeurX, float valeurY, bool relative){
	// Longueur et largeur divise par deux pour la modification centre
	int32 modificationLongueur=0;
	int32 modificationHauteur=0;
	// Decalage X/Y en fonction du centre
	int32 decalageCentreX=0;
	int32 decalageCentreY=0;
	// Si la modification est relative
	if(relative){
		// La modification de taille est realisee selon un pourcentage
		// 200 donne une taille deux fois plus grande
		// 50 donne une taille deux fois plus petite
		// Indiquer une valeur pour X et Y identique effectue une modification qui respecte le ratio
		pointeurSprite->largeur=(pointeurSprite->largeurOriginale*valeurX);
		pointeurSprite->hauteur=(pointeurSprite->hauteurOriginale*valeurY);
	// Sinon la modification est absolue
	}else{
		// La modification de taille est realisee selon une valeur en pixels
		// A moins que le sprite soit un carre, indiquer une valeur pour X et Y identique n'effectue pas une modification qui respecte le ratio
		// Pour respecter le ratio, il est necessaire de faire un calcul prealable pour modifier une valeur par rapport a l'autre
		pointeurSprite->largeur=pointeurSprite->largeurOriginale+valeurX;
		pointeurSprite->hauteur=pointeurSprite->hauteurOriginale+valeurY;
		decalageCentreX=0;
		decalageCentreY=0;
	}
	// Recupere la modification en fonction de la taille originale
	modificationLongueur=pointeurSprite->largeur-pointeurSprite->largeurOriginale;
	modificationHauteur=pointeurSprite->largeur-pointeurSprite->largeurOriginale;
	// Affecte la position des coins
	pointeurSprite->coin1.pt_X=pointeurSprite->coin1.pt_X;
	pointeurSprite->coin1.pt_Y=pointeurSprite->coin1.pt_Y;
	pointeurSprite->coin2.pt_X=pointeurSprite->coin2.pt_X+pointeurSprite->largeur;
	pointeurSprite->coin2.pt_Y=pointeurSprite->coin2.pt_Y;
	pointeurSprite->coin3.pt_X=pointeurSprite->coin3.pt_X+pointeurSprite->largeur;
	pointeurSprite->coin3.pt_Y=pointeurSprite->coin3.pt_Y+pointeurSprite->hauteur;
	pointeurSprite->coin4.pt_X=pointeurSprite->coin4.pt_X;
	pointeurSprite->coin4.pt_Y=pointeurSprite->coin4.pt_Y+pointeurSprite->hauteur;
}

/* Modifie la rotation du sprite en fonction d'un angle dans le sens des aiguilles d'une montre */
void spriteModificationRotation(struct Sprite *pointeurSprite, int32 angle){
	//
	int32 decalageOX=pointeurSprite->centre.pt_X;
	int32 decalageOY=pointeurSprite->centre.pt_Y;

	int32 coin1_X=pointeurSprite->coinOriginal1.pt_X-pointeurSprite->centre.pt_X;
	int32 coin1_Y=pointeurSprite->coinOriginal1.pt_Y-pointeurSprite->centre.pt_Y;	
	int32 coin2_X=pointeurSprite->coinOriginal2.pt_X-pointeurSprite->centre.pt_X;	
	int32 coin2_Y=pointeurSprite->coinOriginal2.pt_Y-pointeurSprite->centre.pt_Y;	
	int32 coin3_X=pointeurSprite->coinOriginal3.pt_X-pointeurSprite->centre.pt_X;	
	int32 coin3_Y=pointeurSprite->coinOriginal3.pt_Y-pointeurSprite->centre.pt_Y;		
	int32 coin4_X=pointeurSprite->coinOriginal4.pt_X-pointeurSprite->centre.pt_X;
	int32 coin4_Y=pointeurSprite->coinOriginal4.pt_Y-pointeurSprite->centre.pt_Y;		
	
	// Affecte la position des coins
	//pointeurSprite->coin1.pt_X=pointeurSprite->coin1.pt_X*tableauCos[angle]-pointeurSprite->coin1.pt_Y*tableauSin[angle];
	//pointeurSprite->coin1.pt_Y=pointeurSprite->coin1.pt_X*tableauSin[angle]+pointeurSprite->coin1.pt_Y*tableauCos[angle];

	
	pointeurSprite->coin1.pt_X=coin1_X*cos(angle)-coin1_Y*sin(angle)+decalageOX;
	pointeurSprite->coin1.pt_Y=coin1_X*sin(angle)+coin1_Y*cos(angle)+decalageOY;
	pointeurSprite->coin2.pt_X=coin2_X*cos(angle)-coin2_Y*sin(angle)+decalageOX;
	pointeurSprite->coin2.pt_Y=coin2_X*sin(angle)+coin2_Y*cos(angle)+decalageOY;
	pointeurSprite->coin3.pt_X=coin3_X*cos(angle)-coin3_Y*sin(angle)+decalageOX;
	pointeurSprite->coin3.pt_Y=coin3_X*sin(angle)+coin3_Y*cos(angle)+decalageOY;
	pointeurSprite->coin4.pt_X=coin4_X*cos(angle)-coin4_Y*sin(angle)+decalageOX;
	pointeurSprite->coin4.pt_Y=coin4_X*sin(angle)+coin4_Y*cos(angle)+decalageOY;
	
	/*pointeurSprite->coin1.pt_X=pointeurSprite->coin1.pt_X+decalageOX;
	pointeurSprite->coin1.pt_Y=pointeurSprite->coin1.pt_Y+decalageOY;
	pointeurSprite->coin2.pt_X=pointeurSprite->coin2.pt_X+decalageOX;
	pointeurSprite->coin2.pt_Y=pointeurSprite->coin2.pt_Y+decalageOY;
	pointeurSprite->coin3.pt_X=pointeurSprite->coin3.pt_X+decalageOX;
	pointeurSprite->coin3.pt_Y=pointeurSprite->coin3.pt_Y+decalageOY;
	pointeurSprite->coin4.pt_X=pointeurSprite->coin4.pt_X+decalageOX;
	pointeurSprite->coin4.pt_Y=pointeurSprite->coin4.pt_Y+decalageOY;*/
	
	/*pointeurSprite->coin2.pt_X=pointeurSprite->coin2.pt_X*tableauCos[angle]-pointeurSprite->coin2.pt_Y*tableauSin[angle];
	pointeurSprite->coin2.pt_Y=pointeurSprite->coin2.pt_X*tableauSin[angle]+pointeurSprite->coin2.pt_Y*tableauCos[angle];
	pointeurSprite->coin3.pt_X=pointeurSprite->coin3.pt_X*tableauCos[angle]-pointeurSprite->coin3.pt_Y*tableauSin[angle];
	pointeurSprite->coin3.pt_Y=pointeurSprite->coin3.pt_X*tableauSin[angle]+pointeurSprite->coin3.pt_Y*tableauCos[angle];
	pointeurSprite->coin4.pt_X=pointeurSprite->coin4.pt_X*tableauCos[angle]-pointeurSprite->coin4.pt_Y*tableauSin[angle];
	pointeurSprite->coin4.pt_Y=pointeurSprite->coin4.pt_X*tableauSin[angle]+pointeurSprite->coin4.pt_Y*tableauCos[angle];*/
	
	/*pointeurSprite->coin1.pt_X=pointeurSprite->coin1.pt_X*tableauCos[angle]-pointeurSprite->coin1.pt_Y*-tableauSin[angle];
	pointeurSprite->coin1.pt_Y=pointeurSprite->coin1.pt_X*-tableauSin[angle]+pointeurSprite->coin1.pt_Y*tableauCos[angle];
	pointeurSprite->coin2.pt_X=pointeurSprite->coin2.pt_X*tableauCos[angle]-pointeurSprite->coin2.pt_Y*-tableauSin[angle];
	pointeurSprite->coin2.pt_Y=pointeurSprite->coin2.pt_X*-tableauSin[angle]+pointeurSprite->coin2.pt_Y*tableauCos[angle];
	pointeurSprite->coin3.pt_X=pointeurSprite->coin3.pt_X*tableauCos[angle]-pointeurSprite->coin3.pt_Y*-tableauSin[angle];
	pointeurSprite->coin3.pt_Y=pointeurSprite->coin3.pt_X*-tableauSin[angle]+pointeurSprite->coin3.pt_Y*tableauCos[angle];
	pointeurSprite->coin4.pt_X=pointeurSprite->coin4.pt_X*tableauCos[angle]-pointeurSprite->coin4.pt_Y*-tableauSin[angle];
	pointeurSprite->coin4.pt_Y=pointeurSprite->coin4.pt_X*-tableauSin[angle]+pointeurSprite->coin4.pt_Y*tableauCos[angle];*/
}

/* Configure la frame affichee du sprite */
void spriteConfigurationFrame(struct Sprite *pointeurSprite, int32 imageIndex){
	// Modifie l'image affichee du sprite
	pointeurSprite->imageIndex=imageIndex;
}

/* Configure l'animation cyclique du sprite */
void spriteConfigurationAnimationCyclique(struct Sprite *pointeurSprite, bool animationCyclique, int32 animationCycliqueMode, int32 animationCycliqueIntervalleDebut, int32 animationCycliqueIntervalleFin, int32 animationCycles){
	// Active ou desactive l'animation cyclique
	pointeurSprite->animationCyclique=animationCyclique;
	// Configure le mode d'animation
	pointeurSprite->animationCycliqueMode=animationCycliqueMode;
	// Si un nombre d'image est indique, et est correct, l'utilise pour configurer l'intervalle d'animation cyclique, tableauSinon utilise toutes les images du sprite
	if(animationCycliqueIntervalleDebut&&(animationCycliqueIntervalleDebut<animationCycliqueIntervalleFin)&&(animationCycliqueIntervalleFin<pointeurSprite->pointeurSpriteContenu->num_Frames)){ pointeurSprite->animationCycliqueIntervalleDebut=animationCycliqueIntervalleDebut; pointeurSprite->animationCycliqueIntervalleFin=animationCycliqueIntervalleFin; }else{ pointeurSprite->animationCycliqueIntervalleDebut=0; pointeurSprite->animationCycliqueIntervalleFin=(pointeurSprite->pointeurSpriteContenu->num_Frames-1); }
	// Initialise le nombre de cycles necessaire au changement de frame
	pointeurSprite->animationCycles=animationCycles;
	// Initialise le nombre de cycles ecoules
	pointeurSprite->animationCyclesCompteur=0;
}