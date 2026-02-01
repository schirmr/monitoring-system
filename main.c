#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int main(){
    while(1){
        long mem_total = ler_arquivo("/proc/meminfo", "MemTotal:");
        long mem_free = ler_arquivo("/proc/meminfo", "MemAvailable:");
        if(mem_total == -1 || mem_free == -1){
            fprintf(stderr, "Erro ao ler informações de memória\n");
            return 1;
        }
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
        sleep(1);
    }
    return 0;
}

