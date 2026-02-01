# Sistema de Monitoramento de Recursos

O objetivo deste projeto é criar uma solução de observabilidade sem depender de agentes pesados de mercado, coletando métricas diretamente do Kernel Linux e exibindo-as em um dashboard web.

## Arquitetura do Projeto

O sistema opera em um modelo **Agente-Servidor**:

1.  **Agente (C):** Roda na máquina monitorada. Lê arquivos do sistema operacional, processa os dados e gera um JSON.
2.  **Backend (Python):** Recebe os dados do agente via HTTP/Sockets e disponibiliza uma API.
3.  **Dashboard (Web):** Interface simples em HTML/JS que consome a API do Python para exibir gráficos em tempo real.

/proc/meminfo - contem informações do uso de memória ram.

/proc/uptime - contem o tempo em segundos que o sistema está ligado.
