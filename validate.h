#ifndef VALIDATE_H
#define VALIDATE_H

#include "types.h"

int validate_file(char *filename);

int check_duplicate(Slist *head, char *filename);

int insert_at_last(Slist **head, char *filename);

void print_list(Slist *head);

void free_list(Slist *head);

int hash_function(char *word);

int insert_word(mainnode_t **head, char *word, char *filename);

void create_database(Slist *head, mainnode_t *hash_table[]);

#endif