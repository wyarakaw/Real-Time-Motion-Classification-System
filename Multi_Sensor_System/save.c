//
// Created by Amaael Antonini on 5/7/17.
//






void collect_training_data(char * folder, char *user)
{

    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }

    // determine which files are to be created and a directory for the files
    // assume training data is needed
    // create folder?? assume folder has been created?? chech if folder exist and then create it

    int hold;       // how long to wait before start recording
    int record;     // how long will the recording be
    int done_training;       // training is complete
    int current;    // index of the current file in use
    int *indexes;   // list of files indexes
    int repeat_all_session;       // restart training if requested
    int i;
    indexes = (int *) malloc(sizeof(int) * FILES);

    char** file_names;
    char *dir;
    dir = (char *) malloc(BUFF_SIZE * sizeof(char));
    memset(dir, 0, BUFF_SIZE);
    snprintf(dir, BUFF_SIZE, "%s_%s", train_file, user);
    file_names = (char**)malloc(FILES*sizeof(char*));

    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;


    for(i = 0; i < FILES; i++) {
        file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
        memset(file_names[i], 0, BUFF_SIZE);
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);
        indexes[i] = i;
    }




//    mraa_i2c_context accel, gyro, mag;
//    accel_scale_t a_scale;
//    gyro_scale_t g_scale;
//    mag_scale_t m_scale;
//    data_t ad, gd, md;
//
//    a_scale = A_SCALE_16G;
//    g_scale = G_SCALE_500DPS;
//    m_scale = M_SCALE_8GS;
//
//    accel = accel_init();
//    set_accel_ODR(accel, A_ODR_100);
//    set_accel_scale(accel, a_scale);
//    a_res = calc_accel_res(a_scale);
//
//    gyro = gyro_init();
//    set_gyro_ODR(gyro, G_ODR_190_BW_70);
//    set_gyro_scale(gyro, g_scale);
//    g_res = calc_gyro_res(g_scale);

//    mag = mag_init();
//    set_mag_ODR(mag, M_ODR_125);
//    set_mag_scale(mag, m_scale);
//    m_res = calc_mag_res(m_scale);

    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    // Train_Data_Set_JY/walk_speed_1_JY.csv

    repeat_all_session = 1;
    while(repeat_all_session)
    {
        repeat_all_session = 0;
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);

        mkdir(dir, 0777);
        for(i = 0; i < FILES; i++)
        {
            if(access(file_names[i], F_OK) == -1 )
            {
                indexes[i] = -1;
            }
            else
            {
                printf("done\n");
            }
        }
        // determine if folder exist and create it otherwise
        // if folder created break
        // if folder existed determine which files are missing
        // set indexes[existing] to -1;
        // request if any overwrites
        // request which to overwrite if yes
        // make a list of the indexes to overwrite
        // set default hold and record???

        i = 0;
        done_training = 0;
        hold = 10; // set hold to 10????
        current = -1;

        while(interrupt_flag && done_training != 1) {

            // dialog to determine which file to train, how long to hold, how long to record
            for(i = 0; i < FILES; i++)
            {
                if(indexes[i] == i)
                {
                    printf("%d\t%s\n", i+1, names[i]);
                    // print the names of the files missing
                    // assume that indexes of not missing files are set to -1
                }
            }
            // prompt to enter the index of the file to record
            // prompt to enter hold and record time or default to standard
            // set current to the index to train: e.g i-1;

            if(current == 1)
            {
                printf("invalid entry, choose again\t");
                continue;
            }


            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            int safe_guard;
            safe_guard = 0;
            printf("next %s\tin:\t%d\n", names[i], (int) (WAIT-time_wait));
            while (time_wait <= hold && safe_guard < 10000)
            {

                printf("%d\t%s\n", (int) (WAIT - time_wait), names[i]);
                usleep(1000000);
                safe_guard++;
                gettimeofday(&time_before, NULL);
                time_end = time_before.tv_sec;
                time_wait = time_end - time_ref;
            }
            usleep(1000000);
            printf("Attempting to write to file \'%s\'.\n", file_names[current]);
            fp = fopen(file_names[current], "w");
            if (fp == NULL) {
                fprintf(stderr,
                        "Failed to write to file \'%s\'.\n",
                        file_names[current]
                );
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
                    " Acceleration z, Gyro x, Gyro y, Gyro z\n");
            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            while(time_wait < RUN & interrupt_flag)
            {
                gettimeofday(&time_before, NULL);


//            ad = read_accel(accel, a_res);
//            gd = read_gyro(gyro, g_res);

                time_wait = time_before.tv_sec - time_ref;

                time_start = time_before.tv_sec + time_before.tv_usec / NORM;
//            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

                usleep(100);
            }

            fclose(fp);
            // dialog to determine if recording is successful
            // determine if section is done_training
        }
        // dialog to restart data collecting
        // if need to restart, repeat_all_session = 1
    }
}


