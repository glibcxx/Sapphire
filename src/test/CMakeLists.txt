add_executable(test-CoroutineTest CoroutineTest.cpp)
add_executable(test-CoroutineBenchmark CoroutineBenchmark.cpp)

target_link_libraries(
    test-CoroutineTest PRIVATE SapphireCommon
)
target_link_libraries(
    test-CoroutineBenchmark PRIVATE SapphireCommon
)