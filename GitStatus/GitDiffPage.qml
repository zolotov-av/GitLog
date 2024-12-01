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

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: header
            implicitHeight: headerLayout.implicitHeight + rowLayout.anchors.margins * 2
            color: "burlywood"
            Layout.fillWidth: true

            RowLayout {
                id: headerLayout
                anchors.fill: parent
                anchors.margins: 8
                spacing: 26

                Label {
                    text: qsTr("Diff")
                }

                Label {
                    text: root.fileName
                    color: root.fileColor
                }

                Label {
                    text: "@ " + root.statusSource
                }

                Item {
                    Layout.fillWidth: true
                }

            }

        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            DiffArea {
                id: diffArea
                model: GitDiffModel {

                }

                readOnly: true
                anchors.fill: parent
            }
        }

        Rectangle {
            id: hints
            color: "burlywood"
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
            }

        }
    }

}
