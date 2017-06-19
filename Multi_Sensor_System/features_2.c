#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"




void features(float **data, float **mean, float **sigma, int *strides, int stride_count,
              float **classifiers, float **turning, float **speeds, float **turn_speeds, int *rows)
{
//    printf("features\n");
    int data_size = (*rows);
    float k_top, k_ratio;
    k_top = 10;
    float weight_cond = 20;
    k_ratio = 1;
    float max = 0;
    int i, j, k, l, cnt;
    float sigma_low_xa[STRIDES];
    float sigma_low_ya[STRIDES];
    float sigma_low_za[STRIDES];
    float sigma_low_xg[STRIDES];
    float sigma_low_yg[STRIDES];
    float sigma_low_zg[STRIDES];

    float sigma_high_xa[STRIDES];
    float sigma_high_ya[STRIDES];
    float sigma_high_za[STRIDES];
    float sigma_high_xg[STRIDES];
    float sigma_high_yg[STRIDES];
    float sigma_high_zg[STRIDES];

    float sigma_all_xa[STRIDES];
    float sigma_all_ya[STRIDES];
    float sigma_all_za[STRIDES];
    float sigma_all_xg[STRIDES];
    float sigma_all_yg[STRIDES];
    float sigma_all_zg[STRIDES];

    float cp_xa[STRIDES][PARTIAL], ct_xa[STRIDES][PARTIAL];
    float cp_ya[STRIDES][PARTIAL], ct_ya[STRIDES][PARTIAL];
    float cp_za[STRIDES][PARTIAL], ct_za[STRIDES][PARTIAL];
    float cp_xg[STRIDES][PARTIAL], ct_xg[STRIDES][PARTIAL];
    float cp_yg[STRIDES][PARTIAL], ct_yg[STRIDES][PARTIAL];
    float cp_zg[STRIDES][PARTIAL], ct_zg[STRIDES][PARTIAL];

    float ci_xa[STRIDES][4];
    float ci_ya[STRIDES][4];
    float ci_za[STRIDES][4];
    float ci_xg[STRIDES][4];
    float ci_yg[STRIDES][4];
    float ci_zg[STRIDES][4];


    float cummulative[7][MAX_DATA];
    int indexes[7][MAX_DATA];

    float mean_stride_xa[2][STRIDES];
    float mean_stride_ya[2][STRIDES];
    float mean_stride_za[2][STRIDES];
    float mean_stride_xg[2][STRIDES];
    float mean_stride_yg[2][STRIDES];
    float mean_stride_zg[2][STRIDES];
    float mean_comb_a[2][STRIDES];
    float mean_comb_g[2][STRIDES];


    float max_partial_xa[STRIDES][PARTIAL];
    float max_partial_ya[STRIDES][PARTIAL];
    float max_partial_za[STRIDES][PARTIAL];
    float max_partial_xg[STRIDES][PARTIAL];
    float max_partial_yg[STRIDES][PARTIAL];
    float max_partial_zg[STRIDES][PARTIAL];
    float min_partial_xa[STRIDES][PARTIAL];
    float min_partial_ya[STRIDES][PARTIAL];
    float min_partial_za[STRIDES][PARTIAL];
    float min_partial_xg[STRIDES][PARTIAL];
    float min_partial_yg[STRIDES][PARTIAL];
    float min_partial_zg[STRIDES][PARTIAL];

    float max_part_cond_xa[STRIDES][PARTIAL];
    float max_part_cond_ya[STRIDES][PARTIAL];
    float max_part_cond_za[STRIDES][PARTIAL];
    float max_part_cond_xg[STRIDES][PARTIAL];
    float max_part_cond_yg[STRIDES][PARTIAL];
    float max_part_cond_zg[STRIDES][PARTIAL];
    float min_part_cond_xa[STRIDES][PARTIAL];
    float min_part_cond_ya[STRIDES][PARTIAL];
    float min_part_cond_za[STRIDES][PARTIAL];
    float min_part_cond_xg[STRIDES][PARTIAL];
    float min_part_cond_yg[STRIDES][PARTIAL];
    float min_part_cond_zg[STRIDES][PARTIAL];


    float troughs_xa[STRIDES][4], peaks_xa[STRIDES][4];
    float troughs_ya[STRIDES][4], peaks_ya[STRIDES][4];
    float troughs_za[STRIDES][4], peaks_za[STRIDES][4];
    float troughs_xg[STRIDES][4], peaks_xg[STRIDES][4];
    float troughs_yg[STRIDES][4], peaks_yg[STRIDES][4];
    float troughs_zg[STRIDES][4], peaks_zg[STRIDES][4];


    float mean_low_xa[STRIDES], mean_high_xa[STRIDES];
    float mean_low_ya[STRIDES], mean_high_ya[STRIDES];
    float mean_low_za[STRIDES], mean_high_za[STRIDES];
    float mean_low_xg[STRIDES], mean_high_xg[STRIDES];
    float mean_low_yg[STRIDES], mean_high_yg[STRIDES];
    float mean_low_zg[STRIDES], mean_high_zg[STRIDES];

    float mean_slot_xa[STRIDES][SLOTS];
    float mean_slot_ya[STRIDES][SLOTS];
    float mean_slot_za[STRIDES][SLOTS];
    float mean_slot_xg[STRIDES][SLOTS];
    float mean_slot_yg[STRIDES][SLOTS];
    float mean_slot_zg[STRIDES][SLOTS];
    float mean_dummy[STRIDES][SLOTS];

    float skew_high_xa[STRIDES], skew_low_xa[STRIDES], kert_high_xa[STRIDES], kert_low_xa[STRIDES];
    float skew_high_ya[STRIDES], skew_low_ya[STRIDES], kert_high_ya[STRIDES], kert_low_ya[STRIDES];
    float skew_high_za[STRIDES], skew_low_za[STRIDES], kert_high_za[STRIDES], kert_low_za[STRIDES];
    float skew_high_xg[STRIDES], skew_low_xg[STRIDES], kert_high_xg[STRIDES], kert_low_xg[STRIDES];
    float skew_high_yg[STRIDES], skew_low_yg[STRIDES], kert_high_yg[STRIDES], kert_low_yg[STRIDES];
    float skew_high_zg[STRIDES], skew_low_zg[STRIDES], kert_high_zg[STRIDES], kert_low_zg[STRIDES];

    float mean_dev_xa[STRIDES], cond_peak_xa[STRIDES][CONDITIONS], cond_trough_xa[STRIDES][CONDITIONS];
    float mean_dev_ya[STRIDES], cond_peak_ya[STRIDES][CONDITIONS], cond_trough_ya[STRIDES][CONDITIONS];
    float mean_dev_za[STRIDES], cond_peak_za[STRIDES][CONDITIONS], cond_trough_za[STRIDES][CONDITIONS];
    float mean_dev_xg[STRIDES], cond_peak_xg[STRIDES][CONDITIONS], cond_trough_xg[STRIDES][CONDITIONS];
    float mean_dev_yg[STRIDES], cond_peak_yg[STRIDES][CONDITIONS], cond_trough_yg[STRIDES][CONDITIONS];
    float mean_dev_zg[STRIDES], cond_peak_zg[STRIDES][CONDITIONS], cond_trough_zg[STRIDES][CONDITIONS];

    float tm_low[STRIDES];
    float tm_high[STRIDES];
    float fm_low[STRIDES];
    float fm_high[STRIDES];
    int stride_all[STRIDES][4];
    int num_strides;
    num_strides = 0;

//    printf("after malloc\n");
    cnt = 0;
//    float ave_p = 0;
    for(j = 1; j < stride_count; j++)
    {
        if(data[0][strides[j]] - data[0][strides[j-1]] < 4)
        {
            stride_all[cnt][0] = strides[j-1];
            stride_all[cnt][1] = strides[j];
            stride_all[cnt][2] = strides[j]-strides[j-1];
            if(stride_all[cnt][2] > 1000)
                stride_all[cnt][2] = 1000;
//            ave_p +=  stride_all[cnt][2];
//            printf("%d\t", stride_all[cnt][2]);
            num_strides++;
            cnt++;
        }
//        printf("\n");

    }

    for(j = 0; j < num_strides-1; j++)
    {
        if(stride_all[j][1] == stride_all[j+1][0])
        {
            stride_all[j][3] = 1;
        }
        else
        {
            stride_all[j][3] = -1;
        }
    }


    for(i = 4; i < 7; i++)
    {
        for(j = 0; j < data_size; j++)
        {
            data[i][j] /= 256;
        }
    }

    sigma_stride_total(data[1], stride_all, sigma_all_xa, num_strides);
    sigma_stride_total(data[2], stride_all, sigma_all_ya, num_strides);
    sigma_stride_total(data[3], stride_all, sigma_all_za, num_strides);
    sigma_stride_total(data[4], stride_all, sigma_all_xg, num_strides);
    sigma_stride_total(data[5], stride_all, sigma_all_yg, num_strides);
    sigma_stride_total(data[6], stride_all, sigma_all_zg, num_strides);


//        printf("before peaks\n");
    peaks(data[1], stride_all, peaks_xa, troughs_xa, num_strides);
    peaks(data[2], stride_all, peaks_ya, troughs_ya, num_strides);
    peaks(data[3], stride_all, peaks_za, troughs_za, num_strides);
    peaks(data[4], stride_all, peaks_xg, troughs_xg, num_strides);
    peaks(data[5], stride_all, peaks_yg, troughs_yg, num_strides);
    peaks(data[6], stride_all, peaks_zg, troughs_zg, num_strides);

    time_slots(data[0], data[1], stride_all, mean_slot_xa, num_strides);
    time_slots(data[0], data[2], stride_all, mean_slot_ya, num_strides);
    time_slots(data[0], data[3], stride_all, mean_slot_za, num_strides);
    time_slots(data[0], data[4], stride_all, mean_slot_xg, num_strides);
    time_slots(data[0], data[5], stride_all, mean_slot_yg, num_strides);
    time_slots(data[0], data[6], stride_all, mean_slot_zg, num_strides);


    kurt_skew(data[1], stride_all, sigma_low_xa, sigma_high_xa, tm_low, tm_high,
              fm_low, fm_high, mean_high_xa, mean_low_xa, skew_high_xa, skew_low_xa,
              kert_high_xa, kert_low_xa, 0.00, &max, k_top, k_ratio, num_strides);

    kurt_skew(data[2], stride_all, sigma_low_ya, sigma_high_ya, tm_low, tm_high,
              fm_low, fm_high, mean_high_ya, mean_low_ya, skew_high_ya, skew_low_ya,
              kert_high_ya, kert_low_ya, 0.00, &max, k_top, k_ratio, num_strides);

    kurt_skew(data[3], stride_all, sigma_low_za, sigma_high_za, tm_low, tm_high,
              fm_low, fm_high, mean_high_za, mean_low_za, skew_high_za, skew_low_za,
              kert_high_za, kert_low_za, 0.00, &max, k_top, k_ratio, num_strides);

    kurt_skew(data[4], stride_all, sigma_low_xg, sigma_high_xg, tm_low, tm_high,
              fm_low, fm_high, mean_high_xg, mean_low_xg, skew_high_xg, skew_low_xg,
              kert_high_xg, kert_low_xg, 0.00, &max, k_top, k_ratio, num_strides);

    kurt_skew(data[5], stride_all, sigma_low_yg, sigma_high_yg, tm_low, tm_high,
              fm_low, fm_high, mean_high_yg, mean_low_yg, skew_high_yg, skew_low_yg,
              kert_high_yg, kert_low_yg, 0.00, &max, k_top, k_ratio, num_strides);

    kurt_skew(data[6], stride_all, sigma_low_zg, sigma_high_zg, tm_low, tm_high,
              fm_low, fm_high, mean_high_zg, mean_low_zg, skew_high_zg, skew_low_zg,
              kert_high_zg, kert_low_zg, 0.00, &max, k_top, k_ratio, num_strides);

//    printf("total maximum kurt, skew dev: %f\n", max);
    max = 0;
    for(i = 0; i < num_strides; i++)
    {
        mean_comb_a[0][i] = mean_stride_xa[0][i];
        mean_comb_a[0][i] += mean_stride_ya[0][i];
        mean_comb_a[1][i] = mean_stride_xa[1][i];
        mean_comb_a[1][i] += mean_stride_ya[1][i];
        mean_comb_a[0][i] /= 2;
        mean_comb_a[1][i] /= 2;
    }

    for(i = 0; i < 6; i++)
    {
        integrate_zeros(data[i+1], cummulative[i+1], indexes[i], data_size);
    }

    max_point(cummulative[1],  stride_all, indexes[1], ci_xa, num_strides, 256);
    max_point(cummulative[2],  stride_all, indexes[2], ci_ya, num_strides, 256);
    max_point(cummulative[3],  stride_all, indexes[3], ci_za, num_strides, 256);
    max_point(cummulative[4],  stride_all, indexes[4], ci_xg, num_strides, 256);
    max_point(cummulative[5],  stride_all, indexes[5], ci_yg, num_strides, 256);
    max_point(cummulative[6],  stride_all, indexes[6], ci_zg, num_strides, 256);

    for(i = num_strides; i < num_strides; i++)
    {
        for(j = 0; j < 4; j++)
        {
            printf("%f\t%f\t%f\t%f\t%f\t%f\t%d\n",
                   ci_xa[i][j],
                   ci_ya[i][j],
                   ci_za[i][j],
                   ci_zg[i][j],
                   ci_yg[i][j],
                   ci_zg[i][j], j);
        }
    }

    max_slot(data[1], stride_all, min_partial_xa, max_partial_xa, 1, PARTIAL, num_strides);
    max_slot(data[2], stride_all, min_partial_ya, max_partial_ya, 1, PARTIAL, num_strides);
    max_slot(data[3], stride_all, min_partial_za, max_partial_za, 1, PARTIAL, num_strides);
    max_slot(data[4], stride_all, min_partial_xg, max_partial_xg, 1, PARTIAL, num_strides);
    max_slot(data[5], stride_all, min_partial_yg, max_partial_yg, 1, PARTIAL, num_strides);
    max_slot(data[6], stride_all, min_partial_zg, max_partial_zg, 1, PARTIAL, num_strides);


    max_slot(cummulative[1], stride_all, cp_xa, ct_xa, 1, PARTIAL, num_strides);
    max_slot(cummulative[2], stride_all, cp_ya, ct_ya, 1, PARTIAL, num_strides);
    max_slot(cummulative[3], stride_all, cp_za, ct_za, 1, PARTIAL, num_strides);
    max_slot(cummulative[4], stride_all, cp_xg, ct_xg, 1, PARTIAL, num_strides);
    max_slot(cummulative[5], stride_all, cp_yg, ct_yg, 1, PARTIAL, num_strides);
    max_slot(cummulative[6], stride_all, cp_zg, ct_zg, 1, PARTIAL, num_strides);


    sigma_condition(max_partial_xa, sigma_all_xa, max_part_cond_xa, 1, num_strides);
    sigma_condition(max_partial_ya, sigma_all_ya, max_part_cond_ya, 1, num_strides);
    sigma_condition(max_partial_za, sigma_all_za, max_part_cond_za, 1, num_strides);
    sigma_condition(max_partial_xg, sigma_all_xg, max_part_cond_xg, 1, num_strides);
    sigma_condition(max_partial_yg, sigma_all_yg, max_part_cond_yg, 1, num_strides);
    sigma_condition(max_partial_zg, sigma_all_zg, max_part_cond_zg, 1, num_strides);
    for(i = PRINT; i < num_strides; i++)
    {
        for(j = 0; j < PARTIAL; j++)
        {
            printf("%f\t%f\t%f\t%f\t%f\t%f\n",
                   max_part_cond_xa[i][j],
                   max_part_cond_ya[i][j],
                   max_part_cond_za[i][j],
                   max_part_cond_zg[i][j],
                   max_part_cond_yg[i][j],
                   max_part_cond_zg[i][j]);
        }
    }

    sigma_condition(min_partial_xa, sigma_all_xa, min_part_cond_xa, -1, num_strides);
    sigma_condition(min_partial_ya, sigma_all_ya, min_part_cond_ya, -1, num_strides);
    sigma_condition(min_partial_za, sigma_all_za, min_part_cond_za, -1, num_strides);
    sigma_condition(min_partial_xg, sigma_all_xg, min_part_cond_xg, -1, num_strides);
    sigma_condition(min_partial_yg, sigma_all_yg, min_part_cond_yg, -1, num_strides);
    sigma_condition(min_partial_zg, sigma_all_zg, min_part_cond_zg, -1, num_strides);
    for(i = PRINT; i < num_strides; i++)
    {
        for(j = 0; j < PARTIAL; j++)
        {
            printf("%f\t%f\t%f\t%f\t%f\t%f\n",
                   min_part_cond_xa[i][j],
                   min_part_cond_ya[i][j],
                   min_part_cond_za[i][j],
                   min_part_cond_zg[i][j],
                   min_part_cond_yg[i][j],
                   min_part_cond_zg[i][j]);
        }
    }
//    partial x grater than sigma? partial x/sigma: 0

//    peak_trough_compare(int strides[][4], float peak1[][4], float peak2[][4], float trough1[][4], float trough2[][4],
//                             float *norm1, float d_p1[][CONDITIONS], float d_t1[][CONDITIONS],
//                             float w1, float w2, float w3, int n)
// [p1/sig, p2/sig, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]

    peak_trough_compare(stride_all, peaks_xa, peaks_ya, troughs_xa, troughs_ya,
                        sigma_all_xa, cond_peak_xa, cond_trough_xa, 1, 1, weight_cond, num_strides);

    peak_trough_compare(stride_all, peaks_ya, peaks_xa, troughs_ya, troughs_xa,
                        sigma_all_ya, cond_peak_ya, cond_trough_ya, 1, 1, weight_cond, num_strides);

    peak_trough_compare(stride_all, peaks_za, peaks_ya, troughs_za, troughs_ya,
                        sigma_all_xa, cond_peak_za, cond_trough_za, 1, 1, weight_cond, num_strides);

    peak_trough_compare(stride_all, peaks_xg, peaks_zg, troughs_xg, troughs_zg,
                        sigma_all_xa, cond_peak_xg, cond_trough_xg, 1, 1, weight_cond, num_strides);

    peak_trough_compare(stride_all, peaks_zg, peaks_xg, troughs_zg, troughs_xg,
                        sigma_all_xa, cond_peak_zg, cond_trough_zg, 1, 1, weight_cond, num_strides);

    // done classsifiers;

    average_deviation(data[1], stride_all, mean_dev_xa, &max, num_strides);
    average_deviation(data[2], stride_all, mean_dev_ya, &max, num_strides);
    average_deviation(data[3], stride_all, mean_dev_za, &max, num_strides);
    average_deviation(data[4], stride_all, mean_dev_xg, &max, num_strides);
    average_deviation(data[5], stride_all, mean_dev_yg, &max, num_strides);
    average_deviation(data[6], stride_all, mean_dev_zg, &max, num_strides);

//    printf("total maximum average dev: %f\n", max);


    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
        classifiers[j][l++] = (float) stride_all[j][2] / 1024;    // period

        classifiers[j][l++] = sigma_all_xa[j] / 5;         // sigma total, 1 throug 5
        classifiers[j][l++] = sigma_all_ya[j] / 5;
        classifiers[j][l++] = sigma_all_za[j] / 5;
        classifiers[j][l++] = sigma_all_yg[j] / 5;
        classifiers[j][l++] = sigma_all_zg[j] / 5;

        classifiers[j][l++] = mean_dev_xa[j] / 5;           // average deviation 6 throgh 10
        classifiers[j][l++] = mean_dev_ya[j] / 5;
        classifiers[j][l++] = mean_dev_za[j] / 5;
        classifiers[j][l++] = mean_dev_yg[j] / 5;
        classifiers[j][l++] = mean_dev_zg[j] / 5;

        classifiers[j][l++] = skew_high_xa[j] / 20;          // skewness and kurtosis 11 trough 26
        classifiers[j][l++] = skew_high_ya[j] / 20;
        classifiers[j][l++] = skew_high_yg[j] / 20;
        classifiers[j][l++] = skew_high_zg[j] / 20;
//
        classifiers[j][l++] = skew_low_xa[j] / 20;
        classifiers[j][l++] = skew_low_ya[j] / 20;
        classifiers[j][l++] = skew_low_yg[j] / 20;
        classifiers[j][l++] = skew_low_zg[j] / 20;
//
        classifiers[j][l++] = kert_high_xa[j] / 20;
        classifiers[j][l++] = kert_high_ya[j] / 20;
        classifiers[j][l++] = kert_high_yg[j] / 20;
        classifiers[j][l++] = kert_high_zg[j] / 20;

        classifiers[j][l++] = kert_low_xa[j] / 20;
        classifiers[j][l++] = kert_low_ya[j] / 20;
        classifiers[j][l++] = kert_low_yg[j] / 20;
        classifiers[j][l++] = kert_low_zg[j] / 20;

        for (k = 0; k < 4; k++) {
            classifiers[j][l++] = peaks_xa[j][k] / 20;       // peak1 and peak with relative loc
            classifiers[j][l++] = peaks_ya[j][k] / 20;       // trough1 and trough2 with relative location
            classifiers[j][l++] = peaks_zg[j][k] / 20;       // 27 through 40

            classifiers[j][l++] = troughs_xa[j][k] / 20;
            classifiers[j][l++] = troughs_ya[j][k] / 20;
            classifiers[j][l++] = troughs_zg[j][k] / 20;

        }

        for (k = 0; k < PARTIAL; k++) {
            classifiers[j][l++] = max_partial_xa[j][k] / 40;     // max values at each partial
            classifiers[j][l++] = max_partial_ya[j][k] / 40;
            classifiers[j][l++] = max_partial_zg[j][k] / 40;     // 40 through 4 * PARTIALS
            classifiers[j][l++] = min_partial_xa[j][k] / 40;
            classifiers[j][l++] = min_partial_zg[j][k] / 40;     // 40 through 76 for 6 partials
            classifiers[j][l++] = min_partial_ya[j][k] / 40;
        }

        for (k = 0; k < PARTIAL; k++) {
            classifiers[j][l++] = max_part_cond_xa[j][k] / 40;     // value>sigma? value/sigma: 0
            classifiers[j][l++] = max_part_cond_ya[j][k] / 40;
            classifiers[j][l++] = max_part_cond_zg[j][k] / 40;     // end previous through 4 * PARTIALS
            classifiers[j][l++] = min_part_cond_xa[j][k] / 40;
            classifiers[j][l++] = min_part_cond_zg[j][k] / 40;     // 76 through 112 for 6 partials
            classifiers[j][l++] = min_part_cond_ya[j][k] / 40;
        }
//        printf("%d\n", l) ;

        for (k = 0; k < CONDITIONS; k++) {
//          [p1/sig, p2/sig, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]
            classifiers[j][l++] = cond_peak_xa[j][k];     // set of conditions some boolean some values
            classifiers[j][l++] = cond_peak_ya[j][k];     // either peak/sigma, peak/sigma -1, peak1/peak2 -1
            classifiers[j][l++] = cond_peak_zg[j][k];     // end previous through 4 * PARTIALS
            classifiers[j][l++] = cond_trough_xa[j][k];
            classifiers[j][l++] = cond_trough_yg[j][k];     // 76 through 112 for 6 partials
            classifiers[j][l++] = cond_trough_zg[j][k];
        }
//        printf("%d\n", l) ;

        for (k = 0; k < 4; k++) {
            classifiers[j][l++] = ci_xa[j][k];              // value and location of max peak and troughs integrals
            classifiers[j][l++] = ci_ya[j][k];
            classifiers[j][l++] = ci_xa[j][k];
            classifiers[j][l++] = ci_ya[j][k];

        }
//        printf("%d\n", l) ;
        for (k = 0; k < SLOTS; k++) {
            classifiers[j][l++] = mean_slot_xa[j][k] / 20;
            classifiers[j][l++] = mean_slot_ya[j][k] / 20;
            classifiers[j][l++] = mean_slot_zg[j][k] / 20;
        }


        for (k = PARTIAL; k < PARTIAL; k++) {
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
    if(PRINT_FEATURES)
        printf("features: %d\t", l);
    max = 0;
    float min = 0;
    int max_l, min_l;
    max_l = min_l = 0;

    for(i = 0; i < num_strides; i++)
    {
        for(j = 0; j < l; j++)
        {
            if(classifiers[i][j] > max)
            {
                max = classifiers[i][j];
                max_l = j;
            }
            if(classifiers[i][j] < min)
            {
                min = classifiers[i][j];
                min_l = j;
            }
        }
    }
//    if(max > 1 || min < -1)
//        printf("activities: max: %f at: %d, min: %f at: %d\n", max, max_l, min, min_l);

    k = 0;
    for(i = PRINT; i < num_strides && k < 10; i+=3)
    {
        k++;
        for(j = 0; j < l; j++)
            printf("j: %d\t%f\t", j, classifiers[i][j]);
        printf("\n");

    }

    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
//        speeds[j][l++] = (float) stride_all[j][2]/1024;    // period

        speeds[j][l++] = sigma_all_xa[j]/5;         // sigma total, 1 throug 5
        speeds[j][l++] = sigma_all_ya[j]/5;
        speeds[j][l++] = sigma_all_za[j]/5;
        speeds[j][l++] = sigma_all_yg[j]/5;
        speeds[j][l++] = sigma_all_zg[j]/5;

        speeds[j][l++] = mean_dev_xa[j]/5;           // average deviation 6 throgh 10
        speeds[j][l++] = mean_dev_ya[j]/5;
        speeds[j][l++] = mean_dev_za[j]/5;
        speeds[j][l++] = mean_dev_yg[j]/5;
        speeds[j][l++] = mean_dev_zg[j]/5;

//        speeds[j][l++] = skew_high_xa[j] /20;          // skewness and kurtosis 11 trough 26
//        speeds[j][l++] = skew_high_ya[j] /20;
//        speeds[j][l++] = skew_high_yg[j] /20;
//        speeds[j][l++] = skew_high_zg[j] /20;
////
//        speeds[j][l++] = skew_low_xa[j] /20;
//        speeds[j][l++] = skew_low_ya[j] /20;
//        speeds[j][l++] = skew_low_yg[j] /20;
//        speeds[j][l++] = skew_low_zg[j] /20;
////
//        speeds[j][l++] = kert_high_xa[j] /20;
//        speeds[j][l++] = kert_high_ya[j] /20;
//        speeds[j][l++] = kert_high_yg[j] /20;
//        speeds[j][l++] = kert_high_zg[j] /20;
//
//        speeds[j][l++] = kert_low_xa[j] /20;
//        speeds[j][l++] = kert_low_ya[j] /20;
//        speeds[j][l++] = kert_low_yg[j] /20;
//        speeds[j][l++] = kert_low_zg[j] /20;

        for(k = 0; k < 4; k++){
            speeds[j][l++] = peaks_xa[j][k]/20;       // peak1 and peak with relative loc
            speeds[j][l++] = peaks_ya[j][k]/20;       // trough1 and trough2 with relative location
            speeds[j][l++] = peaks_zg[j][k]/20;       // 27 through 40

            speeds[j][l++] = troughs_xa[j][k]/20;
            speeds[j][l++] = troughs_ya[j][k]/20;
            speeds[j][l++] = troughs_zg[j][k]/20;

        }

        for(k = PARTIAL; k < PARTIAL; k++)
        {
            speeds[j][l++] = max_partial_xa[j][k] /40;     // max values at each partial
            speeds[j][l++] = max_partial_ya[j][k] /40;
            speeds[j][l++] = max_partial_zg[j][k] /40;     // 40 through 4 * PARTIALS
            speeds[j][l++] = min_partial_xa[j][k] /40;
            speeds[j][l++] = min_partial_zg[j][k] /40;     // 40 through 76 for 6 partials
            speeds[j][l++] = min_partial_ya[j][k] /40;
        }

        for(k = PARTIAL; k < PARTIAL; k++)
        {
            speeds[j][l++] = max_part_cond_xa[j][k] / 40;     // value>sigma? value/sigma: 0
            speeds[j][l++] = max_part_cond_ya[j][k] / 40;
            speeds[j][l++] = max_part_cond_zg[j][k] / 40;     // end previous through 4 * PARTIALS
            speeds[j][l++] = min_part_cond_xa[j][k] / 40;
            speeds[j][l++] = min_part_cond_zg[j][k] / 40;     // 76 through 112 for 6 partials
            speeds[j][l++] = min_part_cond_ya[j][k] / 40;
        }
//        printf("%d\n", l) ;

        for(k = CONDITIONS; k < CONDITIONS; k++)
        {
//          [p1/sig, p2/sig, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]
            speeds[j][l++] = cond_peak_xa[j][k];     // set of conditions some boolean some values
            speeds[j][l++] = cond_peak_ya[j][k];     // either peak/sigma, peak/sigma -1, peak1/peak2 -1
            speeds[j][l++] = cond_peak_zg[j][k];     // end previous through 4 * PARTIALS
            speeds[j][l++] = cond_trough_xa[j][k];
            speeds[j][l++] = cond_trough_yg[j][k];     // 76 through 112 for 6 partials
            speeds[j][l++] = cond_trough_zg[j][k];
        }
//        printf("%d\n", l) ;

        for(k = 0; k < 4; k+=2)
        {
            speeds[j][l++] = ci_xa[j][k];              // value and location of max peak and troughs integrals
            speeds[j][l++] = ci_ya[j][k];
            speeds[j][l++] = ci_xa[j][k];
            speeds[j][l++] = ci_ya[j][k];

        }
//        printf("%d\n", l) ;
        for(k = 0; k < SLOTS; k++)
        {
            speeds[j][l++] = mean_slot_xa[j][k] /20;
            speeds[j][l++] = mean_slot_ya[j][k] /20;
            speeds[j][l++] = mean_slot_zg[j][k] /20;
        }


        for(k = PARTIAL; k < PARTIAL; k++)
        {
            speeds[j][l++] = cp_xa[j][k];
            speeds[j][l++] = cp_ya[j][k];
            speeds[j][l++] = cp_xa[j][k];
            speeds[j][l++] = cp_ya[j][k];

            speeds[j][l++] = ct_yg[j][k];
            speeds[j][l++] = ct_zg[j][k];
            speeds[j][l++] = ct_yg[j][k];
            speeds[j][l++] = ct_zg[j][k];
        }
    }

     if(PRINT_FEATURES)
           printf("speed: %d\t", l);



    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
        turning[j][l++] = stride_all[j][2]/1024;    // period


        turning[j][l++] = sigma_all_ya[j]/5;          // sigma total, 1 throug 5
        turning[j][l++] = sigma_all_za[j]/5;
        turning[j][l++] = sigma_all_zg[j]/5;
        turning[j][l++] = sigma_all_yg[j]/5;

        turning[j][l++] = sigma_high_ya[j]/5;
        turning[j][l++] = sigma_high_za[j]/5;
        turning[j][l++] = sigma_high_zg[j]/5;
        turning[j][l++] = sigma_high_yg[j]/5;

        turning[j][l++] = sigma_low_ya[j]/5;
        turning[j][l++] = sigma_low_za[j]/5;
        turning[j][l++] = sigma_low_zg[j]/5;
        turning[j][l++] = sigma_low_yg[j]/5;

        turning[j][l++] = mean_dev_ya[j]/5;
        turning[j][l++] = mean_dev_za[j]/5;           // average deviation 6 throgh 10
        turning[j][l++] = mean_dev_xg[j]/5;
        turning[j][l++] = mean_dev_yg[j]/5;

        turning[j][l++] = mean_high_ya[j]/5;
        turning[j][l++] = mean_high_za[j]/5;
        turning[j][l++] = mean_high_xg[j]/5;
        turning[j][l++] = mean_high_yg[j]/5;

        turning[j][l++] = mean_low_ya[j]/5;
        turning[j][l++] = mean_low_za[j]/5;
        turning[j][l++] = mean_low_xg[j]/5;
        turning[j][l++] = mean_low_yg[j]/5;

//        turning[j][l++] = skew_high_xa[j];          // skewness and kurtosis 11 trough 26
        turning[j][l++] = skew_high_ya[j] /20;
        turning[j][l++] = skew_high_za[j] /20;
        turning[j][l++] = skew_high_xg[j] /20;
        turning[j][l++] = skew_high_yg[j] /20;

        turning[j][l++] = skew_low_ya[j] /20;
        turning[j][l++] = skew_low_za[j] /20;
        turning[j][l++] = skew_low_xg[j] /20;
        turning[j][l++] = skew_low_yg[j] /20;

        turning[j][l++] = kert_high_ya[j] /20;
        turning[j][l++] = kert_high_za[j] /20;
        turning[j][l++] = kert_high_xg[j] /20;
        turning[j][l++] = kert_high_yg[j] /20;

        turning[j][l++] = kert_low_ya[j] /20;
        turning[j][l++] = kert_low_za[j] /20;
        turning[j][l++] = kert_low_xg[j] /20;
        turning[j][l++] = kert_low_yg[j] /20;

        for(k = 0; k < 4; k++){
            turning[j][l++] = peaks_ya[j][k]/20;       // peak1 and peak with relative loc
            turning[j][l++] = peaks_za[j][k]/20;       // trough1 and trough2 with relative location
            turning[j][l++] = peaks_xg[j][k]/20;       // 27 through 40
            turning[j][l++] = peaks_yg[j][k]/20;

            turning[j][l++] = troughs_ya[j][k]/20;
            turning[j][l++] = troughs_za[j][k]/20;
            turning[j][l++] = troughs_xg[j][k]/20;
            turning[j][l++] = troughs_yg[j][k]/20;

        }

        for(k = 0; k < PARTIAL; k++)
        {
//            turning[j][l++] = max_partial_xa[j][k];     // max values at each partial
            turning[j][l++] = max_partial_ya[j][k]/20;
            turning[j][l++] = max_partial_za[j][k]/20;     // 40 through 4 * PARTIALS
            turning[j][l++] = max_partial_xg[j][k]/20;
            turning[j][l++] = max_partial_yg[j][k]/20;


            turning[j][l++] = min_partial_ya[j][k]/20;
            turning[j][l++] = min_partial_za[j][k]/20;     // 40 through 76 for 6 partials
            turning[j][l++] = min_partial_xg[j][k]/20;
            turning[j][l++] = min_partial_yg[j][k]/20;
        }

        for(k = 0; k < PARTIAL; k++)
        {
            turning[j][l++] = max_part_cond_ya[j][k] /40;     // value>sigma? value/sigma: 0
            turning[j][l++] = max_part_cond_za[j][k] /40;
            turning[j][l++] = max_part_cond_xg[j][k] /40;     // end previous through 4 * PARTIALS
            turning[j][l++] = max_part_cond_yg[j][k] /40;

            turning[j][l++] = min_part_cond_ya[j][k] /40;
            turning[j][l++] = min_part_cond_za[j][k] /40;     // 76 through 112 for 6 partials
            turning[j][l++] = min_part_cond_xg[j][k] /40;
            turning[j][l++] = max_part_cond_yg[j][k] /40;
        }

        for(k = 0; k < CONDITIONS; k++)
        {
//          [p1/sig, p2/sig, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]
            turning[j][l++] = cond_peak_ya[j][k];     // set of conditions some boolean some values
            turning[j][l++] = cond_peak_za[j][k];     // either peak/sigma, peak/sigma -1, peak1/peak2 -1
            turning[j][l++] = cond_peak_xg[j][k];     // end previous through 4 * PARTIALS
            turning[j][l++] = cond_trough_ya[j][k];
            turning[j][l++] = cond_trough_za[j][k];     // 76 through 112 for 6 partials
            turning[j][l++] = cond_trough_xg[j][k];
        }

        for(k = 0; k < 4; k++)
        {
            turning[j][l++] = ci_ya[j][k];              // value and location of max peak and troughs integrals
            turning[j][l++] = ci_za[j][k];
            turning[j][l++] = ci_xg[j][k];
            turning[j][l++] = ci_yg[j][k];

        }

        for(k = 0; k < SLOTS; k++)
        {
            turning[j][l++] = mean_slot_ya[j][k] /10;
            turning[j][l++] = mean_slot_za[j][k] /10;
            turning[j][l++] = mean_slot_xg[j][k] /10;
            turning[j][l++] = mean_slot_yg[j][k] /10;
        }


        for(k = PARTIAL; k < PARTIAL; k++)
        {
            turning[j][l++] = cp_xa[j][k];
            turning[j][l++] = cp_ya[j][k];
            turning[j][l++] = cp_xa[j][k];
            turning[j][l++] = cp_ya[j][k];

            turning[j][l++] = ct_yg[j][k];
            turning[j][l++] = ct_zg[j][k];
            turning[j][l++] = ct_yg[j][k];
            turning[j][l++] = ct_zg[j][k];
        }
    }

    max = min = 0;
    max_l = min_l = 0;

    for(i = 0; i < num_strides; i++)
    {
        for(j = 0; j < l; j++)
        {
            if(turning[i][j] > max)
            {
                max = turning[i][j];
                max_l = j;
            }
            if(turning[i][j] < min)
            {
                min = turning[i][j];
                min_l = j;
            }
        }
    }
