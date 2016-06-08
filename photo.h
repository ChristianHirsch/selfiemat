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

    void setScene(const Scene &_scene);
    void saveImages(const std::string &_fileLocations);
    void save(const std::string &_path);
    void print();

    QString getFileBaseName() const;
    void setFileBaseName(const QString &value);

    float getPageWidth() const;
    void setPageWidth(float value);

    float getPageHeight() const;
    void setPageHeight(float value);

public slots:
    static void selectPrinter();

private:

    Scene scene;
    QTextDocument document;
    QImage image;
    QString fileBaseName;
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
