import QtQuick 2.0
import QtQuick.Controls 1.0
import Grafips 1.0

Item {
    property var model: cpuModel
    id: control
    visible: false
    property int suggested_height: box.height
    CpuFreqControlModel {
        id: cpuModel
        onPoliciesChanged: {
            control.visible = true;
        }
        // onPolicyChanged: {
        //     box.currentIndex = control_model.currentPolicy
        // }
    }
    ComboBox {
        id: box
        editable: false
        model: cpuModel.policies
        width: parent.width

        onActivated: {
            print("onActivated: " + index + " " + model[index])
            console.assert (find(currentText) !== -1, "invalid selection")
            //currentIndex = find(editText)
            cpuModel.SetPolicy(model[index]);
        }
    }
}
