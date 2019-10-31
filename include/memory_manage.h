//
// Created by Administrator on 2019/10/31.
//

#ifndef DNN_MEMORY_MANAGE_H
#define DNN_MEMORY_MANAGE_H


float**** malloc_4D_array(int width, int height, int channels, int filter_nums);
float*** malloc_3D_array(int width, int height, int channels);
int free_3D_array(float*** p);
int free_4D_array(float**** p);


#endif //DNN_MEMORY_MANAGE_H
