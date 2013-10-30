// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "imagewidget.h"

#include <QtCore/QDebug>
#include <QPainter>
#include <QImage>
#include <QSizePolicy>
#include <QMimeData>
#include <QUrl>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
    , mWindowAspectRatio(0)
    , mImageAspectRatio(0)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(320, 240);
    setAcceptDrops(true);
}

const QImage &ImageWidget::image(void) const
{
    return mImage;
}

void ImageWidget::resizeEvent(QResizeEvent* e)
{
    mWindowAspectRatio = (qreal)e->size().width() / e->size().height();
}

void ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (mImage.isNull() || qFuzzyIsNull(mImageAspectRatio))
        return;
    if (mWindowAspectRatio < mImageAspectRatio) {
        const int h = int(width() / mImageAspectRatio);
        mDestRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * mImageAspectRatio);
        mDestRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(mDestRect, mImage);
}

void ImageWidget::setRaw(const QByteArray &raw)
{
    bool ok = mImage.loadFromData(raw, "JPG");
    if (!ok)
        return;
    mImage = mImage.convertToFormat(QImage::Format_ARGB32);
    mImageAspectRatio = (qreal)mImage.width() / mImage.height();
    update();
}

void ImageWidget::dragEnterEvent(QDragEnterEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls() && d->urls().first().toString().contains(QRegExp("\\.(png|jpg|gif|ico|mng|tga|tiff?)$")))
        e->acceptProposedAction();
    else
        e->ignore();
}


void ImageWidget::dragLeaveEvent(QDragLeaveEvent* e)
{
    e->accept();
}


void ImageWidget::dropEvent(QDropEvent* e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls()) {
        QString fileUrl = d->urls().first().toString();
        if (fileUrl.contains(QRegExp("file://.*\\.(png|jpg|jpeg|gif|ico|mng|tga|tiff?)$")))
#ifdef WIN32
            fileUrl.remove("file:///");
#else
            fileUrl.remove("file://");
#endif
        emit imageDropped(QImage(fileUrl));
    }
}