//    if(max > 1 || min < -1)
//        printf("turning: max: %f at: %d, min: %f at: %d\n", max, max_l, min, min_l);
    printf("turn: %d\t", l);

    l = 0;
    for (j = 0; j < num_strides; j++) {

        l = 0;
        turn_speeds[j][l++] = stride_all[j][2]/1024;    // period


        turn_speeds[j][l++] = sigma_all_ya[j]/5;          // sigma total, 1 throug 5
        turn_speeds[j][l++] = sigma_all_za[j]/5;
        turn_speeds[j][l++] = sigma_all_zg[j]/5;
        turn_speeds[j][l++] = sigma_all_yg[j]/5;

        turn_speeds[j][l++] = sigma_high_ya[j]/5;
        turn_speeds[j][l++] = sigma_high_za[j]/5;
        turn_speeds[j][l++] = sigma_high_zg[j]/5;
        turn_speeds[j][l++] = sigma_high_yg[j]/5;

        turn_speeds[j][l++] = sigma_low_ya[j]/5;
        turn_speeds[j][l++] = sigma_low_za[j]/5;
        turn_speeds[j][l++] = sigma_low_zg[j]/5;
        turn_speeds[j][l++] = sigma_low_yg[j]/5;

        turn_speeds[j][l++] = mean_dev_ya[j]/5;
        turn_speeds[j][l++] = mean_dev_za[j]/5;           // average deviation 6 throgh 10
        turn_speeds[j][l++] = mean_dev_xg[j]/5;
        turn_speeds[j][l++] = mean_dev_yg[j]/5;

        turn_speeds[j][l++] = mean_high_ya[j]/5;
        turn_speeds[j][l++] = mean_high_za[j]/5;
        turn_speeds[j][l++] = mean_high_xg[j]/5;
        turn_speeds[j][l++] = mean_high_yg[j]/5;

        turn_speeds[j][l++] = mean_low_ya[j]/5;
        turn_speeds[j][l++] = mean_low_za[j]/5;
        turn_speeds[j][l++] = mean_low_xg[j]/5;
        turn_speeds[j][l++] = mean_low_yg[j]/5;

//        turning[j][l++] = skew_high_xa[j];          // skewness and kurtosis 11 trough 26
//        turn_speeds[j][l++] = skew_high_ya[j] /20;
//        turn_speeds[j][l++] = skew_high_za[j] /20;
//        turn_speeds[j][l++] = skew_high_xg[j] /20;
//        turn_speeds[j][l++] = skew_high_yg[j] /20;
//
//        turn_speeds[j][l++] = skew_low_ya[j] /20;
//        turn_speeds[j][l++] = skew_low_za[j] /20;
//        turn_speeds[j][l++] = skew_low_xg[j] /20;
//        turn_speeds[j][l++] = skew_low_yg[j] /20;
//
//        turn_speeds[j][l++] = kert_high_ya[j] /20;
//        turn_speeds[j][l++] = kert_high_za[j] /20;
//        turn_speeds[j][l++] = kert_high_xg[j] /20;
//        turn_speeds[j][l++] = kert_high_yg[j] /20;
//
//        turn_speeds[j][l++] = kert_low_ya[j] /20;
//        turn_speeds[j][l++] = kert_low_za[j] /20;
//        turn_speeds[j][l++] = kert_low_xg[j] /20;
//        turn_speeds[j][l++] = kert_low_yg[j] /20;

        for(k = 0; k < 4; k+=2){
            turn_speeds[j][l++] = peaks_ya[j][k]/20;       // peak1 and peak with relative loc
            turn_speeds[j][l++] = peaks_za[j][k]/20;       // trough1 and trough2 with relative location
            turn_speeds[j][l++] = peaks_xg[j][k]/20;       // 27 through 40
            turn_speeds[j][l++] = peaks_yg[j][k]/20;

            turn_speeds[j][l++] = troughs_ya[j][k]/20;
            turn_speeds[j][l++] = troughs_za[j][k]/20;
            turn_speeds[j][l++] = troughs_xg[j][k]/20;
            turn_speeds[j][l++] = troughs_yg[j][k]/20;
        }

        for(k = PARTIAL; k < PARTIAL; k++)
        {
//            turning[j][l++] = max_partial_xa[j][k];     // max values at each partial
            turn_speeds[j][l++] = max_partial_ya[j][k]/20;
            turn_speeds[j][l++] = max_partial_za[j][k]/20;     // 40 through 4 * PARTIALS
            turn_speeds[j][l++] = max_partial_xg[j][k]/20;
            turn_speeds[j][l++] = max_partial_yg[j][k]/20;


            turn_speeds[j][l++] = min_partial_ya[j][k]/20;
            turn_speeds[j][l++] = min_partial_za[j][k]/20;     // 40 through 76 for 6 partials
            turn_speeds[j][l++] = min_partial_xg[j][k]/20;
            turn_speeds[j][l++] = min_partial_yg[j][k]/20;
        }

        for(k = PARTIAL; k < PARTIAL; k++)
        {
            turn_speeds[j][l++] = max_part_cond_ya[j][k] /40;     // value>sigma? value/sigma: 0
            turn_speeds[j][l++] = max_part_cond_za[j][k] /40;
            turn_speeds[j][l++] = max_part_cond_xg[j][k] /40;     // end previous through 4 * PARTIALS
            turn_speeds[j][l++] = max_part_cond_yg[j][k] /40;

            turn_speeds[j][l++] = min_part_cond_ya[j][k] /40;
            turn_speeds[j][l++] = min_part_cond_za[j][k] /40;     // 76 through 112 for 6 partials
            turn_speeds[j][l++] = min_part_cond_xg[j][k] /40;
            turn_speeds[j][l++] = max_part_cond_yg[j][k] /40;
        }

        for(k = CONDITIONS; k < CONDITIONS; k++)
        {
//          [p1/sig, p2/sig, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]
            turn_speeds[j][l++] = cond_peak_ya[j][k];     // set of conditions some boolean some values
            turn_speeds[j][l++] = cond_peak_za[j][k];     // either peak/sigma, peak/sigma -1, peak1/peak2 -1
            turn_speeds[j][l++] = cond_peak_xg[j][k];     // end previous through 4 * PARTIALS
            turn_speeds[j][l++] = cond_trough_ya[j][k];
            turn_speeds[j][l++] = cond_trough_za[j][k];     // 76 through 112 for 6 partials
            turn_speeds[j][l++] = cond_trough_xg[j][k];
        }

        for(k = 0; k < 4; k++)
        {
            turn_speeds[j][l++] = ci_ya[j][k];              // value and location of max peak and troughs integrals
            turn_speeds[j][l++] = ci_za[j][k];
            turn_speeds[j][l++] = ci_xg[j][k];
            turn_speeds[j][l++] = ci_yg[j][k];

        }

        for(k = 0; k < SLOTS; k++)
        {
            turn_speeds[j][l++] = mean_slot_ya[j][k] /10;
            turn_speeds[j][l++] = mean_slot_za[j][k] /10;
            turn_speeds[j][l++] = mean_slot_xg[j][k] /10;
            turn_speeds[j][l++] = mean_slot_yg[j][k] /10;
        }


        for(k = PARTIAL; k < PARTIAL; k++)
        {
            turn_speeds[j][l++] = cp_xa[j][k];
            turn_speeds[j][l++] = cp_ya[j][k];
            turn_speeds[j][l++] = cp_xa[j][k];
            turn_speeds[j][l++] = cp_ya[j][k];

            turn_speeds[j][l++] = ct_yg[j][k];
            turn_speeds[j][l++] = ct_zg[j][k];
            turn_speeds[j][l++] = ct_yg[j][k];
            turn_speeds[j][l++] = ct_zg[j][k];
        }
    }
    printf("turn speed: %d\tstrides:%d\n", l, num_strides);

//    printf("strides: %d,\n", num_strides);
//    printf("strides: %d\n", num_strides);
    *rows = num_strides;

}

