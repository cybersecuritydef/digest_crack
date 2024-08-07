#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wordlists.h"
#include "common.h"


static void add_words(struct wordlists *words, const char *word){
    struct lists *data = NULL;
    if((data = (struct lists*)malloc(sizeof(struct lists)))){
        data->word = strdup(word);
        words->count++;
        data->next = words->words;
        words->words = data;
    }
    else
        die("[-] Error allocation memory!");
}

struct wordlists *read_wordlists(const char *filename){
    struct wordlists *words = NULL;
    char buf[LEN] = {'\0'};
    FILE *file = NULL;
    if((file = fopen(filename, "r"))){
        if((words = (struct wordlists*)malloc(sizeof(struct wordlists)))){
            words->count = 0;
            words->words = NULL;
            while(fgets(buf, sizeof(buf), file)){
                if(buf[strlen(buf) - 1] == '\n')
                    buf[strlen(buf) - 1] = '\0';
                add_words(words, buf);
            }
            fclose(file);
            return words;
        }
        else{
            fclose(file);
            die("[-] Error allocation memory!");
        }
    }
    return NULL;
}

void free_wordlists(struct wordlists *words){
    struct lists *tmp = NULL;
    while(words->words->next){
        tmp = words->words;
        words->words = words->words->next;
        free(tmp->word);
        free(tmp);
        tmp = NULL;
    }

    free(words->words->word);
    free(words->words);
    words->words = NULL;

    free(words);
    words = NULL;
}
