#include "photo.h"

#include <string>

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>

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

    //paintImage();
    createDocument();

    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Image");
    //dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dialog->exec() != QDialog::Accepted)
        return;

    printf("Printer name: %s\n", printer.printerName().toStdString().c_str());

    /*
    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    printer.setPageSizeMM(QSizeF(102.0,152.0));
    printer.setOrientation(QPrinter::Landscape);
    printer.setOrientation(QPrinter::Landscape);
    printer.setFullPage(true);

    //printer.setOutputFileName(_path.c_str());
    //printer.setOutputFormat(QPrinter::PdfFormat);
    */

    document.print(&printer);
}

void Photo::print()
{
    QPrinter printer;

    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    printer.setFullPage(true);
    printer.setPageSizeMM(QSizeF(102.0,152.0));
    printer.setOrientation(QPrinter::Landscape);
    printer.setCopyCount(1);

    /*
    printer.setOutputFileName(_path.c_str());
    printer.setOutputFormat(QPrinter::PdfFormat);
    */

    printer.setFullPage(true);
    document.print(&printer);
}

void Photo::createDocument()
{
    using namespace std;

    document.clear();
    document.setIndentWidth(0);
    document.setDocumentMargin(0);
    document.setPageSize(QSize(152.0, 102));

    string html = "<img width=152 height=102 src=\"/home/worker/img.png\">";
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
    QImage background("/home/worker/halloween_frame.png");

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

    img.save("/tmp/img.png");
}

