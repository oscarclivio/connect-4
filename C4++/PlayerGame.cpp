
#include "PlayerGame.h"

//Useful for the genetic algorithm
double gain(int x){
    if (x == 1)
        return 1.;
    return 1.-(x-1.)/20.;
}

Game::Game(const Game &G)
{
    grid = G.grid;
    players[0] = G.players[0];
    players[1] = G.players[1];
    playWithAI = G.playWithAI;
    currentPlayer = G.currentPlayer;
    moves = G.moves;
}


Game& Game::operator=(const Game& G){
    grid = G.grid;
    players[0] = G.players[0];
    players[1] = G.players[1];
    playWithAI = G.playWithAI;
    currentPlayer = G.currentPlayer;
    moves = G.moves;
    return *this;
}

Game::~Game(){
    delete[] players;
}

//Implement the history of moves
string Game::movesString(){
    stringstream ss;
    vector<int> V2(moves);
    while (!V2.empty()){
        int k = V2.back();
        V2.pop_back();
        ss << k;
    }
    return ss.str();
}

//Move
bool Game::playMove(int j, double a, double b, double c){
    if (!grid.insertToken(j,currentPlayer))
        return false;
    else{
        moves.push_back(j);
        pair<double,double> scorePair = grid.evaluate(a, b, c);
        players[0].score=scorePair.first;
        players[1].score=scorePair.second;


        currentPlayer = 3 - currentPlayer;
        pair<int,bool> win = grid.hasWinner();
        if (win.second){
            if (win.first == 1){
                players[0].score=winnerGain();
                players[1].score=-winnerGain();
            }
            else {
                players[0].score=-winnerGain();
                players[1].score=winnerGain();
            }
        }
        return true;
    }
}

//Cancel a move (useful for Alpha-Beta / Mini-Max)
void Game::cancelMove(int j, bool updateScore, double score1, double score2){
    int i = 0;
    while (i<6 and grid(i,j-1) == 0)
                i += 1;
    if (i<6)
        grid(i,j-1) = 0;
    moves.pop_back();
    currentPlayer = 3 - currentPlayer;
    if (updateScore){
        players[0].score=score1;
        players[1].score=score2;
    }
}

//Mini-Max algorithm
pair<double,int> Game::minimax(int depth, bool isMax, double a, double b, double c){
    if (gameOver()){
        if (players[0].score>players[1].score)
            return pair<double,int>(-winnerGain(),-1);
        else if (players[1].score>players[0].score)
            return pair<double,int>(winnerGain(),-1);
        else return pair<double,int>(0., -1);
        }
    else if (depth==0)
            return pair<double,int>(players[1].score-players[0].score,-1);

    else{
        if (isMax){
            double value = -2*winnerGain();
            int move = -1;
            for (int j=1; j<=7; j++){
                if (grid.isLegalMove(j)){
                    double score1 = players[0].score;
                    double score2 = players[1].score;
                    playMove(j,a,b,c);
                    pair<double,int> Pair = minimax(depth-1,false,a,b,c);
                    cancelMove(j,true,score1,score2);
                    if (Pair.first > value){
                        value = Pair.first;
                        move = j;
                        if (value == winnerGain())
                            return pair<double,int>(value,move);
                    } 
                }
            }
            return pair<double,int>(value,move);
        }
        else{
            double value = 2*winnerGain();
            int move = -1;
            for (int j=1; j<=7; j++){
                if (grid.isLegalMove(j)){
                    double score1 = players[0].score;
                    double score2 = players[1].score;
                    playMove(j,a,b,c);
                    pair<double,int> Pair = minimax(depth-1,true,a,b,c);
                    cancelMove(j,true,score1,score2);
                    if (Pair.first < value){
                        value = Pair.first;
                        move = j;
                        if (Pair.first == -winnerGain())
                            return pair<double,int>(value,move);
                    }
                }
            }
            return pair<double,int>(value,move);

        }
    }
}

//Alpha-Beta algorithm
pair<double,int> Game::alphabeta(int depth, bool isMax, double alpha, double beta, double a, double b, double c){
    if (gameOver()){
        if (players[0].score>players[1].score)
            return pair<double,int>(-winnerGain(),-1);
        else if (players[1].score>players[0].score)
            return pair<double,int>(winnerGain(),-1);
        else
            return pair<double,int>(0.,-1);
    }
    else if (depth==0)
            return pair<double,int>(players[1].score-players[0].score,-1);

    else{
        if (isMax){
            double value = -2*winnerGain();
            int move = -1;
            for (int j=1; j<=7; j++){
                if (grid.isLegalMove(j)){
                    double score1 = players[0].score;
                    double score2 = players[1].score;
                    playMove(j,a,b,c);
                    pair<double,int> Pair = alphabeta(depth-1,false,alpha,beta,a,b,c);
                    cancelMove(j,true,score1,score2);
                    if (Pair.first > value){
                        value = Pair.first;
                        move = j;}
                    if (value > alpha)
                        alpha = value;
                    if (beta <= alpha)
                        return pair<double,int>(value,move);
                }
            }
            return pair<double,int>(value,move);
        }
        else{
            double value = 2*winnerGain();
            int move = -1;
            for (int j=1; j<=7; j++){
                if (grid.isLegalMove(j)){
                    double score1 = players[0].score;
                    double score2 = players[1].score;
                    playMove(j,a,b,c);
                    pair<double,int> Pair = alphabeta(depth-1,true,alpha,beta,a,b,c);
                    cancelMove(j,true,score1,score2);
                    if (Pair.first < value){
                        value = Pair.first;
                        move = j;
                        if (Pair.first == -winnerGain())
                            return pair<double,int>(value,move);
                    }
                    if (beta > value)
                        beta = value;
                    if (alpha >= beta)
                        return pair<double,int>(value,move);
                }
            }
            return pair<double,int>(value,move);
        }
    }
}

