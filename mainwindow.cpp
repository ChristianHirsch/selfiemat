#include "mainwindow.h"

#include "common.h"
#include "photo.h"

#include <stdio.h>

#include <QAction>
#include <QApplication>
#include <QContextMenuEvent>
#include <QDateTime>
#include <QDir>
#include <QGridLayout>
#include <QImage>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

#define TIME_PREVIEW 200

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
    idle("icons/press.png"),
    loading("icons/loading.png"),
    noCamera("icons/no_camera.png"),
    noPrinter("icons/no_printer.png"),
    wait("icons/wait.png"),
    smile("icons/smile.png"),
    printing("icons/printing.png"),
    countdown({QImage("icons/1.png"), QImage("icons/2.png"), QImage("icons/3.png")}),
    copyCount(1)
{
    setMinimumSize(480, 320);

    createStdLayout();

    copyCountLabel = new QLabel(QString::number(copyCount));
    createSimplePrintLayout();

    previewBtn = new QPushButton;
    previewBtn->setText("Preview");
    connect(previewBtn, SIGNAL(clicked()), this, SLOT(togglePreview()));
    previewBtn->setEnabled(false);

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    connect(&camInitTimer, SIGNAL(timeout()), this, SLOT(findAndInitCamera()));

    connect(&screen, SIGNAL(contextClicked(const QPoint &)), this, SLOT(openContextMenu(const QPoint &)));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    initActions();

    QDir workDir(Common::getWorkDirectoryPath());
    if(!workDir.mkpath(Common::getWorkDirectoryPath()))
        printf("Error creating working directory \"%s\"", workDir.absolutePath().toStdString().c_str());

    stateTimer.setSingleShot(true);
    connect(&stateTimer, SIGNAL(timeout()), this, SLOT(stateChange()));

    mainLayout.setSpacing(0);
    mainLayout.setContentsMargins(0,0,0,0);
    setLayout(&mainLayout);

    // initialize camera
    state = ERROR_CAMERA;
    stateChange();
}

MainWindow::~MainWindow()
{
    while(Common::getSceneCount() > 0)
        Common::scenePopBack();
}

void MainWindow::updatePreview()
{
    QImage image = eye.takePreview();
    if(image.isNull())
        showImage(loading);
    else
        screen.setPixmap(QPixmap::fromImage(image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio));
}

void MainWindow::togglePreview()
{
    if(previewTimer.isActive())
        stopPreview();
    else
        startPreview();
}

void MainWindow::findAndInitCamera()
{
    if(!eye.initCamera())
        return;

    camInitTimer.stop();

    state = IDLE;
    stateChange();
}

void MainWindow::takeScenePicture()
{
    QImage image = eye.takePicture();
    if(image.isNull() || !eye.isCameraInitialized())
    {
        state = ERROR_CAMERA;
        return stateChange();
    }

    screen.setPixmap(QPixmap::fromImage(image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio));
    Common::getScene()->addImage(image);

    if(Common::getScene()->imagesToAdd() <= 0)
        state = PRINT;
    else
        state = TAKE_PIC_PREVIEW;
}

void MainWindow::loadScene()
{
    Scene *newScene = Common::loadSceneFromFile();
    showImage(newScene->getPreviewImage());
}

void MainWindow::selectAndShowNextScene()
{
    Scene *scene = Common::getNextScene();
    showImage(scene->getPreviewImage());
    screen.drawSelectArrows();
}

void MainWindow::selectAndShowPrevScene()
{
    Scene *scene = Common::getPrevScene();
    showImage(scene->getPreviewImage());
    screen.drawSelectArrows();
}

void MainWindow::showImage(const QImage &_image)
{
    QPixmap pixmap = QPixmap::fromImage(_image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio);
    screen.setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    screen.setPixmap(pixmap);
}

void MainWindow::openContextMenu(const QPoint &_pos)
{
    QMenu menu(this);

    menu.addAction(loadSceneAction);
    menu.addAction(selectPrinterAction);
    menu.addAction(setWorkDirectoryPathAction);
    menu.addAction(setFileBaseNameAction);
    menu.addAction(quitApplicationAction);
    menu.exec(_pos);
}

void MainWindow::showPrintWidget()
{
    stdWidget.hide();
    printWidget.show();
}

void MainWindow::showStdWidget()
{
    printWidget.hide();
    stdWidget.show();
}

void MainWindow::createStdLayout()
{
    stdLayout.setSpacing(0);
    stdLayout.setContentsMargins(0,0,0,0);


    screen.setMinimumSize(240, 160);
    stdLayout.addWidget(&screen);

    stdWidget.setLayout(&stdLayout);
    mainLayout.addWidget(&stdWidget);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    stdWidget.setAutoFillBackground(true);
    stdWidget.setPalette(pal);

    stdWidget.show();
}

