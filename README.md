# Projeto - ZMP

Projeto de cálculo de referência de forças para rodas de um carro a partir de leitura de IMU de 9 GdL e feed back de RPM/Força das rodas.

A ideia é de implementação em Intel Galileo.

Até o presente momento, foi criado um código .ino com as funções de escrita/leitura em barramento CAN e leitura da IMU. Falta a rotina de interpretação dos dados lidos da CAN e cálculo de valor de referência para as rodas.