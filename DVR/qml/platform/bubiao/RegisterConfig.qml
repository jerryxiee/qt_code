import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2
import PlatformSet 1.0

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    property bool valueChanged: false
    property int labelwidth: 200
    property int labelheight: 80

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

                console.log("注册配置")
                platformset.registerEnable = checkenable.checked
                platformset.mainEnableTcp = maintcpenable.checked
                platformset.mainEnableUdp = mainudpenable.checked
                platformset.secEnableTcp = scetcpenable.checked
                platformset.secEnableUdp = sceudpenable.checked
                platformset.registerType = comboregistertype.currentIndex
                platformset.mainServer = mainserveriptext.text+":"+maintcpporttext.text+":"+mainudpporttext.text
                platformset.backupServer  = secserveriptext.text+":"+sectcpporttext.text+":"+secudpporttext.text
                platformset.phoneNumber = phonenumtext.text
                platformset.carLisence = cardlicensetext.text
            }
        }
    }

    PlatformSet{
        id:platformset
    }

    Label{
        id:enableregister
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 100
        width: labelwidth
        text: qsTr("注册使能")
        font.pixelSize: fontpixelSize
    }

    CheckBox{
        id:checkenable
        anchors.verticalCenter: enableregister.verticalCenter
        anchors.left:enableregister.right
        checked: platformset.registerEnable
//        anchors.leftMargin: 20
    }

    Label{
        id:registertype
        anchors.verticalCenter: enableregister.verticalCenter
        anchors.left: checkenable.right
        anchors.leftMargin: 20
        width: labelwidth
        text: qsTr("注册方式")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:comboregistertype
        width: labelwidth
        anchors.verticalCenter: enableregister.verticalCenter
        anchors.left: registertype.right
        currentIndex: platformset.registerType
        model: ["主从方式","同时注册"]
    }


    Label{
        id:mainserverip
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: enableregister.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("主服务器")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:mainserveriptext
        width: boxwidth
        anchors.verticalCenter: mainserverip.verticalCenter
        anchors.left: mainserverip.right
        text: platformset.mainServerIp
//        inputMethodHints: Qt.ImhUppercaseOnly | Qt.ImhDigitsOnly
        onPressed: {
            keyboardvir.visible = true
            pressPos(x,y,width,height)
        }
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:connectstatus
        anchors.verticalCenter: mainserverip.verticalCenter
        anchors.left: mainserveriptext.right
        anchors.leftMargin: 10
        width: labelwidth
        text: qsTr("连接状态")
        font.pixelSize: fontpixelSize
    }
    Label{
        id:connectresult
        anchors.verticalCenter: mainserverip.verticalCenter
        anchors.left: connectstatus.right
//        anchors.leftMargin: 10
        width: labelwidth
        text: qsTr("未连接")
        font.pixelSize: fontpixelSize
    }



    Label{
        id:maintcpport
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: mainserverip.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("TCP端口")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:maintcpporttext
        width: boxwidth
        anchors.verticalCenter: maintcpport.verticalCenter
        anchors.left: maintcpport.right
        text: platformset.mainServerTcpPort
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    CheckBox{
        id:maintcpenable
        anchors.verticalCenter: maintcpport.verticalCenter
        anchors.left:maintcpporttext.right
        anchors.leftMargin: 10
        checked: platformset.mainEnableTcp
    }

    Label{
        id:mainudpport
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: maintcpport.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("UDP端口")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:mainudpporttext
        width: boxwidth
        anchors.verticalCenter: mainudpport.verticalCenter
        anchors.left: mainudpport.right
        text: platformset.mainServerUdpPort
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    CheckBox{
        id:mainudpenable
        anchors.verticalCenter: mainudpport.verticalCenter
        anchors.left:mainudpporttext.right
        anchors.leftMargin: 10
        checked: platformset.mainEnableUdp
    }

    Label{
        id:secserverip
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: mainudpport.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("副服务器")
        font.pixelSize: fontpixelSize
    }
    TextField{
        id:secserveriptext
        width: boxwidth
        anchors.verticalCenter: secserverip.verticalCenter
        anchors.left: secserverip.right
        text: platformset.backupServerIp
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:secconnectstatus
        anchors.verticalCenter: secserverip.verticalCenter
        anchors.left: secserveriptext.right
        anchors.leftMargin: 10
        width: labelwidth
        text: qsTr("连接状态")
        font.pixelSize: fontpixelSize
    }
    Label{
        id:secconnectresult
        anchors.verticalCenter: secserverip.verticalCenter
        anchors.left: secconnectstatus.right
//        anchors.leftMargin: 10
        width: labelwidth
        text: qsTr("未连接")
        font.pixelSize: fontpixelSize
    }


    Label{
        id:sectcpport
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: secserverip.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("TCP端口")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:sectcpporttext
        width: boxwidth
        anchors.verticalCenter: sectcpport.verticalCenter
        anchors.left: sectcpport.right
        text: platformset.backupServerTcpPort
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    CheckBox{
        id:scetcpenable
        anchors.verticalCenter: sectcpport.verticalCenter
        anchors.left:sectcpporttext.right
        anchors.leftMargin: 10
        checked: platformset.secEnableTcp
    }

    Label{
        id:secudpport
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: sectcpport.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("UDP端口")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:secudpporttext
        width: boxwidth
        anchors.verticalCenter: secudpport.verticalCenter
        anchors.left: secudpport.right
        text: platformset.backupServerUdpPort
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }
    CheckBox{
        id:sceudpenable
        anchors.verticalCenter: secudpport.verticalCenter
        anchors.left:secudpporttext.right
        anchors.leftMargin: 10
        checked: platformset.secEnableUdp
    }

    Label{
        id:cardlicense
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: secudpport.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("车牌号")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:cardlicensetext
        width: boxwidth
        anchors.verticalCenter: cardlicense.verticalCenter
        anchors.left: cardlicense.right
        text: platformset.carLisence
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:phonenum
        anchors.horizontalCenter: enableregister.horizontalCenter
        anchors.top: cardlicense.bottom
        anchors.topMargin: 10
        width: labelwidth
        text: qsTr("手机号")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:phonenumtext
        width: boxwidth
        anchors.verticalCenter: phonenum.verticalCenter
        anchors.left: phonenum.right
        text: platformset.phoneNumber
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
