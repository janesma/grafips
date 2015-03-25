import QtQuick 2.0
import QtQuick.Controls 1.0
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
        CheckBox {
            text: qsTr("Save to file")
            checked: false
            onClicked: {
                metricsRouter.SetText(checked);
            }
        }
    }
    ApiControlModel {
        id: controlModel
    }
    
}
