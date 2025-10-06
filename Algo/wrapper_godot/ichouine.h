#pragma once
#include <string>
#include "godot_cpp/classes/ref_counted.hpp"

class Chouine;

namespace godot {

class IChouine : public RefCounted {
	GDCLASS(IChouine, RefCounted)

private:
	Chouine *m_Chouine;

protected:
	static void _bind_methods();

public:
	IChouine();
	~IChouine();

	String Partie();
	void DistributionCartes();
	int GagnantPli();
	int PerdantPli();
	String ChoixJoueur();
	int SetChoixJoueur(String _carte);
	int FinPli();
	int FinPartie();
};

} // namespace godot