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

/*
int main()
{
	char** filenames = malloc(sizeof(char*)*1024);
	char* cmd = "rm Server_Testing_Data_Set_JUN/abc.csv";
	int ret = checkTestDataMatch("JUN",filenames);
	printf("%d\n",ret);
	FILE* fp = fopen("Server_Testing_Data_Set_JUN/abc.csv","r");
	if(fp == NULL)
		printf("fp == NULL\n");
	ret = system(cmd);
	printf("%d\n",ret);
}
*/

 
static volatile int run_flag = 1;

void sig_handler(int sig)
{
	if(sig == SIGINT)
		run_flag = 0;
	else if(sig == SIGSEGV)
	{
		printf("SIGSEGV received!\n");
		exit(1);
	}
}



int main(int argc, char ** argv)
//int main2(int argc, char *argv)
{
	int i, j, rows;
	char *username = argv[1];
//    char *username = "AA";
	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
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
	//printf("malloc: %d inner: %d\n", malloc_usable_size(classifiers), malloc_usable_size(classifiers[26]));

	//char *username = argv[1];
	char *cmd = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *n_dir = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *dir = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *lineptr = (char*)malloc(sizeof(char)*BUFF_SIZE);
	size_t len = 0;	
	FILE *fp;
	int test;
	memset(n_dir, 0, BUFF_SIZE);
	memset(cmd, 0, BUFF_SIZE);
	sprintf(n_dir, "%s_%s/", train_file, username);
	test = chdir(n_dir);

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

	sprintf(dir,"%s_trash",username);
	mkdir(dir, 0777);
	while(run_flag)
	{
		sprintf(cmd,"ls test_%s*.csv > filename_list.txt 2>/dev/null" ,username);
//		sprintf(cmd,"ls ../Train_Data_Set_%s/test_%s*.csv > filename_list.txt", username, username);

		system(cmd);
		printf("Generated filename_list.txt\n");
//		sleep(1);
	
		fp = fopen("filename_list.txt", "r");
		if(fp==NULL)
			fprintf(stderr,"ERROR: Failed opening\n");
		else{
			//printf("Successfully opened.\n");
			fprintf(stdout, "\tReading file contents:\n"); 
			while(getline(&lineptr,&len,fp) != -1 && run_flag)
			{
				fprintf(stdout, "%s",lineptr);
				lineptr[strlen(lineptr)-1] = '\0';
//				printf("before test_classifiers\n");
//				printf("test_clasiifiers call\n");
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

				strides(lineptr, data, sigma, mean,
						&data_length, strides_t2, strides_t1, &count_t2);

                rows = data_length;
				features(data, mean, sigma, strides_t2, count_t2,
						 classifiers, turning, speeds, turn_speeds, &rows);
//				test_classifiers(classifiers, turning, lineptr, &rows);
				//printf("after test_classifiers\n");
				//tes_neural_network(classifiers, turning, rows, "", username);
				for(i = 0; i < rows; i++)
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
//				printf("after neural network\n");
				// pass classifiers to neural networks both turning and all others
				//printf("dir: %s, lineptr: %s\n",dir,lineptr);

				sprintf(cmd,"mv %s %s/%s",lineptr,dir,lineptr);
				system(cmd);
				memset(cmd, 0, BUFF_SIZE);
				printf("\n");

				for(i = 0; i < STRIDES; i++)
				{
					//free(classifiers[i]);
					//free(turning[i]);
				//	memset(classifiers, 0 , FEATURES);
				//	memset(turning, 0 , TURN_FEATURES);
				}
				
				//free(classifiers);
				//free(turning);
				

			}
			sleep(20);
			}
		fclose(fp);	
	}		
	exit(0);
}
