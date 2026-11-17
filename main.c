#include <stdio.h>
#include "validate.h"

int main(int argc, char *argv[])
{
    Slist *head = NULL;
    mainnode_t *hash_table[SIZE] = {NULL};
    int choice;
    char word[50];

    if (argc < 2)
    {
        printf("Error: Pass file names through command line\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        if (validate_file(argv[i]))
        {
            if (check_duplicate(head, argv[i]))
            {
                printf("%s : Duplicate file\n", argv[i]);
                continue;
            }

            if (insert_at_last(&head, argv[i]))
                printf("%s : File added successfully\n", argv[i]);
            else
                printf("Memory allocation failed\n");
        }
    }

    do
    {
        printf("\n----- INVERTED SEARCH MENU -----\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Database\n");
        printf("4. Save Database\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                create_database(head, hash_table);
                break;

            case 2:
                display_database(hash_table);
                break;

            case 3:
                printf("Enter the word to search: ");
                scanf("%49s", word);
                search_database(hash_table, word);
                break;

            case 4:
                save_database(hash_table);
                break;

            case 5:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice\n");
        }

    } while (choice != 5);

    free_list(head);

    return 0;
}