void gather_data(char *file_name, int hold_time, int record_time)
{
    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }
    int i = 0;
    FILE *fp;

    double time_start, time_wait, time_ref;
    struct timeval time_before;
    float a_res, g_res, m_res;

//    mraa_i2c_context accel, gyro, mag;
//    accel_scale_t a_scale;
//    gyro_scale_t g_scale;
//    mag_scale_t m_scale;
//    data_t ad, gd, md;
//
//
//    a_scale = A_SCALE_16G;
//    g_scale = G_SCALE_500DPS;
//    m_scale = M_SCALE_8GS;
//
//    accel = accel_init();
//    set_accel_ODR(accel, A_ODR_100);
//    set_accel_scale(accel, a_scale);
//    a_res = calc_accel_res(a_scale);
//
//    gyro = gyro_init();
//    set_gyro_ODR(gyro, G_ODR_190_BW_70);
//    set_gyro_scale(gyro, g_scale);
//    g_res = calc_gyro_res(g_scale);
//    mag = mag_init();
//    set_mag_ODR(mag, M_ODR_125);
//    set_mag_scale(mag, m_scale);
//    m_res = calc_mag_res(m_scale);

    time_wait = 0;
    fp = fopen(file_name, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to write to file \'%s\'.\n",
                file_name
        );
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
            " Acceleration z, Gyro x, Gyro y, Gyro z\n");
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    while(time_wait < TEST_SIZE && interrupt_flag) {

//        ad = read_accel(accel, a_res);
//        gd = read_gyro(gyro, g_res);

        gettimeofday(&time_before, NULL);
        time_start = time_before.tv_sec + time_before.tv_usec / NORM;
        time_wait += (int) (time_start - time_ref);
//        fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

        usleep(100);
    }

    fclose(fp);
}



#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


//const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 2, 3, 4, 4, 4};
//const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 0, 0, 0, 1, 2};
//const int speeds_start[]  =     {0, 4, 7, 8, 9};
//const int speeds_end[] =        {4, 7, 8, 9, 12};
int main(int argc, char **argv) {

    char *fake_test;
    fake_test = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(fake_test, 0, sizeof(char) * BUFF_SIZE);
    sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[0], user_names[0]);

    //char * test_file_name;
//    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
//    memset(test_file_name, 0, BUFF_SIZE);

    float ***classifiers = NULL;
    // pointer to 3 d array with first D1 is activities,
    // second D2 is rows per activity,
    // third D3 is number of classifies (27 for now)
    int **counts = NULL;
    // pointer to 2 d array with first D1 is holds number of activities (12 total for now)
    // second D number of rows
    // each element is number of classifiers in each row, (constant of 27, not necessary)
    int *dimen = NULL;
    // dime is a pointer to 1 d array with first D1 number of activities (12 for now)
    // second D2 is rows per activity
    int n;
    // n is integer with number of activities (12 for now)
    int input;
    input = FEATURES;
    // when training, there will be sum o n rows if dim[i] for i from 0 to n-1 rows
    // dim[i] is the truth value or element 1 in row of -1 -1 -1 1(ith) -1

    // example follows
    int i, j, k;
    int speeds;
    float **test_array;
    int num_of_test_classifers;
    int truth_value;
    char *training_name;
    training_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
