#ifndef _CLI_H
#define _CLI_H
#include <stddef.h>

typedef struct _command_t {
    char* name;
    void *data;
    void (*fn)(void* data);
} command_t;

typedef struct _command_node_t {
    command_t *cmd;
    struct _command_node_t *prev;
    struct _command_node_t *next;
} command_node_t;

typedef struct _command_list_t {
    struct _command_node_t *begin;
    struct _command_node_t *end;
} command_list_t;



// void
void execCmd(const char* cmdstr, const char* params);
void parseCmd(char* cmd);
void appendCmd(const char* name, void (*function)(void *data), void *data);
void removeCmd(const char* name);
command_node_t* findCmd(const char* name);
void nullCmd(void* data);
void listCmd();
void CLI_init();
int CLI_loop();

#endif
