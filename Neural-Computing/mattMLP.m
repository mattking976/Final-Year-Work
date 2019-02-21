clc;
close all;
clear;
[matt] = audioread('matt.m4a');
[john] = audioread('john.m4a');
[faz] = audioread('faz.m4a');
[pierce] = audioread('pierce.m4a');
subplot(4,1,1)
plot(matt)
subplot(4,1,2)
plot(john)
subplot(4,1,3)
plot(faz)
subplot(4,1,4)
plot(pierce)

john = john(1:730048)';
john = john(100000:160000); 
john = john - min(john); 
john = john / max(john);
matt = matt(1:730048)';
matt = matt(100000:160000); 
matt = matt - min(matt); 
matt = matt / max(matt);
faz = faz(1:730048)';
faz = faz(100000:160000); 
faz = faz - min(faz);
faz = faz / max(faz);
pierce = pierce(1:730048)';
pierce = pierce(100000:160000); 
pierce = pierce - min(pierce);
pierce = pierce / max(pierce);

x = [john matt faz pierce];
d = [0 0; 0 1; 1 0; 1 1]'; 

network = feedforwardnet();
network.layers{1}.size = 10;
network.layers{2}.size = 2;
network = init(network);
network.trainFcn = 'traingdm';
network.trainParam.epochs = 500;
network.trainParam.lr = 0.05;
network.layers{1}.transferFcn = 'logsig';
network.layers{2}.transferFcn = 'logsig';
network.divideParam.trainRatio = 1; 
network.divideParam.valRatio = 0;
network.divideParam.testRatio = 0;
network=train(network,x,d);
Results = sim(network, x)