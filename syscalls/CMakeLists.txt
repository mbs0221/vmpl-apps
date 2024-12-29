# 创建共享库 libtest
add_library(test SHARED src/common.c)
target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(test dl)

# 设置共享库的版本信息
set_target_properties(test PROPERTIES
    VERSION 1.0.0
    SOVERSION 1)

# 定义所有测试程序
set(TEST_PROGRAMS
    test_syscall
    test_hotcalls
    test_faults
    test_mmap
    test_mmap_pages
    test_mmap_file
    test_mmap_read
    test_heap
    test_mem
    test_brk
    test_maps
    test_pagemap
    test_procmaps
    test_mprotect
    test_cpuid
    test_msr
    test_atexit
    test_ugi
    test_file
    test_proc
    test_hello
    test_printf
    test_time
    test_timer
    test_pipe
    test_popen
    test_recvfrom
    test_execve
    test_signal
    test_signal1
    test_sem
    test_shm
    test_msg
    test_socket
    test_epoll
    test_aio
    test_thread
    test_clone
    test_fork
    test_futex
)

# 创建所有测试程序
foreach(program ${TEST_PROGRAMS})
    add_executable(${program} src/${program}.c)
    target_link_libraries(${program} test)
endforeach()

# 为特定程序添加额外的链接库
set(VMPL_PROGRAMS
    test_brk
    test_fork
    test_syscall
    test_hotcalls
    test_proc
    test_pipe
    test_signal
    test_signal1
    test_socket
    test_shm
    test_msg
    test_clone
)

foreach(program ${VMPL_PROGRAMS})
    target_link_libraries(${program} vmpl dune dict hotcalls)
endforeach() 