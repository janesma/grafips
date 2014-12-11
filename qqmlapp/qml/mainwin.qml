import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true

    MetricRouter {
        id: mainPublisher
    }


    Rectangle {
        id: connectionDialog
        anchors.fill: parent
        Component.onCompleted: visible = true
        Text {
            id: enterText
            width: 400
            anchors.centerIn: parent
            text: "Enter the address of the target system."
        }
        Rectangle {
            anchors.left: enterText.left
            anchors.right: enterText.right
            anchors.top: enterText.bottom
            border.color: "black"
            border.width: 1
            height: input.height + 5
            
            TextInput {
                id: input
                focus: true
                anchors.centerIn: parent
                width:parent.width
                verticalAlignment: Text.AlignVCenter
                text: "localhost:53136"
                onAccepted: {
                    cpu.start();
                    glFt.start();
                    glFps.start();
                    mainPublisher.address = text
                    mainView.visible = true
                    connectionDialog.visible = false
                }
            }
        }
    }

    ColumnLayout
    {
        id: mainView
        anchors.fill: parent
        visible: false
        
        MetricGroup {
            id: cpu
            color: "red"
            publisher: mainPublisher
            filters: ["cpu/system/utilization",
                      "cpu/core/*/utilization"]
        }
        MetricGroup {
            id: glFt
            color: "red"
            publisher: mainPublisher
            filters: ["gl/frame_time"]
        }
        MetricGroup {
            id: glFps
            color: "red"
            publisher: mainPublisher
            filters: ["gl/fps"]
        }
    }
}
