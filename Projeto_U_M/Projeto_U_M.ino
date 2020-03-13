#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//Declaração de Parametros
//-----------------------------------------------
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
//                          Identificação Unica por Caminhão
#define SERVICE_UUID        "d5875408-fa51-4763-a75d-7d33cecebc31"
#define CHARACTERISTIC_UUID "a4f01d8c-a037-43b6-9050-1876a8c23584"
//-----------------------------------------------


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

void enviaValorParametrizacao(String informacao) {
  funcaoDecisao(informacao);
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string dados = pCharacteristic->getValue();
      if (dados.length() > 0) {
        enviaValorParametrizacao(dados.c_str());
      }
    }
};
MyCallbacks myCallbacks;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("U&M Mineração");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->setCallbacks(&myCallbacks);
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Aguardando conexão com cliente");
}

void loop() {}
