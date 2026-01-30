#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int ler_arquivo(const char *nome_arquivo, const char *info){
    FILE *file = fopen(nome_arquivo, "r");
    char buffer[256];
    
    if(!file){
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    while(fscanf(file, "%s", buffer) != EOF){

        if(strcmp(buffer, info) == 0){
            fscanf(file, "%s", buffer);
            printf("%s %s\n", info, buffer);
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

int main(){
    ler_arquivo("/proc/meminfo", "MemTotal:");


    return 0;
}

