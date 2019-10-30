//
// Created by Administrator on 2019/10/28.
# include "dnn.h"
# include <stdio.h>


int main(){
    int width = 3;
    int height = 3;
    int channels = 3;
    int filter_nums = 64;



    float**** parameters = malloc_multi_array(3,3,3,64);
    loadParameters("../para_test.dat", parameters,  width, height, channels, filter_nums);
    print_multi_array(parameters,width, height, channels, filter_nums);
    free_multi_array(parameters,width, height, channels, filter_nums);

    return 0;
}

