## Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - USP São Carlos ## 

## Importanto as Bibliotecas ##
import cv2
import mediapipe as mp
import numpy as np
import serial
import time

# Setando a comunicação serial com a ESP-32 # 
ser = serial.Serial('COM3', 115200, timeout=1)
time.sleep(2)

# Configurações do media pipe 
mp_pose = mp.solutions.pose
pose = mp_pose.Pose(
    min_detection_confidence=0.7,
    min_tracking_confidence=0.7
)
mp_draw = mp.solutions.drawing_utils

# Configurações da câmera
cap = cv2.VideoCapture(0)

angulo_filtrado = 90  # inicial neutro

def calcula_angulo(ombro, cotovelo):
    """
    Ângulo do braço em relação à vertical
    """
    vetor = np.array(ombro) - np.array(cotovelo)
    ang = np.degrees(np.arctan2(vetor[0], -vetor[1]))
    return ang

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)  # espelhado (mais intuitivo)
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    resultado = pose.process(frame_rgb)

    if resultado.pose_landmarks:
        lm = resultado.pose_landmarks.landmark

        ombro = [lm[mp_pose.PoseLandmark.RIGHT_SHOULDER].x,
                 lm[mp_pose.PoseLandmark.RIGHT_SHOULDER].y]

        cotovelo = [lm[mp_pose.PoseLandmark.RIGHT_ELBOW].x,
                    lm[mp_pose.PoseLandmark.RIGHT_ELBOW].y]

        angulo = calcula_angulo(ombro, cotovelo)

        # Mapeamento (interação humano p/ servo)
        angulo_servo = int(np.clip(angulo + 90, 0, 180))

        # Filtro (suaviza tremulação) -- MODIFICAR SE ESTIVER TREMENDO MUITO! (aq em casa funcionou bem desse jeito) 
        angulo_filtrado = int(0.8 * angulo_filtrado + 0.2 * angulo_servo)

        # Envia para ESP32
        ser.write(f"{angulo_filtrado}\n".encode())

        # Visualização
        cv2.putText(frame,
                    f"Servo: {angulo_filtrado} deg",
                    (20, 40),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1, (0, 255, 0), 2)

        mp_draw.draw_landmarks(
            frame,
            resultado.pose_landmarks,
            mp_pose.POSE_CONNECTIONS
        )

    cv2.imshow("LISA - Função Mímica", frame)

    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
ser.close()

