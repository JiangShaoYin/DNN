//
// Created by Administrator on 2019/10/30.
//
# include "dnn.h"
# include <stdio.h>



float**** malloc_multi_array(int width, int height, int channels, int filter_nums) {
    float ****para_array = (float****)malloc(sizeof(float***) * filter_nums);  //申请最外围的空间

    for (int i = 0;i < filter_nums; i++) {
        para_array[i] = (float***)malloc(sizeof(float**) * channels);
        for (int j = 0; j < channels; j++) {
            para_array[i][j] = (float **) malloc(sizeof(float *) * height);
            for(int k = 0; k < height; k++){
                para_array[i][j][k] = (float *) malloc(sizeof(float ) * width);
            }
        }
    }
    return para_array;
}


int main(){
//    float**** para_array = malloc_multi_array(2,3,4,5); //5*4*3*2
//    float cnt = 1;
//
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 4; j++) {
//            for (int k = 0; k < 3; k++) {
//                for(int l = 0; l < 2; l++){
//                    para_array[i][j][k][l] = cnt++;
//                }
//            }
//        }
//    }
//
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 4; j++) {
//            for (int k = 0; k < 3; k++) {
//                for (int l = 0; l < 2; l++) {
//                    printf("%f\n", para_array[i][j][k][l]);
//                }
//            }
//        }
//    }
//

    float**** p = malloc_multi_array(2,3,4,5);

    float cnt = 1;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                for(int l = 0; l < 2; l++){
                    p[i][j][k][l] = cnt++;
                }
            }
        }
    }


    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 3; k++) {
                for(int l = 0; l < 2; l++){
                    printf("%d\n", p[i][j][k][l]);
                }
            }
        }
    }


}
