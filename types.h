#ifndef TYPES_H
#define TYPES_H
#define SIZE 27

typedef struct subnode
{
    char filename[100];
    int word_count;
    struct subnode *link;

} subnode_t;

typedef struct mainnode
{
    char word[50];
    int file_count;
    subnode_t *sub_link;
    struct mainnode *link;

} mainnode_t;

typedef struct node
{
    char filename[100];
    struct node *link;

} Slist;

#endif