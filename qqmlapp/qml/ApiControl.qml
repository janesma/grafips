import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import Grafips 1.0

Item {
    property var model: controlModel
    property var metricsRouter
    property int suggested_height: first_box.height * 6
    Column {
        CheckBox {
            id: first_box
            text: qsTr("1x1 scissor")
            checked: false
            onClicked: {
                controlModel.SetControl("ScissorExperiment", checked);
            }
        }
        CheckBox {
            text: qsTr("2x2 textures")
            checked: false
            onClicked: {
                controlModel.SetControl("2x2TextureExperiment", checked);
            }
        }
        CheckBox {
            text: qsTr("Simple Shader")
            checked: false
            onClicked: {
                controlModel.SetControl("SimpleShaderExperiment", checked);
            }
        }
        CheckBox {
            text: qsTr("Disable Draw")
            checked: false
            onClicked: {
                controlModel.SetControl("DisableDrawExperiment", checked);
            }
        }
        CheckBox {
            text: qsTr("Wireframe")
            checked: false
            onClicked: {
                controlModel.SetControl("WireframeExperiment", checked);
            }
        }
        Row {
            CheckBox {
                id: fileCheck
                text: qsTr("Save to file")
                checked: false
                onClicked: {
                    metricsRouter.SetText(checked);
                }
            }
            Rectangle {
                id: file_rect
                border.width: 1
                width: fileCheck.height
                height: fileCheck.height
                Text {
                    anchors.centerIn: parent
                    text: "?"
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed : {
                        fileDialog.visible =  true
                    }
                }

                FileDialog {
                    id: fileDialog
                    visible: false
                    title: "Please choose a file" // 
                    folder: "file:///tmp/"
                    selectExisting: false
                    selectMultiple: false
                    nameFilters: [ "html files (*.html)", "All files (*)" ]
                    onAccepted: {
                        metricsRouter.SetTextFile(fileDialog.fileUrl);
                    }
                }
            }
        }
    }
    ApiControlModel {
        id: controlModel
    }
    
}
