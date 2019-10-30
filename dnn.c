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


int loadParameters(char* path, float* parameters, int height, int width, int channles, int filter_nums){
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        int errNum = errno;
        printf("open fail errno = %d", errNum);
        return -1;
    }

    char parameter[30] ;       // 读dat文件里面保存的每个参数
    int idx = 0;               // 第idx个参数

    while(feof(fp) == 0) { // 文件未到末尾,feof返回0
        memset(parameter, 0, 30);  // 初始清空缓冲区
        fgets(parameter, 99, fp);  // 从fp中最多读99个char到input里面，如果提前遇到\n则读取结束
        parameters[idx++] = atof(parameter);
        printf("the %d th paremeter in buffer\n", idx);
    }
    fclose(fp);
    return 0;
}
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
int free_multi_array(int width, int height, int channels, int filter_nums){
    for (int i = 0;i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            for(int k = 0; k < height; k++){
                free(array[i][j][k]);
            }
        }
    }

    for (int i = 0;i < filter_nums; i++) {
        for (int j = 0; j < channels; j++) {
            free(array[i][j]);
        }
    }

    for (i = 0;i < filter_nums; i++) {
        free(array[i]);
    }

    free(array);
    return 0
}