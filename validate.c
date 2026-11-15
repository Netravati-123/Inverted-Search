#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validate.h"

// Validate file
int validate_file(char *filename)
{
    char *ext;
    FILE *fp;
    int ch;

    ext = strrchr(filename, '.');

    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("%s : Invalid extension\n", filename);
        return 0;
    }

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("%s : File does not exist\n", filename);
        return 0;
    }

    ch = fgetc(fp);

    if (ch == EOF)
    {
        printf("%s : File is empty\n", filename);
        fclose(fp);
        return 0;
    }

    fclose(fp);

    return 1;
}


// Check duplicate filename
int check_duplicate(Slist *head, char *filename)
{
    while (head != NULL)
    {
        if (strcmp(head->filename, filename) == 0)
        {
            return 1;
        }

        head = head->link;
    }

    return 0;
}


// Insert filename at last
int insert_at_last(Slist **head, char *filename)
{
    Slist *newnode;
    Slist *temp;

    newnode = malloc(sizeof(Slist));

    if (newnode == NULL)
    {
        return 0;
    }

    strcpy(newnode->filename, filename);
    newnode->link = NULL;

    if (*head == NULL)
    {
        *head = newnode;
        return 1;
    }

    temp = *head;

    while (temp->link != NULL)
    {
        temp = temp->link;
    }

    temp->link = newnode;

    return 1;
}


// Print linked list
void print_list(Slist *head)
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    while (head != NULL)
    {
        printf("%s -> ", head->filename);
        head = head->link;
    }

    printf("NULL\n");
}


// Free linked list
void free_list(Slist *head)
{
    Slist *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->link;
        free(temp);
    }
}


// Hash function
int hash_function(char *word)
{
    int index;

    index = tolower((unsigned char)word[0]) - 'a';

    if (index < 0 || index > 25)
    {
        index = 26;
    }

    return index;
}


// Insert word into database
int insert_word(mainnode_t **head, char *word, char *filename)
{
    mainnode_t *temp = *head;
    subnode_t *sub_temp;
    subnode_t *new_sub;
    mainnode_t *new_main;

    // Check whether word already exists
    while (temp != NULL)
    {
        if (strcmp(temp->word, word) == 0)
        {
            sub_temp = temp->sub_link;

            // Check whether filename already exists
            while (sub_temp != NULL)
            {
                if (strcmp(sub_temp->filename, filename) == 0)
                {
                    sub_temp->word_count++;
                    return 1;
                }

                sub_temp = sub_temp->link;
            }

            // Word exists, but filename is new
            new_sub = malloc(sizeof(subnode_t));

            if (new_sub == NULL)
            {
                return 0;
            }

            strcpy(new_sub->filename, filename);
            new_sub->word_count = 1;

            new_sub->link = temp->sub_link;
            temp->sub_link = new_sub;

            temp->file_count++;

            return 1;
        }

        temp = temp->link;
    }

    // Word does not exist
    new_main = malloc(sizeof(mainnode_t));

    if (new_main == NULL)
    {
        return 0;
    }

    new_sub = malloc(sizeof(subnode_t));

    if (new_sub == NULL)
    {
        free(new_main);
        return 0;
    }

    strcpy(new_main->word, word);
    new_main->file_count = 1;

    new_main->link = *head;

    strcpy(new_sub->filename, filename);
    new_sub->word_count = 1;
    new_sub->link = NULL;

    new_main->sub_link = new_sub;

    *head = new_main;

    return 1;
}


// Create database
void create_database(Slist *head, mainnode_t *hash_table[])
{
    FILE *fp;
    char word[50];
    int index;
    int i;

    while (head != NULL)
    {
        fp = fopen(head->filename, "r");

        if (fp == NULL)
        {
            printf("Unable to open %s\n", head->filename);

            head = head->link;
            continue;
        }

        while (fscanf(fp, "%49s", word) == 1)
        {
            // Convert word to lowercase
            for (i = 0; word[i] != '\0'; i++)
            {
                word[i] = tolower((unsigned char)word[i]);
            }

            index = hash_function(word);

            if (!insert_word(&hash_table[index], word, head->filename))
            {
                printf("Memory allocation failed\n");
                fclose(fp);
                return;
            }
        }

        fclose(fp);

        head = head->link;
    }

    printf("Database created successfully\n");
}