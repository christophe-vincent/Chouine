/*
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
*/
#include <iostream>
#include <string.h>
#include "Joueur.h"
#include "Chouine.h"
#include "Carte.h"
#include "Algorithme.h"
#include "Annonce.h"

const int Joueur::MAX_CARDS = 5;

using namespace std;

Joueur::Joueur(Chouine& _chouine, int _niveau, int _id):
m_Chouine(_chouine), m_Algo(_niveau, *this), m_Niveau(_niveau)
{
    m_Id = _id;
    m_SecondChoix = false;
    m_10Der = 0;
    m_PointsAnnonces = 0;
    m_LatestAnnonce = nullptr;
    m_Change7Atout = "";
    m_IsChouine = false;
    m_CarteJouee = nullptr;
    m_Annonces.clear();
    m_CouleurAnnonces.clear();
    for (int i=0; i<Carte::NB_COLORS; i++)
    {
        m_CouleurAnnonces[Carte::ALL_COLORS[i]] = false;
    }
    m_CouleurAnnonces[Carte::UNDEF_COLOR] = false;
    m_Quinte = false;
}

Joueur::~Joueur()
{
}

void Joueur::nouvellePartie()
{
    m_10Der = 0;
    m_Annonces.clear();
    m_CarteJouee = nullptr;
    m_CartesGagnees.cartes().clear();
    m_CartesJouees.cartes().clear();
    m_CartesMain.cartes().clear();
    m_Change7Atout = "";
    m_IsChouine = false;
    m_LatestAnnonce = nullptr;
    m_PointsAnnonces = 0;
    for (int i=0; i<Carte::NB_COLORS; i++)
    {
        m_CouleurAnnonces[Carte::ALL_COLORS[i]] = false;
    }
    m_CouleurAnnonces[Carte::UNDEF_COLOR] = false;
    m_Quinte = false;
    m_SecondChoix = false;
}

int Joueur::points()
{
    return m_CartesGagnees.getPoints() + m_10Der + m_PointsAnnonces;
}


Carte *Joueur::carteMain(unsigned int _index)
{
    if (_index >= m_CartesMain.size())
    {
        return nullptr;
    }
    return m_CartesMain[_index];
}


Status Joueur::ajouterCarteMain(Carte &_carte)
{
    if (m_CartesMain.size() >= MAX_CARDS)
    {
        return Status::TOO_MANY_CARDS;
    }

    m_CartesMain.ajouter(&_carte);

    return Status::OK;
}


void Joueur::supprimerCarte(Carte *_carte)
{
    m_CartesMain.supprimer(_carte);
}


void Joueur::TrierCartesMain()
{
    std::vector<Carte *> nouvelOrdre;
    std::map<Carte::Couleur, int> couleurs = {
        {Carte::COEUR, 0}, {Carte::TREFLE, 100}, {Carte::CARREAU, 200}, {Carte::PIC, 300}
    };

    while (m_CartesMain.cartes().size() > 0)
    {
        // on recherche la plus petite valeur
        int min = 1000;
        Carte *ppc = nullptr;  // plus petite carte
        for (auto carte: m_CartesMain.cartes())
        {
            int poidsCarte = (int)carte->valeur() + couleurs[carte->couleur()];
            if (carte->atout())
            {
                // poids + important pour l'atout
                poidsCarte += 500;
            }
            if (poidsCarte < min)
            {
                ppc = carte;
                min = poidsCarte;
            }
        }
        nouvelOrdre.push_back(ppc);
        m_CartesMain.supprimer(ppc);
    }
    m_CartesMain.setCartes(nouvelOrdre);
}

// cherche si la carte fait partie d'une annonce en main
Annonce* Joueur::rechercheAnnonce(Carte &_carte)
{
    if (_carte.valeur() == Carte::SEPT ||
        _carte.valeur() == Carte::HUIT ||
        _carte.valeur() == Carte::NEUF)
    {
        // les 7,8,9 ne font jamais partie d'une annonce
        return nullptr;
    }

    Annonce* plusForteAnnonce = nullptr;
    for (const auto& pair : _carte.annonces())
    {
        Annonce* annonce = pair.first;
        int score = pair.second;
        if (score == 100)
        {
            // l'annonce est complète
            if (annonce > plusForteAnnonce)
            {
                plusForteAnnonce = annonce;
            }
        }
    }
    return plusForteAnnonce;
}


