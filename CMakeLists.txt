cmake_minimum_required(VERSION 3.22)
project(PhaseDelay VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.0
)
FetchContent_MakeAvailable(juce)

juce_add_plugin(PhaseDelay
    COMPANY_NAME "phde"
    PLUGIN_MANUFACTURER_CODE "Phde"
    PLUGIN_CODE "Phde"
    FORMATS VST3
    PRODUCT_NAME "PhaseDelay"
)

juce_generate_juce_header(PhaseDelay)

target_sources(PhaseDelay
    PRIVATE
        Source/PhaseDelay.h
        Source/Biquad.h
        Source/SVF.h
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
)

target_compile_definitions(PhaseDelay
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(PhaseDelay
    PRIVATE
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
)