cmake_minimum_required(VERSION 3.20)

project("ImGuiExample")

find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(ImGuiInstance)

add_executable(imgui_example src/main.cpp)

target_link_libraries(imgui_example PUBLIC imgui_instance::imgui_instance)
target_link_libraries(imgui_example PUBLIC nlohmann_json::nlohmann_json)

target_compile_features(imgui_example PUBLIC cxx_std_20)
