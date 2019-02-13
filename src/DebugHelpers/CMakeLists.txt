add_library(DebugHelpers
    bar.c 
    bar_impl/bar_impl.c)
target_include_directories(DebugHelpers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(DebugHelpers foo)
