# Sistema de Monitoramento de Recursos

O objetivo deste projeto é criar uma solução de observabilidade leve, coletando métricas do sistema operacional (Linux ou Windows) e exibindo-as em um dashboard web simples. O sistema feito em C agora detecta o ambiente automaticamente, utilizando `/proc` em Linux e as APIs nativas de memória em Windows, mantendo a mesma saída `metricas.json` para o servidor web.

## Infos

- **Linux**: o sistema lê `/proc/meminfo` para calcular memória total, disponível e usar esses valores para derivar memória usada e porcentagem.
- **Windows**: o sistema usa `GlobalMemoryStatusEx` para obter os mesmos dados, convertendo tudo para KB antes de gerar `metricas.json`.
- Os valores são convertidos para GB com duas casas decimais e atualizados a cada 1 segundo.

## Arquitetura do Projeto

O sistema segue um modelo **Sistema em C + Servidor Web em Python**:

1. **Sistema (C – main.c):**
	- Em Linux lê `/proc/meminfo`; em Windows usa `GlobalMemoryStatusEx`.
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

- **Linux**
	- Distribuição com acesso ao `/proc`
	- `gcc` para compilar o agente C
	- Python3 
- **Windows**
	- Compilador em C (MSYS2) disponível no `PATH`
	- Python com suporte a `python` ou `python3` no terminal

## Como rodar o projeto

No diretório do projeto, execute apenas:

```bash
# Linux/Windows
python3 servidor.py
```

O script irá:
- compilar o agente C
- iniciar o agente para gerar o `metricas.json`
- subir o servidor HTTP na porta 8000
- abrir automaticamente o dashboard em `http://localhost:8000`

Para encerrar, volte ao terminal onde o servidor está rodando e pressione `Ctrl + C`.





