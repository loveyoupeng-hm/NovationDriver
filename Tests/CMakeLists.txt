cmake_minimum_required(VERSION 3.28)

project(Tests)

enable_testing()

function(add_test_case NAME LIBS)
    add_executable(${NAME} ${NAME})
    target_link_libraries(${NAME} gtest gtest_main gmock ${LIBS})
    add_test(${NAME} ${NAME})
endfunction()

add_test_case(test juce::juce_dsp)