C = 10 * 10^(-6);
Rl = 8;
Rs = 50;
Rg = 82;
t = (-.001:.00001:.005);
t2 = (-.002:.00001:.010);
u = heaviside(t);
u0 = heaviside(t-.001);
t0 = .001;
s = u - u0;
h = (.000513)/(C*(Rs + Rg))*exp(-1/(Rl*C)*t) .* u;
Vl = 20*Rl/(Rs+Rg)*exp(-t/(Rl*C)).*(exp(t/(Rl*C)) - 1) .* (s);
Vl2 = 20.7*(Rl/(Rs+Rg)*((1+t0)^(1/(Rl*C)-1)))*exp(-t/(Rl*C)).*u0;
Vt = Vl+Vl2;
Vl3 = conv(s,h);

figure;
ylim([0 1/(C*(Rs+Rg))]);
plot(t, Vt);
xlabel('Seconds');
ylabel('Volts');

figure;
ylim([0 1/(C*(Rs+Rg))]);
plot(t2, Vl3);
xlabel('Seconds');
ylabel('Volts');