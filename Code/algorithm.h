/*==========================================================================
// //  Author: Carlos Segura, Joel Chacón 
//     Description: 
//
// ===========================================================================*/


#ifndef __EVOLUTION_H_
#define __EVOLUTION_H_

#include <queue>
#include <iomanip>
#include <cfloat>
#include <map>
#include "global.h"
#include "individual.h"

class MOEA
{

public:
	MOEA();
	virtual ~MOEA();

	void init_population();                  // initialize the population

	void evol_population();                                      
	void exec_emo(int run);

	void save_front(char savefilename[1024], bool overwrite=false);       // save the pareto front into files
	void save_pos(char savefilename[1024], bool overwrite=false);

        void penalize_nearest(vector<CIndividual *> &candidates, vector<CIndividual *> &penalized);
        void select_farthest_penalized(vector<CIndividual *> &candidates, vector<CIndividual *> &penalized);
        void select_best_candidate(vector<CIndividual *> &survivors, vector<CIndividual *> &candidates, vector<CIndividual *> &penalized);
        void compute_distances_variable(vector<CIndividual *> &candidates, vector<CIndividual *> &survivors);
        void compute_distances_objective(vector<CIndividual *> &candidates, vector<CIndividual *> &survivors);
        void binary_tournament_selection(vector<CIndividual > &population, vector<CIndividual> &child_pop);
        void recombination(vector<CIndividual> &child_pop);
        void reproduction(vector<CIndividual> &population, vector<CIndividual> &child_pop);
        void update_diversity_factor();
        void computing_dominate_information(vector <CIndividual*> &pool);
        void select_first_survivors(vector<CIndividual*> &survivors, vector<CIndividual*> &candidates);
        void back_select_first_survivors(vector<CIndividual*> &survivors, vector<CIndividual*> &candidates);
	void update_domianted_information(vector<CIndividual*> &survivors, vector<CIndividual*> &candidates, vector<CIndividual*> &penalized);
        void update_population(vector<CIndividual*> &survivors, vector<CIndividual> &population);
	void fast_non_dominated_sorting(vector <CIndividual*> &survivors);
        void real_sbx_xoverA(CIndividual &parent1, CIndividual &parent2, CIndividual &child1, CIndividual &child2);
        void realmutation(CIndividual &ind);
	double distance( vector<double> &a, vector<double> &b);
	double distance_improvement( vector<double> &a, vector<double> &b);
	vector <CIndividual> population;
	vector<CIndividual> child_pop;	// memory solutions
	void operator=(const MOEA &moea);

public:
//
//	// algorithm parameters
	long long int nfes;

};

MOEA::MOEA()
{

}

