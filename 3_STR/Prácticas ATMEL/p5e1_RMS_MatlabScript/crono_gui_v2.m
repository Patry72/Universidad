function varargout = crono_gui_v2(varargin)
% crono_gui_v2 MATLAB code for crono_gui_v2.fig
%      crono_gui_v2, by itself, creates a new crono_gui_v2 or raises the existing
%      singleton*.
%

%      H = crono_gui_v2 returns the handle to a new crono_gui_v2 or the handle to
%      the existing singleton*.
%
%      crono_gui_v2('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in crono_gui_v2.M with the given input arguments.
%
%      crono_gui_v2('Property','Value',...) creates a new crono_gui_v2 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before crono_gui_v2_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to crono_gui_v2_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help crono_gui_v2

% Last Modified by GUIDE v2.5 15-May-2019 10:34:42

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @crono_gui_v2_OpeningFcn, ...
                   'gui_OutputFcn',  @crono_gui_v2_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before crono_gui_v2 is made visible.
function crono_gui_v2_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to crono_gui_v2 (see VARARGIN)

% Choose default command line output for crono_gui_v2
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes crono_gui_v2 wait for user response (see UIRESUME)
% uiwait(handles.figure1);

%%maximize the gui
set(gcf, 'units','normalized','outerposition',[0 0 1 1]);
global confdata;
confdata.period=[];
confdata.tasks=[];
confdata.computation=[];
confdata.data=[];
confdata.gr_handle=[];
confdata.filename='';

%%Closing any port to avoid errors when openning
if (~isempty(instrfind)) fclose(instrfind);end;

ports = instrhwinfo('serial');
set(handles.popupmenu2,'String',ports.SerialPorts);

confdata.port=handles.popupmenu2.String{handles.popupmenu2.Value}

pushbutton3_Callback(hObject, eventdata, handles);

% --- Outputs from this function are returned to the command line.
function varargout = crono_gui_v2_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button LOAD DATA
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global confdata;    
[file,path]=uigetfile('*.txt','Data file');
    if isequal(file,0) || isequal(path,0)
           disp('User pressed cancel')
    else
       disp(['User selected ', fullfile(path, file)])
       confdata.filename=fullfile(path,file);
       [t,c,p,h,log]=show_crono_v3_starts(confdata.filename,0);
       confdata.tasks=t;
       confdata.computation=c;
       confdata.period=p;
       confdata.data=log;
       confdata.gr_handle=h;
      set(handles.uitable2,'Data',[c p]) ;
       
    end


% --- Executes on button press in pushbutton2.
function pushbutton2_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global confdata;
[t,c,p,h1,log]=show_crono_v3_starts(confdata.filename,0);
       confdata.tasks=t;
       confdata.computation=c;
       confdata.period=p;
       confdata.data=log;
       confdata.gr_handle1=h1;
      set(handles.uitable2,'Data',[c p]) ;
      
% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global confdata;
cla;
set(handles.uitable2,'Data',zeros(length(confdata.tasks),2)) ;


% --- Executes on button press in checkbox1.
function checkbox1_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox1
global confdata;
if (get(hObject,'Value')) confdata.gr_handle(1).Visible='on';
else confdata.gr_handle(1).Visible='off';
end
% --- Executes on button press in checkbox2.
function checkbox2_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox2
global confdata;
if (get(hObject,'Value')) confdata.gr_handle(2).Visible='on';
else confdata.gr_handle(2).Visible='off';
end


% --- Executes on button press in checkbox3.
function checkbox3_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox3
global confdata;
if (get(hObject,'Value')) confdata.gr_handle(3).Visible='on';
else confdata.gr_handle(3).Visible='off';
end


% --- Executes on button press in pushbutton4.
%%%%%% CONNECT BUTTON
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global confdata;
disp('Connecting to data, please wait...')
progressbar('Connecting to data, please wait...')
fprintf("Selected port: %s\n", confdata.port)
s=serial(confdata.port);
s.BaudRate=38400;
s.Terminator='CR';
fopen(s);
i=1;
max_length=confdata.maxtime;
progressbar('Executing tasks and waiting to receive data... this is not real-time ;)')
while (i<max_length)
    t=fgetl(s);
    
    progressbar(i/max_length)
    if (isempty(strfind(t,'END TRACE'))~=1)
        break;
    end
    
    if (i==1)
        pos=strfind(t,'INIT TRACE');
    else
        row = sscanf(t,'%u %u %u %u')
        m(i-1,:)=row';
    end
   i=i+1;
   
end
progressbar('Data already received, painting!!')
pause(1);
progressbar(1)
fclose(s)


[t,c,p,h1,log]=show_crono_v3_starts('',m);
       confdata.tasks=t;
       confdata.computation=c;
       confdata.period=p;
       confdata.data=log;
       confdata.gr_handle1=h1;
      set(handles.uitable2,'Data',[c p]) ;
      

% --- Executes on selection change in popupmenu1.
function popupmenu1_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu1


% --- Executes during object creation, after setting all properties.
function popupmenu1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu2
global confdata;
confdata.port=handles.popupmenu2.String{handles.popupmenu2.Value}

% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on slider movement.
function slider1_Callback(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

global confdata;
confdata.maxtime=handles.slider1.Value;
handles.edit1.String=confdata.maxtime;
disp(confdata.maxtime)

% --- Executes during object creation, after setting all properties.
function slider1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
global confdata;
confdata.maxtime=10000;
handles.edit1.String=confdata.maxtime;
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.

global confdata;
confdata.maxtime=10000;
handles.edit1.String=confdata.maxtime;

if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton5.
function pushbutton5_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global confdata;    
[file,path]=uiputfile('*.txt','Data file to Save');
    if isequal(file,0) || isequal(path,0)
           disp('User pressed cancel')
    else
       disp(['User selected ', fullfile(path, file)])
       confdata.filename=fullfile(path,file);
       disp('Saving the following data...')
       confdata.data
       confdata.filename
        m=confdata.data;
       save(confdata.filename,'m','-ascii')
       
       
    end
