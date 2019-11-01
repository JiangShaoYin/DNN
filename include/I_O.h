//
// Created by Administrator on 2019/10/31.
//

#ifndef DNN_I_O_H
#define DNN_I_O_H

// 控制内存的申请设释放

int load_conv_Parameters(char* path, float**** parameters, int width, int height, int channels, int filter_nums); // 加载模型参数到卷积层
int load_weight_Parameters(char* path, float** parameters, int width, int height);
int load_bias_Parameters(char* path, float* bias, int width);
int load_fc_result_2_memory(char* path, float* output, int variables_nums); // 将指定全连接层的输出，读入内存

int save_vector_to_disk(char* filepath, float* p);
int save_2Dmatrix_to_disk(char* filepath, float** p);
int save_3Dmatrix_to_disk(char* filepath, float*** p);
int save_4Dmatrix_to_disk(char* filepath, float**** p);

float* load_calculate_result_2_memeory(char* path, int variables_nums); // 将中间过程产生的计算结果，读入内存

void print_3D_array(float*** p);
void print_4D_array(float**** p);




#endif //DNN_I_O_H
