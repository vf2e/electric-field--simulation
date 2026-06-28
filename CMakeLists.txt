cmake_minimum_required(VERSION 3.16)
project(efield-viewer VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

set(MAGIMAIM_QT_ROOT "D:/2025WorkSpace/magaim-qt" CACHE PATH
    "Path to magaim-qt project (provides VTK64 and openctm)")

set(VTK_ROOT "${MAGIMAIM_QT_ROOT}/3dLib/VTK64" CACHE PATH "VTK 8.0 root")
set(VTK_VERSION "8.0")
set(OPENCTM_DIR "${MAGIMAIM_QT_ROOT}/3dLib/openctm" CACHE PATH "OpenCTM library directory")

set(VTK_INCLUDE_DIR "${VTK_ROOT}/include/vtk-${VTK_VERSION}")
set(VTK_LIB_DIR "${VTK_ROOT}/lib")
set(VTK_BIN_DIR "${VTK_ROOT}/bin")

if(NOT EXISTS "${VTK_INCLUDE_DIR}/vtkPolyData.h")
    message(FATAL_ERROR "VTK headers not found at ${VTK_INCLUDE_DIR}")
endif()

if(NOT EXISTS "${OPENCTM_DIR}/openctm.lib")
    message(FATAL_ERROR "openctm.lib not found at ${OPENCTM_DIR}")
endif()

find_package(Qt5 5.12 REQUIRED COMPONENTS Quick Widgets QuickWidgets)

set(SOURCES
    src/main.cpp
    src/ApplicationShell.cpp
    src/AppController.cpp
    src/VtkRenderWidget.cpp
    src/ModelLoader.cpp
    src/HeadMaterial.cpp
    src/FieldColorMap.cpp
    src/FieldSimulator.cpp
)

set(HEADERS
    src/ApplicationShell.h
    src/AppController.h
    src/VtkRenderWidget.h
    src/ModelLoader.h
    src/HeadMaterial.h
    src/FieldColorMap.h
    src/FieldSimulator.h
    src/AppTheme.h
)

set(RESOURCES
    resources/app.qrc
)

if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
endif()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES} ${APP_ICON_RESOURCE_WINDOWS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${OPENCTM_DIR}
    ${VTK_INCLUDE_DIR}
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${VTK_LIB_DIR}
    ${OPENCTM_DIR}
)

set(EFIELD_VIEWER_VTK_LIBS
    vtkRenderingOpenGL-${VTK_VERSION}
    vtkRenderingCore-${VTK_VERSION}
    vtkRenderingFreeType-${VTK_VERSION}
    vtkRenderingContextOpenGL-${VTK_VERSION}
    vtkRenderingContext2D-${VTK_VERSION}
    vtkRenderingAnnotation-${VTK_VERSION}
    vtkInteractionStyle-${VTK_VERSION}
    vtkIOGeometry-${VTK_VERSION}
    vtkIOCore-${VTK_VERSION}
    vtkIOLegacy-${VTK_VERSION}
    vtkFiltersCore-${VTK_VERSION}
    vtkFiltersGeneral-${VTK_VERSION}
    vtkFiltersGeometry-${VTK_VERSION}
    vtkFiltersSources-${VTK_VERSION}
    vtkImagingCore-${VTK_VERSION}
    vtkCommonExecutionModel-${VTK_VERSION}
    vtkCommonDataModel-${VTK_VERSION}
    vtkCommonCore-${VTK_VERSION}
    vtkCommonColor-${VTK_VERSION}
    vtkCommonMath-${VTK_VERSION}
    vtkCommonTransforms-${VTK_VERSION}
    vtkCommonSystem-${VTK_VERSION}
    vtkCommonMisc-${VTK_VERSION}
    vtksys-${VTK_VERSION}
    vtkzlib-${VTK_VERSION}
    vtkfreetype-${VTK_VERSION}
    vtkpng-${VTK_VERSION}
    vtkjpeg-${VTK_VERSION}
    vtktiff-${VTK_VERSION}
    vtkexpat-${VTK_VERSION}
    vtkjsoncpp-${VTK_VERSION}
    vtkmetaio-${VTK_VERSION}
    vtkverdict-${VTK_VERSION}
    vtklibxml2-${VTK_VERSION}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::Widgets
    openctm
    ${EFIELD_VIEWER_VTK_LIBS}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copy assets directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENCTM_DIR}/openctm.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "Copy openctm.dll"
)

file(GLOB VTK_DLLS "${VTK_BIN_DIR}/*.dll")
foreach(dll ${VTK_DLLS})
    get_filename_component(dll_name ${dll} NAME)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dll}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copy ${dll_name}"
    )
endforeach()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

option(BUILD_TESTING "Build and register unit tests" ON)
if(BUILD_TESTING)
    set(CTM_VIEWER_VTK_LIBS ${EFIELD_VIEWER_VTK_LIBS})
    enable_testing()
    add_subdirectory(tests)
endif()
