cmake_minimum_required(VERSION 3.10)
project(test C CXX)

add_library(good OBJECT good.c)
set_target_properties(good PROPERTIES C_STANDARD 11)
