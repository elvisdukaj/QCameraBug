#pragma once

#include <QAbstractVideoFilter>

class CannyFilter : public QAbstractVideoFilter {
    Q_OBJECT

public:
    QVideoFilterRunnable* createFilterRunnable() override;
};

class CannyVideoFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame run(QVideoFrame* frame, const QVideoSurfaceFormat&, RunFlags) override;
};
