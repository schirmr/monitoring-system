# Sistema de Monitoramento de Recursos

O objetivo deste projeto é criar uma solução de observabilidade leve, coletando métricas diretamente do Kernel Linux e exibindo-as em um dashboard web simples.

## Infos

- `/proc/meminfo` – contém informações do uso de memória RAM. Atualmente o agente lê esse arquivo para calcular:
  - memória total
  - memória disponível
  - memória usada
  - porcentagem de uso de memória

## Arquitetura do Projeto

O sistema segue um modelo **Agente + Servidor Web**:

1. **Sistema (C – main.c):**
	- Roda em loop lendo `/proc/meminfo`.
	- Processa os dados e gera o arquivo `metricas.json` a cada 1 segundo.

2. **Servidor (Python – servidor.py):**
	- Compila automaticamente o sistema em C (`gcc main.c -o main`).
	- Inicia o binário `./main` que produz o `metricas.json`.
	- Sobe um servidor HTTP simples (`python -m http.server 8000`).
	- Abre o dashboard no navegador em `http://localhost:8000`.

3. **Dashboard (Web – index.html, script.js, style.css):**
	- Página HTML que exibe as métricas de memória.
	- O `script.js` faz requisições periódicas para `metricas.json` e atualiza os valores em tela em tempo real.

## Requisitos

- Linux (acesso a `/proc`)
- `gcc` para compilar o agente C
- `python3` para rodar o servidor/orquestrador
- Navegador web moderno (Chrome, Firefox, etc.)

## Como rodar o projeto

No diretório do projeto, execute apenas:

```bash
python3 servidor.py
```

O script irá:
- compilar o agente C
- iniciar o agente para gerar o `metricas.json`
- subir o servidor HTTP na porta 8000
- abrir automaticamente o dashboard em `http://localhost:8000`

Para encerrar, volte ao terminal onde o servidor está rodando e pressione `Ctrl + C`.





