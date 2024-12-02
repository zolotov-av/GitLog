import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore

Item {
    id: root

    required property string fileName
    required property string fileStatus
    required property color fileColor
    required property string statusSource

    function gitLeave() {
        stackView.pop()
    }

    function handleKeys(event) {
        if (event.modifiers !== 0)
            return;

        switch (event.key )
        {
        case Qt.Key_Escape:
        case Qt.Key_Backspace:
            gitLeave()
            event.accepted = true;
            break;
        }
    }

    Keys.onPressed: event => handleKeys(event)
    StackView.onActivated: {
        headModel.update()
        patchModel.update()
        diffModel.update()
        diffArea.editor.forceActiveFocus()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: header
            implicitHeight: headerLayout.implicitHeight + rowLayout.anchors.margins * 2
            color: "#FF5722"
            Layout.fillWidth: true

            RowLayout {
                id: headerLayout
                anchors.fill: parent
                anchors.margins: 8
                spacing: 26

                Label {
                    text: qsTr("Merge")
                }

                Label {
                    text: root.fileName
                    //color: root.fileColor
                }

                Label {
                    text: "@ " + root.statusSource
                }

                Item {
                    Layout.fillWidth: true
                }

            }

        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 0

            Item {
                Layout.fillWidth: true
                implicitHeight: headLabel.implicitHeight
                //implicitWidth: headLabel.implicitWidth
                //color: "#795548"

                Label {
                    id: headLabel
                    text: "HEAD"
                    anchors.centerIn: parent
                }
            }

            Item {
                Layout.fillWidth: true
                implicitHeight: patchLabel.implicitHeight
                Label {
                    id: patchLabel
                    text: "Patch"
                    anchors.centerIn: parent
                }
            }

            Item {
                Layout.fillWidth: true
                implicitHeight: targetLabel.implicitHeight
                Label {
                    id: targetLabel
                    text: "Target"
                    anchors.centerIn: parent
                }
            }

        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Rectangle {
                color: "white"
                Layout.fillWidth: true
                Layout.fillHeight: true

                DiffArea {
                    id: headView
                    readOnly: true
                    anchors.fill: parent

                    model: GitDiffModel {
                        id: headModel
                        repository: gitRepo
                        filePath: root.fileName
                        fileSource: "HEAD"
                    }
                }
            }

            Rectangle {
                color: "lightgrey"
                Layout.fillWidth: true
                Layout.fillHeight: true

                DiffArea {
                    id: patchView
                    readOnly: true
                    anchors.fill: parent

                    model: GitDiffModel {
                        id: patchModel
                        repository: gitRepo
                        filePath: root.fileName
                        fileSource: "patch"
                    }
                }
            }

            Rectangle {
                color: "#8BC34A"
                Layout.fillWidth: true
                Layout.fillHeight: true
                DiffArea {
                    id: diffArea
                    readOnly: true
                    anchors.fill: parent

                    model: GitDiffModel {
                        id: diffModel
                        repository: gitRepo
                        filePath: root.fileName
                        fileSource: root.statusSource
                    }
                }
            }
        }

        Rectangle {
            id: hints
            color: header.color
            implicitHeight: rowLayout.implicitHeight + rowLayout.anchors.margins * 2
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                anchors.margins: 8
                spacing: 26

                Label {
                    text: qsTr("Hints")
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    id: backButton
                    text: qsTr("Back")
                    onClicked: root.gitLeave()
                }
            }

        }
    }

}
