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

int main()
{
    Chouine chouine(0, 1);

    chouine.newGame();
    Joueur joueur1 = chouine.joueur(Chouine::JOUEUR_1);
    Joueur joueur2 = chouine.joueur(Chouine::JOUEUR_2);
    
    cout << "Joueur 1: " << joueur1.main() << endl;
    cout << "Joueur 2: " << joueur2.main() << endl; 
    
    cout << "TOUR 1" << endl;
    chouine.choixJoueur(Chouine::JOUEUR_1);
    chouine.choixJoueur(Chouine::JOUEUR_2);
    cout << "Joueur 1: " << joueur1.main() << endl;
    cout << "Joueur 2: " << joueur2.main() << endl; 
    
    return 0;
}
