/**
*
* @author vincentc
*/
#ifndef _CHOUINE_H_
#define _CHOUINE_H
#include <array>

//#include "Carte.h"
//#include "Announce.h"
#include "Joueur.h"
//#include "Pick.h"

class Joueur;

enum CarteId
{
    COEUR_7,
    COEUR_8,
    COEUR_9,
    COEUR_VALET,
    COEUR_DAME,
    COEUR_ROI,
    COEUR_10,
    COEUR_AS,
    PIC_7,
    PIC_8,
    PIC_9,
    PIC_VALET,
    PIC_DAME,
    PIC_ROI,
    PIC_10,
    PIC_AS,
    TREFLE_7,
    TREFLE_8,
    TREFLE_9,
    TREFLE_VALET,
    TREFLE_DAME,
    TREFLE_ROI,
    TREFLE_10,
    TREFLE_AS,
    CARREAU_7,
    CARREAU_8,
    CARREAU_9,
    CARREAU_VALET,
    CARREAU_DAME,
    CARREAU_ROI,
    CARREAU_10,
    CARREAU_AS,
    NONE
};

class Chouine
{
public:
    enum JOUEUR
    {
        ERREUR = -1,
        JOUEUR_A = 0,
        JOUEUR_B = 1
    };

public:
    Chouine(int _niveauJoueurA, int _niveauJoueurB);
    ~Chouine();
    
    void newGame();

    void distribution_cartes();

    Joueur joueur(JOUEUR _id) { return *m_Joueurs[_id];};

    // retourne le gagnant du pli
    JOUEUR gagnantPli() { return m_GagnantPli->id() == JOUEUR_A ? JOUEUR_A : JOUEUR_B;};

    // retourne le perdant du pli
    JOUEUR perdantPli() { return m_PerdantPli->id() == JOUEUR_A ? JOUEUR_A : JOUEUR_B;};

    ListeCartes& pioche() { return m_Pioche; }
    
    bool piocheVide();

    string choixJoueur(string& _annonce, string& _carteAtout);
    int setChoixJoueur(std::string _choice);
    int setChoixAnnonce(int _joueur, std::string _annonce);
    std::string annoncesEnMainJoueur(int _joueur);

    JOUEUR finPli();
    bool finPartie();

    int pointsJoueur(JOUEUR _joueur);

    // retourne toutes les annonces existantes
    set<Annonce*> getAnnonces() { return m_Annonces; }

    /*
   void testBruteForce();
   */
    Carte::Couleur couleurAtout() { return m_Atout; };
    std::string atout() { return Carte::couleurToStr(m_Atout);}

private:
    Carte::Couleur m_Atout;
    static Chouine *m_Instance;
    Joueur m_JoueurA;
    Joueur m_JoueurB;
    array<Joueur *, 2> m_Joueurs;
    array<int, 2> m_JoueurLevel;
    ListeCartes m_Pioche;
    Joueur* m_GagnantPli;
    Joueur* m_PerdantPli;
    bool m_isChouine;
    // toutes les annonces existantes (17)
    set<Annonce *> m_Annonces;
};
#endif