bool Joueur::prendreCarteAtout()
{
    int index = -1;
    Carte *card = nullptr;

    card = m_CartesMain.getCarteFromList(m_Chouine.couleurAtout(), Carte::SEPT);

    if (card != nullptr)
    {
        unsigned int position = m_CartesMain.position(card);
        Carte* nouvelleCarte = m_Chouine.pioche().echangerCarte(card, 0);
        m_CartesMain.echangerCarte(nouvelleCarte, position);
        m_Change7Atout = card->carteToStr();
        return true;
    }

    return false;
}

int Joueur::choixCarte(std::string& _carte, Carte* _carteAdversaire)
{
    m_LatestAnnonce = nullptr;
    // recherche la carte
    m_CarteJouee = nullptr;
    for (Carte* c: m_CartesMain.cartes())
    {
        if (c->carteToStr().compare(_carte) == 0)
        {
            // on vérifie si la carte est jouable
            if (m_Chouine.piocheVide() && _carteAdversaire != nullptr)
            {
                if (!carteAurotisee(*c, *_carteAdversaire))
                {
                    cout << "Erreur, la carte " << _carte << " n'est pas autorisée à être jouée" << endl;
                    return 2;
                }
            }
            m_CarteJouee = c;
            break;
        }
    }
    if (m_CarteJouee == nullptr)
    {
        cout << "Erreur, la carte " << _carte << "ne fait pas partie de la main" << endl;
        return 1;
    }
    return 0;
}

int Joueur::choixAnnonce(std::string& _annonce)
{
    m_LatestAnnonce = nullptr;
    // recherche l'annonce
    Annonce* annonce = nullptr;
    for (auto* ann: m_Chouine.getAnnonces()) 
    {
        if (ann->to_string().compare(_annonce) == 0 ||
            (_annonce == "chouine" && ann->type() == Annonce::CHOUINE))
        {
            int score = ann->calculeScore(m_CartesMain, m_CartesJouees);
            if (score == 100 && m_Annonces.count(ann) == 0)
            {
                if (((ann->type() == Annonce::QUINTE && m_Quinte == false)) ||
                    (ann->type() != Annonce::QUINTE && m_CouleurAnnonces[ann->couleur()] == false))
                {
                    if (ann->type() == Annonce::QUINTE)
                    {
                        m_Quinte = true;
                    }
                    annonce = ann;
                    break;
                }
            }
        }
    }
    if (annonce == nullptr)
    {
        cout << "Erreur, l'annonce " << _annonce << " ne fait pas partie des annonces possibles" << endl;
        return 1;
    }
    m_PointsAnnonces += annonce->points();
    m_Annonces[annonce] = annonce->points();
    m_CouleurAnnonces[annonce->couleur()] = true;
    if (annonce->type() == Annonce::CHOUINE) m_IsChouine = true;
    m_LatestAnnonce = annonce;
    return 0;
}


std::string Joueur::annoncesEnMain()
{
    std::string annonces;
    std::map<Carte::Couleur, std::map<Annonce::TypeAnnonce, Annonce*>> annonces_possible;

    for (auto* annonce: m_Chouine.getAnnonces()) 
    {
        // le score indique si une annonce est présente, partiellement presente ou absente de la main
        int score = annonce->calculeScore(m_CartesMain, m_CartesJouees);
        if (score == 100 && m_Annonces.count(annonce) == 0 && m_CouleurAnnonces[annonce->couleur()] == false)
        {
            if (annonce->type() == Annonce::TIERCE)
            {
                // supprimer l'annonce mariage
                annonces_possible[annonce->couleur()][Annonce::MARIAGE] = nullptr;
            }
            if (annonce->type() == Annonce::QUARANTE)
            {
                // supprimer l'annonce mariage
                annonces_possible[annonce->couleur()][Annonce::MARIAGE] = nullptr;
                annonces_possible[annonce->couleur()][Annonce::TIERCE] = nullptr;
            }
            if (annonce->type() == Annonce::CHOUINE)
            {
                // supprimer l'annonce mariage
                annonces_possible[annonce->couleur()][Annonce::MARIAGE] = nullptr;
                annonces_possible[annonce->couleur()][Annonce::TIERCE] = nullptr;
                annonces_possible[annonce->couleur()][Annonce::QUARANTE] = nullptr;
            }
            annonces_possible[annonce->couleur()][annonce->type()] = annonce;
        }
    }
    for (auto c: annonces_possible)
    {
        for (auto annonce: c.second)
        {
            if (annonce.second != nullptr)
            {
                if (annonces.size() > 0)
                {
                    annonces += " ";
                }
                annonces += annonce.second->to_string();
            }
        }
    }
    return annonces;
}


