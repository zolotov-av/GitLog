import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore
import Browse

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("Git Browse")
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
        anchors.fill: parent
        initialItem: "GitRefsPage.qml"
    }
}
