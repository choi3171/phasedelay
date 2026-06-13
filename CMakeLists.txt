cmake_minimum_required(VERSION 3.22)

project(PhaseDelay VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.0
)

FetchContent_MakeAvailable(juce)

set(PHASEDELAY_SOURCES
    Source/PhaseDelay.h
    Source/Biquad.h
    Source/SVF.h
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
)

set(PHASEDELAY_COMPILE_DEFINITIONS
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

function(add_phasedelay_plugin target_name product_name plugin_code gui_include)
    juce_add_plugin(${target_name}
        COMPANY_NAME "phde"
        PLUGIN_MANUFACTURER_CODE "Phde"
        PLUGIN_CODE "${plugin_code}"
        FORMATS VST3
        PRODUCT_NAME "${product_name}"
    )

    juce_generate_juce_header(${target_name})

    target_sources(${target_name}
        PRIVATE
            ${PHASEDELAY_SOURCES}
    )

    target_compile_definitions(${target_name}
        PRIVATE
            ${PHASEDELAY_COMPILE_DEFINITIONS}
            GUI_INCLUDE=${gui_include}
    )

    target_link_libraries(${target_name}
        PRIVATE
            juce::juce_audio_plugin_client
            juce::juce_audio_processors
            juce::juce_gui_basics
            juce::juce_gui_extra
    )
endfunction()

add_phasedelay_plugin(
    PhaseDelay
    "PhaseDelay"
    "PdGu"
    1
)

add_phasedelay_plugin(
    PhaseDelay_NoGUI
    "PhaseDelay NoGUI"
    "PdNg"
    0
)
