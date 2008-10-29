#include "fminer.h"

//! Constructor for standard settings: 95% significance level, minimum frequency 2, type trees, dynamic upper bound, BBRC
FMiner::FMiner() : database(minfreq), chisq(0.95, &database), minfreq(2), type(2), do_backbone(false), updated(true), adjust_ub(true), do_pruning(true) {
  InitChisq();
}

//! Like standard constructor, but type and minimum frequency configurable
FMiner::FMiner(int type, unsigned int minfreq) : database(minfreq), chisq(0.95, &database), minfreq(minfreq), type(type), do_backbone(false), updated(true), adjust_ub(true), do_pruning(true) {
  if ((type != 1) && (type != 2)) { cerr << "Error! Invalid value '" << type << "' for parameter type." << endl; exit(1); }
  InitChisq();
}

//! Like standard constructor, but type, minimum frequency, significance level and BBRC configurable
FMiner::FMiner(int type, unsigned int minfreq, float chisq_val, bool do_backbone) : database(minfreq), chisq(chisq_val, &database), minfreq(minfreq), type(type), do_backbone(do_backbone), updated(true), adjust_ub(true), do_pruning(true) {
  if ((type != 1) && (type != 2)) { cerr << "Error! Invalid value '" << type << "' for parameter l." << endl; exit(1); }
  if (minfreq < 1) { cerr << "Error! Invalid value '" << minfreq << "' for parameter minfreq." << endl; exit(1); }
  if (chisq.sig < 0.0 || chisq.sig > 1.0) { cerr << "Error! Invalid value '" << chisq.sig << "' for parameter chisq." << endl; exit(1); }
  InitChisq();

}


//! Destructor
FMiner::~FMiner() {}
