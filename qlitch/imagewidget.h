// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QByteArray>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QImage>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = 0);

    const QImage& image(void) const;

signals:
    void imageDropped(const QImage&);
    void refresh(void);

private:
    QImage mImage;
    QRect mDestRect;
    qreal mWindowAspectRatio;
    qreal mImageAspectRatio;

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);
    void mousePressEvent(QMouseEvent*);

public slots:
    void setRaw(const QByteArray&);
};

#endif // IMAGEWIDGET_H
