
% Obtener la planta en Lazo Cerrado
%%  Preprocesameinto de la señal para ser llevado al system indentification
pi100f = table2array(PI100);
%plot(pi100f);
salida100 = pmovil(pi100f,20);
salida100 = salida100(41:705);
salida100 = salida100-11.2878;
salida100 = salida100./(5.209);
salida100ref = ones(length(salida100),1);
ts = 0.008;
t = 0:ts:ts*(length(salida100)-1);
plot(t,salida100);
hold on;
plot(t,salida100ref);
%%  Controlador en discreto
z = tf('z',ts);
Gcl = (1.069e-6 * z^-1)/(1-3.933*z^-1 + 5.801*z^-2 -3.805*z^-3 + 0.936*z^-4);
syms z p;
Control = 100*z/(z-1);
dPlant = solve((1.069e-6 * z^-1)/(1-3.933*z^-1 + 5.801*z^-2 -3.805*z^-3 + 0.936*z^-4) == (100*z/(z-1) * p)/(1+ (100*z/(z-1))*p),p)

%% Obtener la planta en continuo 
syms s pc;
Gcon = (0.8131*s+246.4)/(1*s^4 + 8.013*s^3 + 52.87*s^2 + 165.6*s + 246.9);
ControlCon = (100*s+1.25e4)/s; %Controlador diseñado en continuo
CloseLoCon = ControlCon*pc/(1+ControlCon*pc);
Plant = solve(Gcon == CloseLoCon,pc);
s = tf('s');
PlantS = syms2tf(Plant); %Pasar de simbólica a tf.
rlocus(PlantS);

%% 

function [y] = pmovil (x, M)
%Moving average
%Sintax:
%  y = pmovil(x, M)
%Inputs:
%  x,    vector with input sequence
%  M,    length of moving average filter
%Outputs:
%  y,    processed sequence

%Edit here %%%%%%%%%%%%%%%%%%%%
u(1:2*M+1)=1;
hn=1/(2*M + 1)*u;
y=conv(x,hn);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
function[ans] = syms2tf(G)
[symNum,symDen] = numden(G); %Get num and den of Symbolic TF
TFnum = sym2poly(symNum);    %Convert Symbolic num to polynomial
TFden = sym2poly(symDen);    %Convert Symbolic den to polynomial
ans =tf(TFnum,TFden);
end


