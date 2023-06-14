cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)
set(CMAKE_OSX_SYSROOT /)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

set(CMAKE_OSX_ARCHITECTURES=x86_64;i386;arm64)

# set(CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos;-iphonesimulator")
# set_property(GLOBAL PROPERTY XCODE_EMIT_EFFECTIVE_PLATFORM_NAME ON)

project(CloudSeedJUCE)

set(CS_SRC

src/PluginEditor.cpp
src/PluginEditor.h
src/PluginProcessor.cpp
src/PluginProcessor.h

src/Widgets.cpp
src/Widgets.hpp

src/Layout.cpp
src/Layout.hpp

src/json.hpp

# later:
# src/CustomStandaloneApp.cpp
# src/CustomStandaloneApp.hpp

src/InfoString.hpp
)

set(CS_HEADER_DIRS
   
    src/
    ../CloudSeedNative/src/
)

###

add_subdirectory(../../third_party/JUCE "JUCE")

set(PLUGIN_FORMATS VST3 AU)

#if (WITH_VST2)
#juce_set_vst2_sdk_path(../../../vst2/)
#set(PLUGIN_FORMATS ${PLUGIN_FORMATS} VST)
#endif()

juce_add_plugin(CloudSeedJUCE
    VERSION 0.0.3                               # Set this if the plugin version is different to the project version
    ICON_BIG ${CMAKE_CURRENT_SOURCE_DIR}/img/icon.png                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    ICON_SMALL ${CMAKE_CURRENT_SOURCE_DIR}/img/icon.png
    COMPANY_NAME "CloudSeed [JUCE]"                          # Specify the name of the plugin's author
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE    # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Clsj               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Clsj                            # A unique four-character plugin id with at least one upper-case character
    FORMATS Standalone ${PLUGIN_FORMATS}                # The formats to build. Other valid formats are: AAX Unity VST AU AUv3 // AU VST3
    PRODUCT_NAME "CloudSeed [JUCE]")        # The name of the final executable, which can differ from the target name

juce_generate_juce_header(CloudSeedJUCE)

set (Targets_Compile_Definitions
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
    # later
    # JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1
)

target_sources(CloudSeedJUCE PRIVATE ${CS_SRC})
target_compile_definitions(CloudSeedJUCE PUBLIC ${Targets_Compile_Definitions})

target_link_libraries(CloudSeedJUCE PRIVATE
    juce::juce_audio_utils
    juce::juce_osc
    juce::juce_dsp
	
    CloudSeedNative
    CloudSeedData
)

juce_add_binary_data(CloudSeedData 
  SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Chorus Delay.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Dull Echoes.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Hyperplane.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Medium Space.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Noise in the Hallway.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Rubi-Ka Fields.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Small Room.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/The 90s Are Back.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/../Factory Programs/Through the Looking Glass.json"

  "${CMAKE_CURRENT_SOURCE_DIR}/font/OpenSans-Regular.ttf"
)

add_compile_options(-fstack-protector-strong)
target_include_directories(CloudSeedJUCE PRIVATE ${CS_HEADER_DIRS})
target_include_directories(CloudSeedJUCE PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/juce_binarydata_CloudSeedData/JuceLibraryCode)

#debug:
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

