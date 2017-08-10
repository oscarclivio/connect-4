#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

#include <sstream>
using namespace std;

//N^4 class
class Quadr{
    int* ints = new int[4];
public:
    Quadr(){ints[0]=0; ints[1]=0; ints[2]=0; ints[3]=0;};
    Quadr(int a, int b, int c, int d) {ints[0]=a; ints[1]=b; ints[2]=c; ints[3]=d;};
    Quadr(const Quadr& Q){ints[0]=Q.ints[0]; ints[1]=Q.ints[1]; ints[2]=Q.ints[2]; ints[3]=Q.ints[3];};
    ~Quadr(){delete[] ints;};
    Quadr& operator=(const Quadr& Q);
    int operator()(int i){return ints[i];};
};

//Connect-4 grid class
class Grid{
private:
    int* elements = new int[42];
public:
    Grid(){for (int k=0; k<42; k++) elements[k] = 0;};
    Grid(const Grid& G);
    ~Grid();
    Grid& operator=(const Grid& G);
    int read(int i, int j) const {return elements[7*i+j];};
    int operator()(int i, int j) const {return elements[7*i+j];};
    int& operator()(int i, int j) {return elements[7*i+j];};
    bool contains(int i, int j) const{return (0 <= i && i <= 5 && 0 <= j && j <= 6);};
    bool isLegalMove(int j) const{return (1 <= j && j <= 7 && read(0,j-1) == 0);};
    bool insertToken(int j, int value);
    int winner();
    pair<int,bool> hasWinner();
    pair<double,double> evaluate(double a=1., double b=10., double c=100.);
};

