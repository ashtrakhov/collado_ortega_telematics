CHANNEL_ID = 38629;
[data, timestamps, channel_info] = thingSpeakRead(CHANNEL_ID);

START_DAY = "03-28-2018";
END_DAY  = "04-03-2018";

start_day = datetime(START_DAY, "InputFormat", "MM-dd-yyyy");
end_day = datetime(END_DAY, "InputFormat", "MM-dd-yyyy");

day_vector = start_day:end_day;

% Correspondencia operadores MATLAB - C:
    % Operador      C       MATLAB
    % Igualdad      ==      ==
    % Distinto      !=      ~=
    % Módulo        %       mod(x, y)

if (day_vector(end) ~= end_day)
 day_vector = [day_vector, end_day];
end

traffic_data = [];
time_data = [];

for dia = 1:length(day_vector) - 1
 day_range = [day_vector(dia), day_vector(dia + 1)];
 [channel_data, time_marks] = thingSpeakRead(CHANNEL_ID, "DateRange", day_range);
 [traffic_data] = [traffic_data; channel_data];
 [time_data] = [time_data; time_marks];
 clear channel_data time_marks
end

figure;
plot(time_data, traffic_data)
datetick
xlabel("Day");
ylabel("Traffic Density");
grid on
title(sprintf("Traffic density between %s and %s", START_DAY, END_DAY))
legend("West zone traffic", "East zone traffic");

% Con estos índices conseguimos:
    % Guardar la primera columna de la matriz traffic_data en west_traffic
    % Guardar la segunda columna de la matriz traffic_data en east_traffic
west_traffic = traffic_data(:,1);
east_traffic = traffic_data(:,2);

num_days = length(day_vector) - 1;

num_values = length(west_traffic);

% Con la función floor() ignoramos el resto. Es equivalente a una división entera
% Vemos que por ejemplo -> floor(5 / 2) = 2
ratio = floor(num_values / num_days);

remainder = mod(num_values, num_days);

% Un par de corchetes vacíos eliminan la parte de la matriz referenciada
% En definitiva estamos eliminando los valores del principio del las
% matrices con para lograr que el número de datos sea divisible por el número
% de días.
if (remainder ~= 0)
 west_traffic(1:remainder) = [];
 east_traffic(1:remainder) = [];
end

% Fijamos el número de filas de west_traffic y east_traffic a ratio.
% Esto provoca que tengamos más columnas, exactamente 1 columna por día
% Hemos tenido que hacer lo anterior porque las medidas no son exactamente
% las mismas todos los días... Es por eso que no queda otra que eliminar lo
% que no encaje para que todo sea divisible...
% El parámetro [] significa que no está definido. Matlab nos obliga a pasar
% todos los argumentos al ser posicionales... Es como pasar NULL en C o
% None en Python
west_mean = mean(reshape(west_traffic, ratio, []));
east_mean = mean(reshape(east_traffic, ratio, []));

% Gráfica con diferente color dependiendo de la intesidad de tráfico;
figure;
subplot(1, 2, 1);
for i = 1:length(day_vector) - 1
    if (west_mean(i) < 20)
        bar(day_vector(i), west_mean(i), "facecolor", "g");
        hold on;
    elseif (west_mean(i) > 25)
        bar(day_vector(i), west_mean(i), "facecolor", "r");
        hold on;
    else
        bar(day_vector(i), west_mean(i), "facecolor", "y");
        hold on;
    end
end
grid on
xlabel("Day")
ylabel("Average traffic density per day")
title("Average traffic density per day in the West zone - 2018")
datetick

subplot(1, 2, 2);
for i = 1:length(day_vector) - 1
    if (east_mean(i) < 20)
        bar(day_vector(i), east_mean(i), "facecolor", "g");
        hold on;
    elseif (east_mean(i) > 25)
        bar(day_vector(i), east_mean(i), "facecolor", "r");
        hold on;
    else
        bar(day_vector(i), east_mean(i), "facecolor", "y");
        hold on;
    end
