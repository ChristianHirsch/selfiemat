#include "photo.h"

#include <string>
#include <QPrinter>
#include <QTextDocument>

Photo::Photo()
{

}

Photo::~Photo()
{

}

void Photo::addImage(const QImage &_image)
{
    images.push_back(_image);
}

void Photo::addImage(const std::vector<QImage> &_images)
{
    images.insert(images.end(), _images.begin(), _images.end());
}

void Photo::save(const std::string &_path)
{
    using namespace std;

    QTextDocument document;

    string html =  "<table width=\"100%\"><tr>";

    int counter = 0;
    for(const QImage &image: images)
    {
        if(counter%2 == 0)
            html += "</tr><tr>";

        string fileName = "/tmp/photo_" + to_string(counter) + ".png";
        image.save(fileName.c_str());

        html += "<td><img width=\"450\" src=\""
                + fileName
                + "\"></td>";

        counter++;
    }
    html += "</tr></table>";

    document.setHtml(html.c_str());

    QPrinter printer;
    printer.setOutputFileName(_path.c_str());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(true);
    document.print(&printer);
}

void Photo::print()
{

}

