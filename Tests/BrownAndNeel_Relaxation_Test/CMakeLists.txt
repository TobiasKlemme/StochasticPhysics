
set(TARGET_NAME BrownAndNeel_Relaxation_Test)
set(CPP_FILES "Test_BrownAndNeel_Relaxation.cpp")
set(INL_FILES "")
set(H_FILES "Test_BrownAndNeel_Relaxation.h;")

add_executable(${TARGET_NAME} ${CPP_FILES} ${H_FILES} ${INL_FILES} $<TARGET_OBJECTS:SimulationCommon>)

set_target_properties (${TARGET_NAME} PROPERTIES FOLDER tests)	

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME}_CommonTarget)

add_test(${TARGET_NAME} ${TARGET_NAME})
