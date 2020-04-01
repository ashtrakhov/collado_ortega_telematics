chID = 38629;
[data,timestamps,chInfo] = thingSpeakRead(chID);

DiaInicio = datetime('03-28-2018', 'InputFormat', 'MM-dd-yyyy');
DiaFinal = datetime('04-03-2018', 'InputFormat', 'MM-dd-yyyy');

VectorDias = DiaInicio: DiaFinal;

if (VectorDias(end) ~= DiaFinal)
 VectorDias = [VectorDias, DiaFinal];
end

DatosTrafico = [];
DatosTiempo = [];

for dia = 1:length(VectorDias)-1
 RangoDias = [VectorDias(dia), VectorDias(dia+1)];
 [DatosCanal, t] = thingSpeakRead(chID, 'DateRange', RangoDias);
 [DatosTrafico] = [DatosTrafico; DatosCanal];
 [DatosTiempo] = [DatosTiempo; t];
 clear DatosCanal t
end

figure;
plot(DatosTiempo, DatosTrafico)
datetick
xlabel('Dia');
ylabel('Densidad de trafico');
grid on
title('Densidad de trafico entre los dias 28/03/2018 y 03/04/2018')
legend('Trafico de la zona Oeste','Trafico de la zona Este');

traficoOeste = DatosTrafico(:,1);
traficoEste = DatosTrafico(:,2);

numDias = length(VectorDias)-1;

numValores = length(traficoOeste);

cociente = floor(numValores/numDias);
valores_multiplos = numDias*cociente;
resta = numValores-valores_multiplos;
if (isequal(resta,0) == 0)
 traficoOeste(1:resta) =[];
 traficoEste(1:resta) = [];
end

mediaDiaOeste = mean(reshape(traficoOeste, cociente,[]));
mediaDiaEste = mean(reshape(traficoEste, cociente,[]));

%Figure con diferente color dependiendo de la intesidad de tráfico;
figure;
subplot(1,2,1);
for i=1:length(VectorDias)-1
 if(mediaDiaOeste(i)<20)
 bar(VectorDias(i), mediaDiaOeste(i),'facecolor','g');
 hold on;
 elseif(mediaDiaOeste(i)>25)
 bar(VectorDias(i), mediaDiaOeste(i),'facecolor','r');
 hold on;
 else
 bar(VectorDias(i), mediaDiaOeste(i),'facecolor','y');
 hold on;
 end
end
grid on
xlabel('Dia')
ylabel('Densidad de trafico medio por dia')
title('Densidad de trafico medio por dia en la zona Oeste. 2018')
datetick
subplot(1,2,2);
for i=1:length(VectorDias)-1
 if(mediaDiaEste(i)<20)
 bar(VectorDias(i), mediaDiaEste(i),'facecolor','g');
 hold on;
 elseif(mediaDiaEste(i)>25)
 bar(VectorDias(i), mediaDiaEste(i),'facecolor','r');
 hold on;
 else
 bar(VectorDias(i), mediaDiaEste(i),'facecolor','y');
 hold on;
 end
end
grid on
xlabel('Dia')
ylabel('Densidad de trafico medio por dia')
title('Densidad de trafico medio por dia en la zona Este. 2018')
datetick

startTime{1} = 'March 28, 2018 00:00:00';% weekend day
stopTime{1} = 'March 28, 2018 23:59:59';
startTime{2} = 'March 29, 2018 00:00:00';% weekend day
stopTime{2} = 'March 29, 2018 23:59:59';
startTime{3} = 'March 30, 2018 00:00:00';% weekend day
stopTime{3} = 'March 30, 2018 23:59:59';
startTime{4} = 'March 31, 2018 00:00:00';% weekend day
stopTime{4} = 'March 31, 2018 23:59:59';
startTime{5} = 'April 01, 2018 00:00:00';% weekend day
stopTime{5} = 'April 01, 2018 23:59:59';
startTime{6} = 'April 02, 2018 00:00:00';% weekend day
stopTime{6} = 'April 02, 2018 23:59:59';

