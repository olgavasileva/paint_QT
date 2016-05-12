#ifndef PAINT_H
#define PAINT_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <vector>
#include <stack>

class Paint : public QWidget
{
    Q_OBJECT

public:
    Paint(QWidget *parent = 0);
    QColor clr() const { return myColor; }

    bool isModified() const { return modified; }
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);

    void paintUndo();
    void paintRedo();
    void setColor(const QColor &newColor);
    void setWidth(int newWidth);
    void setBGColor(const QColor &newColor);
    void setBlock();
    void unSetBlock();
    void unSetEraser();
    void setEraser();
    void unSetRect();
    void setRect();
    void setCircle();
    void unSetCircle();

    int paintWidth() const { return myWidth; }

public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QColor myColor;
    QImage image;
    QPoint lastPoint;
    QColor bgColor;
    std::stack<QImage> undoStack;
    std::stack<QImage> redoStack;
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void eraser(const QPoint &endPoint);
    void drawRect(const QPoint &endPoint);
    void drawEllipse(const QPoint &endPoint);
    int myWidth;
    int myEraserWidth;

    bool modified;
    bool painting;
    bool block;
    bool enableEraser;
    bool enableRect;
    bool enableCircle;

};

#endif // PAINT_H
