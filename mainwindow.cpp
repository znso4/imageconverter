#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    filenamesModel(new QStringListModel(this)),
    bgFormat(this){
    ui->setupUi(this);
    ui->lvFiles->setModel(filenamesModel);
    ui->lvFiles->setUpdatesEnabled(true);
    bgFormat.addButton(ui->rbJpg, SUPPORT_IMAGE_FORMAT::JPG);
    bgFormat.addButton(ui->rbPng, SUPPORT_IMAGE_FORMAT::PNG);
    bgFormat.addButton(ui->rbWebp,SUPPORT_IMAGE_FORMAT::WEBP);
    connect(&bgFormat, SIGNAL(buttonToggled(int, bool)), this, SLOT(onButtonGroupToggled(int, bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPbAddClicked(){
    addFile(QFileDialog::getOpenFileNames(
                this,tr("Choose"), ".", tr("Pictures") + " (*.jpg;*.png;*.webp)"));
}

void MainWindow::flushFilenamesInModel(){
    QStringList lst;
    for(auto iter:filePaths){
        lst.append(iter.section("/", -1, -1));
    }
    filenamesModel->setStringList(lst);
}

void MainWindow::addFile(const QStringList& lst){
    for(auto iter:lst){
        if(!filePaths.contains(iter)){
            filePaths.append(iter);
        }
    }
    flushFilenamesInModel();
    qDebug()<<filePaths<<endl;
}

void MainWindow::onPbDeleteClicked(){
    auto idxs = ui->lvFiles->selectionModel()->selectedIndexes();
    for(auto idx : idxs){
        int i = idx.row();
        filePaths.removeAt(i);
        filenamesModel->removeRow(i);
    }
    qDebug()<<filePaths<<endl;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if(event->source() == this || !event->mimeData()->hasUrls()){event->ignore();}
    else{
        QByteArray url = event->mimeData()->data(QString("text/uri-list"));
        QStringList paths = QUrl::fromPercentEncoding(url).split(newline, QString::SplitBehavior::SkipEmptyParts);
        for(auto str:paths){
            if(str.startsWith("file:///") &&
                    (str.endsWith(".jpg",Qt::CaseInsensitive) || str.endsWith(".png",Qt::CaseInsensitive) || str.endsWith(".webp",Qt::CaseInsensitive))){
                event->accept();
            }
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event){
    qDebug()<<"in dropEvent()"<<endl;
    QStringList paths = QUrl::fromPercentEncoding(event->mimeData()->data(QString("text/uri-list"))).split(newline, QString::SplitBehavior::SkipEmptyParts);
    for(int i = 0; i <paths.size(); i++){
        paths[i] = paths[i].remove(0, 8);
    }
    addFile(paths);
}

void MainWindow::onPbConvertClicked(){
    params.clear();
    switch (IMGFMT) {
    case JPG:params.push_back(cv::IMWRITE_JPEG_QUALITY);params.push_back(ui->spinBox->value());break;
    case PNG:params.push_back(cv::IMWRITE_PNG_COMPRESSION);params.push_back(ui->spinBox->maximum() - ui->spinBox->value());break;
    case WEBP:params.push_back(cv::IMWRITE_WEBP_QUALITY);params.push_back(ui->spinBox->value());break;
    }
    for(QString path:filePaths){
        cv::Mat img = cv::imread(path.toLocal8Bit().toStdString());
        int lastPointIndex = path.lastIndexOf('.');
		std::string newFilename = path.replace(
            lastPointIndex, path.size() - lastPointIndex, "-convert."+IMGFMT_TABLE[IMGFMT]).toLocal8Bit().toStdString();
        cv::imwrite(newFilename, img, params);
    }
    filePaths.clear();
    flushFilenamesInModel();
}

void MainWindow::onButtonGroupToggled(int id, bool checked){
    if(!checked)return;
    IMGFMT = static_cast<SUPPORT_IMAGE_FORMAT>(id);
    int min, max;
    switch (IMGFMT) {
    case JPG:min = 0; max = 100;break;
    case PNG:min = 0; max = 9;break;
    case WEBP:min = 1; max = 100;break;
    }
    ui->verticalSlider->setRange(min, max);
    ui->spinBox->setRange(min, max);
    ui->spinBox->setValue(min);
}
