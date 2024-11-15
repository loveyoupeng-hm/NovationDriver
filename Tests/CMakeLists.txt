cmake_minimum_required(VERSION 3.28)

project(Tests)

enable_testing()

function(add_test_case NAME LIBS)
    add_executable(${NAME} ${NAME}.cpp)
    target_link_libraries(${NAME} gtest gtest_main gmock ${LIBS})
    target_compile_definitions(
        ${NAME} 
        PUBLIC 
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
    )
    add_test(${NAME} ${NAME})
endfunction()

add_test_case(test juce::juce_dsp)