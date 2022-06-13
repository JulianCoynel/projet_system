#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "cp.h"

//hugo.boulanger@lisn.upsaclay.fr rendu

void copie(int op_source, int op_dest){

    int taille = 4096;
    char * buf = malloc(sizeof(char)*taille);
    int r = read(op_source,buf,taille);
    while(r!=0){
        write(op_dest,buf,r);
        r = read(op_source,buf,taille);
    }
    free(buf);
    return;
}

int copie_dir(DIR * dir_source,char * source,char * destination){
    struct stat * buf_stat = malloc(sizeof(struct stat)+1);
    struct dirent *pDirent;
    DIR * dir_source_bis;
    //taille max d'un nom de fichier de 256 caracteres
    char * src = malloc(sizeof(char)*256);
    char * dest = malloc(sizeof(char)*256);   
    while ((pDirent = readdir(dir_source)) != NULL) {
        //path de la source here
        if(strcmp(pDirent->d_name, ".") != 0 && strcmp(pDirent->d_name, "..") != 0){
            strcpy(src,source);
            strcpy(dest,destination);

            if(!strstr(src,"/")){
                src = strcat(src,"/");
            }
            if(!strstr(dest,"/")){
                dest = strcat(dest,"/");
            }
            
            src = strcat(src,pDirent->d_name);
            dest = strcat(dest,pDirent->d_name);
            

            //on test si la source est un fichier ou un dossier
            int op_source = open(src,O_RDONLY);
            if(op_source==-1){
                printf("Erreur fichier ouverture test\n");
                return 1;
            }
            fstat(op_source,buf_stat);

            if(S_ISREG(buf_stat->st_mode) != 0){
                //printf("I'm a file in a dir\n");

                int op_dest = open(dest,O_WRONLY | O_CREAT);
                if(op_dest == -1){
                    printf("Erreur fichier destination sous-jacent\n");
                    return 1;
                }
                fchmod(op_dest,buf_stat->st_mode);

                copie(op_source,op_dest);
                
                close(op_source);
                close(op_dest);
            }else{
                //printf("I'm a dir in a dir\n");
                mkdir(dest,buf_stat->st_mode);

                dir_source_bis = fdopendir(op_source);
                if(dir_source_bis == NULL){
                    perror("Erreur dossier source sous-jacent\n");
                    return 1;
                }
                src = strcat(src,"/");
                dest = strcat(dest,"/");
                if(copie_dir(dir_source_bis,src,dest)==1){
                    return 1;
                }
                closedir(dir_source_bis);
            }

            //on vide src et dest pour le prochain fichier/dossier
            strcpy(src,"");
            strcpy(dest,"");
        }
    }
    free(src);
    free(dest);
    free(buf_stat);
    return 0;
}

int cp_main(int argc,char* argv[]){
    if(argc!=3){
        printf("Mauvais nombre d'agruments, entrez %s [source] [destination]\n",argv[0]);
        return 1;
    }
    char * source = argv[1];
    char * destination = argv[2];
    struct stat * buf_stat = malloc(sizeof(struct stat)+1);
    stat(source,buf_stat);

    if(S_ISREG(buf_stat->st_mode) != 0){
        //printf("I'm a file\n");
        int op_source = open(source,O_RDONLY);
        if(op_source==-1){
            printf("Erreur fichier ouverture test\n");
            return 1;
        }
        int op_dest = open(destination,O_WRONLY | O_CREAT);
        if(op_dest == -1){
            printf("Erreur fichier destination sous-jacent\n");
            return 1;
        }
        fstat(op_source,buf_stat);
        fchmod(op_dest,buf_stat->st_mode);

        copie(op_source,op_dest);
    }else{
        //printf("I'm a directory\n");
        DIR * dir_source = opendir(source);
        if(dir_source == NULL){
            perror("Erreur dossier source");
            return 1;
        }
        DIR * dir_dest = opendir(destination);
        if(dir_dest == NULL){
            mkdir(destination,buf_stat->st_mode);
        }

        if(copie_dir(dir_source,source,destination)==1){
            printf("Erreur copie_dir\n");
            return 1;
        }

        closedir(dir_source);
        closedir(dir_dest);
    }
    
    free(buf_stat);
    return 0;
}