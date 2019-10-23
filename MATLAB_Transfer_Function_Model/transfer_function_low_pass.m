C = 10 * 10^(-6);
Rl = 8;
Rs = 50;
Rg = 82;
t = (-.001:.00001:.005);
u = heaviside(t);
h = (.000513)/(C*(Rs + Rg))*exp(-1/(Rl*C)*t) .* u;

figure;
ylim([0 1/(C*(Rs+Rg))]);
plot(t, h);
xlabel('Seconds');
ylabel('Volts');

