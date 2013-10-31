// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "imagewidget.h"

#include <QtCore/QDebug>
#include <QPainter>
#include <QImage>
#include <QSizePolicy>
#include <QMimeData>
#include <QUrl>


class ImageWidgetPrivate {
public:
    ImageWidgetPrivate(void)
        : mWindowAspectRatio(0)
        , mImageAspectRatio(0)
    { /* ... */ }
    ~ImageWidgetPrivate()
    { /* ... */ }
    QImage mImage;
    QRect mDestRect;
    qreal mWindowAspectRatio;
    qreal mImageAspectRatio;
};


ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new ImageWidgetPrivate)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(320, 240);
    setAcceptDrops(true);
}


const QImage &ImageWidget::image(void) const
{
    return d_ptr->mImage;
}


void ImageWidget::resizeEvent(QResizeEvent* e)
{
    d_ptr->mWindowAspectRatio = (qreal)e->size().width() / e->size().height();
}


void ImageWidget::paintEvent(QPaintEvent*)
{
    Q_D(ImageWidget);
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (d->mImage.isNull() || qFuzzyIsNull(d->mImageAspectRatio))
        return;
    if (d->mWindowAspectRatio < d->mImageAspectRatio) {
        const int h = int(width() / d->mImageAspectRatio);
        d->mDestRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() *d-> mImageAspectRatio);
        d->mDestRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(d->mDestRect, d->mImage);
}


void ImageWidget::setRaw(const QByteArray &raw)
{
    Q_D(ImageWidget);
    bool ok = d->mImage.loadFromData(raw, "JPG");
    if (!ok) {
        emit refresh();
        return;
    }
    d->mImage = d->mImage.convertToFormat(QImage::Format_ARGB32);
    d->mImageAspectRatio = (qreal)d->mImage.width() / d->mImage.height();
    update();
}


void ImageWidget::dragEnterEvent(QDragEnterEvent *e)
{
    const QMimeData* const d = e->mimeData();
    if (d->hasUrls() && d->urls().first().toString().contains(QRegExp("\\.(png|jpg|gif|ico|mng|tga|tiff?)$")))
        e->acceptProposedAction();
    else
        e->ignore();
}


void ImageWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    e->accept();
}


void ImageWidget::dropEvent(QDropEvent *e)
{
    const QMimeData *const d = e->mimeData();
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

void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        emit refresh();
}
