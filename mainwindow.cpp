#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    filenamesModel(new QStringListModel(this)){
    ui->setupUi(this);
    ui->lvFiles->setModel(filenamesModel);
    ui->lvFiles->setUpdatesEnabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFiles(){
    addFile(QFileDialog::getOpenFileNames(
                this,tr("选择"), ".", tr("Picture (*.jpg;*.bmp;*.png)")));
}

void MainWindow::addFile(QStringList lst){
    QStringList nlst;
    for(auto iter : lst){
        if(filePaths.contains(iter)){continue;}
        nlst += iter.section("/", -1, -1).section("\\", -1, -1);
    }
    filePaths += lst;
    filenamesModel->setStringList(filenamesModel->stringList() + nlst);
    qDebug()<<filePaths<<endl;
}

void MainWindow::removeFiles(){
    auto idxs = ui->lvFiles->selectionModel()->selectedIndexes();
    for(auto idx : idxs){
        int i = idx.row();
        filePaths.removeAt(i);
        filenamesModel->removeRow(i);
    }
    qDebug()<<filePaths<<endl;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    auto url = event->mimeData()->data(QString("text/uri-list"));
    qDebug()<<url<<endl;
    if(event->source() != this && event->mimeData()->hasUrls()){
//        url.startsWith("file:///");
//        url.endsWith(".jpg");
    }else{
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event){

}
