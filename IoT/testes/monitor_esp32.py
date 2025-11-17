## Autor: Vitor Augusto Tibério - Estudante de Engenharia Elétrica - USP São Carlos ## 

## Importanto as Bibliotecas ## 

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import requests
from datetime import datetime

# --- CONFIGURAÇÃO ---
# O IP da sua ESP32 (conforme seu print)
URL_ESP32 = "http://192.168.18.47/dados" 
INTERVALO_LEITURA = 1000 # Lê a cada 1000ms (1 segundo)

# Listas para guardar os dados para plot de gráficos
eixo_x = []
temp_y = []
umid_y = []

# Configura a janela do gráfico
fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)
fig.canvas.manager.set_window_title('Monitoramento ESP32 - Tempo Real')

def atualizar_grafico(i):
    try:
        # Pega o JSON da ESP32
        resposta = requests.get(URL_ESP32, timeout=2)
        dados = resposta.json() 
        
        # Pega a hora atual
        agora = datetime.now().strftime("%H:%M:%S")
        
        # Salva as informações na lista criada
        eixo_x.append(agora)
        temp_y.append(float(dados['t']))
        umid_y.append(float(dados['h']))
        
        # Mantém só os últimos 20 pontos para não travar
        if len(eixo_x) > 20:
            eixo_x.pop(0)
            temp_y.pop(0)
            umid_y.pop(0)
        
        # Limpa e desenha de novo
        ax1.clear()
        ax2.clear()
        
        # Gráfico de Temperatura (Vermelho)
        ax1.plot(eixo_x, temp_y, color='tab:red', marker='o', linestyle='-')
        ax1.set_ylabel('Temp (ºC)')
        ax1.set_title(f"Temperatura: {dados['t']}°C")
        ax1.grid(True, linestyle='--', alpha=0.7)
        
        # Gráfico de Umidade (Azul)
        ax2.plot(eixo_x, umid_y, color='tab:blue', marker='o', linestyle='-')
        ax2.set_ylabel('Umidade (%)')
        ax2.set_title(f"Umidade: {dados['h']}%")
        ax2.grid(True, linestyle='--', alpha=0.7)
        
        # Ajusta as datas no eixo X
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        
    except Exception as e:
        print(f"Erro (tentando reconectar...): {e}")

# Inicia a animação
print(f"Conectando em {URL_ESP32}...")
ani = animation.FuncAnimation(fig, atualizar_grafico, interval=INTERVALO_LEITURA)
plt.show()