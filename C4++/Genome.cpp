#include "Genome.h"

std::mt19937_64 generator(time(0));

bernoulli_distribution distrMutate(0.05);
bernoulli_distribution distrReproduce(0.5);
uniform_int_distribution<int> uniform30(1,29);

Genome::Genome()
    {bool chooseOne = false;
    for (int k=0; k<30; k++)
    {
        chooseOne = distrReproduce(generator);
        if (chooseOne) genes[k] = 1;
        else genes[k] = 0;
    }
    updateParams();}

Genome::Genome(int aa, int bb, int cc){
    a=aa;
    b=bb;
    c=cc;
    for (int k=0; k<10; k++){
        genes[k] = aa%2; genes[10+k] = bb%2; genes[20+k] = cc%2;
        aa = (aa - genes[k])/2; bb = (bb - genes[10+k])/2; cc = (cc - genes[20+k])/2;
    }
}

//Update the parameters in the genome
void Genome::updateParams(){
   a=0.; b=0.; c=0.;
   double produit=1.;
   for (int k=0; k<10; k++){
       a += genes[k]*produit;
       b += genes[k+10]*produit;
       c += genes[k+20]*produit;
       produit *= 2.;
   }
}

//Make the genome mutate
void Genome::mutate(){
    for (int k=0; k<30; k++){
        if (distrMutate(generator))
            genes[k] = (1-genes[k]);
    }
    updateParams();
}

//Play a computer-vs-computer game against another genome G
void Genome::playAgainst(Genome& G)
{
    Game game;
    double payment = game.playAIvsAI(a, b, c, G.a, G.b, G.c);
    score += 1.+payment;
    G.score += 1.-payment;
}

//Check if the genome can beat another genome G in computer-vs-computer mode with given depths
bool Genome::winsAgainst(const Genome& G, int depth1, int depth2) const{
    double payment = 0.;


    Game game; payment += game.playAIvsAI(a, b, c, G.a, G.b, G.c, depth1, depth2);
    game = Game(); payment += game.playAIvsAI(G.a, G.b, G.c, a, b, c, depth2, depth1);

    bool result = (payment/2 > 0);

    if (result)
    cout << "(" << a << "," << b << "," << c << ") beats (" <<
            G.a << "," << G.b << "," << G.c << ") : " << payment/2 << endl;
    else
        cout << "(" << a << "," << b << "," << c << ") is beaten by (" <<
                G.a << "," << G.b << "," << G.c << ") : " << payment/2 << endl;
    return result;
}

//Make a genome reproduce itself
void Genome::reproduce(Genome& G){
    int pivot = uniform30(generator);
    for (int i=0; i<pivot; i++)
        genes[i] = G.genes[i];
    for (int i=pivot; i<30; i++)
        G.genes[i] = genes[i];
    updateParams();
    G.updateParams();
}

//Show the content of a genome
string Genome::toString() const {
    stringstream ss;
    ss << "(" << a << "," << b << "," << c << " ; " << score << ")";
    return ss.str();
}

Population::Population(){
    size = 20;
    for (int k=0; k<20; k++)
        genomes.push_back(Genome());
}

Population::Population(int sz){
    size = sz;
    for (int k=0; k<sz; k++)
        genomes.push_back(Genome());
}

Population::Population(vector<Genome> vect){
    genomes = vect;
    size = vect.size();
}

//Update players' scores.
void Population::updateScores(){
    for (int k=0; k<size; k++)
        genomes[k].score = 0.;
    for (int k1=0; k1<size; k1++)
        for (int k2=0; k2<k1; k2++)
        {
               genomes[k1].playAgainst(genomes[k2]);
               genomes[k2].playAgainst(genomes[k1]);
        }
    double scoreBest = 0.;
    for (int i=0; i<size; i++){
        genomes[i].score /= 2*(size-1);
        if (genomes[i].score > scoreBest){
            best = i;
            scoreBest = genomes[i].score;
        }
    }
}

//Select new genomes for the next step of the algorithm
void Population::select(){
    vector<double> weights(size);
    for( int i = 0; i < size; i++) {
      weights[i] = genomes[i].score;
    }
    discrete_distribution<int> distr(weights.begin(), weights.end());
    vector<Genome> newGenomes;
    newGenomes.push_back(genomes[best]);
    int selected;
    for (int k=0; k<size-1; k++){
        selected = distr(generator);
        newGenomes.push_back(genomes[selected]);
    }
    genomes = newGenomes;
    best = 0;
}

//Cross-over reproduction in the population
void Population::crossover(){
    for( int i = 2; i < size; i+=2)
        if (distrReproduce(generator))
            genomes[i].reproduce(genomes[i+1]);
}

//Make all genomes mutate
void Population::mutate(){
    for (int k=0; k<size; k++)
        if (k != best)
            genomes[k].mutate();
}

//Simulate a tournament between all genomes in the population
void Population::simulate(int nb){
    updateScores();
    cout << "Iteration 0: ";
    for (int k=0; k<30; k++)
            cout << genomes[k].toString() << " ";
    cout << endl;
    for (int n=1; n<=nb; n++){
        select();
        cout << "It. " << n << " selection ok" << endl;
        crossover();
        cout << "Crossover " << endl;
        mutate();
        cout << "Iteration " << n << ": ";
        updateScores();
        for (int k=0; k<size; k++)
                cout << genomes[k].toString() << " ";
        cout << endl;
    }
    cout << "Best: " << genomes[best].toString() << endl;
}

// (Apart from the genetic algorithm) Test a given genome G against the entire population
// Two tests : normal (all parameters random) or human (more weight is given to the best token combinations)
void Population::test(const Genome& G) const
{
    int nbOks = 0;
    int nbOksDef = 0;
    Genome def(1,10,100);
    for (int k=0; k<size; k++)
    {
        cout << "Normal test " << k << ":" << endl;
        if (G.winsAgainst(genomes[k]))
            nbOks++;
        if (def.winsAgainst(genomes[k]))
            nbOksDef++;
    }
    cout << "Standard victory rate: " << (nbOks*100.)/size;
    cout << " for " << G.toString() << endl;
    cout << "Standard victory rate: " << (nbOksDef*100.)/size;
    cout << " for " << def.toString() << endl;


    nbOks = 0;
    nbOksDef = 0;

    srand(time(0));
    int a,b,c;
    for (int k=0; k<size; k++)
    {
        do {a = rand()%1000; b = rand()%1000; c = rand()%1000;}
        while (a > b || b > c);
        cout << "Human test " << k << ":" << endl;
        if (G.winsAgainst(Genome(a,b,c)))
            nbOks++;
        if (def.winsAgainst(Genome(a,b,c)))
            nbOksDef++;
    }
    cout << "Human victory rate: " << (nbOks*100.)/size;
    cout << " for " << G.toString() << endl;
    cout << "Human victory rate: " << (nbOksDef*100.)/size;
    cout << " for " << def.toString() << endl;

}
