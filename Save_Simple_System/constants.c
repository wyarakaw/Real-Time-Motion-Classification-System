//
// Created by Amaael Antonini on 5/5/17.
//
#include "constants.h"
//const char * main_path = "../";
const char * main_path = "";
const char * paths[] = {"Data/Train_Data_Set_AA/", "Data/Train_Data_Set_AA/"};
//const char * paths[] = {"Data_sets/AA2_data/", "Data_sets/DA2_data/"};
const char * user_names[] = {"AA2", "DA2"};
// output file names for parameters, currently not in use
const char * class_files[] = {"Classifiers/AA_file/", "Classifiers/DA_file/"};
const char * paths_out[] = {"Data_sets_out/AA2_data/", "Data_sets_out/DA2_data/"};
const char * stride_path = "Data_strides_out/";

// name for training and testing files, will be changed
const char *train_name = "training.txt";
const char * turning_name = "turning_file";
const char * test_name = "test";
const char * activities_file = "activities";
const int who = 0;

const char * train_file = "Train_Data_Set";
const char * test_dir = "Test_Dir";


//const char * train_speeds[] = {"walk", "run", "stairs_up", "stairs_down", "jump"};
//
//const char * names[] = {
//        "walk_speed_1_50sec_32m",
//        "walk_speed_2_35sec_32m",
//        "walk_speed_3_25sec_32m",
//        "walk_speed_4_15sec_32m",
//        "slow_run",
//        "medium_run",
//        "fast_run",
//        "stairs_up",
//        "stairs_down",
//        "low_jump",
//        "medium_jump",
//        "high_jump"
//};
//
//const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 2, 3, 4, 4, 4};
//const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 0, 0, 0, 1, 2};
//const int speeds_start[]  =     {0, 4, 7, 8, 9};
//const int speeds_end[] =        {4, 7, 8, 9, 12};

const char *turn_output[] =        {"", "turn right", "turn left"};
const char *standard_speeds[] =    {"slow      ", "medium     ", "fast      "};
const char *walk_speeds[] =        {"slow      ", "medium slow", "medium fast", "fast      "};

const char *jump_height[] =        {"low       ", "medium     ", "high       "};
const char *two_staires_speeds[] = {"slow      ", "fast       "};


const char * walk_turn_output[]  = {"slight", "sharp"};
const char * run_turn_output[]  = {"slight", "medium", "sharp"};
const char * jump_turn_output[]  = {"slight", "medium", "sharp"};

const char * train_speeds[] = {"walk", "run", "stairs_up", "stairs_down", "two_stairs_up", "two_stairs_down", "jump"};
const char * train_turn_speeds[] = {"walk_turn_right", "walk_turn_left", "run_turn_right",
                                    "run_turn_left", "jump_turn_right", "jump_turn_left"};
const char * activities_output[] = {"walk         ", "run          ", "stairs up    ", "stairs down  ",
                                    "2 stairs up  ", "2 stairs down", "jump         "};
//const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
//const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 6, 6,
                                 7, 7, 8, 8, 9, 9, 9,10,10,10,11,11,11,12,12,12};

const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 0, 1, 0, 1, 2,
                                 0, 1, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2};


const int speeds_start[]  =     {0, 4,  7, 10, 13, 15, 17, 20, 22, 24, 27, 30, 33};
const int speeds_end[] =        {4, 7, 10, 13, 15, 17, 20, 22, 24, 27, 30, 33, 36};

const int total_speeds[] =       {4, 3,  3,  3,  2,  2,  3};
const int turn_speed_outputs[] = {2, 3,  0,  0,  0,  0,  3};

const int train_times[] =       {15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10,
                                 15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10};

//const int turns[] = {0, 1, 2, 3, 20, 21, 22, 23, 24, 25, 26, 27};
const int turn_out[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        1, 1, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2};

const float filter[] ={
        0.0023322 ,0.0137681 ,0.0404694 ,0.0732201 ,0.0969957 ,0.1076743 ,0.1103011 ,
        0.1104785 ,0.1103011 ,0.1076743 ,0.0969957 ,0.0732201 ,0.0404694 ,0.0137681 ,0.0023322};

const char * names[] = {
        "walk_speed_1_50sec_32m",
        "walk_speed_2_35sec_32m",
        "walk_speed_3_25sec_32m",
        "walk_speed_4_15sec_32m",
        "slow_run",
        "medium_run",
        "fast_run",
        "slow_stairs_up",
        "medium_stairs_up",
        "fast_stairs_up",
        "slow_stairs_down",
        "medium_stairs_down",
        "fast_stairs_down",
        "slow_two_stairs_up",
        "fast_two_stairs_up",
        "slow_two_stairs_down",
        "fast_two_stairs_down",
        "low_jump",
        "medium_jump",
        "high_jump",
        "walk_turn_right_2",
        "walk_turn_right_3",
        "walk_turn_left_2",
        "walk_turn_left_3",
        "run_turn_right_1",
        "run_turn_right_2",
        "run_turn_right_3",
        "run_turn_left_1",
        "run_turn_left_2",
        "run_turn_left_3",
        "jump_turn_right_1",
        "jump_turn_right_2",
        "jump_turn_right_3",
        "jump_turn_left_1",
        "jump_turn_left_2",
        "jump_turn_left_3"
};
//const char * names[] = {
//        "walk_speed_1_50sec_32m",
//        "walk_speed_2_35sec_32m",
//        "walk_speed_3_25sec_32m",
//        "walk_speed_4_15sec_32m",
//        "slow_run",
//        "medium_slow_run",
//        "medium_fast_run",
//        "fast_run",
//        "slow_stairs_up",
//        "medium_slow_stairs_up",
//        "medium_fast_stairs_up",
//        "fast_stairs_up",
//        "slow_stairs_down",
//        "medium_slow_stairs_down",
//        "medium_fast_stairs_down",
//        "fast_stairs_down",
//        "low_jump",
//        "medium_low_jump",
//        "medium_high_jump",
//        "high_jump",
//        "turning_right_speed_1",
//        "turning_right_speed_2",
//        "turning_right_speed_3",
//        "turning_right_speed_4",
//        "turning_left_speed_1",
//        "turning_left_speed_2",
//        "turning_left_speed_3",
//        "turning_left_speed_4"
//};

const int no_turn[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
const int turn_right[] = {20, 21, 22, 26, 27, 28, 32, 33, 34};
const int turn_left[]  = {23, 24, 25, 29, 30, 31, 35, 36, 37};

const int s_d[] = {7,	12,	12,	9,	15,	18,	16,	10,	14,	17,
                   9,	12,	16,	6,	10,	6,	11,	7,	7,	4,
                   10,	5,	10,	6,	16,	16,	10,	18,	17,	11,
                   6,	6,	3,	6,	6,	3};
const int s_tr[] = {5,	8,	8,	6,	11,	12,	12,	7,	10,	12,
                   6,	8,	12,	4,	7,	4,	8,	5,	5,	3,
                   7,	3,	7,	4,	12,	12,	7,	12,	12,	8,
                   4,	4,	2,	4,	4,	2};

const int s_ts[] = {5,	8,	8,	6,	11,	12,	12,	7,	10,	12,
                   6,	8,	12,	4,	7,	4,	8,	5,	5,	3, 0, 0,
                   7,	3,	7,	4,	12,	12,	7,	12,	12,	8,
                   4,	4,	2,	4,	4,	2};
