//
// Created by Administrator on 2019/10/30.
//

#ifndef HELLO_JELLY_DNN_H
#define HELLO_JELLY_DNN_H




float**** malloc_multi_array(int width, int height, int channels, int filter_nums);  // 分配卷积层内存
int loadParameters(char* path, float**** parameters, int width, int height, int channels, int filter_nums); // 加载模型参数到卷积层
int free_multi_array(float**** p, int width, int height, int channels, int filter_nums); // 释放卷积层
void print_multi_array(float ****p, int width, int height, int channels, int filter_nums);// 打印卷积层


#endif //HELLO_JELLY_DNN_H
