function [y] = SIGMOID(x)
K = 4.9;
y = (1/(1+exp(-K*x)) - 0.5)*2;
