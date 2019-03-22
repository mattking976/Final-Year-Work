clc
close all

FitnessFunction = @simple_fitness;
numberOfVariables= 1;
opts.PopulationSize =10;
opts.Population = rand(3); %for 1 variable
opts.InitialPopulationRange = [0 32]; %for 1 variables
rng default % For reproducibility
thestate = rng;

[x,Fval,exitFlag,Output] =  ga(FitnessFunction, numberOfVariables ,[],[],[], [],[], [],[], opts);
 
fprintf('The number of generations was : %d\n', Output.generations);
fprintf('The number of function evaluations was : %d\n', Output.funccount);
fprintf('The best function value found was : %g\n', Fval);
fprintf('The best x value found was : %g\n', x);