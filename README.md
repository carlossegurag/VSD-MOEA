*GENERAL INFORMATION*

This is the official source code of Variable Space Diversity based MOEA (VSD-MOEA).
VSD-MOEA is described in the paper
"VSD-MOEA: A Dominance-Based Multi-Objective Evolutionary Algorithm with Explicit Variable Space Diversity Management",
which was submitted to the Evolutionary Computation Journal.

Authors:
- Joel Chacón Castillo
- Carlos Segura
- Carlos Coello Coello

Coder:
- Joel Chacón Castillo

*COMPILATION*
The current version only works with linux environments.

In the directory "Code", run the following: 

./make


*EXECUTION*
The command to execute VSD-MOEA is the following:
VSD-MOEA --n POPULATION_SIZE --nfes NUMBER_FUNCTION_EVALUATIONS --nvar NUMBER_VARIABLES --Instance PROBLEM_NAME --Path CURRENT_DIR --Dist_factor INITIAL_DISTANCE_FACTOR --nobj NUMBER_OBJECTIVES --Seed SEED_NUMBER --param_l DISTANCE_PARAMETERS (ONLY FOR WFG PROBLEMS) --param_k POSITION_PARAMETERS(ONLY FOR WFG PROBLEMS)

All the executions carried out in the paper can be performed by just specifying the appropriate parameters. 
For instance, in order to execute VSD-MOEA with DTLZ1 with the aim of generating the results of the first experiment 
(see Table 2 of the paper) the following command line must be used: 

./VSD-MOEA --n 100 --nfes 25000000 --nvar 6 --Instance DTLZ1 --Path . --Dist_factor 0.4 --nobj 2 --Seed 1 

The results of each run can be accessed in the directory "VSD-MOEA/Code/POF", note that those results refer to the ten fronts reported along all the execution and the last front is at the last lines of each file.

##########################################################
This section has only been tested for linux environments  
##########################################################

*COMPUTING HV INFORMATION*

For the computation of each normalized HV value ratio, a reference set is required, which is provided in the directory "Optimals".
Particularly, each file represents an approximation of the optimal front, following the parameterization that is detailed in the main document the nomenclature of each optimal front is bellow:

 "INSTANCE_NOBJECTIVES.txt"

Note that depending on its parameterization some WFG problems might have a different Pareto location, and although almost all the document was run with the same configurations,
the Decision Variable Scalability Analysis requires a different set of reference points, in those cases (just WFG problems) the nomenclature is detailed bellow.
 
 "INSTANCE_NOBJECTIVES_NVARIABLES.txt"

For instance, the reference set for WFG1 with two objectives and 100 variables are defined in the file "Metrics/Optimals/WFG1_2_100.txt".
Similarly, for the main parameterization "Comparison against State-of-the-art MOEAs for long runs" the reference set for WFG1 with two objectives and 100 variables are defined in the file "Metrics/Optimals/WFG1_2.txt"

For the computation of the HV values the binary "hv" located in the directory "Metrics" can be run in the following way (only for linux with x64):
Given the files of the last front (which can be computed in a fast way and is indicated in the next section) the command to get the normalized HV value is:

  echo print `./hv -r "0.55 0.55" LastFronts/test_DTLZ1.txt`/`./hv -r "0.55 0.55" Optimals/DTLZ1_2.txt` | perl

This command takes into account a reference point of "0.55 0.55" and in this case the optimal information of DTLZ1 with two objectives is applied.
Note that the whole results are automatically computed with the script "VSD-MOEA/Metrics/MetaScripts/HV/calculateHV2.sh", where the reference point for each problem is defined.

*GETTING THE LAST FRONT*
The last front can be obtained running the command "importLastFronts.sh" that is located in the directory "Metrics". 
Note that this script assumes that the results are located in "Code/POF" and that the user is located in the path "Metrics"

*Statistical Tests*
Note that this script is provided with the aim of describing the statistical procedure applied in the validation. However, it requires all the data gathered along all the experiments, but it is not feasible to upload all of them to the repository.
Nevertheless, the results of the main section "Comparison against State-of-the-art MOEAs for long runs" can be accessed in the document "VSD-MOEA/Metrics/MetaScripts/HV".

If the results of one (or more) experiment(s) is(are) required please get in contact with Joel Chacón C. by  email "joel.chacon@cimat.mx".

The statistical tests are computed in python environments (versions>=3), with the script "VSD-MOEA/Metrics/MetaScriptskruskal_posthoc_MannWhitney_correc_hommel.py".

For instance, to run this script given the results reported by the section "Comparison against State-of-the-art MOEAs for long runs", the user first should be located in the directory "VSD-MOEA/Metrics/" then run the following script:

python3 kruskal_posthoc_MannWhitney_correc_hommel.py listfiles


Note that this script receives as an argument a file name (in this case "listfiles") that contains the HV values obtained by each algorithm in each problem, each line of this file defines a problem and each column refers to the file that save the 35 results (HV values) attained by each algorithm.

As an example of the first line of listfile, where the problem is the WFG1 and for the four algorithms (each one the 35 normalized HV values) is:

--WFG1_2 HV/VSDMOEA/WFG1_2 HV/CPDEA/WFG1_2 HV/MOEAD/WFG1_2 HV/R2EMOA/WFG1_2 

Note that the first column is just a description started by "--"


We encourage the reader that for any clarification please send an email to "joel.chacon@cimat.mx"
