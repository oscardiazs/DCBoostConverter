%Control

R1=330;
R2= sym('R2'); %vout
R3= sym('R3'); %pwm
VoutMax=25;
VoutMin=6;
VarMin=0;
VarMax=3.3;

f1 = 1.25*((R1*(R2+R3)+R2*R3)/(R1*R3))-VarMin*(R2)/(R3)==VoutMax;

f2 = 1.25*((R1*(R2+R3)+R2*R3)/(R1*R3))-VarMax*(R2)/(R3)==VoutMin;

S1= solve(f1,f2);
S1t = struct2table(S1);
S1f = table2array(S1t);
Rfinal=double(S1f);
disp(Rfinal);