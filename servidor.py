import time
import sys
import subprocess
import webbrowser

def main():
    global processo_c
    global processo_server

    compilacao = subprocess.run(["gcc", "main.c", "-o", "main"], capture_output=True, text=True)
    if compilacao.returncode != 0:
        print("Erro na compilação:\n", compilacao.stderr)
        return
        
    processo_c = subprocess.Popen(["./main"])

    processo_server = subprocess.Popen([sys.executable, "-m", "http.server", "8000"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    time.sleep(1)
    webbrowser.open("http://localhost:8000")

    print("Servidor iniciado. Pressione Ctrl+C para encerrar.")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        processo_c.terminate()
        processo_server.terminate()
        print("Servidores encerrados.")

if __name__ == "__main__":
    main()