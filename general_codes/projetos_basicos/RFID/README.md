# 🔐 Fechadura Eletrônica com RFID – ESP32

Este projeto implementa uma **fechadura eletrônica controlada por RFID** utilizando o **microcontrolador ESP32** e o módulo **MFRC522**.  
O sistema libera ou nega acesso com base no **UID (Unique Identifier)** do cartão RFID, fornecendo **feedback visual** por LEDs e **feedback sonoro** por buzzer.

---

## ⚙️ Funcionalidades

- Leitura de cartões RFID (MFRC522)
- Identificação do UID do cartão
- Validação de acesso por UID autorizado
- Acionamento de fechadura elétrica via relé
- Feedback visual (LED verde e vermelho)
- Feedback sonoro (buzzer)
- Depuração via Serial Monitor

---

## 🧰 Componentes utilizados

- ESP32  
- Módulo RFID MFRC522  
- Cartão ou chaveiro RFID  
- Módulo relé ou fechadura elétrica  
- LED verde  
- LED vermelho  
- Buzzer  
- Resistores para LEDs  
- Jumpers  

---

> ⚠️ O módulo MFRC522 opera exclusivamente em **3.3 V**.

--- 

## 📚 Bibliotecas necessárias

Instale a biblioteca abaixo pelo **Gerenciador de Bibliotecas** da Arduino IDE:

--- 

## Como configugar o UID no código

Antes de utilizar a fechadura eletrônica, é necessário identificar o UID do cartão RFID que será autorizado.

Passo a passo:

1. Conecte o ESP32 ao computador via USB
2. Abra a Arduino IDE
3. Carregue no ESP32 o código de leitura de UID (disponível em GET_RFID_UID.md)
4. Faça o upload do código
5. Abra o Serial Monitor
6. Configure a taxa de comunicação para: 115200 baud
7. Ao aproximar o cartão, o Serial Monitor exibirá uma mensagem semelhante a: "UID: 0xDE 0xAD 0xBE 0xEF"
8. Após identificar o UID, copie os valores exibidos no Serial Monitor e substitua no código principal: "byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF}";
> ⚠️ O número de bytes do UID pode variar dependendo do tipo de cartão RFID.
9. Faça o Upload do código e teste
