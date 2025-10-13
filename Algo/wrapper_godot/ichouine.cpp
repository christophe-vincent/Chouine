#include "ichouine.h"
#include "Chouine.h"
#include "ListeCartes.h"
#include "godot_cpp/core/class_db.hpp"

using namespace godot;

void IChouine::_bind_methods() {
	ClassDB::bind_method(D_METHOD("partie"), &IChouine::Partie);
	ClassDB::bind_method(D_METHOD("distribution_cartes"), &IChouine::DistributionCartes);
	ClassDB::bind_method(D_METHOD("gagnant_pli"), &IChouine::GagnantPli);
	ClassDB::bind_method(D_METHOD("perdant_pli"), &IChouine::PerdantPli);
	ClassDB::bind_method(D_METHOD("choix_joueur"), &IChouine::ChoixJoueur);
	ClassDB::bind_method(D_METHOD("set_choix_joueur"), &IChouine::SetChoixJoueur);
	ClassDB::bind_method(D_METHOD("fin_pli"), &IChouine::FinPli);
	ClassDB::bind_method(D_METHOD("fin_partie"), &IChouine::FinPartie);
	ClassDB::bind_method(D_METHOD("cartes_joueur"), &IChouine::CartesJoueur);
	ClassDB::bind_method(D_METHOD("points_joueur"), &IChouine::PointsJoueur);
	ClassDB::bind_method(D_METHOD("annonces_en_main_joueur"), &IChouine::AnnoncesEnMainJoueur);
	ClassDB::bind_method(D_METHOD("annonce_joueur"), &IChouine::AnnonceJoueur);
}

IChouine::IChouine(): m_Chouine(new Chouine(5, 5)) 
{
}

IChouine::~IChouine() {
	delete m_Chouine;
}

String IChouine::Partie() {
	printf("Nouvelle partie\n");
	m_Chouine->newGame();
	return m_Chouine->pioche().nomCartes().c_str();
}

void IChouine::DistributionCartes()
{
	m_Chouine->distribution_cartes();
}

String IChouine::CartesJoueur(int _joueur) {
	std::string cartes = m_Chouine->joueur((Chouine::JOUEUR)_joueur).cartesMainToStr();
	UtilityFunctions::print(cartes.c_str());
	return String(cartes.c_str());
}

int IChouine::GagnantPli() {
	return m_Chouine->gagnantPli();
}

int IChouine::PerdantPli() {
	return m_Chouine->perdantPli();
}

String IChouine::ChoixJoueur() {
	std::string annonce = "";
	std::string carteAtout = "";
	std::string ret = m_Chouine->choixJoueur(annonce, carteAtout);
	UtilityFunctions::print(annonce.c_str());
	ret += "|" + annonce + "|" + carteAtout;
	return String(ret.c_str());
}

int IChouine::SetChoixJoueur(String _carte) {
 	return m_Chouine->setChoixJoueur(std::string(_carte.utf8().get_data()));
}

int IChouine::AnnonceJoueur(String _annonce)
{
	return m_Chouine->setChoixAnnonce(1, std::string(_annonce.utf8().get_data()));
}

String IChouine::AnnoncesEnMainJoueur(int _joueur)
{
	return String(m_Chouine->annoncesEnMainJoueur(_joueur).c_str());
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
