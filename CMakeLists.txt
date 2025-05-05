cmake_minimum_required(VERSION 3.30)
project(Client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(I18N_TRANSLATED_LANGUAGES ru)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        WebSockets
        LinguistTools
        Test)
include_directories(${Qt6_INCLUDE_DIRS})

qt6_add_translations(Client TS_FILES src/translations/ru.ts)

set(project_files
        src/ui/main_window.cpp src/ui/main_window.h
        src/ui/auth/login_page.cpp src/ui/auth/login_page.h
        src/ui/game/game_page.cpp src/ui/game/game_page.h
        src/ui/game/lobby_page.cpp src/ui/game/lobby_page.h
        src/ui/game/game_item_widget.h
        src/common/websocket_worker.h
        src/common/base_page.h
        src/common/message/command_code.h
        src/common/message/tools.h
        src/common/message/message.h
        src/model/player.h
        src/resource.qrc)

set(libraries
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Network
        Qt6::WebSockets)


add_executable(Client src/main.cpp ${project_files})

target_link_libraries(Client ${libraries})

add_executable(test_auth tests/auth_test.cpp ${project_files})
target_link_libraries(test_auth ${libraries} Qt6::Test)
add_test(NAME auth COMMAND test_auth)

add_executable(test_lobby tests/lobby_test.cpp ${project_files})
target_link_libraries(test_lobby ${libraries} Qt6::Test)
add_test(NAME lobby COMMAND test_lobby)

add_executable(test_game tests/game_test.cpp ${project_files})
target_link_libraries(test_game ${libraries} Qt6::Test)
add_test(NAME game COMMAND test_game)
