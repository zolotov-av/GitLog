import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import GitCore

ApplicationWindow {
    id: window
    title: qsTr("Create brunch")
    width: 500
    height: 600
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    //visible: true

    property int commitIndex: -1

    ColumnLayout {
        id: layout
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10

        LineInput {
            id: branchName
            label: qsTr("Branch name")
            placeholderText: qsTr("Enter branch name...")
            Layout.fillWidth: true
        }

        GroupBox {
            Layout.fillWidth: true

            label: Label {
                text: qsTr("Base On")
                font.bold: true
                font.pixelSize: GitTheme.smallerFont
            }

            GridLayout {
                id: baseOnLayout
                columns: 2
                anchors.fill: parent

                RadioButton {
                    text: qsTr("HEAD")
                    checked: false
                    enabled: false
                }
                Item {

                }

                RadioButton {
                    text: qsTr("Branch")
                    enabled: false
                }
                RefsComboBox {
                    Layout.fillWidth: true
                    model: form.branchesModel
                }

                RadioButton {
                    text: qsTr("Tag")
                    enabled: false
                }
                RefsComboBox {
                    Layout.fillWidth: true
                    model: form.tagsModel
                }

                RadioButton {
                    text: qsTr("Commit")
                    enabled: true
                    checked: true
                }
                Label {
                    Layout.fillWidth: true
                    text: form.commitHash
                }

            }
        }

        GroupBox {
            Layout.fillWidth: true

            label: Label {
                text: qsTr("Options")
                font.bold: true
                font.pixelSize: GitTheme.smallerFont
            }

            RowLayout {
                id: optionsLayout

                CheckBox {
                    text: qsTr("Track")
                }

                CheckBox {
                    text: qsTr("Force")
                }

                CheckBox {
                    text: qsTr("Switch to new branch")
                }
            }
        }

        RowLayout {
            id: buttons
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Create")
                onClicked: {
                    window.hide()
                    form.accept()
                }
            }

            Button {
                text: qsTr("Cancel")
                onClicked: window.hide()
            }
        }
    }
}
