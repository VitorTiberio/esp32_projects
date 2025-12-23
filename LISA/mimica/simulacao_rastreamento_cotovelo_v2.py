# Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - EESC USP #

# Importando as Bibliotecas 
import cv2
import numpy as np
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

# Configurando o Media Pipe
BaseOptions = python.BaseOptions
PoseLandmarker = vision.PoseLandmarker
PoseLandmarkerOptions = vision.PoseLandmarkerOptions
VisionRunningMode = vision.RunningMode

options = PoseLandmarkerOptions(
    base_options=BaseOptions(model_asset_path="models/pose_landmarker.task"),
    running_mode=VisionRunningMode.VIDEO,
    num_poses=1,
    min_pose_detection_confidence=0.7,
    min_pose_presence_confidence=0.7,
    min_tracking_confidence=0.7
)

landmarker = PoseLandmarker.create_from_options(options)

# Configuração da WebCam
cap = cv2.VideoCapture(0)

# Controle de referências
calibrado = False
angulo_humano_ref = 0.0
angulo_filtrado = 0.0  

SERVO_MIN = 0
SERVO_MAX = 150

def calcula_angulo(ombro, cotovelo):
    vetor = np.array(ombro) - np.array(cotovelo)
    ang = np.degrees(np.arctan2(vetor[0], -vetor[1]))
    return ang

print("=== LISA – Função Mímica (MediaPipe Tasks) ===")

timestamp = 0

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)

    result = landmarker.detect_for_video(mp_image, timestamp)
    timestamp += 33  # ~30 FPS

    if result.pose_landmarks:
        lm = result.pose_landmarks[0]

        # Braço direito
        ombro = [lm[12].x, lm[12].y]      # ombro direito
        cotovelo = [lm[14].x, lm[14].y]   # cotovelo direito

        angulo_humano = calcula_angulo(ombro, cotovelo)

        # Calibração das posições
        if not calibrado:
            angulo_humano_ref = angulo_humano
            calibrado = True
            print(f"[CALIBRADO] Braço abaixado = Servo 0° (ref humano = {angulo_humano_ref:.2f})")

        # Conversão do ângulo p/ servo 
        delta = angulo_humano - angulo_humano_ref
        angulo_servo = delta  # 0° quando braço abaixado

        # Limites do servo
        angulo_servo = np.clip(angulo_servo, SERVO_MIN, SERVO_MAX)

        # Filtro de Suavização
        angulo_filtrado = 0.8 * angulo_filtrado + 0.2 * angulo_servo

        print(f"Servo (simulado): {angulo_filtrado:.2f}°")

        cv2.putText(frame,
                    f"Servo (sim): {int(angulo_filtrado)} deg",
                    (20, 40),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1, (0, 255, 0), 2)

    cv2.imshow("LISA - Mimica (Simulacao)", frame)

    tecla = cv2.waitKey(1) & 0xFF
    if tecla == 27:
        break
    elif tecla == ord('c'):
        calibrado = False
        print("\n[RECALIBRANDO]\n")

cap.release()
cv2.destroyAllWindows()
