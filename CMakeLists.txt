# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "C:/Users/Haydar/Desktop/components/sht3x" #kütüphanelerin kayıtlı olduğu konum ile değiştir
"C:/Users/Haydar/Desktop/components/i2cdev"
"C:/Users/Haydar/Desktop/components/esp_idf_lib_helpers"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(example-sht3x)
