#ifndef _SCORE_H_
#define _SCORE_H_

#define MAX_SIZE 10
#define FILE_NAME "scores"

typedef struct {
    int size;
    int* scores;
} scores_t;

void free_scores(scores_t* scores);

scores_t* init_scores();

void save_score(scores_t* scores, int score);

#endif
