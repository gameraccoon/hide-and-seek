#-------------------------------------------------
#
# Project created by QtCreator 2019-04-21T17:11:34
#
#-------------------------------------------------

QT       += core gui \
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StealthGameEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/editorcommands/changeentitycommand.cpp \
    src/editorcommands/editorcommandsstack.cpp \
    src/componenteditcontent/abstracteditfactory.cpp \
    src/componenteditcontent/componentcontentfactory.cpp \
    src/componenteditcontent/componentregistration.cpp \
    src/componenteditcontent/typeseditconstructor.cpp \
    src/componenteditcontent/generated/CameraComponentEditFactory.generated.cpp \
    src/componenteditcontent/generated/CollisionComponentEditFactory.generated.cpp \
    src/componenteditcontent/generated/LightComponentEditFactory.generated.cpp \
    src/componenteditcontent/generated/MovementComponentEditFactory.generated.cpp \
    src/componenteditcontent/generated/RenderComponentEditFactory.generated.cpp \
    src/componenteditcontent/generated/TransformComponentEditFactory.generated.cpp \
    src/editorcommands/generated/ChangeCameraComponentIsFloatingCommand.generated.cpp \
    src/editorcommands/generated/ChangeCollisionComponentBoundingBoxCommand.generated.cpp \
    src/editorcommands/generated/ChangeCollisionComponentGeometryCommand.generated.cpp \
    src/editorcommands/generated/ChangeCollisionComponentIsBoundingBoxDirtyCommand.generated.cpp \
    src/editorcommands/generated/ChangeCollisionComponentOriginalBoundingBoxCommand.generated.cpp \
    src/editorcommands/generated/ChangeLightComponentBrightnessCommand.generated.cpp \
    src/editorcommands/generated/ChangeMovementComponentLastStepCommand.generated.cpp \
    src/editorcommands/generated/ChangeMovementComponentSpeedCommand.generated.cpp \
    src/editorcommands/generated/ChangeRenderComponentAnchorCommand.generated.cpp \
    src/editorcommands/generated/ChangeRenderComponentScaleCommand.generated.cpp \
    src/editorcommands/generated/ChangeRenderComponentSizeCommand.generated.cpp \
    src/editorcommands/generated/ChangeRenderComponentTextureHandleCommand.generated.cpp \
    src/editorcommands/generated/ChangeRenderComponentTexturePathCommand.generated.cpp \
    src/editorcommands/generated/ChangeTransformComponentLocationCommand.generated.cpp \
    src/editorcommands/generated/ChangeTransformComponentRotationCommand.generated.cpp

HEADERS += \
    src/mainwindow.h \
    src/editorcommands/editorcommand.h \
    src/editorcommands/changeentitycommand.h \
    src/editorcommands/editorcommandsstack.h \
    src/componenteditcontent/abstracteditfactory.h \
    src/componenteditcontent/componentcontentfactory.h \
    src/componenteditcontent/componentregistration.h \
    src/componenteditcontent/typeseditconstructor.h \
    src/componenteditcontent/typeeditconstructorhelpers.h \
    src/componenteditcontent/generated/CameraComponentEditFactory.generated.h \
    src/componenteditcontent/generated/CollisionComponentEditFactory.generated.h \
    src/componenteditcontent/generated/LightComponentEditFactory.generated.h \
    src/componenteditcontent/generated/MovementComponentEditFactory.generated.h \
    src/componenteditcontent/generated/RenderComponentEditFactory.generated.h \
    src/componenteditcontent/generated/TransformComponentEditFactory.generated.h \
    src/editorcommands/generated/ChangeCameraComponentIsFloatingCommand.generated.h \
    src/editorcommands/generated/ChangeCollisionComponentBoundingBoxCommand.generated.h \
    src/editorcommands/generated/ChangeCollisionComponentGeometryCommand.generated.h \
    src/editorcommands/generated/ChangeCollisionComponentIsBoundingBoxDirtyCommand.generated.h \
    src/editorcommands/generated/ChangeCollisionComponentOriginalBoundingBoxCommand.generated.h \
    src/editorcommands/generated/ChangeMovementComponentLastStepCommand.generated.h \
    src/editorcommands/generated/ChangeLightComponentBrightnessCommand.generated.h \
    src/editorcommands/generated/ChangeMovementComponentSpeedCommand.generated.h \
    src/editorcommands/generated/ChangeRenderComponentAnchorCommand.generated.h \
    src/editorcommands/generated/ChangeRenderComponentScaleCommand.generated.h \
    src/editorcommands/generated/ChangeRenderComponentSizeCommand.generated.h \
    src/editorcommands/generated/ChangeRenderComponentTextureHandleCommand.generated.h \
    src/editorcommands/generated/ChangeRenderComponentTexturePathCommand.generated.h \
    src/editorcommands/generated/ChangeTransformComponentLocationCommand.generated.h \
    src/editorcommands/generated/ChangeTransformComponentRotationCommand.generated.h

