#Con este script se genera la lista de ejecuciones....
#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
my $file = "ExecutionScalability";
my $fout;
open($fout, '>' ,$file);
my $PathAlgorithm =  `cd ..; pwd`;#"/home/joel.chacon/Current/MyResearchTopics/MOEA-D-Diversity/MOEAD-DE/vsd-moead-opt";
chomp $PathAlgorithm;
#my $Path = "/home/joel.chacon/Chacon/Tesis/MOEA-D_Diversity/Code_CEC09/moead_based_diversity_SBX/";
my $Instance=0;
my $Sed=0;

####Realizar la búsqueda del parámetro D inicial que proporcione mejores resultados
my @Conf =(
"UF1 2",
"UF2 2",
"UF3 2",
"UF4 2",
"UF5 2",
"UF6 2",
"UF7 2",
"UF8 3",
"UF9 3",
"UF10 3",
"DTLZ1 2",
"DTLZ2 2",
"DTLZ3 2",
"DTLZ4 2",
"DTLZ5 2",
"DTLZ6 2",
"DTLZ7 2");#,
#"DTLZ1 3",
#"DTLZ2 3",
#"DTLZ3 3",
#"DTLZ4 3",
#"DTLZ5 3",
#"DTLZ6 3",
#"DTLZ7 3");

my @Variables= ("50","100","250", "500", "1000");
#my @Variables= ("500");

foreach my $v (@Variables)
{
        foreach my $configuration (@Conf)
        {
                for(my $Sed = 1; $Sed <=5; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
                {

		       my @configuration2 = split ' ', $configuration;#~ s/ /_/g; 
                       my $inst = $configuration2[0];
                       my $nobj = $configuration2[1];
		       print $fout "~$PathAlgorithm/Ejecutable --n 100 --nfes 250000000 --Instance $inst --nvar $v --nobj $nobj --Seed $Sed --Path $PathAlgorithm --Dist_factor 0.4 \n";
                }
        }

}
#########################WFG#
@Conf =(
"WFG1 2",
"WFG2 2",
"WFG3 2",
"WFG4 2",
"WFG5 2",
"WFG6 2",
"WFG7 2",
"WFG8 2",
"WFG9 2",
"WFG1 3",
"WFG2 3",
"WFG3 3",
"WFG4 3",
"WFG5 3",
"WFG6 3",
"WFG7 3",
"WFG8 3",
"WFG9 3");

foreach my $v (@Variables)
{
        foreach my $configuration (@Conf)
        {
                for(my $Sed = 1; $Sed <=5; $Sed++) ##Realizar 35 ejecuciones con distintas semilla de cada instancia..
                {

		      my @configuration2 = split ' ', $configuration;#~ s/ /_/g; 
                      my $inst = $configuration2[0];
                      my $nobj = $configuration2[1];
		      my $k = int((20.0/24.0)*$v);
		      my $l = $v-$k; 
		     print $fout "~$PathAlgorithm/Ejecutable --n 100 --nfes 25000000 --Instance $inst --nvar $v --nobj $nobj --Seed $Sed --Path $PathAlgorithm --Dist_factor 0.4 --param_l $l --param_k $k \n";
                }
        }
}

