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



//int main(int argc, char ** argv)
int main2(int argc, char *argv)
{
	int i, j, k, l, server_rows, client_rows;
//	char *username = argv[1];
    char *username = "AA";
	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
	if(argc != 2)
	{
		printf("Error: incorrect number of arguments\n");
		exit(1);
	}
    float ** client_classifiers;
    client_classifiers = (float**)malloc(sizeof(float*)*STRIDES);
    float ** client_turning;
    client_turning = (float**)malloc(sizeof(float*)*STRIDES);
    float ** client_speeds;
    client_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    float ** client_turn_speeds;
    client_turn_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    //printf("Strides: %d, Features: %d\n", STRIDES, FEATURES);
    for(i=0;i<STRIDES;i++)
    {

        client_classifiers[i] = (float*)malloc(sizeof(float)*2*FEATURES);
        client_turning[i] = (float*)malloc(sizeof(float)*2*TURN_FEATURES);
        client_speeds[i] = (float*)malloc(sizeof(float)*2*SPEED_FEATURES);
        client_turn_speeds[i] = (float*)malloc(sizeof(float)*2*TURN_SPEED);
        //	printf("malloc has: %d, need: %d\n" ,malloc_usable_size(client_classifiers[i]), FEATURES * 4);
    }

    float ** server_classifiers;
    server_classifiers = (float**)malloc(sizeof(float*)*STRIDES);
    float ** server_turning;
    server_turning = (float**)malloc(sizeof(float*)*STRIDES);
    float ** server_speeds;
    server_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    float ** server_turn_speeds;
    server_turn_speeds = (float**)malloc(sizeof(float*)*STRIDES);
    //printf("Strides: %d, Features: %d\n", STRIDES, FEATURES);
    for(i=0;i<STRIDES;i++)
    {

        server_classifiers[i] = (float*)malloc(sizeof(float)*FEATURES);
        server_turning[i] = (float*)malloc(sizeof(float)*TURN_FEATURES);
        server_speeds[i] = (float*)malloc(sizeof(float)*SPEED_FEATURES);
        server_turn_speeds[i] = (float*)malloc(sizeof(float)*TURN_SPEED);
        //	printf("malloc has: %d, need: %d\n" ,malloc_usable_size(client_classifiers[i]), FEATURES * 4);
    }
	//printf("malloc: %d inner: %d\n", malloc_usable_size(client_classifiers), malloc_usable_size(client_classifiers[26]));

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
//	sprintf(n_dir, "%s_%s/", train_file, username);
//	test = chdir(n_dir);

	float **client_data;
	float **client_sigma;
	float **client_mean;
	int * server_strides, count_client;
	int * client_strides, count_server;
	int client_data_length;




	server_strides = (int*) malloc(sizeof(int) * STRIDES);
	client_strides = (int*) malloc(sizeof(int) * STRIDES);
    client_data  = (float **) malloc(sizeof(float*) * DATA);
    client_mean  = (float **) malloc(sizeof(float*) * DATA);
    client_sigma = (float **) malloc(sizeof(float*) * DATA);

	for(j = 0; j < DATA; j++)
	{
        client_data[j] = (float *) malloc(sizeof(float) * MAX_DATA);
        client_mean[j] = (float *) malloc(sizeof(float) * MAX_DATA);
        client_sigma[j] = (float *) malloc(sizeof(float) * MAX_DATA);
	}


    float **server_data;
    float **server_sigma;
    float **server_mean;

    int server_data_length;


    server_data = (float **) malloc(sizeof(float*) * DATA);
    server_mean = (float **) malloc(sizeof(float*) * DATA);
    server_sigma = (float **) malloc(sizeof(float*) * DATA);

    for(j = 0; j < DATA; j++)
    {
        server_data[j] = (float *) malloc(sizeof(float) * MAX_DATA);
        server_mean[j] = (float *) malloc(sizeof(float) * MAX_DATA);
        server_sigma[j] = (float *) malloc(sizeof(float) * MAX_DATA);
    }

    char** filename_list = (char**) malloc(sizeof(char*)*BUFF_SIZE);
    int n_filename_list;
    char * server_name = (char*) malloc(sizeof(char)*BUFF_SIZE);
    char * client_name = (char*) malloc(sizeof(char)*BUFF_SIZE);


	sprintf(dir,"%s_trash",username);
	mkdir(dir, 0777);
	while(run_flag)
	{
        n_filename_list = checkTestDataMatch(username, filename_list);

        for(l=0; i<n_filename_list; l++)
        {
            count_server = 0;
            count_client = 0;
            client_data_length = 0;
            memset(client_strides, 0, sizeof(int) * STRIDES);
            memset(server_strides, 0, sizeof(int) * STRIDES);


            memset(server_name, 0, STRIDES);
            memset(client_name, 0, STRIDES);

            sprintf(server_name, "Server_Testing_Data_Set_%s/%s", username, filename_list[l]);
            sprintf(client_name, "Client_Testing_Data_Set_%s/%s", username, filename_list[l]);

            for(j = 0; j < DATA; j++)
            {
                memset(server_data[j], 0, sizeof(float) * MAX_DATA);
                memset(server_sigma[j], 0, sizeof(float) * MAX_DATA);
                memset(server_mean[j], 0, sizeof(float) * MAX_DATA);
            }

            for(j = 0; j < DATA; j++)
            {
                memset(client_data[j], 0, sizeof(float) * MAX_DATA);
                memset(client_sigma[j], 0, sizeof(float) * MAX_DATA);
                memset(client_mean[j], 0, sizeof(float) * MAX_DATA);
            }

            strides(server_name, server_data, server_sigma, server_mean,
                    &server_data_length, client_strides, server_strides, &count_server);

            server_rows = server_data_length;
            count_client = count_server;

            client_data_length = server_data_length;

            match_strides(client_name, client_data, client_sigma, client_mean, server_data[0],
                          &client_data_length, server_strides, client_strides, &count_client);

            client_rows = client_data_length;

            features(server_data, server_mean, server_sigma, server_strides, count_server,
                     server_classifiers, server_turning, server_speeds, server_turn_speeds, &server_rows);

            features(client_data, client_mean, client_sigma, client_strides, count_client,
                     &client_classifiers[FEATURES], &client_turning[TURN_FEATURES],
                     &client_speeds[SPEED_FEATURES], &client_turn_speeds[TURN_SPEED], &client_rows);

            for(i = 0; i < server_rows; i++)
            {
                for(j = 0; j < FEATURES; j++)
                {
                    if(server_classifiers[i][j] > 1){
                        server_classifiers[i][j] = 1;
                    }else if(server_classifiers[i][j] < -1){
                        server_classifiers[i][j] = 1;
                    }
                }

                for(j = 0; j < TURN_FEATURES; j++)
                {
                    if(server_turning[i][j] > 1){
                        server_turning[i][j] = 1;
                    }else if(server_turning[i][j] < -1){
                        server_turning[i][j] = 1;
                    }
                }
            }
            for(i = 0; i < server_rows; i++)
            {
                for(j = 0; j < SPEED_FEATURES; j++)
                {
                    if(server_speeds[i][j] > 1){
                        server_speeds[i][j] = 1;
                    }else if(server_speeds[i][j] < -1){
                        server_speeds[i][j] = 1;
                    }
                }

                for(j = 0; j < TURN_SPEED; j++)
                {
                    if(server_turn_speeds[i][j] > 1){
                        server_turn_speeds[i][j] = 1;
                    }else if(server_turn_speeds[i][j] < -1){
                        server_turn_speeds[i][j] = 1;
                    }
                }
            }

            for(i = 0; i < server_rows; i++)
            {
                for(j = 0; j < FEATURES; j++)
                {
                    if(client_classifiers[i][j] > 1){
                        client_classifiers[i][j] = 1;
                    }else if(client_classifiers[i][j] < -1){
                        client_classifiers[i][j] = 1;
                    }
                }

                for(j = 0; j < TURN_FEATURES; j++)
                {
                    if(client_turning[i][j] > 1){
                        client_turning[i][j] = 1;
                    }else if(client_turning[i][j] < -1){
                        client_turning[i][j] = 1;
                    }
                }
                for(i = 0; i < server_rows; i++)
                {
                    for(j = 0; j < SPEED_FEATURES; j++)
                    {
                        if(client_speeds[i][j] > 1){
                            client_speeds[i][j] = 1;
                        }else if(client_speeds[i][j] < -1){
                            client_speeds[i][j] = 1;
                        }
                    }

                    for(j = 0; j < TURN_SPEED; j++)
                    {
                        if(client_turn_speeds[i][j] > 1){
                            client_turn_speeds[i][j] = 1;
                        }else if(client_turn_speeds[i][j] < -1){
                            client_turn_speeds[i][j] = 1;
                        }
                    }
                }
                for(j = 0; j < FEATURES; j++)
                {
                    client_classifiers[i][j] = server_classifiers[i][j];
                }
                for(j = 0; j < TURN_FEATURES; j++)
                {
                    client_turning[i][j] = server_turning[i][j];
                }
                for(j = 0; j < SPEED_FEATURES; j++)
                {
                    client_speeds[i][j] = server_speeds[i][j];
                }
                for(j = 0; j < TURN_SPEED; j++)
                {
                    client_turn_speeds[i][j] = server_turn_speeds[i][j];
                }

            }



            test_neural_network(client_classifiers, client_turning, client_speeds, client_turn_speeds,
                                server_rows, "./", username );
            comm_test_neural_network(client_classifiers, client_turning, client_speeds, client_turn_speeds,
                                server_rows, "./", username);
//				printf("after neural network\n");
            // pass client_classifiers to neural networks both client_turning and all others
            //printf("dir: %s, lineptr: %s\n",dir,lineptr);

            memset(cmd, 0, BUFF_SIZE);
            sprintf(cmd,"mv Server_Testing_Data_Set_%s/%s %s_trash/",username,filename_list[l],username);
            system(cmd);
            sprintf(cmd,"mv Client_Testing_Data_Set_%s/%s %s_trash/",username,filename_list[l],username);
            system(cmd);


            //sprintf(cmd,"mv %s %s/%s",lineptr,dir,lineptr);
            //system(cmd);

            printf("\n");


        }
        sleep(10);
	}		
	exit(0);
}

