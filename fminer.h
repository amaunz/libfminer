// fminer.h
// Andreas Maunz, andreas@maunz.de, oct 2008
#ifndef FMINER_H
#define FMINER_H

#include "database.h"
#include "misc.h"
#include "constraints.h"

class FMiner {

  friend class Path;
  friend class PatternTree;
  friend class GraphState;

  public:
    FMiner ();
    FMiner (int type, unsigned int minfreq);
    FMiner (int type, unsigned int minfreq, float chisq_val, bool do_backbone);
    ~FMiner ();
    void SetDynamicUpperBound(bool val) {
      if ((val == false) && (!do_backbone || !do_pruning)) { cerr << "Error! Dynamic Upper Bound Adjustment cannot be switched off!" << endl; exit(1); }
      else adjust_ub=val;
    }
    void SetPruning(bool val) {do_pruning=val;}
    void AddChiSqNa(){chisq.na++;}
    void AddChiSqNi(){chisq.ni++;}
    void AddChiSqN(){chisq.n=chisq.na+chisq.ni;}
    
    Database database;
  private:
    Statistics statistics;
    vector<string> result;
    void InitChisq() { chisq.active = true; }

    ChisqConstraint chisq;
    unsigned int minfreq;
    int type;
    bool do_backbone;
    bool updated;

    bool adjust_ub;
    bool do_pruning;

};

#endif