void MainWindow::createPrintLayout()
{
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *plusBtn = new QPushButton("+");
    QPushButton *minusBtn = new QPushButton("-");
    QPushButton *printBtn = new QPushButton("Print");

    plusBtn->setMinimumSize(50, 50);
    minusBtn->setMinimumSize(50, 50);
    printBtn->setMinimumHeight(50);
    
    btnLayout->setSpacing(0);
    btnLayout->setContentsMargins(0,0,0,0);

    printLayout.setSpacing(0);
    printLayout.setContentsMargins(0,0,0,0);

    printWidget.setStyleSheet(""
            "QLabel { font-size: 30pt; background-color: black; color: white; }"
            "QPushButton { font-size: 30pt; font-weight: bold; }"
            );
    plusBtn->setStyleSheet("QPushButton { background-color: green }");
    minusBtn->setStyleSheet("QPushButton { background-color: red }");

    btnLayout->addWidget(minusBtn, 0, Qt::AlignVCenter | Qt::AlignLeft);
    btnLayout->addWidget(copyCountLabel, 1, Qt::AlignVCenter | Qt::AlignHCenter);
    btnLayout->addWidget(plusBtn, 0, Qt::AlignVCenter | Qt::AlignLeft);

    printLayout.addLayout(btnLayout);
    printLayout.addWidget(printBtn, 0, Qt::AlignBottom | Qt::AlignHCenter);

    connect(plusBtn, SIGNAL(clicked()), this, SLOT(incCopyCount()));
    connect(minusBtn, SIGNAL(clicked()), this, SLOT(decCopyCount()));
    connect(printBtn, SIGNAL(clicked()), this, SLOT(stateChange()));

    printWidget.setLayout(&printLayout);
    printWidget.hide();

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    printWidget.setAutoFillBackground(true);
    printWidget.setPalette(pal);

    mainLayout.addWidget(&printWidget);
}

void MainWindow::createSimplePrintLayout()
{
    QGridLayout *btnLayout = new QGridLayout;
    QPushButton *zeroBtn = new QPushButton("Nein, danke!");
    QPushButton  *oneBtn = new QPushButton("1x");
    QPushButton  *twoBtn = new QPushButton("2x");
    QPushButton *fourBtn = new QPushButton("4x");
    QLabel *label = new QLabel("Ausdrucken?");

    zeroBtn->setMinimumSize(150, 150);
     oneBtn->setMinimumSize(150, 150);
     twoBtn->setMinimumSize(150, 150);
    fourBtn->setMinimumSize(150, 150);

    btnLayout->setSpacing(0);
    btnLayout->setContentsMargins(0,0,0,0);

    printLayout.setSpacing(0);
    printLayout.setContentsMargins(0,0,0,0);

    printWidget.setStyleSheet(""
            "QLabel { font-size: 30pt; background-color: black; color: white; }"
            "QPushButton { font-size: 30pt; font-weight: bold; }"
            );

    zeroBtn->setStyleSheet("QPushButton { background-color: red }");
     oneBtn->setStyleSheet("QPushButton { background-color: green }");
     twoBtn->setStyleSheet("QPushButton { background-color: green }");
    fourBtn->setStyleSheet("QPushButton { background-color: green }");

    btnLayout->addWidget(zeroBtn, 0, 0);
    btnLayout->addWidget( oneBtn, 0, 1);
    btnLayout->addWidget( twoBtn, 1, 0);
    btnLayout->addWidget(fourBtn, 1, 1);

    printLayout.addWidget(label, 0, Qt::AlignBottom | Qt::AlignHCenter);
    printLayout.addLayout(btnLayout);

    connect(zeroBtn, SIGNAL(clicked()), this, SLOT(setNoCopyAndChangeState()));
    connect( oneBtn, SIGNAL(clicked()), this, SLOT(setOneCopyAndChangeState()));
    connect( twoBtn, SIGNAL(clicked()), this, SLOT(setTwoCopiesAndChangeState()));
    connect(fourBtn, SIGNAL(clicked()), this, SLOT(setFourCopiesAndChangeState()));

    printWidget.setLayout(&printLayout);
    printWidget.hide();

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    printWidget.setAutoFillBackground(true);
    printWidget.setPalette(pal);

    mainLayout.addWidget(&printWidget);
}

