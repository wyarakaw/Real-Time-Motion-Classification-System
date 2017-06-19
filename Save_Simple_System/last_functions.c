//
// Created by Amaael Antonini on 5/26/17.
//
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "constants.h"
#include "classifiers.h"



int integrate_zeros(float * data, float *out, int * indexes, int n)
{
    int i, cnt;
    cnt = i = 0;
    while(i < n)
    {
        indexes[cnt++] = i;
        out[i++] = 0;
        while(i < n && data[i] > 0)
        {
            out[i] = out[i-1] + data[i];
            i++;
        }
        if(i < n)
        {
            out[i] = 0;
            indexes[cnt++] = i;
            i++;
        }
        while(i < n && data[i] <= 0)
        {
            out[i] = out[i-1] + data[i];
            i++;
        }
    }
//    out[n-1] = 0;
//    indexes[cnt++] = n;
    return cnt;
}

//def integrate(data):
//    out = np.empty(shape=(len(data)), dtype=float)
//    out_2 = np.empty(shape=(len(data)), dtype=float)
//    stops = np.zeros(shape=(len(data)), dtype="int")
//    num = 0
//    out[0] = 0
//    up = data[0] > 0
//    down = data[0] <= 0
//    i = 0
//    n = len(data)
//    while i < n:
//        out[i] = 0
//        stops[num] = i
//        num += 1
//        i += 1
//        while i < n and data[i] > 0:
//            out[i] = out[i-1] + data[i]
//            i += 1
//        if i < n:
//            out[i] = 0
//            stops[num] = i
//            num += 1
//            i += 1
//        while i < n and data[i] <= 0:
//            out[i] = out[i-1] + data[i]
//            i += 1
//    t = 0
//    for i in range(num-1):
//        t2 = stops[i+1]-stops[i]
//        for j in range(stops[i], stops[i+1]):
//            out_2[j] = out[j]/float(t2)
//    t2 = n - stops[num-1]
//    if t2 > 0:
//        for j in range(stops[num-1], n):
//            out_2[j] = out[j] /float(t2)
//    return out, out_2, stops, num


void max_point_2(float * data, int *strides, int *indexes,
               float *peak, float *peak_dist, float *trough, float *trough_dist, int n, float ratio)
{
    int i, j, k;
    float high, low;
    int h_l, l_l;
    k = j = 0;
    for(i = 1; i < n; i++)
    {

        high = low = 0;
        h_l = l_l = strides[i-1];
        while(indexes[j] < strides[i])
        {
            if(data[indexes[j]] > high)
            {
                high = data[indexes[j]];
                h_l = indexes[j];
            }
            if(data[indexes[j]] < low)
            {
                low = data[indexes[j]];
                l_l = indexes[j];
            }
            j++;
            printf("max_point\n");
        }
        h_l -= strides[i-1];
        l_l -= strides[i-1];
        if(ratio > 1){
            high /= ratio;
            low /= ratio;
        }
        peak[i] = high;
        trough[i] = low;
        peak_dist[i] = h_l;
        trough_dist[i] = l_l;
    }
}

