#ifndef PKSIM_H
#define PKSIM_H
#include <string>
#include <vector>
using namespace std;
struct PK {
    double F;          
    double k1_f;       
    double k1_s;       
    double k2;         
    double k3;         
    double frac_f;     
    double r;          
};

struct DoseEvent {
    double timeH;          
    double dose;           
    string way;            
    string ester;          
    double xishou;         
    string buwei;          
    double wearDuration;   
};

struct GridPoint {
    double time;          
    double conc;           
};

struct SimulationResult {
    vector<GridPoint> grid;   
    double auc;               
};
double realE2(string ester);
double utility(string way, string ester, double xishou = 0, string buwei = "");
PK calculatePK(string way, string ester = "", double xishou = 0, double r = 0, string buwei = "");
double oneCompAmount(double tau, double doseMG, double F, double ka, double ke);
SimulationResult runSimulation(vector<DoseEvent>& events, double weight);
double interpolateConcentration(const SimulationResult& sim, double hour);
#endif