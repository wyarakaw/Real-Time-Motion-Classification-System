#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include <string.h>
#include "floatfann.h"
#include "fann.h"
#include "constants.h"

//#define NUM_CLASSIFIERS 27
#define NUM_MOTIONS 7
//#define NUM_SPEEDS 4
//#define BUFF_SIZE 1024

void create_names3(char *name, char ***c, char **turn, char ***turn_speed){
    char **dot_net_name;
    char **dot_net_turn_speed;
    char *dot_turn;
    int i;
    dot_net_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_net_turn_speed = (char **) malloc(sizeof(char*) * (NUM_TURNS));
    dot_turn = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_turn, 0, sizeof(char) * BUFF_SIZE);
    for (i = 0; i < ACTIVITIES + 1; i++) {
        dot_net_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);

    }

    for (i = 0; i < NUM_TURNS; i++) {
        dot_net_turn_speed[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_net_turn_speed[i], 0, sizeof(char) * BUFF_SIZE);
    }
    sprintf(dot_net_name[0], "%s%s_%s/%s_%s.net", main_path, train_file, name, activities_file, name);
    sprintf(dot_turn, "%s%s_%s/%s_%s.net", main_path, train_file, name, turning_name, name);
    for (i = 0; i < ACTIVITIES; i++) {
        sprintf(dot_net_name[i+1], "%s%s_%s/%s_%s.net", main_path, train_file, name, train_speeds[i], name);
    }

    for (i = 0; i < NUM_TURNS; i++) {
        sprintf(dot_net_turn_speed, "%s%s_%s/%s_%s.net", main_path, train_file, name, train_turn_speeds[i], name);
    }


    *c = dot_net_name;
}
/*
NOTE: Directory containing all the .net files. NOTE: Name MUST end with a '/', e.g. "home/directory/", NOT
"home/directory"
*/
void comm_test_neural_network(float **class_v, float **turning, float **speeds,
                         float **turn_speed, int num_rows_classifiers, char* net_files_directory, char* name) {
    int i, j, motion, speed, turn, turning_speed;//, turn_left, turn_right;
    float max;
    float turn_max;
    float speed_max;
    float turn_speed_max;

    char * dot_net_dir = malloc(sizeof(char)*BUFF_SIZE);
    memset(dot_net_dir, 0, BUFF_SIZE);
    sprintf(dot_net_dir, "%s%s_%s", main_path, train_file, name);


    //names of files
    char *TRAINING = (char *) malloc(sizeof(char) * 1024);
    char *WALKING = (char *) malloc(sizeof(char) * 1024);
    char *RUNNING = (char *) malloc(sizeof(char) * 1024);
    char *JUMPING = (char *) malloc(sizeof(char) * 1024);

    char *STAIR_ASCENT = (char *) malloc(sizeof(char) * 1024);
    char *STAIR_DESCENT = (char *) malloc(sizeof(char) * 1024);
    char *TWO_STAIR_ASCENT = (char *) malloc(sizeof(char) * 1024);
    char *TWO_STAIR_DESCENT = (char *) malloc(sizeof(char) * 1024);
    char *TURNING_ANN = (char *) malloc(sizeof(char) * 1024);

    char *WALK_TURN_RIGHT = (char *) malloc(sizeof(char) * 1024);
    char *WALK_TURN_LEFT = (char *) malloc(sizeof(char) * 1024);
    char *RUN_TURN_RIGHT = (char *) malloc(sizeof(char) * 1024);
    char *RUN_TURN_LEFT = (char *) malloc(sizeof(char) * 1024);
    char *JUMP_TURN_RIGHT = (char *) malloc(sizeof(char) * 1024);
    char *JUMP_TURN_LEFT = (char *) malloc(sizeof(char) * 1024);


    //char *TURNING_RIGHT = (char*)malloc(sizeof(char)*1024);

//    sprintf(TRAINING,"%sTRAINING_%s.net", net_files_directory, name);
//    sprintf(WALKING,"%sWALKING_%s.net", net_files_directory, name);
//    sprintf(RUNNING,"%sRUNNING_%s.net", net_files_directory, name);
//    sprintf(JUMPING,"%sJUMPING_%s.net", net_files_directory, name);
//    sprintf(STAIR_ASCENT,"%sSTAIR_ASCENT_%s.net", net_files_directory, name);
//    sprintf(STAIR_DESCENT,"%sSTAIR_DESCENT_%s.net", net_files_directory, name);
//    sprintf(TURNING_LEFT,"%sTURNING_LEFT_%s.net", net_files_directory, name);
//    sprintf(TURNING_RIGHT,"%sTURNING_RIGHT_%s.net", net_files_directory, name);

    sprintf(TRAINING, "%s/Comm_%s_%s.net", dot_net_dir, activities_file, name);
    sprintf(WALKING, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[0], name);
    sprintf(RUNNING, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[1], name);
    sprintf(STAIR_ASCENT, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[2], name);
    sprintf(STAIR_DESCENT, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[3], name);
    sprintf(TWO_STAIR_ASCENT, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[4], name);
    sprintf(TWO_STAIR_DESCENT, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[5], name);
    sprintf(JUMPING, "%s/Comm_%s_%s.net", dot_net_dir, train_speeds[6], name);

    sprintf(TURNING_ANN, "%s/Comm_%s_%s.net", turning_name, name);

    sprintf(WALK_TURN_RIGHT, "%s/Comm_%s_%s.net", dot_net_dir, train_turn_speeds[0], name);
    sprintf(WALK_TURN_LEFT, "%s/Comm_%s_%s.net", dot_net_dir, train_turn_speeds[1], name);
    sprintf(RUN_TURN_RIGHT, "%s/Comm_%s_%s.net", dot_net_dir, train_turn_speeds[2], name);
    sprintf(RUN_TURN_LEFT, "%s/Comm_%s_%s.net", dot_net_dir, train_turn_speeds[3], name);
    sprintf(JUMP_TURN_RIGHT, "%s/Comm_%s_%s.net", dot_net_dir, train_turn_speeds[4], name);
    sprintf(JUMP_TURN_LEFT, "%s/Comm_%s_%s.net", dot_net_dir, train_turn_speeds[5], name);



//    sprintf(TURNING_RIGHT,"%sTURNING_RIGHT_%s.net", net_files_directory, name);
    // calc_out returns an array of outputs, with the number of outputs being equal to number
    // of neurons in output layer
    fann_type *calc_out_motion, *calc_out_speed, *calc_out_turning, *calc_out_turn_speed;//_left, *calc_out_turning_right;
    fann_type input[FEATURES];
    fann_type turn_input[TURN_FEATURES];
    fann_type speed_input[SPEED_FEATURES];
    fann_type turn_speed_input[NUM_TURNS];

    // Our artificial neural networks for motions and speeds
    struct fann *motion_ann,
            *walking_ann,
            *running_ann,
            *stair_ascent_ann,
            *stair_descent_ann,
            *two_stair_ascent_ann,
            *two_stair_descent_ann,
            *jumping_ann,
            *turning_ann,

            *walk_turn_right_ann,
            *walk_turn_left_ann,
            *run_turn_right_ann,
            *run_turn_left_ann,
            *jump_turn_right_ann,
            *jump_turn_left_ann;



    // Will help determine motion
    motion_ann = fann_create_from_file(TRAINING);

    // These will help determine speed corresponding to a specific motion
    walking_ann = fann_create_from_file(WALKING);
    running_ann = fann_create_from_file(RUNNING);
    stair_ascent_ann = fann_create_from_file(STAIR_ASCENT);
    stair_descent_ann = fann_create_from_file(STAIR_DESCENT);
    two_stair_ascent_ann = fann_create_from_file(STAIR_ASCENT);
    two_stair_descent_ann = fann_create_from_file(STAIR_DESCENT);
    jumping_ann = fann_create_from_file(JUMPING);
    turning_ann = fann_create_from_file(TURNING_ANN);
    //turning_right_ann   = fann_create_from_file(TURNING_RIGHT);

    walk_turn_right_ann = fann_create_from_file(WALK_TURN_RIGHT);
    walk_turn_left_ann = fann_create_from_file(WALK_TURN_LEFT);
    run_turn_right_ann = fann_create_from_file(RUN_TURN_RIGHT);
    run_turn_left_ann = fann_create_from_file(RUN_TURN_LEFT);
    jump_turn_right_ann = fann_create_from_file(JUMP_TURN_RIGHT);
    jump_turn_left_ann = fann_create_from_file(JUMP_TURN_LEFT);

    // Go through all n rows for class_v
    for (i = 0; i < num_rows_classifiers; i++)
    {

        // Populate our input array in preparation for neural network
        for (j = 0; j < FEATURES; j++) {
            input[j] = class_v[i][j];
        }
        for (j = 0; j < TURN_FEATURES; j++) {
            turn_input[j] = turning[i][j];

        }
        for (j = 0; j < SPEED_FEATURES; j++) {
            speed_input[j] = speeds[i][j];
        }
        for (j = 0; j < NUM_TURNS; j++) {
            turn_speed_input[j] = turn_speed[i][j];

        }

        calc_out_motion = fann_run(motion_ann, input);
        calc_out_turning = fann_run(turning_ann, turn_input);

        max = -100;
        speed_max = -100;
        turn_max = -100;
        turn_speed_max = -100;
        motion = 0;
        turn = 0;
        speed = 0;
        turning_speed = 0;
        for (j = 0; j < NUM_MOTIONS; j++) {
            if (calc_out_motion[j] > max) {
                max = calc_out_motion[j];
                motion = j;
            }
            printf("%f\t", calc_out_motion[j]);
        }
        printf("\n");
//        printf("turning:\t");
        for (j = 0; j < 3; j++) {
            // check calc_out for left and right turns, return the maximum
            if (calc_out_turning[j] > turn_max) {
                turn_max = calc_out_turning[j];
                turn = j;
            }
//            printf("%f\t", calc_out_turning[j]);
        }
//        printf("\n");
        /* Now that motion is determined, find speed */
        switch (motion) {
            case 0:
                calc_out_speed = fann_run(walking_ann, speed_input);
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
//                printf("\n");
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);

                if(turn == 1)
                {
                    calc_out_turn_speed = fann_run(walk_turn_right_ann, turn_speed_input);
//                    printf("turn speed:   \t");
                    for (j = 0; j < turn_speed_outputs[motion]; j++) {

                        if (calc_out_speed[j] > turn_speed_max) {
                            turn_speed_max = calc_out_turn_speed[j];
                            turning_speed = j;
                        }
//                        printf("%f\t", calc_out_speed[j]);
                    }
                } else if(turn == 2)
                {
                    calc_out_turn_speed = fann_run(walk_turn_left_ann, turn_speed_input);
//                    printf("turn speed:   \t");
                    for (j = 0; j < turn_speed_outputs[motion]; j++) {

                        if (calc_out_speed[j] > turn_speed_max) {
                            turn_speed_max = calc_out_turn_speed[j];
                            turning_speed = j;
                        }
//                        printf("%f\t", calc_out_speed[j]);
                    }
                }
//                printf("\n");
                if(turn == 0){
                    printf("Motion: %s --> Speed: %s\t%s\n", activities_output[motion], walk_speeds[speed], turn_output[turn]);
                }
                else{
                    printf("Motion: %s --> Speed: %s\tTurn: %s %s\n", activities_output[motion],
                           walk_speeds[speed], turn_output[turn], walk_turn_output[turning_speed]);
                }

                break;
            case 1:
                calc_out_speed = fann_run(running_ann, speed_input);
                speed_max = -100;
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
//                printf("\n");
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);

                if(turn == 1)
                {
                    calc_out_turn_speed = fann_run(run_turn_right_ann, turn_speed_input);
//                    printf("turn speed:   \t");
                    for (j = 0; j < turn_speed_outputs[motion]; j++) {

                        if (calc_out_speed[j] > turn_speed_max) {
                            turn_speed_max = calc_out_turn_speed[j];
                            turning_speed = j;
                        }
//                        printf("%f\t", calc_out_speed[j]);
                    }
//                    printf("\n");
                } else if(turn == 2)
                {
                    calc_out_turn_speed = fann_run(run_turn_left_ann, turn_speed_input);
//                    printf("turn speed:   \t");
                    for (j = 0; j < turn_speed_outputs[motion]; j++) {

                        if (calc_out_speed[j] > turn_speed_max) {
                            turn_speed_max = calc_out_turn_speed[j];
                            turning_speed = j;
                        }
//                        printf("%f\t", calc_out_speed[j]);
                    }
//                    printf("\n");
                }

                if(turn == 0){
                    printf("Motion: %s --> Speed: %s\t%s\n", activities_output[motion], walk_speeds[speed], turn_output[turn]);
                }
                else{
                    printf("Motion: %s --> Speed: %s\tTurn: %s %s\n", activities_output[motion],
                           walk_speeds[speed], turn_output[turn], run_turn_output[turning_speed]);
                }
                break;
            case 2:
                calc_out_speed = fann_run(stair_ascent_ann, speed_input);
                speed_max = -100;
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);
                printf("Motion: %s --> Speed: %s\n", activities_output[motion], standard_speeds[speed]);
                break;
            case 3:
                calc_out_speed = fann_run(stair_descent_ann, speed_input);
                speed_max = -100;
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);
                printf("Motion: %s --> Speed: %s\n", activities_output[motion], standard_speeds[speed]);
                break;
            case 4:
                calc_out_speed = fann_run(two_stair_ascent_ann, speed_input);
                speed_max = -100;
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);
                printf("Motion: %s --> Speed: %s\t%s\n", activities_output[motion], two_staires_speeds[speed],
                       turn_output[turn]);
                break;
            case 5:
                calc_out_speed = fann_run(two_stair_descent_ann, speed_input);
                speed_max = -100;
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);
                printf("Motion: %s --> Speed: %s\t%s\n", activities_output[motion], two_staires_speeds[speed],
                       turn_output[turn]);
                break;
            case 6:
                calc_out_speed = fann_run(jumping_ann, speed_input);
                speed_max = -100;
