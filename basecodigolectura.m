% Inicializar el puerto serial
s = serial('COM1');
s.BaudRate = 9600;
s.DataBits = 8;
s.StopBits = 1;
s.Parity = 'none';
s.Terminator = 'LF';
fopen(s);

% Crear vectores para almacenar los datos de cada se�al
data1 = [];
data2 = [];
data3 = [];
data4 = [];
data5 = [];
data6 = [];

% Iniciar un bucle infinito para leer las se�ales en tiempo real
while true
    % Leer seis valores desde el puerto serial
    values = fscanf(s, '%f %f %f %f %f %f');
    
    % Agregar los valores le�dos a los vectores de datos correspondientes
    data1 = [data1 values(1)];
    data2 = [data2 values(2)];
    data3 = [data3 values(3)];
    data4 = [data4 values(4)];
    data5 = [data5 values(5)];
    data6 = [data6 values(6)];
    
    % Graficar las se�ales en tiempo real
    subplot(3, 2, 1);
    plot(data1);
    ylabel('Se�al 1');
    xlabel('Muestra');
    subplot(3, 2, 2);
    plot(data2);
    ylabel('Se�al 2');
    xlabel('Muestra');
    subplot(3, 2, 3);
    plot(data3);
    ylabel('Se�al 3');
    xlabel('Muestra');
    subplot(3, 2, 4);
    plot(data4);
    ylabel('Se�al 4');
    xlabel('Muestra');
    subplot(3, 2, 5);
    plot(data5);
    ylabel('Se�al 5');
    xlabel('Muestra');
    subplot(3, 2, 6);
    plot(data6);
    ylabel('Se�al 6');
    xlabel('Muestra');
    drawnow;
end

% Cerrar el puerto serial
fclose(s);