MOEA::~MOEA()
{

}
double MOEA::distance( vector<double> &a, vector<double> &b)
{
	double dist = 0.0 ;
   for(int i = 0; i < a.size(); i++)
	{
	   double factor = (a[i]-b[i])/(vuppBound[i] - vlowBound[i]);
	   dist += factor*factor;
	}
   return sqrt(dist);
}
double MOEA::distance_improvement( vector<double> &a, vector<double> &b)
{
	double dist = 0 ;
	double maxd = -INFINITY;
   for(int i = 0; i < a.size(); i++)
	{
	   double factor = max(0.0, a[i]-b[i]);
	   dist += factor*factor;
	}
   return dist;
}
void MOEA::init_population()
{
	
    for(int i=0; i<pops; i++)
	{
		CIndividual indiv1;
		// Randomize and evaluate solution
		indiv1.rnd_init();

		// Save in the population
		indiv1.rnd_init();
		indiv1.obj_eval();
		population.push_back(indiv1);

		child_pop.push_back(indiv1);
		nfes++;
	}
}
void MOEA::operator=(const MOEA &alg)
{
	//population = alg.population;
}
void MOEA::evol_population()
{
	vector<CIndividual *> penalized, survivors;
	//join the offspring and parent populations
	vector<CIndividual *> candidates;
	for(int i = 0; i < pops; i++)
	{	
	  candidates.push_back( &(population[i]));
	  candidates.push_back( &(child_pop[i]));
	}
	computing_dominate_information(candidates); //computing the dominate count of each candidate individual...
	//update the diversity-factor-parameter...	
	update_diversity_factor();
	//Pre-computing the neares distances both objective and decision spaces..
        select_first_survivors(survivors, candidates);
	compute_distances_variable(candidates, survivors);
	compute_distances_objective(candidates, survivors);

       	while( survivors.size() < pops )
	{
	  penalize_nearest(candidates, penalized);//penalize the nearest individuals.. 
	  if(candidates.empty())	  
	     select_farthest_penalized(survivors, penalized);//in case that all the individuals are penalized pick up the farstest and add it to survirvors
	  else
	    {
	     update_domianted_information(survivors, candidates, penalized); //update the rank of each candidate whitout penalized
	     select_best_candidate(survivors, candidates, penalized); // the best candidate is selected considering the improvemente distance, and the rank..
	    }
	}
	fast_non_dominated_sorting(survivors);//rank the survivors individuals..


	//this procedure is necesary since the penalized individuals
	update_population(survivors, population); //update the parent population 

	reproduction(population, child_pop); //generate a new population considering the survivors individuals...
}
void MOEA::fast_non_dominated_sorting(vector <CIndividual*> &survivors)
{
   vector< vector < int > > dominate_list(survivors.size()); //in the worst case the number of fronts is the same as the survivors size
   vector< int > dominated_count (survivors.size(), 0), currentfront;
   for(int i = 0; i < survivors.size(); i++)
   {
	   for(int j = 0; j < survivors.size(); j++)
	  {
		if(i==j) continue;
	       if( *(survivors[i]) < *(survivors[j]))
	   	    dominate_list[i].push_back(j);
		else if (*(survivors[j]) < *(survivors[i]))
		   dominated_count[i]++;
 	  }
	if(dominated_count[i] == 0 ) currentfront.push_back(i);// get the first front
   }
   int rank = 0;
   while(!dominate_list[rank].empty())
   {
	vector<int> nextFront;
	for(int i = 0; i < currentfront.size(); i++)
	{
	   survivors[currentfront[i]]->rank = rank;
	   for(int j = 0; j < dominate_list[currentfront[i]].size(); j++)
	   {
		dominated_count[dominate_list[currentfront[i]][j]]--;
		if( dominated_count[dominate_list[currentfront[i]][j]] == 0) nextFront.push_back(dominate_list[currentfront[i]][j]);
		
	   }
	}	
	rank++;
	currentfront = nextFront;
   }
}
void MOEA::update_population(vector<CIndividual*> &survivors, vector<CIndividual> &population)
{
	vector<CIndividual> pool;
   for(int i = 0; i < survivors.size(); i++) pool.push_back(*(survivors[i]));
   for(int i = 0; i < population.size(); i++) population[i] = pool[i];
}
void MOEA::update_domianted_information(vector<CIndividual*> &survivors, vector<CIndividual*> &candidates, vector<CIndividual*> &penalized)
{

     bool firstfrontcurrent = false; 
     for(int i = 0; i < candidates.size(); i++) if(candidates[i]->times_dominated==0) firstfrontcurrent = true; //check if there exists at least one candidate in the lowest current front
     
     if( !firstfrontcurrent) //this indicates that there is not able a current in the lowest front, so the next front is to be considered
	{	
	   for(int i = 0; i < survivors.size(); i++)
	   {
		if(survivors[i]->times_dominated == 0)
		{
		      for(int j = 0; j < survivors[i]->ptr_dominate.size(); j++)
		  	   {
		  		survivors[i]->ptr_dominate[j]->times_dominated--;
		   	   }
		  	   survivors[i]->times_dominated--;
		}
	   }
	   firstfrontcurrent = false;
	  back_select_first_survivors(survivors, candidates);
	  penalize_nearest(candidates, penalized);//penalize the nearest individuals.. 
	  compute_distances_objective(candidates, survivors);
	}
}
void MOEA::back_select_first_survivors(vector<CIndividual*> &survivors, vector<CIndividual*> &candidates)
{
	vector<int> idx(nobj);
	for(int i = 0; i < nobj; i++)idx[i]=i;
	random_shuffle(idx.begin(), idx.end());
	///Select the best improvement distance candidates....
	for(auto m:idx)
	{
		int indxmaxim = -1;
		double bestvector = DBL_MAX;
		for(int i = 0; i <  candidates.size(); i++)
		 {	
			if(candidates[i]->times_dominated != 0) continue; //just consider the first front
		        double s = 0.0;	
		        double maxv = -DBL_MAX;
		        for(int k = 0; k < nobj; k++)
		        {
		      	   double fi = fabs(candidates[i]->y_obj[k]);
		      	   s += fi;
		      	   double ti = (k==m)?fi:1e5*fi;
		            if(ti > maxv)   maxv=ti;
		        }
		         maxv = maxv + 0.0001*s;
		        if(bestvector > maxv && candidates[i]->nearest_variable_distance > lowestDistanceFactor)
		        { indxmaxim = i; bestvector = maxv;}
		  }
		if(indxmaxim != -1)
		{

		   for(int j = 0 ; j < candidates.size(); j++)
	           {
			if( j != indxmaxim) // Avoid to be updated by itself..
		        {
			 candidates[j]->nearest_variable_distance = min( candidates[j]->nearest_variable_distance, distance(candidates[j]->x_var, candidates[indxmaxim]->x_var ) );
			}
		   }
		   survivors.push_back( candidates[indxmaxim]);
		   iter_swap(candidates.begin()+indxmaxim, candidates.end()-1);
		   candidates.pop_back();
		}
	}
}
void MOEA::select_first_survivors(vector<CIndividual*> &survivors, vector<CIndividual*> &candidates)
{
	vector<int> idx(nobj);
	for(int i = 0; i < nobj; i++)idx[i]=i;
	random_shuffle(idx.begin(), idx.end());
	///Select the best improvement distance candidates....
	for(auto m:idx)
	{
		int indxmaxim = 0;
		double bestvector = DBL_MAX;
		for(int i = 0; i <  candidates.size(); i++)
		 {	
		        double s = 0.0;	
		        double maxv =  -DBL_MAX;
		        for(int k = 0; k < nobj; k++)
		        {
		           double fi = fabs(candidates[i]->y_obj[k]);
		           s += fi;
		           double ti = (k==m)?fi:1e5*fi;
		            if(ti > maxv)   maxv=ti;
		        }
		         maxv = maxv + 0.0001*s;
		        if(bestvector > maxv)
		        { indxmaxim = i; bestvector = maxv;}
		  }
		   survivors.push_back( candidates[indxmaxim]);
		   iter_swap(candidates.begin()+indxmaxim, candidates.end()-1);
		   candidates.pop_back();
	}
}
//get the rank of each individual...
void MOEA::computing_dominate_information(vector <CIndividual*> &pool)
{
    for(int i = 0; i < pool.size(); i++)
    {
	pool[i]->times_dominated = 0;
	pool[i]->ptr_dominate.clear();
	for(int j = 0; j < pool.size(); j++)
	{
	    if(i == j) continue;
	    if( *(pool[i]) < *(pool[j]) ) //the check if pop[i] dominates pop[j], tht '<' is overloaded
	    {
		pool[i]->ptr_dominate.push_back(pool[j]);
	    }
	    else if( *(pool[j]) < *(pool[i]) )
	   {
		pool[i]->times_dominated++;	
	   }
	}
    }
}
//updates the lowest distance factor of the diversity explicitly promoted
void MOEA::update_diversity_factor()
{

	double ratio = ((double) nfes)/max_nfes;
	if( Type_Model == LINEAL)
	{
	   lowestDistanceFactor = Initial_lowest_distance_factor - Initial_lowest_distance_factor*(ratio/Final_time_diversity_promotion);
	   if(ratio > Final_time_diversity_promotion) lowestDistanceFactor = 0.0;
	}
        else if( Type_Model == GEOMETRIC1)
	{
	   double alpha = Final_time_diversity_promotion;
	   lowestDistanceFactor = Initial_lowest_distance_factor*pow(alpha, ratio);
	}
	else if( Type_Model == GEOMETRIC2)
	{
	   double epsilon = 0.0001, alpha=0.9;
	   lowestDistanceFactor = Initial_lowest_distance_factor*pow(exp(log(epsilon/Initial_lowest_distance_factor)/Final_time_diversity_promotion), ratio);
	   if(ratio > Final_time_diversity_promotion) lowestDistanceFactor = 0.0;
	}		
	else if(Type_Model == EXPONENTIAL)
	{
	  double alpha=Final_time_diversity_promotion;
	  lowestDistanceFactor = Initial_lowest_distance_factor*exp(-6.0*pow(ratio,alpha));
	}
	else if(Type_Model == LOGARITHMIC)
	{
	   double alpha = 0.05;
	   lowestDistanceFactor = (Initial_lowest_distance_factor*alpha)/(log(1.0+ratio)/log(2.0));
	   lowestDistanceFactor -=Final_time_diversity_promotion;
	}
	
}
void MOEA::reproduction(vector<CIndividual> &population, vector<CIndividual> &child_pop)
{
   //binary tournament selction procedure
   binary_tournament_selection(population, child_pop);
   //recombination of the individuals, through the SBX code (taken from the nsga-ii code), also the evaluation of the population is performed
   recombination(child_pop); 
}
void MOEA::recombination(vector<CIndividual> &child_pop)
{
   vector<CIndividual> child_pop2 = child_pop;
	
   for(int i = 0; i < child_pop.size(); i+=2)
    {
       int indexa = i;
       int indexb = i+1;	
       real_sbx_xoverA( child_pop2[indexa], child_pop2[indexb], child_pop[i], child_pop[i+1]);//the crossover probability and index distribution eta are configured in the global.h file
       realmutation(child_pop[i]); //the index distribution (eta) and  mutation probability are configured in the global.h file
       realmutation(child_pop[i+1]);
       child_pop[i].obj_eval();
       child_pop[i+1].obj_eval();
    }
}
void MOEA::binary_tournament_selection(vector<CIndividual > &population, vector<CIndividual> &child_pop)
{
   for(int i = 0; i < population.size(); i++)
	{
	   int indexa = int(rnd_uni(&rnd_uni_init)*pops);
	   int indexb = int(rnd_uni(&rnd_uni_init)*pops);
	   if(population[indexa].rank < population[indexb].rank)
	      child_pop[i] = population[indexa];
	   else if(population[indexa].rank > population[indexb].rank)
	      child_pop[i] = population[indexb];
	   else 
	   {
	      child_pop[i] = (rnd_uni(&rnd_uni_init) < 0.5  )? population[indexa] : population[indexb];
	   }	
	}
}
void MOEA::compute_distances_variable(vector<CIndividual *> &candidates, vector<CIndividual *> &survivors)
{
	for(int i = 0; i < candidates.size(); i++)
	{
	    candidates[i]->nearest_variable_distance = INFINITY;
	   for(int j = 0; j < survivors.size(); j++)
		candidates[i]->nearest_variable_distance = min( candidates[i]->nearest_variable_distance, distance(candidates[i]->x_var, survivors[j]->x_var));
	}	
}
void MOEA::compute_distances_objective(vector<CIndividual *> &candidates, vector<CIndividual *> &survivors)
{
	for(int i = 0; i < candidates.size(); i++)
	{
	    candidates[i]->neares_objective_distance = INFINITY;
	   for(int j = 0; j < survivors.size(); j++)
	   {
		if(survivors[j]->times_dominated == 0)
		candidates[i]->neares_objective_distance = min( candidates[i]->neares_objective_distance, distance_improvement(survivors[j]->y_obj, candidates[i]->y_obj));
	   }
	}	

}
void MOEA::select_best_candidate(vector<CIndividual *> &survivors, vector<CIndividual *> &candidates, vector<CIndividual *> &penalized)
{
	int best_index_lastfront = -1;//the index of current with the farthes improvement distance
	double max_improvement = -INFINITY;
	  for(int i = 0 ; i < candidates.size(); i++)
	    {
		   if(candidates[i]->times_dominated != 0) continue;
			if(  max_improvement < candidates[i]->neares_objective_distance  )
			{
				max_improvement = candidates[i]->neares_objective_distance;
				best_index_lastfront= i;
			}
	    }
	 if(best_index_lastfront == -1) return; //this occurs when the first m-survirvors are dominated bewteen them, thus there are not candidates availables to pick, therefore this iteration is skiped, so in the next iteration will be available some candidates...

	//update distances of Current and penalized
	  for(int i = 0 ; i < candidates.size(); i++)
	   {
		if( i != best_index_lastfront) // Avoid to be updated by itself..
	        {
		 candidates[i]->nearest_variable_distance = min( candidates[i]->nearest_variable_distance, distance(candidates[i]->x_var, candidates[best_index_lastfront]->x_var ) );
		 candidates[i]->neares_objective_distance = min( candidates[i]->neares_objective_distance, distance_improvement(candidates[best_index_lastfront]->y_obj, candidates[i]->y_obj));
		}
	   }
	  for(int i = 0 ; i < penalized.size(); i++)
	  {
		penalized[i]->nearest_variable_distance = min( penalized[i]->nearest_variable_distance, distance( penalized[i]->x_var, candidates[best_index_lastfront]->x_var ) )  ;
		penalized[i]->neares_objective_distance  =  min( penalized[i]->neares_objective_distance, distance_improvement(candidates[best_index_lastfront]->y_obj, penalized[i]->y_obj));
	  }
	  survivors.push_back(candidates[best_index_lastfront]);
	  iter_swap(candidates.begin()+best_index_lastfront, candidates.end()-1);
	  candidates.pop_back();
}
void MOEA::select_farthest_penalized(vector<CIndividual *> &survivors, vector<CIndividual *> &penalized)
{
    	double largestDCN = -INFINITY;
	int index_largestDCN=0;
	for(int i = 0; i < (int)penalized.size(); i++) // get the index of penalized with larges DCN
	{
		if(penalized[i]->nearest_variable_distance >  largestDCN )
		{
			index_largestDCN = i;
			largestDCN = penalized[i]->nearest_variable_distance;
		}
	}

	for(int i = 0 ; i < (int)penalized.size(); i++) //update the nearest distance once that the penalized is moved to candidate (thereafter to survivors)
	{
		if( i != index_largestDCN )
		penalized[i]->nearest_variable_distance = min( penalized[i]->nearest_variable_distance, distance( penalized[i]->x_var, penalized[index_largestDCN]->x_var));
	}	

	survivors.push_back(penalized[index_largestDCN]);
	iter_swap(penalized.begin()+index_largestDCN, penalized.end()-1);
	penalized.pop_back();
}
void MOEA::penalize_nearest(vector<CIndividual *> &candidates, vector<CIndividual *> &penalized)
{

   	for(int i = candidates.size()-1; i >=0; i--)
	{	
		if( candidates[i]->nearest_variable_distance < lowestDistanceFactor )
		{
			penalized.push_back(candidates[i]);
			for(int j = 0; j < candidates[i]->ptr_dominate.size(); j++)
			{
				candidates[i]->ptr_dominate[j]->times_dominated--; //decreasing the times in which a survivor is dominated, this since penalized individuals are not considered..
			}
			//remove the candidate with index "i"
			iter_swap(candidates.begin()+i, candidates.end()-1);
			candidates.pop_back();
		}
	}
}
void MOEA::exec_emo(int run)
{
       char filename1[5024];
       char filename2[5024];
		seed = run;
	srand(seed);
//	seed = (seed + 23)%1377;
//	rnd_uni_init = -(long)seed;

	nfes      = 0;
	init_population(); //Initialize individuals...

	sprintf(filename1,"%s/POS/POS_VSD-MOEA_%s_RUN_%d_seed_%d_nvar_%d_nobj_%d_Di_%lf_nfes_%lldDf_%lf_pc_%lf_pm_%lf_Model_%s",currentPATH, strTestInstance,run, seed, nvar, nobj, Initial_lowest_distance_factor/sqrt(nvar), max_nfes, Final_time_diversity_promotion, realx, realm, id_model[Type_Model].c_str());
	sprintf(filename2,"%s/POF/POF_VSD-MOEA_%s_RUN_%d_seed_%d_nvar_%d_nobj_%d_Di_%lf_nfes_%lld_Df_%lf_pc_%lf_pm_%lf_Model_%s",currentPATH, strTestInstance,run, seed, nvar, nobj, Initial_lowest_distance_factor/sqrt(nvar), max_nfes, Final_time_diversity_promotion, realx, realm, id_model[Type_Model].c_str());
	save_front(filename2, true); //save the objective space information
	save_pos(filename1, true); //save the decision variable space information
        long long nfes1 = nfes, nfes2 = nfes;
        long long countnfes=0;
	while(nfes < max_nfes )
	{
	   nfes1=nfes;
		evol_population();
		nfes += pops;
	    
	    nfes2 = nfes;
	   countnfes += (nfes2 - nfes1);
	   if(  countnfes > 0.1*max_nfes  )
	    {	
	      countnfes -= 0.1*max_nfes;
              save_front(filename2); //save the objective space information
	      save_pos(filename1); //save the decision variable space information
	    }
	}
	save_pos(filename1); //save the decision variable space information
        save_front(filename2); //save the objective space information
	population.clear();
}
void MOEA::save_front(char saveFilename[1024], bool overwrite)
{
        std::fstream fout;
	if(overwrite)
	  fout.open(saveFilename);
	else
	  fout.open(saveFilename,fstream::app|fstream::out );
	for(int n=0; n<pops; n++)
	{
		for(int k=0;k<nobj;k++)
			fout<<population[n].y_obj[k]<<"  ";
		fout<<"\n";
	}
	fout.close();
}

