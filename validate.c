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

void display_database(mainnode_t *hash_table[])
{
    mainnode_t *m_temp;
    subnode_t *s_temp;

    printf("\nIndex\tWord\tFile Count\tFilename\tWord Count\n");

    for (int i = 0; i < SIZE; i++)
    {
        m_temp = hash_table[i];

        while (m_temp != NULL)
        {
            printf("%d\t%s\t%d\t\t",
                   i, m_temp->word, m_temp->file_count);

            s_temp = m_temp->sub_link;

            while (s_temp != NULL)
            {
                printf("%s\t%d\t",
                       s_temp->filename, s_temp->word_count);

                s_temp = s_temp->link;
            }

            printf("\n");
            m_temp = m_temp->link;
        }
    }
}

void search_database(mainnode_t *hash_table[], char *word)
{
    int index;
    mainnode_t *temp;
    subnode_t *s_temp;

    // Convert word to lowercase
    for (int i = 0; word[i] != '\0'; i++)
    {
        word[i] = tolower((unsigned char)word[i]);
    }

    // Find index
    index = hash_function(word);

    // Access hash table index
    temp = hash_table[index];

    // Search for word
    while (temp != NULL)
    {
        if (strcmp(temp->word, word) == 0)
        {
            printf("Word \"%s\" is present in %d files\n",
                   temp->word, temp->file_count);

            s_temp = temp->sub_link;

            while (s_temp != NULL)
            {
                printf("In %s : %d times\n",s_temp->filename, s_temp->word_count);

                s_temp = s_temp->link;
            }

            return;
        }

        temp = temp->link;
    }

    printf("Word \"%s\" is not present in the database\n", word);
}

void save_database(mainnode_t *hash_table[])
{
    FILE *fp;
    char filename[100];
    char *ext;
    mainnode_t *m_temp;
    subnode_t *s_temp;

    printf("Enter the file name to save: ");
    scanf("%99s", filename);

    ext = strrchr(filename, '.');

    if (ext == NULL || strcmp(ext, ".txt") != 0)
    {
        printf("Error: Invalid file extension\n");
        return;
    }

    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Error: Unable to open file\n");
        return;
    }

    for (int i = 0; i < SIZE; i++)
    {
        m_temp = hash_table[i];

        if (m_temp != NULL)
        {
            fprintf(fp, "#%d;", i);

            while (m_temp != NULL)
            {
                fprintf(fp, "%s;%d;",
                        m_temp->word, m_temp->file_count);

                s_temp = m_temp->sub_link;

                while (s_temp != NULL)
                {
                    fprintf(fp, "%s;%d;",
                            s_temp->filename,
                            s_temp->word_count);

                    s_temp = s_temp->link;
                }

                m_temp = m_temp->link;
            }

            fprintf(fp, "#\n");
        }
    }

    fclose(fp);

    printf("Database saved successfully\n");
}
