#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Movie {
    char* title;
    int year;
    char** langs;
    float rating;
    struct Movie* next;
};

struct Movie* create_movie(char* title, int year, char** langs, float rating) {
    struct Movie* movie = malloc(sizeof(struct Movie));
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

struct Movie* readMovieFile(char* filePath) {
    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    FILE *movieFile = fopen(filePath, "r");
    struct Movie* head = NULL;
    struct Movie* tail = NULL;
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
        struct Movie* newMovie = create_movie(title, year, langs, rating);

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

int main(int argc, char **argv){
	if(argc < 2){
		printf("PROVIDE VALID FILEPATH\n");
		return EXIT_FAILURE;
	}
	struct Movie* movieList = readMovieFile(argv[1]);

	// Test: print first few movies to verify
	struct Movie* curr = movieList;
	while (curr != NULL) {
		printf("%s (%d) - Rating: %.1f\n", curr->title, curr->year, curr->rating);
		curr = curr->next;
	}

	return EXIT_SUCCESS;
}