FORMS += \
    forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/static-libs/release/ -lEngineLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/static-libs/debug/ -lEngineLib
else:unix: LIBS += -L$$PWD/../bin/static-libs/ -lEngineLib

INCLUDEPATH += $$PWD/../src/Engine/include
DEPENDPATH += $$PWD/../src/Engine/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/release/libEngineLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/debug/libEngineLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/release/EngineLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/debug/EngineLib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../bin/static-libs/libEngineLib.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/static-libs/release/ -lDebugHelpersLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/static-libs/debug/ -lDebugHelpersLib
else:unix: LIBS += -L$$PWD/../bin/static-libs/ -lDebugHelpersLib

INCLUDEPATH += $$PWD/../src/DebugHelpers/include
DEPENDPATH += $$PWD/../src/DebugHelpers/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/release/libDebugHelpersLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/debug/libDebugHelpersLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/release/DebugHelpersLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../bin/static-libs/debug/DebugHelpersLib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../bin/static-libs/libDebugHelpersLib.a

# externals
INCLUDEPATH += $$PWD/../external/include

# googletest
INCLUDEPATH += $$PWD/../external/googletest

SOURCES += \
    ../external/googletest/src/gtest-all.cc

HEADERS += \
    ../external/googletest/gtest/internal/custom/gtest-port.h \
    ../external/googletest/gtest/internal/custom/gtest-printers.h \
    ../external/googletest/gtest/internal/custom/gtest.h \
    ../external/googletest/gtest/internal/gtest-death-test-internal.h \
    ../external/googletest/gtest/internal/gtest-filepath.h \
    ../external/googletest/gtest/internal/gtest-internal.h \
    ../external/googletest/gtest/internal/gtest-linked_ptr.h \
    ../external/googletest/gtest/internal/gtest-param-util-generated.h \
    ../external/googletest/gtest/internal/gtest-param-util.h \
    ../external/googletest/gtest/internal/gtest-port-arch.h \
    ../external/googletest/gtest/internal/gtest-port.h \
    ../external/googletest/gtest/internal/gtest-string.h \
    ../external/googletest/gtest/internal/gtest-tuple.h \
    ../external/googletest/gtest/internal/gtest-tuple.h.pump \
    ../external/googletest/gtest/internal/gtest-type-util.h \
    ../external/googletest/gtest/gtest-death-test.h \
    ../external/googletest/gtest/gtest-message.h \
    ../external/googletest/gtest/gtest-param-test.h \
    ../external/googletest/gtest/gtest-param-test.h.pump \
    ../external/googletest/gtest/gtest-printers.h \
    ../external/googletest/gtest/gtest-spi.h \
    ../external/googletest/gtest/gtest-test-part.h \
    ../external/googletest/gtest/gtest-typed-test.h \
    ../external/googletest/gtest/gtest.h \
    ../external/googletest/gtest/gtest_pred_impl.h \
    ../external/googletest/gtest/gtest_prod.h \
    ../external/googletest/src/gtest-internal-inl.h

unix|win32: LIBS += -lstdc++fs
