project(buffer)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(LIBBUFFER_LIB_DIR "bin")
else()
  set(LIBBUFFER_LIB_DIR "lib")
endif()


# Create a shared library
file(GLOB SOURCES src/*.cpp src/*.c)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Ensure clients can find the includes
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
set(LIB_DEPENDS common error)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_DEPENDS})

# What to install
install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIBBUFFER_LIB_DIR}
  ARCHIVE DESTINATION ${LIBBUFFER_LIB_DIR}
)
install(
  DIRECTORY include/makestuff DESTINATION include
)

# Maybe build tests
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
