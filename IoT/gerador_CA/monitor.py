# Autor: Vitor Augusto Tibério - 14658834 # 
# Universidade de São Paulo (USP) - Engenharia Elétrica # 
# Projeto: Gerador CA Integrado à IoT # 

# Importando as Bibliotecas # 

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import requests
from datetime import datetime

# Configurações Gerais # 

URL_ESP32 = "http://192.168.18.47/dados"  # Atualizar o IP da ESP32 no json

TENSAO_ALVO = 127.0  # Tensão que é objetivo da geração

JANELA_DE_TEMPO = 30 # Quantos pontos serão mostrados no gráfico 

# Listas para guardar os dados
eixo_x = []
eixo_y = []

# Configuração visual da janela
fig, ax = plt.subplots()
fig.canvas.manager.set_window_title('Gerador CA - Monitor de Tensão')

def atualizar_grafico(i):
    try:
        # Pega o JSON da ESP-32
        resposta = requests.get(URL_ESP32, timeout=2)
        dados = resposta.json()
        
        # Pega o valor da chave 'v' (definida no código da ESP32)
        valor_tensao = float(dados['v'])
        hora_atual = datetime.now().strftime("%H:%M:%S")
        
        # Atualiza as listas
        eixo_x.append(hora_atual)
        eixo_y.append(valor_tensao)
        
        # Mantém o tamanho fixo da janela
        if len(eixo_x) > JANELA_DE_TEMPO:
            eixo_x.pop(0)
            eixo_y.pop(0)
        
        #  Limpa e desenha o gráfico novamente
        ax.clear()
        
        # Plot da Linha de Tensão Real (Laranja) 
        ax.plot(eixo_x, eixo_y, color='#ff8c00', linewidth=2.5, marker='o', label='Tensão Medida')
        
        # Plot da Linha de Referência (verde)
        ax.axhline(y=TENSAO_ALVO, color='green', linestyle='--', alpha=0.6, label=f'Alvo ({TENSAO_ALVO}V)')
        
        # Formatação do Gráfico
        ax.set_title(f"Tensão Atual: {valor_tensao:.2f} V", fontsize=16, fontweight='bold')
        ax.set_ylabel("Tensão RMS (Volts)", fontsize=12)
        ax.legend(loc='upper left') # Legenda no canto
        ax.grid(True, linestyle='--', alpha=0.7)
        
        # Preenchimento translúcido abaixo da linha (efeito visual)
        ax.fill_between(eixo_x, eixo_y, alpha=0.1, color='#ff8c00')
        
        # Ajusta a inclinação das horas no eixo X para não encavalar
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        
    except Exception as e:
        print(f"Aguardando conexão com o Gerador... Erro: {e}")

# Inicia a execução
print(f"Iniciando monitoramento em: {URL_ESP32}")
print("Pressione Ctrl+C para encerrar.")

ani = animation.FuncAnimation(fig, atualizar_grafico, interval=1000) # Atualiza a cada 1000ms (1s)
plt.show()
