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

float*** conv1(float*** input_pic, char* parameter_file, int conv_width, int conv_height, int conv_channels,  int kernel_nums, int stride, int padding){
    // input_pic的shape
    int channels = _msize(input_pic) / sizeof(float**);
    int height = _msize(input_pic[0]) / sizeof(float*);
    int width =  _msize(input_pic[0][0]) / sizeof(float);

    // 执行padding
    float*** matrix_after_padding = feature_padding(input_pic, padding);

    save_3Dmatrix_to_disk("../intermediate_value_dat/input_data_after_padding.txt", matrix_after_padding);

    // 计算卷积后的尺寸
    int output_width = (int)((width + 2 * padding - conv_width) / stride); // 向下取整
    int output_height = (int)((height + 2 * padding - conv_height) / stride);
    int output_channels = kernel_nums;

    // 申请存储卷积输出结果，所需要的内存
    float*** out_put_matrix = malloc_3D_array(output_width, output_height, output_channels);

    // 创建4D matrix， 读conv1层的参数到parameters矩阵中
    float**** parameters = malloc_4D_array(conv_width, conv_height, conv_channels, kernel_nums);
    // 读参数到parameters中（4D matrix）
    loadParameters(parameter_file, parameters, conv_width, conv_height, conv_channels, kernel_nums);

    save_4Dmatrix_to_disk("../intermediate_value_dat/conv1.txt", parameters);

    for(int kernel_idx=0; kernel_idx < kernel_nums; kernel_idx++){
        for(int output_height_idx=0; output_height_idx < output_height; output_height_idx++){
            for(int output_width_idx=0; output_width_idx < output_width; output_width_idx++){ // 遍历out_put_matrix上的每个pixel，output_width_idx最大值为223

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
    save_3Dmatrix_to_disk("../intermediate_value_dat/pic_after_conv1.txt",out_put_matrix);
    return out_put_matrix;

}

