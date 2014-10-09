import QtQuick 2.3
import GripsRendering 1.0

Rectangle {
    width: 200
    height: 100
    color: "green"

    Renderer {
        id: renderer
        width: 200
        height: 100
        anchors.fill: parent
        anchors.margins: 10

        // The transform is just to show something interesting..
        // transform: [
        //     Rotation { id: rotation; axis.x: 0; axis.z: 0; axis.y: 1; angle: 0; origin.x: renderer.width / 2; origin.y: renderer.height / 2; },
        //     Translate { id: txOut; x: -renderer.width / 2; y: -renderer.height / 2 },
        //     Scale { id: scale; },
        //     Translate { id: txIn; x: renderer.width / 2; y: renderer.height / 2 }
        // ]
    }
    // property int animatedValue: 0
    // SequentialAnimation on animatedValue {
    //     loops: Animation.Infinite
    //     PropertyAnimation { to: 150; duration: 1000 }
    //     PropertyAnimation { to: 0; duration: 1000 }
    // }

    // Text {
    //     anchors.centerIn: parent
    //     text: animatedValue
    // }
    // MouseArea {
    //     anchors.fill: parent
    //     onClicked: parent.color = "blue"
    // }
    // focus: true
    // Keys.onPressed: {
    //     if (event.key == Qt.Key_Return) {
    //         color = "blue";
    //         event.accepted = true;
    //     }
    //     else {
    //         color = "red";
    //         event.accepted = true;
    //     }
    // }

    // Rectangle {
    //     width: parent.width / 2
    //     height: parent.height/4
    //     color: "orange"
    //     property color nextColor: "blue"
    // }

    // Rectangle {
    //     width: parent.width / 2
    //     height: parent.height/4
    //     x: parent.width / 2
    //     color: "green"
    // }

    // Column {
    //     width: 200; height: 200; y:parent.height*3/4

    //     TextInput { id: myTextInput; text: "Hello World" }

    //     Text { text: myTextInput.text }
    // }

}
