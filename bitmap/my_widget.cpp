#include <iostream>
using namespace std;

#include <qfiledialog.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <QMenuBar>
#include <QToolBar>

#include "my_widget.h"
#include "paint.h"


MainWindow::MainWindow() {
    paintArea = new Paint;
    setCentralWidget(paintArea);

    tbIsVisible = true;

    createActions();
    createMenus();

    setWindowTitle(tr("OV Paint"));
    resize(900, 500);
}


void MainWindow::createActions() {

    QPixmap colorIcon(":icons/color.png");
    QPixmap widthIcon(":icons/width.png");
    QPixmap bgIcon(":icons/bg.png");
    QPixmap penIcon(":icons/pen.png");
    QPixmap lineIcon(":icons/line.png");
    QPixmap eraserIcon(":icons/eraser.png");
    QPixmap undoIcon(":icons/undo.png");
    QPixmap redoIcon(":icons/redo.png");
    QPixmap clearIcon(":icons/clear.png");
    QPixmap squareIcon(":icons/square.png");
    QPixmap circleIcon(":icons/circle.png");

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));

    newImageAct = new QAction(tr("&New Image"), this);
    connect(newImageAct, SIGNAL(triggered()), this, SLOT(newImage()));

    QImageWriter writer;
    writer.setFormat("bmp");
    QByteArray format = writer.format();
    QAction *action = new QAction("Save", this);
    action->setData(format);
    connect(action, SIGNAL(triggered()), this, SLOT(isSavedFile()));
    saveAsActs.append(action);

    widthAct = new QAction(tr("Line Width"), this);
    widthAct->setIcon(QIcon(widthIcon));
    connect(widthAct, SIGNAL(triggered()), this, SLOT(setWidth()));

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    colorAct = new QAction(tr("Line Color"), this);
    colorAct->setIcon(QIcon(colorIcon));
    connect(colorAct, SIGNAL(triggered()), this, SLOT(colorPicker()));

    bgrColorAct = new QAction(tr("&BG Color"), this);
    bgrColorAct->setIcon(QIcon(bgIcon));
    connect(bgrColorAct, SIGNAL(triggered()), this, SLOT(bgrColor()));

    penAct = new QAction(tr("Pen"), this);
    penAct->setIcon(QIcon(penIcon));
    connect(penAct, SIGNAL(triggered()), this, SLOT(setPen()));

    lineAct = new QAction(tr("Line"), this);
    lineAct->setIcon(QIcon(lineIcon));
    connect(lineAct, SIGNAL(triggered()), this, SLOT(setLine()));

    eraserAct = new QAction(tr("Eraser"), this);
    eraserAct->setIcon(QIcon(eraserIcon));
    connect(eraserAct, SIGNAL(triggered()), this, SLOT(setEraser()));

    showToolBarAct = new QAction(tr("Show ToolBar"), this);
    connect(showToolBarAct, SIGNAL(triggered()), this, SLOT(showToolBar()));

    undoAct = new QAction(tr("Undo"), this);
    undoAct->setIcon(QIcon(undoIcon));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("Redo"), this);
    redoAct->setIcon(QIcon(redoIcon));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    circleAct = new QAction(tr("Circle"), this);
    circleAct->setIcon(QIcon(circleIcon));
    connect(circleAct, SIGNAL(triggered()), this, SLOT(setCircle()));

    rectAct =  new QAction(tr("Square"), this);
    rectAct->setIcon(QIcon(squareIcon));
    connect(rectAct, SIGNAL(triggered()), this, SLOT(setRect()));


    clearScreenAct = new QAction(tr("&Clean"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    clearScreenAct->setIcon(QIcon(clearIcon));
    connect(clearScreenAct, SIGNAL(triggered()),
            paintArea, SLOT(clearImage()));
}

void MainWindow::createMenus() {

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(newImageAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsActs.first());
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Edit"), this);
    optionMenu->addAction(undoAct);
    optionMenu->addAction(redoAct);
    toolsMenu = optionMenu->addMenu("Tools");
    optionMenu->addSeparator();
    optionMenu->addAction(showToolBarAct);

    toolsMenu->addAction(bgrColorAct);
    toolsMenu->addAction(colorAct);
    toolsMenu->addAction(widthAct);
    toolsMenu->addAction(penAct);
    toolsMenu->addAction(lineAct);
    toolsMenu->addAction(eraserAct);
    toolsMenu->addAction(circleAct);
    toolsMenu->addAction(rectAct);

    toolsMenu->addSeparator();
    toolsMenu->addAction(clearScreenAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);



    toolBar = addToolBar("Toolbar");
    toolBar->setMovable(false);
   // toolBar->addAction(shapesBar->menuAction());
   // shapesBar = toolBar->addMenu("Shapes");
   // shapesBar->addAction(circleAct);
    toolBar->addAction(undoAct);
    toolBar->addAction(redoAct);
    toolBar->addAction(bgrColorAct);
    toolBar->addAction(colorAct);
    toolBar->addAction(eraserAct);
    toolBar->addAction(penAct);
    toolBar->addAction(lineAct);
    toolBar->addAction(circleAct);
    toolBar->addAction(rectAct);
    toolBar->addAction(clearScreenAct);
}


/*void MainWindow::mouseReleaseEvent ( QMouseEvent * event )
    {
      if(event->button() == Qt::RightButton)
      {
       QMessageBox * msgBox = new QMessageBox();
        msgBox->setWindowTitle("Hello");
        msgBox->setText("Right click");
        msgBox->show();
      }

    }*/


void MainWindow::closeEvent(QCloseEvent *event) {
    if (isSaved()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::openFile() {
    if (isSaved()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            paintArea->openImage(fileName);
    }
}

void MainWindow::isSavedFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    isSavedFile(fileFormat);
}


bool MainWindow::isSavedFile(const QByteArray &fileFormat) {
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    } else {
        return paintArea->saveImage(fileName, fileFormat.constData());
    }
}


void MainWindow::newImage() {
  if (isSaved()) {
      paintArea->clearImage();
  }
}


void MainWindow::bgrColor() {
    QColor newColor = QColorDialog::getColor();
    if (newColor.isValid())
        paintArea->setBGColor(newColor);
}

void MainWindow::colorPicker() {
    QColor newColor = QColorDialog::getColor(paintArea->clr());
    if (newColor.isValid())
        paintArea->setColor(newColor);
}

void MainWindow::setPen() {
   paintArea->setBlock();
   paintArea->unSetEraser();
   paintArea->unSetRect();
   paintArea->unSetCircle();
}

void MainWindow::setLine() {
   paintArea->unSetBlock();
   paintArea->unSetEraser();
   paintArea->unSetRect();
   paintArea->unSetCircle();
}

void MainWindow::setEraser() {
   paintArea->setEraser();
   paintArea->unSetBlock();
   paintArea->unSetRect();
   paintArea->unSetCircle();
}

void MainWindow::setRect() {
   paintArea->setRect();
   paintArea->unSetBlock();
   paintArea->unSetEraser();
   paintArea->unSetCircle();
}

void MainWindow::setCircle(){
    paintArea->setCircle();
    paintArea->unSetRect();
    paintArea->unSetBlock();
    paintArea->unSetEraser();
 }


void MainWindow::undo() {
   paintArea->paintUndo();
}

void MainWindow::redo() {
   paintArea->paintRedo();
}

void MainWindow::setWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Paint"),
                                        tr("Select width:"),
                                        paintArea->paintWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        paintArea->setWidth(newWidth);
}

void MainWindow::showToolBar() {
    if(tbIsVisible) {
       toolBar->setVisible(false);
       tbIsVisible = false;
    } else {
       toolBar->setVisible(true);
       tbIsVisible = true;
    }
}

bool MainWindow::isSaved() {
    if (paintArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Paint"), tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return isSavedFile("bmp");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

