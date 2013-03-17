
PROJECT( MGV_MOD_CUSTOM_COMMANDS )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_CUSTOM_COMMANDS_SOURCE_DIR}
    ${MGV_MOD_CUSTOM_COMMANDS_BINARY_DIR}
)

SET( SRC_FILES

    CustomCommandsView.cpp
    CustomCommandsModule.cpp
    CustomCommandListCfgPage.cpp
)

SET( HDR_FILES

    CustomCommandsView.hpp
    CustomCommandsModule.hpp
    CustomCommandListCfgPage.hpp
)

SET( UI_FILES

    EditCustomCommandDlg.ui
    CustomCommandListCfgPage.ui
)

QT_UIC( UIC_FILES ${UI_FILES} )
QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_MGV_MODULE(
    CustomCommands

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES} ${UI_FILES}
)

MSVC_SPLIT_SOURCES( ModCustomCommands )

