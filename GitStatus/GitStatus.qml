import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("Git Status")
    width: 1200
    height: 800

    font.pixelSize: 20

    Component.onCompleted: {
        gitRepo.open()
    }

    GitRepository {
        id: gitRepo
        path: "/home/alex/prj/test/GitTest"
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "GitStatusPage.qml"
    }

}
