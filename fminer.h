// fminer.h
// Andreas Maunz, andreas@maunz.de, oct 2008
#ifndef FMINER_H
#define FMINER_H

#include "misc.h"
#include "closeleg.h"
#include "graphstate.h"

extern bool adjust_ub;
extern bool do_pruning;
extern ChisqConstraint* chisq;

class Fminer {

  public:

    //! Constructor for standard settings: 95% significance level, minimum frequency 2, type trees, dynamic upper bound, BBRC
    Fminer ();
    //! Like standard constructor, but type and minimum frequency configurable
    Fminer (int _type, unsigned int _minfreq);
    //! Like standard constructor, but type, minimum frequency, significance level and BBRC configurable
    Fminer (int _type, unsigned int _minfreq, float chisq_val, bool _do_backbone);

    ~Fminer();

    
    bool AddCompound(string smiles, unsigned int comp_id); //!< Add a compound to the database
    bool AddActivity(bool act, unsigned int comp_id); //!< Add an activity to the database
    int GetNoRootNodes() {return database->nodelabels.size();} //!< Get no of root node (element type)
    int GetNoCompounds() {return database->trees.size();}
    vector<string>* MineRoot(unsigned int j); //!< Mine fragments rooted at the j-th root node (element type)

    void SetDynamicUpperBound(bool val) {adjust_ub=val;} //!< Pass 'false' here to disable dynamic upper bound pruning (e.g. for performance measures).
    void SetPruning(bool val) {do_pruning=val;} //!< Pass 'false' here to disable statistical pruning completely.
    void Reset(); //!< Use this to clear the database before feeding new compounds and activities
    void Defaults(); //!< Use this to set default parameters as in default constructor
        
  private:
    void AddChiSqNa(){chisq->na++;chisq->n++;}
    void AddChiSqNi(){chisq->ni++;chisq->n++;}

    bool init_mining_done;
    int comp_runner;
    int comp_no;

    vector<string> r;

};

#endif
