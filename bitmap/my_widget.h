#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include <QList>
#include <QMainWindow>
#include <QtWidgets>

class Paint;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
   // void mouseReleaseEvent ( QMouseEvent * event );
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:

    void showToolBar();

    //file operations
    void openFile();
    void isSavedFile();

    //image settings
    void newImage();
    void bgrColor();

    //tools settings
    void colorPicker();
    void setWidth();
    void setPen();
    void setLine();
    void setEraser();
    void setRect();
    void setCircle();
    void undo();
    void redo();



private:
    Paint *paintArea;

    QToolBar *toolBar;
   // QMenu *shapesBar;
    QMenu *fileMenu;
    QMenu *saveMenu;
    QMenu *optionMenu;
    QMenu *toolsMenu;
    QAction *newImageAct;
    QAction *showToolBarAct;
    QAction *openAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *colorAct;
    QAction *widthAct;
    QAction *clearScreenAct;
    QAction *bgrColorAct;
    QAction *penAct;
    QAction *lineAct;
    QAction *rectAct;
    QAction *circleAct;
    QAction *eraserAct;


    QList<QAction *> saveAsActs;

    void createActions();
    void createMenus();

    bool isSaved();
    bool isSavedFile(const QByteArray &fileFormat);

    bool tbIsVisible;
};

#endif
