project(DNN)
set(APP_SRC main.c)
add_executable(${PROJECT_NAME} main.c dnn.h dnn.c 1.c)

#print message
message(${PROJECT_SOURCE_DIR})