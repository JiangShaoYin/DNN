//
// Created by Administrator on 2019/10/31.
//
# include "../include/I_O.h"
# include "../include/memory_manage.h"
# include "../include/dnn.h"
# include "string.h"
# include <malloc.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>

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
//        printf("parameter：%s\n", parameter);
        parameters[idx_filter][idx_channels][idx_height][idx_width] = atof(parameter);
//        printf("%.8f\n",parameters[idx_filter][idx_channels][idx_height][idx_width]);
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

int save_3Dmatrix_to_disk(char* filepath, float*** p){
    FILE* fp = fopen(filepath, "w");

    int channels = _msize(p) / sizeof(float**);
    int height = _msize(p[0]) / sizeof(float*);
    int width =  _msize(p[0][0]) / sizeof(float);

    int idx = 0;
    for (int j = 0; j < channels; j++) {
        fprintf(fp,"\n\nchannel %d: ", j);
        for (int k = 0; k < height; k++) {
            fprintf(fp,"\nrow %d: \n", k);
            for(int l = 0; l < width; l++){
                fprintf(fp, "%.8f ", p[j][k][l]);
                idx++;
            }
        }
    }
    fclose(fp);
    return 0;
}
int save_4Dmatrix_to_disk(char* filepath, float**** p){
    FILE* fp = fopen(filepath, "w");

    int filter_nums = _msize(p) / sizeof(float***);
    int channels = _msize(p[0]) / sizeof(float**);
    int height = _msize(p[0][0]) / sizeof(float*);
    int width =  _msize(p[0][0][0]) / sizeof(float);

    for (int i = 0; i < filter_nums; i++) {
        fprintf(fp,"\n\nkernel %d: ", i);
        for (int j = 0; j < channels; j++) {
            fprintf(fp,"\n\nchannel %d: ", j);
            for (int k = 0; k < height; k++) {
                fprintf(fp,"\nrow %d: \n", k);
                for(int l = 0; l < width; l++){
                    fprintf(fp, "%.8f ", p[i][j][k][l]);
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

