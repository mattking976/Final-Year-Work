clc;
close all;
[matt] = audioread('matt.m4a');
subplot(211)
plot(matt)

[john] = audioread('john.m4a');
subplot(212)
plot(john,'r')

john = john(1:730048);
john = john';
x = [john matt(:,1)];
d = [0 1];

learning_function='learnp';
transfer_function='logsig';
net = newp(x,d,transfer_function,learning_function);
net.trainParam.epochs = 1000;
net=train(net,x,d);
Results = sim(net, x)