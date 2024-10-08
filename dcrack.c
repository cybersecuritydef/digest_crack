#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>

#include "md5.h"
#include "wordlists.h"
#include "common.h"

#define LEN_HASH 32

struct options{
    char *wordlist;
    char *username;
    char *url;
    char *realm;
    char *nonce;
    char *nc;
    char *cnonce;
    char *qop;
    char *method;
    char *response;
    bool verbose;
};


void help(void){
    printf("OPTIONS:\n\t");
    printf("-w\t--wordlist=\tpath wordlist\n\t");
    printf("-U\t--username=\tuser login\n\t");
    printf("-r\t--realm=\tvalue realm\n\t");
    printf("-n\t--nonce=\tvalue nonce\n\t");
    printf("-u\t--url=\t\turl path\n\t");
    printf("-c\t--nc=\t\tvalue nc\n\t");
    printf("-e\t--cnonce=\tvalue cnonce\n\t");
    printf("-q\t--qop=\t\tvalue qop\n\t");
    printf("-m\t--method=\tmethod request\n\t");
    printf("-o\t--response=\thash password\n\t");
    printf("-v\t--verbose\tverbose\n\t");
    printf("-h\t--help\t\tUsing help\n\n");
    printf("EXAMPLES:\n\t");
    printf("dcrack --wordlist=words.txt --username=admin --realm=\"Private Area\" --nonce=1447149417 --url=/ --nc=00000001 --cnonce=69dd8dd24dd85752 --qop=auth --method=GET --response=a36f9b9239f1c8bf427f9a66db2a9e90");
}

int crack(struct options *opts, struct wordlists *word){
    char h1[LEN_HASH + 1] = {'\0'};
    char h2[LEN_HASH + 1] = {'\0'};
    char h3[LEN_HASH + 1] = {'\0'};
    char *payload = NULL;
    size_t len = 0;
    struct lists *ls = word->words;
    while(ls != NULL){
        len = strlen(opts->username) + strlen(opts->realm) + strlen(ls->word) + strlen(opts->method) + strlen(opts->url) + strlen(opts->nonce) + strlen(opts->nc) + strlen(opts->cnonce) + strlen(opts->qop) + 72;
        if((payload = (char *)calloc(len + 1, sizeof(char))) != NULL){
            sprintf(payload, "%s:%s:%s", opts->username, opts->realm, ls->word);
            md5(payload, strlen(payload), h1);
            memset(payload, '\0', len);

            sprintf(payload, "%s:%s", opts->method, opts->url);
            md5(payload, strlen(payload), h2);
            memset(payload, '\0', len);

            sprintf(payload, "%s:%s:%s:%s:%s:%s", h1, opts->nonce, opts->nc, opts->cnonce, opts->qop, h2);
            md5(payload, strlen(payload), h3);
            free(payload);
            if(strcmp(opts->response, h3) == 0){
                printf("[+] Found: %s\n\n", ls->word);
                return 0;
            }
            else if(opts->verbose)
                printf("%s\n", ls->word);
        }
        else
            die("[-] Error allocation memory!");
        ls = ls->next;
    }
    return EOF;
}

int main(int argc, char **argv){
    struct wordlists *words = NULL;
    struct options opts;
    struct option longopts[] = {
        {"wordlist", 1, NULL, 'w'},
        {"username", 1, NULL, 'U'},
        {"realm", 1, NULL, 'r'},
        {"nonce", 1, NULL, 'n'},
        {"url", 1, NULL, 'u'},
        {"nc", 1, NULL, 'c'},
        {"cnonce", 1, NULL, 'e'},
        {"qop", 1, NULL, 'q'},
        {"method", 1, NULL, 'm'},
        {"response", 1, NULL, 'o'},
        {"help", 0, NULL, 'h'},
        {"verbose", 0, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };
    char shortopts[] = "w:U:r:n:u:c:e:q:m:o:hv";
    int opt = 0;
    int index_opt = 0;
    opterr = false;
    opts.verbose = false;
    while((opt = getopt_long(argc, argv, shortopts, longopts, &index_opt)) != EOF){
        switch(opt){
            case 'w':
                opts.wordlist = optarg;
                break;
            case 'U' :
                opts.username = optarg;
                break;
            case 'r' :
                opts.realm = optarg;
                break;
            case 'n' :
                opts.nonce = optarg;
                break;
            case 'u' :
                opts.url = optarg;
                break;
            case 'c' :
                opts.nc = optarg;
                break;
            case 'e' :
                opts.cnonce = optarg;
                break;
            case 'q' :
                opts.qop = optarg;
                break;
            case 'm' :
                opts.method = optarg;
                break;
            case 'o' :
                opts.response = optarg;
                break;
            case 'h' :
                help();
                return 0;
            case 'v' :
                opts.verbose = true;
                break;
        }
    }
    if(argc >= 11){
        printf("Username:...%s\n", opts.username);
        printf("realm:......%s\n", opts.realm);
        printf("nonce:......%s\n", opts.nonce);
        printf("url:........%s\n", opts.url);
        printf("nc:.........%s\n", opts.nc);
        printf("cnonce:.....%s\n", opts.cnonce);
        printf("qop:........%s\n", opts.qop);
        printf("method:.....%s\n", opts.method);
        printf("response:...%s\n\n", opts.response);

        printf("[!] Generating wordlists...\n\n");
        if((words = read_wordlists(opts.wordlist))){
            printf("[+] Wordlist: %s\n[+] words: %ld\n\n", opts.wordlist, words->count);
            sleep(1);
            printf("[!] Start crack...\n\n");
            crack(&opts, words);
            free_wordlists(words);
        }
        else
            die("[-] File not found!");
        printf("[!] Finish crack...\n");
    }
    return 0;
}
