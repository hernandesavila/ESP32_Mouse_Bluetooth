# 🖱️ ESP32 Mouse Bluetooth

ESP32 Mouse Bluetooth é um firmware para **ESP32** desenvolvido com a biblioteca **BleMouse** para transformar o microcontrolador em um mouse Bluetooth com controle de rolagem e monitoramento de bateria.

O sketch configura entradas para botões físicos, realiza a leitura do nível da bateria via ADC com divisor resistivo e publica o nível ao host Bluetooth enquanto controla LEDs de status e o modo de economia de energia.

---

## 🛠️ Tecnologias Utilizadas

<p align="center">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/arduino/arduino-original.svg" alt="Arduino" width="30" height="30"/>
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/embeddedc/embeddedc-original.svg" alt="C/C++" width="30" height="30"/>
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/vscode/vscode-original.svg" alt="VS Code" width="30" height="30"/>
</p>

- **Arduino Core para ESP32** – plataforma alvo e SDK
- **BleMouse** – biblioteca para HID sobre Bluetooth Low Energy
- **C/C++** – linguagem do sketch
- **Visual Studio Code / Arduino IDE** – ambientes de desenvolvimento sugeridos

---

## 📂 Estrutura do Projeto

- `Mouse_Bluetooth_ESP32.ino` – sketch principal com configuração de pinos, leitura da bateria e comandos HID

---

## ✅ Pré-requisitos

- Placa **ESP32** com suporte a Bluetooth Low Energy
- **Arduino IDE** (1.8+) ou **PlatformIO** configurado com o core ESP32
- Biblioteca **BleMouse** instalada no ambiente de desenvolvimento
- Fonte de alimentação e divisor resistivo (5.45 kΩ / 10 kΩ) conectados ao pino analógico

---

## ⚙️ Configuração

1. **Instale o core ESP32**
   - No Arduino IDE, adicione `https://dl.espressif.com/dl/package_esp32_index.json` nas URLs adicionais de placas e instale o pacote ESP32.

2. **Adicione a biblioteca BleMouse**
   - Instale via Library Manager ou copie para a pasta `libraries` do seu ambiente.

3. **Mapeamento de hardware**
   - `PIN_UP (22)` e `PIN_DOWN (23)` para botões de rolagem.
   - `PIN_BATERY (36)` conectado ao divisor resistivo da bateria.
   - `PIN_LED (2)` LED de status da conexão.
   - `PIN_LED_BATERY (21)` LED de alerta de bateria fraca.

4. **Ajuste do divisor de tensão**
   - Confirme os valores dos resistores (`5.45 kΩ` e `10 kΩ`) e ajuste `resistor1`/`resistor2` no código se necessário para corresponder ao hardware real.

---

## 🛠️ Compilação

1. Abra `Mouse_Bluetooth_ESP32.ino` no Arduino IDE ou VS Code com PlatformIO.
2. Selecione a placa ESP32 correta (ex.: **ESP32 Dev Module**).
3. Configure a porta serial correspondente.
4. Compile e faça o upload para o dispositivo.

---

## ▶️ Execução

1. Alimente o ESP32 e aguarde a inicialização do Bluetooth.
2. Emparelhe o dispositivo anunciado como mouse BLE.
3. Utilize os botões conectados aos pinos `PIN_UP` e `PIN_DOWN` para simular rolagem.
4. Pressione ambos os botões simultaneamente para mover o cursor horizontalmente.
5. Acompanhe o LED da bateria para alertas quando `realBateryVoltage <= 3.4 V` e observe o modo de economia caso caia abaixo de `3.0 V`.

---

## 🔎 Funcionamento

- A média de `VOLTAGE_AVERAGE_INTERATIONS` leituras suaviza o valor do ADC.
- O cálculo `realBateryVoltage` ajusta a leitura para o divisor resistivo antes de mapear para porcentagem.
- O nível da bateria é enviado ao host com `bleMouse.setBatteryLevel` sempre que há variação.
- Quando desconectado, o firmware reinicia o stack BLE para facilitar o pareamento.
- Caso a tensão caia abaixo de `3.0 V`, o ESP32 entra em `esp_deep_sleep_start()` para preservar a bateria.

---

## 📌 Observações

- Ajuste `minVoltage` e `maxVoltage` conforme a química da bateria utilizada.
- Certifique-se de que o hardware suporte o consumo do BLE Mouse e monitore a dissipação no divisor resistivo.
- O código utiliza `delay(100)` no loop principal; reduza se necessitar de maior responsividade.

---

## 📄 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
