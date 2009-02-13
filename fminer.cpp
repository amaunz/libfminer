#include "fminer.h"
#include "globals.h"

Fminer::Fminer() : init_mining_done(false) {
  if (!instance_present) {
      database = NULL; statistics = NULL; chisq = NULL; result = NULL;
      Reset();
      Defaults();
      instance_present=true;
      if (getenv("FMINER_LAZAR")) do_yaml = false;
      if (getenv("FMINER_SMARTS")) gsp_out = false; 
  }
  else {
    cerr << "Error! Can't create more than 1 instance." << endl; 
    exit(1);
  }
}

Fminer::Fminer(int _type, unsigned int _minfreq) : init_mining_done(false) {
  if (!instance_present) {
      database = NULL; statistics = NULL; chisq = NULL; result = NULL;
      Reset();
      Defaults();
      SetType(_type);
      SetMinfreq(_minfreq);
      instance_present=true;
      if (getenv("FMINER_LAZAR")) do_yaml = false;
      if (getenv("FMINER_SMARTS")) gsp_out = false; 
  }
  else {
    cerr << "Error! Can't create more than 1 instance." << endl; 
    exit(1);
  }

}

Fminer::Fminer(int _type, unsigned int _minfreq, float _chisq_val, bool _do_backbone) : init_mining_done(false) {
  if (!instance_present) {
      database = NULL; statistics = NULL; chisq = NULL; result = NULL;
      Reset();
      Defaults();
      SetType(_type);
      SetMinfreq(_minfreq);
      SetChisqSig(_chisq_val);
      SetBackbone(_do_backbone);
      instance_present=true;
      if (getenv("FMINER_LAZAR")) do_yaml = false;
      if (getenv("FMINER_SMARTS")) gsp_out = false; 

  }
  else {
    cerr << "Error! Can't create more than 1 instance." << endl; 
    exit(1);
  }

}

Fminer::~Fminer() {
    if (instance_present) {
        delete database;
        delete statistics; 
        delete chisq; 
        instance_present=false;
    }
}

void Fminer::Reset() { 
    delete database; database = new Database();
    delete statistics; statistics = new Statistics();
    delete chisq; chisq = new ChisqConstraint(0.95);
    chisq->active = true; 

    result = &r;

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
    console_out = false;
    aromatic = true;
    refine_singles = false;
    do_output=true;
    do_yaml=true;
    gsp_out=true;
    bbrc_sep=false;
}

void Fminer::SetConsoleOut(bool val) {
    if (val==true && bbrc_sep) cerr << "Warning: Console output could not be enabled!" << endl;
    else console_out=val;
}

void Fminer::SetBbrcSep(bool val) {
    if (val==true && console_out) {
        cerr << "Warning: Disabling console output, using result vector!" << endl;
        console_out=false;
    }
    bbrc_sep=val;
}

vector<string>* Fminer::MineRoot(unsigned int j) {
    if (!init_mining_done) {
        if (chisq->active) {
            each (database->trees) {
                if (database->trees[i]->activity == -1) {
                    cerr << "Error! ID " << database->trees[i]->orig_tid << " is missing activity information." << endl;
                    exit(1);
                }
            }
        }
        database->edgecount (); cerr << "Edgecount" << endl; database->reorder (); cerr << "Reorder" << endl; initLegStatics (); graphstate.init (); 
        init_mining_done=true; 
    }

    result->clear();
    if (j >= database->nodelabels.size()) { cerr << "Error! Root node does not exist." << endl;  exit(1); }
    if ( database->nodelabels[j].frequency >= minfreq && database->nodelabels[j].frequentedgelabels.size () ) {
        Path path(j);
        path.expand(); // mining step
    }

    return result;
}

void Fminer::ReadGsp(FILE* gsp){
    database->readGsp(gsp);
}

bool Fminer::AddCompound(string smiles, unsigned int comp_id) {
    bool insert_done=false;
    if (comp_id<=0) { cerr << "Error! IDs must be of type: Int > 0." << endl;}
    else {
        if (database->readTreeSmi (smiles, comp_no, comp_id, comp_runner)) {
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

void Fminer::SetType(int _type) {
    if ((_type != 1) && (_type != 2)) { cerr << "Error! Invalid value '" << _type << "' for parameter type." << endl; exit(1); }
    type = _type;
}

void Fminer::SetMinfreq(int _minfreq) {
    if (_minfreq < 1) { cerr << "Error! Invalid value '" << _minfreq << "' for parameter minfreq." << endl; exit(1); }
    minfreq = _minfreq;
}

void Fminer::SetChisqSig(float _chisq_val) {
    if (_chisq_val < 0.0 || _chisq_val > 1.0) { cerr << "Error! Invalid value '" << _chisq_val << "' for parameter chisq->" << endl; exit(1); }
    chisq->sig = gsl_cdf_chisq_Pinv(_chisq_val, 1);
}

void Fminer::SetChisqActive(bool _val) {
    chisq->active = _val;
    if (_val == false) {
        SetDynamicUpperBound(false); //order important
        SetBackbone(false);
        SetPruning(false);
    }
}

void Fminer::SetPruning(bool val) {
    do_pruning=val;
    if (!do_pruning) {
        if (GetDynamicUpperBound()) cerr << "Notice: Disabling dynamic upper bound pruning." << endl;
        SetDynamicUpperBound(false); 
    }
}

void Fminer::SetBackbone(bool val) {
    do_backbone = val;
    if (!do_backbone) {
        if (GetDynamicUpperBound()) cerr << "Notice: Disabling dynamic upper bound pruning." << endl;
        SetDynamicUpperBound(false);
    }
}

void Fminer::SetDynamicUpperBound(bool val) {
    adjust_ub=val; 
    if (adjust_ub) {
        if (!GetPruning()) cerr << "Notice: Enabling statistical metrical pruning." << endl;
        SetPruning(true);
        if (!GetBackbone()) cerr << "Notice: Enabling mining for backbone refinement class representatives." << endl;
        SetBackbone(true);
    }
}



void Fminer::SetAromatic(bool val) {
    aromatic = val;
}

void Fminer::SetRefineSingles(bool val) {
    refine_singles = val;
}

void Fminer::SetDoOutput(bool val) {
    do_output = val;
}


bool Fminer::GetConsoleOut(){return console_out;}
bool Fminer::GetBbrcSep(){return bbrc_sep;}
int Fminer::GetType(){return type;}
int Fminer::GetMinfreq(){return minfreq;}
bool Fminer::GetChisqSig(){return chisq->sig;}
bool Fminer::GetBackbone(){return do_backbone;}

bool Fminer::GetDynamicUpperBound(){return adjust_ub;}
bool Fminer::GetPruning() {return do_pruning;}
bool Fminer::GetAromatic() {return aromatic;}
bool Fminer::GetRefineSingles() {return refine_singles;}
bool Fminer::GetDoOutput() {return do_output;}



