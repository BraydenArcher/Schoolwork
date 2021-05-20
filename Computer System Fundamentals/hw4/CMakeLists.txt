cmake_minimum_required(VERSION 3.17)
project(hw4)

set(CMAKE_CXX_STANDARD  14)

add_executable(hw4 imgproc.c expose.c image.c image.h image_plugin.h mirrorh.c mirrorv.c pnglite.c pnglite.h swapbg.c tile.c)
