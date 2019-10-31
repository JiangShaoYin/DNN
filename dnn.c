//
// Created by Administrator on 2019/10/30.
//
# include "include/dnn.h"
# include "stdio.h"
#include <fcntl.h>
# include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


float**** malloc_4D_array(int width, int height, int channels, int filter_nums) {
    float ****para_array = (float****)malloc(sizeof(float***) * filter_nums);  //申请filter_nums个3D matrix

    for (int i = 0;i < filter_nums; i++) {
        para_array[i] = (float***)malloc(sizeof(float**) * channels);  // 申请channels个2D matrix
        for (int j = 0; j < channels; j++) {
            para_array[i][j] = (float **) malloc(sizeof(float *) * height); // 申请height个1D matrix
            for(int k = 0; k < height; k++){
                para_array[i][j][k] = (float *) malloc(sizeof(float ) * width); // 申请长度为width的数组
            }
        }
    }
    return para_array;
}

float*** malloc_3D_array(int width, int height, int channels) {
    float ***para_array = (float***)malloc(sizeof(float**) * channels);  // 申请channels个2D matrix

    for (int i = 0;i < channels; i++) {
        para_array[i] = (float**)malloc(sizeof(float*) * height); // 申请height个1D matrix
        for (int j = 0; j < height; j++) {
            para_array[i][j] = (float *) malloc(sizeof(float) * width);// 申请长度为width的数组
            memset(para_array[i][j], 0, width);
        }
    }
    return para_array;
}

int loadParameters(char* path, float**** parameters, int width, int height, int channels, int filter_nums){
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        int errNum = errno;
        printf("open fail errno = %d", errNum);
        return -1;
    }

    char parameter[30] ;       // 读dat文件里面保存的每个参数
    int idx = 0;               // 第idx个参数

    int parameter_nums = width * height * channels * filter_nums;

    int idx_filter=0, idx_channels=0, idx_height=0, idx_width=0;
    while(feof(fp) == 0 && idx < parameter_nums) { // 文件未到末尾,feof返回0
        idx_filter = idx / (width * height * channels); // 以idx == 101为例，idx_filter = 101 / 3*3*3 == 3
        idx_channels = idx % (width * height * channels) / (width * height); // 101 % 3 * 3 * 3 == 20   20/9 = 2
        idx_height = idx % (width * height * channels) % (width * height) / width; // 101 % 3 * 3 * 3 == 20,   20 % 9 = 2, 2/3 = 0
        idx_width = idx % (width * height * channels) % (width * height) % width;  // 2 %3 = 2

        memset(parameter, 0, 30);  // 初始清空缓冲区
        fgets(parameter, 99, fp);  // 从fp中最多读99个char到input里面，如果提前遇到\n则读取结束

        parameters[idx_filter][idx_channels][idx_height][idx_width] = atof(parameter);
//        printf("%f\n",parameters[idx_filter][idx_channels][idx_height][idx_width]);
        idx++;

    }
    fclose(fp);
    return 0;
}

void print_3D_array(float ***p){
    int channels = _msize(p) / sizeof(float**);
    int height = _msize(p[0]) / sizeof(float*);
    int width =  _msize(p[0][0]) / sizeof(float);

    int idx = 0;
        for (int j = 0; j < channels; j++) {
            printf("\n\nchannel %d: ", j);
            for (int k = 0; k < height; k++) {
                printf("\nrow %d: \n", k);
                for(int l = 0; l < width; l++){
                    printf("%.1f ", p[j][k][l]);
                    idx++;
                }
            }
        }
    }
int free_3D_array(float*** p){
    int channels = _msize(p) / sizeof(float**);
    int height = _msize(p[0]) / sizeof(float*);
    int width =  _msize(p[0][0]) / sizeof(float);

    for (int j = 0; j < channels; j++) {
        for(int k = 0; k < height; k++){
            free(p[j][k]);
            p[j][k] = NULL;
        }
    }

    for (int j = 0; j < channels; j++) {
        free(p[j]);
        p[j] = NULL;
    }

    free(p);
    p = NULL;
    printf("free_3D_array has done.\n");
    return 0;
}

int free_4D_array(float**** p){
    int filter_nums = _msize(p) / sizeof(float***);
    int channels = _msize(p[0]) / sizeof(float**);
    int height = _msize(p[0][0]) / sizeof(float*);
    int width =  _msize(p[0][0][0]) / sizeof(float);

    for (int i = 0;i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            for(int k = 0; k < height; k++){
                free(p[i][j][k]);
                p[i][j][k] = NULL;
            }
        }
    }
    for (int i = 0;i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            free(p[i][j]);
            p[i][j] = NULL;
        }
    }
    for (int i = 0;i < filter_nums; i++) {
        free(p[i]);
        p[i] = NULL;
    }
    free(p);
    p = NULL;
    printf("free_4D_array has done.\n");
    return 0;
}

void print_4D_array(float ****p){
    int filter_nums = _msize(p) / sizeof(float***);
    int channels = _msize(p[0]) / sizeof(float**);
    int height = _msize(p[0][0]) / sizeof(float*);
    int width =  _msize(p[0][0][0]) / sizeof(float);
    int idx = 0;
    for (int i = 0; i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            for (int k = 0; k < height; k++) {
                for(int l = 0; l < width; l++){
                    printf("%d th, %f\n", idx, p[i][j][k][l]);
                    idx++;
                }
            }
        }
    }
}