#include "photo.h"

#include "common.h"

#include <string>

#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDateTime>
#include <QStandardPaths>

bool Photo::isPrinterInitialized = false;
QPrinter *Photo::printer = NULL;

Photo::Photo():
    sceneCreated(QDateTime::currentDateTime())
{
    pageHeight = 100.0f;
    pageWidth = 148.0f;

    if(!isPrinterInitialized)
        initializePrinter();
}

Photo::~Photo()
{

}

void Photo::setScene(Scene *_scene)
{
    sceneCreated = QDateTime::currentDateTime();
    scene = _scene;
}

void Photo::print()
{
    QString originalFileName = printer->outputFileName();

    if(printer->outputFormat() == QPrinter::PdfFormat)
    {
        QString fileName = originalFileName;

        if(fileName.endsWith(".pdf"), Qt::CaseInsensitive)
            fileName = fileName.split(".", QString::SkipEmptyParts).at(0);

        fileName += QDateTime::currentDateTime().toString("_yyyy-MM-dd_hh-mm-ss");
        fileName += ".pdf";

        printer->setOutputFileName(fileName);
    }
    createDocument();

    printer->setPageSizeMM(QSizeF(pageHeight, pageWidth));
    printer->setColorMode(QPrinter::Color);
    printer->setResolution(300);
    printer->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
//    printer->setFullPage(true);
    printer->setOrientation(QPrinter::Landscape);
    printer->setCopyCount(1);
    printer->setOutputFormat(QPrinter::NativeFormat);
//    printer->setOutputFileName(originalFileName);

    document.print(printer);

}

void Photo::selectPrinter()
{
    if(!printer)
        printer = new QPrinter();

    printf("Selecting printer...\n");
    QPrintDialog *dialog = new QPrintDialog(printer);
    dialog->setWindowTitle("Print Image");
    dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if(dialog->exec() != QDialog::Accepted)
        return;

    printf("Selected printer: %s\n", printer->printerName().toStdString().c_str());
//    printer->setOutputFormat(QPrinter::NativeFormat);

    isPrinterInitialized = true;
}

QString Photo::getAbsoluteStampedFileName(QString _ending) const
{
    return Common::getAbsoluteBaseFilePath()
            + sceneCreated.toString("_yyyy-MM-dd_hh-mm-ss")
            + _ending;
}

void Photo::initializePrinter()
{
    if(!printer)
        printer = new QPrinter();

    /*
    printer->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    printer->setFullPage(true);
    printer->setPageSizeMM(QSizeF(pageHeight, pageWidth));
    printer->setOrientation(QPrinter::Landscape);
    printer->setCopyCount(1);

    printer->setFullPage(true);

    isPrinterInitialized = false;
    */
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


QPrinter::PrinterState Photo::getPrinterState()
{
    if(!printer)
        return QPrinter::Error;
    return printer->printerState();
}

void Photo::createDocument()
{
    using namespace std;

    paintImage();

    document.clear();
    document.setIndentWidth(0);
    document.setDocumentMargin(0);
    document.setPageSize(QSize(printer->widthMM(), printer->heightMM()));

    string html = "<img width=" + to_string(printer->widthMM())
            + " height=" + to_string(printer->heightMM())
            + " src=\"" + getAbsoluteStampedFileName(".png").toStdString()
            + "\">";

    document.setHtml(html.c_str());
}

void Photo::paintImage()
{
    scene->paint();
    scene->save(getAbsoluteStampedFileName(".png"));
}
