#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "constants.h"
#include <sys/stat.h>
#include "classifiers.h"



int main(int argc, char **argv) {

    char *name;
    name = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(name, 0, sizeof(char) * BUFF_SIZE);
    sprintf(name, "AA");
//    sprintf(name, argv[1]);
    int i, j, k;
    printf("start\n");

    char * dot_net_dir = malloc(sizeof(char)*BUFF_SIZE);
    memset(dot_net_dir, 0, BUFF_SIZE);
    sprintf(dot_net_dir, "%s%s_%s", main_path, train_file, name);
    mkdir(dot_net_dir, 0777);


    float ***classifiers;
    float ***speeds;
    float ***turning_all;
    float ***turn_speeds;
    classifiers = (float ***) malloc(sizeof ( float **) * FILES);
    speeds = (float ***) malloc(sizeof ( float **) * FILES);
    turning_all = (float ***) malloc(sizeof ( float **) * FILES);
    turn_speeds = (float ***) malloc(sizeof ( float **) * FILES);

    for(i = 0; i < FILES; i++)
    {
        classifiers[i] = (float **) malloc(sizeof ( float *) * STRIDES);
        speeds[i] = (float **) malloc(sizeof ( float *) * STRIDES);
        turning_all[i] = (float **) malloc(sizeof ( float *) * STRIDES);
        turn_speeds[i] = (float **) malloc(sizeof ( float *) * STRIDES);

    }
    for(i = 0; i < FILES; i++)
    {
        for(j = 0; j < STRIDES; j++)
        {
            classifiers[i][j] = (float *) malloc(sizeof ( float) * FEATURES);
            memset(classifiers[i][j], 0, sizeof(float) * FEATURES);
            speeds[i][j] = (float *) malloc(sizeof (float) * SPEED_FEATURES);
            memset(speeds[i][j], 0, sizeof(float) * SPEED_FEATURES);
            turn_speeds[i][j] = (float *) malloc(sizeof (float) * TURN_SPEED);
            memset(turn_speeds[i][j], 0, sizeof(float) * TURN_SPEED);
            turning_all[i][j] = (float *) malloc(sizeof (float) * TURN_FEATURES);
            memset(turning_all[i][j], 0, sizeof(float) * TURN_FEATURES);
        }
    }

    float ***time_classifiers;
    float ***time_speeds;
    float ***time_turning_all;
    float ***time_turn_speeds;
    time_classifiers = (float ***) malloc(sizeof ( float **) * FILES);
    time_speeds = (float ***) malloc(sizeof ( float **) * FILES);
    time_turning_all = (float ***) malloc(sizeof ( float **) * FILES);
    time_turn_speeds = (float ***) malloc(sizeof ( float **) * FILES);

    for(i = 0; i < FILES; i++)
    {
        time_classifiers[i] = (float **) malloc(sizeof ( float *) * STRIDES);
        time_speeds[i] = (float **) malloc(sizeof ( float *) * STRIDES);
        time_turning_all[i] = (float **) malloc(sizeof ( float *) * STRIDES);
        time_turn_speeds[i] = (float **) malloc(sizeof ( float *) * STRIDES);

    }
//    printf("62\n");
    for(i = 0; i < FILES; i++)
    {
        for(j = 0; j < STRIDES; j++)
        {
            time_classifiers[i][j] = (float *) malloc(sizeof ( float) * CLIENT_FEATURES);
            memset(time_classifiers[i][j], 0, sizeof(float) * CLIENT_FEATURES);
            time_speeds[i][j] = (float *) malloc(sizeof (float) * CLIENT_SPEED_FEATURES);
            memset(time_speeds[i][j], 0, sizeof(float) * CLIENT_SPEED_FEATURES);
            time_turn_speeds[i][j] = (float *) malloc(sizeof (float) * CLIENT_TURN_SPEED);
            memset(time_turn_speeds[i][j], 0, sizeof(float) * CLIENT_TURN_SPEED);
            time_turning_all[i][j] = (float *) malloc(sizeof (float) * CLIENT_TURN_FEATURES);
            memset(time_turning_all[i][j], 0, sizeof(float) * CLIENT_TURN_FEATURES);
        }
    }

    int *dimen;
    dimen = (int *) malloc(sizeof (int) * FILES);
    memset(dimen, 0, sizeof(int) * FILES);

    int *client_dimen;
    client_dimen = (int *) malloc(sizeof (int) * FILES);
    memset(client_dimen, 0, sizeof(int) * FILES);


    int input;
    input = FEATURES;
    // when training, there will be sum o n rows if dim[i] for i from 0 to n-1 rows
    // dim[i] is the truth value or element 1 in row of -1 -1 -1 1(ith) -1

    // example follows
//    printf("87\n");
    int speeds_count;
//    float **test_array;
    int num_of_test_classifers;
    int truth_value;


//    build names of .csc .txt and .net files for data, training and testing.
    char **dot_text_names;
    char **dot_net_name;
    char **server_dot_csv_names;
    char **dot_text_turn_name;
    char **dot_net_turn_speeds;
    int dot_net_files;

    char *dot_text_turning;
    char *dot_net_turning;


    dot_net_turning = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_net_turning, 0, sizeof(char) * BUFF_SIZE);
    dot_text_turning = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_text_turning, 0, sizeof(char) * BUFF_SIZE);
    sprintf(dot_net_turning, "%s%s_%s/%s_%s.net", main_path, train_file, name, turning_name, name);
    sprintf(dot_text_turning,"%s%s_%s/%s_%s.txt", main_path, train_file, name, turning_name, name);

    dot_text_names = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_net_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_text_turn_name = (char **) malloc(sizeof(char*) * (NUM_TURNS));
    server_dot_csv_names = (char **) malloc(sizeof(char*) * (FILES));
    dot_net_turn_speeds = (char **) malloc(sizeof(char*) * (NUM_TURNS));

    for (i = 0; i < ACTIVITIES + 1; i++) {
        dot_net_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        dot_text_names[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);
        memset(dot_text_names[i], 0, sizeof(char) * BUFF_SIZE);
    }

    for (i = 0; i < FILES; i++) {
        server_dot_csv_names[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(server_dot_csv_names[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(server_dot_csv_names[i], "%s%s_%s/%s_%s.csv",
                main_path, train_file, name, names[i], name);
//        sprintf(server_dot_csv_names[i], "Server_Training_Data_Set_%s/%s_%s.csv",
//                main_path, train_file, name, names[i], name);
    }


    // client names
    char **client_dot_csv_name;
    client_dot_csv_name = (char **) malloc(sizeof(char*) * (FILES));

    for (i = 0; i < FILES; i++) {
        client_dot_csv_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(client_dot_csv_name[i], 0, sizeof(char) * BUFF_SIZE);
//        sprintf(client_dot_csv_name[i], "Client_Training_Data_Set_%s/%s_%s.csv",
//                main_path, train_file, name, names[i], name);
        sprintf(client_dot_csv_name[i], "%s%s_%s/%s_%s.csv",
                main_path, train_file, name, names[i], name);
    }


    printf("if loop\n");

    // path, folder user
    sprintf(dot_net_name[0], "%s%s_%s/%s_%s.net", main_path, train_file, name, activities_file, name);
    sprintf(dot_text_names[0],"%s%s_%s/%s_%s_%s", main_path, train_file, name, activities_file, name, train_name);


    for (i = 0; i < ACTIVITIES; i++) {
        sprintf(dot_net_name[i+1], "%s%s_%s/%s_%s.net", main_path, train_file, name, train_speeds[i], name);
        sprintf(dot_text_names[i+1],"%s%s_%s/%s_%s_%s", main_path, train_file, name, train_speeds[i], name, train_name);
        // now lists of .net and txt names are ready
    }
    for (i = 0; i < NUM_TURNS; i++) {
        dot_text_turn_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_text_turn_name[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(dot_text_turn_name[i], "%s%s_%s/%s_%s.txt", main_path, train_file, name, train_turn_speeds[i], name);

        dot_net_turn_speeds[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_net_turn_speeds[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(dot_net_turn_speeds[i], "%s%s_%s/%s_%s.net", main_path, train_file, name, train_turn_speeds[i], name);
    }


    //    build names of .csc .txt and .net files for data, training and testing for time system
    char **time_dot_text_names;
    char **time_dot_net_name;
    char **time_dot_text_turn_name;
    char **time_dot_net_turn_speeds;
    char *time_dot_text_turning;
    char *time_dot_net_turning;


    time_dot_net_turning = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(time_dot_net_turning, 0, sizeof(char) * BUFF_SIZE);
    time_dot_text_turning = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(time_dot_text_turning, 0, sizeof(char) * BUFF_SIZE);
    sprintf(time_dot_net_turning, "%s%s_%s/Comm_%s_%s.net", main_path, train_file, name, turning_name, name);
    sprintf(time_dot_text_turning,"%s%s_%s/Comm_%s_%s.txt", main_path, train_file, name, turning_name, name);

    time_dot_text_names = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    time_dot_net_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    time_dot_text_turn_name = (char **) malloc(sizeof(char*) * (NUM_TURNS));
    time_dot_net_turn_speeds = (char **) malloc(sizeof(char*) * (NUM_TURNS));

    for (i = 0; i < ACTIVITIES + 1; i++) {
        time_dot_net_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        time_dot_text_names[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(time_dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);
        memset(time_dot_text_names[i], 0, sizeof(char) * BUFF_SIZE);
    }



    // path, folder user
    sprintf(time_dot_net_name[0], "%s%s_%s/Comm_%s_%s.net", main_path, train_file, name, activities_file, name);
    sprintf(time_dot_text_names[0],"%s%s_%s/Comm_%s_%s_%s", main_path, train_file, name, activities_file, name, train_name);


    for (i = 0; i < ACTIVITIES; i++) {
        sprintf(time_dot_net_name[i+1], "%s%s_%s/Comm_%s_%s.net", main_path, train_file, name, train_speeds[i], name);
        sprintf(time_dot_text_names[i+1],"%s%s_%s/Comm_%s_%s_%s", main_path, train_file, name, train_speeds[i], name, train_name);
        // now lists of .net and txt names are ready
    }
    for (i = 0; i < NUM_TURNS; i++) {
        time_dot_text_turn_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(time_dot_text_turn_name[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(time_dot_text_turn_name[i], "%s%s_%s/Comm_%s_%s.txt", main_path, train_file, name, train_turn_speeds[i], name);

        time_dot_net_turn_speeds[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(time_dot_net_turn_speeds[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(time_dot_net_turn_speeds[i], "%s%s_%s/Comm_%s_%s.net", main_path, train_file, name, train_turn_speeds[i], name);
    }


    dot_net_files = 1;
    for (i = 0; i < ACTIVITIES + 1; i++) {
        if (access(dot_net_name[i], F_OK) == -1)
            dot_net_files = -1;
    }
    if(access(dot_net_turning, F_OK) == -1)
        dot_net_files = -1;

    int success = 1;
    if (dot_net_files == -1) {
        printf("Training is required, proceeding to trainging\n");
        // funlion call
//        success = checkCSV(name);
    }

    if(success != 1)
    {
        printf("fail to collect files, will exit\n");
        exit(1);
    }
    float ***data;
    float ***sigma;
    float ***mean;

    float ***client_data;
    float ***client_sigma;
    float ***client_mean;


    int ** strides_t1, *count_t1;
    int ** strides_t2, *count_t2;
    int *data_lengths;
    int *client_data_lengths;

    count_t1 = (int*) malloc(sizeof(int) * FILES);
    count_t2 = (int*) malloc(sizeof(int) * FILES);
    memset(count_t2, 0, sizeof(int) * FILES);
    memset(count_t1, 0, sizeof(int) * FILES);
    strides_t1 = (int**) malloc(sizeof(int*) * FILES);
    strides_t2 = (int**) malloc(sizeof(int*) * FILES);
    for(i = 0; i < FILES; i++)
    {
        strides_t1[i] = (int*) malloc(sizeof(int) * STRIDES);
        strides_t2[i] = (int*) malloc(sizeof(int) * STRIDES);
        memset(strides_t2[i], 0, sizeof(int) * FILES);
        memset(strides_t1[i], 0, sizeof(int) * FILES);
    }
    data_lengths = (int*) malloc(sizeof(int) * FILES);
    client_data_lengths = (int*) malloc(sizeof(int) * FILES);
    data = (float ***) malloc(FILES * sizeof(float **));
    mean = (float ***) malloc(FILES * sizeof(float **));
    sigma = (float ***) malloc(FILES * sizeof(float **));

    client_data = (float ***) malloc(FILES * sizeof(float **));
    client_mean = (float ***) malloc(FILES * sizeof(float **));
    client_sigma = (float ***) malloc(FILES * sizeof(float **));


    for (i = 0; i < FILES; i++) {
        data[i] = (float **) malloc(sizeof(float*) * DATA);
        mean[i] = (float **) malloc(sizeof(float*) * DATA);
        sigma[i] = (float **) malloc(sizeof(float*) * DATA);

        client_data[i] = (float **) malloc(sizeof(float*) * DATA);
        client_mean[i] = (float **) malloc(sizeof(float*) * DATA);
        client_sigma[i] = (float **) malloc(sizeof(float*) * DATA);

    }
    for (i = 0; i < FILES; i++) {
        for(j = 0; j < DATA; j++)
        {
            data[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            mean[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            sigma[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            memset(data[i][j], 0, sizeof(float) * MAX_DATA);
            memset(sigma[i][j], 0, sizeof(float) * MAX_DATA);
            memset(mean[i][j], 0, sizeof(float) * MAX_DATA);

            client_data[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            client_mean[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            client_sigma[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            memset(client_data[i][j], 0, sizeof(float) * MAX_DATA);
            memset(client_sigma[i][j], 0, sizeof(float) * MAX_DATA);
            memset(client_mean[i][j], 0, sizeof(float) * MAX_DATA);

        }

    }

    dot_net_files = -1;
    if (dot_net_files == -1) {
        // also if data returned success
        printf("after training\n");
        // add name to train parameters function
        for(i = 0; i < FILES; i++)
        {
            strides(server_dot_csv_names[i], data[i], sigma[i], mean[i],
                    &data_lengths[i], strides_t2[i], strides_t1[i], &count_t2[i]);

            count_t1[i] = count_t2[i];
            client_data_lengths[i] = data_lengths[i];

            match_strides(client_dot_csv_name[i], client_data[i], client_sigma[i], client_mean[i], data[i][0],
                          &client_data_lengths[i], strides_t2[i], strides_t1[i], &count_t1[i]);

            client_dimen[i] = client_data_lengths[i];
            dimen[i] = data_lengths[i];

            features(data[i], mean[i], sigma[i], strides_t2[i], count_t2[i],
                     classifiers[i], turning_all[i], speeds[i], turn_speeds[i], &dimen[i]);


            features(client_data[i], client_mean[i], client_sigma[i], strides_t1[i], count_t1[i],
                     time_classifiers[i], time_turning_all[i], time_speeds[i], time_turn_speeds[i], &client_dimen[i]);
            printf("%s\n", names[i]);
        }
        for(i = 0; i < FILES; i++)
        {
            for(j = 0; j < s_tr[i] && j < dimen[i]; j+=20)
            {
                printf("%s, up to: %d, max: %d\n", names[i], s_tr[i], dimen[i]);
                for(k = 0; k < FEATURES; k++)
                {
                    printf("%f\t", time_turning_all[i][j][k]);
                    printf("%f\t", time_classifiers[i][j][k]);
                }
                printf("\n");
            }
//            turning[i] = turning_all[i];
        }

        printf("before text file\n");
        training_file(classifiers, dot_text_names[0], dimen, activities, input, 0, NO_TURN, ACTIVITIES);

        turning_file(turning_all, dot_text_turning, dimen, TURN_FEATURES);
        for (i = 0; i < ACTIVITIES; i++) {
            speeds_count = speeds_end[i] - speeds_start[i];
            printf("\n%s speeds: %d\n", dot_text_names[i+1], speeds_count);
            training_file(speeds, dot_text_names[i+1], dimen, speeds_outputs, SPEED_FEATURES, speeds_start[i], speeds_end[i],
                          speeds_count);

            printf("after file\n");
        }
        for (i = 0; i < NUM_TURNS; i++)
        {
            speeds_count = speeds_end[i+ACTIVITIES] - speeds_start[i+ACTIVITIES];
            printf("\n%s speeds: %d\n", dot_text_turn_name[i], speeds_count);
            training_file(turn_speeds, dot_text_turn_name[i], dimen, speeds_outputs,
                          TURN_SPEED, speeds_start[i+ACTIVITIES], speeds_end[i+ACTIVITIES], speeds_count);
        }



//        #define TIME_FEATURES 300
//        #define TIME_SPEED_FEATURES 300
//        #define TIME_TURN_FEATURES 400
//        #define TIME_TURN_SPEED 400
        printf("before time text file\n");

//        void training_file_two_inputs(float ***classifiers_server, float ***classifiers_client, const char * file_name,
//                                      int *row_counts, const int *activities, int input_server,int input_client, int s, int n, int a);


        training_file_two_inputs(classifiers, time_classifiers, time_dot_text_names[0],
                                 dimen, activities, FEATURES, CLIENT_FEATURES, 0, NO_TURN, ACTIVITIES);

//        training_file(classifiers, dot_text_names[0], dimen, activities, input, 0, NO_TURN, ACTIVITIES);

        turning_file_two_inputs(turning_all, time_turning_all, time_dot_text_turning, client_dimen);

//        void turning_file_two_inputs(float ***turning_server, float ***turning_client, const char * file_name, int *row_counts);
        for (i = 0; i < ACTIVITIES; i++) {
            speeds_count = speeds_end[i] - speeds_start[i];
            printf("\n%s speeds: %d\n", dot_text_names[i+1], speeds_count);
//            training_file(time_speeds, time_dot_text_names[i+1], dimen, speeds_outputs, TIME_SPEED_FEATURES,
//                          speeds_start[i], speeds_end[i],
//                          speeds_count);

            training_file_two_inputs(time_speeds, time_classifiers, time_dot_text_names[i+1],
                                     dimen, activities, SPEED_FEATURES, CLIENT_SPEED_FEATURES,
                                     speeds_start[i], speeds_end[i], speeds_count);

            printf("after file\n");
        }
        for (i = 0; i < NUM_TURNS; i++)
        {
            speeds_count = speeds_end[i+ACTIVITIES] - speeds_start[i+ACTIVITIES];
            printf("\n%s speeds: %d\n", dot_text_turn_name[i], speeds_count);

            training_file_two_inputs(turn_speeds, time_turn_speeds, time_dot_text_turn_name[i],
                                     dimen, activities, TURN_SPEED, CLIENT_TURN_SPEED,
                                     speeds_start[i+ACTIVITIES], speeds_end[i+ACTIVITIES], speeds_count);

        }




        train_network(dot_text_turning, dot_net_turning, TURN_FEATURES, 3);
        train_network(dot_text_names[0], dot_net_name[0], FEATURES, ACTIVITIES);
        for (i = 0; i < ACTIVITIES; i++) {
            speeds_count = speeds_end[i] - speeds_start[i];
            train_network(dot_text_names[i+1], dot_net_name[i+1], SPEED_FEATURES, speeds_count);
        }

        for (i = 0; i < NUM_TURNS; i++) {
            speeds_count = speeds_end[i+ACTIVITIES] - speeds_start[i+ACTIVITIES];
            train_network(dot_text_turn_name[i], dot_net_turn_speeds[i], TURN_SPEED, speeds_count);
        }


//        train_network(time_dot_text_turning, time_dot_net_turning, TIME_TURN_FEATURES, 3);
//        train_network(time_dot_text_names[0], time_dot_net_name[0], TIME_FEATURES, ACTIVITIES);
//        for (i = 0; i < ACTIVITIES; i++) {
//            speeds_count = speeds_end[i] - speeds_start[i];
//            train_network(time_dot_text_names[i+1], time_dot_net_name[i+1], TIME_SPEED_FEATURES, speeds_count);
//        }
//
//        for (i = 0; i < NUM_TURNS; i++) {
//            speeds_count = speeds_end[i+ACTIVITIES] - speeds_start[i+ACTIVITIES];
//            train_network(time_dot_text_turn_name[i], time_dot_net_turn_speeds[i], TIME_TURN_SPEED, speeds_count);
//        }

        for (i = 0; i < FILES; i++) {
            for (j = 0; j < dimen[i]; j++) {
                free(classifiers[i][j]);
                free(turning_all[i][j]);
                free(speeds[i][j]);
                free(turn_speeds[i][j]);
            }

            free(classifiers[i]);
            free(turning_all[i]);
            free(speeds[i]);
            free(turn_speeds[i]);
        }
        free(classifiers);
        free(turning_all);
        free(speeds);
        free(turn_speeds);
        free(dimen);
        printf("end\n");
//
    }

//    main3(2, "");
}
