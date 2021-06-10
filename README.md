# Projeto - ZMP

## Introdução

Projeto de cálculo de referência de forças para rodas de um carro a partir de leitura de IMU de 9 GdL e feed back de RPM/Força das rodas.

A ideia é de implementação em Intel Galileo.

## Funções implementadas até o momento

Até o presente momento, foi criado um código .ino com as funções de escrita/leitura em barramento CAN e leitura da IMU. 

## Considerações para implementação

A leitura das mensagens foi feita considerando que a MCU das rodas dianteiras está emitindo mensagem com ID 0x00 e a MCU das rodas traseiras está emitindo mensagem com ID 0x01.

A mensagem que escreveremos contendo o cálculo de referência das rodas será enviada com ID 0x02.

Cada mensagem lida/escrita terá 8 bytes de tamanho.

### Organização dos bytes das mensagens lidas

Tanto para a leitura das rodas dianteiras quanto para a leitura das rodas traseiras, a organização de dados do pacote recebido deve ser considerada assim:

* **leitura.data[1] e leitura.data[0]:** Leitura de força da roda direita
* **leitura.data[3] e leitura.data[2]:** Leitura de força da roda esquerda
* **leitura.data[5] e leitura.data[4]:** Leitura de RPM da roda direita
* **leitura.data[7] e leitura.data[6]:** Leitura de RPM da roda esquerda

Para todos os dados acima, considerar o byte de índice maior como byte mais significativo e considerar bit de maior magnitude neste byte como bit de sinal.

### Organização dos bytes das mensagens escritas

Para escrita de dados, a organização de dados do pacote recebido deve ser considerada assim:

* **escrita.data[1] e escrita.data[0]:** Escrita da referência de força da roda traseira direita
* **escrita.data[3] e escrita.data[2]:** Escrita da referência de força da roda traseira esquerda
* **escrita.data[5] e escrita.data[4]:** Escrita da referência de força da roda dianteira direita
* **escrita.data[7] e escrita.data[6]:** Escrita da referência de força da roda dianteira esquerda

Para todos os dados acima, considerar o byte de índice maior como byte mais significativo e considerar bit de maior magnitude neste byte como bit de sinal.

## Pendências

Falta a rotina de interpretação dos dados lidos da CAN e cálculo de valor de referência para as rodas.
