add_subdirectory(render)
add_subdirectory(app)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    app
    Qt6::Widgets)

if(WIN32)
    get_target_property(_qt_lib_dir Qt6::Widgets LOCATION)
    # Qt6::Widgets/LOCATION указывает на Qt DLL, из него можно вычислить bin
    get_filename_component(QtBinDir "${_qt_lib_dir}/../../bin" ABSOLUTE)
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QtBinDir}/windeployqt.exe" --dir $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Deploying Qt DLLs with windeployqt..."
        VERBATIM
    )
endif()
