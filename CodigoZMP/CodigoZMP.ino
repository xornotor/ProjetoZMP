 /* 
 * Cálculo de referências de forças
 * Autores do Código:
 * - Ana Clara Malheiro Smera Batista
 * - André Paiva Conrado Rodrigues
 * 2021.1 - UFBA - Laboratório Integrado II
 */

//Importação de bibliotecas
#include <SPI.h>
#include <Wire.h>
#include <mcp2515.h>
#include <ICM_20948.h>

//Definição de constantes (ID's de mensagens CAN), Considerando
//0x02 ID da mensagem da MCU das rodas dianteiras
//0x03 ID da mensagem da MCU das rodas traseiras
//0x04 ID da mensagem da MCU de cálculo da ZMP (esta)
#define MCU_DIANT 0x02
#define MCU_TRAS 0x03
#define MCU_ZMP_TRAS 0x04
#define MCU_ZMP_DIANT 0x05

//Definição de structs e módulo CAN
struct can_frame leitura, escrita; //can_id, can_dlc, data[can_dlc]
byte read_dianteira[8], read_traseira[8], write_dianteira[8], write_traseira[8];
float acc[3], gyr[3], magne[3];
MCP2515 mcp2515(10);

//Definição de atributos da IMU 9DoF
#define WIRE_PORT Wire
#define AD0_VAL 1
ICM_20948_I2C myICM;

//Inicialização
void setup() {
  //Inicialização do módulo CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  escrita.can_dlc = 8;

  //Inicialização do módulo IMU
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);
  bool initialized = false;
  while (!initialized){
    myICM.begin(WIRE_PORT, AD0_VAL);
    if (myICM.status == ICM_20948_Stat_Ok){
      initialized = true;
    }
  }
}

//Loop
void loop() {
  while(!leituraCAN(MCU_DIANT));
  while(!leituraCAN(MCU_TRAS));
  while(!leituraIMU(&myICM));
  calculoRef();
  escritaCAN();
}

//Função de leitura do módulo CAN
bool leituraCAN(int id_busca){
  bool exito = false;
  if(mcp2515.readMessage(&leitura) == MCP2515::ERROR_OK){
    if(leitura.can_id == MCU_DIANT && id_busca == MCU_DIANT){
      if(exito == false) exito = true;
      for(int i = 0; i < leitura.can_dlc; i++) read_dianteira[i] = leitura.data[i];
    }else if(leitura.can_id == MCU_TRAS && id_busca == MCU_TRAS){
      if(exito == false) exito = true;
      for(int i = 0; i < leitura.can_dlc; i++) read_traseira[i] = leitura.data[i];
    }
  }
  return exito;
}

//Função de escrita pelo módulo CAN
void escritaCAN(){
  escrita.can_id = MCU_ZMP_DIANT;
  for(int i = 0; i < escrita.can_dlc; i++) escrita.data[i] = write_dianteira[i];
  mcp2515.sendMessage(&escrita);
  escrita.can_id = MCU_ZMP_TRAS;
  for(int i = 0; i < escrita.can_dlc; i++) escrita.data[i] = write_traseira[i];
  mcp2515.sendMessage(&escrita);
}

//Função de leitura do módulo IMU
bool leituraIMU(ICM_20948_I2C *sensor){
  bool exito = false;
  if(myICM.dataReady()){
    exito = true;
    myICM.getAGMT();
    acc[0] = sensor->accX();
    acc[1] = sensor->accY();
    acc[2] = sensor->accZ();
    gyr[0] = sensor->gyrX();
    gyr[1] = sensor->gyrY();
    gyr[2] = sensor->gyrZ();
    magne[0] = sensor->magX();
    magne[1] = sensor->magY();
    magne[2] = sensor->magZ();
  }
  return exito;
}

//Função de cálculo de força de referência das rodas
void calculoRef(){
  unsigned int ref[3], refDiantEsq, refDiantDir, refTrasEsq, refTrasDir, refAnguloEsq, refAnguloDir;
  for(int i = 0; i < 3; i++)
    ref[i] = abs(acc[i]*gyr[i]/magne[i]);
  refDiantEsq = (read_dianteira[3]*256) + read_dianteira[2])/(5*ref[0]);
  refDiantDir = (read_dianteira[1]*256) + read_dianteira[0])/(5*ref[1]);
  refTrasEsq = (read_traseira[3]*256) + read_traseira[2])/(5*ref[0]);
  refTrasDir = (read_traseira[1]*256) + read_traseira[0])/(5*ref[1]);
  refAnguloEsq = (read_dianteira[7]*256) + read_dianteira[6])/ref[2];
  refAnguloDir = (read_dianteira[5]*256) + read_dianteira[4])/ref[2];
  write_dianteira[0] = refDiantDir % 256;
  write_dianteira[1] = refDiantDir / 256;
  write_dianteira[2] = refDiantEsq % 256;
  write_dianteira[3] = refDiantEsq / 256;
  write_dianteira[4] = refAnguloDir % 256;
  write_dianteira[5] = refAnguloDir / 256;
  write_dianteira[6] = refAnguloEsq % 256;
  write_dianteira[7] = refAnguloEsq / 256;
  write_traseira[0] = refTrasDir % 256; 
  write_traseira[1] = refTrasDir / 256;
  write_traseira[2] = refTrasEsq % 256;
  write_traseira[3] = refTrasEsq / 256;
  write_traseira[4] = 0;
  write_traseira[5] = 0;
  write_traseira[6] = 0;
  write_traseira[7] = 0;
}
