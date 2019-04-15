function f = simple_fitness(x)
f= -(10+(-x.^2/10)+3*x)+50 +5*sin(2*x);