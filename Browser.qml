
import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtWebView 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.1


Window {
//    property bool showProgress: webView.loading
//                                && Qt.platform.os !== "ios"
//                                && Qt.platform.os !== "winphone"
//                                && Qt.platform.os !== "winrt"
    visible: true
    x: 0//initialX
    y: 0//initialY
    id: window
    width: 500 //initialWidth
    height: 400 //initialHeight
    //color:"red"
    //title: webView.title

//    Keys.onBackPressed: {
//        event.accepted = true
//    }

    Rectangle {
            focus: true

            Keys.onBackPressed: {
                event.accepted = true;
                window.closeButtonClicked();
                window.close();
//                closeButtonClicked();
//                close();
            }
        }

signal closeButtonClicked()



    Rectangle {
        id: rectangle
        color: "white"
        width: parent.width
        height: 100
        x: 0
        y: 0
        //anchors.fill: parent


//        Keys.onBackPressed: {
//            event.accepted = true;
//            //window.closeButtonClicked();
//            //window.close();
//        }

        Button {
            id: backButton
                text: "<- Go Back"
                onClicked:
                {
                    window.closeButtonClicked();
                    window.close();

                }
            }

        Label
        {
            id: lblProgress
            x: 300
            y: 35
            text: "Loading..."
        }
    }

    Rectangle {
        id: rectangle2
        color: "blue"
         width: parent.width
        height: parent.height - 100
        x: 0
        y: 100

//        Keys.onBackPressed: {
//            event.accepted = true
 //       }

        Label
        {
            id: lblTest
            x: 100
            text: "Please wait while your article is being loaded..."
        }
    }

    function setURL(url)
    {
        webView.url = url;
        //webView.reload();
        var yumyum = webView.loadProgress;
        webView.height = 0;
        rectangle2.height = height - 100;

        return "ok mang";
    }

    WebView {
        id: webView
        objectName: "webber"
        y: 100
        height: 0; //parent.height - 100
        width: parent.width
        onLoadProgressChanged:
        {
            lblProgress.text = qsTr("Loading: ") +
                    webView.loadProgress + "%";

            if (webView.loadProgress >= 50)
            {               
                webView.height = parent.height - 100;
                rectangle2.height = 0;

            }
            if (webView.loadProgress >= 100)
            {
                lblProgress.visible = false;
            }

        }


//        Keys.onBackPressed: {
//            event.accepted = true
//        }

       // url: initialUrl
    }
}
