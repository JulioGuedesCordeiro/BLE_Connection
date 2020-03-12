#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *caracteristica_para_envio_de_informacoes_inTX;

#define SERVICE_UUID           "ab0828b1-198e-4351-b779-901fa0e0371e"
#define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91"
bool controle_conexao = false;

// Recebe as informações e encaminha para a função decisão
void enviaValorParametrizacao(String informacao) {
  funcaoDecisao(informacao);
}

// Verificar conexões com dispositivos
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      controle_conexao = true;
    };

    void onDisconnect(BLEServer* pServer) {
      controle_conexao = false;
    }
};

// Monitora recebimentos de informações
class CharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
      std::string rxValue = characteristic->getValue(); 
      //verifica se existe dados a serem lidos
      if (rxValue.length() > 0) {
        enviaValorParametrizacao(rxValue.c_str());
      }
    }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32-BLE");
  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());
  BLEService *service = server->createService(SERVICE_UUID);

  caracteristica_para_envio_de_informacoes_inTX = service->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  caracteristica_para_envio_de_informacoes_inTX->addDescriptor(new BLE2902());

  BLECharacteristic *characteristic = service->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  characteristic->setCallbacks(new CharacteristicCallbacks());
  service->start();
  server->getAdvertising()->start();
  Serial.println("Aguardando a conexão");
}

void loop() {// Ainda sem qualquer uso, a mesma permanece por se tratar da infra}


// A implementação deve partir abaixo
// Seguir de preferência o Princípio da Responsabilidade Unica (SOLID), dessa forma garantiremos maior perfomance,
// confiabilidade e fácil leitura do código
void funcaoDecisao(String Info) {
  // Aqui pode-se criar a logica para enviar as informações para a placa de controle
  // deve-se se pensar ainda em uma melhor forma das informações serem enviadas
  // ou seja, conseguir em uma mesma string informar mais de um valor
  // um exemplo para isso seria em cada palavra(instrução) ter um separador /
  // poderiamos assim ter algo do tipo aceleracao/20/frente
  Serial.print(Info);
}
