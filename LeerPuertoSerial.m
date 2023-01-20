%se lee el puerto serial
function LeerPuertoSerial(Tiva, ~) %significa ignorar otras entradaas
global dato
dato= fscanf(Tiva, '%f');
end