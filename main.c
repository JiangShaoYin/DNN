//
// Created by Administrator on 2019/10/28.
# include "include/dnn.h"
# include "include/I_O.h"
# include <stdio.h>
# include "./lib/stb_image.h"


char* parameter_file = "../para_test.dat";

int main(){

    char* filename = "../face.jpg";
    int channels, height, width;
    int conv_width, conv_height, conv_channels, kernel_nums, stride, padding;

    unsigned char* input_pic = stbi_load(filename, &width, &height, &channels, 0); // 按照rgb顺序打开, 申请空间
    float*** input_matrix = convert_vector_2_3Dmatrix(input_pic, width, height, channels); // stblib里面打开jpg用的一维数组，这里将其转换为3D，并将pixel从unsigned char 变为float

    conv1(input_matrix, parameter_file, conv_width = 3, conv_height= 3, conv_channels = 3,  kernel_nums =64, stride = 1, padding = 1);





//
//    int stride, padding;
//    float*** x;
//    x = conv1(input_pic, width=3, height=3, channels=3, filter_nums=64, stride=2, padding=1);
//
//
//    print_4D_array(parameters);
//    free_4D_array(parameters);
//
//    return 0;
}

