#ifndef _JOUEUR_H_
#define _JOUEUR_H_
#include <string>
#include <set>
#include <map>
#include "Carte.h"
#include "ListeCartes.h"
#include "Annonce.h"
#include "Algorithme.h"

using namespace std;

class Chouine;

class Node
{
private:
    Node *m_Parent;
    // pair : card idx play 1, card idx player 2
    map<pair<int, int>, Node *> m_Nodes;
    int m_Points;
};

class Joueur
{
public:
    Joueur(Chouine &_chouine, int _niveau);
    virtual ~Joueur();

    static const int MAX_CARDS;

    void niveau(int _niveau) { m_Niveau = _niveau; }
    int niveau() { return m_Niveau; }
    ListeCartes &cartes() { return m_Cartes; }
    ListeCartes &cartesGagnees() { return m_CartesGagnees; }
    Annonce *getLatestAnnonce() { return m_LatestAnnonce; }
    bool getIsChouine() { return m_IsChouine; }
    int CarteLeft() { return m_Cartes.size(); }
    int getPoints() { return m_CartesGagnees.getPoints(); }
    int get10Der() { return m_10Der; }
    set<Annonce *> getAnnouces() { return m_Annonces; }
    Carte* carteJouee() { return m_CarteJouee; }

    string nomCartesMain();
    string nomCartesGagnees();
    Carte *getCarte(unsigned int _index);
    void ajouterCartesGagneesAdversaire(Carte *_c1, Carte *_c2);
    bool hasChange7Trump();
    Status ajouterCarte(Carte &_carte);
    Status supprimerCarte(Carte *_carte);
    Annonce* rechercheAnnonce(Carte &_carte);
    void printAnnonces();
    Carte* choisirCarte(Carte *_enemyChoice);
    void pliGagnant(Carte& _carteAdversaire);
    void pliPerdant(Carte& _carteAdversaire);

    Carte *getSmallestTrump();
    int trumpNumber();
    bool replaceTrumpCarte(Carte *_newCarte);
    bool isCarteAllowed(Carte &_card, Carte &_otherCarte);
    Status PlayCarte(Carte &_card);
    Status ajouterCartesGagnees(Carte &_card1, Carte &_card2);
    Carte *bruteForceAttack(ListeCartes &_hisCartes);

protected:
    Carte *EmptyPickSimulation(Carte &_userChoice);

    bool m_IsChouine;
    bool m_Change7Trump;

private:
    Chouine &m_Chouine;
    ListeCartes m_Cartes;    // 5 cards to be played
    ListeCartes m_CartesGagnees; // cards win
    ListeCartes m_CartesGagneesAdversaire;
    set<Annonce *> m_Annonces;
    Annonce *m_LatestAnnonce;
    int m_Niveau;
    int m_10Der;
    Algorithme m_Algo;
    Carte *m_CarteJouee;
};

#endif