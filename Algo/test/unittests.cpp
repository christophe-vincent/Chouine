#include "Carte.h"
#include "Chouine.h"
#include "Joueur.h"

int testCarte()
{
    Carte c1(Carte::COEUR, Carte::AS, true);
    Carte c2(Carte::PIC, Carte::DIX, false);
    Carte c3(Carte::COEUR, Carte::DAME, true);
    Carte c4(Carte::COEUR, Carte::ROI, true);
    Carte c5(Carte::COEUR, Carte::SEPT, true);

    // if (!(c1 > c2)) return 1;
    // if (!(c3 < c1)) return 1;
    // if (!(c4 < c1)) return 1;
    // if (!(c5 < c1)) return 1;
    // if (!(c3 < c4)) return 1;
    // if (!(c5 < c4)) return 1;
    // if (!(c5 < c3)) return 1;
    // if (!(c2 < c5)) return 1;

    return 0;
}

int testAnnonce()
{
    Chouine chouine(3, 3);
    chouine.newGame();
    Joueur joueur1 = chouine.joueur(Chouine::JOUEUR_A);
    joueur1.supprimerCarte(joueur1.carteMain(0));
    joueur1.supprimerCarte(joueur1.carteMain(0));
    Carte c1(Carte::COEUR, Carte::ROI, true);
    Carte c2(Carte::COEUR, Carte::DAME, true);
    joueur1.ajouterCarteMain(c1);
    joueur1.ajouterCarteMain(c2);
    Annonce* annonce = joueur1.rechercheAnnonce(c1);
    return 0;
}


int main()
{
    int ret = 1;

    ret = testCarte();
    if (ret != 0) {
        printf("test_carte failed\n");
        return 1;
    }

    ret = testAnnonce();
    if (ret != 0) {
        printf("test_carte failed\n");
        return 1;
    }
    printf("All tests passed\n");
    return 0;
}