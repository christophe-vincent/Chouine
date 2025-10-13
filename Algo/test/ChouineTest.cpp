// ChouineTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <iomanip>
#include "Chouine.h"
#include "Joueur.h"


/*
0 -> 1: 69.0%
0 -> 2: 72.4%
0 -> 3: 80.4%
0 -> 4: 81.3%
0 -> 5: 82.5%

1 -> 2: 68.9%
2 -> 3: 63.8%
3 -> 4: 52.8%
4 -> 5: 53.0%
*/


unsigned int niveau1 = 5;
unsigned int niveau2 = 5;
int nbParties = 10000;

bool verbose = nbParties == 1 ? true : false;

void log() {}

template<typename First, typename ...Rest>
void log(First && first, Rest && ...rest)
{
    if (verbose)
    {
        std::cout << std::forward<First>(first);
        log(std::forward<Rest>(rest)...);
    }
}

std::string getCardcolor(int _id)
{
    std::string ret = "";
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

bool testChoix(const std::string& _choix, bool& _erreur)
{
    if ((_choix == "erreur") || (_choix == "E_E"))
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
    chouine.distribution_cartes();

    Joueur joueur1 = chouine.joueur(Chouine::JOUEUR_A);
    Joueur joueur2 = chouine.joueur(Chouine::JOUEUR_B);
    
    log("Atout : ", chouine.atout(), "\n");
    
    std::string choix;
    bool stop = false;
    int tour = 0;
    bool erreur = false;
    Chouine::JOUEUR gagnant = chouine.gagnantPli();
    Chouine::JOUEUR perdant = chouine.perdantPli();
    std::string annonce;
    std::string priseAtout;
    
    while (!stop)
    {
        tour++;
        log("\nTOUR ", tour, "\n");
        log("Pioche  : ", chouine.pioche().nomCartes(), "\n");

        choix = chouine.choixJoueur(annonce, priseAtout);
        log("Cartes joueur ", gagnant + 1, ": ", chouine.joueur(gagnant).cartesMainToStr(), "\n");
        stop = testChoix(choix, erreur);
        std::string message = "Choix Joueur " + std::to_string(gagnant + 1) + "  : " + choix;
        message += (annonce != "") ? " (" + annonce + ")" : "";
        log(message, "\n");

        annonce = "";
        choix = chouine.choixJoueur(annonce, priseAtout);
        log("Cartes joueur ", perdant + 1, ": ", chouine.joueur(perdant).cartesMainToStr(), "\n");
        stop |= testChoix(choix, erreur);
        message = "Choix Joueur " + std::to_string(perdant + 1) + "  : " + choix;
        message += (annonce != "") ? " (" + annonce + ")" : "";
        log(message, "\n");

        if (erreur)
        {
            std::cout << "Erreur" << std::endl;
            return 1;
        }
        if (!stop)
        {
            // affichage des annonces possibles
            std::string annonces = chouine.annoncesEnMainJoueur(0);
            if (annonces.size() > 0) log("Annonces joueur 1: " + annonces + "\n");
            annonces = chouine.annoncesEnMainJoueur(1);
            if (annonces.size() > 0) log("Annonces joueur 2: " + annonces + "\n");
            gagnant = chouine.finPli();
            perdant = chouine.perdantPli();
            stop = chouine.finPartie();
            log("Gagnant pli: ", chouine.gagnantPli() + 1, "\n");
        }
    }
    
    _pointsJoueur1 = chouine.pointsJoueur(Chouine::JOUEUR_A);
    _pointsJoueur2 = chouine.pointsJoueur(Chouine::JOUEUR_B);
    log("Cartes joueur 1 : ", joueur1.cartesGagneesToStr(), "\n");
    log("Cartes joueur 2 : ", joueur2.cartesGagneesToStr(), "\n");
    log("Points joueur 1 : ", _pointsJoueur1, "\n");
    log("Points joueur 2 : ", _pointsJoueur2, "\n");
    
    return 0;
}

class SimpleMultiThreadGame {
private:
    std::atomic<int> partiesJoueur1{0};
    std::atomic<int> partiesJoueur2{0};
    std::atomic<int> pointsJoueur1{0};
    std::atomic<int> pointsJoueur2{0};
    std::atomic<bool> shouldStop{false};

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
};

int main()
{
    std::cout << "Running " << nbParties << " games with multithreading..." << std::endl;
    
    SimpleMultiThreadGame engine;
    
    // Measure execution time
    auto start = std::chrono::high_resolution_clock::now();
    int result = engine.runGamesParallel(nbParties, niveau1, niveau2);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    if (result == 0) {
        std::cout << "=== RESULTS ===" << std::endl;
        std::cout << "Joueur 1 (level " << niveau1 << "): Parties " << engine.getPartiesJoueur1();
        std::cout << ", points " << engine.getPointsJoueur1() << std::endl;
        std::cout << "Joueur 2 (level " << niveau2 << "): Parties " << engine.getPartiesJoueur2();
        std::cout << ", points " << engine.getPointsJoueur2() << std::endl;
        
        // Calculate percentages
        double winRate1 = (static_cast<double>(engine.getPartiesJoueur1()) / nbParties) * 100.0;
        double winRate2 = (static_cast<double>(engine.getPartiesJoueur2()) / nbParties) * 100.0;
        
        std::cout << "Win rates: Joueur 1: " << std::fixed << std::setprecision(2) << winRate1 << "%, ";
        std::cout << "Joueur 2: " << winRate2 << "%" << std::endl;
        std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    } else {
        std::cout << "Error occurred: " << result << std::endl;
    }
    
    return result;
}