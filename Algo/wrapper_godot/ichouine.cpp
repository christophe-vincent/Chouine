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


int IChouine::GagnantPli() {
	return m_Chouine->gagnantPli();
}

int IChouine::PerdantPli() {
	return m_Chouine->perdantPli();
}

String IChouine::ChoixJoueur() {
	std::string annonce;
	return String(m_Chouine->choixJoueur(annonce).c_str());
}

int IChouine::SetChoixJoueur(String _carte) {
 	return m_Chouine->setChoixJoueur(std::string(_carte.utf8().get_data()));
}

int IChouine::FinPli()
{
	return m_Chouine->finPli();
}

int IChouine::FinPartie()
{
	return m_Chouine->finPartie();
}