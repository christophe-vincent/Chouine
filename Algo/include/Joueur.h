#ifndef _JOUEUR_H_
#define _JOUEUR_H_
#include <string>
#include <set>
#include <map>
#include "Carte.h"
#include "ListeCartes.h"
#include "Algorithme.h"

class Annonce;
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
    Joueur(Chouine &_chouine, int _niveau, int _id);
    virtual ~Joueur();

    static const int MAX_CARDS;

    int id() { return m_Id; }

    // gestion du niveau du joueur
    void niveau(int _niveau) { m_Niveau = _niveau; }
    int niveau() { return m_Niveau; }

    void nouvellePartie();

    // retourne la list des cartes en main
    ListeCartes &cartes() { return m_CartesMain; }

    // nombre de cartes en main
    int quantiteCartesMain() { return m_CartesMain.size(); }

    int get10Der() { return m_10Der; }
    Carte* carteJouee() { return m_CarteJouee; }
    ListeCartes& cartesJouees() { return m_CartesJouees; }

    // retourne le nombre de points du joueur
    int points();
    
    // retourne la carte
    Carte *carteMain(unsigned int _index);

    // ajoute une carte à la main
    Status ajouterCarteMain(Carte &_carte);

    // supprime une carte de la main
    void supprimerCarte(Carte *_carte);

    // Vérifie si une annonce doit être déclarée avant de jouer cette carte
    Annonce* rechercheAnnonce(Carte &_carte);

    // une carte a été choisie
    int choixCarte(std::string& _carte);

    // choisir une carte à jouer
    Carte* choisirCarte(Carte *_enemyChoice, std::string& _annonce);

    // fin du pli
    void finPli(bool _gagnant, Carte& _carteAdversaire);

    // prend la carte d'atout avec le 7 d'atout
    bool prendreCarteAtout();

    // a t'on le droit de jouer cette carte quand la pioche est vide ?
    bool carteAurotisee(Carte &_card, Carte &_otherCarte);
    
    std::string cartesMainToStr();
    std::string cartesGagneesToStr();

protected:
    Carte *EmptyPickSimulation(Carte &_userChoice);

    bool m_IsChouine;
    bool m_Change7Trump;

private:
    int m_Id;
    bool m_SecondChoix;
    Chouine &m_Chouine;
    ListeCartes m_CartesMain;    // 5 cartes de la main
    ListeCartes m_CartesGagnees; // cards win
    ListeCartes m_CartesJouees;
    Annonce *m_LatestAnnonce;
    int m_Niveau;
    int m_10Der;
    int m_PointsAnnonces;
    Algorithme m_Algo;
    Carte *m_CarteJouee;
    std::map<Annonce*, int> m_Annonces;
};

#endif