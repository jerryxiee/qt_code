import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2
import PlatformSet 1.0

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    property bool valueChanged: false

    signal pressPos(int x,int y,int w,int h)

    onPressPos: {
        if(x+keyboardvir.width > parent.width){
            keyboardvir.x = parent.width - keyboardvir.width
        }else{
            keyboardvir.x = x
        }
        if(y + h + keyboardvir.height > parent.height){
            keyboardvir.y = y - keyboardvir.height
        }else{
            keyboardvir.y = y+h
        }

        keyboardvir.visible = true;

    }

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        onClicked: {
            keyboardvir.visible = false

        }
    }


    Connections{
        target: bubiaoregister
        onPressed:{
            if(setindex == index){

                console.log("运行管理")

            }
        }
    }


    Label{
        id:authid
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 80
        text: qsTr("鉴权码")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:authidtext
        width: 300
        anchors.verticalCenter: authid.verticalCenter
        anchors.left: authid.right
        anchors.leftMargin: 20
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Button{
        id:register
        anchors.top: authid.bottom
        anchors.topMargin: 20
        anchors.left: authid.left
        width: 100
        text: qsTr("注册")
        font.pixelSize: fontpixelSize-8
    }

    Button{
        id:platformauth
        anchors.verticalCenter: register.verticalCenter
        anchors.left: register.right
        anchors.leftMargin: 20
        width: 200
        text: qsTr("平台鉴权")
        font.pixelSize: fontpixelSize-8
    }

    Button{
        id:unregister
        anchors.verticalCenter: register.verticalCenter
        anchors.left: platformauth.right
        anchors.leftMargin: 20
        width: 100
        text: qsTr("注消")
        font.pixelSize: fontpixelSize-8
    }

    InputPanel {
        id: keyboardvir
        visible: false
//            anchors.right: parent.right
//            anchors.left: parent.left
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: parent.bottom
        width: parent.width/1.5
//        height: parent.height/3
        onActiveChanged: {
            if(!active) { visible = false; }
        }
    }

}

