#include "fminer.h"

using namespace fm;

Fminer::Fminer() : init_mining_done(false) {
  if (!instance_present) {
      fm::database = NULL; fm::statistics = NULL; fm::chisq = NULL; fm::result = NULL;
      Reset();
      Defaults();
      instance_present=true;
      if (getenv("FMINER_LAZAR")) fm::do_yaml = false;
      if (getenv("FMINER_SMARTS")) fm::gsp_out = false; 
  }
  else {
    cerr << "Error! Can't create more than 1 instance." << endl; 
    exit(1);
  }
}

Fminer::Fminer(int _type, unsigned int _minfreq) : init_mining_done(false) {
  if (!instance_present) {
      fm::database = NULL; fm::statistics = NULL; fm::chisq = NULL; fm::result = NULL;
      Reset();
      Defaults();
      SetType(_type);
      SetMinfreq(_minfreq);
      instance_present=true;
      if (getenv("FMINER_LAZAR")) fm::do_yaml = false;
      if (getenv("FMINER_SMARTS")) fm::gsp_out = false; 
  }
  else {
    cerr << "Error! Can't create more than 1 instance." << endl; 
    exit(1);
  }

}

Fminer::Fminer(int _type, unsigned int _minfreq, float _chisq_val, bool _do_backbone) : init_mining_done(false) {
  if (!instance_present) {
      fm::database = NULL; fm::statistics = NULL; fm::chisq = NULL; fm::result = NULL;
      Reset();
      Defaults();
      SetType(_type);
      SetMinfreq(_minfreq);
      SetChisqSig(_chisq_val);
      SetBackbone(_do_backbone);
      instance_present=true;
      if (getenv("FMINER_LAZAR")) fm::do_yaml = false;
      if (getenv("FMINER_SMARTS")) fm::gsp_out = false; 

  }
  else {
    cerr << "Error! Can't create more than 1 instance." << endl; 
    exit(1);
  }

}

Fminer::~Fminer() {
    if (instance_present) {
        delete fm::database;
        delete fm::statistics; 
        delete fm::chisq; 
        instance_present=false;
    }
}

void Fminer::Reset() { 
    delete fm::database; fm::database = new Database();
    delete fm::statistics; fm::statistics = new Statistics();
    delete fm::chisq; fm::chisq = new ChisqConstraint(0.95);
    fm::chisq->active = true; 

    fm::result = &r;

    comp_runner=1; 
    comp_no=0; 
    init_mining_done = false;
}

void Fminer::Defaults() {
    fm::minfreq = 2;
    fm::type = 2;
    fm::do_backbone = true;
    fm::updated = true;
    fm::adjust_ub = true;
    fm::do_pruning = true;
    fm::console_out = false;
    fm::aromatic = true;
    fm::refine_singles = false;
    fm::do_output=true;
    fm::do_yaml=true;
    fm::gsp_out=true;
    fm::bbrc_sep=false;
}

void Fminer::SetConsoleOut(bool val) {
    if (val==true && fm::bbrc_sep) cerr << "Warning: Console output could not be enabled!" << endl;
    else fm::console_out=val;
}

void Fminer::SetBbrcSep(bool val) {
    if (val==true && fm::console_out) {
        cerr << "Warning: Disabling console output, using result vector!" << endl;
        fm::console_out=false;
    }
    fm::bbrc_sep=val;
}

vector<string>* Fminer::MineRoot(unsigned int j) {
    if (!init_mining_done) {
        if (fm::chisq->active) {
            each (fm::database->trees) {
                if (fm::database->trees[i]->activity == -1) {
                    cerr << "Error! ID " << fm::database->trees[i]->orig_tid << " is missing activity information." << endl;
                    exit(1);
                }
            }
        }
        fm::database->edgecount (); cerr << "Edgecount" << endl; fm::database->reorder (); cerr << "Reorder" << endl; initLegStatics (); graphstate.init (); 
        init_mining_done=true; 
    }

    fm::result->clear();
    if (j >= fm::database->nodelabels.size()) { cerr << "Error! Root node does not exist." << endl;  exit(1); }
    if ( fm::database->nodelabels[j].frequency >= fm::minfreq && fm::database->nodelabels[j].frequentedgelabels.size () ) {
        Path path(j);
        path.expand(); // mining step
    }

    return fm::result;
}

