

#define SLOTS 8       // split strides into slots
#define PARTIAL 6
#define PRINT 100000
#define MAX_DATA 20000
#define STRIDES 200
#define CONDITIONS 12
#define CLASS_SLOTS 100

#ifndef FILE_EXAMPLE_CLASSIFIERS_H
#define FILE_EXAMPLE_CLASSIFIERS_H

int checkCSV(char * username);

int checkTestDataMatch(char* username, char** filename_list);

void fake_data(const char **names);

void collect_training_data(char * folder, char *user);

void gather_data(char *file_name, int hold_time, int record_time);
// gets a file name and prints sensor data into that file

//void train_network(char * train_name, char);

//void test_neural_network(float **class_v, int n);

void test_data(char *test_path, char *test_name);

int test_classifiers(float **classifiers, float ** turning, const char * name, int *n);

void training_file(float ***classifiers, const char * file_name, int *row_counts, const int *activities, int inputs, int s, int n, int a);


int temp_strides(float **data, int *out, float *sigma_y, float *sigma_z, float max_accel,
                 float max_gyro, float max_time, float min_time, int n);

void train_parameters(float ****classifiers, int **dimensions);

int select_strides(float * times, int * strides, int * out, float min_time, int n);

void sample_mean(float * data, float * out, int chunk, int n);


void sample_sigma(float * data, float * mean, float * out, int chunk, int n);

void sample_sigma_2(float * data, float * mean, float ** out, int chunk, int n);

void center_data(float * data, float * mean, float * sigma, int chunk, int n);

int peak_strides(float * time, float * data, int * strides, int *out, float *sigma, float weight, int sn,  int n);

void move_back(int * strides, int offset, int n);

void sigma_stride(float * data, int stride[][4], float * level, float * sigma_low, float * sigma_high, int n);
//void center_data(float * data, float * center, int n);

void peaks(float * data, int strides[][4], float max_peak[][4], float max_trough[][4], int n);

void mean_slot(float * data, int strides[][4], float mean_low[][SLOTS], float mean_high[][SLOTS], float slots, int n);

int outlier(float * data, int * strides, int * out, float min_diff, int n);

void consumer(char * name);

void strides(char * input_file, float **data, float **sigma, float **mean,
             int *data_length, int *strides_t2, int *strides_t1, int *rows);

void features(float **data, float **mean, float **sigma, int *strides, int stride_count,
              float **classifiers, float **turning, float **speeds, float **turn_speeds, int *rows);

void turning_file(float ***turning, const char * file_name, int *row_counts, int inputs);


//int find_strides(float *data, int * indexes, float *data2, int * start1, int * norms, int *dist, int *widths, float weight, int n);


int find_strides(float *data, int * indexes, float *data2, int * start1, int *dist, float * norms,
                 float *widths, float w_h, float w_w, int dist_range, int n);

int integrate_zeros(float * data, float *out, int * indexes, int n);

void max_point(float * data, int strides[][4], int *indexes,
                float peak[][4], int n, float ratio);
int main3(int argc, char *argv);
int main2(int argc, char *argv);

void filter_data(float *data, float * dest, const float * shape, int s, int n);

void mean_stride(float * data, int strides[][4],
                 float *upper, float *lower, int n);

void max_slot(float * data, int strides[][4], float max_low[][PARTIAL], float max_high[][PARTIAL], float ratio, int slots, int n);


void sigma_stride_hl(float * data, int stride[][4], float * sigma_low, float * sigma_high, int n);

void higher_moment(float * data, int stride[][4], float *sigma_low, float *sigma_high,
                  float * tm_low, float * tm_high, float * fm_low, float * fm_high, float w, int n);

void kurt_skew(float * data, int stride[][4], float *sigma_low, float *sigma_high,
               float * tm_low, float * tm_high, float * fm_low, float * fm_high, float *mean_high, float *mean_low,
               float *s_h, float *s_l, float *k_h, float *k_l, float w, float *max_d, float top, float ratio,  int n);


void sigma_stride_total(float * data, int stride[][4], float * dest, int n);

void time_slots(float *time, float * data, int strides[][4], float dest[][SLOTS], int n);

void norm_minus_one(float *source, float*norm, float*dest, float w, int n);


void peak_trough_compare(int strides[][4], float peak1[][4], float peak2[][4], float trough1[][4], float trough2[][4],
                         float *norm1, float d_p1[][CONDITIONS], float d_t1[][CONDITIONS],
                         float w1, float w2, float w3, int n);

void sigma_condition(float source[][PARTIAL], float*norm, float dest[][PARTIAL], float w, int n);

void average_deviation(float *data, int strides[][4], float *dest, float *max_d, int n);

void time_features(float **data, float **mean, float **sigma, int *strides, int stride_count,
                   float **classifiers, float **turning, float **speeds, float **turn_speeds, int *rows);

void class_slots(float *time, float * data, int strides[][4], float dest[][CLASS_SLOTS], int n);

void stride_match(float *time_source, float *time_dest, int * strides_source, int *strides_dest, int n_s, int n_d);

void match_strides(char * input_file, float **data, float **sigma, float **mean,
                   float *source_times, int *data_length, int *strides_source, int *strides_dest, int *rows);

void turning_file_two_inputs(float ***turning_server, float ***turning_client, const char * file_name, int *row_counts);

void training_file_two_inputs(float ***classifiers_server, float ***classifiers_client, const char * file_name,
                              int *row_counts, const int *activities, int input_server,int input_client, int s, int n, int a);

#endif //FILE_EXAMPLE_CLASSIFIERS_H
