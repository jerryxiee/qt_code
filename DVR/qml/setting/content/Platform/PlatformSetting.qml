import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls 2.2 as Control2
import "../../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300


    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Normal"
            Item{
                MyCheckBox{
                    id:platform
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 100
                    checked: true
                    pixSize:fontpixelSize

                    str:qsTr("平台接入")

                }

                Text {
                    id: deviceid
                    width: boxwidth-100
                    text: qsTr("设备ID")
                    anchors.top: platform.bottom
                    anchors.topMargin: 20
                    anchors.left: platform.left
                    font.pixelSize:fontpixelSize
                    enabled: platform.checked
                }

                Control2.TextField {
                    id: textfield_deviceid
                    anchors.top: deviceid.top
                    anchors.left: deviceid.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: platform.checked
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                    }

                    validator: RegExpValidator{regExp:/\d{1,9}/}
                }
                Text {
                    id: serveriptype
                    width: boxwidth-100
                    text: qsTr("服务器地址类型")
                    anchors.top: deviceid.bottom
                    anchors.topMargin: 20
                    anchors.left: deviceid.left
                    font.pixelSize:fontpixelSize
                    enabled: platform.checked
                }

                ComboBox{
                    id:serveriptypebox
                    width: boxwidth
                    height: serveriptype.height
                    anchors.left: serveriptype.right
                    anchors.leftMargin: 50
                    anchors.verticalCenter: serveriptype.verticalCenter
                    model: ["IP地址","域名"]
                    enabled: platform.checked
                }

                Text {
                    id: serverip
                    width: boxwidth-100
                    text: qsTr("IP地址")
                    anchors.top: serveriptype.bottom
                    anchors.topMargin: 20
                    anchors.left: serveriptype.left
                    font.pixelSize:fontpixelSize
                    enabled: platform.checked
                }

                Control2.TextField {
                    id: textfield_serverip
                    anchors.top: serverip.top
                    anchors.left: serverip.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: platform.checked

                    placeholderText: serveriptypebox.currentIndex == 0? qsTr("0.0.0.0"):""
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                    }

                    validator: RegExpValidator{regExp:serveriptypebox.currentIndex == 0?/(?=(\b|\D))(((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))(?=(\b|\D))/:
                                                                                         /[a-zA-Z0-9][-a-zA-Z0-9]{0,62}(\.[a-zA-Z0-9][-a-zA-Z0-9]{0,62})+\.?/}
                }

                Text {
                    id: deviceport
                    width: boxwidth-100
                    text: qsTr("端口号")
                    anchors.top: serverip.bottom
                    anchors.topMargin: 20
                    anchors.left: serverip.left
                    font.pixelSize:fontpixelSize
                    enabled: platform.checked
                }

                Control2.TextField {
                    id: textfield_deviceport
                    anchors.top: deviceport.top
                    anchors.left: deviceport.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: platform.checked
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                    }

                    validator: RegExpValidator{regExp:/\d{1,9}/}
                }


            }

        }
    }

    Connections{
        target: setparent
        onSuresignal:{
//            pressed(tabView.currentIndex)
            console.log("platform set")
        }
        onCancelsignal:{

            console.log("cancel press")
        }
    }

}
