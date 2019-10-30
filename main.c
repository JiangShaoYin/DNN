//
// Created by Administrator on 2019/10/28.
# include "dnn.h"
# include <stdio.h>

int main(){
    int width = 3;
    int height = 3;
    int channels = 3;
    int filter_nums = 64;
    int parameter_nums = height * width * channels * filter_nums;






    float (*parameters)[channels][height][width] = (float(*)[channels][height][width])malloc(parameter_nums* sizeof(float));
    printf("%d\n", sizeof(parameters)/sizeof(float));
    loadParameters("../para_test.dat", parameters,  width, height, channels, filter_nums);
    int a[height];
    a[0] = 1;
    printf("%d\n", a[0]);
    return 0;
}

