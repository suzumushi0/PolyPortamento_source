cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "D:/VST/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(PolyPortamento
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "PolyPortamento VST 3 Plug-in"
)

# suzumushi
unset(SMTG_CREATE_PLUGIN_LINK)
set (SMTG_CXX_STANDARD "20")

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(PolyPortamento
    source/version.h
    source/PPcids.h
    source/PPparam.h
    source/PPpitch.h
    source/PPprocessor.h
    source/PPprocessor.cpp
    source/PPcontroller.h
    source/PPcontroller.cpp
    source/PPentry.cpp
    source/SOextparam.h
    source/SOextparam.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ADD_VSTGUI)
    target_sources(PolyPortamento
        PRIVATE
            resource/PolyPortamento.uidesc
    )
    target_link_libraries(PolyPortamento
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(PolyPortamento
        RESOURCES
            "resource/PolyPortamento.uidesc"
    )
endif(SMTG_ADD_VSTGUI)
# -------------------

smtg_target_add_plugin_snapshots (PolyPortamento
    RESOURCES
        resource/4CCF5BB359C7586B9C0046EFA69EED29_snapshot.png
        resource/4CCF5BB359C7586B9C0046EFA69EED29_snapshot_2.0x.png
)

target_link_libraries(PolyPortamento
    PRIVATE
        sdk
)

smtg_target_configure_version_file(PolyPortamento)

if(SMTG_MAC)
    smtg_target_set_bundle(PolyPortamento
        BUNDLE_IDENTIFIER foo
        COMPANY_NAME "suzumushi"
    )
    smtg_target_set_debug_executable(PolyPortamento
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(PolyPortamento PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PolyPortamento)

        smtg_target_set_debug_executable(PolyPortamento
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
