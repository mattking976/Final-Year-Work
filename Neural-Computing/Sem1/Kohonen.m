clear;
close all

[train1, fs1]=audioread('train1.wav');
train1 = downsample(train1,2);

[train2, fs2]=audioread('train2.wav');

[train3, fs3]=audioread('train3.wav');
train3 = downsample(train3,2); 

fs=fs2;
Ts= 1/fs;

len_train1= length(train1);
len_train2= length(train2);
len_train3= length(train3);

t1=(0:len_train1-1)*Ts;
t2=(0:len_train2-1)*Ts;
t3=(0:len_train3-1)*Ts;

figure(1)
subplot(311)
plot(t1,train1)
xlabel('t, sec')
ylabel('amplitude')
grid
title('Train 1')

subplot(312)
plot(t2,train2, 'r')
xlabel('t, sec')
ylabel('amplitude')
grid
title('Train 2')

subplot(313)
plot(t3,train3, 'k')
xlabel('t, sec')
ylabel('amplitude')
grid
title('Train 3')

train1_spec= (2/len_train1)* abs(fft(train1));
train2_spec= (2/len_train2)* abs(fft(train2));
train3_spec= (2/len_train3)* abs(fft(train3));

f1=(0:len_train1 -1 ) /(len_train1*Ts);
f2=(0:len_train2 -1 ) /(len_train2*Ts);
f3=(0:len_train3 -1 ) /(len_train3*Ts);

figure(2)
subplot(311)
plot(f1,train1_spec)
xlabel('f, Hz')
ylabel('magnitude')
grid
title('Train 1')

subplot(312)
plot(f2,train2_spec)
xlabel('f, Hz')
ylabel('magnitude')
grid
title('Train 2')

subplot(313)
plot(f3,train3_spec)
xlabel('f, Hz')
ylabel('magnitude')
grid
title('Train 3')

fea1 = [0.01786,345.2, 0.01764,565.1,0.01599, 815.8]
fea2 = [0.01677, 94.98, 0.01255, 270.7, 0.01213, 208.4]
fea3 = [0.0226, 121.3, 0.01484, 480.6, 0.01229, 528.6]
p = [fea1;fea2;fea3]';


dimension = [10,10]; 
coverStep = 500; 
initNeighbor = 4;
topologyFcn = 'hextop';
distanceFcn = 'linkdist';
net=selforgmap(dimension,coverStep,initNeighbor,topologyFcn,distanceFcn);
[net,y]=train(net,p);

figure(3)
plotsompos(net,p)
figure(4)
plotsomnd(net)
figure(5)
plotsomhits(net,p)