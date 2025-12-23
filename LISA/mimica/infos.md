# LISA – Função Mímica (Rastreamento de Braço com MediaPipe)

Este documento descreve a implementação da **função mímica** do projeto **LISA**, responsável por mapear o movimento do braço humano para um ângulo de servo motor utilizando visão computacional.

Nesta versão, o sistema realiza **simulação do servo**, permitindo testes completos fora do laboratório.

## Tecnologias Utilizadas

O sistema foi desenvolvido utilizando as seguintes tecnologias:

- **Python** – linguagem principal do projeto
- **OpenCV** – captura e visualização de vídeo
- **MediaPipe Tasks (Pose Landmarker)** – rastreamento corporal
- **NumPy** – operações vetoriais e cálculos trigonométricos

---

## Aquisição de Imagem

A aquisição de imagem é feita por meio de uma webcam conectada ao computador, utilizando o OpenCV:

```python
cap = cv2.VideoCapture(0)
```
---

## Rastreamento do Braço (Ombro → Punho)

O rastreamento do braço humano é realizado utilizando landmarks fornecidos pelo **MediaPipe Pose**. Para representar a inclinação global do braço, são utilizados dois pontos principais:

- `RIGHT_SHOULDER` (índice 12)
- `RIGHT_WRIST` (índice 16)
- `RIGHT_ELBOW` (índice 14) --> opção alternativa de rastreamento

Esses pontos definem o vetor **ombro → punho**, que representa o movimento completo do braço. Essa abordagem é mais estável do que utilizar apenas o cotovelo, pois reduz a influência da flexão do braço e fornece uma estimativa mais fiel do gesto de levantar ou abaixar o braço.

---

## Cálculo do Ângulo do Braço

A inclinação do braço é calculada em relação à vertical da imagem. Para isso, utiliza-se o vetor formado entre o ombro e o punho, aplicando a função `arctan2` para obter o ângulo em graus:

```python
def calcula_angulo(ombro, punho):
    vetor = np.array(punho) - np.array(ombro)
    ang = np.degrees(np.arctan2(vetor[0], -vetor[1]))
    return ang
```
---

## Calibração Automática

Na primeira detecção válida do braço humano, o sistema realiza uma calibração automática. O ângulo medido nesse instante é armazenado como referência:

```python
angulo_humano_ref = angulo_humano
```

Essa posição de referência corresponde ao braço completamente abaixado, sendo definida como 0° do servo motor.

A partir desse ponto, todo o movimento do braço é interpretado de forma relativa, eliminando a necessidade de posições iniciais rígidas ou calibração manual.

O sistema também permite recalibração dinâmica pressionando a tecla C, desde que o braço esteja novamente abaixado.

---
## Conversão Humano → Servo e Simulação

O ângulo do servo é obtido pela diferença entre o ângulo atual do braço e o ângulo de referência definido na calibração:

```python
delta = angulo_humano - angulo_humano_ref
angulo_servo = delta
```

Para garantir um comportamento fisicamente plausível, são aplicados:

Limites mecânicos do servo (0° a 150°)

Filtro passa-baixa para suavizar o sinal e reduzir oscilações naturais do rastreamento:

```python
angulo_filtrado = 0.8 * angulo_filtrado + 0.2 * angulo_servo
```

Nesta etapa do projeto, o servo motor não é acionado fisicamente. O valor calculado é:

Exibido no terminal

Sobreposto na imagem da câmera em tempo real

Essa simulação permite validar o funcionamento do algoritmo de rastreamento, calibração e controle antes da integração com o ESP32 e os atuadores reais.

---

## Controles do Programa

Durante a execução do programa, algumas teclas do teclado permitem controlar o comportamento do sistema em tempo real:

### ⌨️ Teclas disponíveis

- **ESC**  
  Encerra imediatamente a execução do programa, liberando a câmera e fechando todas as janelas.

- **C**  
  Força uma **nova calibração automática** do sistema.  
  Ao pressionar essa tecla, o usuário deve estar com o braço **totalmente abaixado**, pois essa posição será novamente definida como **0° do servo**.

### 🔁 Recalibração

A funcionalidade de recalibração é útil para:
- Ajustar mudanças de posição do usuário
- Corrigir pequenas variações na postura inicial
- Reiniciar o sistema sem precisar encerrar o programa

O processo de calibração é totalmente automático e ocorre na primeira detecção válida após a tecla `C` ser pressionada.

---
