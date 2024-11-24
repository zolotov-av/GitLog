import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore
import MyDiff

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("Git Diff")
    width: 1200
    height: 800
    property alias headerVisible: header.visible
    property int fontSize: 18
    property string fontFamily: "Monospace"

    Component.onCompleted: {
        GitDiff.diffFiles(":/MyDiff/tmp/left.txt", ":/MyDiff/tmp/right.txt")
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: header
            color: "lightgrey"
            Layout.fillWidth: true
            implicitHeight: grid.implicitHeight + grid.anchors.margins * 2
            visible: true

            GridLayout {
                id: grid
                columns: 2
                columnSpacing: 4
                rowSpacing: columnSpacing
                anchors.fill: parent
                anchors.margins: 8

                TextInput {
                    text: GitDiff.oldFileName
                    Layout.fillWidth: true
                    font.pixelSize: root.fontSize
                    font.family: root.fontFamily
                }

                TextInput {
                    text: GitDiff.newFileName
                    Layout.fillWidth: true
                    font.pixelSize: root.fontSize
                    font.family: root.fontFamily
                    horizontalAlignment: Qt.AlignRight
                }
            }
        }

        DiffArea {
            id: rightView
            fontFamily: root.fontFamily
            fontSize: root.fontSize
            model: GitDiff.lineModel
            readOnly: true
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }

}
