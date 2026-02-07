#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie {
    char *title;
    int year;
    char *languages;
    double rating;
    struct movie *next;
};

/* Parse the current line which is comma delimited and create a
 * movie struct with the data in this line
 */
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

/* Return a linked list of movies by parsing data from each line
 * of the specified file.
 */
struct movie *processFile(char *filePath) {
    FILE *movieFile = fopen(filePath, "r");
    if (movieFile == NULL) {
        printf("Could not open file %s\n", filePath);
        return NULL;
    }

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    int count = 0;

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
        count++;
    }

    free(currLine);
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n", filePath, count);
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }

    struct movie *list = processFile(argv[1]);
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    /* TODO: Add interactive menu and processing functions */

    freeMovies(list);
    return EXIT_SUCCESS;
}
