
PROJECT( MGV_MOD_INDEX )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_MOD_INDEX_SOURCE_DIR}
    ${MGV_MOD_INDEX_BINARY_DIR}
)

SET( SRC_FILES

    IndexDlg.cpp
    IndexModule.cpp
    IndexView.cpp
    IndexDelegate.cpp
)

SET( HDR_FILES

    IndexDlg.h
    IndexModule.h
    IndexView.h
    IndexDelegate.h
)

SET( UI_FILES

    IndexDlg.ui
)

SET( HID_FILES

    IndexViewActions.hid
)

QT_MOC( MOC_FILES ${HDR_FILES} )
QT_UIC( UIC_FILES ${UI_FILES} )
HIC( HIC_FILES ${HID_FILES} )

ADD_MGV_MODULE(
    Index

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
    ${UIC_FILES}
    ${HIC_FILES}
)
