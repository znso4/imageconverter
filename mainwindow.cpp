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
                this,tr("选择"), ".", tr("Picture") + " (*.jpg;*.bmp;*.png)"));
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
    if(event->source() == this || !event->mimeData()->hasUrls()){event->ignore();}
    else{
        QByteArray url = event->mimeData()->data(QString("text/uri-list"));
        QStringList paths = QUrl::fromPercentEncoding(url).split(newline, QString::SplitBehavior::SkipEmptyParts);
        for(auto str:paths){
            if(str.startsWith("file:///") &&
                    (str.endsWith(".jpg",Qt::CaseInsensitive) || str.endsWith(".png",Qt::CaseInsensitive) || str.endsWith(".gif",Qt::CaseInsensitive))){
                event->accept();
            }
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event){
    qDebug()<<"in dropEvent()"<<endl;
    QStringList paths = QUrl::fromPercentEncoding(event->mimeData()->data(QString("text/uri-list"))).split(newline, QString::SplitBehavior::SkipEmptyParts);
    QStringList fn;
    for(auto str:paths){
        fn.append(str.remove(0, 8));
    }
    addFile(fn);
}
