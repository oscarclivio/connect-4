#pragma once

#include "PlayerGame.h"

//Genetic algorithm : implementing a genome
class Genome{
public:
    int* genes = new int[30];
    double a,b,c;
    double score=0.;
    Genome();
    Genome(int aa, int bb, int cc);
    Genome(const Genome& G){
        a=G.a; b=G.b; c=G.c; for (int k=0; k<30; k++) genes[k] = G.genes[k]; score=G.score;};
    Genome& operator=(const Genome& G)
        {a=G.a; b=G.b; c=G.c; for (int k=0; k<30; k++) genes[k] = G.genes[k]; score=G.score; return *this;};
    ~Genome(){delete[] genes;};
    void updateParams();
    void mutate();
    void playAgainst(Genome& G);
    bool winsAgainst(const Genome& G, int depth1=DEPTH, int depth2=DEPTH) const;
    void reproduce(Genome& G);
    string toString() const;
};

//Genetic algorithm : implementing a genome population
class Population{
    int size;
    vector<Genome> genomes;
    int best = -1;
public:
    Population();
    Population(int sz);
    Population(vector<Genome> vect);
    void updateScores();
    void select();
    void crossover();
    void mutate();
    void simulate(int nb);
    Genome bestGenome() const {return genomes[best];};
    void test(const Genome& G) const;
};
