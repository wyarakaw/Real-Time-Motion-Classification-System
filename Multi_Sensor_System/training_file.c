//
// Created by Amaael Antonini on 4/11/17.
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"

void training_file(float ***classifiers, const char * file_name, int *row_counts, const int *activities, int inputs, int s, int n, int a)
{
    printf("training\n");
    char * direct = "";
    int total, i, j, k;
    char * train_file;
    train_file = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(train_file, 0, BUFF_SIZE);
    sprintf(train_file, "%s%s", direct, file_name);
    total = 0;
    for(i = s; i< n; i++)
        total+= row_counts[i];
    FILE *fp;
    printf("total: %d\n", total);

    fp = fopen(train_file, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to write to file \'%s\'.\n",
                train_file
        );
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d\t%d\t%d\n", total, inputs, a);
    for(i = s; i < n; i++) {
        for (j = 0; j < row_counts[i]; j++) {
//            printf("%d\t %d\t %d\n", n, dimen[i], counts[i][j]);
            for (k = 0; k < inputs; k++) {
                fprintf(fp, "%f\t", (classifiers[i][j][k]< 1 && -1< classifiers[i][j][k]?  classifiers[i][j][k]: 0.5));
            }
            fprintf(fp, "\n");
            for(k = 0; k < a; k++){
                fprintf(fp, "%d\t", (k == activities[i]? 1: -1));
            }
            fprintf(fp, "\n");
        }
    }
    printf("%s, \n", train_file);
    fclose(fp);
}


void training_file_two_inputs(float ***classifiers_server, float ***classifiers_client, const char * file_name,
                              int *row_counts, const int *activities, int input_server, int input_client, int s, int n, int a)
{
    printf("training with two inputs: %d\n", a);
    char * direct = "";
    int total, i, j, k;
    char * train_file;
    train_file = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(train_file, 0, BUFF_SIZE);
    sprintf(train_file, "%s%s", direct, file_name);
    total = 0;
    for(i = s; i< n; i++)
        total+= row_counts[i];
    FILE *fp;
    printf("total: %d\n", total);

    fp = fopen(train_file, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to write to file \'%s\'.\n",
                train_file
        );
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d\t%d\t%d\n", total, input_server + input_client, a);
    for(i = s; i < n; i++) {
        for (j = 0; j < row_counts[i]; j++) {
//            printf("%d\t %d\t %d\n", n, dimen[i], counts[i][j]);
            for (k = 0; k < input_server; k++) {
                fprintf(fp, "%f\t", (classifiers_server[i][j][k]< 1 && -1< classifiers_server[i][j][k]?  classifiers_server[i][j][k]: 0.5));
            }
            for (k = 0; k < input_client; k++) {
                fprintf(fp, "%f\t", (classifiers_client[i][j][k]< 1 && -1< classifiers_client[i][j][k]?  classifiers_client[i][j][k]: 0.5));
            }
            fprintf(fp, "\n");
            for(k = 0; k < a; k++){
                fprintf(fp, "%d\t", (k == activities[i]? 1: -1));
            }
            fprintf(fp, "\n");
        }
    }
    printf("%s, \n", train_file);
    fclose(fp);
}

