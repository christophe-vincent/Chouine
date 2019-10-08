// ChouineTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include "Chouine.h"
#include "Joueur.h"
using namespace std;

bool verbose = true;
void log(){}


template<typename First, typename ...Rest>
void log(First && first, Rest && ...rest)
{
    if (verbose)
    {
        std::cout << std::forward<First>(first);
        log(std::forward<Rest>(rest)...);
    }
}


string getCardcolor(int _id)
{
    string ret = "";
    switch (_id >> 3)
    {
    case 0:
        ret = "Coeur";
        break;
    case 1:
        ret = "Pic";
        break;
    case 2:
        ret = "Trefle";
        break;
    case 3:
        ret = "Carreau";
        break;
    default:
        ret = "???";
    }
    return ret;
}

bool testChoix(string _choix)
{
    if ( (_choix == "erreur") || (_choix == "fin") || (_choix == "E_E"))
    {
        return true;
    }
    return false;
}

void partie(unsigned int _niveauJoueur1,
            unsigned int _niveauJoueur2, 
            int& _pointsJoueur1, 
            int& _pointsJoueur2)
{
    Chouine chouine(_niveauJoueur1, _niveauJoueur2);

    chouine.newGame();
    Joueur& joueur1 = chouine.joueur(Chouine::JOUEUR_1);
    Joueur& joueur2 = chouine.joueur(Chouine::JOUEUR_2);
    
    log("Atout : ", chouine.atout(), "\n");
    
    string choix;
    bool stop = false;
    int tour = 0;
    
    while (! stop)
    {
        tour ++;
        log("TOUR ", tour, "\n");
        log("Pioche  : ", chouine.pioche().cartes(), "\n");
        log("Joueur 1: ", joueur1.nomCartesMain(), "\n");
        log("Joueur 2: ", joueur2.nomCartesMain(), "\n");
        if (chouine.gagnantPli() == Chouine::JOUEUR_1)
        {
            choix = chouine.choixJoueur(Chouine::JOUEUR_1);
            stop = testChoix(choix);
            log("Choix Joueur 1 : ", choix, "\n");
            choix = chouine.choixJoueur(Chouine::JOUEUR_2);
            stop |= testChoix(choix);
            log("Choix Joueur 2 : ", choix, "\n");
        } else
        {
            choix = chouine.choixJoueur(Chouine::JOUEUR_2);
            stop = testChoix(choix);
            log("Choix Joueur 2 : ", choix, "\n");
            choix = chouine.choixJoueur(Chouine::JOUEUR_1);
            stop |= testChoix(choix);
            log("Choix Joueur 1 : ", choix, "\n");        
        }
        if (! stop)
        {
            chouine.finPli();
            stop = chouine.finPartie();
        }
        //if (tour > 3) stop = true;
    }
    _pointsJoueur1 = chouine.pointsJoueur(Chouine::JOUEUR_1);
    _pointsJoueur2 = chouine.pointsJoueur(Chouine::JOUEUR_2);
    log("Cartes joueur 1 : ", joueur1.nomCartesGagnees(), "\n");
    log("Cartes joueur 2 : ", joueur2.nomCartesGagnees(), "\n");
    log("Points joueur 1 : ", _pointsJoueur1, "\n");
    log("Points joueur 2 : ", _pointsJoueur2, "\n");
}


int main()
{
    unsigned int niveau1 = 0;
    unsigned int niveau2 = 0;
    int points1;
    int points2;
    int partiesJoueur1 = 0;
    int partiesJoueur2 = 0;
    int nbParties = 1;

    for(int i=0; i<nbParties; i++)
    {
        partie(niveau1, niveau2, points1, points2);
        points1 > points2 ? partiesJoueur1++ : partiesJoueur2++;
    }
    cout << "Parties gagnees joueur 1 : " << partiesJoueur1 << endl;
    cout << "Parties gagnees joueur 2 : " << partiesJoueur2 << endl;
    return 0;
}
