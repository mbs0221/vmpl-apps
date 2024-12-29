cmake_minimum_required(VERSION 3.10)
project(vmpl-apps 
    LANGUAGES C
    VERSION 1.0.0
)

# 设置安装前缀（删除强制设置，允许命令行参数覆盖）
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()

# 包含 GNUInstallDirs 以获取标准安装目录定义
include(GNUInstallDirs)

# 设置通用编译选项
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -g -O2)

# 设置动态链接为默认
set(BUILD_SHARED_LIBS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置库的查找路径
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
link_directories(/usr/local/lib)

# 添加子目录
add_subdirectory(syscalls)
add_subdirectory(bench)
add_subdirectory(test)

# CPack基本配置
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME "vmpl-apps")
set(CPACK_PACKAGE_VENDOR "VMPL")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "VMPL Applications Package")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

# DEB包特定配置
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "VMPL Team <vmpl@example.com>")  # 必需字段
set(CPACK_PACKAGE_CONTACT "VMPL Team <vmpl@example.com>")           # 必需字段
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libvmpl (>= 1.0.0), libdune (>= 1.0.0)")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)  # 使用标准的Debian命名格式

# 安装路径配置（使用CMAKE_INSTALL_PREFIX的值）
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

# 安装头文件
install(DIRECTORY bench/include/ DESTINATION include)
install(DIRECTORY syscalls/include/ DESTINATION include)
install(DIRECTORY test/include/ DESTINATION include)

# 安装库文件
install(TARGETS test
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 收集并安装所有可执行文件
get_property(BENCH_TARGETS
    DIRECTORY bench
    PROPERTY BUILDSYSTEM_TARGETS)
get_property(TEST_TARGETS
    DIRECTORY test
    PROPERTY BUILDSYSTEM_TARGETS)
get_property(SYSCALLS_TARGETS
    DIRECTORY syscalls
    PROPERTY BUILDSYSTEM_TARGETS)

install(TARGETS ${BENCH_TARGETS} ${TEST_TARGETS} ${SYSCALLS_TARGETS}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

# 必须在设置完所有CPACK变量后包含CPack
include(CPack) 