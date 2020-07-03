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
The current version only works with linux enviroments. T
To compile in the directory "Code" run the following: 

./make


*EXECUTION*
The command to execute VSD-MOEA is the following:
VSD-MOEA --n POPULATION_SIZE --nfes NUMBER_FUNCTION_EVALUATOINS --nvar NUMBER_VARIABLES --Instance PROBLEM_NAME --Path CURRENT_DIR --Dist_factor INITIAL_DISTANCE_FACTOR --nobj NUMBER_OBJECTIVES --Seed SEED_NUMBER --param_l DISTANCE_PARAMETERS (ONLY FOR WFG PROBLEMS) --param_k POSITION_PARAMETERS(ONLY FOR WFG PROBLEMS)

All the executions carried out in the paper can be performed by just specifying the appropiate parameters. 
For instance, in order to execute VSD-MOEA with DTLZ1 with the aim of generating the results of the first experiment 
(see Table 2 of the paper) the following command line must be used: 

./VSD-MOEA --n 100 --nfes 25000000 --nvar 6 --Instance DTLZ1 --Path . --Dist_factor 0.4 --nobj 2 --Seed 1 

