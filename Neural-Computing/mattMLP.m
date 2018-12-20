clc;
close all;
clear;
%[insert,fs] = audioread('filename.wav')
[adam] = audioread('adam.m4a');
[john] = audioread('john.m4a');
[faz] = audioread('faz.m4a');
[pierce] = audioread('pierce.m4a');
%plot the graph
subplot(4,1,1)
plot(adam)
subplot(4,1,2)
plot(john)
subplot(4,1,3)
plot(faz)
subplot(4,1,4)
plot(pierce)

john = john(1:730048)';
john = john(100000:160000); %name = name(sample start point:sample end point)
john = john - min(john); % don't know what this does just add it
john = john / max(john);
adam = adam(1:730048)';
adam = adam(100000:160000); %name = name(sample start point:sample end point)
adam = adam - min(adam); % don't know what this does just add it
adam = adam / max(adam);
faz = faz(1:730048)';
faz = faz(100000:160000); %name = name(sample start point:sample end point)
faz = faz - min(faz); % don't know what this does just add it
faz = faz / max(faz);
pierce = pierce(1:730048)';
pierce = pierce(100000:160000); %name = name(sample start point:sample end point)
pierce = pierce - min(pierce); % don't know what this does just add it
pierce = pierce / max(pierce);

x = [john adam faz pierce];
d = [0 0; 0 1; 1 0; 1 1]'; 

%MLP
network = feedforwardnet();
network.layers{1}.size = 10;
network.layers{2}.size = 2;
network = init(network);
network.trainFcn = 'traingdm';
network.trainParam.epochs = 500;
network.trainParam.lr = 0.05;
network.layers{1}.transferFcn = 'logsig';
network.layers{2}.transferFcn = 'logsig';
network.divideParam.trainRatio = 1; %data used for training 100%
network.divideParam.valRatio = 0;
network.divideParam.testRatio = 0;
network=train(network,x,d);%data all voices
Results = sim(network, x)