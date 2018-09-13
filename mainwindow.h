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
#include <QButtonGroup>
#include <QMainWindow>

#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
private:
    Ui::MainWindow *ui;
    QStringListModel *filenamesModel;
    QStringList filePaths;
    QButtonGroup bgFormat;
    std::vector<int> params;
    const QStringList IMGFMT_TABLE = {"jpg", "png", "webp",};
enum SUPPORT_IMAGE_FORMAT{
        JPG = 0,
        PNG = 1,
        WEBP = 2,
    }IMGFMT = JPG;
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
    bool convert(const QString& path);
private slots:
    void onPbAddClicked();
    void onPbDeleteClicked();
    void onPbConvertClicked();
    void onButtonGroupToggled(int, bool);

};

#endif // MAINWINDOW_H
