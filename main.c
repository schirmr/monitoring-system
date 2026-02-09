#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h> // sleep no windows

boolean first_read = 1; // Variável global para controlar a primeira leitura da CPU

unsigned long long filetime_para_64(FILETIME ft){
    return ((unsigned long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime; // Converte FILETIME para um valor de 64 bits
}

int ler_cpu_windows(unsigned long long *idle, unsigned long long *total){
    FILETIME idleTime, kernelTime, userTime;
    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    *idle = filetime_para_64(idleTime);
    *total = filetime_para_64(kernelTime) + filetime_para_64(userTime);

    return 0;
}

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
        #ifdef _WIN32 // Cálculo do uso da CPU no Windows
        unsigned long long total1, idle1, total_diff, idle_diff;
        unsigned long long total2, idle2;
        if(first_read){
            ler_cpu_windows(&total1, &idle1);
            Sleep(1000);
            ler_cpu_windows(&total2, &idle2);
            first_read = 0;
            total_diff = total2 - total1;
            idle_diff  = idle2 - idle1;
        }
        else{
            ler_cpu_windows(&total2, &idle2);
            total_diff = total2 - total1;
            idle_diff  = idle2 - idle1;
            total1 = total2;
            idle1 = idle2;
        }
        float cpu_usage = ((((float)idle_diff / total_diff)-1) * 100.0f);
        #endif
        if(fp){
            fprintf(fp, "{\n");
            fprintf(fp, "  \"memoria_total_gb\": %.2f,\n", fmem_total);
            fprintf(fp, "  \"memoria_disponivel_gb\": %.2f,\n", fmem_free);
            fprintf(fp, "  \"memoria_usada_gb\": %.2f,\n", fmem_used);
            fprintf(fp, "  \"porcentagem_memoria_usada\": %.2f,\n", porc_used);
            fprintf(fp, "  \"porcentagem_cpu_usada\": %.2f\n", cpu_usage);
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

