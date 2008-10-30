// fminer.h
// Andreas Maunz, andreas@maunz.de, oct 2008
#ifndef FMINER_H
#define FMINER_H

#include "database.h"
#include "misc.h"
#include "constraints.h"
#include "closeleg.h"

class FMiner {

  friend class Path;
  friend class PatternTree;
  friend class GraphState;
  friend void addCloseExtensions ( vector<CloseLegPtr> &targetcloselegs, int number );
  friend CloseLegOccurrences* join(LegOccurrences&, CloseLegOccurrences&);
  friend CloseLegOccurrences* join(CloseLegOccurrences&, CloseLegOccurrences&);
  friend LegOccurrences* join(LegOccurrences&, NodeId, LegOccurrences&);
  friend LegOccurrences* join(LegOccurrences&);
  friend void initLegStatics();
  friend void extend(LegOccurrences&);
  friend void candidateCloseLegsAllocate(int, int);
  friend void extend(LegOccurrences&, EdgeLabel, EdgeLabel);

  public:
    //! Constructor for standard settings: 95% significance level, minimum frequency 2, type trees, dynamic upper bound, BBRC
    FMiner ();
    //! Like standard constructor, but type and minimum frequency configurable
    FMiner (int type, unsigned int minfreq);
    //! Like standard constructor, but type, minimum frequency, significance level and BBRC configurable
    FMiner (int type, unsigned int minfreq, float chisq_val, bool do_backbone);

    
    bool AddCompound(string smiles, unsigned int comp_id); //!< Add a Compound to the database
    bool AddActivity(bool act, unsigned int comp_id); //!< Add an activity to the database
    int GetNoRootNodes() {return database.nodelabels.size();} //!< Get no of root node (element type)
    vector<string>* MineRoot(int j); //!< Mine fragments rooted at the j-th root node (element type)

    void SetDynamicUpperBound(bool val) {adjust_ub=val;} //!< Pass 'false' here to disable dynamic upper bound pruning (e.g. for performance measures).
    void SetPruning(bool val) {do_pruning=val;} //!< Pass 'false' here to disable statistical pruning completely.
        
  private:
    Database database;
    Statistics statistics;
    string outl;
    vector<string> result;
    void Init() { chisq.active = true; comp_runner=1; comp_no=0; }
    void AddChiSqNa(){chisq.na++;chisq.n++;}
    void AddChiSqNi(){chisq.ni++;chisq.n++;}

    ChisqConstraint chisq;
    unsigned int minfreq;
    int type;
    bool do_backbone;
    bool updated;
    bool adjust_ub;
    bool do_pruning;

    bool init_mining_done;
    int comp_runner;
    int comp_no;

};

#endif
