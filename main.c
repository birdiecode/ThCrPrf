//
// Created by birdiecode on 17.02.2024.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <windows.h>
#include "crypto.h"

char* readline(FILE *file) {
    char *line = NULL;
    int position = 0;
    int bufferSize = 128;
    line = (char*)malloc(bufferSize * sizeof(char));

    if (!line) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return NULL;
    }

    int c;

    while (1) {
        c = fgetc(file);

        if (c == -1 || c == '\n') {
            line[position] = '\0';
            if (position == 0) return NULL;
            break;
        }



        line[position] = c;
        position++;

        if (position >= bufferSize) {
            bufferSize += 128;
            char *temp = realloc(line, bufferSize * sizeof(char));
            if (!temp) {
                fprintf(stderr, "Ошибка перевыделения памяти\n");
                free(line);
                return NULL;
            }
            line = temp;
        }
    }

    return line;
}


void add_smtpserver(FILE *file, int num, char *host, char *port, char *username){
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.authMethod\", 4);\n", num);
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.hostname\", \"%s\");\n", num, host);
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.port\", \"%s\");\n", num, port);
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.try_ssl\", 2);\n", num);
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.username\", \"%s\");\n", num, username);
}

void add_server(FILE *file, int num, char *host, char *acname, char *servtype, char *username){
    fprintf(file, "user_pref(\"mail.server.server%d.authMethod\", 4);\n", num);
    fprintf(file, "user_pref(\"mail.server.server%d.hostname\", \"%s\");\n", num, host);
    fprintf(file, "user_pref(\"mail.server.server%d.name\", \"%s\");\n", num, acname);
    fprintf(file, "user_pref(\"mail.server.server%d.type\", \"%s\");\n", num, servtype);
    fprintf(file, "user_pref(\"mail.server.server%d.userName\", \"%s\");\n", num, username);
}

void add_identity(FILE *file, int num, char *fullname, int smtp_num, char *useremail){
    fprintf(file, "user_pref(\"mail.identity.identity%d.fullName\", \"%s\");\n", num, fullname);
    fprintf(file, "user_pref(\"mail.identity.identity%d.smtpServer\", \"smtp%d\");\n", num, smtp_num);
    fprintf(file, "user_pref(\"mail.identity.identity%d.useremail\", \"%s\");\n", num, useremail);
    fprintf(file, "user_pref(\"mail.identity.identity%d.valid\", true);\n", num);
}

void add_account(FILE *file, int num, int server_num, int identity_num){
    fprintf(file, "user_pref(\"mail.account.account%d.identities\", \"identity%d\");\n", num, identity_num);
    fprintf(file, "user_pref(\"mail.account.account%d.server\", \"server%d\");\n", num, server_num);
}


int main(int argc, char *argv[]) {

    int c;
    char *dll_path;
    char *profile_path;
    char *config_path;
    char *path_prefjs[256];
    char delimiter[] = ";";
    char *token;
    char *line = NULL;
    char *config_arg[6];
    int config_col_itr = 0;
    int smtpserver_it = 1;
    int server_it = 1;
    int identity_it = 1;
    int account_it = 1;


    // region option parser
    while ((c = getopt(argc, argv, "hl:p:c:")) != -1) {
        switch (c) {
            case 'h':
                printf("Usage: %s -h -l <path_library_nss> -p <profile_path>\n", argv[0]);
                return 0;

            case 'p':
                printf("Profile: %s\n", optarg);
                profile_path = strdup(optarg);
                sprintf(path_prefjs, "%s\\prefs.js", profile_path);
                break;

            case 'l':
                printf("Lib file: %s\n", optarg);
                dll_path = strdup(optarg);
                break;

            case 'c':
                printf("Config file: %s\n", optarg);
                config_path = strdup(optarg);
                break;

            case '?':
                if (optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option -%c'.\n", optopt);
                }
                return 1;

            default:
                return 1;
        }
    }
    // endregion


    FILE *config_file = fopen(config_path, "r");
    FILE *pref_file = fopen(path_prefjs, "w");
    HINSTANCE hDll = LoadLibrary(dll_path);

    if (config_file == NULL) {
        fprintf(stderr, "Open config file error\n");
        return 1;
    }
    if (pref_file == NULL) {
        fprintf(stderr, "Open pref file error\n");
        return 1;
    }
    if (hDll == NULL) {
        DWORD error = GetLastError();
        printf("Error load library code: %u\n", error);
        return 1;
    }


    CRPAPI_init(hDll, profile_path);


    while ((line = readline(config_file)) != NULL) {
        // присваеваем NULL массиву строк
        for (int i = 0; i < 6; i++) {
            config_arg[i] = NULL;
        }


        token = strtok(line, delimiter);


        while (token != NULL) {
            config_arg[config_col_itr] = strdup(token);
            token = strtok(NULL, delimiter);
            config_col_itr++;
        }

        if (strcmp(config_arg[0], "smtpserver") == 0){
            add_smtpserver(pref_file, smtpserver_it, config_arg[1], config_arg[2], config_arg[3]);
            smtpserver_it++;
        } else if (strcmp(config_arg[0], "server") == 0){
            add_server(pref_file, server_it, config_arg[1], config_arg[2], config_arg[5], config_arg[3]);
            server_it++;
        } else if (strcmp(config_arg[0], "identity") == 0){
            add_identity(pref_file, identity_it, config_arg[1], atoi(config_arg[2]), config_arg[3]);
            identity_it++;
        } else if (strcmp(config_arg[0], "account") == 0){
            add_account(pref_file, account_it, atoi(config_arg[1]), atoi(config_arg[2]));
            account_it++;
        } else if (strcmp(config_arg[0], "pref") == 0){
            fprintf(pref_file, "user_pref(\"%s\", %s);\n",  config_arg[1], config_arg[2]);
        } else {
            fprintf(stderr, "Error no method: %s", config_arg[0]);
        }


        for (int i = 0; i < 6; i++) {
            free(config_arg[i]);
        }
        config_col_itr = 0;
        free(line);
    }

    fclose(config_file);

    CRPAPI_shutdown(hDll);

    FreeLibrary(hDll);
    return 0;
}
