import matplotlib.pyplot as plt
import matplotlib.animation as animation
import requests
from datetime import datetime

# --- CONFIGURAÇÕES ---
URL_ESP32 = "http://192.168.18.47/dados" # puder mudar, caso a rede de internet altere
# Tensão Alvo (Linha Verde)
TENSAO_ALVO = 1.0  
# Tamanho da Janela para plot
JANELA_DE_TEMPO = 30 
# Listas de dados
eixo_x = []
eixo_y = []

# Configuração da Janela
fig, ax = plt.subplots()
fig.canvas.manager.set_window_title('Gerador CA - Monitoramento')
try:
    plt.style.use('seaborn-v0_8-darkgrid')
except:
    pass

def atualizar_grafico(i):
    try:
        # Pede os dados para a ESP32
        resposta = requests.get(URL_ESP32, timeout=2)
        dados = resposta.json()
        
        # Pega o valor da chave 'v' do JSON
        valor_tensao = float(dados['v'])
        hora_atual = datetime.now().strftime("%H:%M:%S")
        
        # Lógica de Aviso no Terminal (Sincronizado com o LED)
        if valor_tensao > 0.3:
            print(f"[{hora_atual}] ⚠️ GERANDO ENERGIA: {valor_tensao:.1f} V")
        else:
            pass

        # Atualiza listas
        eixo_x.append(hora_atual)
        eixo_y.append(valor_tensao)
        
        # Mantém tamanho fixo
        if len(eixo_x) > JANELA_DE_TEMPO:
            eixo_x.pop(0)
            eixo_y.pop(0)
        
        # Limpa e Redesenha
        ax.clear()
        
        # Linha Laranja (Tensão Real)
        ax.plot(eixo_x, eixo_y, color='#ff8c00', linewidth=2.5, marker='o', label='Tensão Medida')
        
        # Linha Verde (Alvo)
        ax.axhline(y=TENSAO_ALVO, color='green', linestyle='--', alpha=0.6, label=f'Alvo ({TENSAO_ALVO}V)')
        
        # Configurações do Gráfico
        ax.set_title(f"Tensão Atual: {valor_tensao:.2f} V", fontsize=16, fontweight='bold')
        ax.set_ylabel("Tensão RMS (Volts)", fontsize=12)
        ax.legend(loc='upper left')
        ax.grid(True, linestyle='--', alpha=0.7)
        ax.fill_between(eixo_x, eixo_y, alpha=0.1, color='#ff8c00')
        
        # Ajusta eixo X
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        
    except Exception as e:
        print(f"Aguardando conexão... {e}")

# Início
print(f"Conectando em {URL_ESP32}...")
ani = animation.FuncAnimation(fig, atualizar_grafico, interval=1000)
plt.show()
