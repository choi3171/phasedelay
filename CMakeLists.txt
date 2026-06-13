cmake_minimum_required(VERSION 3.22)

project(PhaseDelay VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.0
)

FetchContent_MakeAvailable(juce)

set(PHASEDELAY_DSP_SOURCES
    Source/PhaseDelay.cpp
    Source/PhaseDelay.h
    Source/Biquad.h
    Source/SVF.h
)

set(PHASEDELAY_PLUGIN_SOURCES
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    ${PHASEDELAY_DSP_SOURCES}
)

set(PHASEDELAY_COMMON_DEFINITIONS
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
            ${PHASEDELAY_PLUGIN_SOURCES}
    )

    target_include_directories(${target_name}
        PRIVATE
            Source
    )

    target_compile_definitions(${target_name}
        PRIVATE
            ${PHASEDELAY_COMMON_DEFINITIONS}
            GUI_INCLUDE=${gui_include}
    )

    target_link_libraries(${target_name}
        PRIVATE
            juce::juce_audio_plugin_client
            juce::juce_audio_processors
            juce::juce_audio_basics
            juce::juce_dsp
            juce::juce_gui_basics
            juce::juce_gui_extra
    )
endfunction()

if(PHASEDELAY_BUILD_PLUGIN)
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
endif()

if(BENCH)
    juce_add_console_app(PhaseDelayBench
        PRODUCT_NAME "PhaseDelayBench"
    )

    juce_generate_juce_header(PhaseDelayBench)

    target_sources(PhaseDelayBench
        PRIVATE
            Bench/Bench.cpp
            ${PHASEDELAY_DSP_SOURCES}
    )

    target_include_directories(PhaseDelayBench
        PRIVATE
            Source
    )

    target_compile_definitions(PhaseDelayBench
        PRIVATE
            ${PHASEDELAY_COMMON_DEFINITIONS}
            PHASEDELAY_BENCH=1
        )

    target_link_libraries(PhaseDelayBench
        PRIVATE
            juce::juce_core
            juce::juce_audio_basics
            juce::juce_dsp
    )
endif()
