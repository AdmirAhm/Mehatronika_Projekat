set(MEHATRONIKA_NAME testMehatronika)				#Naziv prvog projekta u solution-u

file(GLOB MEHATRONIKA_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB MEHATRONIKA_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB MEHATRONIKA_BPA  ${CMAKE_CURRENT_LIST_DIR}/src/bpalgo/*)
set(MEHATRONIKA_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB MEHATRONIKA_INC_TD  ${MY_INC}/td/*.h)
file(GLOB MEHATRONIKA_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB MEHATRONIKA_INC_GUI_GL ${MY_INC}/gui/gl/*.h)
file(GLOB MEHATRONIKA_INC_GUI_SOCKET ${CMAKE_CURRENT_LIST_DIR}/src/Socket/*.h)

# add executable
add_executable(${MEHATRONIKA_NAME} ${MEHATRONIKA_INCS} ${MEHATRONIKA_SOURCES} ${MEHATRONIKA_INC_TD} ${MEHATRONIKA_INC_GUI} ${MEHATRONIKA_INC_GUI_GL} ${MEHATRONIKA_INC_GUI_SOCKET} ${MEHATRONIKA_BPA})

#include(${CMAKE_CURRENT_LIST_DIR}/../Ekstruzija/Extrusion/Extrusion.cmake)

source_group("inc"            FILES ${MEHATRONIKA_INCS})
source_group("src"            FILES ${MEHATRONIKA_SOURCES})
source_group("inc\\td"        FILES ${MEHATRONIKA_INC_TD})
source_group("inc\\gui"        FILES ${MEHATRONIKA_INC_GUI})
source_group("inc\\gl"        FILES ${MEHATRONIKA_INC_GUI_GL})
source_group("inc\\socket"        FILES ${MEHATRONIKA_INC_GUI_SOCKET})
source_group("inc\\bpalgo"        FILES ${MEHATRONIKA_BPA})

target_link_libraries(${MEHATRONIKA_NAME} #${EXTRUSION_SHLIB_NAME}
									debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

setApplicationIcon(${MEHATRONIKA_NAME} "${WORK_ROOT}/Common/WinIcons/apple.ico" "${CMAKE_CURRENT_LIST_DIR}/Common/Icons/apple128.png")

addOpenGLLibraries(${MEHATRONIKA_NAME})

setTargetPropertiesForGUIApp(${MEHATRONIKA_NAME} ${MEHATRONIKA_PLIST})

setIDEPropertiesForGUIExecutable(${MEHATRONIKA_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${MEHATRONIKA_NAME})



