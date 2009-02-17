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

    Database* database=NULL;
    Statistics* statistics=NULL;
    ChisqConstraint* chisq=NULL;
    vector<string>* result=NULL;

}
