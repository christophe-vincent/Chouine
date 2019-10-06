// ChouineTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "Chouine.h"
#include "Joueur.h"
using namespace std;

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
    if ( (_choix == "erreur") || (_choix == "fin"))
    {
        return true;
    }
    return false;
}

int main()
{
    Chouine chouine(0, 0);

    chouine.newGame();
    Joueur joueur1 = chouine.joueur(Chouine::JOUEUR_1);
    Joueur joueur2 = chouine.joueur(Chouine::JOUEUR_2);
    
    cout << "Atout : " << chouine.atout() << endl;
    
    string choix;
    bool stop = false;
    int tour = 0;
    
    while (! stop)
    {
        tour ++;
        cout << endl;
        cout << "TOUR " << tour << endl;
        cout << "Pioche  : " << chouine.pioche().cartes() << endl;
        cout << "Joueur 1: " << joueur1.main() << endl;
        cout << "Joueur 2: " << joueur2.main() << endl;
        if (chouine.gagnantPli() == Chouine::JOUEUR_1)
        {
            choix = chouine.choixJoueur(Chouine::JOUEUR_1);
            stop = testChoix(choix);
            cout << "Choix Joueur 1 : " << choix << endl;
            choix = chouine.choixJoueur(Chouine::JOUEUR_2);
            stop |= testChoix(choix);
            cout << "Choix Joueur 2 : " << choix << endl;
        } else
        {
            choix = chouine.choixJoueur(Chouine::JOUEUR_2);
            stop = testChoix(choix);
            cout << "Choix Joueur 2 : " << choix << endl;
            choix = chouine.choixJoueur(Chouine::JOUEUR_1);
            stop |= testChoix(choix);
            cout << "Choix Joueur 1 : " << choix << endl;        
        }
        if (! stop)
        {
            chouine.finPli();
            stop = chouine.finPartie();
        }
        //if (tour > 3) stop = true;
    }
    
    return 0;
}
