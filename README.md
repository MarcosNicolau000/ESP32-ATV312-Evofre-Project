# 🛠️ Controle de Motor via ESP32 e Inversor de Frequência

Este projeto tem como objetivo controlar remotamente um motor elétrico trifásico utilizando um ESP32 conectado a um inversor de frequência, com comunicação via protocolo Modbus RTU. O controle pode ser feito por meio de um aplicativo mobile conectado à rede Wi-Fi do ESP32.

## 🚀 Funcionalidades

- Controle remoto de velocidade e sentido de rotação do motor
- Comunicação com inversor de frequência via protocolo Modbus RTU
- Interface mobile (via Wi-Fi) para envio de comandos
- Programação do ESP32 em C++

## 🧰 Tecnologias Utilizadas

- ESP32 Dev Module
- Inversor de frequência Schneider ATV312
- Protocolo Modbus RTU via comunicação serial (RS-485)
- Website para controle do motor.
- Linguagem: C++ (Arduino IDE)
- Bibliotecas: `ModbusMaster.h`, `WiFi.h`, `ESPAsyncWebServer`, `AsyncTCP`

## 🔌 Esquema de Conexão

- ESP32 -> Módulo RS485 -> Inversor de Frequência
- ESP32 -> Wi-Fi -> Website
- Inversor -> Motor Trifásico

## 🧪 Testado com

- ESP32 DevKit V1
- Inversor Schneider ATV312
- Motor trifásico WEG W CV
- Módulo RS485 TTL

## 📲 Controle Mobile

O website permite:
- Alterar a frequência do motor (velocidade)
- Inverter o sentido de rotação
- Ligar/desligar o motor

## ⚠️ Requisitos

- Fonte de alimentação adequada para ESP32 e módulos
- Configuração correta do endereço Modbus do inversor
- Dispositivo deve estar na mesma rede do ESP32

## 💡 Aprendizados

- Comunicação industrial com Modbus RTU
- Integração entre eletrônica e redes sem fio
- Programação embarcada com ESP32

## 📸 Imagens do Projeto

![5](https://github.com/user-attachments/assets/49403607-9947-491c-a023-59c369441b78)
![4](https://github.com/user-attachments/assets/3c4fc8e4-ec18-456b-88fc-1049dae55995)
![3](https://github.com/user-attachments/assets/1c13aece-cd8b-44b1-952c-0f166b7c9300)


## 📄 Licença

**Todos os direitos reservados.**  
Este projeto é de uso pessoal e educacional. A reprodução, distribuição ou modificação não autorizada não é permitida sem o consentimento do autor.


---

Desenvolvido por [Marcos Nicolau](https://www.linkedin.com/in/seulinkedin)
