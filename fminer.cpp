#include "fminer.h"
#include "graphstate.h"

FMiner::FMiner() : database(minfreq), chisq(0.95, &database), minfreq(2), type(2), do_backbone(false), updated(true), adjust_ub(true), do_pruning(true), init_mining_done(false) {
  Init();
}

FMiner::FMiner(int type, unsigned int minfreq) : database(minfreq), chisq(0.95, &database), minfreq(minfreq), type(type), do_backbone(false), updated(true), adjust_ub(true), do_pruning(true), init_mining_done(false) {
  if ((type != 1) && (type != 2)) { cerr << "Error! Invalid value '" << type << "' for parameter type." << endl; exit(1); }
  Init();
}

FMiner::FMiner(int type, unsigned int minfreq, float chisq_val, bool do_backbone) : database(minfreq), chisq(chisq_val, &database), minfreq(minfreq), type(type), do_backbone(do_backbone), updated(true), adjust_ub(true), do_pruning(true), init_mining_done(false) {
  if ((type != 1) && (type != 2)) { cerr << "Error! Invalid value '" << type << "' for parameter l." << endl; exit(1); }
  if (minfreq < 1) { cerr << "Error! Invalid value '" << minfreq << "' for parameter minfreq." << endl; exit(1); }
  if (chisq.sig < 0.0 || chisq.sig > 1.0) { cerr << "Error! Invalid value '" << chisq.sig << "' for parameter chisq." << endl; exit(1); }
  Init();

}


vector<string>* FMiner::MineRoot(int j) {
    if (!init_mining_done) {
                        each (database.trees) {
                            if (database.trees[i]->activity == -1) {
                                cerr << "Error! ID " << database.trees[i]->orig_tid << " is missing activity information." << endl;
                                exit(1);
                            }
                        }
                        database.edgecount (); database.reorder (); initLegStatics (); graphstate.init (); 
                        if (!do_pruning || !do_backbone) fm->SetDynamicUpperBound(false); init_mining_done=true; 
                    }

    result.clear();
    if ( database.nodelabels[j].frequency >= minfreq && database.nodelabels[j].frequentedgelabels.size () ) {
        Path path(j);
        path.expand(); // mining step
    }

    return &result;
}

bool FMiner::AddCompound(string smiles, unsigned int comp_id) {
    bool insert_done=false;
    if (comp_id<=0) { cerr << "Error! IDs must be of type: Int > 0." << endl;}
    else {
        if (database.readTree (smiles, comp_no, comp_id, comp_runner)) {
            insert_done=true;
            comp_no++;
        }
        else { cerr << "Error on compound " << comp_runner << ", id " << comp_id << "." << endl; }
        comp_runner++;
    }
    return insert_done;
}

bool FMiner::AddActivity(bool act, unsigned int comp_id) {
    if (database.trees_map[comp_id] == NULL) { 
        cerr << "No structure for ID " << comp_id << ". Ignoring entry!" << endl; return false; 
    }
    else {
        if ((database.trees_map[comp_id]->activity = act)) AddChiSqNa();
        else AddChiSqNi();
        return true;
    }
}
