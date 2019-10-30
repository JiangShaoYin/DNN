//
// Created by Administrator on 2019/10/30.
//
# include "dnn.h"
# include "stdio.h"
#include <fcntl.h>
# include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


float**** malloc_multi_array(int width, int height, int channels, int filter_nums) {
    float ****para_array = (float****)malloc(sizeof(float***) * filter_nums);  //申请最外围的空间

    for (int i = 0;i < filter_nums; i++) {
        para_array[i] = (float***)malloc(sizeof(float**) * channels);
        for (int j = 0; j < channels; j++) {
            para_array[i][j] = (float **) malloc(sizeof(float *) * height);
            for(int k = 0; k < height; k++){
                para_array[i][j][k] = (float *) malloc(sizeof(float ) * width);
            }
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

int free_multi_array(float**** p, int width, int height, int channels, int filter_nums){
    for (int i = 0;i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            for(int k = 0; k < height; k++){
                free(p[i][j][k]);
            }
        }
    }
    for (int i = 0;i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            free(p[i][j]);
        }
    }
    for (int i = 0;i < filter_nums; i++) {
        free(p[i]);
    }
    free(p);
    return 0;
}

void print_multi_array(float ****p, int width, int height, int channels, int filter_nums){
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