#ifndef PHOTO_H
#define PHOTO_H

#include <vector>

#include <QImage>
#include <QPrinter>
#include <QTextDocument>

class Photo
{
public:
    Photo();
    ~Photo();

    void addImage(const QImage &_image);
    void addImage(const std::vector<QImage> &_images);
    void saveImages(const std::string &_fileLocations);
    void save(const std::string &_path);
    void print();

    QString getFileName() const;
    void setFileName(const QString &value);

    void selectPrinter();

    float getPageWidth() const;
    void setPageWidth(float value);

    float getPageHeight() const;
    void setPageHeight(float value);

private:

    std::vector<QImage> images;
    QTextDocument document;
    QImage image;
    QString fileName;
    QPrinter printer;

    float pageWidth, pageHeight;

    void createDocument();
    void paintImage();
};

#endif // PHOTO_H