//int find_strides(float *data, int * indexes, float *data2, int * start1, int * norms, int *dist, int *widths, float weight, int n)
//{
//    float ave, temp;
//    int cnt, i;
//    cnt = 0;
//    ave = 0;
//    float w = weight;
//    for(i = 1; i < n; i++)
//    {
//        temp = data[indexes[i]-1];
//        if(temp < -6000)
//        {
//            data2[cnt] = temp;
//            start1[cnt] = indexes[i-1]-1;
//            dist[cnt] = indexes[i] - indexes[i-1];
//            cnt++;
//            ave += temp;
//        }
//    }
//    if(cnt > 0 && w > 0)
//    {
//        ave /= cnt;
////        ave *= w;
//        ave *= 0.5;
//    }
//    int cnt2 = 0;
//    float ave2 = 0;
//
//    // Eliminate small peaks round 1
//    for(i =0; i < cnt; i++)
//    {
//        if(data2[i] < ave)
//        {
//            data2[cnt2] = data2[i];
//            start1[cnt2] = start1[i];
//            dist[cnt2] = dist[i];
//            ave2 += data2[cnt2];
//            cnt2++;
//
//        }
//    }
//
//    for(i = cnt2; i < n; i++){
//        data2[i] = 0;
//        dist[i] = 0;
//        start1[i] = 0;
//    }
//    if(cnt2 > 0) {
//        ave2 /= cnt2;
//        ave2 *= 0.5;
//    }
//
//    cnt = 0;
//    // Eliminate small peaks round 2
//    for(i =0; i < cnt2; i++)
//    {
//        if(data2[i] < ave2)
//        {
//            data2[cnt] = data2[i];
//            start1[cnt] = start1[i];
//            dist[cnt] = dist[i];
//            cnt++;
//
//        }
//    }
//
//    for(i = cnt; i < cnt2; i++){
//        data2[i] = 0;
//        dist[i] = 0;
//        start1[i] = 0;
//    }
//
//    // scale peaks by width
//    for(i = 0; i < cnt; i++)
//    {
//        if(dist[i] > 0)
//        {
//            data2[i] /= dist[i];
//        }
//        else
//            printf("dist was less than zero: ERROR\n");
//    }
//
//    int num, j;
//    int wa;
//
////    cnt2 = 0;
////    for(i = 0; i < cnt; i++)
////    {
////        if(data2[i] < ave)
////        {
////            data2[cnt2] = data2[i];
////            start1[cnt2++] = start1[i];
////
////        }
////    }
////
////    for(i = cnt2; i < n; i++){
////        data2[i] = 0;
////        dist[i] = 0;
////        start1[i] = 0;
////    }
//    return cnt2;
//}

