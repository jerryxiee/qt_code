import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls 2.2 as Control2
import "../../../Controls"

Item {
//    signal pressed(int setindex)
    property int fontpixelSize : 32
    property int boxwidth: 300

    TabView{
        id: tabView
        focus: true

        anchors.fill: parent

        Tab{
            title: "Normal"
            Item{
                property bool change: false
                ExclusiveGroup{
                    id:boxexcl
                }

                function checknet(str){
                    var strlist = str.split(".");

//                    console.log(strlist.length)
                    return strlist.length < 4 ? false:true
                }

                function checkmac(str){
                    var strlist = str.split(":")

                    return strlist.length < 6 ? false:true
                }

                MyCheckBox{
                    id:netauto
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.left: parent.left
                    anchors.leftMargin: 100
                    checked: true
                    exclusiveGroup:boxexcl
                    pixSize:fontpixelSize

                    str:qsTr("DHCP")
                    onCheckedChanged:change = true

                }

                MyCheckBox{
                    id:netmanual
                    anchors.top: netauto.top
                    anchors.left: netauto.right
                    anchors.leftMargin: 50
                    exclusiveGroup:boxexcl
                    pixSize:fontpixelSize



                    str:qsTr("手动设置")

                }

                Text {
                    id: textip
                    width: boxwidth-100
                    text: qsTr("IP地址")
                    anchors.top: netauto.bottom
                    anchors.topMargin: 20
                    anchors.left: netauto.left
                    font.pixelSize:fontpixelSize
                    enabled: netmanual.checked
                }

                Control2.TextField {
                    id: textfield_textip
                    anchors.top: textip.top
                    anchors.left: textip.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: netmanual.checked

                    placeholderText: qsTr("0.0.0.0")
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                        console.log("textfield_textip.x:"+textfield_textip.x+" textfield_textip.y"+textfield_textip.y)
                    }


                    validator: RegExpValidator{regExp:/(?=(\b|\D))(((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))(?=(\b|\D))/}
                }

                Text {
                    id: textmask
                    width: boxwidth-100
                    text: qsTr("子网掩码")
                    anchors.top: textip.bottom
                    anchors.topMargin: 20
                    anchors.left: textip.left
                    font.pixelSize:fontpixelSize
                    enabled: netmanual.checked
                }

                Control2.TextField {
                    id: textfield_textmask
                    anchors.top: textmask.top
                    anchors.left: textmask.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: netmanual.checked

                    placeholderText: qsTr("0.0.0.0")
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                        console.log("textfield_textmask.x:"+textfield_textmask.x+" textfield_textmask.y"+textfield_textmask.y)

                    }

                    validator: RegExpValidator{regExp:/^(254|252|248|240|224|192|128|0)\.0\.0\.0$|^(255\.(254|252|248|240|224|192|128|0)\.0\.0)$|^(255\.255\.(254|252|248|240|224|192|128|0)\.0)$|^(255\.255\.255\.(254|252|248|240|224|192|128|0))$/}
                }

                Text {
                    id: gatewayip
                    width: boxwidth-100
                    text: qsTr("默认网关")
                    anchors.top: textmask.bottom
                    anchors.topMargin: 20
                    anchors.left: textmask.left
                    font.pixelSize:fontpixelSize
                    enabled: netmanual.checked
                }

                Control2.TextField {
                    id: textfield_gatewayip
                    anchors.top: gatewayip.top
                    anchors.left: gatewayip.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: netmanual.checked

                    placeholderText: qsTr("0.0.0.0")
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                    }

                    validator: RegExpValidator{regExp:/(?=(\b|\D))(((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))(?=(\b|\D))/}
                }

                Text {
                    id: dnsip
                    width: boxwidth-100
                    text: qsTr("DNS服务器")
                    anchors.top: gatewayip.bottom
                    anchors.topMargin: 20
                    anchors.left: gatewayip.left
                    font.pixelSize:fontpixelSize
                    enabled: netmanual.checked
                }

                Control2.TextField {
                    id: textfield_dnsip
                    anchors.top: dnsip.top
                    anchors.left: dnsip.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    enabled: netmanual.checked

                    placeholderText: qsTr("0.0.0.0")
                    font.pixelSize: fontpixelSize
                    focus:true
                    onPressed: {
                        console.log("curpos "+cursorPosition)
                    }

                    validator: RegExpValidator{regExp:/(?=(\b|\D))(((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))\.){3}((\d{1,2})|(1\d{1,2})|(2[0-4]\d)|(25[0-5]))(?=(\b|\D))/}
                }

                Text {
                    id: macaddr
                    width: boxwidth-100
                    text: qsTr("MAC地址")
                    anchors.top: dnsip.bottom
                    anchors.topMargin: 20
                    anchors.left: dnsip.left
                    font.pixelSize:fontpixelSize
                    enabled: netmanual.checked
                }

                Control2.TextField {
                    id:textfield_mac

                    anchors.top: macaddr.top
                    anchors.left: macaddr.right
                    anchors.leftMargin: 50
                    width: boxwidth
                    text: ""
                    visible: true
                    font.pixelSize: fontpixelSize
                    enabled: netmanual.checked


                }

                Rectangle{
                    id:warning
                    height: 100
                    width: 200
                    visible: false
                    color: "lightblue"
                    border.color: "black"
                    border.width: 2

                    anchors.centerIn: parent

                    Button{
                        id:warningsure
                        height: 30
                        width: 50
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("sure")

                        onClicked: parent.visible = false
                    }

                    Text {
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        anchors.horizontalCenter: parent.horizontalCenter
//                        width: parent.width
//                        height:parent.height - warningsure.height
                        id: warntext
                        text: qsTr("warning")
                    }


                }

                Connections{
                    target: setparent
                    onSuresignal:{
                        if(change){
                            if(netauto.checked){
                                SystemConfig.dhcp = netauto.checked;
                            }else{
                                if(!checknet(textfield_textip.text)){
                                    warntext.text = "ip error"
                                    warning.visible = true
                                }else{
                                    if(!checknet(textfield_textmask.text)){
                                        warntext.text = "mask error"
                                        warning.visible = true
                                    }else{
                                        if(!checknet(textfield_gatewayip.text)){
                                            warntext.text = "gatewayip error"
                                            warning.visible = true
                                        }else{
                                            if(!checknet(textfield_dnsip.text)){
                                                warntext.text = "dnsip error"
                                                warning.visible = true
                                            }else{
                                                if(!checkmac(textfield_mac.text)){
                                                    warntext.text = "macaddr error"
                                                    warning.visible = true
                                                }else{
                                                    SystemConfig.setSystemNet(netauto.checked,textfield_textip.text,textfield_textmask.text,
                                                                              textfield_gatewayip.text,textfield_dnsip.text,textfield_mac.text)
                                                }


                                            }

                                        }
                                    }
                                }


                            }
                        }
            //            pressed(tabView.currentIndex)
                        console.log("net set")
                    }
                    onCancelsignal:{

                        console.log("cancel press")
                    }
                }





            }
        }

    }

//    Connections{
//        target: setparent
//        onSuresignal:{
////            pressed(tabView.currentIndex)
//            console.log("net set")
//        }
//        onCancelsignal:{

//            console.log("cancel press")
//        }
//    }


}
