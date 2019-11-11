//
// Created by Administrator on 2019/10/30.
//
# include "../include/dnn.h"
# include "../include/I_O.h"
# include "../include/memory_manage.h"
# include "stdio.h"
# include <fcntl.h>
# include <stdlib.h>
# include <windef.h>
# include <math.h>
# include "../lib/stb_image.h"


float*** feature_padding(float*** input_pic, int padding) {
    int channels = _msize(input_pic) / sizeof(float**);
    int height = _msize(input_pic[0]) / sizeof(float*);
    int width =  _msize(input_pic[0][0]) / sizeof(float);

    int new_height = height + 2 * padding; // padding之后的高
    int new_width = width + 2 * padding;
    float*** matrix_after_padding = malloc_3D_array(new_width, new_height, channels); // 为padding之后的matrix申请空间

    for (int i = 0; i < channels; i++) {
        for (int j = 1; j < new_height - 1; j++) { // 第0行，和最后一行保留原始值0
            memcpy(&matrix_after_padding[i][j][1], input_pic[i][j - 1], width * sizeof(float)); // &matrix_after_padding[i][j][1],从width第1个值后开始填充，
        }
    }

    free_3D_array(input_pic); // 释放掉之前input_pic的空间
    return matrix_after_padding;
}

float*** convert_vector_2_3Dmatrix(unsigned char* input_pic, int width, int height, int channels) {  // 将读出来的jpg转成3D矩阵，type由unsigned char转为float
    float*** three_D_array= malloc_3D_array(width, height, channels);
    int pixel_nums = width * height * channels;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            float R = (float) (input_pic[channels * width * row + col * channels + 0]);
            float G = (float) (input_pic[channels * width * row + col * channels + 1]);
            float B = (float) (input_pic[channels * width * row + col * channels + 2]);
            three_D_array[0][row][col] = B;  // 恢复成opencv里面brg的打开方式
            three_D_array[1][row][col] = G;
            three_D_array[2][row][col] = R;
        }
    }

    stbi_image_free(input_pic);  // 释放上一级占用的内存空间
    return three_D_array;
}

float* convert_3Dmatrix_2_vector(float*** input_tensor) { // 将featuremap转成一维tensor，方便全连接层计算
    int input_channels = _msize(input_tensor) / sizeof(float**);
    int input_height = _msize(input_tensor[0]) / sizeof(float*);
    int input_width =  _msize(input_tensor[0][0]) / sizeof(float);

    float* output = (float*)malloc(sizeof(float) * input_channels * input_height * input_width);

    for (int channel = 0; channel < input_channels; channel++) {
        for (int row = 0; row < input_height; row++) {
            for (int col = 0; col < input_width; col++) {
                int idx = channel * input_height * input_width + row * input_width + col; // input_tensor[channel][row][col] 对应idx在一维vector上所指向的值
                output[idx] = input_tensor[channel][row][col];
            }
        }
    }
    free_3D_array(input_tensor); // 释放输入matrix的空间
    return output;
}

float*** conv3D(float*** input_tensor, char* parameter_file, int conv_width, int conv_height,
        int conv_channels,  int kernel_nums, int stride, int padding, char* input_name, char* conv_name, char* output_name) {
    // input_tensor的shape
    int channels = _msize(input_tensor) / sizeof(float**);
    int height = _msize(input_tensor[0]) / sizeof(float*);
    int width =  _msize(input_tensor[0][0]) / sizeof(float);
    // 先把输入变量保存一下
    if (SAVE_FILE) {
        save_3Dmatrix_to_disk(input_name, input_tensor);
    }
    // 执行padding
    float*** matrix_after_padding = feature_padding(input_tensor, padding);

    // 计算卷积后的尺寸
    int output_widths = (int)((width + 2 * padding - conv_width) / stride) + 1; // 向下取整
    int output_heights = (int)((height + 2 * padding - conv_height) / stride) + 1;
    int output_channels = kernel_nums;

    // 申请内存， 存储卷积输出结果
    float*** out_put_matrix = malloc_3D_array(output_widths, output_heights, output_channels);

    // 创建4D matrix， 读conv1层的参数到parameters矩阵中
    float**** parameters = malloc_4D_array(conv_width, conv_height, conv_channels, kernel_nums);
    // 读参数到parameters中（4D matrix）
    load_conv_Parameters(parameter_file, parameters, conv_width, conv_height, conv_channels, kernel_nums);
    // 保存conv的参数到磁盘
    if (SAVE_FILE) {
        save_4Dmatrix_to_disk(conv_name, parameters);
    }

    for (int kernel_idx = 0; kernel_idx < kernel_nums; kernel_idx++) {
        for (int output_height = 0; output_height < output_heights; output_height++) {
            for (int output_width = 0; output_width < output_widths; output_width++) { // 遍历out_put_matrix上的每个pixel，output_width_idx最大值为224

                out_put_matrix[kernel_idx][output_height][output_width] = ceil_conv(conv_width, conv_height, conv_channels, kernel_idx,
                        output_width, output_height, stride, matrix_after_padding, parameters);

            }
        }
    }
    free_3D_array(matrix_after_padding); // 释放input空间
    free_4D_array(parameters);           // 释放参数占用的内存空间
    if (SAVE_FILE) {
        save_3Dmatrix_to_disk(output_name, out_put_matrix); // 保存卷积层计算结果
    }
    printf("convolution computing completed! \n");
    return out_put_matrix;
}

