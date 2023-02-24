#include <stdio.h>

#include "helpers.h"
#include "score.h"

int compare(const void* a, const void* b) {
   return *(int*) b - *(int*) a;
}

void free_scores(scores_t* scores) {
    free(scores->scores);
    free(scores);

    return;
}

scores_t* init_scores() {
    scores_t* scores = malloc(sizeof(scores_t));
    must_init(scores, "scores");

    scores->size = 0;
    scores->scores = calloc(MAX_SIZE, sizeof(int));
    must_init(scores->scores, "scores");

    char line[STR_SIZE];
    char file[STR_SIZE];
    sprintf(file, "%s/%s", al_get_current_directory(), FILE_NAME);

    FILE* fp = fopen(file, "r");

    if (fp == NULL)
        return scores;

    fgets(line, STR_SIZE, fp);
    fclose(fp);

    scores->size = 1;

    for (int i = 0; i < strlen(line); i++)
        scores->size += (line[i] == ',');

    scores->scores[0] = atoi(strtok(line, ","));

    for (int i = 1; i < scores->size; i++)
        scores->scores[i] = atoi(strtok(NULL, ","));

    return scores;
}

void save_score(scores_t* scores, int score) {
    if (scores->size == MAX_SIZE) {
        if (score < scores->scores[MAX_SIZE - 1])
            return;

        scores->scores[MAX_SIZE - 1] = score;
    } else {
        scores->scores[scores->size] = score;
        scores->size += 1;
    }

    qsort(scores->scores, scores->size, sizeof(int), compare);

    char file[STR_SIZE];
    sprintf(file, "%s/%s", al_get_current_directory(), FILE_NAME);

    FILE* fp = fopen(file, "w");

    for (int i = 0; i < scores->size - 1; i++)
        fprintf(fp, "%d,", scores->scores[i]);

    fprintf(fp, "%d", scores->scores[scores->size - 1]);

    fclose(fp);

    return;
}
