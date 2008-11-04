#include "database.h"
#include "constraints.h"

unsigned int minfreq;
int type;
bool do_backbone;
bool updated;
bool adjust_ub;
bool do_pruning;

string* outl=NULL;
Database* database=NULL;
Statistics* statistics=NULL;
ChisqConstraint* chisq=NULL;
vector<string>* result=NULL;
