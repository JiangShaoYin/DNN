//
// Created by Administrator on 2019/10/28.
# include "include/dnn.h"
# include "include/I_O.h"
# include "include/memory_manage.h"
# include <stdio.h>
# include "./lib/stb_image.h"

# include <windows.h>
# include <windef.h>




char* filename = "../face.jpg";

bool SAVE_FILE = TRUE;

int main(){

//    int channels, height, width;
//
//    unsigned char* input_pic = stbi_load(filename, &width, &height, &channels, 0); // 按照rgb顺序打开, 申请空间
//    float*** input_matrix = convert_vector_2_3Dmatrix(input_pic, width, height, channels); // stblib里面打开jpg用的一维数组，这里将其转换为3D，并将pixel从unsigned char 变为float
//
//    // layer1
//    float*** conv1_before_relu3D = conv3D(input_matrix, "../parameters/_features.0.weight", 3, 3,
//            3,  64, 1, 1,
//            "../conv1_input.txt", "../conv1.txt", "../conv1_before_relu3D.txt");
//    float*** conv1_relu3D = relu3D(conv1_before_relu3D, "../conv1_relu3D.txt");
//    float*** conv1_max_pooing = maxpooling(conv1_relu3D,2,2,0,"../conv1_max_pooing.txt");
//
//    // layer2
//    float*** conv2_before_relu3D = conv3D(conv1_max_pooing,"../parameters/_features.3.weight", 3, 3,
//                                        64,  128, 1, 1,
//                                        "../conv2_input.txt", "../conv2.txt", "../conv2_before_relu3D.txt");
//    float*** conv2_relu3D = relu3D(conv2_before_relu3D, "../conv2_relu3D.txt");
//    float*** conv2_max_pooing = maxpooling(conv2_relu3D,2,2,0,"../conv2_max_pooing.txt");
//
//    // layer3
//    float*** conv3_before_relu3D = conv3D(conv2_max_pooing,"../parameters/_features.6.weight", 3, 3,
//                                        128, 256, 1, 1,
//                                        "../conv3_input.txt", "../conv3.txt", "../conv3_before_relu3D.txt");
//    float*** conv3_relu3D = relu3D(conv3_before_relu3D, "../conv3_relu3D.txt");
//
//    // layer4
//    float*** conv4_before_relu3D = conv3D(conv3_relu3D,"../parameters/_features.8.weight", 3, 3,
//                                        256,  256, 1, 1,
//                                        "../conv4_input.txt", "../conv4.txt", "../conv4_before_relu3D.txt");
//    float*** conv4_relu3D = relu3D(conv4_before_relu3D, "../conv4_relu3D.txt");
//    float*** conv4_max_pooing = maxpooling(conv4_relu3D,2,2,0,"../conv4_max_pooing.txt");
//
//    // layer5
//    float*** conv5_before_relu3D = conv3D(conv4_max_pooing,"../parameters/_features.11.weight", 3, 3,
//                                        256,  512, 1, 1,
//                                        "../conv5_input.txt", "../conv5.txt", "../conv5_before_relu3D.txt");
//    float*** conv5_relu3D = relu3D(conv5_before_relu3D, "../conv5_relu3D.txt");
//
//    // layer6
//    float*** conv6_before_relu3D = conv3D(conv5_relu3D,"../parameters/_features.13.weight", 3, 3,
//                                        512,  512, 1, 1,
//                                        "../conv6_input.txt", "../conv6.txt", "../conv6_before_relu3D.txt");
//    float*** conv6_relu3D = relu3D(conv6_before_relu3D, "../conv6_relu3D.txt");
//    float*** conv6_max_pooing = maxpooling(conv6_relu3D,2,2,0,"../conv4_max_pooing.txt");
//
//    // layer7
//    float*** conv7_before_relu3D = conv3D(conv6_max_pooing,"../parameters/_features.16.weight", 3, 3,
//                                        512,  512, 1, 1,
//                                        "../conv7_input.txt", "../conv7.txt", "../conv7_before_relu3D.txt");
//    float*** conv7_relu3D = relu3D(conv7_before_relu3D, "../conv7_relu3D.txt");
//
//    // layer8
//    float*** conv8_before_relu3D = conv3D(conv7_relu3D,"../parameters/_features.18.weight", 3, 3,
//                                        512,  512, 1, 1,
//                                        "../conv8_input.txt", "../conv8.txt", "../conv8_before_relu3D.txt");
//    float*** conv8_relu3D = relu3D(conv8_before_relu3D, "../conv8_relu3D.txt");
//    float*** conv8_max_pooing = maxpooling(conv8_relu3D,2,2,0,"../conv8_max_pooing.txt");
//
//
//
//    // fc0
//    float* fc0_input = convert_3Dmatrix_2_vector(conv8_max_pooing);
//    printf("fc0_input\n");
//    float* fc0 = linear(fc0_input, "../parameters/linear0.weight", "../parameters/linear0.bias", 4096, 25088, "../fc0.txt", "../fc0_before_relu.txt");
//    float* fc0_relu = relu(fc0, "../fc0_relu.txt");
//    float* fc0_dropout = dropout(fc0_relu, 0.5, "../fc0_dropout.txt");

//

    float* fc0_dropout = (float*)malloc(sizeof(float*) * 4096);
    load_fc_result_2_memory("../fc0_dropout.txt", fc0_dropout, 4096);
    // fc1
    float* fc1 = linear(fc0_dropout, "../parameters/linear1.weight", "../parameters/linear1.bias", 4096, 4096, "../fc1.txt", "../fc1_before_relu.txt");
    float* fc1_relu = relu(fc1, "../fc1_relu.txt");
    float* fc1_dropout = dropout(fc1_relu, 0.5, "../fc1_dropout.txt");

    //
    float* fc2 = linear(fc1_dropout, "../parameters/last_linear.weight", "../parameters/last_linear.bias", 101, 4096, "../fc2.txt", "../fc2_before_relu.txt");
    printf("fc2_before_relu\n");




//    float* fc2 = (float*)malloc(sizeof(float*) * 101);
//    load_fc_result_2_memory("../fc2_before_relu.txt", fc2, 101);

    softmax(fc2);
    float ans = cal(fc2);
    printf("%.2f\n", ans);

    return 0;
}

