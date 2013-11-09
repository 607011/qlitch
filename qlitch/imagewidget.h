// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __IMAGEWIDGET_H_
#define __IMAGEWIDGET_H_

#include <QWidget>
#include <QImage>
#include <QByteArray>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QScopedPointer>

class ImageWidgetPrivate;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = NULL);
    ~ImageWidget();
    QImage image(void);

public slots:
    void setRaw(const QByteArray&);
    void setBPos(int);
    void showHelp(bool);

signals:
    void imageDropped(const QImage&);
    void refresh(void);
    void positionChanged(int pos, int maxPos);

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    void calcDestRect(void);

private:
    QScopedPointer<ImageWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ImageWidget)
    Q_DISABLE_COPY(ImageWidget)

};

#endif // __IMAGEWIDGET_H_
