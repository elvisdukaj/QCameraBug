#include "cannyfilter.hpp"
#include <opencv2/imgproc.hpp>
#include <QVector>
#include <iostream>

QVideoFilterRunnable* CannyFilter::createFilterRunnable()
{
    return new CannyVideoFilterRunnable();
}

cv::Mat convertVideoFrameToGrayMat(QVideoFrame* frame)
{
    auto width = frame->width();
    auto height = frame->height();
    auto data = frame->bits();

    cv::Mat rgbMat{height, width, CV_8UC1};

    switch(frame->pixelFormat()) {
    case QVideoFrame::Format_YUV420P:
    case QVideoFrame::Format_NV12:
    {
        auto convertEnum = frame->pixelFormat() == QVideoFrame::Format_YUV420P ?
                               cv::COLOR_YUV2GRAY_YV12 : cv::COLOR_YUV2GRAY_NV12;

        const cv::Mat tmp{height + height / 2, width, CV_8UC1, data};

        cv::cvtColor(tmp, rgbMat, convertEnum);
        break;
    }

    case QVideoFrame::Format_RGB24:
    {
        cv::Mat tmp{height, width, CV_8UC3, data};
        cv::cvtColor(tmp, rgbMat, cv::COLOR_RGB2GRAY);
        break;
    }

    case QVideoFrame::Format_RGB32:
    {
        cv::Mat tmp{height, width, CV_8UC4, data};
        cv::cvtColor(tmp, rgbMat, cv::COLOR_RGBA2GRAY);
        break;
    }

    default:
        throw std::invalid_argument{"Unknown video format"};
    }

    return rgbMat;
}

cv::Mat convertVideoFrameToRGBAMat(QVideoFrame* frame)
{
    auto width = frame->width();
    auto height = frame->height();
    auto data = frame->bits();

    cv::Mat rgbMat{height, width, CV_8UC4};

    switch(frame->pixelFormat()) {
    case QVideoFrame::Format_YUV420P:
    case QVideoFrame::Format_NV12:
    {
        auto convertEnum = frame->pixelFormat() == QVideoFrame::Format_YUV420P ?
                               cv::COLOR_YUV2RGBA_YV12 : cv::COLOR_YUV2RGBA_NV12;

        const cv::Mat tmp{height + height / 2, width, CV_8UC1, data};

        cv::cvtColor(tmp, rgbMat, convertEnum);
        break;
    }

    case QVideoFrame::Format_RGB24:
    {
        cv::Mat tmp{height, width, CV_8UC3, data};
        cv::cvtColor(tmp, rgbMat, cv::COLOR_RGB2RGBA);
        break;
    }

    case QVideoFrame::Format_RGB32:
    {
        cv::Mat tmp{height, width, CV_8UC4, data};
        tmp.copyTo(rgbMat);
        break;
    }

    default:
        throw std::invalid_argument{"Unknown video format"};
    }

    return rgbMat;
}

bool isVideoFrameHandable(const QVideoFrame* frame)
{
    auto handleType = frame->handleType();
    return handleType == QAbstractVideoBuffer::NoHandle;
}

bool isVideoFrameOpenGLHandled(const QVideoFrame* frame)
{
    auto handleType = frame->handleType();
    return handleType == QAbstractVideoBuffer::GLTextureHandle;
}

bool isFrameValid(const QVideoFrame* frame)
{
    auto isValid = frame->isValid();
    return isValid;
}

QImage qImageFromRGBAMat(const cv::Mat& grayMat)
{

    QImage result(grayMat.data, grayMat.cols, grayMat.rows, (int) grayMat.step, QImage::Format_RGB32);
    return result.copy();
}

QVideoFrame CannyVideoFilterRunnable::run(QVideoFrame* frame, const QVideoSurfaceFormat&, QVideoFilterRunnable::RunFlags)
{
    if (!isFrameValid(frame) || (!isVideoFrameHandable(frame) && !isVideoFrameOpenGLHandled(frame)))
    {
        std::cerr << "Invalid input format";
        return QVideoFrame();
    }

    frame->map(QAbstractVideoBuffer::ReadOnly);

    cv::Mat rgbaMat;
    auto grayMat = convertVideoFrameToGrayMat(frame);
    cv::Canny(grayMat, grayMat, 100, 200);
    cv::cvtColor(grayMat, rgbaMat, cv::COLOR_GRAY2RGBA);

    if (frame->handleType() == QAbstractVideoBuffer::NoHandle)
        frame->unmap();

    return QVideoFrame{qImageFromRGBAMat(rgbaMat)};
}
