#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QPdfDocument;
class QPdfView;
class QSpinBox;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openPdf();
    void saveCopyAs();
    void nextPage();
    void prevPage();
    void pageSpinChanged(int oneBased);
    void zoomIn();
    void zoomOut();
    void fitWidth();
    void fitPage();

private:
    void setupUi();
    void updatePageUi();

    Ui::MainWindow *ui;
    QPdfDocument   *m_doc;
    QPdfView       *m_view;
    QString         m_currentFile;

    QSpinBox *m_pageSpin;
    QLabel   *m_pageLabel;
};









// #pragma once
// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

// class QPdfDocument;
// class QPdfView;
// class QSpinBox;
// class QLabel;

// class MainWindow : public QMainWindow {
//     Q_OBJECT
// public:
//     explicit MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private slots:
//     void openPdf();
//     void saveCopyAs();
//     void nextPage();
//     void prevPage();
//     void pageSpinChanged(int oneBased);
//     void zoomIn();
//     void zoomOut();
//     void fitWidth();
//     void fitPage();
//     void rotateLeft();
//     void rotateRight();

//     // qpdf-powered edits
//     void mergePdfs();
//     void extractRange();
//     void splitIntoSinglePages();

// private:
//     void setupUi();
//     void updatePageUi();
//     bool ensureQpdf(QString *exePathOut = nullptr) const;
//     bool runQpdf(const QStringList &args, QString *stderrOut = nullptr) const;

//     Ui::MainWindow *ui;
//     QPdfDocument   *m_doc;
//     QPdfView       *m_view;
//     QString         m_currentFile;

//     QSpinBox *m_pageSpin;
//     QLabel   *m_pageLabel;
// };




// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private:
//     Ui::MainWindow *ui;
// };
// #endif // MAINWINDOW_H
