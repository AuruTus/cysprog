#include "ast.h"
#include "alloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

struct node* make_node(char* data, struct node* next) {
    struct node* t = malloc(sizeof(*t));
    t->data = data;
    t->next = next;
    return t;
}

// constructor of each AST node
Cmd_t Cmd_Seq_new(Cmd_t left, Cmd_t right) {
    Cmd_Seq cmd;
    NEW(cmd);
    cmd->type = CMD_SEQ;
    cmd->left = left;
    cmd->right = right;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Back_new(Cmd_t back) {
    Cmd_Back cmd;

    NEW(cmd);
    cmd->type = CMD_BACK;
    cmd->back = back;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Pipe_new(Cmd_t left, Cmd_t right) {
    Cmd_Pipe cmd;

    NEW(cmd);
    cmd->type = CMD_PIPE;
    cmd->left = left;
    cmd->right = right;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Redir_new(Cmd_t left, Cmd_t right, int fd) {
    Cmd_Redir cmd;

    NEW(cmd);
    cmd->type = CMD_REDIR;
    cmd->left = left;
    cmd->right = right;
    cmd->fd = fd;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Atom_new(struct node* node) {
    Cmd_Atom cmd;

    NEW(cmd);
    cmd->type = CMD_ATOM;
    cmd->node = node;

    return (Cmd_t)cmd;
}

// print AST
void Cmd_print(struct Cmd_t* cmd) {
    switch (cmd->type) {
    case CMD_ATOM: {
        struct Cmd_Atom* t = (struct Cmd_Atom*)cmd;
        struct node* node = t->node;
        while (node) {
            printf("%s ", node->data);
            node = node->next;
        }
        printf("\n");
        break;
    }

    case CMD_SEQ: {
        printf("enter\n");
        struct Cmd_Seq* t = (struct Cmd_Seq*)cmd;
        Cmd_t left = t->left;
        Cmd_t right = t->right;

        printf("seq_left: ");
        Cmd_print(left);
        printf("; ");
        printf("seq_right: ");
        Cmd_print(right);
        break;
    }

    case CMD_BACK: {
        Cmd_Back t = (Cmd_Back)cmd;
        Cmd_t back = t->back;

        printf("back: ");
        Cmd_print(back);
        break;
    }

    case CMD_PIPE: {
        Cmd_Pipe t = (Cmd_Pipe)cmd;
        Cmd_t left = t->left;
        Cmd_t right = t->right;

        printf("pipe\n");
        printf("pipe_left: ");
        Cmd_print(left);
        printf("; ");
        printf("pipe_right: ");
        Cmd_print(right);
        break;
    }

    case CMD_REDIR: {
        Cmd_Redir t = (Cmd_Redir)cmd;
        Cmd_t left = t->left;
        Cmd_t right = t->right;
        int fd = t->fd;

        printf("redir: %d\n", fd);
        printf("redir_left: ");
        Cmd_print(left);
        printf("; ");
        printf("redir_right: ");
        Cmd_print(right);
        break;
    }
    default:
        break;
    }
}

// run cmd
void Cmd_run(struct Cmd_t* cmd) {
    switch (cmd->type) {
    case CMD_ATOM: {
        char* arg[10] = { 0 };
        int i = 0;
        struct Cmd_Atom* t = (struct Cmd_Atom*)cmd;
        struct node* node = t->node;
        while (node) {
            arg[i] = node->data;
            node = node->next;
            i++;
        }
        char* root = "/bin/";
        char binPath[50];
        strcpy(binPath, root);
        strcat(binPath, arg[0]);
        if (execv(binPath, arg) == -1) {
            char* path = "/usr/bin/";
            char usrBinPath[50];
            strcpy(usrBinPath, path);
            strcat(usrBinPath, arg[0]);
            if (execv(usrBinPath, arg) == -1) {
                fprintf(stderr, "cannot run command, check your input.\n");
            }
        }
        break;
    }

    case CMD_SEQ: {
        struct Cmd_Seq* t = (struct Cmd_Seq*)cmd;
        Cmd_t left = t->left;
        Cmd_t right = t->right;

        switch (fork()) {
        case -1:
            fprintf(stderr, "cannot spawn child process.\n");
            break;
        case 0:
            Cmd_run(left);
            break;
        default:
            wait(NULL);
            Cmd_run(right);
            break;
        }

        break;
    }

    case CMD_BACK: {
        Cmd_Back t = (Cmd_Back)cmd;
        Cmd_t back = t->back;

        switch (fork()) {
        case -1:
            fprintf(stderr, "cannot spawn child process.\n");
            break;
        case 0:
            Cmd_run(back);
            break;
        default:
            break;
        }

        break;
    }

    case CMD_PIPE: {
        Cmd_Pipe t = (Cmd_Pipe)cmd;
        Cmd_t left = t->left;
        Cmd_t right = t->right;
        int fd[2];

        pipe(fd);

        switch (fork()) {
        case -1:
            fprintf(stderr, "cannot spawn child process.\n");
            break;
        case 0:
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            Cmd_run(left);
            close(fd[1]);
            break;
        default:
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            wait(NULL);
            Cmd_run(right);
            close(fd[0]);
            break;
        }

        break;
    }

    case CMD_REDIR: {

        break;
    }

    default:
        break;
    }

    exit(0);
}
