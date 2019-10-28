#include "Annonce.h"

const Carte::Valeur Annonce::CARTES_MARIAGE[] =
    {
        Carte::DAME,
        Carte::ROI};
const Carte::Valeur Annonce::CARTES_TIERCE[] =
    {
        Carte::VALET, 
        Carte::DAME, 
        Carte::ROI};
const Carte::Valeur Annonce::CARTES_QUARANTE[] =
    {
        Carte::ROI, 
        Carte::DAME, 
        Carte::VALET, 
        Carte::AS};
const Carte::Valeur Annonce::CARTES_CHOUINE[] =
    {
        Carte::ROI, 
        Carte::DAME, 
        Carte::VALET, 
        Carte::DIX,
        Carte::AS};

const Annonce::TypeAnnonce Annonce::ANNONCES[] =
    {Annonce::MARIAGE, Annonce::TIERCE, 
     Annonce::QUARANTE, Annonce::QUINTE,
     Annonce::CHOUINE};

const set<int> Annonce::POINTS_ANNONCES =
    {20, 30, 40, 50, 1000};


Annonce::~Annonce()
{
}

void Annonce::ajouterCarte(Carte &_carte)
{
    // verificationd e la couleur
    if (m_Annonce != QUINTE)
    {
        if (_carte.couleur() != m_Couleur)
        {
            // pas la meme couleur, rien a faire
            return;
        }
    }
    // vérifier si la carte appartient bien à l'annnonce en question
    if (_carte.getValeur() == Carte::VALET)
    {
        if ((m_Annonce == TIERCE) || (m_Annonce == QUARANTE) ||
            (m_Annonce == CHOUINE) )
        {
            m_Cartes.ajouter(&_carte);
        }
    }
    if ((_carte.getValeur() == Carte::ROI) || 
        (_carte.getValeur() == Carte::DAME))
    {
        if (m_Annonce != QUINTE)
        {
            m_Cartes.ajouter(&_carte);
        }
    }
    if (_carte.getValeur() == Carte::DIX)
    {
        if ((m_Annonce == QUARANTE) || (m_Annonce == QUINTE) || 
            (m_Annonce == CHOUINE) )
        {
            m_Cartes.ajouter(&_carte);
        }
    }
    if (_carte.getValeur() == Carte::AS)
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
        if (_list[i]->getValeur() == Carte::AS || _list[i]->getValeur() == Carte::DIX)
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