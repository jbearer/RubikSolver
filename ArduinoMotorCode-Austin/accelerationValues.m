%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Acceleration Function %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

format compact

% User Defined Variables
accelTurns = 10;
accelTurns2 = 20;
decelTurns = 20;
decelTurns2 = 0;

accel = 1500.0; % Defines acceleration rate
accel2 = 3000.0;
decel = 4500.0;
decel2 = 2200.0;

% Other constants
numTurns = accelTurns + accelTurns2 + ...
           decelTurns + decelTurns2;
maxDeay = 900;

plusC_a = 16.0; % Integration constant
t_n = 0;
t_n_1 = 0;
first400 = false;

% Initialize deay array
deay = zeros(numTurns,1);

% Acceleration Phase (1)
for i = 1:accelTurns
   t_n_1 = 10000/(accel*10^(-6)*t_n + plusC_a) + t_n;
   deay(i) = t_n_1 - t_n;
   t_n = t_n_1;
   
%    if(deay(i) < 400 && first400 == false)
%       i
%       first400 = true;
%    end
end

t_n = 0;
plusC_a2 = 10^4 / deay(accelTurns);

% Acceleration Phase (1)
for i = accelTurns : accelTurns + accelTurns2
   t_n_1 = 10000/(accel2*10^(-6)*t_n + plusC_a2) + t_n;
   deay(i) = t_n_1 - t_n;
   t_n = t_n_1;
  
end


t_n = 0;
plusC_d = 10^4 / deay(accelTurns + accelTurns2);

% Develeration Phase
for j = accelTurns + accelTurns2 : ...
        accelTurns + accelTurns2 + decelTurns
   if (deay(j-1) > maxDeay) 
       deay(j) = deay(j-1);
       continue;
   end
   t_n_1 = 10000/(-decel*10^(-6)*t_n + plusC_d) + t_n;
   deay(j) = t_n_1 - t_n;
   t_n = t_n_1;
end

t_n = 0;
plusC_d2 = 10^4 / deay(accelTurns + accelTurns2 + decelTurns);

% Develeration Phase
for j = accelTurns + accelTurns2 + decelTurns : ...
        numTurns
   if (deay(j-1) > maxDeay) 
       deay(j) = deay(j-1);
       continue;
   end
   t_n_1 = 10000/(-decel2*10^(-6)*t_n + plusC_d2) + t_n;
   deay(j) = t_n_1 - t_n;
   t_n = t_n_1;
end

% Round to integer values
delay = round(deay);

% Print values
%table([1:numTurns]', round(deay))
fastest_us = min(delay)
%end_us = delay(end)
turnTime_ms = 2*sum(delay)*10^-3
solveTime_s = 30*turnTime_ms*10^-3


figure(1)
stem(deay,'-o');
grid on
title('Deay vs iteration')
xlabel('i')
ylabel('deay [us]')


t = zeros(numTurns,1);
TPS = 10000./deay;

for i = 2:numTurns
   t(i) = t(i-1) + deay(i); 
end

t = t .* 2* 10^-3;

figure(2)
stem(t,TPS)
grid on
xlabel('time [ms]')
ylabel('Angular Velocity [TPS]')
title('TPS vs time')
axis([0 max(t) 0 60])

