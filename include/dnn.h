//
// Created by Administrator on 2019/10/30.
//

#ifndef HELLO_JELLY_DNN_H
#define HELLO_JELLY_DNN_H


float*** feature_padding(float*** input_pic, int padding);
float*** convert_vector_2_3Dmatrix(unsigned char* input_pic, int width, int height, int channels);

float*** conv3D(float*** input_tensor, char* parameter_file, int conv_width, int conv_height,
               int conv_channels,  int kernel_nums, int stride, int padding, char* input_name, char* conv_name, char* output_name);
float* relu(float* input_tensor, char* output_name);
float*** maxpooling(float*** input_tensor, int kernel_size, int stride, int padding, char* output_name);

float* linear(float*** input_tensor, char* parameter_wight, char* parameter_bias, int width, int height, char* fc_name, char* output_name);




#endif //HELLO_JELLY_DNN_H

