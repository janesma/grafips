import QtQuick 2.0
import QtQuick.Controls 1.0
import Grafips 1.0

Item {
    property var model: controlModel
    Column {
        CheckBox {
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
    }
    ApiControlModel {
        id: controlModel
    }
    
}
