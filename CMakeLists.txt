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
        LinguistTools)
include_directories(${Qt6_INCLUDE_DIRS})

set(TS_FILES translations/ru.ts)

qt_add_translation(QM_FILES ${TS_FILES})
qt_add_resources(GENERATED_RESOURCE_FILES resource.qrc)

#qt6_create_translation(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR} ru.ts)

add_executable(Client main.cpp
        ui/main_window.cpp ui/main_window.h
        ui/auth/login_page.cpp ui/auth/login_page.h
        ui/game/game_page.cpp ui/game/game_page.h
        ui/game/lobby_page.cpp ui/game/lobby_page.h
        ui/game/game_item_widget.h
        common/websocket_worker.h
        common/base_page.h
        common/message/command_code.h
        common/message/tools.h
        common/message/message.h
        model/player.h
        ${QM_FILES}
)

target_link_libraries(Client
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::Network
        Qt6::WebSockets
)

target_sources(Client PRIVATE ${GENERATED_RESOURCE_FILES})
