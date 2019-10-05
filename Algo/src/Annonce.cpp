#include "Annonce.h"

const Carte::Valeur Annonce::CARTES_MARIAGE[] =
    {
        Carte::DAME,
        Carte::ROI};
const Carte::Valeur CARTES_TIERCE[] =
    {
        Carte::VALET, 
        Carte::DAME, 
        Carte::ROI};
const Carte::Valeur CARTES_QUARANTE[] =
    {
        Carte::ROI, 
        Carte::DAME, 
        Carte::VALET, 
        Carte::AS};
const Carte::Valeur CARTES_CHOUINE[] =
    {
        Carte::ROI, 
        Carte::DAME, 
        Carte::VALET, 
        Carte::DIX,
         Carte::AS};

const set<Annonce::TypeAnnonce> Annonce::ANNONCES =
    {MARIAGE, TIERCE, QUARANTE, QUINTE, CHOUINE};

Annonce::Annonce(
    Carte::Couleur _couleur, 
    TypeAnnonce _type, 
    bool _atout, 
    CarteList &_cartes) : 
    m_Couleur(_couleur), m_Annonce(_type), m_IsTrump(_atout), m_Cartes(_cartes)
{
    //TODO computeScore();
}

Annonce::~Annonce()
{
}

void Annonce::ajouterCarte(Carte &_carte)
{
    m_Cartes.ajouter(&_carte);
    // increase the score
    //TODO computeScore();
}

void Annonce::supprimerCarte(Carte &_carte)
{
    m_Cartes.supprimer(&_carte);
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

bool Annonce::mariage(CarteList &_list)
{
    if ((_list.isContainsValeur(Carte::DAME)) ||
        (_list.isContainsValeur(Carte::ROI)))
    {
        return true;
    }
    return false;
}

bool Annonce::tierce(CarteList &_list)
{
    if ((_list.isContainsValeur(Carte::VALET)) ||
        (_list.isContainsValeur(Carte::DAME)) ||
        (_list.isContainsValeur(Carte::ROI)))
    {
        return true;
    }
    return false;
}

bool Annonce::quarante(CarteList &_list)
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

bool Annonce::quinte(CarteList &_list)
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

bool Annonce::chouine(CarteList &_list)
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

set<Annonce *> Annonce::annonces(
    CarteList &_list, 
    set<Annonce *> &_annoncesConnues, 
    bool *_isChouine)
{
    Annonce *ann;
    CarteList couleurList;
    CarteList cartes;
    set<Carte::Couleur> couleurToCheck;
    *_isChouine = false;

    set<Annonce *> ret;
    if (_list.size() <= 1)
    {
        return ret;
    }

    if (chouine(_list))
    {
        ann = new Annonce(_list[0]->couleur(),
                          TypeAnnonce::CHOUINE,
                          _list[0]->atout(),
                          _list);
        *_isChouine = true;
        ret.insert(ann);
    }

    if (quinte(_list))
    {
        // if quinte already announced, do not accept it
        bool quintePresent = false;
        for (Annonce *ca : _annoncesConnues)
        {
            if (ca->type() == TypeAnnonce::QUINTE)
            {
                quintePresent = true;
            }
        }
        if (!quintePresent)
        {
            ann = new Annonce(Carte::BACK, TypeAnnonce::QUINTE, false, _list);
            ret.insert(ann);
        }
        return ret;
    }

    for (Carte::Couleur couleur : couleurToCheck)
    {
        couleurList = _list.getCartesFromList(couleur);

        if (quarante(couleurList))
        {
            ann = new Annonce(couleur, TypeAnnonce::QUARANTE,
                              _list[0]->atout(), cartes);
            ret.insert(ann);
        }
        else if (tierce(couleurList))
        {
            ann = new Annonce(couleur, TypeAnnonce::TIERCE,
                              _list[0]->atout(), cartes);
            ret.insert(ann);
        }
        else if (mariage(couleurList))
        {
            ann = new Annonce(couleur, TypeAnnonce::MARIAGE,
                              _list[0]->atout(), cartes);
            ret.insert(ann);
        }
    }
    return ret;
}
