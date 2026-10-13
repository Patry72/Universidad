% Matlab Script for visualizing real-time scheduling process.
% by Cipriano Galindo 2017-2019
% -----------------------------------------------------------
%This version is purely for testing and teaching purposes
%It is not completely commented, debugged, nor optimized. It's as it's.
%Any question or doubt, I will be at cipriano@ctima.uma.es.
%
%Usage: show_crono_v2(filename)
%where filename contains a data file obtained by the execution of a given
%scheduling problem (see submited paper)
%
%Sample files can be download from http://mapir.isa.uma.es/work/rts-uma-aeb
%
%The structure of the file must be a n x 4 array with the following data:
% number_of_tasks 0 0 0
% task_id computation_time 0 0 
% ...
% task_id period_time 0 0
% ...
% TimerTick task_id_running x y 
    %(where x=10 indicates first run of the task within each period
    % x=3 indicates a task is trying to access to a resources (only one is
    % valid by now)
    % x=1 the task has taken the resource
    % x=2 the taks has release the resource.
    % y for future used. Now it's always 0)
% ...
%
%The crono_gui_v2.m graphical interface loads files and call this function
%It can also directly connect to the Arduino UNO + UMA-AEB device for
%getting the data via serial communication.

%%%% Version 3
%%%% -Default start vector
%%%% -Processing the matrix M to remove glithes: CPU given to a task
%%%%    for 1 ms at the end of its process.
function [task_ids,c_times,p_times,hgres,log]= show_crono_v3(filename,m)

%%%first activation of each task is taken also from the file or m. 
%%% Improvement:TODO: change the format is to set all the task info in one row, so change the
% format of m as:   num_tasks 0 0 0
%                               task1 computation_time period start
% this should be more compact....

    function list=compute_times(m)
        %% list contains: task_duration, task_id
        duration=0;
        task=m(1,2);
        pos=1;

        i=1;
        while (i<size(m,1))
            list(pos,3)=m(i,1);
            while m(i,2)==task && i<size(m,1)
                %%Sometimes there are two traces at the same time......
                if m(i,1)~=m(i+1,1) duration=duration+1;end
                i=i+1;
            end
              list(pos,1)=duration;
              list(pos,2)=task;
            
              pos=pos+1;
              duration=0;
              task=m(i,2);           
        end
        
    end

if (isempty(filename)~=1)
    m=load(filename);
end



max_length=1200; %%This should be passed as argument :S
%%I read the whole file and discard from time=max_length
max_index=find(m(:,1)>max_length);
if (max_index)
    m=m(1:max_index,:);
end
simulation_length=size(m,1);

log=m;

num_tasks=m(1,1);
fprintf('Num_tasks: %d\n',num_tasks);


task_ids=m(2:num_tasks+1,1);
fprintf('Tasks ids: ');
fprintf('%d ',task_ids);
fprintf('\n');

fprintf('Computation times: ')
c_times=m(2:num_tasks+1,2);
fprintf('%d ',c_times);
fprintf('\n');

fprintf('Periods: ')
p_times=m(num_tasks+2:2*num_tasks+1,2);
fprintf('%d ',p_times);
fprintf('\n');

fprintf('First_activations:');
start=m(2*num_tasks+2:3*num_tasks+1,2);
fprintf('%d ',start);
fprintf('\n');

%% data contains 3 rows per task 
data=m(3*num_tasks+2:size(m,1),1:2);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Finding and removing wrong behaviors
toremove=[];
for q=2:size(data,1)-2
    if (data(q,2)~=data(q-1,2))
       if (data(q+1,1)- data(q,1) >1)
           toremove(length(toremove)+1)=q;
       end
    end
end
data(toremove,:)=[];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Computing time consumption for each task
cdata=compute_times(data);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% painting tasks
cla;
hold on
color=[0.0 0.0 0.75];
color_start=[0.1,0.9,0];
color_taken=[1 0 0];
color_free=[0 1 0];

bar_height=0.3;

over_bar_height=bar_height+0.02;

text_height=bar_height+0.15;

activation_height=bar_height+0.2;

h1=hggroup;
    h2=hggroup;
    h3=hggroup;
    h4=hggroup;
    h5=hggroup;
    
for q=1:size(cdata,1)-1
    x=cdata(q,3);
    duration=cdata(q,1);
    id=cdata(q,2);
    operation=m(q,3);
    resource=m(q,4);    
    
    set(gca,'YTick',0:num_tasks);
    set(gca,'XLim',[0 simulation_length]);
    
    if (duration>=1 && id<10 && id>0)
        r=rectangle( 'Position',[x, -id, duration, bar_height], 'FaceColor',color);
        uistack(r,'bottom');
        text(x+duration/2,-id+text_height,num2str(duration),'FontSize',10);
    end
end

for q=1:size(m,1)-1
    id=m(q,2);
    operation=m(q,3);
    resource=m(q,4);
    
    x=m(q,1);
    
    if (operation==10)%%%%painting the start of each task (computation time 
       %%small green square
       plot(x+0.1,-id+bar_height,'sg','MarkerSize',7,'MarkerEdgeColor',[0 0 0],'MarkerFaceColor',color_start,'Parent',h1);
        %%add a small rectangle to distinguish several continous
        %%executions.
       rectangle( 'Position',[x, -id, 0.2, bar_height], 'FaceColor',[0 0 0]);
       
    end
    if (operation==1) %%% task taking a shared resource
        plot(x,-id+over_bar_height,'kv','MarkerFacecolor',color_taken,'MarkerSize',7,'Parent',h2);
    end
    
    if (operation==3 ) %%% task blocked when taking a resource
        plot(x,-id+over_bar_height,'kx','LineWidth',1,'MarkerFacecolor',color_taken,'MarkerSize',5,'Parent',h2);
        
    end

    if (operation==2) %%% task releasing a shared resource
        plot(x,-id+over_bar_height,'k^','MarkerFacecolor',color_free,'MarkerSize',7,'Parent',h2);
    end

end

%%First activation displaced by the action A is not fully implemented
%%nor automatized by the current solution
%%%%From desp=[2 2 4 2]; the task number (odd values) and the times
%%%%displaced (even values) should be extracted
%%%%By hand by now.....
%%tasks_AAction=[2 4] %%Takes the odd values of desp, that is the taskids
%%move_AAction=[2 2] %%2 means x2


%%%%showing activation and deadlines
max_time=max(data(:,1));
for (k=1:num_tasks)
    %% task id
    text(-150,-k+text_height/2,sprintf("TASK # %d",k),'FontSize',15);
    %% horizontal line    
    line ([0 max_time],[-task_ids(k) -task_ids(k)],'Color',[0 0 0]); 
    
    %% check for A Action tasks
   % found=find(tasks_AAction==k)
    additionaltime=start(k);
    
    %if (not(isempty(found)))
    %       timesmov=move_AAction(found)
    %       additionaltime=p_times(k)/(timesmov)
    %end    

    
        rep=floor(max_time/p_times(k)); %%number of task's activations

        
        for (j=0:rep)
            plot((j*p_times(k))+additionaltime,-task_ids(k)+activation_height,'b^','MarkerEdgeColor',[0 0 0],'MarkerFaceColor',[0,0,0],'Parent',h4);
            plot((j*p_times(k))+additionaltime,-task_ids(k)+0.005,'bv','MarkerEdgeColor',[0 0 0],'MarkerFaceColor',[0,0,0],'Parent',h4);
            line([(j*p_times(k))+additionaltime (j*p_times(k))+additionaltime],[-task_ids(k) -task_ids(k)+activation_height],'Color',[0,0,0],'Parent',h4);
            
        end
    
end

%uistack(h1,'top');
uistack(h4,'top');
hgres=[h1 h2 h3 h4 h5];

end