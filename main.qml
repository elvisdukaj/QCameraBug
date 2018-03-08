import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtMultimedia 5.5
import com.qubicaamf.vision 1.0

ApplicationWindow {
    id: mainWindow

    visible: true
    width: 800
    height: 600
    title: qsTr("Markerless Example")

    property bool isCaptured: false

    Camera {
        id: camera

        captureMode: Camera.CaptureStillImage

        imageCapture {
            onImageCaptured: {
                photoPreview.source = preview;
                photoPreview.visible = true;
            }
        }
    }

    CannyFilter {
        id: grayFilter
    }

    VideoOutput {
        id: videoOutput
        source: camera

        anchors.fill: parent

        focus : visible // to receive focus and capture key events when visible
        filters: [grayFilter]
    }

    Image {
        id: photoPreview
        visible: false
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if (!parent.isCaptured)
                camera.imageCapture.capture()
            else
                photoPreview.visible = false
            parent.isCaptured = !parent.isCaptured;
        }
    }
}
