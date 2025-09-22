// ChouineTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include "Chouine.h"
#include "Joueur.h"
using namespace std;

bool verbose = false;
void log(){}

template<typename First, typename ...Rest>
void log(First && first, Rest && ...rest)
{
    if (verbose)
    {
        std::cout << std::forward<First>(first);
        log(std::forward<Rest>(rest)...);
    }
}


string getCardcolor(int _id)
{
    string ret = "";
    switch (_id >> 3)
    {
    case 0:
        ret = "Coeur";
        break;
    case 1:
        ret = "Pic";
        break;
    case 2:
        ret = "Trefle";
        break;
    case 3:
        ret = "Carreau";
        break;
    default:
        ret = "???";
    }
    return ret;
}

bool testChoix(string _choix, bool& _erreur)
{
    if ( (_choix == "erreur") || (_choix == "E_E"))
    {
        _erreur = true;
    }
    if (_choix == "fin")
    {
        return true;
    }
    return false;
}


int partie(unsigned int _niveauJoueur1,
            unsigned int _niveauJoueur2, 
            int& _pointsJoueur1, 
            int& _pointsJoueur2)
{
    Chouine chouine(_niveauJoueur1, _niveauJoueur2);

    chouine.newGame();

    Joueur joueur1 = chouine.joueur(Chouine::JOUEUR_A);
    Joueur joueur2 = chouine.joueur(Chouine::JOUEUR_B);
    
    log("Atout : ", chouine.atout(), "\n");
    
    string choix;
    bool stop = false;
    int tour = 0;
    bool erreur = false;
    Chouine::JOUEUR gagnant = chouine.gagnantPli();
    Chouine::JOUEUR perdant = chouine.perdantPli();
    string annonce;
    
    while (! stop)
    {
        tour ++;
        log("\nTOUR ", tour, "\n");
        log("Pioche  : ", chouine.pioche().nomCartes(), "\n");
        log("Joueur ", gagnant + 1, ": ", chouine.joueur(gagnant).cartesMainToStr(), "\n");
        log("Joueur ", perdant + 1, ": ", chouine.joueur(perdant).cartesMainToStr(), "\n");
        
        choix = chouine.choixJoueur(annonce);
        stop = testChoix(choix, erreur);
        string message = "Choix Joueur " +  to_string(gagnant + 1) + " : " + choix;
        message += (annonce != "") ? " (" + annonce + ")" : "";
        log(message, "\n");

        annonce = "";
        choix = chouine.choixJoueur(annonce);
        stop |= testChoix(choix, erreur);
        message = "Choix Joueur " +  to_string(perdant + 1) + " : " + choix;
        message += (annonce != "") ? " (" + annonce + ")" : "";
        log(message, "\n");

        if (erreur)
        {
            cout << "Erreur" << endl;
            return 1;
        }
        if (! stop)
        {
            gagnant = chouine.finPli();
            perdant = chouine.perdantPli();
            stop = chouine.finPartie();
            log("Gagnant pli: ", chouine.gagnantPli() + 1, "\n");
            //log("Points joueur 1 : ", _pointsJoueur1, "\n");
            //log("Points joueur 2 : ", _pointsJoueur2, "\n");
        }
        //stop = true;
        //if (tour > 0) stop = true;
    }
    _pointsJoueur1 = chouine.pointsJoueur(Chouine::JOUEUR_A);
    _pointsJoueur2 = chouine.pointsJoueur(Chouine::JOUEUR_B);
    log("Cartes joueur 1 : ", joueur1.cartesGagneesToStr(), "\n");
    log("Cartes joueur 2 : ", joueur2.cartesGagneesToStr(), "\n");
    log("Points joueur 1 : ", _pointsJoueur1, "\n");
    log("Points joueur 2 : ", _pointsJoueur2, "\n");
    if (_pointsJoueur1 > _pointsJoueur2) {
        //cout << "Joueur 1 gagne " << _pointsJoueur1 << " - " << _pointsJoueur2 << endl;
    }
    else {
        //cout << "Joueur 2 gagne " << _pointsJoueur2 << " - " << _pointsJoueur1 << endl;
    }
    return 0;
}