end
grid on
xlabel("Day")
ylabel("Average traffic density per day")
title("Average traffic density per day in the East zone - 2018")
datetick

% Rellenamos las matrtices de celdas start_time y stop_time con fechas
% Una matriz de celdas es un contenedor de celdas, pudiendo estas tener
% cualquier tipo de datos, incluso distintos. Si accedemos con {}
% obtenemos el valor de la celda que queremos pero si utilizamos ()
% se devolverá un subconjunto que a su vez es otro contenedor de celdas!

% En nuestro intervalo de tiempo el 31 de marzo y el 1 de abril eran fin de semana!

% sscanf() nos devolverá una matriz con 3 elementos en orden
% el mes, la fecha y el año de cada uno de los días que le pasemos
limit_days_data = sscanf(START_DAY, "%d-%d-%d");

for x = 1:num_days
    flag = false;
    curr_day = limit_days_data(2);
    curr_month = limit_days_data(1);
    curr_year = limit_days_data(3);

    % Necesitamos imprimir con un 'padding' de 0s para tener siempre 2 cifras en el día y en el mes
    start_time{x} = sprintf('%04d-%02d-%02d 00:00:00.000', curr_year, curr_month, curr_day);
    stop_time{x}  = sprintf('%04d-%02d-%02d 23:59:59.999', curr_year, curr_month, curr_day);

    if (curr_day == 30 && (curr_month == 4 || curr_month == 6 || curr_month == 9 || curr_month == 11))
        flag = true;

    elseif(curr_day == 28 && curr_month == 2)
        flag = true;

    elseif (curr_day == 31)
        flag = true;
    end

    if (flag)
        limit_days_data(2) = 1;
        limit_days_data(1) = limit_days_data(1) + 1;
    else
        limit_days_data(2) = limit_days_data(2) + 1;
    end
end

for ind = 1:num_days
    t_o = datetime(start_time{ind}, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');
    t_f = datetime(stop_time{ind}, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');

    % Concatenamos ambos vectores
    day_vector = [t_o, t_f];

    % Desempaquetamos los datos que llegan desde ThingSpeak
    [day_data, time_data] = thingSpeakRead(CHANNEL_ID, 'DateRange', day_vector);
    west_day_data = day_data(:,1);
    east_day_data = day_data(:,2);
    data_day = start_time{ind};
    data_day = {data_day(1:length('yyyy-MM-dd'))};


    % Un día tiene 48 periodos de 30 min!

    half_hours_per_day = 24 * 2;

    num_half_hours = floor(length(west_day_data) / half_hours_per_day);
    half_hour_instants = downsample(time_data, num_half_hours);
    foo_half_hour_instants = downsample(time_data, num_half_hours);
    for ind_30 = 1:half_hours_per_day
        west_data_half_hour(ind_30) = mean(west_day_data(1 + num_half_hours * (ind_30 - 1):num_half_hours * ind_30)); %#ok<*SAGROW>
        east_data_half_hour(ind_30) = mean(east_day_data(1 + num_half_hours * (ind_30 - 1):num_half_hours * ind_30));
    end
    
    % Borramos el primer elemento de los instantes recogidos con lo que el primer tiempo es aproximadamente 00:30 en vez de 00:00
    half_hour_instants(1) = [];

    half_hour_instants_conv = datetime(half_hour_instants, 'InputFormat', 'yyyy-MM-dd HH:mm:ss.SSS');

    figure;
    subplot(1, 2, 1);
    findpeaks(west_data_half_hour, half_hour_instants_conv, 'Threshold', 0.3, 'MinPeakHeight', 5)
    datetick
    xlabel('Time of the day')
    ylabel('Traffic density every 30 minutes')
    title(['Traffic density in the Western area for day ', data_day])

    subplot(1, 2, 2);
    findpeaks(east_data_half_hour, half_hour_instants_conv, 'Threshold', 0.3, 'MinPeakHeight', 5)
    datetick
    xlabel('Time of the day')
    ylabel('Traffic density every 30 minutes')
    title(['Traffic density in the Eastern area for day ', data_day])
end
