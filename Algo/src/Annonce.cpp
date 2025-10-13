#include <algorithm>
#include <cmath>
#include "Annonce.h"

const std::array<Carte::Valeur, 2> Annonce::CARTES_MARIAGE =
{
    Carte::DAME,
    Carte::ROI
};
const std::array<Carte::Valeur, 3> Annonce::CARTES_TIERCE =
{
    Carte::VALET,
    Carte::DAME,
    Carte::ROI
};
const std::array<Carte::Valeur, 4> Annonce::CARTES_QUARANTE =
{
    Carte::ROI,
    Carte::DAME,
    Carte::VALET,
    Carte::AS
};
const std::array<Carte::Valeur, 5> Annonce::CARTES_CHOUINE =
{
    Carte::ROI,
    Carte::DAME,
    Carte::VALET,
    Carte::DIX,
    Carte::AS
};

const Annonce::TypeAnnonce Annonce::ANNONCES[] =
{
    Annonce::MARIAGE,
    Annonce::TIERCE,
    Annonce::QUARANTE,
    Annonce::QUINTE,
    Annonce::CHOUINE
};

const set<int> Annonce::POINTS_ANNONCES =
    {20, 30, 40, 50, 60};



Annonce::Annonce(Carte::Couleur _couleur, TypeAnnonce _type, bool _atout) :
    m_Couleur(_couleur), m_Annonce(_type), m_Atout(_atout), m_Points(0)
{

    switch (m_Annonce)
    {
        case MARIAGE:
            m_Points = 20;
            break;
        case TIERCE:
            m_Points = 30;
            break;
        case QUARANTE:
            m_Points = 40;
            break;
        case CHOUINE:
            m_Points = 150;
            break;
        case QUINTE:
            m_Points = 50;
            break;
        default:
            break;
    }
    if (m_Atout)
    {
        m_Points *= 2;
    }
}


Annonce::~Annonce()
{
}

int Annonce::calculeScore(ListeCartes &_cartesMain, ListeCartes &_cartesJouees)
{
    int nbCarte = 0;
    for (unsigned int i = 0; i < _cartesMain.size(); i++)
    {
        if (carteDansAnnonce(*_cartesMain[i]) == true)
        {
            nbCarte++;
        }
    }
    int nbCartes = 10000;
    switch (m_Annonce)
    {
        case MARIAGE:
            nbCartes = sizeof(CARTES_MARIAGE) / sizeof(Carte::Valeur);
            break;
        case TIERCE:
            nbCartes = sizeof(CARTES_TIERCE) / sizeof(Carte::Valeur);
            break;
        case QUARANTE:
            nbCartes = sizeof(CARTES_QUARANTE) / sizeof(Carte::Valeur);
            break;
        case CHOUINE:
            nbCartes = sizeof(CARTES_CHOUINE) / sizeof(Carte::Valeur);
            break;
        case QUINTE:
            nbCartes = 5;
            break;
        
        default:
            break;
    }
    // pourcentage de cartes dans l'annonce
    //int score = int((nbCarte * 100) / nbCartes);
    // probabilité d'avoir cette annonce en main
    int proba = 100*std::pow(0.5, nbCartes - nbCarte);

    // prendre en compte les cartes déjà jouées
    for (unsigned int i = 0; i < _cartesJouees.size(); i++)
    {
        if (carteDansAnnonce(*_cartesJouees[i]) == true)
        {
            proba = -1; // annonce plus possible
        }
    }
    return proba;
}

void Annonce::ajouterCarte(Carte &_carte)
{
    // verification de la couleur
    if (m_Annonce != QUINTE)
    {
        if (_carte.couleur() != m_Couleur)
        {
            // pas la meme couleur, rien a faire
            return;
        }
    }
    // vérifier si la carte appartient bien à l'annnonce en question
    if (_carte.valeur() == Carte::VALET)
    {
        if ((m_Annonce == TIERCE) || (m_Annonce == QUARANTE) ||
            (m_Annonce == CHOUINE) )
        {
            m_Cartes.ajouter(&_carte);
        }
    }
    if ((_carte.valeur() == Carte::ROI) || 
        (_carte.valeur() == Carte::DAME))
    {
        if (m_Annonce != QUINTE)
        {
            m_Cartes.ajouter(&_carte);
        }
    }
    if (_carte.valeur() == Carte::DIX)
    {
        if ((m_Annonce == QUARANTE) || (m_Annonce == QUINTE) || 
            (m_Annonce == CHOUINE) )
        {
            m_Cartes.ajouter(&_carte);
        }
    }
    if (_carte.valeur() == Carte::AS)
    {
        if ((m_Annonce == QUINTE) || (m_Annonce == CHOUINE) )
        {
            m_Cartes.ajouter(&_carte);
        }
    }
}

void Annonce::supprimerCarte(Carte *_carte)
{
    m_Cartes.supprimer(_carte);
}