//    if(access("training.txt", F_OK) == -1 || (argv[1] == "y" || argv[1] == "Y")){
    printf("if loop\n");


    int dot_net_files = 0;
    char *dot_net_name;
    dot_net_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_net_name, 0, sizeof(char) * BUFF_SIZE);
    sprintf(dot_net_name, "%s_%s/%s_%s.net", main_path, argv[1], train_file, activities_file, argv[1]);
    if (access(dot_net_name, F_OK) == -1)
        dot_net_files = -1;
    for (i = 0; i < ACTIVITIES; i++) {
        memset(dot_net_name, 0, sizeof(char) * BUFF_SIZE);
        sprintf(dot_net_name, "%s_%s/%s_%s.net", main_path, argv[1], train_speeds[i], argv[1]);
        if (access(dot_net_name, F_OK) == -1)
            dot_net_files = -1;
    }
    if (dot_net_files == -1) {
        printf("Training is required, proceeding to trainging\n");
        // function call
    }

    if (dot_net_files == -1) {
        printf("after training\n");
        // add name to train parameters function
        train_parameters(&classifiers, &counts, &dimen, &n);
        memset(training_name, 0, sizeof(char) * BUFF_SIZE);
        sprintf(training_name, "%s%s_%s", main_path, activities_file, train_name);
        printf("before text file\n");
        training_file(classifiers, training_name, dimen, activities, input, 0, n, ACTIVITIES);
        for (i = 0; i < ACTIVITIES; i++) {
            memset(training_name, 0, sizeof(char) * BUFF_SIZE);
            sprintf(training_name, "%s%s_%s", main_path, train_speeds[i], train_name);
            speeds = speeds_end[i] - speeds_start[i];
            training_file(classifiers, training_name, dimen, speeds_outputs, input, speeds_start[i], speeds_end[i],
                          speeds);
        }
        //train neural_network
        train_network(train_name);
        for (i = 0; i < n; i++) {
            for (j = 0; j < dimen[i]; j++) {
                free(classifiers[i][j]);
            }
            free(classifiers[i]);
            free(counts[i]);
        }
        free(classifiers);
        free(dimen);
    }
//}
//void consumer()
//{

//    test_data(test_name2);
//     Check if file exists; -1 means file does not exists

    // consumer code
    int loops;
    int check;
    check = 0;
    loops = 0;
    int l = 0;
    const char *test_file_name;
    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
    //memset(test_file_name, 0, sizeof(char)*BUFF_SIZE);

//    while (loops < CYCLES && run_flag){
    while (loops < FILES ) {
        memset(fake_test, 0, sizeof(char)*BUFF_SIZE);
        sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[l++], user_names[0]);


        sprintf(test_file_name, "%s%s_%d.csv", main_path, test_name, loops);
        // delete next line
        test_data(main_path, test_name);

        if(access(fake_test, F_OK) == -1){
            // File does not exist
            sleep(1);
            printf("%s, sleeping\n", fake_test);
        }

        else {
            // File exists, so populate our data structure

            truth_value = test_classifiers(&test_array, fake_test, &num_of_test_classifers);
            printf("%s\t", test_file_name);
            check = remove(test_file_name);
            printf("%d\n", check);
            test_neural_network(test_array, num_of_test_classifers);
            for(i = 0; i < num_of_test_classifers; i++)
                free(test_array[i]);
            free(test_array);
        }
        loops++;
    }


}




float *zg_data = (float *) malloc(sizeof(float) * MAX_DATA);
memset(zg_data, 0, sizeof(float) * MAX_DATA);
float *temp_data = (float *) malloc(sizeof(float) * MAX_DATA);
memset(zg_data, 0, sizeof(float) * MAX_DATA);
float *widths = (float *) malloc(sizeof(float) * MAX_DATA);
memset(widths, 0, sizeof(float) * MAX_DATA);
float *norms = (float *) malloc(sizeof(float) * MAX_DATA);
memset(norms, 0, sizeof(float) * MAX_DATA);
int *indexes = (int *) malloc(sizeof(int) * MAX_DATA);
memset(indexes, 0, sizeof(int) * MAX_DATA);
int *dist = (int *) malloc(sizeof(int) * MAX_DATA);
memset(dist, 0, sizeof(int) * MAX_DATA);