void MOEA::save_pos(char saveFilename[1024], bool overwrite)
{
        std::fstream fout;
	if(overwrite)
	fout.open(saveFilename);
	else
	fout.open(saveFilename, fstream::app|fstream::out);
	for(int n=0; n<pops; n++)
	{
		for(int k=0;k<nvar;k++)
			fout<<population[n].x_var[k] << "  ";
		fout<<"\n";
	}
	fout.close();
}
/* Routine for real polynomial mutation of an T */
void MOEA::realmutation(CIndividual &ind)
{
    double rnd, delta1, delta2, mut_pow, deltaq;
    double y, yl, yu, val, xy;
	double eta_m = etam;

	int    id_rnd = int(rnd_uni(&rnd_uni_init)*nvar);

    for (int j=0; j<nvar; j++)
    {
        if (rnd_uni(&rnd_uni_init)<= realm)
        {
            y  = ind.x_var[j];
            yl = vlowBound[j];
            yu = vuppBound[j];
            delta1 = (y-yl)/(yu-yl);
            delta2 = (yu-y)/(yu-yl);
            rnd = rnd_uni(&rnd_uni_init);
            mut_pow = 1.0/(eta_m+1.0);
            if (rnd <= 0.5)
            {
                xy = 1.0-delta1;
                val = 2.0*rnd+(1.0-2.0*rnd)*(pow(xy,(eta_m+1.0)));
                deltaq =  pow(val,mut_pow) - 1.0;
            }
            else
            {
                xy = 1.0-delta2;
                val = 2.0*(1.0-rnd)+2.0*(rnd-0.5)*(pow(xy,(eta_m+1.0)));
                deltaq = 1.0 - (pow(val,mut_pow));
            }
            y = y + deltaq*(yu-yl);
            if (y<yl)
                y = yl;
            if (y>yu)
                y = yu;
            ind.x_var[j] = y;
        }
    }
    return;
}


