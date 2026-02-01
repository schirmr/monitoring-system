import json
import time

while True:
    with open("metricas.json", "r") as file:
        dados = json.load(file)
            
        print(f"Memória Total: {dados['memoria_total_gb']} GB")
        print(f"Memória Disponivel: {dados['memoria_disponivel_gb']} GB");
        print(f"Memória Usada: {dados['memoria_usada_gb']} GB")
        print(f"Uso Atual: {dados['porcentagem_memoria_usada']}%")
    time.sleep(1) 