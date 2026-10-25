cmake_minimum_required(VERSION 3.22) # for 6.1 espidf

set(EXTRA_COMPONENT_DIRS "src")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(app)
