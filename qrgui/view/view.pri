HEADERS += \
	view/editorView.h \
	view/editorViewScene.h \
	view/sceneCustomizationInterface.h \
	view/sceneCustomizer.h \
	view/propertyEditorView.h \
	view/private/pushButtonProperty.h \
	view/private/editorViewMVIface.h \
	view/gestures/pathCorrector.h \
	view/gestures/mouseMovementManager.h \
	view/gestures/levenshteinDistance.h \
	view/gestures/keyManager.h \
	view/gestures/keyManagerInterface.h \
	view/gestures/abstractRecognizer.h \
	view/gestures/mixedgesturesmanager.h \
	view/gestures/rectanglegesturesmanager.h \
	view/gestures/nearestposgridgesturesmanager.h \
	view/gestures/curveKeyBuilder.h \
	view/gestures/sorts.h \
	view/copyPaste/pasteCommand.h \
	view/copyPaste/clipboardHandler.h \
	view/copyPaste/pasteNodeCommand.h \
	view/copyPaste/pasteGroupCommand.h \
	view/copyPaste/pasteEdgeCommand.h \
    view/private/exploserView.h

SOURCES += \
	view/editorView.cpp \
	view/editorViewScene.cpp \
	view/sceneCustomizer.cpp \
	view/propertyEditorView.cpp \
	view/private/pushButtonProperty.cpp \
	view/private/editorViewMVIface.cpp \
	view/gestures/pathCorrector.cpp \
	view/gestures/mouseMovementManager.cpp \
	view/gestures/levenshteinDistance.cpp \
	view/gestures/keyManager.cpp \
	view/gestures/mixedgesturesmanager.cpp \
	view/gestures/rectanglegesturesmanager.cpp \
	view/gestures/nearestposgridgesturesmanager.cpp \
	view/copyPaste/pasteCommand.cpp \
	view/copyPaste/clipboardHandler.cpp \
	view/copyPaste/pasteNodeCommand.cpp \
	view/copyPaste/pasteGroupCommand.cpp \
	view/copyPaste/pasteEdgeCommand.cpp \
    view/private/exploserView.cpp