int zero_count;

count_t1 = 0;
zero_count = integrate_zeros(data[6], zg_data, indexes, data_size);
//    printf("%d\n", zero_count);
count_t2 =  find_strides(zg_data, indexes, temp_data, strides_t2, dist,
                         widths, norms, 0.6, 1.2, 300, zero_count);



int find_strides(float *data, int * indexes, float *data2, int * start1, int *dist, float * norms,
                 float *widths, float w_h, float w_w, int dist_range, int n)
{
    float ave, temp;
    int cnt, i;
    cnt = 0;
    ave = 0;
    float w = w_h;
    for(i = 1; i < n; i++)
    {
        temp = data[indexes[i]-1];
        if(temp < -6000)
        {
            data2[cnt] = temp;
            start1[cnt] = indexes[i-1]-1;
            dist[cnt] = indexes[i] - indexes[i-1];
            cnt++;
            ave += temp;
        }
    }
    if(cnt > 0 && w > 0)
    {
        ave /= cnt;
//        ave *= w;
        ave *= 0.5;
    }
    int cnt2 = 0;
    float ave2 = 0;

    // Eliminate small peaks round 1
    for(i =0; i < cnt; i++)
    {
        if(data2[i] < ave)
        {
            data2[cnt2] = data2[i];
            start1[cnt2] = start1[i];
            dist[cnt2] = dist[i];
            ave2 += data2[cnt2];
            cnt2++;

        }
    }

    for(i = cnt2; i < n; i++){
        data2[i] = 0;
        dist[i] = 0;
        start1[i] = 0;
    }
    if(cnt2 > 0) {
        ave2 /= cnt2;
        ave2 *= 0.5;
    }

    cnt = 0;
    // Eliminate small peaks round 2
    for(i =0; i < cnt2; i++)
    {
        if(data2[i] < ave2)
        {
            data2[cnt] = data2[i];
            start1[cnt] = start1[i];
            dist[cnt] = dist[i];
            cnt++;

        }
    }

    for(i = cnt; i < cnt2; i++){
        data2[i] = 0;
        dist[i] = 0;
        start1[i] = 0;
    }

    // scale peaks by width
    for(i = 0; i < cnt; i++)
    {
        if(dist[i] > 0)
        {
            data2[i] /= dist[i];
        }
        else
            printf("dist was less than zero: ERROR\n");
    }

    int num, j;
    float wa;
    for(i = 0; i < cnt; i++)
    {
        num = 0;
        wa = 0;
        for(j = 0; j < 10; j++)
        {
            if(i - j < 0 || start1[i] - start1[i-j] > dist_range)
                break;
            ave += data2[i-j];
            wa += dist[i-j];
            num++;
        }
        for(j = 0; j < 10; j++)
        {
            if(i + j < cnt || start1[i+j] - start1[i] > dist_range)
                break;
            ave += data2[i+j];
            wa += dist[i+j];
            num++;
        }
        if(num > 0)
        {
            ave /= num;
            wa /= num;
        }
        widths[i] = wa;
        norms[i] = ave;
    }

    cnt2 = 0;
    for(i = 0; i < cnt; i++)
    {
        if(data2[i] < norms[i]*0.6 && dist[i] <= widths[i]*1.2)
        {
            data2[cnt2] = data2[i];
            start1[cnt2] = start1[i];
            dist[cnt2] = dist[i];
            cnt2++;

        }
    }

    for(i = cnt2; i < cnt; i++){
        data2[i] = 0;
        dist[i] = 0;
        start1[i] = 0;
    }

    return cnt2;
}


