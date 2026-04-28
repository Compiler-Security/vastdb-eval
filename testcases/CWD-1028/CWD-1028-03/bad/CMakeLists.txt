cmake_minimum_required(VERSION 3.10)
project(test C CXX)

add_library(bad OBJECT bad.c)
set_target_properties(bad PROPERTIES C_STANDARD 11)
