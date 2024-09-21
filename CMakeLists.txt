cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "E:/Programming/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(Gain_Knob
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "Gain_Knob VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(Gain_Knob
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(Gain_Knob
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(Gain_Knob
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(Gain_Knob
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (Gain_Knob
    RESOURCES
        resource/1148CF49EC2F59D597294578045D3467_snapshot.png
        resource/1148CF49EC2F59D597294578045D3467_snapshot_2.0x.png
)

target_link_libraries(Gain_Knob
    PRIVATE
        sdk
)

smtg_target_configure_version_file(Gain_Knob)

if(SMTG_MAC)
    smtg_target_set_bundle(Gain_Knob
        BUNDLE_IDENTIFIER e
        COMPANY_NAME "ArtificialVoid1"
    )
    smtg_target_set_debug_executable(Gain_Knob
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(Gain_Knob PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Gain_Knob)

        smtg_target_set_debug_executable(Gain_Knob
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
