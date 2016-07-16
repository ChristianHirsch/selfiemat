#ifndef PHOTO_H
#define PHOTO_H

#include "scene.h"

#include <vector>

#include <QDateTime>
#include <QImage>
#include <QPrinter>
#include <QTextDocument>

class Photo
{
public:
    Photo();
    ~Photo();

    void setScene(Scene *_scene);
    void save(const std::string &_path);
    void print();

    float getPageWidth() const;
    void setPageWidth(float value);

    float getPageHeight() const;
    void setPageHeight(float value);

    QPrinter::PrinterState getPrinterState();

public slots:
    static void selectPrinter();

private:

    Scene *scene;
    QTextDocument document;
    QImage image;
    QDateTime sceneCreated;

    static bool isPrinterInitialized;
    static QPrinter *printer;

    float pageWidth, pageHeight;

    QString getAbsoluteStampedFileName(QString _ending = "") const;
    void initializePrinter();
    void createDocument();
    void paintImage();
};

#endif // PHOTO_H
