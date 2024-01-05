#include "cli.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

static command_list_t COMMANDS;

void nullCmd(void *data) {
    command_t* cmd = (command_t*) data;
    printf("NullFn: %s\n", cmd->name);
    if (cmd->data) {
        printf("params: %s\n", (const char*) cmd->data);
    }
}

void execCmd(const char* cmdstr, const char* params) {
    /*
    for (command_t *cmd = cmds; cmd->data != NULL; cmd++) {
        if (!strcmp(cmd->name, cmdstr)) {
            cmd->fn(cmd->data);
        }
    }
    */
    command_node_t *n = findCmd(cmdstr);
    if (!n) {
        printf("No such command\n");
        return;
    }
    if (n->cmd->name) printf("Executing %s\n", n->cmd->name);
    if (!n->cmd->data) n->cmd->data = (void*) params;
    printf("Foind command: %s", n->cmd->name);
    n->cmd->fn(n->cmd->data);
}

void parseCmd(char* cmd) {
    char *command;
    char *params;
    uint8_t length = 0;
    command = cmd;
    for (char *c = cmd; *c != 0; c++) {
        if (*c == ' ') {
            command[c-cmd]='\0';
            c++;
            params = c;
        }
    }
    execCmd(command, params);
}

void appendCmd(const char* name, void (*function)(void *data), void *data) {
    command_node_t *node = findCmd(name);
    if (!node) {
        printf("Trying to allocate\n");
        node = malloc(sizeof(command_node_t));
        printf("Allocated\n");
    }
    node->cmd = malloc(sizeof(command_t));
    node->cmd->name = (char*) name;
    printf("--- name\n");
    node->cmd->fn = function;
    printf("--- fn\n");
    node->cmd->data = data;  
    printf("--- data\n");
    printf("Trying to add to container\n");
    if (!COMMANDS.begin && !COMMANDS.end) {
        node->prev = NULL;
        node->next = NULL;
        COMMANDS.begin = node;    
        COMMANDS.end = node;
    } else {
        node->prev = COMMANDS.end;
        node->prev->next = node;
        node->next = NULL;
        COMMANDS.end = node;
    }
    
    printf("Added command %s\n", node->cmd->name);
    printf("Begin %s\n", COMMANDS.begin->cmd->name);
    printf("End %s\n", COMMANDS.end->cmd->name);
}

void removeCmd(const char* name) {
    command_node_t *node = findCmd(name);
    if (!node) return;
    node->prev->next = node->next;
    free(node);
}

command_node_t* findCmd(const char* name) {
    printf("Searching command %s\n", name);
    for (command_node_t *it = COMMANDS.begin;
         it != NULL;
         it=it->next) {
        if (it && !strcmp(it->cmd->name, name)) {
            printf("FOUND\n");
            return it;
        }
    }
    printf("NOTHING FOUND\n");
    return NULL;
}

void listCmd() {
    for (command_node_t *it = COMMANDS.begin;
         it != NULL;
         it = it->next) {
             if (it) printf("==> %s\n", it->cmd->name);
         }
}

void CLI_init() {
    COMMANDS.begin = NULL;
    COMMANDS.end = NULL;
}

int CLI_loop() {
    char cmd[64];
    printf("\n>");
    fgets(cmd, 64, stdin);
    if (!strcmp(cmd, "quit\n")) return 0;
    if (!strcmp(cmd, "list\n")) {
        listCmd();
        return CLI_loop();    
    }
    else {
        parseCmd(cmd);
        return CLI_loop();
    }
}
