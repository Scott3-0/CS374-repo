#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define PREFIX "movies_"
#define EXTENSION ".csv"
#define ONID "chaudhrn"

struct movie {
    char *title;
    int year;
    char *languages;
    double rating;
    struct movie *next;
};

/* Parse a CSV line into a movie struct */
struct movie *createMovie(char *currLine) {
    struct movie *currMovie = malloc(sizeof(struct movie));

    char *saveptr;
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = strtod(token, NULL);

    currMovie->next = NULL;
    return currMovie;
}

/* Parse a CSV file into a linked list of movies */
struct movie *processFile(char *filePath) {
    FILE *movieFile = fopen(filePath, "r");
    if (movieFile == NULL) {
        printf("Could not open file %s\n", filePath);
        return NULL;
    }

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;

    struct movie *head = NULL;
    struct movie *tail = NULL;

    /* Skip the header line */
    nread = getline(&currLine, &len, movieFile);

    while ((nread = getline(&currLine, &len, movieFile)) != -1) {
        struct movie *newNode = createMovie(currLine);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    free(currLine);
    fclose(movieFile);
    return head;
}

/* Free the linked list of movies */
void freeMovies(struct movie *list) {
    struct movie *tmp;
    while (list != NULL) {
        tmp = list;
        list = list->next;
        free(tmp->title);
        free(tmp->languages);
        free(tmp);
    }
}

/* Find the largest file matching movies_*.csv in the current directory */
void findLargestFile(char *result) {
    DIR *d = opendir(".");
    struct dirent *entry;
    struct stat fileStat;
    off_t maxSize = -1;
    result[0] = '\0';

    while ((entry = readdir(d)) != NULL) {
        if (strncmp(entry->d_name, PREFIX, strlen(PREFIX)) == 0) {
            char *ext = strstr(entry->d_name, EXTENSION);
            if (ext != NULL && strcmp(ext, EXTENSION) == 0) {
                stat(entry->d_name, &fileStat);
                if (fileStat.st_size > maxSize) {
                    maxSize = fileStat.st_size;
                    strcpy(result, entry->d_name);
                }
            }
        }
    }
    closedir(d);
}

/* Find the smallest file matching movies_*.csv in the current directory */
void findSmallestFile(char *result) {
    DIR *d = opendir(".");
    struct dirent *entry;
    struct stat fileStat;
    off_t minSize = -1;
    result[0] = '\0';

    while ((entry = readdir(d)) != NULL) {
        if (strncmp(entry->d_name, PREFIX, strlen(PREFIX)) == 0) {
            char *ext = strstr(entry->d_name, EXTENSION);
            if (ext != NULL && strcmp(ext, EXTENSION) == 0) {
                stat(entry->d_name, &fileStat);
                if (minSize == -1 || fileStat.st_size < minSize) {
                    minSize = fileStat.st_size;
                    strcpy(result, entry->d_name);
                }
            }
        }
    }
    closedir(d);
}

/* Create directory and write year files with movie titles */
void writeFiles(struct movie *list) {
    /* Generate random directory name */
    srand(time(NULL));
    int randNum = rand() % 100000;
    char dirName[256];
    sprintf(dirName, "%s.movies.%d", ONID, randNum);

    /* Create directory with rwxr-x--- permissions */
    mkdir(dirName, 0750);
    printf("Created directory with name %s\n\n", dirName);

    /* Write movie titles to year files */
    struct movie *curr = list;
    while (curr != NULL) {
        char filePath[512];
        sprintf(filePath, "%s/%d.txt", dirName, curr->year);

        FILE *fp = fopen(filePath, "a");
        fprintf(fp, "%s\n", curr->title);
        fclose(fp);

        /* Set file permissions to rw-r----- */
        chmod(filePath, 0640);

        curr = curr->next;
    }
}

/* File selection submenu */
void fileSelectionMenu() {
    int choice;
    char fileName[256];

    while (1) {
        printf("\nWhich file you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n\n");
        printf("Enter a choice from 1 to 3: ");
        scanf("%d", &choice);

        if (choice == 1) {
            findLargestFile(fileName);
        } else if (choice == 2) {
            findSmallestFile(fileName);
        } else if (choice == 3) {
            printf("Enter the complete file name: ");
            scanf("%s", fileName);
            /* Check if file exists */
            if (access(fileName, F_OK) != 0) {
                printf("The file %s was not found. Try again\n", fileName);
                continue;
            }
        } else {
            printf("You entered an incorrect choice. Try again.\n");
            continue;
        }

        printf("Now processing the chosen file named %s\n", fileName);
        struct movie *list = processFile(fileName);
        writeFiles(list);
        freeMovies(list);
        return;
    }
}

int main() {
    int choice;

    while (1) {
        printf("1. Select file to process\n");
        printf("2. Exit the program\n\n");
        printf("Enter a choice 1 or 2: ");
        scanf("%d", &choice);

        if (choice == 1) {
            fileSelectionMenu();
        } else if (choice == 2) {
            return EXIT_SUCCESS;
        } else {
            printf("You entered an incorrect choice. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}