class SimpleMultiThreadGame {
private:
    std::atomic<int> partiesJoueur1{0};
    std::atomic<int> partiesJoueur2{0};
    std::atomic<int> pointsJoueur1{0};
    std::atomic<int> pointsJoueur2{0};
    std::atomic<bool> shouldStop{false};
    std::mutex resultsMutex;

public:
    int runGamesParallel(int nbParties, int niveau1, int niveau2) {
        const int numThreads = std::min(static_cast<int>(std::thread::hardware_concurrency()), nbParties);
        const int gamesPerThread = nbParties / numThreads;
        const int extraGames = nbParties % numThreads;
        
        std::vector<std::thread> threads;
        std::atomic<int> globalError{0};
        
        // Reset state
        partiesJoueur1 = 0;
        partiesJoueur2 = 0;
        pointsJoueur1 = 0;
        pointsJoueur2 = 0;
        shouldStop = false;
        
        // Launch threads
        for (int t = 0; t < numThreads; ++t) {
            int startGame = t * gamesPerThread;
            int endGame = startGame + gamesPerThread + (t < extraGames ? 1 : 0);
            
            threads.emplace_back([this, startGame, endGame, niveau1, niveau2, &globalError]() {
                workerFunction(startGame, endGame, niveau1, niveau2, globalError);
            });
        }
        
        // Wait for all threads
        for (auto& thread : threads) {
            thread.join();
        }
        
        return globalError;
    }
    
    // Getters for results
    int getPartiesJoueur1() const { return partiesJoueur1; }
    int getPartiesJoueur2() const { return partiesJoueur2; }
    int getPointsJoueur1() const { return pointsJoueur1; }
    int getPointsJoueur2() const { return pointsJoueur2; }

private:
    void workerFunction(int startGame, int endGame, int niveau1, int niveau2, std::atomic<int>& globalError) {
        for (int i = startGame; i < endGame; ++i) {
            // Check for early termination
            if (shouldStop || globalError != 0) {
                break;
            }
            
            int points1 = 0, points2 = 0;
            int ret = partie(niveau1, niveau2, points1, points2);
            
            if (ret != 0) {
                globalError = ret;
                shouldStop = true;
                break;
            }
            
            // Update counters atomically
            if (points1 > points2) {
                partiesJoueur1++;
            } else {
                partiesJoueur2++;
            }
            
            pointsJoueur1 += points1;
            pointsJoueur2 += points2;
        }
    }
    
    int partie(int niveau1, int niveau2, int& points1, int& points2);
};


int main()
{
    unsigned int niveau1 = 3;
    unsigned int niveau2 = 5;
    int points1;
    int points2;
    int partiesJoueur1 = 0;
    int partiesJoueur2 = 0;
    int pointsJoueur1 = 0;
    int pointsJoueur2 = 0;
    int nbParties = 100000;
    int ret;

    SimpleMultiThreadGame engine;
    
    int result = engine.runGamesParallel(100000, 3, 5);
    
    if (result == 0) {
        std::cout << "Player 1 wins: " << engine.getPartiesJoueur1() << std::endl;
        std::cout << "Player 2 wins: " << engine.getPartiesJoueur2() << std::endl;
        std::cout << "Player 1 total points: " << engine.getPointsJoueur1() << std::endl;
        std::cout << "Player 2 total points: " << engine.getPointsJoueur2() << std::endl;
    } else {
        std::cout << "Error occurred: " << result << std::endl;
    }
    return 0;

    for(int i=0; i<nbParties; i++)
    {
        ret = partie(niveau1, niveau2, points1, points2);
        if (ret != 0)
        {
            break;
        }
        points1 > points2 ? partiesJoueur1++ : partiesJoueur2++;
        pointsJoueur1 += points1;
        pointsJoueur2 += points2;
    }
    cout << "Joueur 1 (level " << niveau1 << "): Parties " << partiesJoueur1;
    cout << " points " << pointsJoueur1 << endl;
    cout << "Joueur 2 (level " << niveau2 << "): Parties " << partiesJoueur2;
    cout << " points " << pointsJoueur2 << endl;

    return ret;
}
