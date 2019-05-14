import QtQuick 2.0
import QtQml 2.2
import QtQuick.Controls 2.2

Button {
    id: control

    property alias buttonText: control.text
    property alias imageSource: image.source

    contentItem: Image {
//        Image {
            id: image
            fillMode: Image.Pad
            sourceSize { width: 40; height: 40 }

//            sourceSize: { width: 40/*control.width >0 ? control.width:40*/
//                height: 40/*control.height > 0 ? control.height:40*/ }
//            source: "file"
//        }
//        Text {
//            anchors.top: parent.bottom
//            anchors.topMargin: 5
//            anchors.horizontalCenter: parent.horizontalCenter
//            width: contentWidth
//            text: control.text
//            color: "#ffffff"
//            font.pixelSize: 12

//        }
    }
    background: Rectangle{
        height: control.height
        width: height
//        radius: width / 2
        border.color: "red"

    }

//    background: Image {
//        source: pressed ? "images/back.png" : "images/back.png"
//    }

}
