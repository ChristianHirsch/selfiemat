#include <QApplication>
#include "mainwindow.h"

#include "common.h"
#include <unistd.h>

void processArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    processArgs(argc, argv);
    QApplication a(argc, argv);

    MainWindow mw;

#ifdef NO_FULLSCREEN
    mw.show();
#else
    mw.showFullScreen();
#endif

    return a.exec();
}

void processArgs(int argc, char *argv[])
{
    int opt;

    while((opt = getopt(argc, argv, "t:")) != -1)
    {
        switch(opt)
        {
            case 't':
                Common::loadSceneFromFile(optarg);
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
}
