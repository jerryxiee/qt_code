import QtQuick 2.0

Item {
    id: root
    width: parent.width
    height: 88

    property alias backgroundcolor:back.color
    property alias text: textitem.text
    property alias textcolor:textitem.color
    property alias imageSource: image.source
    signal clicked

    Rectangle {
        id:back
        anchors.fill: parent
//        color: mouse.pressed ? "red":"white"
        visible: mouse.pressed
    }

    Image {
        id:image
        visible: source==""?false:true
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
//        source: "qrc:/qml/setting/images/settings.png"
        sourceSize{height: root.height;width: root.width}
    }

    Text {
        id: textitem
//        color: "white"
        font.pixelSize: 32
        text: modelData
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: image.right
        anchors.leftMargin: 5
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
//        anchors.margins: 15
        height: 1
        color: "#424246"
    }

//    Image {
//        anchors.right: parent.right
//        anchors.rightMargin: 20
//        anchors.verticalCenter: parent.verticalCenter
//        source: "images/navigation_next_item.png"
//    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked()

    }

}
