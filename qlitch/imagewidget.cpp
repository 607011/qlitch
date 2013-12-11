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
#include <QMessageBox>

class ImageWidgetPrivate {
public:
    ImageWidgetPrivate(void)
        : windowAspectRatio(0)
        , imageAspectRatio(0)
        , mouseDown(false)
        , showHelp(true)
        , bPos(0)
        , maxPos(0)
    { /* ... */ }
    ~ImageWidgetPrivate()
    { /* ... */ }
    QImage image;
    QRect destRect;
    qreal windowAspectRatio;
    qreal imageAspectRatio;
    bool mouseDown;
    bool showHelp;
    int bPos;
    int maxPos;
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


ImageWidget::~ImageWidget()
{
    // ...
}


QImage ImageWidget::image(void)
{
    Q_D(ImageWidget);
    return d->image;
}


void ImageWidget::setBPos(int bPos)
{
    Q_D(ImageWidget);
    d->bPos = bPos;
    update();
}


void ImageWidget::resizeEvent(QResizeEvent* e)
{
    Q_D(ImageWidget);
    d->windowAspectRatio = qreal(e->size().width()) / e->size().height();
    calcDestRect();
}


void ImageWidget::paintEvent(QPaintEvent*)
{
    Q_D(ImageWidget);
    QPainter p(this);
    p.fillRect(rect(), Qt::black);
    if (d->image.isNull() || qFuzzyIsNull(d->imageAspectRatio))
        return;

    p.drawImage(d->destRect, d->image);
    if (d->showHelp) {
        const QRect &leftTopBoundingBox = QRect(5, 5, 200, 100);
        const QRect &leftTopTextBox = leftTopBoundingBox.marginsRemoved(QMargins(5, 5, 5, 5));
        const QRect &rightTopBoundingBox = QRect(width() - 205, 5, 200, 100);
        const QRect &rightTopTextBox = rightTopBoundingBox.marginsRemoved(QMargins(5, 5, 5, 5));

        p.setBrush(QColor(0, 0, 0, 128));
        p.setPen(Qt::transparent);
        p.drawRect(leftTopBoundingBox);
        p.setBrush(Qt::transparent);
        p.setPen(Qt::white);
        p.drawText(leftTopTextBox, tr("Click and move cursor to select glitch position. Turn mouse wheel to randomize."));

        p.setBrush(QColor(0, 0, 0, 128));
        p.setPen(Qt::transparent);
        p.drawRect(rightTopBoundingBox);
        p.setBrush(Qt::transparent);
        p.setPen(Qt::white);
        p.drawText(rightTopTextBox, tr("Position: %1%").arg(1e2 * d->bPos / d->maxPos, 6, 'g', 3), QTextOption(Qt::AlignRight));
    }
}


void ImageWidget::setRaw(const QByteArray &raw)
{
    Q_D(ImageWidget);
    bool ok = d->image.loadFromData(raw, "JPG");
    if (ok) {
        d->image = d->image.convertToFormat(QImage::Format_ARGB32);
        d->imageAspectRatio = qreal(d->image.width()) / d->image.height();
        calcDestRect();
        update();
    }
    else {
        emit invalidJPGData();
    }
}


void ImageWidget::showHelp(bool enabled)
{
    Q_D(ImageWidget);
    d->showHelp = enabled;
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
#if defined(_WIN32) || defined(_WIN64)
            fileUrl.remove("file:///");
#else
            fileUrl.remove("file://");
#endif
        emit imageDropped(QImage(fileUrl));
    }
}


void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    Q_D(ImageWidget);
    if (e->button() == Qt::LeftButton) {
        d->mouseDown = true;
        update();
    }
    e->accept();
}


template <typename T>
inline T clamp(T x, T a, T b) {
    return (x < a) ? a : (x > b) ? b : x;
}


void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    Q_D(ImageWidget);
    if (d->mouseDown) {
        QPoint p = e->pos() - d->destRect.topLeft();
        p.setX(clamp(p.x(), 0, d->destRect.width()));
        p.setY(clamp(p.y(), 0, d->destRect.height()));
        d->bPos = p.x() + p.y() * d->destRect.width();
        emit positionChanged(d->bPos, d->maxPos);
        update();
    }
}


void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_D(ImageWidget);
    if (e->button() == Qt::LeftButton) {
        d->mouseDown = false;
        update();
    }
}


void ImageWidget::wheelEvent(QWheelEvent *)
{
    emit refresh();
}


void ImageWidget::calcDestRect(void)
{
    Q_D(ImageWidget);
    if (d->windowAspectRatio < d->imageAspectRatio) {
        const int h = int(width() / d->imageAspectRatio);
        d->destRect = QRect(0, (height()-h)/2, width(), h);
    }
    else {
        const int w = int(height() * d-> imageAspectRatio);
        d->destRect = QRect((width()-w)/2, 0, w, height());
    }
    d->maxPos = d->destRect.width() * d->destRect.height();
}
