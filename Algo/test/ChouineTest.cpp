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

bool testChoix(string _choix, bool& _erreur)
{
    if ( (_choix == "erreur") || (_choix == "E_E"))
    {
        _erreur = true;
        if (_choix == "fin")
        {
            return true;
        }        
    }
    return false;
}


int partie(unsigned int _niveauJoueur1,
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
    bool erreur = false;
    Chouine::JOUEUR gagnant;
    
    while (! stop)
    {
        tour ++;
        log("\nTOUR ", tour, "\n");
        log("Pioche  : ", chouine.pioche().nomCartes(), "\n");
        log("Joueur 1: ", joueur1.nomCartesMain(), "\n");
        log("Joueur 2: ", joueur2.nomCartesMain(), "\n");
        if (chouine.gagnantPli() == Chouine::JOUEUR_1)
        {
            choix = chouine.choixJoueur(Chouine::JOUEUR_1);
            stop = testChoix(choix, erreur);
            log("Choix Joueur 1 : ", choix, "\n");
            choix = chouine.choixJoueur(Chouine::JOUEUR_2);
            stop |= testChoix(choix, erreur);
            log("Choix Joueur 2 : ", choix, "\n");
        } else
        {
            choix = chouine.choixJoueur(Chouine::JOUEUR_2);
            stop = testChoix(choix, erreur);
            log("Choix Joueur 2 : ", choix, "\n");
            choix = chouine.choixJoueur(Chouine::JOUEUR_1);
            stop |= testChoix(choix, erreur);
            log("Choix Joueur 1 : ", choix, "\n");        
        }
        if (erreur)
        {
            cout << "Erreur" << endl;
            return 1;
        }
        if (! stop)
        {
            gagnant = chouine.finPli();
            stop = chouine.finPartie();
            log("Gagnant: ", gagnant+1, "\n");
            log("Gagnant pli: ", chouine.gagnantPli()+1, "\n");
            //log("Points joueur 1 : ", _pointsJoueur1, "\n");
            //log("Points joueur 2 : ", _pointsJoueur2, "\n");
        }
        //if (tour > 3) stop = true;
    }
    _pointsJoueur1 = chouine.pointsJoueur(Chouine::JOUEUR_1);
    _pointsJoueur2 = chouine.pointsJoueur(Chouine::JOUEUR_2);
    log("Cartes joueur 1 : ", joueur1.nomCartesGagnees(), "\n");
    log("Cartes joueur 2 : ", joueur2.nomCartesGagnees(), "\n");
    log("Points joueur 1 : ", _pointsJoueur1, "\n");
    log("Points joueur 2 : ", _pointsJoueur2, "\n");
    return 0;
}


int main()
{
    unsigned int niveau1 = 0;
    unsigned int niveau2 = 1;
    int points1;
    int points2;
    int partiesJoueur1 = 0;
    int partiesJoueur2 = 0;
    int pointsJoueur1 = 0;
    int pointsJoueur2 = 0;
    int nbParties = 1;
    int ret;

    for(int i=0; i<nbParties; i++)
    {
        ret = partie(niveau1, niveau2, points1, points2);
        if (ret != 0)
        {
            break;
        }
        points1 > points2 ? partiesJoueur1++ : partiesJoueur2++;
        pointsJoueur1 += points1;
        pointsJoueur2 += points2;
    }
    cout << "Joueur 1: Parties " << partiesJoueur1;
    cout << " points " << pointsJoueur1 << endl;
    cout << "Joueur 2: Parties " << partiesJoueur2;
    cout << " points " << pointsJoueur2 << endl;
    return ret;
}
