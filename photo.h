#ifndef PHOTO_H
#define PHOTO_H

#include <vector>

#include <QImage>
#include <QTextDocument>

class Photo
{
public:
    Photo();
    ~Photo();

    void addImage(const QImage &_image);
    void addImage(const std::vector<QImage> &_images);
    void save(const std::string &_path);
    void print();

private:

    std::vector<QImage> images;
    QTextDocument document;
    QImage image;

    void createDocument();
    void paintImage();
};

#endif // PHOTO_H