Carte* Joueur::choisirCarte(Carte *_carteAdversaire, string& _annonce)
{
    m_LatestAnnonce = nullptr;
    // Si il ne reste que 2 carte en pioche, échanger le 7 d'atout
    if (m_Niveau >= Algorithme::ECHANGE_7_ATOUT)
    {
        if (m_Chouine.pioche().cartes().size() <= 2 && m_Chouine.pioche().cartes().size() > 0 &&
            m_CartesMain.searchCarte(Carte::SEPT, m_Chouine.couleurAtout()) != nullptr)
        {
            prendreCarteAtout();
        }
    }
    // recalculer les annonces
    if (m_Niveau >= Algorithme::VERIFIE_ANNONCE)
    {
        for (Carte* carteMain: m_CartesMain.cartes())
        {
            carteMain->annonces().clear();
        }
        for (auto* annonce: m_Chouine.getAnnonces()) 
        {
            // le score indique si une annonce est présente, partiellement presente ou absente de la main
            int score = annonce->calculeScore(m_CartesMain, m_CartesJouees);
            if (score > 0)
            {
                // cout << "Annonce dans la main: " << annonce->to_string() << " Score=" << score << endl;

                // ajoutons l'annonce et son score à chaque carte concernée
                for (Carte* carteMain: m_CartesMain.cartes())
                {
                    //cout << "annonce: " << annonce->to_string() << " carte: " << carteMain->carteToStr();
                    //cout << " Present ? " << annonce->carteDansAnnonce(*carteMain) << endl;
                    if (annonce->carteDansAnnonce(*carteMain))
                    {
                        // ajoutons cette annonce dans la carte
                        carteMain->annonces()[annonce] = score;
                    }
                }
            }
        }
    }

    // for (Carte* carteMain: m_CartesMain.cartes())
    // {
    //     if (carteMain->annonces().size() > 0)
    //     {
    //         cout << carteMain->carteToStr() + " -> " << carteMain->annonceToStr() << endl;
    //     }
    // }

    m_CarteJouee = nullptr;
    if (m_Chouine.piocheVide())
    {
        // on doit donner la bonne couleur
        m_CarteJouee = m_Algo.choisirCartePiocheVide(_carteAdversaire);
        //m_CarteJouee = m_CartesMain[0];

        if (_carteAdversaire != nullptr)
        {
            if (!carteAurotisee(*m_CarteJouee, *_carteAdversaire))
            {
                cout << "Erreur, la carte n'est pas autorisée à être jouée" << endl;
                return nullptr;
            }
        }
    }
    else
    {
        m_CarteJouee = m_Algo.choisirCarte(_carteAdversaire);
    }

    _annonce = "";
    if (m_Niveau >= Algorithme::VERIFIE_ANNONCE)
    {
        // vérifier si la carte jouée fait partie d'une annonce
        Annonce* annonce = rechercheAnnonce(*m_CarteJouee);
        if (annonce != nullptr)
        {
            if (m_Annonces.find(annonce) == m_Annonces.end() &&
                m_CouleurAnnonces[annonce->couleur()] == false)
            {
                // l'annonce n'a pas déjà été faite
                if (annonce->type() == Annonce::CHOUINE) m_IsChouine = true;
                _annonce = annonce->to_string();
                m_CouleurAnnonces[annonce->couleur()] = true;
                m_PointsAnnonces += annonce->points();
                m_Annonces[annonce] = annonce->points();
                m_LatestAnnonce = annonce;
            }
        }
    }

    // si la carte jouée est le 7 d'atout, échange avec l'atout de la pioche et on revoit le choix
    if (m_Niveau >= Algorithme::ECHANGE_7_ATOUT)
    {
        if (m_CarteJouee->valeur() == Carte::SEPT && m_CarteJouee->atout() && m_Chouine.pioche().size() > 2 && m_SecondChoix == false)
        {
            prendreCarteAtout();
            m_SecondChoix = true;
            m_CarteJouee = choisirCarte(_carteAdversaire, _annonce);
        }
    }
    return m_CarteJouee;
}


