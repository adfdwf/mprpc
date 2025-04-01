# 当前目录下的所有源文件放入SRC_LIST中
aux_source_directory(. SRC_LIST)
add_library(mprpc ${SRC_LIST})
# 添加 THREADED 宏定义
add_definitions(-DTHREADED)
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)