/* Routine for real variable SBX crossover */
void MOEA::real_sbx_xoverA(CIndividual &parent1, CIndividual &parent2, CIndividual &child1, CIndividual &child2)
{
    double rand;
    double y1, y2, yl, yu;
    double c1, c2;
    double alpha, beta, betaq;
	double eta_c = etax;
    if (rnd_uni(&rnd_uni_init) <= realx) 
    {
        for (int i=0; i<nvar; i++)
        {
            if (rnd_uni(&rnd_uni_init)<=0.5 )
            {
                if (fabs(parent1.x_var[i]-parent2.x_var[i]) > EPS2)
                {
                    if (parent1.x_var[i] < parent2.x_var[i])
                    {
                        y1 = parent1.x_var[i];
                        y2 = parent2.x_var[i];
                    }
                    else
                    {
                        y1 = parent2.x_var[i];
                        y2 = parent1.x_var[i];
                    }
                    yl = vlowBound[i];
                    yu = vuppBound[i];
                    rand = rnd_uni(&rnd_uni_init);
                    beta = 1.0 + (2.0*(y1-yl)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c1 = 0.5*((y1+y2)-betaq*(y2-y1));
                    beta = 1.0 + (2.0*(yu-y2)/(y2-y1));
                    alpha = 2.0 - pow(beta,-(eta_c+1.0));
                    if (rand <= (1.0/alpha))
                    {
                        betaq = pow ((rand*alpha),(1.0/(eta_c+1.0)));
                    }
                    else
                    {
                        betaq = pow ((1.0/(2.0 - rand*alpha)),(1.0/(eta_c+1.0)));
                    }
                    c2 = 0.5*((y1+y2)+betaq*(y2-y1));
                    if (c1<yl)
                        c1=yl;
                    if (c2<yl)
                        c2=yl;
                    if (c1>yu)
                        c1=yu;
                    if (c2>yu)
                        c2=yu;
                    if (rnd_uni(&rnd_uni_init)<=0.5)
                    {
                        child1.x_var[i] = c2;
                        child2.x_var[i] = c1;
                    }
                    else
                    {
                        child1.x_var[i] = c1;
                        child2.x_var[i] = c2;
                    }
                }
                else
                {
                    child1.x_var[i] = parent1.x_var[i];
                    child2.x_var[i] = parent2.x_var[i];
                }
            }
            else
            {
                child1.x_var[i] = parent1.x_var[i];
                child2.x_var[i] = parent2.x_var[i];
            }
        }
    }
    else
    {
        for (int i=0; i<nvar; i++)
        {
            child1.x_var[i] = parent1.x_var[i];
            child2.x_var[i] = parent2.x_var[i];
        }
    }
    return;
}

#endif
