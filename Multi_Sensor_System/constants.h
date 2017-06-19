//
// Created by Amaael Antonini on 5/5/17.
//

#ifndef REAL_TIME_TRIAL_CONSTANTS_H
#define REAL_TIME_TRIAL_CONSTANTS_H


#define FILES 36       // number of files for training
#define TURNING 36
#define NO_TURN 20


#define FEATURES 235     // number of features to use
#define SPEED_FEATURES 66
#define TURN_FEATURES 289
#define TURN_SPEED 89

#define CLIENT_FEATURES 235     // number of features to use
#define CLIENT_SPEED_FEATURES 66
#define CLIENT_TURN_FEATURES 289
#define CLIENT_TURN_SPEED 89


//#define TIME_FEATURES 300     // number of features to use
//#define TIME_SPEED_FEATURES 300
//#define TIME_TURN_FEATURES 400
//#define TIME_TURN_SPEED 400

#define NUM_TURNS 6
#define ACTIVITIES 7    // number of activities

#define DATA 7         // number of columns in file time acceleration x, y, z, gyro x, y, z

#define BUFF_SIZE 1024  // buffer for chars
    // buffer for strides

#define PRINT_FEATURES 1

#define NULL 0          // 0 char


#define WAIT 30         // time to wait before starting to train
#define RUN 20          // time collecting training data each file
#define NORM 1000000.0f // normalizer for data collection
#define IDLE 40         // idle time collecting data

#define TEST_SIZE 10
#define MAX_FILES 1000

extern const int who;
extern const char *turn_output[];
extern const char * main_path;

extern const char * names[];

extern const char * activities_file;

extern const char * train_speeds[];
extern const char * train_turn_speeds[];
extern const char *train_name;
extern const char * test_name;

extern const char * speed_output[];
extern const char * activities_output[];

extern const char *standard_speeds[];
extern const char *walk_speeds[];
extern const char *two_staires_speeds[];
extern const char *jump_height[];


extern const char * walk_turn_output[];
extern const char * run_turn_output[] ;
extern const char * jump_turn_output[];


extern const int no_turn[];
extern const int turn_right[];
extern const int turn_left[];
extern const int turn_out[];
extern const int activities[];

extern const int speeds_outputs[];
extern const int turn_speed_outputs[];
extern const int speeds_start[];
extern const int speeds_end[];
extern const int total_speeds[];
extern const int turns[];

extern const char * user_names[];
extern const char * main_path;
extern const char * class_files[];

extern const char * paths[];
extern const char * paths_out[];
extern const char * stride_path;
extern const char * train_file;
extern const char * test_dir;
extern const char * turning_name;
extern const int train_times[];

extern const float filter[];

extern const int s_d[];
extern const int s_tr[];
extern const int s_ts[];


#endif //REAL_TIME_TRIAL_CONSTANTS_H




