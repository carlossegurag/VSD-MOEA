#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <memory.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "random.h"


using namespace std;

#define LINEAL 1
#define GEOMETRIC1 2
#define GEOMETRIC2 3
#define LOGARITHMIC 4
#define EXPONENTIAL 5

map<string, int>  model_id;
map<int, string> id_model;
//------------- Parameters in test instance ------------------
int Type_Model=1;
int     nvar=10,  nobj=2;                    //  the number of variables and objectives
int pops = 100; //population size

double  vlowBound[2000] ,   vuppBound[2000];   //  lower and upper bounds of variables
double nadir[2000], ideal[2000];

char    strTestInstance[556];
char    currentPATH[1500];
int param_l=5, param_k=5; // the distance and position parameters for the WFG problems..

long long int  max_nfes = 10000; //The function evaluation criteria is prefered than generations..
//------------- Parameters in random number ------------------
int     seed    = 177; //Default seed...
long    rnd_uni_init;        

double Initial_lowest_distance_factor=0.2*sqrt(nvar), lowestDistanceFactor, Final_time_diversity_promotion=0.5; 

//------------- Parameters in VSD-MOEA
double          scale[200];  

int		etax    = 2, 	etam    = 50;   // distribution indexes of crossover and mutation

double  realx=0.1,  realm = -1.0;    // crossover, mutation, selection probabilities
int run;
double EPS2=EPS;


#endif