%startTime{1} = '2018-03-28 00:00:00.000';% weekend day
%stopTime{1} = '2018-03-28 23:59:59.999';
%startTime{2} = '2018-03-28 00:00:00.000';% weekend day
%stopTime{2} = '2018-03-28 23:59:59.999';
%startTime{3} = '2018-03-28 00:00:00.000';% weekend day
%stopTime{3} = '2018-03-28 23:59:59.999';
%startTime{4} = '2018-03-28 00:00:00.000';% weekend day
%stopTime{4} = '2018-03-28 23:59:59.999';
%startTime{5} = '2018-04-01 00:00:00.000';% weekend day
%stopTime{5} = '2018-04-01 23:59:59.999';
%startTime{6} = '2018-04-02 00:00:00.000';% weekend day
%stopTime{6} = '2018-04-02 23:59:59.999';

numDias = length(startTime);

for ind = 1:numDias
    DiaInicio = datetime(startTime{ind}, 'InputFormat', 'MMMM d, yyyy HH:mm:ss ');
    DiaFinal = datetime(stopTime{ind}, 'InputFormat', 'MMMM d, yyyy HH:mm:ss ');
    %DiaInicio = datetime(startTime{ind}, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');
    %DiaFinal = datetime(stopTime{ind}, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');
    VectorDias = [DiaInicio, DiaFinal];
    [DatosDia, DatosTiempo] = thingSpeakRead(chID, 'DateRange', VectorDias);
    DatosOeste = DatosDia(:,1);
    DatosEste = DatosDia(:,2);
    DiaAnalisis = startTime{ind};
    DiaAnalisis = {DiaAnalisis(1:(end-8))};

    %instantes = datetime(DatosTiempo,'ConvertFrom','datenum');
    instantes = datetime(DatosTiempo,'InputFormat','yyyy-MM-dd HH:mm:ss.SSS');
    num_mediashoras = floor(length(DatosOeste)/48);
    instantes30min = downsample(DatosTiempo, num_mediashoras);
    for ind_30 = 1:48
        DatosOeste_30min(ind_30) = mean(DatosOeste(1+num_mediashoras*(ind_30-1):num_mediashoras*ind_30)); %#ok<*SAGROW>
        DatosEste_30min(ind_30) = mean(DatosEste(1+num_mediashoras*(ind_30-1):num_mediashoras*ind_30));
    end
    
    instantes30min(1) = [];
    instantes30min_conv = datetime(instantes30min, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');
    %instantes30min_conv=datetime(instantes30min,'ConvertFrom','datenum');
    
    [picosOeste,posicionOeste] = findpeaks(DatosOeste_30min, 'Threshold',0.3, 'MinPeakHeight', 5);
    [picosEste,posicionEste] = findpeaks(DatosEste_30min, 'Threshold',0.3, 'MinPeakHeight', 5);

    figure();
    subplot(1,2,1);
    findpeaks(DatosOeste_30min, instantes30min_conv, 'Threshold',0.3, 'MinPeakHeight', 5)
    datetick
    xlabel('Hora del dia')
    ylabel('Densidad de trafico medio cada 30 minutos')
    title(['Densidad de trafico en la zona Oeste. Dia', DiaAnalisis])

    subplot(1,2,2);
    findpeaks(DatosEste_30min, instantes30min_conv, 'Threshold',0.3, 'MinPeakHeight', 5)
    datetick
    xlabel('Hora del dia')
    ylabel('Densidad de trafico medio cada 30 minutos')
    title(['Densidad de trafico en la zona Eeste. Dia', DiaAnalisis])
    
    TiempoPicosOeste = instantes30min_conv(posicionOeste);
    TiempoPicosEste = instantes30min_conv(posicionEste);

end
