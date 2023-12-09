cmake_minimum_required(VERSION 3.10)
project(test)
set(MAHLER_PATH "..")

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${MAHLER_PATH}/inc"
    "${MAHLER_PATH}/src"
)

add_subdirectory(${MAHLER_PATH} build)
target_link_libraries(${PROJECT_NAME} PUBLIC mahler)