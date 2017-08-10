
#pragma once

#include "random"

#include <Imagine/Graphics.h>
using namespace Imagine;

const double INF = 200000.;
const int DEPTH=3;

const int WindowWidth = 800;
const int WindowHeight = 800;
const int SquareSize = 80;
const int Radius = 30;
const int Margin = (WindowWidth-7*SquareSize)/2;


const Color blueDark(0,0,100) ;

#include "Grid.h"

//Player class
class Player{
public:
    string name;
    double score;
    int id;
    Player(){name="Joueur"; score=0.; id=0;};
    Player& operator=(const Player& P)
        {name = P.name; score = P.score; id = P.id;
        return *this;}
    Player(string NAME, double SCORE, int ID) {name=NAME; score=SCORE; id=ID;};
    string nameScore()
    {   stringstream ss;
        ss << name << ": " << score;
        return ss.str();
    };
};

//Game class making up the game engine
class Game{
    Player* players = new Player[2];
    Grid grid;
    bool playWithAI = true;
    int currentPlayer = 1;
    vector<int> moves;
public:
    Game(){players[0].id = 1; players[1].name = "Ordinateur"; players[1].id = 2;};
    Game(bool AI){players[0].id = 1; players[1].name = "Ordinateur"; players[1].id = 2; playWithAI = AI;};
    Game(const Game& G);
    ~Game();
    string movesString();
    Game& operator=(const Game& G);
    double winnerGain(){if (moves.empty()) return 3*INF; else return INF*(1+1/((moves.size()+1)/2));};
    bool playMove(int j, double a=1., double b=10., double c=100.);
    void cancelMove(int j, bool updateScore=true, double score1=0., double score2=0.);
    bool gameOver(){return grid.hasWinner().second;};
    int winner() {grid.hasWinner().first;};
    pair<double,int> minimax(int depth, bool isMax, double a=1., double b=10., double c=100.);
    pair<double,int> alphabeta(int depth, bool isMax, double alpha, double beta, double a=1., double b=10., double c=100.);
    void drawColumn(int j);
    void highlightColumn(int j);
    void show(bool someonesTurn = true);
    void showResults();

    void play(double a=1., double b=10., double c=100., int currentplayer = 1);
    double playAIvsAI(double a, double b, double c, double d, double e, double f, int depth1 = DEPTH, int depth2 = DEPTH);
    };

