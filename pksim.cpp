#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>
#include <iomanip>
#include <vector>    
#include <algorithm>
using namespace std;
struct PK{
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
double realE2(string ester){
    double mwe2=272.38;
    if(ester=="E2"){
        return 1.0;
    }
    else if(ester=="EB"){
        double eb=376.50;
        return mwe2/eb;
    }
    else if(ester=="EV"){
        double ev=356.50;
        return mwe2/ev;
    }
    else if(ester=="EC"){
        double ec=396.58;
        return mwe2/ec;
    }
    else if(ester=="EN"){
        double en=384.56;
        return mwe2/en;
    }
    return 0.0;
}
double utility(string way,string ester,double xishou=0,string buwei=""){
    if(way=="koufu"){
        return 0.03;
    }else if(way=="shexia"){
        return (1-xishou)*0.03+xishou;
    }else if(way=="ningjiao"){
        if(buwei=="dd"){
            return 0.4;
        }else{
            return 0.05;
        }
        
    }else if(way=="tiepian"){
        return 1.00;
    }else if(way=="zhushe"){
        if(ester=="EB"){
            return 0.1092237647;
        }else if(ester=="EV"){
            return 0.0622582882;
        }else if(ester=="EC"){
            return 0.117255838;
        }else if(ester=="EN"){
            return 0.12;
        }
    }
    return 0.00;
    
}
PK calculatePK(string way,string ester="",double xishou=0,double r=0,string buwei=""){
    PK p;
    p.F = 0.0; p.k1_f = 0.0; p.k1_s = 0.0; p.k2 = 0.0; p.k3 = 0.0; p.frac_f = 0.0; p.r = 0.0;
    p.F = realE2(ester) * utility(way, ester, xishou, buwei);
    if(way=="koufu"){
        if(ester=="EV"){
            p.k1_f=0.0;
            p.k1_s=0.05;
            p.k2=0.0;
            p.k3=0.41;
            p.frac_f=0.0;
        }else{
            p.k1_f=0.0;
            p.k1_s=0.32;
            p.k2=0.0;
            p.k3=0.41;
            p.frac_f=0.0;
        }
        
    }
    else if(way=="shexia"){
       if(ester=="EV"){
            p.k1_f=1.8;
            p.k1_s=0.05;
            p.k2=0.070;
            p.k3=0.41;
            p.frac_f=xishou;
        }else if(ester=="E2"){
            p.k1_f=1.8;
            p.k1_s=0.32;
            p.k2=0.0;
            p.k3=0.41;
            p.frac_f=xishou;
        }else{
            p.k1_f=0.0;
            p.k1_s=0.0;
            p.k2=0.0;
            p.k3=0.0;
            p.frac_f=0;
        }

    }else if(way=="ningjiao"){
        p.k1_f=0.0;
        p.k1_s=0.022;
        p.k2=0.0;
        p.k3=0.41;
        p.frac_f=0.0;
    }
    else if(way=="tiepian"){
        if(r!=0){
            p.k1_f=0.0;
            p.k1_s=0.0;
            p.k2=0.0;
            p.k3=0.41;
            p.frac_f=1.0;
            p.r=r;
        }else{
            p.k1_f=0.0075;
            p.k1_s=0.0;
            p.k2=0.0;
            p.k3=0.41;
            p.frac_f=1.0;
            p.r=0.0;
        }
    }else if(way=="zhushe"){
        if(ester=="EB"){
            p.k1_f=0.144;
            p.k1_s=0.14;
            p.k2=0.090;
            p.k3=0.041;
            p.frac_f=0.90;
        }else if(ester=="EV"){
            p.k1_f=0.0216;
            p.k1_s=0.0138;
            p.k2=0.070;
            p.k3=0.041;
            p.frac_f=0.40;
        }else if(ester=="EC"){
            p.k1_f=0.005035046;
            p.k1_s=0.004510574;
            p.k2=0.045;
            p.k3=0.041;
            p.frac_f=0.229164549;
        }else if(ester=="EN"){
            p.k1_f=0.0010;
            p.k1_s=0.0050;
            p.k2=0.015;
            p.k3=0.041;
            p.frac_f=0.05;
        }
    }
    return p;
}
double oneCompAmount(double tau, double doseMG, double F, double ka, double ke) {
    if (tau < 0) return 0.0;
    if (abs(ka - ke) < 1e-5) {
        return doseMG * F * ka * tau * exp(-ke * tau);
    }
    double numerator = doseMG * F * ka;
    double denominator = ka - ke;
    return (numerator / denominator) * (exp(-ke * tau) - exp(-ka * tau));
}
double _analytic3C(double tau, double doseMG, double F, double k1, double k2, double k3) {
    if (tau < 0) return 0.0;
    if (abs(k1 - k2) < 1e-5 || abs(k1 - k3) < 1e-5 || abs(k2 - k3) < 1e-5) {
        return 0.0;
    }
    double term1 = exp(-k1 * tau) / ((k1 - k2) * (k1 - k3));
    double term2 = exp(-k2 * tau) / ((k2 - k1) * (k2 - k3));
    double term3 = exp(-k3 * tau) / ((k3 - k1) * (k3 - k2));
    return doseMG * F * k1 * k2 * (term1 + term2 + term3);
}

SimulationResult runSimulation(vector<DoseEvent>& events, double weight) {
    SimulationResult res;
    if (events.empty()) {
        res.auc = 0.0;
        return res;
    }
    sort(events.begin(), events.end(), [](const DoseEvent& a, const DoseEvent& b) {
        return a.timeH < b.timeH;
    });
    double tmin = events.front().timeH;
    double tmax = events.back().timeH;
    double tstart = tmin - 24.0;
    double tend = tmax + 24.0 * 21.0; 

    
    int steps = 1000;
    double delta_t = (tend - tstart) / (steps - 1);
    res.grid.resize(steps);
    double Vplasma = (2.0 * weight) * 1000.0;
    for (int i = 0; i < steps; ++i) {
        double ti = tstart + i * delta_t;
        double total_amount = 0.0; 

        for (const auto& event : events) {
            double tau = ti - event.timeH; 
            if (tau < 0) continue;         

            PK p = calculatePK(event.way, event.ester, event.xishou, event.dose, event.buwei);
            
            double T = (event.way == "tiepian") ? event.wearDuration : 999999.0;
            double amount = 0.0;
            if (event.way == "tiepian") {
                if (p.r > 0.0) {
                    double R = p.r / (24.0 * 1000.0);
                    if (tau <= T) amount = (R / p.k3) * (1.0 - exp(-p.k3 * tau));
                    else          amount = ((R / p.k3) * (1.0 - exp(-p.k3 * T))) * exp(-p.k3 * (tau - T));
                } else {
                    if (tau <= T) amount = oneCompAmount(tau, event.dose, p.F, p.k1_f, p.k3);
                    else          amount = oneCompAmount(T, event.dose, p.F, p.k1_f, p.k3) * exp(-p.k3 * (tau - T));
                }
            } 
            else if (event.way == "zhushe") {
                double fast_part = 0.0, slow_part = 0.0;
                if (p.frac_f > 0.0) fast_part = p.frac_f * _analytic3C(tau, event.dose, p.F, p.k1_f, p.k2, p.k3);
                if (p.frac_f < 1.0) slow_part = (1.0 - p.frac_f) * _analytic3C(tau, event.dose, p.F, p.k1_s, p.k2, p.k3);
                amount = fast_part + slow_part;
            } 
            else {
                double fast_part = p.frac_f * oneCompAmount(tau, event.dose, p.F, p.k1_f, p.k3);
                double slow_part = (1.0 - p.frac_f) * oneCompAmount(tau, event.dose, p.F, p.k1_s, p.k3);
                amount = fast_part + slow_part;
            }

            total_amount += amount;
        }
        res.grid[i].time = ti;
        res.grid[i].conc = (total_amount * 1000000000.0) / Vplasma;
    }
    double total_auc = 0.0;
    for (int i = 1; i < steps; ++i) {
        double c_curr = res.grid[i].conc;
        double c_prev = res.grid[i - 1].conc;
        double t_curr = res.grid[i].time;
        double t_prev = res.grid[i - 1].time;

        total_auc += ((c_curr + c_prev) / 2.0) * (t_curr - t_prev);
    }
    res.auc = total_auc;

    return res;
}
double interpolateConcentration(const SimulationResult& sim, double hour) {
    if (sim.grid.empty()) return 0.0;
    int N = sim.grid.size();
    if (hour <= sim.grid[0].time) return sim.grid[0].conc;
    if (hour >= sim.grid[N - 1].time) return sim.grid[N - 1].conc;
    int low = 0;
    int high = N - 1;
    int k = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sim.grid[mid].time <= hour) {
            k = mid; 
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    double tk = sim.grid[k].time;
    double tk1 = sim.grid[k + 1].time;
    double ck = sim.grid[k].conc;
    double ck1 = sim.grid[k + 1].conc;
    double fraction = (hour - tk) / (tk1 - tk);
    return ck + fraction * (ck1 - ck);
}
int main(){
    
    return 0;
}