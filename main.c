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
    int bufferSize = 128; // Начальный размер буфера
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
            bufferSize += 128; // Увеличиваем размер буфера
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

    //user_pref("mail.smtpserver.smtp0.clientid", "5cc07b73-f7dc-46d6-aed3-d43aa1225dfe");

    //user_pref("mail.smtpserver.smtp0.description", "");
    //fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.authMethod\", 4);", num);

    //user_pref("mail.smtpserver.smtp0.hostname", "mail.ickkp.ru");
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.hostname\", \"%s\");\n", num, host);


    //user_pref("mail.smtpserver.smtp0.port", "587");
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.port\", \"%s\");\n", num, port);

    //user_pref("mail.smtpserver.smtp0.try_ssl", 2);
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.try_ssl\", 2);\n", num);

    //user_pref("mail.smtpserver.smtp0.username", "vpopov@ickkp.ru");
    fprintf(file, "user_pref(\"mail.smtpserver.smtp%d.username\", \"%s\");\n", num, username);
}

void add_server(FILE *file, int num, char *host, char *acname, char *servtype, char *username){
    //user_pref("mail.server.server0.authMethod", 4);
    fprintf(file, "user_pref(\"mail.server.server%d.authMethod\", 4);\n", num);

    //user_pref("mail.server.server0.hostname", "mail.ickkp.ru");
    fprintf(file, "user_pref(\"mail.server.server%d.hostname\", \"%s\");\n", num, host);

    //user_pref("mail.server.server0.name", "Account name");
    fprintf(file, "user_pref(\"mail.server.server%d.name\", \"%s\");\n", num, acname);

    //user_pref("mail.server.server0.type", "imap");
    fprintf(file, "user_pref(\"mail.server.server%d.type\", \"%s\");\n", num, servtype);

    //user_pref("mail.server.server0.userName", "vpopov@ickkp.ru");
    fprintf(file, "user_pref(\"mail.server.server%d.userName\", \"%s\");\n", num, username);
}

void add_identity(FILE *file, int num, char *fullname, int smtp_num, char *useremail){
    //user_pref("mail.identity.identity1.fullName", "User Userov");
    fprintf(file, "user_pref(\"mail.identity.identity%d.fullName\", \"%s\");\n", num, fullname);

    //user_pref("mail.identity.identity1.smtpServer", "smtp0");
    fprintf(file, "user_pref(\"mail.identity.identity%d.smtpServer\", \"smtp%d\");\n", num, smtp_num);

    //user_pref("mail.identity.identity1.useremail", "vpopov@ickkp.ru");
    fprintf(file, "user_pref(\"mail.identity.identity%d.useremail\", \"%s\");\n", num, useremail);

    //user_pref("mail.identity.identity1.valid", true);
    fprintf(file, "user_pref(\"mail.identity.identity%d.valid\", true);\n", num);
}

void add_account(FILE *file, int num, int server_num, int identity_num){
    //user_pref("mail.account.account1.identities", "identity1");
    fprintf(file, "user_pref(\"mail.account.account%d.identities\", \"identity%d\");\n", num, identity_num);

    //user_pref("mail.account.account1.server", "server0");
    fprintf(file, "user_pref(\"mail.account.account%d.server\", \"server%d\");\n", num, server_num);
}


void add_login_head(FILE *file){
    fprintf(file, "{\"logins\": [");
}

void add_login_smtp(FILE *file, int itr, char *host, char *username, char *passwd){
    if (itr == 1)
        fprintf(file, "{\"id\": %d, \"hostname\": \"smtp://%s\", \"httpRealm\": \"smtp://%s\","
                      " \"formSubmitURL\": null, \"usernameField\": \"\", \"passwordField\": \"\","
                      " \"encryptedUsername\": \"%s\", \"encryptedPassword\": \"%s\", \"encType\": 1,\"timesUsed\": 1}", itr, host, host, username, passwd);
    else
        fprintf(file, ",{\"id\": %d, \"hostname\": \"smtp://%s\", \"httpRealm\": \"smtp://%s\","
                      " \"formSubmitURL\": null, \"usernameField\": \"\", \"passwordField\": \"\","
                      " \"encryptedUsername\": \"%s\", \"encryptedPassword\": \"%s\", \"encType\": 1,\"timesUsed\": 1}", itr, host, host, username, passwd);
}
void add_login_server(FILE *file, int itr, char *typeserv, char *host, char *username, char *passwd){
    if (itr == 1)
        fprintf(file, "{\"id\": %d, \"hostname\": \"%s://%s\", \"httpRealm\": \"%s://%s\","
                      " \"formSubmitURL\": null, \"usernameField\": \"\", \"passwordField\": \"\","
                      " \"encryptedUsername\": \"%s\", \"encryptedPassword\": \"%s\", \"encType\": 1,\"timesUsed\": 1}", itr, typeserv, host, typeserv, host, username, passwd);
    else
        fprintf(file, ",{\"id\": %d, \"hostname\": \"%s://%s\", \"httpRealm\": \"%s://%s\","
                      " \"formSubmitURL\": null, \"usernameField\": \"\", \"passwordField\": \"\","
                      " \"encryptedUsername\": \"%s\", \"encryptedPassword\": \"%s\", \"encType\": 1,\"timesUsed\": 1}", itr, typeserv, host, typeserv, host, username, passwd);
}

