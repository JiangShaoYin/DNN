//
// Created by Administrator on 2019/10/30.
//
# include "../include/dnn.h"
# include "../include/I_O.h"
# include "../include/memory_manage.h"

# include "stdio.h"
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <windows.h>
# include <windef.h>


float*** feature_padding(float*** input_pic, int padding){
    int channels = _msize(input_pic) / sizeof(float**);
    int height = _msize(input_pic[0]) / sizeof(float*);
    int width =  _msize(input_pic[0][0]) / sizeof(float);

    int new_height = height + 2 * padding;
    int new_width = width + 2 * padding;
    float*** matrix_after_padding = malloc_3D_array(new_width, new_height, channels); // padding之后的数组
    for(int i = 0; i < channels; i++){
        for(int j = 1; j < new_height - 1; j++){ // 第0行，和最后一行保留原始值0
            memcpy(&matrix_after_padding[i][j][1], input_pic[i][j - 1], width * sizeof(float)); // width第1个值不填充
        }
    }

    free_3D_array(input_pic); // 释放掉之前input_pic的空间
    return matrix_after_padding;
}

float*** convert_vector_2_3Dmatrix(unsigned char* input_pic, int width, int height, int channels){  // 将读出来的jpg转成3D矩阵，type由unsigned char转为float
    float*** three_D_array= malloc_3D_array(width, height, channels);
    int pixel_nums = width * height * channels;

    for(int row = 0; row < height; row++){
        for(int col = 0; col < width; col++) {
            float R = (float) (input_pic[channels * width * row + col * channels + 0]);
            float G = (float) (input_pic[channels * width * row + col * channels + 1]);
            float B = (float) (input_pic[channels * width * row + col * channels + 2]);
            three_D_array[0][row][col] = R;
            three_D_array[1][row][col] = G;
            three_D_array[2][row][col] = B;
        }
    }

    stbi_image_free(input_pic);  // 释放上一级占用的内存空间
    return three_D_array;
}

float* convert_3Dmatrix_2_vector(float*** input_tensor){
    int input_channels = _msize(input_tensor) / sizeof(float**);
    int input_height = _msize(input_tensor[0]) / sizeof(float*);
    int input_width =  _msize(input_tensor[0][0]) / sizeof(float);

    float* output = (float*)malloc(sizeof(float) * input_channels * input_height * input_width);

    for(int channel = 0; channel < input_channels; channel++){
        for(int row = 0; row < input_height; row++){
            for(int col = 0; col < input_width; col++){
                int idx = channel * input_height * input_width + row * input_width + col;
                output[idx] = input_tensor[channel][row][col];
            }
        }
    }
    free_3D_array(input_tensor);
    return output;
}

float*** conv3D(float*** input_tensor, char* parameter_file, int conv_width, int conv_height,
        int conv_channels,  int kernel_nums, int stride, int padding, char* input_name, char* conv_name, char* output_name){
    // input_tensor的shape
    int channels = _msize(input_tensor) / sizeof(float**);
    int height = _msize(input_tensor[0]) / sizeof(float*);
    int width =  _msize(input_tensor[0][0]) / sizeof(float);
    // 先把输入变量保存一下
    if(SAVE_FILE) {
        save_3Dmatrix_to_disk(input_name, input_tensor);
    }
    // 执行padding
    float*** matrix_after_padding = feature_padding(input_tensor, padding);

    // 计算卷积后的尺寸
    int output_width = (int)((width + 2 * padding - conv_width) / stride) + 1; // 向下取整
    int output_height = (int)((height + 2 * padding - conv_height) / stride) + 1;
    int output_channels = kernel_nums;

    // 申请内存， 存储卷积输出结果
    float*** out_put_matrix = malloc_3D_array(output_width, output_height, output_channels);

    // 创建4D matrix， 读conv1层的参数到parameters矩阵中
    float**** parameters = malloc_4D_array(conv_width, conv_height, conv_channels, kernel_nums);
    // 读参数到parameters中（4D matrix）
    load_conv_Parameters(parameter_file, parameters, conv_width, conv_height, conv_channels, kernel_nums);
    // 保存conv的参数到磁盘
    if(SAVE_FILE) {
        save_4Dmatrix_to_disk(conv_name, parameters);
    }

    for(int kernel_idx=0; kernel_idx < kernel_nums; kernel_idx++){
        for(int output_height_idx=0; output_height_idx < output_height; output_height_idx++){
            for(int output_width_idx=0; output_width_idx < output_width; output_width_idx++){ // 遍历out_put_matrix上的每个pixel，output_width_idx最大值为224

                float conv_operation_ans = 0;  // 承接每次卷积的计算结果

                for(int conv_channel_idx=0; conv_channel_idx < channels; conv_channel_idx++){
                    for(int conv_height_idx=0; conv_height_idx < conv_height; conv_height_idx++){
                        for(int conv_width_idx=0; conv_width_idx < conv_width; conv_width_idx++){
                            int i = output_height_idx + conv_height_idx;  // 输入向量当前的参与计算的点的坐标 = 输出向量的坐标conv_height_idx + 卷积当前的偏移
                            int j = output_width_idx + conv_width_idx;    // 最右边坐标为223 + 2 = 225
                            conv_operation_ans +=
                                    matrix_after_padding[conv_channel_idx][i][j]
                                    * parameters[kernel_idx][conv_channel_idx][conv_height_idx][conv_width_idx];
                        }
                    }
                }
                out_put_matrix[kernel_idx][output_height_idx][output_width_idx] = conv_operation_ans;
            }
        }
    }
    free_3D_array(matrix_after_padding); // 释放input空间
    free_4D_array(parameters);           // 释放参数占用的内存空间
    if(SAVE_FILE) {
        save_3Dmatrix_to_disk(output_name, out_put_matrix); // 保存卷积层计算结果
    }
    return out_put_matrix;
}
float*** relu3D(float*** input_tensor, char* output_name){
    int channels = _msize(input_tensor) / sizeof(float**);
    int height = _msize(input_tensor[0]) / sizeof(float*);
    int width =  _msize(input_tensor[0][0]) / sizeof(float);

    for (int i = 0; i < channels; i++) {
        for (int j = 0; j < height; j++) {
            for(int k = 0; k < width; k++){
                if(input_tensor[i][j][k] < 0){
                    input_tensor[i][j][k] = 0;
                }
            }
        }
    }
    if(SAVE_FILE) {
        save_3Dmatrix_to_disk(output_name, input_tensor);
    }
    return input_tensor;
}
float* relu(float* input_tensor, char* output_name){
    int width =  _msize(input_tensor) / sizeof(float);

    for(int k = 0; k < width; k++){
        if(input_tensor[k] < 0){
            input_tensor[k] = 0;
        }
    }
    if(SAVE_FILE) {
        save_vector_to_disk(output_name, input_tensor);
    }
    return input_tensor;
}

