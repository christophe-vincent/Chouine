#ifndef _ALGORITHME_H_
#define _ALGORITHME_H_

class Joueur;
class Carte;

class Algorithme
{
public:
    Algorithme(int _niveau, Joueur& _joueur):
     m_Niveau(_niveau), m_Joueur(_joueur) {};

    Carte* choisirCarte();
    Carte* choisirCarte(Carte &_choixAdversaire);
    Carte* choisirCartePiocheVide();
    Carte* choisirCartePiocheVide(Carte &_choixAdversaire);
    

private:
    int m_Niveau;
    Joueur &m_Joueur;

};

#endif