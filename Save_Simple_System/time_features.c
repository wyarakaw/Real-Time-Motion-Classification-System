#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"




void time_features(float **data, float **mean, float **sigma, int *strides, int stride_count,
              float **classifiers, float **turning, float **speeds, float **turn_speeds, int *rows)
{
//    printf("features\n");
    int data_size = (*rows);
    float k_top, k_ratio;
    k_top = 10;
    float weight_cond = 20;
    k_ratio = 1;
    float max = 0;
    int i, j, k, l, cnt;

    float class_slots_xa[STRIDES][CLASS_SLOTS];
    float class_slots_ya[STRIDES][CLASS_SLOTS];
    float class_slots_za[STRIDES][CLASS_SLOTS];
    float class_slots_xg[STRIDES][CLASS_SLOTS];
    float class_slots_yg[STRIDES][CLASS_SLOTS];
    float class_slots_zg[STRIDES][CLASS_SLOTS];

    int stride_all[STRIDES][4];
    int num_strides;
    num_strides = 0;

//    printf("after malloc\n");
    cnt = 0;
//    float ave_p = 0;
    for(j = 1; j < stride_count; j++)
    {
        if(data[0][strides[j]] - data[0][strides[j-1]] < 4)
        {
            stride_all[cnt][0] = strides[j-1];
            stride_all[cnt][1] = strides[j];
            stride_all[cnt][2] = strides[j]-strides[j-1];
            if(stride_all[cnt][2] > 1000)
                stride_all[cnt][2] = 1000;
//            ave_p +=  stride_all[cnt][2];
//            printf("%d\t", stride_all[cnt][2]);
            num_strides++;
            cnt++;
        }
//        printf("\n");

    }

    for(j = 0; j < num_strides-1; j++)
    {
        if(stride_all[j][1] == stride_all[j+1][0])
        {
            stride_all[j][3] = 1;
        }
        else
        {
            stride_all[j][3] = -1;
        }
    }


    for(i = 4; i < 7; i++) {
        for (j = 0; j < data_size; j++) {
            data[i][j] /= 256;
        }
    }

    class_slots(data[0], data[1], stride_all, class_slots_xa, num_strides);
    class_slots(data[0], data[2], stride_all, class_slots_ya, num_strides);
    class_slots(data[0], data[3], stride_all, class_slots_za, num_strides);
    class_slots(data[0], data[4], stride_all, class_slots_xg, num_strides);
    class_slots(data[0], data[5], stride_all, class_slots_yg, num_strides);
    class_slots(data[0], data[6], stride_all, class_slots_zg, num_strides);

//    printf("total maximum average dev: %f\n", max);


    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;

        for (k = 0; k < CLASS_SLOTS; k++) {
            classifiers[j][l++] = class_slots_xa[j][k] / 20;       // peak1 and peak with relative loc
            classifiers[j][l++] = class_slots_ya[j][k] / 20;       // trough1 and trough2 with relative location
            classifiers[j][l++] = class_slots_zg[j][k] / 20;       // 27 through 40

        }
    }
    if(PRINT_FEATURES)
        printf("number of features: %d\n", l);
    max = 0;
    float min = 0;
    int max_l, min_l;
    max_l = min_l = 0;

    for(i = 0; i < num_strides; i++)
    {
        for(j = 0; j < l; j++)
        {
            if(classifiers[i][j] > max)
            {
                max = classifiers[i][j];
                max_l = j;
            }
            if(classifiers[i][j] < min)
            {
                min = classifiers[i][j];
                min_l = j;
            }
        }
    }
//    if(max > 1 || min < -1)
//        printf("activities: max: %f at: %d, min: %f at: %d\n", max, max_l, min, min_l);

    k = 0;
    for(i = PRINT; i < num_strides && k < 10; i+=3)
    {
        k++;
        for(j = 0; j < l; j++)
            printf("j: %d\t%f\t", j, classifiers[i][j]);
        printf("\n");

    }

    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
        for(k = 0; k < CLASS_SLOTS; k++)
        {
            speeds[j][l++] = class_slots_xa[j][k] /40;     // max values at each partial
            speeds[j][l++] = class_slots_ya[j][k] /40;
            speeds[j][l++] = class_slots_zg[j][k] /40;     // 40 through 4 * PARTIALS
        }
    }

    if(PRINT_FEATURES)
        printf("number of speed features: %d\n", l);



    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
        for(k = 0; k < CLASS_SLOTS; k++){
            turning[j][l++] = class_slots_ya[j][k]/20;       // peak1 and peak with relative loc
            turning[j][l++] = class_slots_za[j][k]/20;       // trough1 and trough2 with relative location
            turning[j][l++] = class_slots_xg[j][k]/20;       // 27 through 40
            turning[j][l++] = class_slots_yg[j][k]/20;

        }
    }

    max = min = 0;
    max_l = min_l = 0;

    for(i = 0; i < num_strides; i++)
    {
        for(j = 0; j < l; j++)
        {
            if(turning[i][j] > max)
            {
                max = turning[i][j];
                max_l = j;
            }
            if(turning[i][j] < min)
            {
                min = turning[i][j];
                min_l = j;
            }
        }
    }
//    if(max > 1 || min < -1)
//        printf("turning: max: %f at: %d, min: %f at: %d\n", max, max_l, min, min_l);
    printf("number of turning features: %d\n", l);

    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
        for(k = 0; k < CLASS_SLOTS; k++){
            turn_speeds[j][l++] = class_slots_ya[j][k]/20;       // peak1 and peak with relative loc
            turn_speeds[j][l++] = class_slots_za[j][k]/20;       // trough1 and trough2 with relative location
            turn_speeds[j][l++] = class_slots_xg[j][k]/20;       // 27 through 40
            turn_speeds[j][l++] = class_slots_yg[j][k]/20;

        }
    }
    printf("number of turn speed features: %d\n", l);

//    printf("strides: %d,\n", num_strides);
    printf("strides: %d\n", num_strides);
    *rows = num_strides;

}

