#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h> // sleep no windows


int ler_memoria_windows(long *total_kb, long *disponivel_kb){
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if(!GlobalMemoryStatusEx(&memInfo)){
        return -1;
    }

    *total_kb = memInfo.ullTotalPhys / 1024;
    *disponivel_kb = memInfo.ullAvailPhys / 1024;
    return 0;
}

#else
#include <unistd.h> // sleep no linux

long ler_arquivo(const char *nome_arquivo, const char *info){
    FILE *file = fopen(nome_arquivo, "r");
    char buffer[256];
    long valor;
    
    if(!file){
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    while(fscanf(file, "%s", buffer) != EOF){

        if(strcmp(buffer, info) == 0){
            fscanf(file, "%ld", &valor);
            fclose(file);
            return valor;
        }
    }
    fclose(file);
    return -1;
}

#endif


int main(){
    while(1){
        #ifdef _WIN32
        long mem_total, mem_free;
        if(ler_memoria_windows(&mem_total, &mem_free) != 0){
            fprintf(stderr, "Erro ao ler memória no Windows\n");
            return 1;
        }
        #else
        mem_total = ler_arquivo("/proc/meminfo", "MemTotal:");
        mem_free = ler_arquivo("/proc/meminfo", "MemAvailable:");
        if(mem_total == -1 || mem_free == -1){
            fprintf(stderr, "Erro ao ler informações de memória\n");
            return 1;
        }
        #endif
        long mem_used = mem_total - mem_free;

        float fmem_total = (float)mem_total / (1024 * 1024);
        float fmem_free = (float)mem_free / (1024 * 1024); 
        float fmem_used = (float)mem_used / (1024 * 1024); 
        float porc_used = (fmem_used / fmem_total) * 100;

        FILE *fp = fopen("metricas.json", "w");

        if(fp){
            fprintf(fp, "{\n");
            fprintf(fp, "  \"memoria_total_gb\": %.2f,\n", fmem_total);
            fprintf(fp, "  \"memoria_disponivel_gb\": %.2f,\n", fmem_free);
            fprintf(fp, "  \"memoria_usada_gb\": %.2f,\n", fmem_used);
            fprintf(fp, "  \"porcentagem_memoria_usada\": %.2f\n", porc_used);
            fprintf(fp, "}\n");
            fclose(fp);
        }
        else {
            perror("Erro ao abrir o arquivo de saída");
        }
        #ifdef _WIN32
        Sleep(1000); // sleep no windows (1000 ms = 1 segundo)
        #else
        sleep(1); // sleep no linux (1 segundo)
        #endif
    }
    return 0;
}

