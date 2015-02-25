import QtQuick 2.0
import QtQuick.Controls 1.0
import Grafips 1.0

Item {
    property var model: controlModel
    Column {
        CheckBox {
            id: scissorBox
            text: qsTr("1x1 scissor")
            checked: false
            onClicked: {
                print ("checked1 " + checked);
                controlModel.SetControl("ScissorExperiment", checked);
            }
        }
        CheckBox {
            id: textureBox
            text: qsTr("2x2 textures")
            checked: false
            onClicked: {
                print ("checked2 " + checked);
                controlModel.SetControl("2x2TextureExperiment", checked);
            }
        }
    }
    ApiControlModel {
        id: controlModel
    }
    
}
