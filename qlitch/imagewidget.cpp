// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "imagewidget.h"

#include <QtCore/QDebug>
#include <QPainter>
#include <QImage>
#include <QSizePolicy>
#include <QMimeData>
#include <QUrl>
#include <QMouseEvent>
#include <QPoint>
#include <QRect>


class ImageWidgetPrivate {
public:
    ImageWidgetPrivate(void)
        : windowAspectRatio(0)
        , imageAspectRatio(0)
        , mouseDown(false)
    { /* ... */ }
    ~ImageWidgetPrivate()
    { /* ... */ }
    QImage image;
    QRect destRect;
    qreal windowAspectRatio;
    qreal imageAspectRatio;
    bool mouseDown;
    QRect selectedArea;
};


ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new ImageWidgetPrivate)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(320, 240);
    setAcceptDrops(true);
    setFocus();
}


QImage ImageWidget::image(void)
{
    Q_D(ImageWidget);
    qDebug() << "ImageWidget::image()" << d->selectedArea;
    const QRect &selection = d->selectedArea.normalized();
    if (selection.isEmpty())
        return d->image;
    else {
        QRect rect = QRect(selection.topLeft() - d->destRect.topLeft(), selection.size());
        qDebug() << rect;
        return d->image.copy(rect);
    }
}


void ImageWidget::resizeEvent(QResizeEvent* e)
{
    d_ptr->windowAspectRatio = (qreal)e->size().width() / e->size().height();
}


void ImageWidget::paintEvent(QPaintEvent*)
{
    Q_D(ImageWidget);
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (d->image.isNull() || qFuzzyIsNull(d->imageAspectRatio))
        return;
    if (d->windowAspectRatio < d->imageAspectRatio) {
        const int h = int(width() / d->imageAspectRatio);
        d->destRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * d-> imageAspectRatio);
        d->destRect = QRect((width()-w)/2, 0, w, height());
    }
    p.drawImage(d->destRect, d->image);
    if (!d->selectedArea.isNull()) {
        p.setBrush(Qt::transparent);
        p.setPen(QPen(QBrush(QColor(255, 255, 255, 128)), 1, Qt::DashLine));
        p.drawRect(d->selectedArea.normalized());
    }
}


void ImageWidget::setRaw(const QByteArray &raw)
{
    Q_D(ImageWidget);
    bool ok = d->image.loadFromData(raw, "JPG");
    if (!ok) {
        emit refresh();
        return;
    }
    d->image = d->image.convertToFormat(QImage::Format_ARGB32);
    d->imageAspectRatio = (qreal)d->image.width() / d->image.height();
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