bool Annonce::carteDansAnnonce(Carte &_carte)
{
    // cas particulier de la quinte
    if (m_Annonce == QUINTE) 
    {
        // Si la carte est un As ou 10, alors OK
        if (_carte.valeur() == Carte::AS || _carte.valeur() == Carte::DIX)
        {
            return true;
        }
        return false;
    }

    // autres annonces, vérifier la couleur
    if (_carte.couleur() != m_Couleur)
    {
        return false;
    }
    // vérifier si la valeur fait partie de l'annonce
    switch (m_Annonce)
    {
        case MARIAGE:
            return std::find(CARTES_MARIAGE.begin(), CARTES_MARIAGE.end(), _carte.valeur()) != CARTES_MARIAGE.end();
        case TIERCE:
            return std::find(CARTES_TIERCE.begin(), CARTES_TIERCE.end(), _carte.valeur()) != CARTES_TIERCE.end();
        case QUARANTE:
            return std::find(CARTES_QUARANTE.begin(), CARTES_QUARANTE.end(), _carte.valeur()) != CARTES_QUARANTE.end();
        case CHOUINE:
            return std::find(CARTES_CHOUINE.begin(), CARTES_CHOUINE.end(), _carte.valeur()) != CARTES_CHOUINE.end();
    }
    return false;
}

/*void Annonce::computeScore()
{
   // score depends on the carte number
   switch (m_Annonce)
   {

   case MARIAGE:
      m_Score = (float)m_Cartes.size() / (float)MARIAGE_CARDS.length;
      break;
   case TIERCE:
      m_Score = (float)m_Cartes.size() / (float)TIERCE_CARDS.length;
      break;
   case QUARANTE:
      m_Score = (float)m_Cartes.size() / (float)QUARANTE_CARDS.length;
      break;
   case QUINTE:
      m_Score = (float)m_Cartes.size() / 5.0f;
      break;
   case CHOUINE:
      m_Score = (float)m_Cartes.size() / (float)CHOUINE_CARDS.length;
      break;
   }
}*/

bool Annonce::mariage(ListeCartes &_list)
{
    if ((_list.isContainsValeur(Carte::DAME)) ||
        (_list.isContainsValeur(Carte::ROI)))
    {
        return true;
    }
    return false;
}

bool Annonce::tierce(ListeCartes &_list)
{
    if ((_list.isContainsValeur(Carte::VALET)) ||
        (_list.isContainsValeur(Carte::DAME)) ||
        (_list.isContainsValeur(Carte::ROI)))
    {
        return true;
    }
    return false;
}

bool Annonce::quarante(ListeCartes &_list)
{
    if ((_list.isContainsValeur(Carte::VALET)) ||
        (_list.isContainsValeur(Carte::DAME)) ||
        (_list.isContainsValeur(Carte::ROI)) ||
        (_list.isContainsValeur(Carte::DIX)))
    {
        return true;
    }
    return false;
}

bool Annonce::quinte(ListeCartes &_list)
{
    int nBrisques = 0;

    for (unsigned int i = 0; i < _list.size(); i++)
    {
        if (_list[i]->valeur() == Carte::AS || _list[i]->valeur() == Carte::DIX)
        {
            nBrisques++;
        }
    }
    return nBrisques == 5;
}

bool Annonce::chouine(ListeCartes &_list)
{
    bool ret = true;
    Carte::Couleur couleur;

    if (_list.size() < 5)
        return false;

    // first check that all carte have the same couleur
    couleur = _list[0]->couleur();
    for (unsigned int i = 1; i < _list.size(); i++)
    {
        if (_list[i]->couleur() != couleur)
        {
            return false;
        }
    }
    if ((_list.isContainsValeur(Carte::SEPT)) ||
        (_list.isContainsValeur(Carte::HUIT)) ||
        (_list.isContainsValeur(Carte::NEUF)))
    {
        ret = false;
    }
    return ret;
}


unsigned int Annonce::cartesManquantes()
{
    int ret = 0;
    switch (m_Annonce)
    {
    case MARIAGE:
        ret = sizeof(CARTES_MARIAGE) / sizeof(Carte::Valeur);
        break;
    case TIERCE:
        ret = sizeof(CARTES_TIERCE) / sizeof(Carte::Valeur);
        break;
    case QUARANTE:
        ret = sizeof(CARTES_QUARANTE) / sizeof(Carte::Valeur);
        break;
    case CHOUINE:
        ret = sizeof(CARTES_CHOUINE) / sizeof(Carte::Valeur);
        break;
    case QUINTE:
        ret = 5;
        break;
    
    default:
        break;
    }
    return ret - m_Cartes.nombreCartes();
}


string Annonce::to_string()
{
    string text;
    switch (m_Annonce)
    {
        case Annonce::MARIAGE :
            text = "mariage";
            break;
        case Annonce::TIERCE :
            text = "tierce";
            break;
        case Annonce::QUARANTE :
            text = "quarante";
            break;
        case Annonce::CHOUINE :
            text = "chouine";
            break;
        case Annonce::QUINTE :
            text = "quinte";
            break;
        default:
            break;
        }
        text += "-";

        switch (m_Couleur)
        {
        case Carte::PIC:
            text += "pic";
            break;
        case Carte::TREFLE:
            text += "trefle";
            break;
        case Carte::CARREAU:
            text += "carreau";
            break;
        case Carte::COEUR:
            text += "coeur";
            break;
        default:
            break;
    }
    return text;
}