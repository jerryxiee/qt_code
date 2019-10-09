import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2

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
        target: bubiaoconfig
        onPressed:{
            if(setindex == index){

                console.log("电话设置")

            }
        }
    }

    Label{
        id:phonestyle
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 80
        text: qsTr("终端电话接听策略")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:combophonestyle
        anchors.verticalCenter: phonestyle.verticalCenter
        anchors.left: phonestyle.right
        anchors.leftMargin: 10
        width: 400
        model: ["自动接听","ACC ON时自动接听，OFF时手动接听"]
    }

    Label{
        id:monitornum
        anchors.top: phonestyle.bottom
        anchors.topMargin: 10
        anchors.left: phonestyle.left
        text: qsTr("监听电话")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:monitornumtext
        width: 200
        anchors.verticalCenter: monitornum.verticalCenter
        anchors.left: monitornum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:resetphonenum
        anchors.verticalCenter: monitornum.verticalCenter
        anchors.left: monitornumtext.right
        anchors.leftMargin: 20
        text: qsTr("监听电话")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:resetphonenumtext
        width: 200
        anchors.verticalCenter: monitornum.verticalCenter
        anchors.left: resetphonenum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:platformnum
        anchors.top: monitornum.bottom
        anchors.topMargin: 10
        anchors.left: monitornum.left
        text: qsTr("平台电话")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:platformnumtext
        width: 200
        anchors.verticalCenter: platformnum.verticalCenter
        anchors.left: platformnum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:smsphonenum
        anchors.verticalCenter: platformnum.verticalCenter
        anchors.left: platformnumtext.right
        anchors.leftMargin: 20
        text: qsTr("SMS电话")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:smsphonenumtext
        width: 200
        anchors.verticalCenter: platformnum.verticalCenter
        anchors.left: smsphonenum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:time
        anchors.top: platformnum.bottom
        anchors.topMargin: 10
        anchors.left: platformnum.left
        text: qsTr("每次最长通话时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:timetext
        width: 200
        anchors.verticalCenter: time.verticalCenter
        anchors.left: time.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:timesec
        anchors.verticalCenter: timetext.verticalCenter
        anchors.left: timetext.right
        anchors.leftMargin: 10
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }


    Label{
        id:time1
        anchors.top: time.bottom
        anchors.topMargin: 10
        anchors.left: time.left
        text: qsTr("当月最长通话时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:time1text
        width: 200
        anchors.verticalCenter: time1.verticalCenter
        anchors.left: time1.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:time1sec
        anchors.verticalCenter: time1text.verticalCenter
        anchors.left: time1text.right
        anchors.leftMargin: 10
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:emsnum
        anchors.top: time1.bottom
        anchors.topMargin: 10
        anchors.left: time1.left
        text: qsTr("平台特权短信号码")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:emsnumtext
        width: 200
        anchors.verticalCenter: emsnum.verticalCenter
        anchors.left: emsnum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:factorynum
        anchors.top: emsnum.bottom
        anchors.topMargin: 10
        anchors.left: emsnum.left
        text: qsTr("恢复出厂设置电话")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:factorynumtext
        width: 200
        anchors.verticalCenter: factorynum.verticalCenter
        anchors.left: factorynum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:smsalarmnum
        anchors.top: factorynum.bottom
        anchors.topMargin: 10
        anchors.left: factorynum.left
        width: factorynum.width
        text: qsTr("SMS报警电话")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:smsalarmnumtext
        width: 200
        anchors.verticalCenter: smsalarmnum.verticalCenter
        anchors.left: smsalarmnum.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
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