//def find_strides(data, stops, n, dist_range = 500):
//    # print(n)
//    ave = 0
//    cnt = 0
//    out = []
//    loc = []
//    dist = []
//    for i in range(1, n):
//        temp = data[stops[i]-1]
//        if temp < -6000:
//            out.append(temp)
//            loc.append(stops[i-1]-1)
//            dist.append(stops[i] - stops[i-1])
//            cnt += 1
//            ave += temp
//    if cnt > 0:
//        ave /= cnt
//    out_2 = []
//    loc_2 = []
//    dist_2 = []
//    for i in range(len(out)):
//        if out[i] < ave*0.5 and out[i] < -6000:
//            out_2.append(out[i])
//            loc_2.append(loc[i])
//            dist_2.append(dist[i])
//
//    # for i in loc_2:
//        # print(i, "\t", end = "")
//    # print("")
//
//    del out[:]
//    del loc[:]
//    del dist[:]
//    ave = 0
//    for i in range(len(out_2)):
//        ave += out_2[i]
//    if(len(out_2) > 0):
//        ave/= len(out_2)
//    for i in range(len(out_2)):
//        if out_2[i] < ave*0.5:
//            out.append(out_2[i])
//            loc.append(loc_2[i])
//            dist.append(dist_2[i])
//
//    out_3 = []
//    loc_3 = []
//    dist_3 = []
//    ave = 0
//    for i in range(len(out)):
//        out[i] /= dist[i]
//        ave += out[i]
//    if len(out) > 0:
//        ave /= len(out)
//    for i in range(len(out)):
//        if out[i] < ave * 0.5:
//            out_3.append(out[i])
//            loc_3.append(loc[i])
//            dist_3.append(dist[i])
//
//    del out_2[:]
//    del loc_2[:]
//    del dist_2[:]
//    norms = []
//    widths = []
//    for i in range(len(out_3)):
//        ave = 0
//        num = 0
//        wa = 0
//        for j in range(10):
//            if i - j < 0:
//                break
//            if loc_3[i] - loc_3[i-j] > dist_range:
//                break
//            ave += out_3[i-j]
//            wa += dist_3[i-j]
//            num += 1
//        for j in range(10):
//            if i + j < len(out_3):
//                break
//            if loc_3[i+j] - loc_3[i] > dist_range:
//                break
//            ave += out_3[i-j]
//            wa += dist_3[i - j]
//            num += 1
//        if num > 0:
//            ave /= num
//            wa /= num
//        norms.append(ave)
//        widths.append(wa)
//
//    for i in range (len(out_3)):
//        if out_3[i] < norms[i] * 0.6 and dist_3[i] <= widths[i]*1.2:
//            # print(widths[i], end = " ")
//            out_2.append(out_3[i])
//            loc_2.append(loc_3[i])
//            dist_2.append(dist_3[i])
//    # print("")
//    return out_2, loc_2


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

    for(i = cnt2; i < cnt; i++){
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
    ave = 0;
    for(i = 0; i < cnt; i++)
    {
        if(dist[i] > 0)
        {
            data2[i] /= dist[i];
            ave += data2[i];
        }
        else
            printf("dist was less than zero: ERROR\n");
    }
    if(cnt > 0) {
        ave /= cnt;
        ave *= 0.5;
    }
    cnt2 = 0;

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

    for(i = cnt2; i < cnt; i++){
        data2[i] = 0;
        dist[i] = 0;
        start1[i] = 0;
    }

    int num, j;
    float wa;
    for(i = 0; i < cnt; i++)
    {
        num = 0;
        wa = 0;
        ave = 0;
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
//        printf("%f\t%f\n", norms[i], widths[i]);
    }



    cnt2 = 0;
//    printf("%d\n", cnt);
    for(i = 0; i < cnt; i++)
    {
//        printf("data: %f\tdist: %d\tnorm: %f\twidth: %f\n", data2[i], dist[i], norms[i], widths[i]);
//        printf("before contidion\n");
        if(data2[i] < norms[i]*0.6 && dist[i] <= widths[i]*1.2)
        {
//            printf("after contidion\n");
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

void max_point(float * data, int strides[][4], int *indexes,
               float peak[][4], int n, float ratio)
{
//    printf("start\n");
    int i, j, k;
    float high, low;
    int h_l, l_l;
    k = j = 0;
    for(i = 0; i < n; i++)
    {
//        printf("%d\n", strides[i][0]);
        high = low = data[strides[i][0]];
        h_l = l_l = strides[i][0];
        j = h_l + 1;
        for(j = strides[i][0]; j < strides[i][1]; j++) {
            if (data[j] > high) {
                high = data[j];
                h_l = j;
            }
            if (data[j] < low) {
                low = data[j];
                l_l = j;
            }
        }
//        printf("max\n");

        h_l -= strides[i][0];
        l_l -= strides[i][0];

        if(ratio > 1){
            high /= ratio;
            low /= ratio;
        }
//        printf("ratio\n");
//        printf("high: %f, low: %f\n", high, low);
        peak[i][0] = high;
        peak[i][2] = low;
        if(strides[i][2] > 0) {
            peak[i][1] = (float) h_l / strides[i][2];
            peak[i][3] = (float) l_l / strides[i][2];
        }
        else{
            peak[i][1] = 0;
            peak[i][3] = 0;
        }
        if(peak[i][1] > 0.95)
            peak[i][1] = 0;
        if(peak[i][3] > 0.95)
            peak[i][3] = 0;
    }
}


void filter_data(float *data, float * dest, const float * shape, int s, int n)
{
    int i, j , t;
    float w, temp;
    t = (int) s/2;
    if(s < t)
    {
        return;
    }
    dest[0] = 0;
    for(i = 0; i < t; i++)
    {
        temp = 0;
        w = 0;
        for(j = 0; j < i+t; j++)
        {
            w += shape[t+j-i];
            temp += data[j] * shape[t+j-i];
        }
        dest[i] = temp/w;
    }
    for(i = t; i < n-t; i++)
    {
        temp = 0;
        for(j = 0; j < s; j++)
        {
            temp += data[i+j-1] * shape[j];
        }
        dest[i] = temp;
    }
    for(i = n - t; i < n; i++)
    {
        temp = 0;
        w = 0;
        for(j = 0; j < n - i; j++)
        {
            w += shape[j];
            temp += data[i - t + j] * shape[j];
        }
        dest[i] = temp / w;
    }
}

//def smooth(data, shape, sl = 50):
//    out = np.empty(shape=(len(data)), dtype=float)
//    t1 = int(sl/2)
//    end = len(data)
//    if len(data) < sl:
//        return
//    out[0] = 0
//    for i in range(0, t1):
//        temp = 0
//        weight = 0
//        for j in range(i+t1):
//            weight += shape[t1+j-i]
//            temp += data[j] * shape[t1+j-i]
//        out[i] = temp/weight
//    for i in range(t1, end-t1):
//        temp = 0
//        for j in range(sl):
//            temp += data[i+j-t1] * shape[j]
//        out[i] = temp
//    for i in range(end - t1, end):
//        temp = 0
//        weight = 0
//        for j in range(end - i):
//            weight += shape[j]
//            temp += data[i - t1 + j] * shape[j]
//        out[i] = temp / weight
//    return out


void mean_stride(float * data, int strides[][4],
               float *upper, float *lower, int n)
{
//    printf("start\n");
    int i, j, k;
    float high, low;
    int h_l, l_l;
    k = j = 0;
    for(i = 0; i < n; i++)
    {
        high = low = 0;
        h_l = l_l = 0;
        for(j = strides[i][0]; j < strides[i][1]; j++)
        {
            if(data[j] > 0)
            {
                high += data[j];
                h_l ++;
            }
            else if(data[j] < 0)
            {
                low += data[j];
                l_l ++;
            }
        }
        if(h_l > 0)
            high /= h_l;
        if(l_l > 0)
            low /= l_l;
        upper[i] = high;
        lower[i] = low;
    }
}


void max_slot(float * data, int strides[][4], float max_low[][PARTIAL], float max_high[][PARTIAL], float ratio, int slots, int n)
{
    float low, high;
    int i, j, k, l;
    for(j = 0; j < n; j++)
    {
        l = (int) ((strides[j][1] - strides[j][0])/slots);
        for(k = 0; k < slots; k++) {
            high = 0;
            low = 0;

            for (i = strides[j][0] + k * l; i < strides[j][0] + (k + 1) * l; i++) {
                if (data[i] > high) {
                high = data[i];
                }
                else if(data[i] < low) {
                    low = data[i];
                }
            }
            if(ratio > 1)
            {
                high /= ratio;
                low /= ratio;
            }

            max_low[j][k] = low;
            max_high[j][k] = high;
//            printf("%d\t%d\t%d\t%f\t%f\n", n, j, k, low, high);

        }
    }
}


void higher_moment(float * data, int stride[][4], float *sigma_low, float *sigma_high,
                  float * tm_low, float * tm_high, float * fm_low, float * fm_high, float w, int n)
{
    double t_low, t_high, f_low, f_high;
    double n_low, n_high;
    int i, j;
    for(j = 0; j < n; j++)
    {
        t_low = f_low = 0;
        t_high = f_high = 0;
        n_low = 0;
        n_high = 0;
        for(i = stride[j][0]; i < stride[j][1]; i++)
        {
            if(data[i] > sigma_high[j]*w)
            {
//                printf("data: %f", data[i]);
                t_high += pow(data[i], 3);
                f_high += pow(data[i], 4);
                n_high++;
            }
            else if(data[i] < -sigma_low[j]*w)
            {
                t_low += pow(-data[i], 3);
                f_low += pow(data[i], 4);
                n_low++;
            }
        }
        if(n_low == 0)
            n_low = 1;
        if(n_high == 0)
            n_high = 1;
        t_low /= n_low;
        t_high /= n_high;
        f_low /= n_low;
        f_high /= n_high;
//        printf("high: %f, low: %f\n", n_high, n_low);
        tm_low[j] = (float) t_low;
        tm_high[j] = (float) t_high;
        fm_low[j] = (float) f_low;
        fm_high[j] = (float) f_high;
//        printf("stride: %d\tthird low: %f\tthird high: %f\tfourth low: %f\tfourth high: %f\n",
//               j, t_low, t_high, f_low, f_high);

    }
}



void sigma_stride_hl(float * data, int stride[][4], float * sigma_low, float * sigma_high, int n)
{
    double low, high;
    double n_low, n_high;
    int i, j;
    for(j = 0; j < n; j++)
    {
        low = 0;
        high = 0;
        n_low = 0;
        n_high = 0;
        for(i = stride[j][0]; i < stride[j][1]; i++)
        {
            if(data[i] > 0)
            {
//                printf("data: %f", data[i]);
                high += data[i] * data[i];
                n_high++;
            }
            else if(data[i] < 0)
            {
                low += data[i] * data[i];
                n_low++;
            }
        }
        if(n_low == 0)
            n_low = 1;
        if(n_high == 0)
            n_high = 1;
        low = low/n_low;
        high = high/n_high;
        high = sqrt(high);
        low = sqrt(low);
        sigma_low[j] = (float) low;
        sigma_high[j] = (float) high;
//        printf("stride: %d\tsigma low: %f\tsigma high: %f\n", j, low, high);
    }
}

void mean_hl_stride(float *data, float *m_h, float *m_l, int stride[][4], int n)
{
    float low, high;
    float n_low, n_high;
    int i, j;
    for(j = 0; j < n; j++)
    {
        low = 0;
        high = 0;
        n_low = 0;
        n_high = 0;
        for(i = stride[j][0]; i < stride[j][1]; i++)
        {
            if(data[i] > 0)
            {
//                printf("data: %f", data[i]);
                high += data[i];
                n_high++;
            }
            else if(data[i] < 0)
            {
                low -= data[i];
                n_low++;
            }
        }
        if(n_low == 0)
            n_low = 1;
        if(n_high == 0)
            n_high = 1;
        low = low/n_low;
        high = high/n_high;
        m_l[j] = low;
        m_h[j] = high;
//        printf("stride: %d\tmean low: %f\tmean high: %f\n", j, m_l[j], m_h[j]);
    }
}

void kurt_skew(float * data, int stride[][4], float *sigma_low, float *sigma_high,
               float * tm_low, float * tm_high, float * fm_low, float * fm_high, float *mean_high, float *mean_low,
               float *s_h, float *s_l, float *k_h, float *k_l, float w, float *max_d, float top, float ratio, int n)
{
    mean_hl_stride(data, mean_high, mean_low, stride, n);
    sigma_stride_hl(data, stride, sigma_low, sigma_high, n);
    higher_moment(data, stride, sigma_low, sigma_high, tm_low, tm_high, fm_low, fm_high, w, n);
    float max, min;
    max = (*max_d);
    min = -(*max_d);
    double kh, kl, sh, sl;
    int i, j, k;
    for(i = 0; i < n; i++)
    {
        sh = tm_high[i];
        sl = tm_low[i];
        sh -= 3 * mean_high[i] * pow(sigma_high[i], 2);
        sl -= 3 * mean_low[i] * pow(sigma_high[i], 2);
        sh -= pow(mean_high[i], 3);
        sl -= pow(mean_low[i], 3);
//        printf("stride: %d\tskew low: %2.5f\tskew high: %2.5f\t", i, sl, sh);
        if(sigma_high[i] != 0)
            sh /= (ratio * pow(sigma_high[i], 3));
        if(sigma_low[i] != 0)
            sl /= (ratio * pow(sigma_low[i], 3));
        if(sl > top)
            sl = top;
        else if(sl < -top)
            sl = -top;
        if(sh > top)
            sh = top;
        if(sh < -top)
            sh = -top;

        s_h[i] = (float) sh;
        s_l[i] = (float) sl;
        if(s_h[i] > max){
            max = s_h[i];
        }
        if(s_l[i] > max){
            max = s_l[i];
        }
        if(s_h[i] < min){
             min = s_h[i];
        }
        if(s_l[i] < min){
            min = s_l[i];
        }
        kh = fm_high[i];
        kl = fm_low[i];
        kh -= 4 * mean_high[i] * tm_high[i];
        kl -= 4 * mean_low[i] * tm_low[i];
        kh += 6 * pow(mean_high[i], 2) * pow(sigma_high[i], 2);
        kl += 6 * pow(mean_low[i], 2) * pow(sigma_low[i], 2);
        kh += 3 * pow(mean_high[i], 4);
        kl += 3 * pow(mean_low[i], 4);
//        printf("kurt low: %2.5f\tkurt high: %2.5f\n", kl, kh);
        if(sigma_high[i] != 0)
            kh /= (ratio * pow(sigma_high[i], 4));
        if(kh > top)
            kh = top;
        if(kh < -top)
            kh = -top;
        if(sigma_low[i] != 0)
            kl /= (ratio * pow(sigma_low[i], 4));
        if(kl > top)
            kl = top;
        if(kl < -top)
            kl = -top;
        k_h[i] = (float) kh;
        k_l[i] = (float) kl;
        // return the max value;
        if(k_h[i] > max){
            max = k_h[i];
        }
        if(k_l[i] > max){
            max = k_l[i];
        }
        if(k_h[i] < min){
            min = k_h[i];
        }
        if(k_l[i] < min){
            min = k_l[i];
        }
        if(max > -min){
            *max_d = max;
        }else{
            *max_d = -min;
        }
//        printf("stride: %d\tskew low: %2.5f\tskew high: %2.5f\tkurt low: %2.5f\tkurt high: %2.5f\n",
//               i, s_l[i], s_h[i], k_l[i], k_h[i]);

    }
}


void sigma_stride_total(float * data, int stride[][4], float * dest, int n)
{
    double temp;
    int i, j;
    for(j = 0; j < n; j++)
    {
        temp = 0;
        for(i = stride[j][0]; i < stride[j][1]; i++)
        {
            temp += data[i] * data[i];
        }
        if(stride[j][1] - stride[j][0] > 0) {
            temp /= (stride[j][2]);
        }
        dest[j] = (float) sqrt(temp);
    }
}



void time_slots(float *time, float * data, int strides[][4], float dest[][SLOTS], int n)
{
    float step, ave, end;
    int i, j, k, l;

    for(j = 0; j < n; j++)
    {
        step = (time[strides[j][1]] - time[strides[j][0]])/SLOTS;
//        printf("stride start: %d\tstride end: %d\n", strides[j][0], strides[j][1]);
//        printf("time start: %f\ttime end: %f\n", time[strides[j][0]], time[strides[j][1]]);
//        printf("step: %f\n", step);
        i = strides[j][0];
        for(k = 0; k < SLOTS; k++)
        {
            ave = 0;
            end = time[strides[j][0]] + (k + 1) * step;
            l = 0;
            while (time[i] < end) {
                ave += data[i++];
                l++;
            }
            if(l > 0)
            {
                ave /= l;
                dest[j][k] = ave;
            }
            else
            {
                dest[j][k] = 0;
                printf("error, zero samples\n");
            }

        }
    }
}


void norm_minus_one(float *source, float*norm, float*dest, float w, int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(norm[i] != 0)
            if(source[i] > norm[i] || source[i] < - norm[i])
                dest[i] = source[i]/norm[i] -1;
            else
                dest[i] = 0;
        else
            dest[i] = 0;
    }
}


void sigma_condition(float source[][PARTIAL], float*norm, float dest[][PARTIAL], float w, int n)
{
//    printf("%f\n", w);
    int i, j;
    for(i = 0; i < n; i++)
    {

        for(j = 0; j < PARTIAL; j++)
        {
            if(norm[i] != 0) {
//                printf("sigma cond\n");
                if (w * source[i][j] > norm[i]) {
//                    printf("condition met\n");
                    dest[i][j] = source[i][j] / norm[i];
//                    printf("sigma cond: %f\n", dest[i][j]);
                } else{
//                    printf("condition not met: %f, %f\n", norm[i], source[i][j]);
                    dest[i][j] = 0;
                }

            }
            else{
                dest[i][j] = 0;
                printf("norm was zero\n");
            }
        }
    }
}

void peak_trough_compare(int strides[][4], float peak1[][4], float peak2[][4], float trough1[][4], float trough2[][4],
                         float *norm1, float d_p1[][CONDITIONS], float d_t1[][CONDITIONS], float w1, float w2, float w3, int n)
{
    // [p1/sig -1, p2/sig -1, p1/t1, p1/p2, l1>l2?, l2>l1?, l1<1/3?, 1/3<l1<2/3?, l1<2/3?, l2<1/3?, 1/3<l2<2/3?, l2<2/3?]
    int i, j, k;
    float s1, s2, r1, r2;
    for(i = 0; i < n; i++)
    {
        if(norm1[i] > 0){
            // max peak and trough are bigger than sigma and sigma not zero
            if(peak1[i][0] > norm1[i]) {
                d_p1[i][0] = w1 * peak1[i][0] / norm1[i] - 1;
                d_p1[i][0]/=w3;
            } else {
                d_p1[i][0] = 0;
            }
            if(trough1[i][0] < -norm1[i]) {
                d_t1[i][0] = -w1 * trough1[i][0] / norm1[i] - 1;
                d_t1[i][0] /= w3;
            } else {
                d_t1[i][0] = 0;
            }
            // second max peak and trough are bigger than sigma and sigma not zero
            if(peak1[i][2] > norm1[i]) {
                d_p1[i][1] = w1 * peak1[i][2] / norm1[i] - 1;
                d_p1[i][1] /= w3;
            } else {
                d_p1[i][1] = 0;
            }
            if(trough1[i][2] < -norm1[i]) {
                d_t1[i][1] = w1 * trough1[i][2] / norm1[i] - 1;
                d_t1[i][1] /= w3;
            } else {
                d_t1[i][1] = 0;
            }
        } else{
            d_p1[i][0] = 0;
            d_p1[i][1] = 0;
            d_t1[i][0] = 0;
            d_t1[i][1] = 0;
        }
        // second data set cound be xa and ya, probably that will be

        // done part 1, first and second compare sigma to peaks
        // part 2,

        // peak bigger than trough
        if(peak1[i][0] > -trough1[i][0] && trough1[i][0] < 0) {
            d_p1[i][2] = -peak1[i][0]/trough1[i][0];
            d_p1[i][2] /= w3;
        } else {
             d_p1[i][2] = 0;
        }
        // trough bigger than peak
        if(trough1[i][0] < -peak1[i][0] && peak1[i][0] < 0)
        {
            d_t1[i][2] = -trough1[i][0]/peak1[i][0];
            d_t1[i][2] /= w3;
        } else {
            d_t1[i][2] = 0;
        }

        // compare peak axis and peak axis2, trough axis 1 and trough axis 2
        if(peak1[i][0] > peak2[i][0] && peak2[i][0] > 0) {
            d_p1[i][3] = peak1[i][0]/trough1[i][0];
            d_p1[i][3] /= w3;
        } else {
            d_p1[i][3] = 0;
        }
        if(trough1[i][0] < trough2[i][0] && trough2[i][0] < 0) {
            d_t1[i][3] = trough1[i][0]/trough2[i][0];
            d_t1[i][3] /= w3;
        } else {
            d_t1[i][3] = 0;
        }
        // find whih peak comes first
        if(peak1[i][1] < peak1[i][3]) {
            d_p1[i][4] = .5;
            d_p1[i][5] = 0;
        } else {
            d_p1[i][4] = 0;
            d_p1[i][5] = .5;
        }
        if(trough1[i][1] < trough1[i][3]) {
            d_t1[i][4] = .5;
            d_t1[i][5] = 0;
        } else {
            d_t1[i][4] = 0;
            d_t1[i][5] = .5;
        }

        // is the max peak at the beginning or at the end or in the middle
        if(strides[i][1] > 0 ) {
            s1 = 3*peak1[i][1];
            s2 = 3*peak1[i][3];
            r1 = 3*trough1[i][1];
            r2 = 3*trough1[i][3];
            if(s1 < 1){
//                printf("s1 1\t");
                d_p1[i][6] = 0.5;
                d_p1[i][7] = 0;
                d_p1[i][8] = 0;
            } else if(s1 < 2){
//                printf("s1 2\t");
                d_p1[i][6] = 0;
                d_p1[i][7] = 0.5;
                d_p1[i][8] = 0;
            } else{
//                printf("s1 3\t");
                d_p1[i][6] = 0;
                d_p1[i][7] = 0;
                d_p1[i][8] = 0.5;
            }

            if(s2 < 1){
//                printf("s2 1\t");
                d_p1[i][9 ] = 0.5;
                d_p1[i][10] = 0;
                d_p1[i][11] = 0;
            } else if(s2 < 2){
//                printf("s2 2\t");
                d_p1[i][9 ] = 0;
                d_p1[i][10] = 0.5;
                d_p1[i][11] = 0;
            } else{
//                printf("s2 3\t");
                d_p1[i][9 ] = 0;
                d_p1[i][10] = 0;
                d_p1[i][11] = 0.5;
            }

            if(r1 < 1){
//                printf("r1 1\t");
                d_t1[i][6] = 0.5;
                d_t1[i][7] = 0;
                d_t1[i][8] = 0;
            } else if(r1 < 2){
//                printf("r1 2\t");
                d_t1[i][6] = 0;
                d_t1[i][7] = 0.5;
                d_t1[i][8] = 0;
            } else{
//                printf("r1 3\t");
                d_t1[i][6] = 0;
                d_t1[i][7] = 0;
                d_t1[i][8] = 0.5;
            }
            if(r2 < 1){
//                printf("r2 1\t");
                d_t1[i][9 ] = 0.5;
                d_t1[i][10] = 0;
                d_t1[i][11] = 0;
            } else if(r2 < 2){
//                printf("r2 2\t");
                d_t1[i][9 ] = 0;
                d_t1[i][10] = 0.5;
                d_t1[i][11] = 0;
            } else{
//                printf("r2 3\t");
                d_t1[i][9 ] = 0;
                d_t1[i][10] = 0;
                d_t1[i][11] = 0.5;
            }
        }
    }
}

void average_deviation(float *data, int strides[][4], float *dest, float *max_d, int n)
{
    int i, j;
    float ave;
    float max;
    max = (*max_d);
    for(i = 0; i < n; i++)
    {
        ave = 0;
        for(j = strides[i][0]; j < strides[i][1]; j++)
        {
            if(data[j] > 0){
                ave += data[j];
            }else{
                ave -= data[j];
            }
        }
        if(strides[i][1] - strides[i][0] > 0){
            ave /= (strides[i][1] - strides[i][0]);
        }
        if(ave > max){
            max = ave;
        }
        dest[i] = ave;
    }
    *max_d = max;
//    printf("max average deviation: %f\n", max);
}

void class_slots(float *time, float * data, int strides[][4], float dest[][CLASS_SLOTS], int n)
{
    float step, ave, end;
    int i, j, k, l;

    for(j = 0; j < n; j++)
    {
        step = (time[strides[j][1]] - time[strides[j][0]])/CLASS_SLOTS;
//        printf("stride start: %d\tstride end: %d\n", strides[j][0], strides[j][1]);
//        printf("time start: %f\ttime end: %f\n", time[strides[j][0]], time[strides[j][1]]);
//        printf("step: %f\n", step);
        i = strides[j][0];
//        printf("%d\t%d\t%f\n", strides[j][0], strides[j][1], step);
        for(k = 0; k < CLASS_SLOTS; k++)
        {
//            printf("k: %d\t", k);
            ave = 0;
            end = time[strides[j][0]] + (k + 1) * step;
            l = 0;
            while (time[i] < end) {
                ave += data[i++];
                l++;
            }
            if(l > 0)
            {
                ave /= l;
                dest[j][k] = ave;
            }
            else
            {
//                printf("k: %d\t", k);
                dest[j][k] = 0;
                printf("\nerror, zero samples: %d\n", j);
//                printf("%d\t%d\t%f\n", strides[j][0], strides[j][1], step);
//                printf("%f\t%f\t%f\t%d\n", time[strides[j][0]] + (k + 1) * step, time[strides[j][0]] + (k) * step, step, k);
            }
        }
//        printf("\n");
    }
}

void stride_match(float *time_source, float *time_dest, int * strides_source, int *strides_dest, int n_s, int n_d)
{
    int i, j, k, l;
    k = l = 0;

    for(j = 0; j < n_s; j++)
    {
        while(k < n_d && time_dest[k++] < time_source[strides_source[j]]);
        strides_dest[j] = k;
    }
}










