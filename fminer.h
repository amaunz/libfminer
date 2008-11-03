// fminer.h
// Andreas Maunz, andreas@maunz.de, oct 2008
#ifndef FMINER_H
#define FMINER_H

#include "misc.h"
#include "closeleg.h"
#include "graphstate.h"
#include "globals.h"

class FMiner {

  public:
    //! Constructor for standard settings: 95% significance level, minimum frequency 2, type trees, dynamic upper bound, BBRC
    FMiner ();
    //! Like standard constructor, but type and minimum frequency configurable
    FMiner (int _type, unsigned int _minfreq);
    //! Like standard constructor, but type, minimum frequency, significance level and BBRC configurable
    FMiner (int _type, unsigned int _minfreq, float chisq_val, bool _do_backbone);

    
    bool AddCompound(string smiles, unsigned int comp_id); //!< Add a Compound to the database
    bool AddActivity(bool act, unsigned int comp_id); //!< Add an activity to the database
    int GetNoRootNodes() {return database->nodelabels.size();} //!< Get no of root node (element type)
    int GetNoCompounds() {return database->trees.size();}
    vector<string>* MineRoot(unsigned int j); //!< Mine fragments rooted at the j-th root node (element type)

    void SetDynamicUpperBound(bool val) {adjust_ub=val;} //!< Pass 'false' here to disable dynamic upper bound pruning (e.g. for performance measures).
    void SetPruning(bool val) {do_pruning=val;} //!< Pass 'false' here to disable statistical pruning completely.
        
  private:
    void Init();
    void AddChiSqNa(){chisq->na++;chisq->n++;}
    void AddChiSqNi(){chisq->ni++;chisq->n++;}

    bool init_mining_done;
    int comp_runner;
    int comp_no;

    vector<string> r;

};

#endif
