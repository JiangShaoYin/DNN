project(DNN)
#aux_source_directory(. DIR_SRCS)
set(APP_SRC main.c)

#set(./include) # 设置头文件路径
#set(./lib)  # 设置库的路径

#include_directories(${INC_DIR}) # 设置头文件目录
#link_directories(${LINK_DIR})   # 设置库目录
#link_libraries(wfdb)            # 设置要链接的库

add_executable(${PROJECT_NAME} main.c include/dnn.h dnn.c 1.c lib/stb_image.h lib/jiang.h)

#print message
message(${PROJECT_SOURCE_DIR})