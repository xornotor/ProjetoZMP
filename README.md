# Projeto - ZMP

## Introdução

Projeto de cálculo de referência de forças para rodas de um carro a partir de leitura de IMU de 9 GdL e feed back de RPM/Força das rodas.

A ideia é de implementação em Intel Galileo.

## Funções implementadas até o momento

Até o presente momento, foi criado um código ```.ino``` com as funções de escrita/leitura em barramento CAN e leitura da IMU.

## Considerações para implementação

A leitura das mensagens foi feita considerando que a MCU das rodas dianteiras está emitindo mensagem com ID ```0x02``` e a MCU das rodas traseiras está emitindo mensagem com ID ```0x03```.

A mensagem que escreveremos contendo os dados de referência das rodas *traseiras* será enviada com ID ```0x04```, e a mensagem que escreveremos contendo os dados de referência das rodas *dianteiras* será enviada com ID ```0x05```.

Cada mensagem lida/escrita terá 8 bytes de tamanho.

### Organização dos bytes das mensagens lidas

Tanto para a leitura das rodas dianteiras quanto para a leitura das rodas traseiras, a organização de dados do pacote recebido deve ser considerada assim:

* ```leitura.data[1]``` e ```leitura.data[0]```: Leitura de força da roda direita
* ```leitura.data[3]``` e ```leitura.data[2]```: Leitura de força da roda esquerda
* ```leitura.data[5]``` e ```leitura.data[4]```: Leitura de ângulo da roda direita (dianteira)
* ```leitura.data[7]``` e ```leitura.data[6]```: Leitura de ângulo da roda esquerda (dianteira)

Para todos os dados acima, considerar o byte de índice maior como byte mais significativo e considerar bit de maior magnitude neste byte como bit de sinal.

Lembrando que os dados de ```leitura.data[]``` serão automaticamente escritos nos vetores ```read_dianteira[]``` e ```read_traseira[]```, então **NÃO MANIPULAR DIRETAMENTE ```leitura.data[]``` NA FUNÇÃO DE CÁLCULO, E SIM OS VETORES ```read_dianteira[]``` E ```read_traseira[]``` .**

### Organização dos bytes das mensagens escritas

Para escrita de dados, a organização de dados do pacote recebido deve ser considerada assim:

* ```escrita.data[1]``` e ```escrita.data[0]```: Escrita da referência de força da roda direita
* ```escrita.data[3]``` e ```escrita.data[2]```: Escrita da referência de força da roda esquerda
* ```escrita.data[5]``` e ```escrita.data[4]```: Escrita da ângulo da roda direita (dianteira)
* ```escrita.data[7]``` e ```escrita.data[6]```: Escrita da ângulo da roda esquerda (dianteira)

Para todos os dados acima, considerar o byte de índice maior como byte mais significativo e considerar bit de maior magnitude neste byte como bit de sinal.

Lembrando que os dados presentes nos vetores ```write_dianteira[]``` e ```write_traseira[]``` serão transferidos pra a ```escrita.data[]``` na função de escrita, então **NÃO MANIPULAR DIRETAMENTE ```escrita.data[]``` NA FUNÇÃO DE CÁLCULO, E SIM OS VETORES ```write_dianteira[]``` E ```write_traseira[]``` .**

### Função de cálculo de referência

**ATENÇÃO: ESTE NÃO É UM CÁLCULO REAL DE REFERÊNCIA DE TORQUE DAS RODAS. O TRABALHO FOI FEITO SEM CONHECIMENTO DE SISTEMAS DE CONTROLE E A ROTINA ADOTADA É APENAS UM CÁLCULO ARBIRTRÁRIO QUE NÃO CONDIZ COM ROTINAS REAIS DE CÁLCULO DE REFERÊNCIA DE TORQUE DE RODAS. POR FAVOR, DESCONSIDERE A SIGNIFICÂNCIA DE QUALQUER DADO NUMÉRICO RESULTANTE DO CÁLCULO A SEGUIR.**

Na função ```calculoRef()```:

* Transformação de dados ```read_dianteira[]``` e ```read_traseira[]``` em ```int```;
* Uso de dados convertidos e dados da leitura da IMU (```acc[]```, ```gyr[]``` e ```magne[]```) para fazer o cálculo de referência de torque das rodas;
* Conversão de cálculo de cada roda e cálculo dos ângulos (rodas dianteiras) em sequência de 2 bytes cada e gravação em ```write_dianteira[]``` e ```write_traseira[]``` na sequência estabelecida para ```escrita.data[]```.
