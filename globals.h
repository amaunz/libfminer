#include "database.h"
#include "constraints.h"

unsigned int minfreq;
int type;
bool do_backbone;
bool updated;
bool adjust_ub;
bool do_pruning;

string* outl;
Database* database;
Statistics* statistics;
ChisqConstraint* chisq;
vector<string>* result;
