project(Launchpad VERSION ${PROJECT_VERSION})

juce_add_plugin(
    ${PROJECT_NAME}
    COMPANY_NAME loveyoupeng
    IS_SYNTH FALSE
    IS_MIDI_EFFECT TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    PLUGIN_NAME ${PROJECT_NAME}
    PRODUCT_NAME ${PROJECT_NAME}
    FORMATS VST3 Standalone
)

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC 
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
        juce::juce_audio_utils
        juce::juce_graphics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        includes
)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
        source/VstProcessor.cpp
        source/VstProcessorEditor.cpp
        source/LaunchpadComponent.cpp
        source/LaunchpadBroadcaster.cpp
        source/LaunchpadLayout.cpp
        source/LaunchpadDriver.cpp
        source/Main.cpp
)