void Joueur::finPli(bool _gagnant, Carte& _carteAdversaire)
{
    m_SecondChoix = false;
    if (_gagnant)
    {
        m_CartesGagnees.ajouter(&_carteAdversaire);
        m_CartesGagnees.ajouter(m_CarteJouee);
    }
    if (_carteAdversaire.getPoints() > 0 && !m_CartesJouees.rechercheCarte(_carteAdversaire))
    {
        m_CartesJouees.ajouter(&_carteAdversaire);
    }
    if (m_CarteJouee->getPoints() > 0)
    {
        m_CartesJouees.ajouter(m_CarteJouee);
    }
    supprimerCarte(m_CarteJouee);
    m_CarteJouee = nullptr;
    Carte* pioche = m_Chouine.pioche().piocheCarte();
    if (pioche != nullptr)
    {
        ajouterCarteMain(*pioche);
    }
}



Carte *Joueur::EmptyPickSimulation(Carte &_userChoice)
{
    Carte *playCarte;
    ListeCartes couleurList;

    // we must play a higher card
    m_CartesMain.getCouleurSubset(_userChoice.couleur(), couleurList);

    playCarte = couleurList.getHigherCarte(_userChoice);
    if (playCarte == nullptr)
    {
        // play trump if user card is not trump...
        if (!_userChoice.atout())
        {
            ListeCartes trumpList;
            m_CartesMain.getCouleurSubset(m_Chouine.couleurAtout(), trumpList);
            playCarte = couleurList.plusFaible();
        }
    }

    return playCarte;
}

// check if the Joueur is allowed to play this card or not
// it is only done when pick is empty : user must play higher card
bool Joueur::carteAurotisee(Carte &_card, Carte &_otherCarte)
{
    if (m_CartesMain.size() == 1)
    {
        // last card...
        return true;
    }

    // je joueur a t-il la même couleur que l'adservaire ?
    bool couleur = false;
    bool couleur_gagante = false;
    bool atout = false;
    for (Carte* carteMain: m_CartesMain.cartes())
    {
        if (carteMain->couleur() == _otherCarte.couleur())
        {
            couleur = true;
            if (carteMain->getPoints() > _otherCarte.getPoints())
            {
                couleur_gagante = true;
            }
        }
        if (carteMain->atout())
        {
            atout = true;
        }
    }
    if (couleur && _card.couleur() != _otherCarte.couleur())
    {
        // on doit jouer la même couleur
        return false;
    }
    if (couleur_gagante && _card.getPoints() < _otherCarte.getPoints())
    {
        // on doit monter
        return false;
    }

    if (!couleur && atout && !_card.atout())
    {
        // on a pas joué de l'atout face à l'adversaire qui n'a pas joué d'atout, on aurait dû...
        return false;
    }

    return true;
}

