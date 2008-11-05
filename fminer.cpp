#include "fminer.h"
#include "globals.h"

Fminer::Fminer() : init_mining_done(false) {
  outl = NULL; database = NULL; statistics = NULL; chisq = NULL; result = NULL;
  Reset();
  Defaults();
}

Fminer::Fminer(int _type, unsigned int _minfreq) : init_mining_done(false) {
  if ((_type != 1) && (_type != 2)) { cerr << "Error! Invalid value '" << _type << "' for parameter type." << endl; exit(1); }
  if (_minfreq < 1) { cerr << "Error! Invalid value '" << _minfreq << "' for parameter minfreq." << endl; exit(1); }
  outl = NULL; database = NULL; statistics = NULL; chisq = NULL; result = NULL;
  Reset();
  Defaults();
  type = _type;
  minfreq = _minfreq;
}

Fminer::Fminer(int _type, unsigned int _minfreq, float _chisq_val, bool _do_backbone) : init_mining_done(false) {
  if (_chisq_val < 0.0 || _chisq_val > 1.0) { cerr << "Error! Invalid value '" << _chisq_val << "' for parameter chisq->" << endl; exit(1); }
  if ((_type != 1) && (_type != 2)) { cerr << "Error! Invalid value '" << _type << "' for parameter type." << endl; exit(1); }
  if (_minfreq < 1) { cerr << "Error! Invalid value '" << _minfreq << "' for parameter minfreq." << endl; exit(1); }
  outl = NULL; database = NULL; statistics = NULL; chisq = NULL; result = NULL;
  Reset();
  Defaults();
  type = _type;
  minfreq = _minfreq;
  chisq->sig = _chisq_val;
  do_backbone = _do_backbone; if (!do_backbone) SetDynamicUpperBound(false);
}

Fminer::~Fminer() {
    delete database;
    delete statistics; 
    delete chisq; 
    delete outl;
}

void Fminer::Reset() { 
    delete database; database = new Database();
    delete statistics; statistics = new Statistics();
    delete chisq; chisq = new ChisqConstraint(0.95);
    chisq->active = true; 
    result = &r;
    delete outl; outl = new string();

    comp_runner=1; 
    comp_no=0; 
    init_mining_done = false;
}

void Fminer::Defaults() {
    minfreq = 2;
    type = 2;
    do_backbone = true;
    adjust_ub = true;
    do_pruning = true;
    updated = true;
}

vector<string>* Fminer::MineRoot(unsigned int j) {
    if (!init_mining_done) {
                        each (database->trees) {
                            if (database->trees[i]->activity == -1) {
                                cerr << "Error! ID " << database->trees[i]->orig_tid << " is missing activity information." << endl;
                                exit(1);
                            }
                        }
                        database->edgecount (); database->reorder (); initLegStatics (); graphstate.init (); 
                        if (!do_pruning || !do_backbone) {SetDynamicUpperBound(false);} init_mining_done=true; 
                        
                    }

    result->clear();
    if (j >= database->nodelabels.size()) { cerr << "Error! Root node does not exist." << endl;  exit(1); }
    if ( database->nodelabels[j].frequency >= minfreq && database->nodelabels[j].frequentedgelabels.size () ) {
        Path path(j);
        path.expand(); // mining step
    }

    return result;
}

bool Fminer::AddCompound(string smiles, unsigned int comp_id) {
    bool insert_done=false;
    if (comp_id<=0) { cerr << "Error! IDs must be of type: Int > 0." << endl;}
    else {
        if (database->readTree (smiles, comp_no, comp_id, comp_runner)) {
            insert_done=true;
            comp_no++;
        }
        else { cerr << "Error on compound " << comp_runner << ", id " << comp_id << "." << endl; }
        comp_runner++;
    }
    return insert_done;
}

bool Fminer::AddActivity(bool act, unsigned int comp_id) {
    if (database->trees_map[comp_id] == NULL) { 
        cerr << "No structure for ID " << comp_id << ". Ignoring entry!" << endl; return false; 
    }
    else {
        if ((database->trees_map[comp_id]->activity = act)) AddChiSqNa();
        else AddChiSqNi();
        return true;
    }
}

void Fminer::SetDynamicUpperBound(bool val) {
    adjust_ub=val; 
    if ((!do_pruning && adjust_ub) || (!do_backbone && adjust_ub)) {
        cerr << "Error! Can't switch on dynamic upper bound pruning: statistical metrical pruning or backbone mining is disabled!" << endl; 
        exit(1); 
    }
}

void Fminer::SetPruning(bool val) {
    do_pruning=val; 
    if (!do_pruning && adjust_ub) {
        cerr << "Error! Can't switch off statistical metrical pruning: dynamic upper bound pruning is enabled." << endl; 
        exit(1); 
    }
}

