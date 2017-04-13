cmake_minimum_required(VERSION 3.4.1)

add_library(str-crypt SHARED
            str-crypt.c)

target_link_libraries(str-crypt
                      android
                      log)
