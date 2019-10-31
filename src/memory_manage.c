//
// Created by Administrator on 2019/10/31.
//

#include "../include/memory_manage.h"
# include "string.h"
# include "stdio.h"
# include "stdlib.h"


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
float** malloc_2D_array(int width, int height) {
    float **para_array = (float**)malloc(sizeof(float*) * height);  // 申请height个vector

    for (int i = 0;i < height; i++) {
            para_array[i] = (float *) malloc(sizeof(float) * width);// 申请长度为width的数组
            memset(para_array[i], 0, width);
    }
    return para_array;
}
int free_2D_array(float** p){
    int height = _msize(p) / sizeof(float*);
    int width = _msize(p[0]) / sizeof(float);

    for (int j = 0; j < height; j++) {
        free(p[j]);
        p[j] = NULL;
    }

    free(p);
    p = NULL;
    printf("free_2D_array has done.\n");
    return 0;
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