float ceil_conv(int conv_widths, int conv_heights, int channels, int kernel,  // 卷积核参数
        int output_width, int output_height, int stride, float*** input, float**** parameters) {

    float conv_operation_ans = 0;
    for (int channel = 0; channel < channels; channel++) {
        for (int conv_height = 0; conv_height < conv_heights; conv_height++) {
            for (int conv_width = 0; conv_width < conv_widths; conv_width++) {

                int height = output_height * stride + conv_height;  // 输入向量当前的参与计算的点的坐标 = 输出向量的坐标conv_height_idx + 卷积当前的偏移
                int width = output_width * stride + conv_width;    // 最右边坐标为223 + 2 = 225
                conv_operation_ans += input[channel][height][width] * parameters[kernel][channel][conv_height][conv_width];

            }
        }
    }
    return  conv_operation_ans;
}

float*** relu3D(float*** input_tensor, char* output_name) {
    int channels = _msize(input_tensor) / sizeof(float**);
    int height = _msize(input_tensor[0]) / sizeof(float*);
    int width =  _msize(input_tensor[0][0]) / sizeof(float);

    for (int i = 0; i < channels; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < width; k++) {
                if (input_tensor[i][j][k] < 0) {
                    input_tensor[i][j][k] = 0;
                }
            }
        }
    }
    if (SAVE_FILE) {
        save_3Dmatrix_to_disk(output_name, input_tensor);
    }
    printf("RELU computing completed! \n");
    return input_tensor;
}

float* relu(float* input_tensor, char* output_name) {
    int width =  _msize(input_tensor) / sizeof(float);

    for (int k = 0; k < width; k++) {
        if (input_tensor[k] < 0) {
            input_tensor[k] = 0;
        }
    }
    if (SAVE_FILE) {
        save_vector_to_disk(output_name, input_tensor);
    }
    printf("RELU computing completed! \n");
    return input_tensor;
}

float*** maxpooling(float*** input_tensor, int kernel_size, int stride, int padding, char* output_name) {
    // input_tensor的shape
    int channels = _msize(input_tensor) / sizeof(float**);
    int height = _msize(input_tensor[0]) / sizeof(float*);
    int width =  _msize(input_tensor[0][0]) / sizeof(float);

    // 计算maxpooling后的尺寸
    int output_width = (int)((width + 2 * padding - kernel_size) / stride) + 1; // 向下取整
    int output_height = (int)((height + 2 * padding - kernel_size) / stride) + 1;
    int output_channels = channels;

    // 申请内存， 存放maxpool的输出结果
    float*** output_matrix = malloc_3D_array(output_width, output_height, output_channels);

    for (int output_channel = 0; output_channel < channels; output_channel++) {
        for (int output_height_idx = 0; output_height_idx < output_height; output_height_idx++) {
            for (int output_width_idx = 0; output_width_idx < output_width; output_width_idx++) { // 遍历out_put_matrix上的每个pixel，output_width_idx最大值为223

                float max_ans = 0;  // 承接每次max pooling的计算结果

                for (int conv_height_idx = 0; conv_height_idx < kernel_size; conv_height_idx += stride) {
                    for (int conv_width_idx = 0; conv_width_idx < kernel_size; conv_width_idx += stride) {
                        int i = output_height_idx + conv_height_idx;  // 输入向量当前的参与计算的点的坐标 = 输出向量的坐标conv_height_idx + 卷积当前的偏移
                        int j = output_width_idx + conv_width_idx;    // 最右边坐标为223 + 2 = 225
                        max_ans = max(max_ans, input_tensor[output_channel][i][j]);
                    }
                }

                output_matrix[output_channel][output_height_idx][output_width_idx] = max_ans;
            }
        }
    }

    free_3D_array(input_tensor);
    // 保存输出tensor
    if (SAVE_FILE) {
        save_3Dmatrix_to_disk(output_name, output_matrix);
    }
    printf("maxpooling completed! \n\n");
    return output_matrix;
}

