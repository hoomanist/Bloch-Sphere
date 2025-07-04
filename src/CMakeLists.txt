add_executable(bloch
  main.cpp
)
include_directories(../include)
# Yep, that's it!
target_link_libraries(bloch
  PUBLIC
    ImGui-SFML::ImGui-SFML
)
