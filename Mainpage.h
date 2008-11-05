/** @mainpage LibFminer (libfminer)
 *
 * LibFminer
 *
 * This is the Fminer library.
 * The official Fminer application that uses this library is available from http://github.com/amaunz/fminer/tree/master.
 * Latest scientific documentation is available from http://github.com/amaunz/fminer-doc/tree/master.
 * To create documentation with doxygen, do 'make doc'. The documentation explains API, constructor usage and options.
 * 
 *  @section sec1 Abstract
 *  LibFminer implements a method for efficiently mining relevant tree-shaped molecular fragments, each representing a geometrical classes, with minimum frequency and statistical constraints. Experimental results with four test datasets suggest that the approach is able to optimize correlation with chemical endpoints as well as inter-feature entropy. The mined fragments represent classes of fragments sharing a common core structure (backbone). The proposed method reduces feature set sizes and runtime by >90% and >60% compared to ordinary tree mining. Validation with several publicly available carcinogenicity datasets shows that their expressiveness is comparable to the complete set of tree-shaped fragments and is significantly higher than linear fragments.
 *
 *  \subsection ssec1 Licence
 *
 *   LibFminer is free software ('free' as in 'freedom', see LICENSE). LibFminer uses code from the following projects which are free in the same sense:
 * - Gaston: Siegfried Nijssen and Joost Kok. A Quickstart in Frequent Structure Mining Can Make a Difference. Proceedings of the SIGKDD, 2004 (http://www.liacs.nl/home/snijssen/gaston/)
 * - OpenBabel: The Open Babel Package, version 2.1.1 http://openbabel.sourceforge.net/ (accessed Jul 2008)
 *
 *  @section sec2 Portability
 *  LibFminer is a self-contained library, written in C++. It depends on OpenBabel (<code>libopenbabel</code>) and GNU Scientific Library (<code>libgsl</code>) libraries.
 *  \subsection Portability
 *  The dependency libraries are available for Linux, Mac and Windows, so porting to the latter two platforms should be easy. A Windows-Version will also be released by the author in the near future.
 *  The API can be made available to other languages. A config file for Swig to automagically create languages bindings exists (<code>fminer.i</code>). The Makefile features a target that creates ruby bindings using this file (<code>make fminer.so</code>).
 *
 *  @section sec3 Example program using the LibFminer API
 *  The following code retrieves a vector of fragments along with statistical relevance and occurrences and prints them out. Every root node corresponds to a single chemical element. The output is in YAML format and takes the form
 *  \code
 *  - [ smarts,    p_chisq,    occ_list_active,    occ_list_inactive ]
 *  \endcode
 *
 * Documentation for YAML can be found at: http://yaml.org/spec/cvs/current.html
 *
 * \subsection CPP C++
 *
 *
 * \code
 * #include "fminer.h"
 * #include <iostream>
 * #include <string.h>
 * using namespace std;
 *
 * Fminer* fm;
 * int main(int argc, char *argv[], char *envp) {
 *   fm= new Fminer();
 *   AddCompound ("COC1=CC=C(C=C1)C2=NC(=C([NH]2)C3=CC=CC=C3)C4=CC=CC=C4" , 1);
 *      // ... continue adding compounds
 *   AddCompound ("O=C1NC(=S)NC(=O)C1C(=O)NC2=CC=CC=C2" , 4069);
 *   AddActivity((bool) true, 1);
 *      // ... continue adding activities (true for active, false for inactive)
 *   AddActivity((bool) false, 4069);
 *   cerr << GetNoCompounds() << " compounds" << endl;
 *   for ( int j = 0; j < (int) GetNoRootNodes(); j++ ) {
 *      vector<string>* result = MineRoot(j);
 *      for( int i = 0; i < result->size(); i++) {
 *        cout << (*result)[i] << endl;
 *      }
 *   }
 *   delete fm;
 * }
 *
 *  \endcode
 *
 * \subsection Ruby Ruby
 *
 * This example assumes that you have created ruby bindings using <code>make fminer.so</code>.
 * \code
 *
 * require 'fminer'
 * fm = Fminer::Fminer.new()
 * fm.AddCompound("COC1=CC=C(C=C1)C2=NC(=C([NH]2)C3=CC=CC=C3)C4=CC=CC=C4" , 1)
 *    // ... continue adding compounds
 * fm.AddCompound("O=C1NC(=S)NC(=O)C1C(=O)NC2=CC=CC=C2" , 4069)
 * fm.AddActivity(true, 1)
 *    // ... continue adding activities (true for active, false for inactive)
 * fm.AddActivity(false, 4096)
 * print fm.GetNoCompounds()  
 * puts " compounds"
 * (0 .. fm.GetNoRootNodes()-1).each do |j|
 *    result = fm.MineRoot(j)
 *    puts "Results"
 *    (0 .. result.size-1).each do |i|
 *        puts result[i]
 *   end
 * end
 *
 *  \endcode
 *
 * \section Const Description of Constructors and Options

 * The above program uses the standard settings:
 * \code
 *  //! Constructor for standard settings: 95% significance level, minimum frequency 2, type trees, dynamic upper bound, BBRC
 *  Fminer ();
 *  \endcode

 * There also exist more flexible constructors:
 * \code
 * //! Like standard constructor, but type and minimum frequency configurable
 * Fminer (int type, unsigned int minfreq);
 * //! Like standard constructor, but type, minimum frequency, significance level and BBRC configurable
 * Fminer (int type, unsigned int minfreq, float chisq_val, bool do_backbone);
 * \endcode

*
 * \subsection Options Options Description
 *
 * The following parameters and switches are normally configure via the constructor. At a later point, you may change those settings via public setter methods and get their values via public getter methods, see the <code>Fminer</code> API.
 *
 * Constraint parameters:
 *  - Subgraph type, choices are paths and trees. <code>type</code> may be  1 (paths) and 2 (trees). Default is 2.
 *  - Minimum frequency constraint, used for anti-monotonic pruning. <code>minfreq</code> may be 1,2,... Default is 2.
 *  - Chi-square significance level, used for statistical upper-bound pruning. <code>chisq_val</code> may be 0.0<=chisq_val<=1.0. Default is .95.
 *
 * Constraint switches:
 *  - Switch off mining of only the most significant/most general representative of each backbone. <code>do_backbone</code> may be <code>true</code> or <code>false</code>. Default is <code>false</code>.
 *
 * Some pruning options can also be switched off:
 * \code
 * void SetDynamicUpperBound(bool val) {adjust_ub=val;} //!< Pass 'false' here to disable dynamic upper bound pruning (e.g. for performance measures).
 * void SetPruning(bool val) {do_pruning=val;} //!< Pass 'false' here to disable statistical pruning completely.
 * \endcode
 *

 * Define the FMINER_LAZAR environment variable to produce output in Linfrag format which can be used as input to Lazar.

 * Andreas Maunz, 2008
 *
 *  \author Andreas Maunz, 2008
 **/
