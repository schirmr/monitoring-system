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
            return valor; // configurar para que o char vire int
        }
    }
    fclose(file);
    return -1;
}

int main(){
    long mem_total = ler_arquivo("/proc/meminfo", "MemTotal:");
    if(mem_total == -1){
        fprintf(stderr, "Erro ao ler a memória total\n");
        return 1;
    }
    long mem_free = ler_arquivo("/proc/meminfo", "MemAvailable:");
    if(mem_free == -1){
        fprintf(stderr, "Erro ao ler a memória disponível\n");
        return 1;
    }

    long mem_used = mem_total - mem_free;

    printf("Memória Total: %ld GB\n", mem_total/(1024*1024)); // transformar em float
    printf("Memória Disponível: %ld GB\n", mem_free/(1024*1024)); // transformar em float
    printf("Memória Usada: %ld GB\n", mem_used/(1024*1024)); // transformar em float



    return 0;
}

