/* File : fminer.i */
%module fminer
%{
#include "fminer.h"
%}

class Fminer {
        static void Test();
        Fminer ();
        Fminer (int type, unsigned int minfreq);
        Fminer (int type, unsigned int minfreq, float chisq_val, bool do_backbone);
        bool AddCompound(string smiles, unsigned int comp_id);
        bool AddActivity(bool act, unsigned int comp_id);
        int GetNoRootNodes();
        int GetNoCompounds();
        vector<string>* MineRoot(int j);
        void SetDynamicUpperBound(bool val);
        void SetPruning(bool val);
};
