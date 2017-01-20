%% TIMINGS SERIAL

close all; clear all; clc
%For Npanels 20, error approx 5e-8

nbr_dompoints = 15*1e16;

nbr_mpi = [32; ...
               16; ...
               8; ...
               4; ...
               2; ...
               1];

nbr_omp = [1; ...
               2; ...
               4; ...
               8; ...
               16; ...
               32];    

           
time_sol = [3.0118; ...
            4.4998; ...
            4.4576; ...
            4.4671; ...
            4.3969; ...
            3.504337];
            
time_specq = [11.2847; ...
            9.2675; ...
            9.5978; ...
            7.3970; ...
            5.9774; ...
            3.045589];
            
time_tot = [32.366726; ...
            26.037017; ...
            26.911399; ...
            22.472373; ...
            17.600704; ...
            10.389191];
                   
figure(1); clf;
loglog(nbr_mpi./nbr_omp,time_tot,'.-','LineWidth',2); 
hold on; grid on
loglog(nbr_mpi./nbr_omp,time_sol,'^-','LineWidth',2)
loglog(nbr_mpi./nbr_omp,time_specq,'o-','LineWidth',2)
xlabel('$\frac{\# mpi}{\# omp}$','FontSize',20,'interpreter','latex')
ylabel('time [s]','FontSize',20,'interpreter','latex')
set(gca,'FontSize',17)
h = legend('Total time','Comp. solution','Special quad.');
set(h,'FontSize',17,'interpreter','latex')
title('Timings for hybrid code, nbr panels set to 20','FontSize',20,'interpreter','latex')
%loglog(nbr_mpi/nbr_omp,1./nbr_mpi/nbr_omp*1e1,'k:','LineWidth',2)
%h = legend('Total time','Comp. solution','Special quad.','O(1/N)');

%% BATCH 4 CRAYPAT
%close all; clear all; clc

%Calls 
% figure(2)
% callfun = [85.3 9.3 5.4];
% H = pie(callfun);
% h=legend('IPmultR', 'vandernewton','other');
% title('Function calls, omp code - $N_{dom} = 15\cdot 10^{6}$','FontSize',20,'interpreter','latex')
% set(h,'FontSize',15)

% figure(3)
% timefun = [35.3 30.2 10.3 10.1 5.3 8.8];
% H = pie(timefun);
% h = legend('exit','computeSolution','specialquadlapl','call\_gsl\_gmres','create\_grid','other');
% % colormap(jet)
% set(h,'FontSize',15)
% title('Time, omp code - $N_{dom} = 15\cdot 10^{6}$','FontSize',20,'interpreter','latex')
% legend('Location','northwest')
% %% BATCH 10 CrayPAT
% % %close all;
% % clear all; 
% % clc

% % %Calls 
% % figure(4)
% % callfun = [89.3 8.6 2.1];
% % H = pie(callfun);
% % h=legend('IPmultR', 'vandernewton','others');
% % title('Function calls, serial code - $N_{dom} = 3276800$','FontSize',20,'interpreter','latex')
% % set(h,'FontSize',15)

% % figure(5)
% % timefun = [47.6 38.5 10.8 3.1];
% % H = pie(timefun);
% % h = legend('specialquadlapl','computeSolution','vandernewton','others');
% % % colormap(jet)
% % set(h,'FontSize',15)
% % title('Time, serial code - $N_{dom} = 3276800$','FontSize',20,'interpreter','latex')