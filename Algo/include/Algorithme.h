/*
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
*/
#ifndef _ALGORITHME_H_
#define _ALGORITHME_H_

class Joueur;
class Carte;

class Algorithme
{
public:
enum NIVEAUX
    {
        RANDOM            = 0,  // aléatoire
        PRISE_BRISQUE     = 1,  // Prend les brisques de l'adversaire
        CARTE_PLUS_FAIBLE = 2,  // Carte la plus faible
        VERIFIE_ANNONCE   = 3,  // vérifie si une annonce doit être faite avant de jouer une carte
        ECHANGE_7_ATOUT   = 4,  // prendre la carte d'atout avec le 7 d'atout
        PROBA_ANNONCE     = 5,  // mémoriser les cartes jouées pour faire des annonces
        SAUVER_BRISQUES   = 6   // On tente de sauver une brisque de la main si c'est possible
    };

    Algorithme(int _niveau, Joueur& _joueur):
     m_Niveau(_niveau), m_Joueur(_joueur) {};

    Carte* choisirCarteAlgo(Carte *_choixAdversaire=nullptr);

    // Calcule le score de chaque carte en fonction des annonces possibles et retourne la carte avec le score le plus bas
    Carte* calculScores(Carte *_choixAdversaire=nullptr);
    Carte* choisirCarte(Carte *_choixAdversaire);
    Carte* choisirCartePiocheVide(Carte *_choixAdversaire);

private:
    int m_Niveau;
    Joueur &m_Joueur;
};

#endif