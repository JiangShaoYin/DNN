//
// Created by Administrator on 2019/10/31.
//

#ifndef DNN_I_O_H
#define DNN_I_O_H


int load_conv_Parameters(char* path, float**** parameters, int width, int height, int channels, int filter_nums); // 加载模型参数到卷积层
int load_weight_Parameters(char* path, float** parameters, int width, int height);
int load_bias_Parameters(char* path, float* bias, int width);

int save_vector_to_disk(char* filepath, float* p);
int save_2Dmatrix_to_disk(char* filepath, float** p);
int save_3Dmatrix_to_disk(char* filepath, float*** p);
int save_4Dmatrix_to_disk(char* filepath, float**** p);

void print_3D_array(float*** p);
void print_4D_array(float**** p);




#endif //DNN_I_O_H
