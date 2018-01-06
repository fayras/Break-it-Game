cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "Break-it")
project(${PROJECT_NAME})
set(VERSION_MAJOR "0")
set(VERSION_MINOR "4")
set(VERSION_PATCH "2")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
SET(CMAKE_INSTALL_RPATH "$ORIGIN/lib:$ORIGIN/")

# Set default build type explicitly to Release
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

if (WIN32)
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../SFML-2.4.2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML 2 REQUIRED COMPONENTS main)
endif ()

find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})

# Define sources and executable
set(SOURCE_FILES main.cpp system/Application.cpp system/Application.hpp nodes/SceneNode.cpp nodes/SceneNode.hpp nodes/Category.hpp nodes/SpriteNode.cpp nodes/SpriteNode.hpp system/Command.cpp system/Command.hpp system/CommandQueue.cpp system/CommandQueue.hpp ResourceIdentifiers.hpp system/ResourceHolder.hpp system/StateStack.hpp system/StateStack.cpp system/State.hpp system/State.cpp states/StateIdentifiers.hpp states/GameState.cpp states/GameState.hpp states/TitleState.cpp states/TitleState.hpp gui/Container.cpp gui/Container.hpp gui/Component.cpp gui/Component.hpp gui/Button.cpp gui/Button.hpp system/SoundPlayer.cpp system/SoundPlayer.hpp system/MusicPlayer.cpp system/MusicPlayer.hpp nodes/SoundNode.cpp nodes/SoundNode.hpp states/PauseState.cpp states/PauseState.hpp system/Utility.hpp system/Utility.cpp system/Animation.cpp system/Animation.hpp system/Particle.hpp nodes/ParticleNode.cpp nodes/ParticleNode.hpp nodes/EmitterNode.cpp nodes/EmitterNode.hpp Player.cpp Player.hpp World.cpp World.hpp entities/Entity.cpp entities/Entity.hpp Paddle.cpp Paddle.hpp Block.cpp Block.hpp DataTables.cpp DataTables.hpp Ball.cpp Ball.hpp Wall.cpp Wall.hpp Score.cpp Score.hpp states/GameOverState.cpp states/GameOverState.hpp Level.cpp Level.hpp Life.cpp Life.hpp ScoreBoard.cpp ScoreBoard.hpp gui/TextInput.cpp gui/TextInput.hpp states/NewHighscoreState.cpp states/NewHighscoreState.hpp tween/Tweenable.cpp tween/Tweenable.hpp tween/Tween.cpp tween/Tween.hpp tween/LinearTween.cpp tween/LinearTween.hpp tween/EaseOutElastic.cpp tween/EaseOutElastic.hpp LevelInfo.cpp LevelInfo.hpp system/Observable.cpp system/Observable.hpp states/SettingsState.cpp states/SettingsState.hpp system/SettingsParser.cpp system/SettingsParser.hpp gui/Label.cpp gui/Label.hpp)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif ()

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

# Install target
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/install")
install(DIRECTORY assets DESTINATION "${CMAKE_SOURCE_DIR}/install")

add_custom_target(run_install
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target install
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME}")

if(WIN32)
    set (ZIP_INCLUDE_FILES "Break-it.exe" "assets" "openal32.dll")
else()
    set (ZIP_INCLUDE_FILES "Break-it" "assets" "Start" "lib")
endif ()

add_custom_command(TARGET run_install POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${PROJECT_NAME}-${VERSION}-${CMAKE_SYSTEM_NAME}.zip" --format=zip ${ZIP_INCLUDE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/install
        DEPENDS run_install
        COMMENT "Packing ${PROJECT_NAME}")
