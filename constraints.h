// constraints.h
// Andreas Maunz, andreas@maunz.de, jun 2008
#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <set>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include "legoccurrence.h"
#include "database.h"

namespace fm {
    extern Database* database;
}

class Constraint {};

class ChisqConstraint : public Constraint {
    public:
    unsigned int na, ni, n;
    unsigned int fa, fi;
    float sig, chisq, p, u;
    bool active;
    set<Tid> fa_set, fi_set;

    ChisqConstraint (float sig) : na(0), ni(0), n(0), fa(0), fi(0), sig(sig), chisq(0.0), p(0.0), u(0.0), active(0) {}

    //!< Calculate chi^2 of current and upper bound for chi^2 of more specific features (see Morishita and Sese, 2000)
    template <typename OccurrenceType>
    void Calc(vector<OccurrenceType>& legocc) {

        chisq = 0.0; p = 0.0; u = 0.0;

        LegActivityOccurrence(legocc);
        fa = fa_set.size(); // fa is y(I) in Morishita and Sese
        fi = fi_set.size(); // fi is x(I)-y(I)  in Morishita and Sese

        // chisq_p for current feature
        p = ChiSq(fa+fi, fa);

        // upper bound u for chisq_p of more specific features
        float u1 = 0.0, u2 = 0.0;
        u1 = ChiSq(fa,fa);                                    // upper bound at
        u2 = ChiSq(fi,0);                                     // max{ chisq (y(I), y(I)) ,
        u = u1; if (u2>u1) u = u2;                            //      chisq (x(I)-y(I),0) }
    
    }


    private:

    //!< Calculates chi^2 and upper bound values
    float ChiSq(float x, float y);

    //!< Counts occurrences of legs in active and inactive compounds
    template <typename OccurrenceType>
    void LegActivityOccurrence(vector<OccurrenceType>& legocc) {

      fa_set.clear();
      fi_set.clear();

      each (legocc) { 

        if (fm::database->trees[legocc[i].tid]->activity == 1) {
            fa_set.insert(legocc[i].tid); 
        }

        else if (fm::database->trees[legocc[i].tid]->activity == 0) {
            fi_set.insert(legocc[i].tid); 
        }

      }

    }
    


};


#endif
