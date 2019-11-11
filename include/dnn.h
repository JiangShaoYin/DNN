//
// Created by Administrator on 2019/10/30.
//

#ifndef DNN_H
#define DNN_H
#include <stdbool.h>

extern bool SAVE_FILE;
// tensor，shape的变化，以及tensor的padding操作
float*** feature_padding(float*** input_pic, int padding);
float*** convert_vector_2_3Dmatrix(unsigned char* input_pic, int width, int height, int channels); // 将stb库里面保存图片数据的vector转成3D matrix
float* convert_3Dmatrix_2_vector(float*** input_tensor); // 将featuremap转成一维tensor，方便全连接层计算

// 卷积操作
float*** conv3D(float*** input_tensor, char* parameter_file, int conv_width, int conv_height, // conv_width * conv_height * conv_channels的卷积操作
               int conv_channels,  int kernel_nums, int stride, int padding,
               char* input_name, char* conv_name, char* output_name);// 保存输入tensor到input_name所指向的文件， 卷积参数到conv_name所指向的文件， 输出结果到output_name所指向的文件
float ceil_conv(int conv_widths, int conv_heights, int channels, int kernel,  // 卷积核参数
                int output_width, int output_height, int stride, float*** input, float**** parameters);
float*** relu3D(float*** input_tensor, char* output_name);// 在matrix上的relu， 输出结果到output_name所指向的文件
float*** maxpooling(float*** input_tensor, int kernel_size, int stride, int padding, char* output_name);

// fc层操作
float* linear(float* input_tensor, char* parameter_wight, char* parameter_bias, int width, int height,
        char* fc_name, char* output_name);  // 全连接层，fc参数到fc_name所指向的文件， 输出结果到output_name所指向的文件
float* relu(float* input_tensor, char* output_name); // 在vector上的erlu
float* dropout(float* input_tensor, float p, char* output_name); // p是probability ，定义dropout的比率
float averagepool(float* input_tensor); // 对一维的vector求平均值

// 输出操作
void softmax(float* input_tensor);
int weighted_average(float* input_tensor); // 计算年龄的加权平均值

int forward(char* filename); // filename指向一张 224 * 224的图片

#endif //DNN_H

