#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>

#include <QTextCodec>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QStringListModel>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void selectFiles();
    void removeFiles();
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    Ui::MainWindow *ui;
    QStringListModel *filenamesModel;
    QStringList filePaths;
#ifdef Q_OS_WIN
    QString newline = "\r\n";
#endif
#ifdef Q_OS_LINUX
    QString newline = "\n";
#endif
#ifdef Q_OS_UNIX
    QString newline = "\r";
#endif
    void addFile(const QStringList&);
    void flushFilenamesInModel();

};

#endif // MAINWINDOW_H
