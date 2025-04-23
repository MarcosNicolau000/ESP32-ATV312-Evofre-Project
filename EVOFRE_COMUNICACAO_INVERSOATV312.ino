#include <WiFi.h>
#include <ModbusMaster.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// Configuração do RS485
#define RX_PIN 16
#define TX_PIN 17
#define RE_DE_PIN 15

// Configuração do Wi-Fi
const char *ssid = "Biel07";  
const char *password = "biel0007";    

// ModbusMaster e servidor
ModbusMaster node;
AsyncWebServer server(80);

// Variáveis de controle
uint16_t frequencyValue = 600;  // Frequência padrão (60 Hz * 10)

// Funções para controlar o RS485
void preTransmission() {
  digitalWrite(RE_DE_PIN, HIGH);  // Ativa transmissão
}

void postTransmission() {
  digitalWrite(RE_DE_PIN, LOW);  // Ativa recepção
}

void setup() {
  // Inicialização serial e RS485
  Serial.begin(115200);
  Serial1.begin(19200, SERIAL_8E1, RX_PIN, TX_PIN);

  pinMode(RE_DE_PIN, OUTPUT);
  digitalWrite(RE_DE_PIN, LOW);  // Modo recepção padrão

  node.begin(1, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi. IP: " + WiFi.localIP().toString());

  // Página inicial
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = R"rawliteral(
      <!DOCTYPE html>
      <html lang="en">
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Controle ESP32</title>
        <style>
          body { font-family: Arial, sans-serif; background-color: #373737; margin: 0; padding: 0; display: flex; flex-direction: column; justify-content: center; align-items: center; min-height: 100vh; }
          .logo { margin-bottom: 20px; width: 40%; max-width: 400px; height: auto; }
          .gradient-border { position: relative; background: linear-gradient(90deg, #131313, #3d3d3d); background-size: 300% 300%; border-radius: 12px; padding: 6px; animation: gradient-animation 10s ease infinite; }
          .container { background-color: #ffffff; border-radius: 10px; padding: 20px; max-width: 400px; width: 90%; text-align: center; }
          h1 { font-size: 24px; color: #333333; }
          button { background-color: #2b2b2b; color: white; border: none; padding: 10px 20px; margin: 5px; border-radius: 5px; cursor: pointer; font-size: 16px; transition: background-color 0.3s ease; }
          button:hover { background-color: #00b3d3; }
          input { padding: 8px; margin: 5px 0; width: calc(100% - 20px); font-size: 16px; border: 1px solid #ccc; border-radius: 5px; }
          label { font-size: 14px; color: #555555; display: block; margin-bottom: 5px; }
          p { font-size: 14px; color: #333333; }
          @keyframes gradient-animation { 0% { background-position: 0% 50%; } 50% { background-position: 100% 50%; } 100% { background-position: 0% 50%; } }
        </style>
      </head>
      <body>
        <img src="https://i.postimg.cc/63cb4VkV/LOGO-WHITE.png" class="logo" alt="Logo">
        <div class="gradient-border">
          <div class="container">
            <h1>Controle do Motor via Modbus</h1>
            <div>
              <button onclick="sendCommand('RESET')">RESET</button>
              <button onclick="sendCommand('SISTEMA')">INÍCIO</button>
            </div>
            <br>
            <div>
              <button onclick="sendCommand('HORARIO')">HORÁRIO</button>
              <button onclick="sendCommand('ANTIHORARIO')">ANTI-HORÁRIO</button>
            </div>
            <br>
            <div>
              <label for="frequency">Frequência (Hz): </label>
              <input id="frequency" type="number" min="0" max="500" step="0.1">
              <button onclick="setFrequency()">ALTERAR FREQUÊNCIA</button>
            </div>
          </div>
        </div>
        <script>
          function sendCommand(command) {
            fetch('/command?cmd=' + command)
              .then(response => response.text())
          }
          function setFrequency() {
            const freq = document.getElementById('frequency').value;
            if (freq >= 0 && freq <= 500) {
              fetch('/setfreq?freq=' + (freq * 10))
                .then(response => response.text())
            } else {
              alert("Frequência inválida. Insira um valor entre 0 e 500 Hz.");
            }
          }
        </script>
      </body>
      </html>
    )rawliteral";
    request->send(200, "text/html", html);
  });

  // Endpoint para comandos
  server.on("/command", HTTP_GET, [](AsyncWebServerRequest *request) {
    String cmd = request->getParam("cmd")->value();
    uint8_t result;
    if (cmd == "RESET") result = node.writeSingleRegister(0x2135, 0x0080);
    else if (cmd == "SISTEMA") result = node.writeSingleRegister(0x2135, 0x0006);
    else if (cmd == "HORARIO") result = node.writeSingleRegister(0x2135, 0x000F);
    else if (cmd == "ANTIHORARIO") result = node.writeSingleRegister(0x2135, 0x080F);
    else result = 0xFF;  // Comando inválido

    if (result == node.ku8MBSuccess) {
      request->send(200, "text/plain", "Comando executado: " + cmd);
    } else {
      request->send(500, "text/plain", "Erro no comando: " + cmd);
    }
  });

  // Endpoint para alterar frequência
  server.on("/setfreq", HTTP_GET, [](AsyncWebServerRequest *request) {
    String freqStr = request->getParam("freq")->value();
    frequencyValue = freqStr.toInt();
    uint8_t result = node.writeSingleRegister(0x2136, frequencyValue);
    if (result == node.ku8MBSuccess) {
      request->send(200, "text/plain", "Frequência ajustada para " + String(frequencyValue / 10.0) + " Hz.");
    } else {
      request->send(500, "text/plain", "Erro ao ajustar frequência.");
    }
  });

  // Inicia o servidor
  server.begin();
}

void loop() {
}
