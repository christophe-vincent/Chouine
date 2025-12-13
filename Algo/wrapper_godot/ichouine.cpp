#include "ichouine.h"
#include "Chouine.h"
#include "ListeCartes.h"
#include "godot_cpp/core/class_db.hpp"

using namespace godot;

void IChouine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("partie"), &IChouine::Partie);
    ClassDB::bind_method(D_METHOD("distribution_cartes"), &IChouine::DistributionCartes);
    ClassDB::bind_method(D_METHOD("trier_cartes"), &IChouine::TrierCartes);
    ClassDB::bind_method(D_METHOD("gagnant_pli"), &IChouine::GagnantPli);
    ClassDB::bind_method(D_METHOD("perdant_pli"), &IChouine::PerdantPli);
    ClassDB::bind_method(D_METHOD("choix_joueur"), &IChouine::ChoixJoueur);
    ClassDB::bind_method(D_METHOD("set_choix_joueur"), &IChouine::SetChoixJoueur);
    ClassDB::bind_method(D_METHOD("fin_pli"), &IChouine::FinPli);
    ClassDB::bind_method(D_METHOD("fin_partie"), &IChouine::FinPartie);
    ClassDB::bind_method(D_METHOD("cartes_joueur"), &IChouine::CartesJoueur);
    ClassDB::bind_method(D_METHOD("points_joueur"), &IChouine::PointsJoueur);
    ClassDB::bind_method(D_METHOD("points_joueur_str"), &IChouine::PointsJoueurStr);
    ClassDB::bind_method(D_METHOD("annonces_en_main_joueur"), &IChouine::AnnoncesEnMainJoueur);
    ClassDB::bind_method(D_METHOD("annonce_joueur"), &IChouine::AnnonceJoueur);
    ClassDB::bind_method(D_METHOD("sept_atout_en_main"), &IChouine::SeptAtoutEnMain);
    ClassDB::bind_method(D_METHOD("echanger_carte_atout"), &IChouine::EchangerCarteAtout);
    ClassDB::bind_method(D_METHOD("chouine"), &IChouine::ChouineJoueur);
}

IChouine::IChouine(): m_Chouine(new Chouine(6, 6))
{
}

IChouine::~IChouine()
{
    delete m_Chouine;
}

std::vector<std::string> vecteur_quinte = {
    "10-coeur", "roi-trefle", "as-coeur", "dame-trefle", "10-pic", "roi-coeur", "as-pic", "dame-coeur", "10-trefle"};
std::vector<std::string> vecteur_chouine = {
    "roi-coeur", "roi-trefle",
    "dame-coeur", "dame-trefle",
    "7-trefle", "7-coeur",
    "8-pic", "8-coeur",
    "9-trefle", "9-pic",
    "9-coeur",
    "valet-pic", "7-pic",
    "dame-pic", "7-carreau",
    "roi-pic", "8-trefle",
    "10-pic", "8-carreau",
    "as-pic"};


String IChouine::Partie()
{
    printf("Nouvelle partie\n");
    //m_Chouine->setOrdreCartes(vecteur_quinte);
    //m_Chouine->setOrdreCartes(vecteur_chouine);
    m_Chouine->newGame();
    UtilityFunctions::print(m_Chouine->pioche().nomCartes().c_str());
    return m_Chouine->pioche().nomCartes().c_str();
}

String IChouine::Atout()
{
    return String (m_Chouine->atout().c_str());
}

void IChouine::DistributionCartes()
{
    m_Chouine->distribution_cartes();
}

void IChouine::TrierCartes(int _joueur)
{
    m_Chouine->trierCartes(_joueur);
}

String IChouine::CartesJoueur(int _joueur)
{
    std::string cartes = m_Chouine->joueur((Chouine::JOUEUR)_joueur).cartesMainToStr();
    UtilityFunctions::print(cartes.c_str());
    return String(cartes.c_str());
}

int IChouine::GagnantPli()
{
    return m_Chouine->gagnantPli();
}

int IChouine::PerdantPli()
{
    return m_Chouine->perdantPli();
}

String IChouine::ChoixJoueur() {
    std::string annonce = "";
    std::string carteAtout = "";
    std::string ret = m_Chouine->choixJoueur(annonce, carteAtout);
    //UtilityFunctions::print(annonce.c_str());
    ret += "|" + annonce + "|" + carteAtout;
    return String(ret.c_str());
}

int IChouine::SetChoixJoueur(String _carte) {
    return m_Chouine->setChoixJoueur(std::string(_carte.utf8().get_data()));
}

int IChouine::AnnonceJoueur(String _annonce)
{
    UtilityFunctions::print(_annonce.utf8().get_data());
    return m_Chouine->setChoixAnnonce(1, std::string(_annonce.utf8().get_data()));
}

String IChouine::AnnoncesEnMainJoueur(int _joueur)
{
    return String(m_Chouine->annoncesEnMainJoueur(_joueur).c_str());
}

bool IChouine::SeptAtoutEnMain(int _joueur)
{
    return m_Chouine->septAtoutEnMain((Chouine::JOUEUR)_joueur);
}

bool IChouine::EchangerCarteAtout(int _joueur)
{
    return m_Chouine->echangerCarteAtout((Chouine::JOUEUR)_joueur);
}

int IChouine::FinPli()
{
    return m_Chouine->finPli();
}

int IChouine::FinPartie()
{
    return m_Chouine->finPartie();
}

int IChouine::PointsJoueur(int _joueur)
{
    return m_Chouine->pointsJoueur((Chouine::JOUEUR)_joueur);
}

String IChouine::PointsJoueurStr(int _joueur)
{
    return String(m_Chouine->pointsJoueurStr((Chouine::JOUEUR)_joueur).c_str());
}

bool IChouine::ChouineJoueur(int _joueur)
{
    return m_Chouine->chouine((Chouine::JOUEUR)_joueur);
}
