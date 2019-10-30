//
// Created by Administrator on 2019/10/30.
//

#ifndef HELLO_JELLY_DNN_H
#define HELLO_JELLY_DNN_H


typedef struct Channel{
    float a;
}Channel;



typedef struct Conv{
    int a;
};


int Conv(float input);
int loadParameters(char* path, float* parameters, int height, int width, int channles, int filter_nums);
float**** malloc_multi_array(int width, int height, int channels, int filter_nums)

#endif //HELLO_JELLY_DNN_H
