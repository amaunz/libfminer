/** @mainpage FMiner (libfminer)
 * 
 *  @section sec1 Abstract
 *  FMiner implements a method for efficiently mining relevant tree-shaped molecular fragments, each representing a geometrical classes, with minimum frequency and statistical constraints. Experimental results with four test datasets suggest that the approach is able to optimize correlation with chemical endpoints as well as inter-feature entropy. The mined fragments represent classes of fragments sharing a common core structure (backbone). The proposed method reduces feature set sizes and runtime by >90% and >60% compared to ordinary tree mining. Validation with several publicly available carcinogenicity datasets shows that their expressiveness is comparable to the complete set of tree-shaped fragments and is significantly higher than linear fragments.
 *
 *  \subsection ssec1 Licence
 *  See the file LICENSE.
 *
 *  @section sec2 Example program using the FMiner API
 *  The following code retrieves a vector of fragments along with statistical relevance and occurrences and prints them out. Every root node corresponds to a single chemical element. The output is in YAML format and takes the form
 *  \code
 *  - [ smarts,    p_chisq,    occ_list_active,    occ_list_inactive ]
 *  \endcode
 *
 * Documentation for YAML can be found at: http://yaml.org/spec/cvs/current.html
 *
 *  \code
 *  FMiner* fm= new FMiner();
 *
 *  fm->AddCompound ("COC1=CC=C(C=C1)C2=NC(=C([NH]2)C3=CC=CC=C3)C4=CC=CC=C4" , 1);
 *      // ... continue adding compounds
 *  fm->AddCompound ("O=C1NC(=S)NC(=O)C1C(=O)NC2=CC=CC=C2" , 4069);
 *
 *  fm->AddActivity((bool) true, 1);
 *      // ... continue adding activities (true for active, false for inactive)
 *  fm->AddActivity((bool) false, 4069);
 *
 *  for ( int j = 0; j < (int) fm->GetNoRootNodes(); j++ ) {
 *    vector<string>* result = fm->MineRoot(j);
 *    each (*result) {
 *      cout << (*result)[i] << endl;
 *    }
 *  }
 *
 *  delete fm;
 *
 *  \endcode
 *
 *  \author Andreas Maunz, 2008
 **/