//                printf("speed:   \t");
                for (j = 0; j < total_speeds[motion]; j++) {

                    if (calc_out_speed[j] > speed_max) {
                        speed_max = calc_out_speed[j];
                        speed = j;
                    }
//                    printf("%f\t", calc_out_speed[j]);
                }
//                printf("\n");

                turn_max = -100;
//                printf("turning:\t");
                turn = 0;
                for (j = 0; j < NUM_TURNS; j++){
                    // check calc_out for left and right turns, return the maximum
                    if (calc_out_turning[j] > turn_max) {
                        turn_max = calc_out_turning[j];
                        turn = j;
                    }
//                    printf("%f\t", calc_out_turning[j]);
                }
//                printf("\n");
                printf("%f\t%f\t%f\n", max, speed_max,  turn_max);

                if(turn == 1)
                {
                    calc_out_turn_speed = fann_run(jump_turn_right_ann, turn_speed_input);
//                    printf("turn speed:   \t");
                    for (j = 0; j < turn_speed_outputs[motion]; j++) {

                        if (calc_out_speed[j] > turn_speed_max) {
                            turn_speed_max = calc_out_turn_speed[j];
                            turning_speed = j;
                        }
//                        printf("%f\t", calc_out_speed[j]);
                    }
//                    printf("\n");
                } else if(turn == 2)
                {
                    calc_out_turn_speed = fann_run(jump_turn_left_ann, turn_speed_input);
//                    printf("turn speed:   \t");
                    for (j = 0; j < turn_speed_outputs[motion]; j++) {

                        if (calc_out_speed[j] > turn_speed_max) {
                            turn_speed_max = calc_out_turn_speed[j];
                            turning_speed = j;
                        }
//                        printf("%f\t", calc_out_speed[j]);
                    }
//                    printf("\n");
                }
                if(turn == 0){
                    printf("Motion: %s --> Height: %s\t%s\n", activities_output[motion], jump_height[speed], turn_output[turn]);
                }
                else{
                    printf("Motion: %s --> Height: %s\tTurn: %s %s\n", activities_output[motion],
                           jump_height[speed], turn_output[turn], jump_turn_output[turning_speed]);
                }
                break;
            default:
                break;
        }

        if (max < 0)
            printf("NEGATIVE ACTIVITY\n");
        if (speed_max < 0)
            printf("NEGATIVE SPEED\n");
        if (turn_max < 0)
            printf("NEGATIVE TURN\n");
        printf("\n");

    }

    fann_destroy(motion_ann);
    fann_destroy(walking_ann);
    fann_destroy(running_ann);
    fann_destroy(stair_ascent_ann);
    fann_destroy(stair_descent_ann);
    fann_destroy(two_stair_ascent_ann);
    fann_destroy(two_stair_descent_ann);
    fann_destroy(jumping_ann);
    fann_destroy(turning_ann);

    fann_destroy(walk_turn_right_ann);
    fann_destroy(walk_turn_left_ann);
    fann_destroy(run_turn_right_ann);
    fann_destroy(run_turn_left_ann);
    fann_destroy(jump_turn_right_ann);
    fann_destroy(jump_turn_left_ann);
}