float*** maxpooling(float*** input_tensor, int kernel_size, int stride, int padding, char* output_name){
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

    for(int output_channel=0; output_channel < channels; output_channel++){
        for(int output_height_idx=0; output_height_idx < output_height; output_height_idx++){
            for(int output_width_idx=0; output_width_idx < output_width; output_width_idx++){ // 遍历out_put_matrix上的每个pixel，output_width_idx最大值为223

                float max_ans = 0;  // 承接每次max pooling的计算结果

                for(int conv_height_idx=0; conv_height_idx < kernel_size; conv_height_idx += stride){
                    for(int conv_width_idx=0; conv_width_idx < kernel_size; conv_width_idx += stride){
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
    if(SAVE_FILE){
        save_3Dmatrix_to_disk(output_name, output_matrix);
    }
    return output_matrix;
}

float* linear(float* input_tensor, char* file_weight, char* file_bias, int width, int height, char* fc_name, char* output_name){
    // 创建2D matrix， 读fc层的参数到weight矩阵中
    float** weight = malloc_2D_array(width, height);
    // 读参数到parameters中（2D matrix）
    load_weight_Parameters(file_weight, weight, width, height);


    // 读bias的参数搭配bias中
    float* bias = (float*)malloc(sizeof(float)* width);
    load_bias_Parameters(file_bias, bias, width);

    // 申请 output_vector的内存
    float* output_vector = (float*)malloc(sizeof(float)* width);

    for(int i = 0; i < width; i++) {
        float fc_operate_ans = 0;
        for (int j = 0; j < height; j++) {
            fc_operate_ans += input_tensor[j] * weight[j][i];  // 让weight第i列的所有元素与input_tensor中所有元素相乘
        }
        output_vector[i] = fc_operate_ans;
    }


    // 保存fc的参数到磁盘
//    save_2Dmatrix_to_disk(fc_name, weight);
    if(SAVE_FILE) {
        save_vector_to_disk(output_name, output_vector);
    }

    free(input_tensor);
    input_tensor = NULL;
    free_2D_array(weight);
    free(bias);
    return output_vector;
}
float* dropout(float* input_tensor, float p, char* output_name){
    int width =  _msize(input_tensor) / sizeof(float);

    for(int i = 0; i < width; i++){
        input_tensor[i] *= p;
    }
    if(SAVE_FILE) {
        save_vector_to_disk(output_name, input_tensor);
    }
    return input_tensor;
}
float averagepool(float* input_tensor){
    int width =  _msize(input_tensor) / sizeof(float);
    float ans = 0;
    for(int i = 0; i < width; i++){
        ans += input_tensor[i];
    }

    free(input_tensor);
    input_tensor = NULL;
    return ans / width;
}
