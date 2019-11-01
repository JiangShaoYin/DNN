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

int load_conv_Parameters(char* path, float**** parameters, int width, int height, int channels, int filter_nums){ // 加载conv参数到卷积层
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
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
        parameters[idx_filter][idx_channels][idx_height][idx_width] = atof(parameter); // 将读出的sting数转为float，存储起来
        idx++;

    }
    fclose(fp);
    return 0;
}
int load_weight_Parameters(char* path, float** parameters, int width, int height){ // 加载weight参数到fc层
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        int errNum = errno;
        printf("open fail errno = %d", errNum);
        return -1;
    }

    char parameter[30] ;       // 读dat文件里面保存的每个参数
    int idx = 0;               // 第idx个参数

    int parameter_nums = width * height;

    int idx_height=0, idx_width=0;
    while(feof(fp) == 0 && idx < parameter_nums) { // 文件未到末尾,feof返回0

        idx_height = idx / width; // 101 % 3 * 3 * 3 == 20,   20 % 9 = 2, 2/3 = 0
        idx_width = idx % width;  // 2 %3 = 2

        memset(parameter, 0, 30);  // 初始清空缓冲区
        fgets(parameter, 99, fp);  // 从fp中最多读99个char到input里面，如果提前遇到\n则读取结束

        parameters[idx_height][idx_width] = atof(parameter);
        idx++;

    }
    fclose(fp);
    return 0;
}

int load_bias_Parameters(char* path, float* bias, int width){  // 加载bias参数到fc层
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        int errNum = errno;
        printf("open fail errno = %d", errNum);
        return -1;
    }

    char parameter[30] ;       // 读dat文件里面保存的每个参数

    int idx = 0;               // 第idx个参数
    while(feof(fp) == 0 && idx < width) { // 文件未到末尾,feof返回0

        memset(parameter, 0, 30);  // 初始清空缓冲区
        fgets(parameter, 99, fp);  // 从fp中最多读99个char到input里面，如果提前遇到\n则读取结束
//        printf("%dth: %s", idx,parameter);
        bias[idx++] = atof(parameter);
    }
    fclose(fp);
    return 0;
}

int load_fc_result_2_memory(char* path, float* output, int variables_nums){ // 将指定全连接层的输出，读入内存
    return load_bias_Parameters(path, output, variables_nums);
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

int save_vector_to_disk(char* filepath, float* arrar_to_be_saved){
    FILE* fp = fopen(filepath, "w");
    int width =  _msize(arrar_to_be_saved) / sizeof(float);

    for(int i = 0; i < width; i++){
        fprintf(fp, "%.8f\n", arrar_to_be_saved[i]);
    }

    fclose(fp);
    return 0;
}
int save_2Dmatrix_to_disk(char* filepath, float** arrar_to_be_saved){
    FILE* fp = fopen(filepath, "w");

    int height = _msize(arrar_to_be_saved) / sizeof(float*);
    printf("%d\n", height);
    int width =  _msize(arrar_to_be_saved[0]) / sizeof(float);
    printf("%d\n", width);

    int idx = 0;
    for (int i = 0; i < height; i++) {
        fprintf(fp,"\nrow %d: \n", i);
        for(int j = 0; j < width; j++){
            fprintf(fp, "%.8f ", arrar_to_be_saved[i][j]);
            idx++;
        }
    }

    fclose(fp);
    return 0;
}
int save_3Dmatrix_to_disk(char* filepath, float*** arrar_to_be_saved){
    FILE* fp = fopen(filepath, "w");

    int channels = _msize(arrar_to_be_saved) / sizeof(float**);
    int height = _msize(arrar_to_be_saved[0]) / sizeof(float*);
    int width =  _msize(arrar_to_be_saved[0][0]) / sizeof(float);

    int idx = 0;
    for (int j = 0; j < channels; j++) {
        fprintf(fp,"\n\nchannel %d: ", j);
        for (int k = 0; k < height; k++) {
            fprintf(fp,"\nrow %d: \n", k);
            for(int l = 0; l < width; l++){
                fprintf(fp, "%.8f ", arrar_to_be_saved[j][k][l]);
                idx++;
            }
        }
    }
    fclose(fp);
    return 0;
}
int save_4Dmatrix_to_disk(char* filepath, float**** arrar_to_be_saved){
    FILE* fp = fopen(filepath, "w");

    int filter_nums = _msize(arrar_to_be_saved) / sizeof(float***);
    int channels = _msize(arrar_to_be_saved[0]) / sizeof(float**);
    int height = _msize(arrar_to_be_saved[0][0]) / sizeof(float*);
    int width =  _msize(arrar_to_be_saved[0][0][0]) / sizeof(float);

    for (int i = 0; i < filter_nums; i++) {
        fprintf(fp,"\n\nkernel %d: ", i);
        for (int j = 0; j < channels; j++) {
            fprintf(fp,"\n\nchannel %d: ", j);
            for (int k = 0; k < height; k++) {
                fprintf(fp,"\nrow %d: \n", k);
                for(int l = 0; l < width; l++){
                    fprintf(fp, "%.8f ", arrar_to_be_saved[i][j][k][l]);
                }
            }
        }
    }
    fclose(fp);
    return 0;
}

float* load_calculate_result_2_memeory(char* path, int variables_nums){ // 将中间过程产生的计算结果，读入内存
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        int errNum = errno;
        printf("open fail errno = %d", errNum);
        return NULL;
    }

    float* output = (float*)malloc(sizeof(float) * variables_nums);

    char parameter[30] ;       // 读dat文件里面保存的每个参数
    int idx = 0;               // 第idx个参数


    while(feof(fp) == 0) { // 文件未到末尾,feof返回0

        memset(parameter, 0, 30);  // 初始清空缓冲区
        fgets(parameter, 99, fp);  // 从fp中最多读99个char到input里面，如果提前遇到\n则读取结束

        output[idx] = atof(parameter);
        idx++;

    }
    fclose(fp);
    return 0;
}

