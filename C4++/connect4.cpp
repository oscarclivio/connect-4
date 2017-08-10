#include <time.h>
#include <cmath>
#include<limits>
using namespace std;

#include "Genome.h"


//Play random computer-vs-computer games and compare results obtained Alpha-Beta and Mini-Max algorithms
//(Same winner, different times)
void playRandom(int nbGames, int Depth){
    srand (time(NULL));
    Game Gmm, Gab;
    int nbMatches = 0;
    int randomMove = 0;
    double timeMiniMax = 0.;
    double timeAlphaBeta = 0.;
    string equals[2] = {"UNEQUAL", "equal"};
    for (int game=0; game<nbGames; game++){
        Gmm = Game();
        Gab = Game();
        clock_t clock1 = clock();
        pair<double,int> pairMinimax = Gmm.minimax(Depth,false);
        clock_t clock2 = clock();
        pair<double,int> pairAlphaBeta = Gab.alphabeta(Depth,false,-Gab.winnerGain(),Gab.winnerGain());
        clock_t clock3 = clock();
        timeMiniMax += (double)(clock2-clock1);
        timeAlphaBeta += (double)(clock3-clock2);
        Gmm.playMove(pairMinimax.second);
        Gab.playMove(pairAlphaBeta.second);
        while (true){
            while (!Gmm.playMove(randomMove))
                randomMove = 1 + rand()%7;
            if (!Gab.playMove(randomMove))
            {
                cout << Gmm.movesString() << " " << Gab.movesString() << " " << equals[(int)false] << endl;
                break;
            }
            if (Gmm.gameOver() || Gab.gameOver())
            {
                int equal = (int)(Gmm.movesString() == Gab.movesString());
                nbMatches += equal;
                cout << Gmm.movesString() << " " << Gab.movesString() << " "
                     << Gmm.winner() << " " << Gab.winner() << " " << equals[equal] << endl;
                break;
            }

        }

        cout << "Iteration " << game << endl;
    }
    timeMiniMax /= (nbGames*CLOCKS_PER_SEC);
    timeAlphaBeta /= (nbGames*CLOCKS_PER_SEC);
    cout << nbMatches << " matches out of " << nbGames << "are OK" << endl;
    cout << "Total time Mini-Max: " << timeMiniMax << endl;
    cout << "Total time Alpha-Beta: " << timeAlphaBeta << endl;
}

//Test a genome (the one who has been selected)
void testGenome(const Genome& winner){

    cout << "WINNER: " << winner.toString() << endl;

    Genome oldGuy = Genome(1,10,100);
    Genome oldGuy2 = Genome(10,100,1000);

    winner.winsAgainst(oldGuy);
    winner.winsAgainst(oldGuy2);

    Population Ptest(250);
    Ptest.test(winner);

}


int main(){
    srand (time(0));

    //Choose between "Game" and "Genetic algorithm" sections

    // Game //

    Game G;
    G.play(15,244,953);


    // Genetic algorithm //
/*
    Population P(16);
    P.simulate(100);

    Population P2(16);
    P2.simulate(100);

    Population P3(16);
    P3.simulate(100);
*/
  //  testGenome(Genome(15,244,953));

    //testGenome(Genome(17,88,444));
    //testGenome(Genome(17,90,656));
    //testGenome(Genome(15,77,603));
    //testGenome(Genome(36,197,732));


    //Genome winner(15,244,953);
    //Genome oldGuy = Genome(1,10,100);

    //winner.winsAgainst(oldGuy);


    /*
    testGenome(Genome(1,10,100));
    testGenome(Genome(10,100,1000));
    //testGenome(Genome(178,920,510));
    testGenome(Genome(178,920,508));
    testGenome(Genome(34,156,945));
    testGenome(Genome(50,923,510));
    testGenome(Genome(50,904,946));
    testGenome(Genome(50,922,510));
    testGenome(Genome(227,714,13));
    testGenome(Genome(227,712,13));
    testGenome(Genome(235,718,13));
    testGenome(Genome(195,834,637));
    testGenome(Genome(179,834,125));

*/
    return 0;
}