for (j = 0; j < num_strides; j++) {

        l = 0;
        classifiers[j][l++] = stride_all[j][2];
//            printf("%f\t", classifiers[j][l-1]);
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_xa[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }

        for(k = 0; k < 4; k++){
            classifiers[j][l++] = troughs_xa[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_ya[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }

        for(k = 0; k < 4; k++){
            classifiers[j][l++] = troughs_ya[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_zg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = troughs_zg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = cp_xa[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = cp_ya[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = cp_za[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = cp_xg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = cp_yg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = cp_zg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < SLOTS; k++)
        {
//        classifiers[j][l++] = mean_low_xa[j][k];
//        classifiers[j][l++] = mean_low_ya[j][k];
//        classifiers[j][l++] = mean_low_zg[j][k];
            classifiers[j][l++] = mean_high_xa[j][k];
            classifiers[j][l++] = mean_high_ya[j][k];
            classifiers[j][l++] = mean_high_zg[j][k];
        }

        classifiers[j][l++] = sigma_high_xa[j];
//            printf("%f\t", classifiers[j][l-1]);
        classifiers[j][l++] = sigma_low_xa[j];
//            printf("%f\t", classifiers[j][l-1]);

    }

for(j = 0; j < 4; j++)
    {
         free(cp_xa[j]);
         free(cp_ya[j]);
         free(cp_za[j]);
         free(cp_xg[j]);
         free(cp_yg[j]);
         free(cp_zg[j]);


    }




if(norm2[i] > 0){
            // max peak and trough are bigger than sigma and sigma not zero
            if(peak2[i][0] > norm2[i]) {
                d_p2[i][0] = w1 * peak2[i][0] / norm2[i] - 1;
            }
            else
            {
                d_p2[i][1] = 0;
            }
            if(trough2[i][0] < -norm2[i]) {
                d_t2[i][0] = -w1 * trough2[i][0] / norm2[i] - 1;
            }
            else
            {
                d_t2[i][0] = 0;
            }
            if(peak2[i][2] > norm2[i]) {
                d_p2[i][1] = w1 * peak2[i][1] / norm2[i] - 1;
            }
            else
            {
                d_p2[i][1] = 0;
            }
            if(trough2[i][2] > norm2[i]) {
                d_t2[i][1] = w1 * trough2[i][1] / norm2[i] - 1;
            }
            else
            {
                d_t2[i][1] = 0;
            }
        }
        else
        {
            d_p2[i][0] = 0;
            d_p2[i][1] = 0;
            d_p2[i][0] = 0;
            d_t2[i][0] = 0;
        }




        if(peak2[i][2] > norm2[i] * w2)
        {
             d_p2[i][2] = peak2[i][2]/peak2[i][0];
        }
        else
        {
             d_p2[i][2] = 0;
        }




        if(peak1[i][2] > peak1[i][0] * w2)
        {
            dest1[i][2] = peak1[i][2]/norm1[i];
        }
        else
        {
            dest1[i][2] = 0;
        }
        if(source2[i][2] > norm2[i] * w2)
        {
            dest2[i][2] = source2[i][2]/norm2[i];
        }
        else
        {
            dest2[i][2] = 0;
        }


    }


    for (j = num_strides - 1; j < num_strides; j++) {

        l = 0;
        classifiers[j][l++] = stride_all[j][2]/1024;    // period
        classifiers[j][l++] = sigma_all_xa[j];         // sigma total, 1 throug 5
        classifiers[j][l++] = sigma_all_ya[j];
        classifiers[j][l++] = sigma_all_za[j];
        classifiers[j][l++] = sigma_all_yg[j];
        classifiers[j][l++] = sigma_all_zg[j];
        printf("sigma all: %d\n", l-1);
        classifiers[j][l++] = mean_dev_xa[j];           // average deviation 6 throgh 10
        classifiers[j][l++] = mean_dev_ya[j];
        classifiers[j][l++] = mean_dev_za[j];
        classifiers[j][l++] = mean_dev_yg[j];
        classifiers[j][l++] = mean_dev_zg[j];
        printf("mean dev: %d\n", l-1);
        classifiers[j][l++] = skew_high_xa[j];          // skewness and kurtosis 11 trough 26
        classifiers[j][l++] = skew_high_ya[j];
        classifiers[j][l++] = skew_high_yg[j];
        classifiers[j][l++] = skew_high_zg[j];

        classifiers[j][l++] = skew_low_xa[j];
        classifiers[j][l++] = skew_low_ya[j];
        classifiers[j][l++] = skew_low_yg[j];
        classifiers[j][l++] = skew_low_zg[j];

        classifiers[j][l++] = kert_high_xa[j];
        classifiers[j][l++] = kert_high_ya[j];
        classifiers[j][l++] = kert_high_yg[j];
        classifiers[j][l++] = kert_high_zg[j];

        classifiers[j][l++] = kert_low_xa[j];
        classifiers[j][l++] = kert_low_ya[j];
        classifiers[j][l++] = kert_low_yg[j];
        classifiers[j][l++] = kert_low_zg[j];
        printf("kert: %d\n", l-1);

        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_xa[j][k];       // peak1 and peak with relative loc
            classifiers[j][l++] = peaks_ya[j][k];       // trough1 and trough2 with relative location
            classifiers[j][l++] = peaks_zg[j][k];       // 27 through 40

            classifiers[j][l++] = troughs_xa[j][k];
            classifiers[j][l++] = troughs_ya[j][k];
            classifiers[j][l++] = troughs_zg[j][k];

        }
        printf("peak trough: %d\n", l-1);
        for(k = 0; k < PARTIAL; k++)
        {
            classifiers[j][l++] = max_partial_xa[j][k];     // max values at each partial
            classifiers[j][l++] = max_partial_ya[j][k];
            classifiers[j][l++] = max_partial_zg[j][k];     // 40 through 4 * PARTIALS
            classifiers[j][l++] = min_partial_xa[j][k];
            classifiers[j][l++] = min_partial_zg[j][k];     // 40 through 76 for 6 partials
            classifiers[j][l++] = min_partial_ya[j][k];
        }
        printf("max min partial: %d\n", l-1);
        for(k = 0; k < PARTIAL; k++)
        {
            classifiers[j][l++] = max_part_cond_xa[j][k];     // value>sigma? value/sigma: 0
            classifiers[j][l++] = max_part_cond_ya[j][k];
            classifiers[j][l++] = max_part_cond_zg[j][k];     // end previous through 4 * PARTIALS
            classifiers[j][l++] = min_part_cond_xa[j][k];
            classifiers[j][l++] = min_part_cond_zg[j][k];     // 76 through 112 for 6 partials
            classifiers[j][l++] = min_part_cond_ya[j][k];
        }
        printf("mean_part cond: %d\n", l-1);

        for(k = 0; k < CONDITIONS; k++)
        {
//          [p1/sig, p2/sig, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]
            classifiers[j][l++] = cond_peak_xa[j][k];     // set of conditions some boolean some values
            classifiers[j][l++] = cond_peak_ya[j][k];     // either peak/sigma, peak/sigma -1, peak1/peak2 -1
            classifiers[j][l++] = cond_peak_zg[j][k];     // end previous through 4 * PARTIALS
            classifiers[j][l++] = cond_trough_xa[j][k];
            classifiers[j][l++] = cond_trough_yg[j][k];     // 76 through 112 for 6 partials
            classifiers[j][l++] = cond_trough_zg[j][k];
        }
        printf("conditions: %d\n", l-1);
        for(k = 0; k < 4; k++)
        {
            classifiers[j][l++] = ci_xa[j][k];              // value and location of max peak and troughs integrals
            classifiers[j][l++] = ci_ya[j][k];
            classifiers[j][l++] = ci_xa[j][k];
            classifiers[j][l++] = ci_ya[j][k];

        }
        printf("ci: %d\n", l-1);
        for(k = 0; k < SLOTS; k++)
        {
            classifiers[j][l++] = mean_slot_xa[j][k];
            classifiers[j][l++] = mean_slot_ya[j][k];
            classifiers[j][l++] = mean_slot_zg[j][k];
        }
        printf("slots: %d\n", l-1);


        for(k = PARTIAL; k < PARTIAL; k++)
        {
            classifiers[j][l++] = cp_xa[j][k];
            classifiers[j][l++] = cp_ya[j][k];
            classifiers[j][l++] = cp_xa[j][k];
            classifiers[j][l++] = cp_ya[j][k];

            classifiers[j][l++] = ct_yg[j][k];
            classifiers[j][l++] = ct_zg[j][k];
            classifiers[j][l++] = ct_yg[j][k];
            classifiers[j][l++] = ct_zg[j][k];
        }
    }