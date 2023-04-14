
# 配置头文件搜索路径
# include_directories()
# 配置库文件搜索路径
# link_directories()

# 可执行文件放在项目根目录下的bin目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置需要编译的源文件列表
set(SRC_LIST muduo_server.cpp)

# 将.指定路径下所有源文件名放入变量SRC_LIST中 
#aux_source_directory(. SRC_LIST)

# 生成可执行文件
add_executable(server ${SRC_LIST})

# 指定可执行文件需要链接的库 -lxxx,把xxx填入
target_link_libraries(server muduo_net muduo_base pthread)




