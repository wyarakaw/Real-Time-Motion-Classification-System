//
// Created by Amaael Antonini on 4/12/17.
//

#include <stdio.h>
#include <mraa/i2c.h>
#include <signal.h>
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "LSM9DS0.h"
#include "constants.h"


void test_data(char *test_name)
{
    sig_atomic_t volatile run_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            run_flag = 0;
    }
    char* file_name;
    file_name = (char*)malloc(BUFF_SIZE*sizeof(char));
    memset(file_name, 0, BUFF_SIZE);
    int i = 0;
    int stop = 0;
    int file_number;
    file_number = 0;
    FILE *fp;

    double time_start, time_wait, time_ref;
    struct timeval time_before, time_after;

    mraa_i2c_context accel, gyro, mag;
    float a_res, g_res, m_res;
    accel_scale_t a_scale;
    gyro_scale_t g_scale;
    mag_scale_t m_scale;
    data_t ad, gd, md;


    a_scale = A_SCALE_16G;
    g_scale = G_SCALE_500DPS;
    m_scale = M_SCALE_8GS;

    accel = accel_init();
    set_accel_ODR(accel, A_ODR_100);
    set_accel_scale(accel, a_scale);
    a_res = calc_accel_res(a_scale);

    gyro = gyro_init();
    set_gyro_ODR(gyro, G_ODR_190_BW_70);
    set_gyro_scale(gyro, g_scale);
    g_res = calc_gyro_res(g_scale);
    mag = mag_init();
    set_mag_ODR(mag, M_ODR_125);
    set_mag_scale(mag, m_scale);
    m_res = calc_mag_res(m_scale);
//    while(stop < MAX_FILES && run_flag) {
//        stop++;
//        while(time_wait < RUN && run_flag)
//        {
//
//            ad = read_accel(accel, a_res);
//            gd = read_gyro(gyro, g_res);
//            gettimeofday(&time_before, NULL);
//            time_wait = time_before.tv_sec - time_ref;
//            time_start = time_before.tv_sec + time_before.tv_usec / NORM;
//
//            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);
//
//            usleep(100);
//        }
//
//        fclose(fp);
//        file_number++;
//    }
    while(stop < MAX_FILES && run_flag) {
        time_wait = 0;
        stop++;
        sprintf(file_name, "%s%s_%d.csv", main_path, test_name, file_number);
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
        while(time_wait < TEST_SIZE && run_flag)
        {
            ad = read_accel(accel, a_res);
            gd = read_gyro(gyro, g_res);
            gettimeofday(&time_before, NULL);
            time_start = time_before.tv_sec + time_before.tv_usec / NORM;
            time_wait += (int) (time_start - time_ref);
            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

            usleep(100);
        }

        fclose(fp);
//        printf("\n");
        file_number++;
    }
}