void add_login_footer(FILE *file, int nextid){
    fprintf(file, "],\"nextId\": %d, \"potentiallyVulnerablePasswords\": [], \"dismissedBreachAlertsByLoginGUID\": {}, \"version\": 3 }", nextid);
    fclose(file);
}


int main(int argc, char *argv[]) {

    int c;
    char *profile_path;
    char *config_path;
    char *path_prefjs[256];
    char *path_loginjsn[256];
    char delimiter[] = ";";
    char *token;
    char *line = NULL;
    char *config_arg[6];
    int config_col_itr = 0;
    int smtpserver_it = 1;
    int server_it = 1;
    int identity_it = 1;
    int account_it = 1;
    int logins_it=1;

    char *encusername;
    char *encpassword;


    // region option parser
    while ((c = getopt(argc, argv, "hp:c:")) != -1) {
        switch (c) {
            case 'h':
                printf("Usage: %s -h -c <path_config> -p <profile_path>\n", argv[0]);
                return 0;

            case 'p':
                printf("Profile: %s\n", optarg);
                profile_path = strdup(optarg);
                sprintf(path_prefjs, "%s\\prefs.js", profile_path);
                sprintf(path_loginjsn, "%s\\logins.json", profile_path);
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
    FILE *logins_file = fopen(path_loginjsn, "w");

    HINSTANCE hDll = LoadLibrary("nss3.dll");

    if (config_file == NULL) {
        fprintf(stderr, "Open config file error\n");
        return 1;
    }
    if (pref_file == NULL) {
        fprintf(stderr, "Open pref file error\n");
        return 2;
    }
    if (logins_file == NULL) {
        fprintf(stderr, "Open logins file error\n");
        return 3;
    }
    if (hDll == NULL) {
        DWORD error = GetLastError();
        printf("Error load library code: %u\n", error);
        return 4;
    }


    CRPAPI_init(hDll, profile_path);
    add_login_head(logins_file);

    while ((line = readline(config_file)) != NULL) {
        for (int i = 0; i < 6; i++) {
            config_arg[i] = NULL;
        }
        printf("%s\n", line);

        token = strtok(line, delimiter);


        while (token != NULL) {
            config_arg[config_col_itr] = strdup(token);
            token = strtok(NULL, delimiter);
            config_col_itr++;
        }

        if (strcmp(config_arg[0], "smtpserver") == 0){
            add_smtpserver(pref_file, smtpserver_it, config_arg[1], config_arg[2], config_arg[3]);

            CRPAPI_encrypt(hDll, config_arg[3], &encusername);
            CRPAPI_encrypt(hDll, config_arg[4], &encpassword);
            add_login_smtp(logins_file, logins_it, config_arg[1], encusername, encpassword);
            logins_it++;
            smtpserver_it++;
            free(encusername);
            free(encpassword);
        } else if (strcmp(config_arg[0], "server") == 0){
            add_server(pref_file, server_it, config_arg[1], config_arg[2], config_arg[5], config_arg[3]);
            CRPAPI_encrypt(hDll, config_arg[3], &encusername);
            CRPAPI_encrypt(hDll, config_arg[4], &encpassword);
            add_login_server(logins_file, logins_it,  config_arg[5], config_arg[1], encusername, encpassword);
            logins_it++;
            server_it++;
            free(encusername);
            free(encpassword);
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

    add_login_footer(logins_file, logins_it);



    CRPAPI_shutdown(hDll);
    fclose(config_file);
    fclose(pref_file);
    fclose(logins_file);
    FreeLibrary(hDll);
    return 0;
}
