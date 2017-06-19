#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


void test_strides(char * input_file, float **data, float **sigma, float **mean,
                  int *strides_t2, int *count_t2, int *strides_t1)
{
    // receive arrays with data pre allocated, return strides
    printf("test_strides\n");
    int rv;
    int data_lengths;
    int i, j, k;
    int c1, c2;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *record;
    line = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(line, 0, sizeof(char) * BUFF_SIZE);
    line = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(line, 0, sizeof(char) * BUFF_SIZE);

    k = 0;

    fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to read from file \'%s\'.\n",
                input_file
        );
        exit(EXIT_FAILURE);
    }
    read = getline(&line, &len, fp); //discard header of file
    j = 0;
    double time_ref;
    time_ref = 0;
    printf("file open\n");
    printf("%s\n", input_file);
    if ((read = getline(&line, &len, fp)) != -1) //get first line to zero time
    {
        printf("%s\n", line);
        record = strtok(line, ",");
        printf("next\n");
        time_ref = atof(record);
        printf("next\n");
        k = 0;
        printf("here\n");
        data[k][j] = 0;
        printf("next\n");
//            printf("time: %f, ref:%f\n", data[i][0][j], time_ref);
//            printf("real value ref: %20.10f\n", strtod(record, NULL));
        printf("next\n");
        record = strtok(NULL, ",");

        while (record != NULL) {
            printf("reading\n");
                if (k == 1)
                    record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
//                printf("%lf\n", data[i][k-1][j]);
            record = strtok(NULL, ",");

        }
        j++;
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("reading\n");

        record = strtok(line, ",");
        k = 0;
        data[k++][j] = (float) (atof(record) - time_ref);
//            printf("time: %2.10lf, value: %2.10lf\t", data[i][0][j], (float) (atof(record) - time_ref));
//            printf("%lf\n", time_ref);
        record = strtok(NULL, ",");

        while (record != NULL) {
                if (k == 1)
                    record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
            record = strtok(NULL, ",");

        }
        j++;

    }
    data_lengths = j;
    fclose(fp);
    printf("file read\n");

    printf("mean and sigma\n");

//        printf("i: %d\n", i);
    for (j = 0; j < DATA - 1; j++) {

        sample_mean(data[j + 1], mean[j], 3000, data_lengths);
//            printf("mean done: %d\n", j);
        sample_sigma(data[j + 1], mean[j], sigma[j], 3000, data_lengths);
//            printf("sigma done: %d\n", j);
        center_data(data[j + 1], mean[j], sigma[j], 3000, data_lengths);
//            printf("center done: %d\n", j);
    }


    c1 = temp_strides(data, strides_t1, sigma[1], sigma[5], 4, 500, 3, 0.2, data_lengths);

    c2 = select_strides(data[0], strides_t1, strides_t2, 0.4, c1);

    for (j = 0; j < STRIDES; j++)
        strides_t1[j] = 0;

    c1 = peak_strides(data[0], data[6], strides_t2, strides_t1, sigma[5], 1.4, c2, data_lengths);

    c2 = select_strides(data[0], strides_t1, strides_t2, 0.01, c1);

    c1 = outlier(data[0], strides_t2, strides_t1, .65, c2);

    c2 = outlier(data[0], strides_t1, strides_t2, 0.65, c1);

    c2 = select_strides(data[0], strides_t1, strides_t2, 0.3, c1);

    c1 = select_strides(data[0], strides_t2, strides_t1, 0.3, c2);

    move_back(strides_t2[i], 100, count_t2[i]);
    *count_t2 = c2;

}

