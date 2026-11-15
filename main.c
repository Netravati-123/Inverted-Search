#include <stdio.h>
#include "validate.h"

int main(int argc, char *argv[])
{
    Slist *head = NULL;

    mainnode_t *hash_table[SIZE] = {NULL};

    if (argc < 2)
    {
        printf("Error: Pass file names through command line\n");
        return 1;
    }

    // Validate and insert filenames
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
            {
                printf("%s : File added successfully\n", argv[i]);
            }
            else
            {
                printf("Memory allocation failed\n");
            }
        }
    }

    // Create database
    printf("\nCreating Database...\n");

    create_database(head, hash_table);

    // Print valid filenames
    printf("\nValid files:\n");

    print_list(head);

    // Free filename linked list
    free_list(head);

    return 0;
}