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

        String CartesJoueur(int _joueur);
        String Partie();
        String Atout();
        void DistributionCartes();
        int GagnantPli();
        int PerdantPli();
        String ChoixJoueur();
        int SetChoixJoueur(String _carte);
        int AnnonceJoueur(String _annonce);
        String AnnoncesEnMainJoueur(int _joueur);
        bool SeptAtoutEnMain(int _joueur);
        bool EchangerCarteAtout(int _joueur);
        int FinPli();
        int FinPartie();
        int PointsJoueur(int _joueur);
        String PointsJoueurStr(int _joueur);
    };
} // namespace godot