// serialize le joueur en format json
std::string Joueur::sauvegarde()
{
    std::string ret = "{";
    ret += "\"id\":" + std::to_string(m_Id) + ",";
    ret += "\"second_choix\":" + std::to_string(m_SecondChoix ? 1 : 0) + ",";
    ret += "\"change_7_atout\":\"" + m_Change7Atout + "\",";
    ret += "\"niveau\":" + std::to_string(m_Niveau) + ",";
    ret += "\"10der\":" + std::to_string(m_10Der) + ",";
    ret += "\"points_annonces\":" + std::to_string(m_PointsAnnonces) + ",";
    ret += "\"is_chouine\":" + std::to_string(m_IsChouine ? 1 : 0) + ",";
    ret += "\"carte_jouee\":\"";
    if (m_CarteJouee != nullptr)
    {
        ret += m_CarteJouee->carteToStr();
    }
    ret += "\",";
    ret += "\"quinte\":" + std::to_string(m_Quinte ? 1 : 0) + ",";
    ret += "\"cartes_main\":[";
    for (size_t i=0; i<m_CartesMain.size(); i++)
    {
        ret += "\"" + m_CartesMain[i]->carteToStr() + "\"";
        if (i < m_CartesMain.size() -1)
        {
            ret += ",";
        }
    }
    ret += "],";
    ret += "\"cartes_gagnees\":[";
    for (size_t i=0; i<m_CartesGagnees.size(); i++)
    {
        ret += "\"" + m_CartesGagnees[i]->carteToStr() + "\"";
        if (i < m_CartesGagnees.size() -1)
        {
            ret += ",";
        }
    }
    ret += "]";
    ret += ",\"cartes_jouees\":[";
    for (size_t i=0; i<m_CartesJouees.size(); i++)
    {
        ret += "\"" + m_CartesJouees[i]->carteToStr() + "\"";
        if (i < m_CartesJouees.size() -1)
        {
            ret += ",";
        }
    }
    ret += "]";
    // sauvegarde les annonces
    ret += ",\"annonces\":[";
    size_t count = 0;
    for (const auto& pair : m_Annonces)
    {
        Annonce* annonce = pair.first;
        ret += "\"" + annonce->to_string() + "\"";;
        if (count < m_Annonces.size() -1)
        {
            ret += ",";
        }
        count++;
    }
    ret += "]";
    // sauvegarde les couleur d'annonces
    ret += ",\"couleur_annonces\":[";
    count = 0;
    for (const auto& pair : m_CouleurAnnonces)
    {
        Carte::Couleur couleur = pair.first;
        bool faite = pair.second;
        if (faite)
        {
            ret += "\"" + Carte::couleurToStr(couleur) + "\"";;
            if (count < m_CouleurAnnonces.size() -1)
            {
                ret += ",";
            }
        }
        count++;
    }
    ret += "]";

    ret += "}";
    return ret;
}


// restaure le joueur
void Joueur::restauration(const std::string& _data)
{
    // simple parser json
    m_CartesMain.cartes().clear();
    m_CartesGagnees.cartes().clear();
    m_CartesJouees.cartes().clear();
    m_Annonces.clear();
    m_CouleurAnnonces.clear();

    size_t pos = 0;
    size_t next_pos = 0;

    // id
    pos = _data.find("\"id\":", pos);
    next_pos = _data.find(",", pos);
    m_Id = std::stoi(_data.substr(pos + 5, next_pos - pos -5));
    pos = next_pos +1;

    // second_choix
    pos = _data.find("\"second_choix\":", pos);
    next_pos = _data.find(",", pos);
    m_SecondChoix = (std::stoi(_data.substr(pos + 15, next_pos - pos -15)) != 0);
    pos = next_pos +1;

    // change_7_atout
    pos = _data.find("\"change_7_atout\":\"", pos);
    next_pos = _data.find("\",", pos);
    m_Change7Atout = _data.substr(pos + 18, next_pos - pos -18);
    pos = next_pos +2;

    // niveau
    pos = _data.find("\"niveau\":", pos);
    next_pos = _data.find(",", pos);
    m_Niveau = std::stoi(_data.substr(pos + 8, next_pos - pos -8));
    pos = next_pos +1;

    // 10der
    pos = _data.find("\"10der\":", pos);
    next_pos = _data.find(",", pos);
    m_10Der = std::stoi(_data.substr(pos + 7, next_pos - pos -7));
    pos = next_pos +1;

    // points_annonces
    pos = _data.find("\"points_annonces\":", pos);
    next_pos = _data.find(",", pos);
    m_PointsAnnonces = std::stoi(_data.substr(pos + 18, next_pos - pos -18));
    pos = next_pos +1;

    // is_chouine
    pos = _data.find("\"is_chouine\":", pos);
    next_pos = _data.find(",", pos);
    //m_IsChouine = (std::stoi(_data.substr(pos
    // a finir...
}

// retourne la main du joueur en string
string Joueur::cartesMainToStr()
{
    return m_CartesMain.nomCartes();
}

// carte gagnees
string Joueur::cartesGagneesToStr()
{
    return m_CartesGagnees.nomCartes();
}

// points en string
std::string Joueur::pointsToStr()
{
    std::string ret = std::to_string(points()) + "|";
    ret += std::to_string(m_CartesGagnees.getPoints()) + "|";
    ret += std::to_string(m_PointsAnnonces) + "|";
    if (m_10Der > 0)
    {
        ret += "10";
    }
    return ret;
}