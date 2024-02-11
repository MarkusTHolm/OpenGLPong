# Make library
add_library(Keyboard keyboard.cpp)

# State that anybody linking to this library needs to include the
# current source dir to find keyboard.h, while we dont
target_include_directories(Keyboard
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )                         