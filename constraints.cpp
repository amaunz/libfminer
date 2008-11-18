// constraints.cpp
// Andreas Maunz, andreas@maunz.de, jun 2008

#include "constraints.h"

float ChisqConstraint::ChiSq(float x, float y) {

        float ea = 0.0, ei = 0.0, impact = 0.0;
        
        impact = x/(float)n;
        ea = na * impact; 
        ei = ni * impact; 

        if (ea>0 && ei>0) chisq = (y-ea-0.5)*(y-ea-0.5)/ea + (x-y-ei-0.5)*(x-y-ei-0.5)/ei;

        return(chisq);

}
