project(Tests)

function(add_test_case NAME LIBS ...)
    add_executable(${NAME} ${NAME}.cpp)
    set(arg_count ${ARGC})

  # Iterate over arguments, starting from the second one
    foreach(arg RANGE 1 ${arg_count})
        target_link_libraries(${NAME} ${ARGV${arg}})
    endforeach()
    target_link_libraries(${NAME} gtest gtest_main gmock ${LIBS} ${CMAKE_THREAD_LIBS_INIT})
    target_compile_definitions(
        ${NAME} 
        PUBLIC 
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
    )
    add_test(${NAME} ${NAME})
endfunction()

add_test_case(SimpleTests juce::juce_dsp atomic)