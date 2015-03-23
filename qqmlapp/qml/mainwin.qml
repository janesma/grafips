import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import Grafips 1.0

ApplicationWindow {
    width: 600
    height: 500
    visible: true
    id: mainWindow

    function usage() {
        print("usage: grafips [-a host:port] [-h]");
        print("\t-a: provide host and port of target system");
        print("\t-h: print this message");
    }
    
    function processArgs() {
        var address = "localhost:53136";
        for (var i = 0; i < Qt.application.arguments.length; ++i) {
            if (Qt.application.arguments[i] == "-a") {
                if (i + 1 < Qt.application.arguments.length) {
                    address = Qt.application.arguments[i+1];
                    i += 1;
                } else {
                    print("Error: -a requires parameter");
                    break;
                }
            }
            if (Qt.application.arguments[i] == "-h") {
                usage();
            }
        }
        return address;
    }

    MetricRouter {
        id: mainPublisher
    }
    ControlRouterHost {
        id: controlRouter
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
                text: processArgs()
                onAccepted: {
                    cpu.start();
                    glFt.start();
                    glFps.start();
                    glGpu.start();
                    mainPublisher.address = text
                    controlRouter.address = text
                    row.visible = true
                    connectionDialog.visible = false
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        id: row
        visible: false
        spacing: 0


        ColumnLayout {
            Layout.preferredWidth: 500
            Layout.alignment: Qt.AlignTop
            MetricList {
                id: currentList
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: mainPublisher.metrics
                publisher: mainPublisher
                dragParent: mainWindow.contentItem
            }
            Rectangle {
                height: 20
            }
            CpuFreqPolicy {
                id: frequencyControl
                implicitHeight: frequencyControl.suggested_height
                Layout.fillWidth: true
                Component.onCompleted: {
                    frequencyControl.model.SetControlRounter(controlRouter)
                }
            }
            ApiControl {
                id: apiControl
                implicitHeight: apiControl.suggested_height
                Layout.fillWidth: true
                Component.onCompleted: {
                    apiControl.model.SetControlRounter(controlRouter)
                }
            }
        }
        ColumnLayout
        {
            Layout.preferredWidth: 1500
            id: mainView
            
            MetricGroup {
                id: cpu
                publisher: mainPublisher
            }
            MetricGroup {
                id: glFt
                publisher: mainPublisher
            }
            MetricGroup {
                id: glFps
                publisher: mainPublisher
            }
            MetricGroup {
                id: glGpu
                publisher: mainPublisher
            }
        }
    }
}