float* linear(float* input_tensor, char* file_weight, char* file_bias, int width, int height, char* fc_name, char* output_name) {

    printf("fully connected layer computing begin! \n");

    // 创建2D matrix， 读fc层的参数到weight矩阵中
    float** weight = malloc_2D_array(width, height);
    // 读参数到parameters中（2D matrix）
    load_weight_Parameters(file_weight, weight, width, height);


    // 读bias的参数搭配bias中
    float* bias = (float*)malloc(sizeof(float)* width);
    load_bias_Parameters(file_bias, bias, width);

    // 申请 output_vector的内存
    float* output_vector = (float*)malloc(sizeof(float)* width);

    for (int i = 0; i < width; i++) {
        float fc_operate_ans = 0;
        for (int j = 0; j < height; j++) {
            fc_operate_ans += input_tensor[j] * weight[j][i];  // 让weight第i列的所有元素与input_tensor中所有元素相乘
        }
        output_vector[i] = fc_operate_ans;
    }


    // 保存fc的参数到磁盘
    if (SAVE_FILE) {
        save_vector_to_disk(output_name, output_vector);
    }

    free(input_tensor);
    input_tensor = NULL;
    free_2D_array(weight);
    free(bias);
    printf("fully connected computing completed! \n");
    return output_vector;
}
float* dropout(float* input_tensor, float p, char* output_name) {
    int width =  _msize(input_tensor) / sizeof(float);

    for (int i = 0; i < width; i++) {
        input_tensor[i] *= p;
    }
    if (SAVE_FILE) {
        save_vector_to_disk(output_name, input_tensor);
    }
    printf("dropout completed! \n\n");
    return input_tensor;
}
float averagepool(float* input_tensor) { // 一维tensor的平均值
    int width =  _msize(input_tensor) / sizeof(float);
    float ans = 0;
    for (int i = 0; i < width; i++) {
        ans += input_tensor[i];
    }

    free(input_tensor);
    input_tensor = NULL;
    return ans / width;
}
void softmax(float* input_tensor) {
    int width =  _msize(input_tensor) / sizeof(float);
    float total = 0;
    for (int i = 0; i < width; i++) {
        total += exp(input_tensor[i]);
    }
    for (int i = 0; i < width; i++) {
        input_tensor[i] = exp(input_tensor[i]) / total;
    }

    return ;
}

int weighted_average(float* input_tensor) {
    int width =  _msize(input_tensor) / sizeof(float);
    float ans = 0;
    for (int i = 0; i < width; i++) {
        ans += input_tensor[i] * (i+1);
    }

    free(input_tensor);
    input_tensor = NULL;
    return (int)ans;
}


