//
// Created by Administrator on 2019/10/30.
//

#ifndef HELLO_JELLY_DNN_H
#define HELLO_JELLY_DNN_H


float*** feature_padding(float*** input_pic, int padding);
float*** convert_vector_2_3Dmatrix(unsigned char* input_pic, int width, int height, int channels);

float*** conv1(float*** input_pic, char* parameter_file, int conv_width, int conv_height, int conv_channels,  int kernel_nums, int stride, int padding);





#endif //HELLO_JELLY_DNN_H

