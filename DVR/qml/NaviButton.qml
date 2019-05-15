import QtQuick 2.7
import QtQuick.Controls 2.0
//import "Style"

AbstractButton {
    id: button

//    property int edge: Qt.TopEdge
    property alias imageSource: image.source
    property int imagewidth
    property int imageheight

    contentItem: Image {
        id: image
        fillMode: Image.Pad
        sourceSize { width: imagewidth; height: imageheight } // ### TODO: resize the image
    }

    background: Rectangle {
        height: button.height
        width: button.width
//        radius: width / 2
        border.color: "red"
//        color: "blue"

//        anchors.horizontalCenter: button.horizontalCenter
//        anchors.top: edge === Qt.BottomEdge ? button.top : undefined
//        anchors.bottom: edge === Qt.TopEdge ? button.bottom : undefined


//        color: UIStyle.colorQtGray2
    }

//    transform: Translate {
//        Behavior on y { NumberAnimation { } }
//        y: enabled ? 0 : edge === Qt.TopEdge ? -button.height : button.height
//    }
}
