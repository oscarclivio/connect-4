
#include "Grid.h"


Quadr& Quadr::operator=(const Quadr& Q)
{ints[0]=Q.ints[0]; ints[1]=Q.ints[1]; ints[2]=Q.ints[2]; ints[3]=Q.ints[3];
return *this;}

Grid::Grid(const Grid& G){
    for (int k=0; k<42; k++)
        elements[k] = G.elements[k];
}

Grid& Grid::operator=(const Grid& G){
    for (int k=0; k<42; k++)
        elements[k] = G.elements[k];
    return *this;
}

Grid::~Grid(){
    delete[] elements;
}

//Insert a token in the grid, if permitted
bool Grid::insertToken(int j, int value){
    if (!isLegalMove(j))
        return false;
    else{
        int i = 1;
        while (i<6 and read(i,j-1) == 0)
                    i += 1;
        elements[7*(i-1)+(j-1)] = value;
        return true;
    }
}

//Return x=1,2 if player x has won the game, otherwise return 0
int Grid::winner(){
    for (int j=0; j<7; j++)
               for (int i=5; i>=0; i--){
                   if(read(i,j)==0)
                       break;
                   else{
                       int PotentialWinner =  read(i,j);
                       bool GoUp = (i >= 3);
                       bool GoDown = (i <= 2);
                       bool GoLeft = (j >= 3);
                       bool GoRight = (j <= 3);
                       if (GoUp){
                           if ((read(i-1,j) == PotentialWinner
                               and read(i-2,j) == PotentialWinner
                               and read(i-3,j) == PotentialWinner)
                           || (GoLeft and read(i-1,j-1) == PotentialWinner
                               and read(i-2,j-2) == PotentialWinner
                               and read(i-3,j-3) == PotentialWinner)
                           || (GoRight and read(i-1,j+1) == PotentialWinner
                               and read(i-2,j+2) == PotentialWinner
                               and read(i-3,j+3) == PotentialWinner))
                               return PotentialWinner;
                       }
                       if (GoDown){
                           if ((GoLeft and read(i+1,j-1) == PotentialWinner
                               and read(i+2,j-2) == PotentialWinner
                               and read(i+3,j-3) == PotentialWinner)
                           || (GoRight and read(i+1,j+1) == PotentialWinner
                               and read(i+2,j+2) == PotentialWinner
                               and read(i+3,j+3) == PotentialWinner))
                               return PotentialWinner;
                       }
                       if (GoLeft){
                           if ((read(i,j-1) == PotentialWinner
                               and read(i,j-2) == PotentialWinner
                               and read(i,j-3) == PotentialWinner))
                               return PotentialWinner;
                       }
                       if (GoRight){
                           if ((read(i,j+1) == PotentialWinner
                               and read(i,j+2) == PotentialWinner
                               and read(i,j+3) == PotentialWinner))
                               return PotentialWinner;
                       }
               }
               }
           return 0;
}

//Check if the game has got a winner
pair<int,bool> Grid::hasWinner(){
    int theWinner = winner();
    if (theWinner>0)
        return pair<int,bool>(theWinner,true);
    else{
        bool NotPlayable = true;
        int j = 1;
        while (j <= 7 and NotPlayable){
             NotPlayable = not(isLegalMove(j));
             j += 1;
        }
        return pair<int,bool>(0, NotPlayable);
    }
}

//Evaluate a position (useful for Alpha-Beta / Mini-Max algorithms)
pair<double,double> Grid::evaluate(double a, double b, double c){
    vector<Quadr> slot;
    for (int i=0; i<6; i++)
        for (int j=0; j<7; j++){
            if(i>=0 && i+3<6)
                slot.push_back(Quadr(read(i,j),read(i+1,j),read(i+2,j),read(i+3,j)));
        if(j>=0 && j+3<7)
                slot.push_back(Quadr(read(i,j),read(i,j+1),read(i,j+2),read(i,j+3)));
        if(i-3>=0 && i<6 && j>=0 && j+3<7)
                slot.push_back(Quadr(read(i,j),read(i-1,j+1),read(i-2,j+2),read(i-3,j+3)));
        if(i>=0 && i+3<6 && j>=0 && j+3<7)
                slot.push_back(Quadr(read(i,j),read(i+1,j+1),read(i+2,j+2),read(i+3,j+3)));
        }
    double N1_player1=0., N2_player1=0., N3_player1=0.;
    double N1_player2=0., N2_player2=0., N3_player2=0.;
    while (!slot.empty()){
                Quadr slotQuadr(slot.back());
                slot.pop_back();
                int tokensPlayer1=0;
                int tokensPlayer2=0;
                for (int i=0; i<4; i++){
                    if (slotQuadr(i)== 1)
                        tokensPlayer1+=1;
                    else if (slotQuadr(i)== 2)
                        tokensPlayer2+=1;}
                if (tokensPlayer1==1 && tokensPlayer2==0)
                    N1_player1+=1;
                else if (tokensPlayer1==2 && tokensPlayer2==0)
                    N2_player1+=1;
                else if (tokensPlayer1==3 && tokensPlayer2==0)
                    N3_player1+=1;
                else if (tokensPlayer2==1 && tokensPlayer1==0)
                    N1_player2+=1;
                else if (tokensPlayer2==2 && tokensPlayer1==0)
                    N2_player2+=1;
                else if (tokensPlayer2==3 && tokensPlayer1==0)
                    N3_player2+=1;
            }

    return pair<double,double>(a*N1_player1+b*N2_player1+c*N3_player1,a*N1_player2+b*N2_player2+c*N3_player2);


}

