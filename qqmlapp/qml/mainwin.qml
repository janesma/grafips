import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true

    PublisherStub {
        id: mainPublisher
    }


    Rectangle {
        id: connectionDialog
        anchors.fill: parent
        Component.onCompleted: visible = true
        Text {
            id: enterText
            width: 400
            height: 20
            anchors.centerIn: parent
            text: "Enter the address of the target system."
        }
        Rectangle {
            anchors.left: enterText.left
            anchors.right: enterText.right
            anchors.top: enterText.bottom
            border.color: "black"
            border.width: 1
            height: 20
            
            TextInput {
                anchors.fill: parent
                anchors.margins: 3
                verticalAlignment: Text.AlignVCenter
                text: "localhost"
                onAccepted: {
                    mainPublisher.address = text
                    mainView.visible = true
                    connectionDialog.visible = false
                    cpu.start();
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
        }

        Rectangle {
            Layout.fillWidth:  true
            Layout.fillHeight: true
            Layout.minimumHeight:20
            Layout.preferredHeight: 100
            color: "green"
        }
        Rectangle {
            Layout.fillWidth:  true
            Layout.fillHeight: true
            Layout.minimumHeight:20
            Layout.preferredHeight: 100
            color: "yellow"
        }
    }
}