//Draw a cell column
void Game::drawColumn(int j){
    int X = Margin + (j*SquareSize + SquareSize/2);
    Color colors[3] = {WHITE, RED, YELLOW};
    for (int i=0; i<6; i++){
        int Y = WindowHeight - Margin - ((11*SquareSize)/2-i*SquareSize);
        fillCircle(X,Y,Radius,colors[grid(i,j)]);
    }
}

//Show the game
void Game::show(bool someonesTurn){
    fillRect(Margin, WindowHeight-Margin-6*SquareSize,
             7*SquareSize,6*SquareSize,blueDark);
    for (int j=0; j<7; j++)
        drawColumn(j);
    fillRect(0, 0, WindowWidth, WindowHeight-Margin-6*SquareSize-2,BLACK);
    drawString(Margin, 30, players[0].nameScore(),WHITE,15);
    drawString(Margin, 70, players[1].nameScore(),WHITE,15);
    if (someonesTurn){
        stringstream ss;
        ss << players[0].name << " doit jouer.";
        drawString(Margin, 110, ss.str(),WHITE,15);
    }
}

//Show who won the game
void Game::showResults(){
    pair<int,bool> Pair = grid.hasWinner();
    fillRect(0,0,WindowWidth,WindowHeight-Margin-6*SquareSize-2,BLACK);
    stringstream ss2;
    if (Pair.first != 0)
        ss2 << players[Pair.second].name << " a gagné !";
    else
        ss2 << "Match nul...";
    drawString(Margin, 110, ss2.str(),WHITE,15);
}

//Play to the game
void Game::play(double a, double b, double c, int firstplayer){
    openWindow(WindowWidth, WindowHeight);
    int Depth = DEPTH;
    bool playersTurn = true;
    bool keepGoing = true;
    currentPlayer = firstplayer;
    if (playWithAI && currentPlayer == 2){
        playersTurn = false;
        //Choose between Alpha-Beta and Mini-Max
        pair<double,int> pairMinimax = alphabeta(Depth,true,-winnerGain(),winnerGain(), a, b, c);
        //pair<double,int> pairMinimax = minimax(Depth,true);
        playMove(pairMinimax.second, a, b, c);
        show();
        playersTurn = true;
    }
    int x,y; //x,y mouse
    fillRect(0,0,WindowWidth,WindowHeight,BLACK);
    show();
    Event ev;
    while (keepGoing){
        getEvent(-1,ev);
        switch (ev.type){
        case EVT_KEY_ON:
            if (ev.key == 'q'){
                keepGoing = false;
                endGraphics();
            }
            break;
        case EVT_BUT_ON:
            if (playersTurn){
            int j=0;
            while (j<8 && Margin + j*SquareSize < ev.pix.x())
                j++;
            if (currentPlayer == 1 && playMove(j)){
                show(false);
            }
            if (playWithAI){
                playersTurn = false;
                pair<double,int> pairMinimax = alphabeta(Depth,true,-winnerGain(),winnerGain(), a, b, c);
                //pair<double,int> pairMinimax = minimax(Depth,true);
                playMove(pairMinimax.second, a, b, c);
                show();
                playersTurn = true;
            }
            if (gameOver()){
                show();
                showResults();
                playersTurn = false;
            }
            }
        }
    }
}

double Game::playAIvsAI(double a, double b, double c, double d, double e, double f, int depth1, int depth2){
        while (!gameOver()){
            playMove(alphabeta(depth1,false,-2*winnerGain(),2*winnerGain(),a,b,c).second, a, b, c);
            playMove(alphabeta(depth2,true,-2*winnerGain(),2*winnerGain(),d,e,f).second, d, e, f);
            //playMove(minimax(depth1,false,a,b,c).second, a, b, c);
            //playMove(minimax(depth2,true,d,e,f).second, d, e, f);
        }
        double sign[3]={0.,1.,-1.};

        return sign[winner()]*gain((moves.size()+1)/2);
    }


