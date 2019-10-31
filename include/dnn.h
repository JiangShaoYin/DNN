//
// Created by Administrator on 2019/10/30.
//

#ifndef HELLO_JELLY_DNN_H
#define HELLO_JELLY_DNN_H




float**** malloc_4D_array(int width, int height, int channels, int filter_nums);  // 分配4D卷积层内存
float*** malloc_3D_array(int width, int height, int channels);  // 分配3D feature map 内存
int loadParameters(char* path, float**** parameters, int width, int height, int channels, int filter_nums); // 加载模型参数到卷积层
int free_3D_array(float*** p); // 释放卷积层
void print_3D_array(float ***p);// 打印卷积层
int free_4D_array(float**** p); // 释放卷积层
void print_4D_array(float ****p);// 打印卷积层


#endif //HELLO_JELLY_DNN_H

