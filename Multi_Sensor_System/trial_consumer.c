#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <sys/stat.h>
#include "constants.h"
#include "classifiers.h"
#include "neural_network.h"


static volatile int run_flag = 1;

void sig_handler_2(int sig)
{
    if(sig == SIGINT)
        run_flag = 0;
    else if(sig == SIGSEGV)
    {
        printf("SIGSEGV received!\n");
        exit(1);
    }
}

//int main(int argc, char ** argv)
int main3(int argc, char *argv)
{

    int i, j, k, rows;
//	char *username = argv[1];
    char *username = "AA";
    FILE *fp;
    signal(SIGINT, sig_handler_2);
    signal(SIGSEGV, sig_handler_2);
    if(argc != 2)
    {
        printf("Error: incorrect number of arguments\n");
        exit(1);
    }
    float ** classifiers;
    classifiers = (float**)malloc(sizeof(float*)*STRIDES);
    float ** turning;
    turning = (float**)malloc(sizeof(float*)*STRIDES);
    float ** speeds;
    speeds = (float**)malloc(sizeof(float*)*STRIDES);
    float ** turn_speeds;
    turn_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    //printf("Strides: %d, Features: %d\n", STRIDES, FEATURES);
    for(i=0;i<STRIDES;i++)
    {

        classifiers[i] = (float*)malloc(sizeof(float)*FEATURES);
        turning[i] = (float*)malloc(sizeof(float)*TURN_FEATURES);
        speeds[i] = (float*)malloc(sizeof(float)*FEATURES);
        turn_speeds[i] = (float*)malloc(sizeof(float)*FEATURES);
        //	printf("malloc has: %d, need: %d\n" ,malloc_usable_size(classifiers[i]), FEATURES * 4);
    }


    float ** time_classifiers;
    time_classifiers = (float**)malloc(sizeof(float*)*STRIDES);
    float ** time_turning;
    time_turning = (float**)malloc(sizeof(float*)*STRIDES);
    float ** time_speeds;
    time_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    float ** time_turn_speeds;
    time_turn_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    //printf("Strides: %d, Features: %d\n", STRIDES, FEATURES);
    for(i=0;i<STRIDES;i++)
    {

        time_classifiers[i] = (float*)malloc(sizeof(float)*FEATURES);
        time_turning[i] = (float*)malloc(sizeof(float)*TURN_FEATURES);
        time_speeds[i] = (float*)malloc(sizeof(float)*FEATURES);
        time_turn_speeds[i] = (float*)malloc(sizeof(float)*FEATURES);
        //	printf("malloc has: %d, need: %d\n" ,malloc_usable_size(classifiers[i]), FEATURES * 4);
    }


    char *n_dir;
    n_dir = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(n_dir, 0, BUFF_SIZE);
    sprintf(n_dir, "%s_%s/", train_file, username);
    int test;
    test = chdir(n_dir);
    char **dot_csv_name;
    dot_csv_name = (char **) malloc(sizeof(char*) * (FILES));
    for (i = 0; i < FILES; i++) {
        dot_csv_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_csv_name[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(dot_csv_name[i], "%s_%s.csv", names[i], username);
        sprintf(dot_csv_name[i], "%s%s_%s/%s_%s.csv", main_path, train_file, username, names[i], username);
    }

    float **data;
    float **sigma;
    float **mean;
    int * strides_t1, count_t1;
    int * strides_t2, count_t2;
    int data_length;




    strides_t1 = (int*) malloc(sizeof(int) * STRIDES);
    strides_t2 = (int*) malloc(sizeof(int) * STRIDES);
    data = (float **) malloc(sizeof(float*) * DATA);
    mean = (float **) malloc(sizeof(float*) * DATA);
    sigma = (float **) malloc(sizeof(float*) * DATA);

    for(j = 0; j < DATA; j++)
    {
        data[j] = (float *) malloc(sizeof(float) * MAX_DATA);
        mean[j] = (float *) malloc(sizeof(float) * MAX_DATA);
        sigma[j] = (float *) malloc(sizeof(float) * MAX_DATA);
    }

    for(k = 0; k < FILES; k++){
        count_t2 = 0;
        data_length = 0;
        memset(strides_t2, 0, sizeof(int) * STRIDES);
        memset(strides_t1, 0, sizeof(int) * STRIDES);

        for(j = 0; j < DATA; j++)
        {
            memset(data[j], 0, sizeof(float) * MAX_DATA);
            memset(sigma[j], 0, sizeof(float) * MAX_DATA);
            memset(mean[j], 0, sizeof(float) * MAX_DATA);
        }
        printf("%s\n", dot_csv_name[k]);
        strides(dot_csv_name[k], data, sigma, mean,
                &data_length, strides_t2, strides_t1, &count_t2);

        rows = data_length;
        features(data, mean, sigma, strides_t2, count_t2,
                 classifiers, turning, speeds, turn_speeds, &rows);

        time_features(data, mean, sigma, strides_t2, count_t2,
                 time_classifiers, time_turning, time_speeds, time_turn_speeds, &rows);
        for(i = s_tr[k]; i < rows; i++)
        {
            for(j = 0; j < FEATURES; j++)
            {
                if(classifiers[i][j] > 1){
                    classifiers[i][j] = 1;
                }else if(classifiers[i][j] < -1){
                    classifiers[i][j] = 1;
                }


                //printf("%f\t", classifiers[i][j]);
            }
            //printf("\nturning: ");
            for(j = 0; j < TURN_FEATURES; j++)
            {
                if(turning[i][j] > 1){
                    turning[i][j] = 1;
                }else if(turning[i][j] < -1){
                    turning[i][j] = 1;
                }

//						printf("%f\t", turning[i][j]);
            }
//					printf("\n");
        }
        test_neural_network(classifiers, turning, speeds, turn_speeds, rows, "./", username );
        printf("TIME VALUES\n");
//        time_test_neural_network(time_classifiers, time_turning, time_speeds, time_turn_speeds, rows, "./", username );
        usleep(10000);

    }
    return 0;

}
