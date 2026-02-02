#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie {
    char* title;
    int year;
    char** langs;
    float rating;
    struct movie* next;
};

struct movie* create_movie(char* title, int year, char** langs, float rating) {
    struct movie* movie = malloc(sizeof(struct movie));
    movie->title = title;
    movie->year = year;
    movie->langs = langs;
    movie->rating = rating;
    movie->next = NULL;
    return movie;
}

char** parseLangs(char* langStr, int* langCount) {
    // langStr comes in as "[English;French;Spanish]"
    // Remove brackets
    char* start = langStr + 1;  // skip '['
    char* end = strchr(start, ']');
    if (end) *end = '\0';

    // Count languages
    int count = 1;
    for (char* p = start; *p; p++) {
        if (*p == ';') count++;
    }

    char** langs = malloc(sizeof(char*) * (count + 1));
    int i = 0;
    char* saveptr;
    char* token = strtok_r(start, ";", &saveptr);
    while (token != NULL) {
        langs[i] = strdup(token);
        i++;
        token = strtok_r(NULL, ";", &saveptr);
    }
    langs[i] = NULL;  // null terminate array
    *langCount = count;
    return langs;
}

struct movie* readMovieFile(char* filePath) {
    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    FILE *movieFile = fopen(filePath, "r");
    struct movie* head = NULL;
    struct movie* tail = NULL;
    int lineNum = 0;
    int movieCount = 0;

    if (movieFile == NULL) {
        printf("Cannot open file %s\n", filePath);
        return NULL;
    }

    while ((nread = getline(&currLine, &len, movieFile)) != -1) {
        lineNum++;
        // Skip header line
        if (lineNum == 1) continue;

        // Remove newline
        if (currLine[nread - 1] == '\n') {
            currLine[nread - 1] = '\0';
        }

        // Parse: Title,Year,[langs],Rating
        char* token;
        char* saveptr;
        char* lineCopy = strdup(currLine);

        // Get title (everything before first comma)
        token = strtok_r(lineCopy, ",", &saveptr);
        char* title = strdup(token);

        // Get year
        token = strtok_r(NULL, ",", &saveptr);
        int year = atoi(token);

        // Get languages - need to handle brackets
        token = strtok_r(NULL, ",", &saveptr);
        int langCount;
        char** langs = parseLangs(token, &langCount);

        // Get rating
        token = strtok_r(NULL, ",", &saveptr);
        float rating = atof(token);

        // Create movie node and add to list
        struct movie* newMovie = create_movie(title, year, langs, rating);

        if (head == NULL) {
            head = newMovie;
            tail = newMovie;
        } else {
            tail->next = newMovie;
            tail = newMovie;
        }
        movieCount++;
        free(lineCopy);
    }

    free(currLine);
    fclose(movieFile);
    printf("Processed file %s and parsed data for %d movies\n", filePath, movieCount);
    return head;
}

void showMoviesByYear(struct movie* list) {
    int year;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &year);

    struct movie* curr = list;
    int found = 0;
    while (curr != NULL) {
        if (curr->year == year) {
            printf("%s\n", curr->title);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found) {
        printf("No data about movies released in the year %d\n", year);
    }
}

void showHighestRatedByYear(struct movie* list) {
    // Find all unique years and track highest rated movie for each
    // First pass: find min and max years
    int minYear = 9999, maxYear = 0;
    struct movie* curr = list;
    while (curr != NULL) {
        if (curr->year < minYear) minYear = curr->year;
        if (curr->year > maxYear) maxYear = curr->year;
        curr = curr->next;
    }

    // For each year in range, find highest rated
    for (int y = minYear; y <= maxYear; y++) {
        struct movie* best = NULL;
        curr = list;
        while (curr != NULL) {
            if (curr->year == y) {
                if (best == NULL || curr->rating > best->rating) {
                    best = curr;
                }
            }
            curr = curr->next;
        }
        if (best != NULL) {
            printf("%d %.1f %s\n", best->year, best->rating, best->title);
        }
    }
}

void showMoviesByLanguage(struct movie* list) {
    char lang[21];
    printf("Enter the language for which you want to see movies: ");
    scanf("%s", lang);

    struct movie* curr = list;
    int found = 0;
    while (curr != NULL) {
        // Check if this movie has the language
        char** l = curr->langs;
        while (*l != NULL) {
            if (strcmp(*l, lang) == 0) {
                printf("%d %s\n", curr->year, curr->title);
                found = 1;
                break;
            }
            l++;
        }
        curr = curr->next;
    }
    if (!found) {
        printf("No data about movies released in %s\n", lang);
    }
}

void printMenu() {
    printf("\n1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
    printf("Enter a choice from 1 to 4: ");
}

int main(int argc, char **argv){
    if(argc < 2){
        printf("PROVIDE VALID FILEPATH\n");
        return EXIT_FAILURE;
    }
    struct movie* movieList = readMovieFile(argv[1]);

    int choice;
    do {
        printMenu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                showMoviesByYear(movieList);
                break;
            case 2:
                showHighestRatedByYear(movieList);
                break;
            case 3:
                showMoviesByLanguage(movieList);
                break;
            case 4:
                break;
            default:
                printf("You entered an incorrect choice. Try again.\n");
        }
    } while (choice != 4);

    return EXIT_SUCCESS;
}
