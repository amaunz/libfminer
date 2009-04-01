/** @mainpage LibFminer (libfminer)
 *
 * LibFminer
 *
 * This is the Fminer library, available at http://github.com/amaunz/libfminer/tree/master.<br>
 * An Fminer frontend application is available from http://github.com/amaunz/fminer/tree/master.<br>
 * You may download the scientific documentation from http://www.maunz.de/ls_graph_mining_using_bbrcs.pdf.<br>
 * Contact details are located at the end of this page.
 * 
 *  @section sec1 Abstract
 *
 *  We present a new approach to large-scale graph mining
 *  based on so-called backbone refinement classes. The method
 *  efficiently mines tree-shaped subgraph descriptors under minimum
 *  frequency and significance constraints, using classes
 *  of fragments to reduce feature set size and running times.
 *  The classes are defined in terms of fragments sharing a common
 *  backbone. The method is able to optimize structural
 *  inter-feature entropy as opposed to occurrences, which is
 *  characteristic for open or closed fragment mining. In the
 *  experiments, the proposed method reduces feature set sizes
 *  by >90 % and >30 % compared to complete tree mining and
 *  open tree mining, respectively. Evaluation using crossvalidation
 *  runs shows that their classification accuracy is similar
 *  to the complete set of trees but significantly better than
 *  that of open trees. Compared to open or closed fragment
 *  mining, a large part of the search space can be pruned due
 *  to an improved statistical constraint (dynamic upper bound
 *  adjustment), which is also confirmed in the experiments in
 *  lower running times compared to ordinary (static) upper
 *  bound pruning. Further analysis using large-scale datasets
 *  yields insight into important properties of the proposed descriptors,
 *  such as the dataset coverage and the class size
 *  represented by each descriptor. A final cross-validation run
 *  confirms that the novel descriptors render large training sets
 *  feasible which previously might have been intractable.
 *
 *  \subsection ssec1 Licence
 *
 *   LibFminer is licensed under the terms of the GNU General Public License (GPL, see LICENSE). LibFminer is derived from (i.e., includes code from) the following project, licensed under GPL:
 * - Gaston: Siegfried Nijssen and Joost Kok. A Quickstart in Frequent Structure Mining Can Make a Difference. Proceedings of the SIGKDD, 2004 (http://www.liacs.nl/home/snijssen/gaston/)
 *
 *   LibFminer uses (i.e., links to) the following projects, also licensed under GPL:
 * - OpenBabel: The Open Babel Package, version 2.1.1 (http://openbabel.sourceforge.net/)
 * - GSL: GNU Scientific Library, version 0.1 (http://www.gnu.org/software/gsl/)
 *
 *   These licensing conditions mean, that you may only use LibFminer, in whatever form, under the condition that your source code is also freely available.
 *
 *  @section sec2 Installation
 *  LibFminer is a library, written in C++. It dynamically links to OpenBabel and GSL libraries. LibFminer is normally also built as dynamically loadable library. 
 *  Currently, there is no automated install procedure, you have to build libfminer from source. 
 *
 *  @subsection ssec21 OS Portability
 *  Windows DLL: 
 *  - Install Msys and MinGW (http://www.mingw.org/, then update gcc-core and gcc-g++ packages manually to the latest version). 
 *  - OpenBabel: follow the installation instrucations at http://openbabel.org/wiki/Install_(MinGW) to build yourself or download the binary DLLs at http://github.com/amaunz/openbabel-dll/tree (with git: <code>git clone git clone git://github.com/amaunz/openbabel-dll.git</code>).
 *  - GSL: Get the binary at http://gnuwin32.sourceforge.net/packages/gsl.htm
 *  - Get the source code at http://github.com/amaunz/libfminer/tree (with git: <code>git clone git clone git://github.com/amaunz/libfminer.git</code>). The <code>Makefile</code> automagically detects Windows. However, you have to adjust the include (-I) and linker (-L, -l) flags. Run <code>make</code>.
 *  - To create this documentation with doxygen, type 'make doc'. The documentation explains API, constructor usage and options.
 *
 *  Linux SO: install development tools (gcc, g++, GNU make) and GSL as well as OpenBabel development package, then compile LibFminer. On Ubuntu, you can e.g. do it like this:
 *  - Install build tools and GSL:
 *    \code
 *    apt-get install build-essential             # development tools
 *    apt-get install libgsl0-dev                 # GSL binary lib and headers
 *    \endcode
 *  - OpenBabel: follow the installation instrucations at http://openbabel.org/wiki/Install_(source_code) to build yourself after doing:
 *    \code
 *    apt-get build-dep libopenbabel-dev          # build dependencies for OB
 *    apt-get source libopenbabel-dev             # extracts OB source to the current dir
 *    \endcode
 *    or try the repository version:
 *    \code
 *    apt-get install libopenbabel-dev            # OB binary lib and headers
 *    \endcode
 *  - Get the source code at http://github.com/amaunz/libfminer/tree (with git: <code>git clone git clone git://github.com/amaunz/libfminer.git</code>). In the <code>Makefile</code>, adjust the include (-I) and linker (-L, -l) flags. Run <code>make</code>.
 *  - To create this documentation with doxygen, type 'make doc'. The documentation explains API, constructor usage and options.
 *  @subsection ssec22 Language Portability
 *  The API can be made available to other languages (currently only on Linux). Follow the installation instructions above. A config file for Swig to automagically create languages bindings exists (<code>fminer_wrap.i</code>). The Makefile also features a target that creates ruby bindings using this file. On Ubuntu, you can e.g. do this:
 *  - Swig: 
 *    \code
 *    apt-get install swig1.3 swig1.3-doc swig1.3-examples
 *    \endcode
 *  - Run <code>make ruby</code>.
 *  @section sec3 Example program using the LibFminer API
 *  LibFminer uses the 'singleton' design pattern known from software engineering, i.e., class instantiation is restricted to one object.
 *  The following code retrieves a vector of fragments along with statistical relevance and occurrences and prints them out. Every root node corresponds to a single chemical element. The output consists of gSpan graphs.
 *
 * Define the FMINER_SMARTS environment variable to produce output in SMARTS format. In this case, each line is a YAML sequence, containing SMARTS fragment, <i>p</i>-value, and two sequences denoting positive and negative class occurrences (line numbers in Smiles file): 
 *
 *  \code
 *  - [ smarts,    p_chisq,    occ_list_active,    occ_list_inactive ]
 *  \endcode
 *
 * Documentation for YAML can be found at: http://yaml.org/spec/cvs/current.html#
 *
 * Additionally define the FMINER_LAZAR environment variable to produce output in linfrag format which can be used as input to <code>Lazar</code>.
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
 * Fminer* MyFminer;
 * int main(int argc, char *argv[], char *envp) {
 *   MyFminer= new Fminer();
 *   MyFminer->AddCompound ("COC1=CC=C(C=C1)C2=NC(=C([NH]2)C3=CC=CC=C3)C4=CC=CC=C4" , 1);
 *   MyFminer->AddCompound ("O=C1NC(=S)NC(=O)C1C(=O)NC2=CC=CC=C2" , 2);
 *      // ... continue adding compounds
 *   MyFminer->AddActivity((bool) true, 1);
 *   MyFminer->AddActivity((bool) false, 2);
 *      // ... continue adding activities (true for active, false for inactive)
 *   cerr << MyFminer->GetNoCompounds() << " compounds" << endl;
 *   // Toy example: special settings for mining all fragments
 *   MyFminer->SetChisqSig(0); // use no significance constraint
 *   MyFminer->SetRefineSingles(true); // refine structures with support 1
 *   // gather results for every root node in vector instead of immediate output
 *   MyFminer->SetConsoleOut(false);
 *   for ( int j = 0; j < (int) MyFminer->GetNoRootNodes(); j++ ) {
 *      vector<string>* result = MyFminer->MineRoot(j);
 *      for( int i = 0; i < result->size(); i++) {
 *        cout << (*result)[i] << endl;
 *      }
 *   }
 *   delete MyFminer;
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
 * MyFminer = Fminer::Fminer.new()
 * MyFminer.AddCompound("COC1=CC=C(C=C1)C2=NC(=C([NH]2)C3=CC=CC=C3)C4=CC=CC=C4" , 1)
 * MyFminer.AddCompound("O=C1NC(=S)NC(=O)C1C(=O)NC2=CC=CC=C2" , 2)
 *    # ... continue adding compounds
 * MyFminer.AddActivity(true, 1)
 * MyFminer.AddActivity(false, 2)
 *    # ... continue adding activities (true for active, false for inactive)
 * print MyFminer.GetNoCompounds()  
 * puts " compounds"
 * # Toy example: special settings for mining all fragments
 * # use no significance constraint
 * MyFminer.SetChisqSig(0) 
 * # refine structures with support 1
 * MyFminer.SetRefineSingles(true) 
 * # gather results for every root node in vector instead of immediate output
 * MyFminer.SetConsoleOut(false)
 * (0 .. MyFminer.GetNoRootNodes()-1).each do |j|
 *    result = MyFminer.MineRoot(j)
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
 * The following parameters and switches are normally configured via the constructor. At a later point, you may change those settings via public setter methods and get their values via public getter methods, see the <code>Fminer</code> API.
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
 * @section Contact Contact
 * Dipl.-Inf. Andreas Maunz<br>
 * Freiburg Center for Data Analysis and Modelling<br>
 * Hermann-Herder-Str. 3a<br>
 * 79104 Freiburg, Germany<br>
 * Phone: +49761/203-8442, Fax: +49761/203-7700<br>
 * Email: maunza@fdm.uni-freiburg.de<br>
 * Web: http://cs.maunz.de
 *
 *  \author © 2008 by Andreas Maunz, 2008
 **/
