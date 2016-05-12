#include <QtWidgets>
#include <math.h>

#include "paint.h"

Paint::Paint(QWidget *parent)
    : QWidget(parent) {

    setAttribute(Qt::WA_StaticContents);
    modified = false;
    painting = false;

    myWidth = 1;
    myColor = Qt::black;
    bgColor = Qt::white;

    block = true;
    myEraserWidth = 15;
    enableEraser = false;
    enableRect = false;
    enableCircle = false;
}

bool Paint::openImage(const QString &fileName) {
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool Paint::saveImage(const QString &fileName, const char *fileFormat) {
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void Paint::setColor(const QColor &newColor) {
    myColor = newColor;
}

void Paint::setBGColor(const QColor &newColor) {
    image.fill(newColor);
    bgColor = newColor;
}

void Paint::setWidth(int newWidth) {
    myWidth = newWidth;
}

void Paint::setBlock() {
    block = true;
}

void Paint::unSetBlock() {
    block = false;
}

void Paint::unSetEraser() {
       enableEraser = false;
}

void Paint::setEraser() {
       enableEraser = true;
}

void Paint::unSetCircle() {
       enableCircle = false;
}

void Paint::setCircle() {
       enableCircle = true;
}

void Paint::unSetRect() {
       enableRect = false;
}

void Paint::setRect() {
       enableRect = true;
}

void Paint::clearImage() {
    image.fill(bgColor);
    modified = true;
    undoStack.push(image);
    update();
}

void Paint::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        painting = true;
    }
}

void Paint::mouseMoveEvent(QMouseEvent *event) {
    if(block || enableEraser){
     if ((event->buttons() & Qt::LeftButton) && painting){
         if(block)
         {
          drawLineTo(event->pos());
         }else{
          eraser(event->pos());
         }
     }
    }
}

void Paint::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && painting) {
        if(enableRect)
        {
          drawRect(event->pos());
        }
        else if(enableCircle)
        {
          drawEllipse(event->pos());
        }
        else{
          drawLineTo(event->pos());
        }
        painting = false;
    }
    undoStack.push(image);
}

void Paint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect r = event->rect();
    painter.drawImage(r, image, r);
}

void Paint::paintUndo() {
    if(!undoStack.empty()){
        redoStack.push(undoStack.top());
        undoStack.pop();
    }

    if(!undoStack.empty()){
        image = undoStack.top();
    }else{
        clearImage();
    }

    update();
}

void Paint::paintRedo() {

    if(!redoStack.empty()){
        image = redoStack.top();
    }

    if(!redoStack.empty()){
        undoStack.push(redoStack.top());
        redoStack.pop();
    }

    update();
}

void Paint::resizeEvent(QResizeEvent *event) {
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void Paint::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(myColor, myWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void Paint::drawRect(const QPoint &endPoint) {
    int width = endPoint.x() - lastPoint.x();
    int height = endPoint.y() - lastPoint.y();
    QPainter painter(&image);
    painter.setPen(QPen(myColor, myWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawRect(lastPoint.x(), lastPoint.y(), width, height);
    modified = true;

    int rad = (myWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void Paint::drawEllipse(const QPoint &endPoint) {
    int width = endPoint.x() - lastPoint.x();
    int height = endPoint.y() - lastPoint.y();
    QPainter painter(&image);
    painter.setPen(QPen(myColor, myWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawEllipse(lastPoint.x(), lastPoint.y(), width, height);
    modified = true;

    int rad = (myWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}




void Paint::eraser(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setPen(QPen(bgColor, myEraserWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myEraserWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void Paint::resizeImage(QImage *image, const QSize &newSize) {
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(bgColor);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
