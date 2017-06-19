#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


void match_strides(char * input_file, float **data, float **sigma, float **mean,
                   float *source_times, int *data_length, int *strides_source, int *strides_dest, int *rows){
    // receive arrays with data pre allocated, return strides
    int i, j, k, l;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    char *record;
    int data_size;
    int count_t2;
    int count_t1;
    int last_period;
    int source_count = (*rows);
    int n = (*data_length);



    fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to read from file \'%s\'.\n",
                input_file
        );
        exit(EXIT_FAILURE);
    }
    getline(&line, &len, fp); //discard header of file
    j = 0;
    double time_ref;
    time_ref = 0;
    if ((getline(&line, &len, fp)) != -1) //get first line to zero time
    {
        record = strtok(line, ",");
        time_ref = atof(record);
        k = 0;
        data[k++][j] = 0;
//            printf("time: %f, ref:%f\n", data[i][0][j], time_ref);
//            printf("real value ref: %20.10f\n", strtod(record, NULL));
        record = strtok(NULL, ",");

        while (record != NULL) {
//                if (k == 1)
//                    record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
//                printf("%lf\n", data[i][k-1][j]);
            record = strtok(NULL, ",");

        }
        j++;
    }

    while ((getline(&line, &len, fp)) != -1) {

        record = strtok(line, ",");
        k = 0;
        data[k++][j] = (float) (atof(record) - time_ref);
//            printf("time: %2.10lf, value: %2.10lf\t", data[i][0][j], (float) (atof(record) - time_ref));
//            printf("%lf\n", time_ref);
        record = strtok(NULL, ",");

        while (record != NULL) {
//                if (k == 1)
//                    record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
            record = strtok(NULL, ",");

        }
        j++;

    }
    *data_length = j;
    data_size = j;
    fclose(fp);


    //    printf("mean and sigma\n");
    for (i = 0; i < FILES; i++) {
//        printf("i: %d\n", i);
        for (j = 0; j < DATA - 1; j++) {

            sample_mean(data[j + 1], mean[j], 3000, data_size);
//            printf("mean done: %d\n", j);
            sample_sigma(data[j + 1], mean[j], sigma[j], 3000, data_size);
//            printf("sigma done: %d\n", j);
            center_data(data[j + 1], mean[j], sigma[j], 3000, data_size);
//            printf("center done: %d\n", j);
        }
    }


    float zg_data[MAX_DATA];
    memset(zg_data, 0, sizeof(float) * MAX_DATA);
    float temp_data[MAX_DATA];
    memset(zg_data, 0, sizeof(float) * MAX_DATA);

    int zero_count;

    for(i = 1; i < 7; i++)
    {
        filter_data(data[i], temp_data, filter, 15, data_size);
        filter_data(data[i], temp_data, filter, 15, data_size);
    }
    count_t1 = 0;

//    void stride_match(float *time_source, float *time_dest, int * strides_source, int *strides_dest,
//                      int *stride_count, int n_s, int n_d)

//    for(i = 0; i < data_size; i+=100)
//        printf("%f\t", data[0][i]);
//    printf("\n");

    k = l = 0;
//    printf("%d\t", source_count);
    for(j = 0; j < source_count; j++)
    {
        if(k >= n)
            break;

        while(k < n && data[0][k++] < source_times[strides_source[j]]);
        strides_dest[j] = k-1;
        l++;

    }

//    stride_match(source_times, data[0], strides_source, strides_dest, &count_t2, source_count, data_size);


    for (i = 1; i < 1; i++) {
//        printf("file: %d\t%d\n", i, count_t1);
        printf("%d\t[", l);
        for (j = 0; j < l-1; j++) {
            printf("%d, ", strides_source[j]);
        }
        printf("%d", strides_source[l-1]);
        printf("],\n");
    }


    *rows = l;
//    printf("done strides\n");
}