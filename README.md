# Projeto - ZMP

## Introdução

Projeto de cálculo de referência de forças para rodas de um carro a partir de leitura de IMU de 9 GdL e feed back de RPM/Força das rodas.

A ideia é de implementação em Intel Galileo.

## Funções implementadas até o momento

Até o presente momento, foi criado um código ```.ino``` com as funções de escrita/leitura em barramento CAN e leitura da IMU. 

## Considerações para implementação

A leitura das mensagens foi feita considerando que a MCU das rodas dianteiras está emitindo mensagem com ID ```0x02``` e a MCU das rodas traseiras está emitindo mensagem com ID ```0x03```.

A mensagem que escreveremos contendo o cálculo de referência das rodas será enviada com ID ```0x04```.

Cada mensagem lida/escrita terá 8 bytes de tamanho.

### Organização dos bytes das mensagens lidas

Tanto para a leitura das rodas dianteiras quanto para a leitura das rodas traseiras, a organização de dados do pacote recebido deve ser considerada assim:

* ```leitura.data[1]``` e ```leitura.data[0]```: Leitura de força da roda direita
* ```leitura.data[3]``` e ```leitura.data[2]```: Leitura de força da roda esquerda
* ```leitura.data[5]``` e ```leitura.data[4]```: Leitura de RPM da roda direita
* ```leitura.data[7]``` e ```leitura.data[6]```: Leitura de RPM da roda esquerda

Para todos os dados acima, considerar o byte de índice maior como byte mais significativo e considerar bit de maior magnitude neste byte como bit de sinal.

Lembrando que os dados de ```leitura.data[]``` serão automaticamente escritos nos vetores ```read_dianteira[]``` e ```read_traseira[]```, então **NÃO MANIPULAR DIRETAMENTE ```leitura.data[]``` NA FUNÇÃO DE CÁLCULO, E SIM OS VETORES ```read_dianteira[]``` E ```read_traseira[]``` .**

### Organização dos bytes das mensagens escritas

Para escrita de dados, a organização de dados do pacote recebido deve ser considerada assim:

* ```escrita.data[1]``` e ```escrita.data[0]```: Escrita da referência de força da roda traseira direita
* ```escrita.data[3]``` e ```escrita.data[2]```: Escrita da referência de força da roda traseira esquerda
* ```escrita.data[5]``` e ```escrita.data[4]```: Escrita da referência de força da roda dianteira direita
* ```escrita.data[7]``` e ```escrita.data[6]```: Escrita da referência de força da roda dianteira esquerda

Para todos os dados acima, considerar o byte de índice maior como byte mais significativo e considerar bit de maior magnitude neste byte como bit de sinal.

Lembrando que os dados escritos no vetor ```writebyte[]``` serão transferidos pra a ```escrita.data[]``` na função de escrita, então **NÃO MANIPULAR DIRETAMENTE ```escrita.data[]``` NA FUNÇÃO DE CÁLCULO, E SIM O VETOR ```writebyte[]``` .**

## Pendências

Falta a rotina de interpretação dos dados lidos da CAN e cálculo de valor de referência para as rodas.

### O que fazer para resolver as pendências

Na função ```calculoRef()```:
* Transformar dados ```read_dianteira[]``` e ```read_traseira[]``` em ```float```;
* Usar dados convertidos e dados da leitura da IMU (```acc[]```, ```gyr[]``` e ```magne[]```) para fazer o cálculo de referência de força das rodas;
* Converter cálculo de cada roda em sequência de 2 bytes cada e gravar em ```writebyte[]``` na sequência estabelecida para ```escrita.data[]```
