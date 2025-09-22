#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include "Chouine.h"
#include "Annonce.h"

using namespace std;
static std::random_device rd;
static std::mt19937 mt(rd());
static std::uniform_int_distribution<int> dist(1, 2);


Chouine::Chouine(int _niveauJoueurA, int _niveauJoueurB) : 
m_JoueurA(*this, _niveauJoueurA, JOUEUR_A), 
m_JoueurB(*this, _niveauJoueurB, JOUEUR_B)
{
    m_Joueurs[0] = &m_JoueurA;
    m_Joueurs[1] = &m_JoueurB;
    m_isChouine = false;
}

Chouine::~Chouine()
{
}


void Chouine::newGame()
{
    m_GagnantPli = &m_JoueurA;
    m_PerdantPli = &m_JoueurB;
    if (dist(mt) == 2)
    {
        m_GagnantPli = &m_JoueurB;
        m_PerdantPli = &m_JoueurA;
    }
    // TODO: remove
    // m_GagnantPli = JOUEUR_2;
    // m_PerdantPli = JOUEUR_1;
    // otherJoueur = 0;
    
    // ajoute les cartes à la pioche
    unsigned int id = 0;
    for (auto c = 0; c < Carte::NB_COLORS; c++)
    {
        for (auto v = 0; v < Carte::NB_VALUES; v++)
        {
            m_Pioche.ajouter(new Carte(
                Carte::ALL_COLORS[c],
                Carte::ALL_VALUES[v],
                false));
        }
    }

    m_Pioche.shuffle();
    m_Atout = m_Pioche.getLastCarte()->couleur();

    //cout << m_Pioche.nomCartes() << endl;
    //cout << m_Pioche.getLastCarte()->nom() << endl;
    
    // ajoute l'attribut Atout pour les cartes d'atout
    auto pioche = m_Pioche.cartes();
    for (auto it=pioche.begin(); it!=pioche.end(); ++it)
    {
        if ((*it)->couleur() == m_Atout)
        {
            (*it)->atout(true);
        }
    }

    // Distribution, les 2 joueurs piochent pour avoir 5 cartes chacun
    for (int i = 0; i < Joueur::MAX_CARDS; i++)
    {
        Carte *card = m_Pioche.piocheCarte();
        if (card != nullptr)
            m_GagnantPli->ajouterCarteMain(*card);

        card = m_Pioche.piocheCarte();
        if (card != nullptr)
            m_PerdantPli->ajouterCarteMain(*card);
    }

    // creation de toutes les annnonces possibles
    Annonce *annonce;
    for (unsigned int i=0; i<Carte::NB_COLORS; i++)
    {
        bool atout = false;
        if (Carte::ALL_COLORS[i] == m_Atout)
            atout = true;
        annonce = new Annonce(Carte::ALL_COLORS[i],
                      Annonce::MARIAGE, atout);
        m_Annonces.insert(annonce);
        annonce = new Annonce(Carte::ALL_COLORS[i],
                      Annonce::TIERCE, atout);
        m_Annonces.insert(annonce);
        annonce = new Annonce(Carte::ALL_COLORS[i],
                      Annonce::QUARANTE, atout);
        m_Annonces.insert(annonce);
        annonce = new Annonce(Carte::ALL_COLORS[i],
                      Annonce::CHOUINE, atout);
        m_Annonces.insert(annonce);
    }
    annonce = new Annonce(Carte::UNDEF_COLOR, Annonce::QUINTE, false);
    m_Annonces.insert(annonce);
}

bool Chouine::piocheVide()
{
    bool ret;
    (m_Pioche.size() > 0) ? ret = false : ret = true;
    return ret;
}

string Chouine::choixJoueur(string& _annonce)
{
    string ret("erreur");
    Carte *carte = nullptr;
    string annonce = "";

    if (m_GagnantPli->carteJouee() == nullptr)
    {
        // C'est le premier joueur à jouer
        carte = m_GagnantPli->choisirCarte(nullptr, _annonce);
    } else
    {
        // c'est au second joueur de jouer
        carte = m_PerdantPli->choisirCarte(m_GagnantPli->carteJouee(), _annonce);
    }
    if (carte == nullptr)
    {
        return ret;
    }

    return carte->carteToStr();
}

bool Chouine::setJoueurChoice(int _player, int _choice)
{
    if (_player > 1)
    {
        return false;
    }
   /* Carte *card;
    card = m_Joueurs[_player]->getCarte(_choice);
    if (card == nullptr)
    {
        return false;
    }
    // check if user can play this card
    if ((m_Pioche.size() == 0) && (m_GagnantPli != _player))
    {
        if (!m_Joueurs[_player]->isCarteAllowed(*card, *m_CarteJouee))
        {
            return false;
        }
    }

    // TODO : lancer la résolution de la done ici
*/
    return true;
}

Chouine::JOUEUR Chouine::finPli()
{
    // les carte ne seront ensuite plus accessibles
    Carte* carteJoueurA = m_JoueurA.carteJouee();
    Carte* carteJoueurB = m_JoueurB.carteJouee();

    if (m_GagnantPli->id() == m_JoueurA.id())
    {
        if (!carteJoueurA->gagnante(*carteJoueurB))
        {
            // le joueur A a gagné
            m_JoueurA.finPli(true, *carteJoueurB);
            m_JoueurB.finPli(false, *carteJoueurA);
        }
        else
        {
            // le joueur B a gagné
            m_GagnantPli = &m_JoueurB;
            m_PerdantPli = &m_JoueurA;
            m_JoueurB.finPli(true, *carteJoueurA);
            m_JoueurA.finPli(false, *carteJoueurB);
        }
    }
    else
    {
        if (!carteJoueurB->gagnante(*carteJoueurA))
        {
            // le joueur B a gagné
            m_JoueurB.finPli(true, *carteJoueurA);
            m_JoueurA.finPli(false, *carteJoueurB);
        }
        else
        {
            // le joueur A a gagné
            m_GagnantPli = &m_JoueurA;
            m_PerdantPli = &m_JoueurB;
            m_JoueurA.finPli(true, *carteJoueurB);
            m_JoueurB.finPli(false, *carteJoueurA);
        }
    }

    return (JOUEUR)m_GagnantPli->id();
}

bool Chouine::finPartie() 
{ 
    return (m_Pioche.size() + m_JoueurA.cartes().size() ) == 0 ? true : false;
}


int Chouine::pointsJoueur(JOUEUR _joueur)
{
    return m_Joueurs[_joueur]->points();
}
