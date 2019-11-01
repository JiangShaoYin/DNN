//
// Created by Administrator on 2019/10/28.
# include "include/dnn.h"
# include <windef.h>
# include <stdio.h>

char* FILEPATH = "../face.jpg"; // 尚未完成resize功能，目前只能接受224*224*3的图片输入

bool SAVE_FILE = FALSE;  // 设置全局变量，控制是否保存网络参数，和中间层的feature map

int main(){
    int age = forward(FILEPATH);
    printf("\n\nthe age is : %d\n", age);
}

