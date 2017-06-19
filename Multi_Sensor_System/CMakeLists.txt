cmake_minimum_required(VERSION 3.6)
project(SPEEDS)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c classifiers.c  collection.c
        do_not_use_file.c time_features.c training_file.c constants.c training_data_new.c
        consumer.c features_2.c turning_file.c last_functions.c strides_2.c trial_consumer.c match_strides.c)

add_executable(SPEEDS
        ${SOURCE_FILES})