void MainWindow::initActions()
{
    selectPrinterAction = new QAction(tr("Select &Printer"), this);
    connect(selectPrinterAction, &QAction::triggered, Photo::selectPrinter);

    setWorkDirectoryPathAction = new QAction(tr("Select &Working Directory"), this);
    connect(setWorkDirectoryPathAction, &QAction::triggered, Common::setWorkDirectoryPathWithDialog);

    setFileBaseNameAction = new QAction(tr("Select &File base name"), this);
    connect(setFileBaseNameAction, &QAction::triggered, Common::setFileBaseNameWithDialog);

    loadSceneAction = new QAction(tr("Load &Scene"), this);
    connect(loadSceneAction, SIGNAL(triggered(bool)), this, SLOT(loadScene()));

    quitApplicationAction = new QAction(tr("&Quit Application"), this);
    connect(quitApplicationAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
}

void MainWindow::startPreview()
{
    previewTimer.start(TIME_PREVIEW);
}

void MainWindow::stopPreview()
{
    previewTimer.stop();
}

void MainWindow::setCopyCount(int _cnt)
{
    copyCount = _cnt;
    copyCountLabel->setText(QString::number(copyCount));
}

void MainWindow::stateChange()
{
    static State prevState;
    static Photo photo;
    bool sceneStateSelected = false;

    static int cntdwn = 3;

    switch(state)
    {
        case IDLE:
            printf("idle state\n");

            showStdWidget();
            showImage(idle);
            Common::getScene()->clear();
            connect(&screen, SIGNAL(clickedCenter()), this, SLOT(stateChange()));

            //state = SELECT_SCENE;
            state = TAKE_PIC_PREVIEW;
            break;

        case SELECT_SCENE:
            printf("select scene state\n");
            sceneStateSelected = true;

            showImage(Common::getScene()->getPreviewImage());
            screen.drawSelectArrows();
            connect(&screen, SIGNAL(clickedRight()), this, SLOT(selectAndShowNextScene()));
            connect(&screen, SIGNAL(clickedLeft()), this, SLOT(selectAndShowPrevScene()));

            state = TAKE_PIC_PREVIEW;
            break;

        case TAKE_PREVIEW:
            break;

        case TAKE_PIC_PREVIEW:
            printf("take pic preview state [%d]\n", cntdwn);

            if(cntdwn == 3)
            {
                if(sceneStateSelected)
                {
                    disconnect(&screen, SIGNAL(clickedRight()), this, SLOT(selectAndShowNextScene()));
                    disconnect(&screen, SIGNAL(clickedLeft()), this, SLOT(selectAndShowPrevScene()));
                }
                disconnect(&screen, SIGNAL(clickedCenter()), this, SLOT(stateChange()));
            }

            cntdwn--;
            showImage(countdown[cntdwn]);

            if(cntdwn == 0)
            {
                state = SMILE;
                cntdwn = 3;
            }

            sceneStateSelected = false;

            //startPreview();
            stateTimer.start(1750);
            break;

        case SMILE:
            printf("smile state\n");
            stopPreview();

            showImage(smile);

            state = TAKE_PIC;
            stateTimer.start(1750);
            break;

        case TAKE_PIC:
            printf("take picture state\n");

            takeScenePicture();
            stateTimer.start(1750);
            break;

        case ERROR_CAMERA:
            printf("error camera state\n");
            showImage(noCamera);

            camInitTimer.start(1000);
            break;

        case ERROR_PRINTER:
            printf("error printer state\n");
            showImage(noPrinter);

            connect(&screen, SIGNAL(clickedCenter()), this, SLOT(stateChange()));
            state = prevState;
            break;

        case PRINT:
            printf("print state\n");

            disconnect(&screen, SIGNAL(clickedCenter()), this, SLOT(stateChange()));

            Common::getScene()->paint();
            showImage(Common::getScene()->getSceneImage());

            photo.setScene(Common::getScene());
            if(photo.getPrinterState() == QPrinter::Error)
            {
                prevState = PRINT;
                state = ERROR_PRINTER;
                stateChange();
            }

            state = SELECT_COPY_COUNT;
            state = IDLE;   // disable printing
            photo.print(0); // disable printing
            stateTimer.start(5000);
            break;

        case SELECT_COPY_COUNT:
            printf("select copy count state\n");

            setCopyCount(1);
            showPrintWidget();

            state = PRINTING;
            break;

        case PRINTING:
            QPrinter::PrinterState printerState = photo.getPrinterState();

            showStdWidget();
            showImage(printing);
            repaint();

            printf("printing state\n");
            printf("Copy count: %d\nPrinting...\n", copyCount);

            photo.print(0);
            for(int i=0; i < copyCount; i++)
            {
                photo.print(1);
            }
            printf("Printing finished.");

            int timer = 50000 * copyCount;
            if(copyCount == 0)
                timer = 0;

            printf("Printing %d samples. Wakeup in %d ms.\n", copyCount, timer);

            state = IDLE;
            stateTimer.start(timer);

            break;
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *_event)
{
    openContextMenu(_event->globalPos());
}

void MainWindow::setCopyCountAndChangeState(int _cnt)
{
    printf("setCopyCountAndChangeState() called\n");
    copyCount = _cnt;
    stateChange();
}

void MainWindow::setNoCopyAndChangeState(void)
{
    setCopyCountAndChangeState(0);
}

void MainWindow::setOneCopyAndChangeState(void)
{
    setCopyCountAndChangeState(1);
}

void MainWindow::setTwoCopiesAndChangeState(void)
{
    setCopyCountAndChangeState(2);
}

void MainWindow::setFourCopiesAndChangeState(void)
{
    setCopyCountAndChangeState(4);
}
    
void MainWindow::incCopyCount()
{
    setCopyCount(qMin(copyCount + 1, (int) 6));
}

void MainWindow::decCopyCount()
{
    setCopyCount(qMax(copyCount - 1, (int) 0));
}
