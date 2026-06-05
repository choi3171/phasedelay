cmake_minimum_required(VERSION 3.22)
project(FAPP VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.0
)
FetchContent_MakeAvailable(juce)

juce_add_plugin(FAPP
    COMPANY_NAME "fapp"
    PLUGIN_MANUFACTURER_CODE "Fapp"
    PLUGIN_CODE "Fapp"
    FORMATS VST3
    PRODUCT_NAME "FAPP"
)

juce_generate_juce_header(FAPP)

target_sources(FAPP
    PRIVATE
        Source/FAPP.h
        Source/Biquad.h
        Source/SVF.h
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
)

target_compile_definitions(FAPP
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(FAPP
    PRIVATE
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_gui_basics
        juce::juce_gui_extra
)