int forward(char* filename) {
    int channels, height, width; // 接收stb库打开图片读出来的高,宽,长

    unsigned char* input_pic = stbi_load(filename, &width, &height, &channels, 0); // 按照rgb顺序打开, 申请空间, opencv是用brg打开，要换一下
    float*** input_matrix = convert_vector_2_3Dmatrix(input_pic, width, height, channels); // stblib里面打开jpg用的一维数组，这里将其转换为3D，并将pixel从unsigned char 变为float

    // layer1
    float*** conv1_before_relu3D = conv3D(input_matrix, "../parameters/_features.0.weight", // 从parameter_file读出参数
                                          3, 3, 3,  64, 1, 1,                           // conv == 3*3*3*64, stride = 1, padding =1
                                          "../conv1_input.txt", "../conv1.txt", "../conv1_before_relu3D.txt"); // // 保存输入tensor到conv1_input.txt， 卷积参数到conv1.txt， 输出结果到conv1_before_relu3D.txt
    float*** conv1_relu3D = relu3D(conv1_before_relu3D, "../conv1_relu3D.txt");        // 输出结果到conv1_relu3D.txt
    float*** conv1_max_pooing = maxpooling(conv1_relu3D,2,2,0,"../conv1_max_pooing.txt"); // 输出结果到conv1_max_pooing.txt

    // layer2
    float*** conv2_before_relu3D = conv3D(conv1_max_pooing,"../parameters/_features.3.weight",
                                          3, 3, 64,  128, 1, 1,
                                          "../conv2_input.txt", "../conv2.txt", "../conv2_before_relu3D.txt");
    float*** conv2_relu3D = relu3D(conv2_before_relu3D, "../conv2_relu3D.txt");
    float*** conv2_max_pooing = maxpooling(conv2_relu3D,2,2,0,"../conv2_max_pooing.txt");

    // layer3
    float*** conv3_before_relu3D = conv3D(conv2_max_pooing,"../parameters/_features.6.weight",
                                          3, 3, 128, 256, 1, 1,
                                          "../conv3_input.txt", "../conv3.txt", "../conv3_before_relu3D.txt");
    float*** conv3_relu3D = relu3D(conv3_before_relu3D, "../conv3_relu3D.txt");

    // layer4
    float*** conv4_before_relu3D = conv3D(conv3_relu3D,"../parameters/_features.8.weight",
                                          3, 3, 256,  256, 1, 1,
                                          "../conv4_input.txt", "../conv4.txt", "../conv4_before_relu3D.txt");
    float*** conv4_relu3D = relu3D(conv4_before_relu3D, "../conv4_relu3D.txt");
    float*** conv4_max_pooing = maxpooling(conv4_relu3D,2,2,0,"../conv4_max_pooing.txt");

    // layer5
    float*** conv5_before_relu3D = conv3D(conv4_max_pooing,"../parameters/_features.11.weight", 
                                          3, 3, 256,  512, 1, 1,
                                          "../conv5_input.txt", "../conv5.txt", "../conv5_before_relu3D.txt");
    float*** conv5_relu3D = relu3D(conv5_before_relu3D, "../conv5_relu3D.txt");

    // layer6
    float*** conv6_before_relu3D = conv3D(conv5_relu3D,"../parameters/_features.13.weight", 
                                          3, 3, 512,  512, 1, 1,
                                          "../conv6_input.txt", "../conv6.txt", "../conv6_before_relu3D.txt");
    float*** conv6_relu3D = relu3D(conv6_before_relu3D, "../conv6_relu3D.txt");
    float*** conv6_max_pooing = maxpooling(conv6_relu3D, 2, 2, 0, "../conv4_max_pooing.txt");

    // layer7
    float*** conv7_before_relu3D = conv3D(conv6_max_pooing,"../parameters/_features.16.weight", 
                                          3, 3, 512,  512, 1, 1,
                                          "../conv7_input.txt", "../conv7.txt", "../conv7_before_relu3D.txt");
    float*** conv7_relu3D = relu3D(conv7_before_relu3D, "../conv7_relu3D.txt");

    // layer8
    float*** conv8_before_relu3D = conv3D(conv7_relu3D,"../parameters/_features.18.weight",
                                          3, 3, 512,  512, 1, 1,
                                          "../conv8_input.txt", "../conv8.txt", "../conv8_before_relu3D.txt");
    float*** conv8_relu3D = relu3D(conv8_before_relu3D, "../conv8_relu3D.txt");
    float*** conv8_max_pooing = maxpooling(conv8_relu3D, 2, 2, 0, "../conv8_max_pooing.txt");

    // fc0
    float* fc0_input = convert_3Dmatrix_2_vector(conv8_max_pooing);  // 将featuremap转成一维tensor，方便全连接层计算

    float* fc0 = linear(fc0_input, "../parameters/linear0.weight", "../parameters/linear0.bias", // 从txt中读出w和b
                         4096, 25088, "../fc0.txt", "../fc0_before_relu.txt"); //全连接层的shape是4096 * 25088，参数保存到fc0.txt，保存全连接层的计算结果到fc0_before_relu.txt
    float* fc0_relu = relu(fc0, "../fc0_relu.txt");
    float* fc0_dropout = dropout(fc0_relu, 0.5, "../fc0_dropout.txt");


    float* fc1 = linear(fc0_dropout, "../parameters/linear1.weight", "../parameters/linear1.bias", 4096, 4096, "../fc1.txt", "../fc1_before_relu.txt");
    float* fc1_relu = relu(fc1, "../fc1_relu.txt");
    float* fc1_dropout = dropout(fc1_relu, 0.5, "../fc1_dropout.txt");


    float* fc2 = linear(fc1_dropout, "../parameters/last_linear.weight", "../parameters/last_linear.bias", 101, 4096, "../fc2.txt", "../fc2_before_relu.txt");

    softmax(fc2);
    int ans = weighted_average(fc2);
    return ans;
}