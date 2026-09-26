add_executable(test-CrashHandler CrashHandlerTest.cpp)
target_link_libraries(
    test-CrashHandler PRIVATE SapphireCommon
)

add_executable(test-SymbolizerTest SymbolizerTest.cpp)
target_link_libraries(
    test-SymbolizerTest PRIVATE SapphireCommon
)

add_executable(test-StackTraceTest StackTraceTest.cpp)
target_link_libraries(
    test-StackTraceTest PRIVATE SapphireCommon
)

add_executable(test-StackProbeAndCreateThreadTest StackProbeAndCreateThreadTest.cpp)
target_link_libraries(
    test-StackProbeAndCreateThreadTest PRIVATE SapphireCommon
)

add_executable(test-CrashHandlerEdgeCaseTest CrashHandlerEdgeCaseTest.cpp)
target_link_libraries(
    test-CrashHandlerEdgeCaseTest PRIVATE SapphireCommon
)

sapphire_enable_release_pdb(
    test-CrashHandler
    test-SymbolizerTest
    test-StackTraceTest
    test-StackProbeAndCreateThreadTest
    test-CrashHandlerEdgeCaseTest
)
