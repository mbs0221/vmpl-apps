# 定义基准测试程序
set(BENCH_PROGRAMS
    bench_dune
    bench_dune_ring
    bench_dune_ipi
    bench_fork_ipi
    bench_self_ipi
    bench_linux
    posted_ipi
)

# 创建所有基准测试程序
foreach(program ${BENCH_PROGRAMS})
    add_executable(${program} src/${program}.c)
    target_include_directories(${program} PRIVATE include)
    target_link_libraries(${program} PRIVATE dune vmpl)
endforeach()

# bench_linux 动态链接
target_link_libraries(bench_linux PRIVATE pthread)