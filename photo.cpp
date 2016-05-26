#include "photo.h"

#include <string>

#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDateTime>

Photo::Photo()
{
    pageHeight = 100.0f;
    pageWidth = 148.0f;

    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    printer.setFullPage(true);
    printer.setPageSizeMM(QSizeF(pageHeight, pageWidth));
    printer.setOrientation(QPrinter::Landscape);
    printer.setCopyCount(1);

    printer.setFullPage(true);
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

void Photo::saveImages(const std::string &_fileLocations)
{
    QDir dir(QString::fromStdString(_fileLocations));
    if(dir.exists(QString::fromStdString(_fileLocations)))
        dir.mkdir(QString::fromStdString(_fileLocations));

    int i=0;
    for(const QImage &img : images )
    {
        img.save(getFileName() + "/" + i + ".png");
        i++;
    }
}

void Photo::save(const std::string &_path)
{
    using namespace std;

    paintImage();
    createDocument();

    QPrinter printer;

    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    printer.setPageSizeMM(QSizeF(pageHeight, pageWidth));
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(true);

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(_path.c_str());

    document.print(&printer);
}

void Photo::print()
{
    document.print(&printer);
}

QString Photo::getFileName() const
{
    return fileName;
}

void Photo::setFileName(const QString &value)
{
    fileName = value;
}

void Photo::selectPrinter()
{
    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Image");
    dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dialog->exec() != QDialog::Accepted)
        return;

    printf("Selected printer: %s\n", printer.printerName().toStdString().c_str());
}

float Photo::getPageWidth() const
{
    return pageWidth;
}

void Photo::setPageWidth(float value)
{
    pageWidth = value;
}

float Photo::getPageHeight() const
{
    return pageHeight;
}

void Photo::setPageHeight(float value)
{
    pageHeight = value;
}

void Photo::createDocument()
{
    using namespace std;

    document.clear();
    document.setIndentWidth(0);
    document.setDocumentMargin(0);
    document.setPageSize(QSize(pageWidth, pageHeight));

    string html = "<img width=" + to_string(pageWidth)
            + " height=" + to_string(pageHeight)
            + " src=\"" + getFileName().toStdString() + ".png\">";
    /*
    string html =  "<table width=\"100%\"><tr>";
    int counter = 0;
    for(const QImage &image: images)
    {
        if(counter%2 == 0)
            html += "</tr><tr>";

        string fileName = "/tmp/photo_" + to_string(counter) + ".png";
        image.save(fileName.c_str());

        html += "<td>"
                "<img width=\"450\" src=\"" + fileName + "\">"
                "</td>";

        counter++;
    }
    html += "</tr></table>";
    */

    /*
    html += "<img style=\"position: absolute; top: 0px; left: 0px\""
            "src=\"/home/worker/frame.png\">";
            */

    document.setHtml(html.c_str());
}

void Photo::paintImage()
{
    float scale = (float)2048 / images[0].width();

    QImage img(2048, round(scale * (float)images[1].height()),
            QImage::Format_ARGB32);
    QImage background("/home/foreman/Downloads/hochzeit_rahmen.png");

    QPainter painter;
    painter.begin(&img);
    painter.drawImage(QRectF(0.0, 0.0, img.width()/2, img.height()/2),
                      images[0]);
    painter.drawImage(QRectF(img.width()/2, 0.0, img.width()/2, img.height()/2),
                      images[1]);
    painter.drawImage(QRectF(0.0, img.height()/2, img.width()/2, img.height()/2),
                      images[2]);
    painter.drawImage(QRectF(img.width()/2, img.height()/2, img.width()/2, img.height()/2),
                      images[3]);
    painter.drawImage(img.rect(), background);
    painter.end();

    img.save(getFileName() + ".png");
}

