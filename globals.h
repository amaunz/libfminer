#ifndef GLOBALS_H
#define GLOBALS H

#include "database.h"
#include "constraints.h"

namespace fm {

    unsigned int minfreq;
    int type;
    bool do_backbone;
    bool updated;
    bool adjust_ub;
    bool do_pruning;
    bool instance_present;
    bool console_out;
    bool aromatic;
    bool refine_singles;
    bool do_output;
    bool do_yaml;
    bool gsp_out;
    bool bbrc_sep;
    bool most_specific_trees_only;

    Database* database=NULL;
    Statistics* statistics=NULL;
    ChisqConstraint* chisq=NULL;
    GraphState* graphstate=NULL;
    CloseLegOccurrences* closelegoccurrences=NULL; 
    LegOccurrences* legoccurrences=NULL;
    vector<string>* result=NULL;

}

#endif