void Fminer::ReadGsp(FILE* gsp){
    fm::database->readGsp(gsp);
}

bool Fminer::AddCompound(string smiles, unsigned int comp_id) {
    bool insert_done=false;
    if (comp_id<=0) { cerr << "Error! IDs must be of type: Int > 0." << endl;}
    else {
        if (fm::database->readTreeSmi (smiles, comp_no, comp_id, comp_runner)) {
            insert_done=true;
            comp_no++;
        }
        else { cerr << "Error on compound " << comp_runner << ", id " << comp_id << "." << endl; }
        comp_runner++;
    }
    return insert_done;
}

bool Fminer::AddActivity(bool act, unsigned int comp_id) {

    if (fm::database->trees_map[comp_id] == NULL) { 
        cerr << "No structure for ID " << comp_id << ". Ignoring entry!" << endl; return false; 
    }
    else {
        if ((fm::database->trees_map[comp_id]->activity = act)) AddChiSqNa();
        else AddChiSqNi();
        return true;
    }
}

void Fminer::SetType(int _type) {
    if ((_type != 1) && (_type != 2)) { cerr << "Error! Invalid value '" << _type << "' for parameter type." << endl; exit(1); }
    fm::type = _type;
}

void Fminer::SetMinfreq(int _minfreq) {
    if (_minfreq < 1) { cerr << "Error! Invalid value '" << _minfreq << "' for parameter minfreq." << endl; exit(1); }
    fm::minfreq = _minfreq;
}

void Fminer::SetChisqSig(float _chisq_val) {
    if (_chisq_val < 0.0 || _chisq_val > 1.0) { cerr << "Error! Invalid value '" << _chisq_val << "' for parameter chisq->" << endl; exit(1); }
    fm::chisq->sig = gsl_cdf_chisq_Pinv(_chisq_val, 1);
}

void Fminer::SetChisqActive(bool _val) {
    fm::chisq->active = _val;
    if (_val == false) {
        SetDynamicUpperBound(false); //order important
        SetBackbone(false);
        SetPruning(false);
    }
}

void Fminer::SetPruning(bool val) {
    fm::do_pruning=val;
    if (!fm::do_pruning) {
        if (GetDynamicUpperBound()) cerr << "Notice: Disabling dynamic upper bound pruning." << endl;
        SetDynamicUpperBound(false); 
    }
}

void Fminer::SetBackbone(bool val) {
    fm::do_backbone = val;
    if (!fm::do_backbone) {
        if (GetDynamicUpperBound()) cerr << "Notice: Disabling dynamic upper bound pruning." << endl;
        SetDynamicUpperBound(false);
    }
}

void Fminer::SetDynamicUpperBound(bool val) {
    fm::adjust_ub=val; 
    if (fm::adjust_ub) {
        if (!GetPruning()) cerr << "Notice: Enabling statistical metrical pruning." << endl;
        SetPruning(true);
        if (!GetBackbone()) cerr << "Notice: Enabling mining for backbone refinement class representatives." << endl;
        SetBackbone(true);
    }
}



void Fminer::SetAromatic(bool val) {
    fm::aromatic = val;
}

void Fminer::SetRefineSingles(bool val) {
    fm::refine_singles = val;
}

void Fminer::SetDoOutput(bool val) {
    fm::do_output = val;
}


bool Fminer::GetConsoleOut(){return fm::console_out;}
bool Fminer::GetBbrcSep(){return fm::bbrc_sep;}
int Fminer::GetType(){return fm::type;}
int Fminer::GetMinfreq(){return fm::minfreq;}
bool Fminer::GetChisqSig(){return fm::chisq->sig;}
bool Fminer::GetBackbone(){return fm::do_backbone;}

bool Fminer::GetDynamicUpperBound(){return fm::adjust_ub;}
bool Fminer::GetPruning() {return fm::do_pruning;}
bool Fminer::GetAromatic() {return fm::aromatic;}
bool Fminer::GetRefineSingles() {return fm::refine_singles;}
bool Fminer::GetDoOutput() {return fm::do_output;}



