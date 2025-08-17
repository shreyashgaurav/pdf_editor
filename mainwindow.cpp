#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtPdf/QPdfDocument>
#include <QtPdfWidgets/QPdfView>
#include <QtPdf/QPdfPageNavigator>

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QMessageBox>
#include <QPointF>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_doc(new QPdfDocument(this)), m_view(new QPdfView(this)),
    m_pageSpin(nullptr), m_pageLabel(nullptr)
{
    ui->setupUi(this);
    setupUi();

    m_view->setDocument(m_doc);
    m_view->setZoomMode(QPdfView::ZoomMode::FitToWidth);
    m_view->setPageMode(QPdfView::PageMode::SinglePage);

    setWindowTitle("PDFEditor");
    resize(1100, 780);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupUi() {
    auto *layout = new QVBoxLayout(ui->centralwidget);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_view);

    auto *tb = addToolBar("Main");
    tb->addAction("Open", this, &MainWindow::openPdf);
    tb->addAction("Save Copy", this, &MainWindow::saveCopyAs);
    tb->addSeparator();

    tb->addAction("Prev", this, &MainWindow::prevPage);
    tb->addAction("Next", this, &MainWindow::nextPage);

    m_pageSpin = new QSpinBox(this);
    m_pageSpin->setMinimum(1);
    m_pageSpin->setMaximum(1);
    m_pageSpin->setEnabled(false);
    connect(m_pageSpin, &QSpinBox::valueChanged, this, &MainWindow::pageSpinChanged);
    tb->addWidget(m_pageSpin);

    m_pageLabel = new QLabel("/ 0", this);
    tb->addWidget(m_pageLabel);

    tb->addSeparator();
    tb->addAction("Zoom +", this, &MainWindow::zoomIn);
    tb->addAction("Zoom −", this, &MainWindow::zoomOut);
    tb->addAction("Fit Width", this, &MainWindow::fitWidth);
    tb->addAction("Fit Page", this, &MainWindow::fitPage);
}

void MainWindow::openPdf() {
    const QString fn = QFileDialog::getOpenFileName(this, "Open PDF", {}, "PDF Files (*.pdf)");
    if (fn.isEmpty()) return;

    auto err = m_doc->load(fn);
    // In newer Qt, compare to QPdfDocument::Error::None; in older, QPdfDocument::NoError.
#if (QT_VERSION >= QT_VERSION_CHECK(6, 6, 0))
    if (err != QPdfDocument::Error::None) {
#else
    if (err != QPdfDocument::NoError) {
#endif
        QMessageBox::warning(this, "Open PDF", "Failed to open PDF.");
        return;
    }
    m_currentFile = fn;

    if (auto nav = m_view->pageNavigator())
        nav->jump(0, QPointF(0, 0), 0);   // first page

    updatePageUi();
    setWindowTitle(QString("PDFEditor — %1").arg(QFileInfo(fn).fileName()));
}

void MainWindow::saveCopyAs() {
    if (m_currentFile.isEmpty()) {
        QMessageBox::information(this, "Save Copy", "Open a PDF first.");
        return;
    }
    const QString out = QFileDialog::getSaveFileName(this, "Save Copy As", {}, "PDF Files (*.pdf)");
    if (out.isEmpty()) return;
    if (!QFile::copy(m_currentFile, out)) {
        QMessageBox::warning(this, "Save Copy", "Failed to save copy.");
    }
}

void MainWindow::updatePageUi() {
    const int pages = m_doc->pageCount();
    m_pageSpin->setEnabled(pages > 0);
    m_pageSpin->setMaximum(pages > 0 ? pages : 1);
    m_pageSpin->setValue(pages > 0 ? 1 : 1);
    m_pageLabel->setText(QString("/ %1").arg(pages));
}

void MainWindow::nextPage() {
    if (!m_doc->pageCount()) return;
    auto nav = m_view->pageNavigator();
    int p = nav->currentPage();
    if (p + 1 < m_doc->pageCount()) {
        nav->jump(p + 1, QPointF(0,0), 0);
        m_pageSpin->setValue(p + 2);
    }
}

void MainWindow::prevPage() {
    if (!m_doc->pageCount()) return;
    auto nav = m_view->pageNavigator();
    int p = nav->currentPage();
    if (p > 0) {
        nav->jump(p - 1, QPointF(0,0), 0);
        m_pageSpin->setValue(p);
    }
}

void MainWindow::pageSpinChanged(int oneBased) {
    if (!m_doc->pageCount()) return;
    auto nav = m_view->pageNavigator();
    int clamped = qBound(0, oneBased - 1, m_doc->pageCount() - 1);
    nav->jump(clamped, QPointF(0,0), 0);
}

void MainWindow::zoomIn() {
    m_view->setZoomMode(QPdfView::ZoomMode::Custom);
    m_view->setZoomFactor(m_view->zoomFactor() * 1.2);
}

void MainWindow::zoomOut() {
    m_view->setZoomMode(QPdfView::ZoomMode::Custom);
    m_view->setZoomFactor(m_view->zoomFactor() / 1.2);
}

void MainWindow::fitWidth() { m_view->setZoomMode(QPdfView::ZoomMode::FitToWidth); }
void MainWindow::fitPage()  { m_view->setZoomMode(QPdfView::ZoomMode::FitInView); }





































// #include "mainwindow.h"
// #include "ui_mainwindow.h"

// #include <QtPdf/QPdfDocument>
// #include <QtPdf/QPdfPageNavigation>
// #include <QtPdfWidgets/QPdfView>

// #include <QVBoxLayout>
// #include <QToolBar>
// #include <QAction>
// #include <QSpinBox>
// #include <QLabel>
// #include <QFileDialog>
// #include <QFileInfo>
// #include <QFile>
// #include <QMessageBox>
// #include <QStandardPaths>
// #include <QProcess>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent), ui(new Ui::MainWindow),
//     m_doc(new QPdfDocument(this)), m_view(new QPdfView(this)),
//     m_pageSpin(nullptr), m_pageLabel(nullptr)
// {
//     ui->setupUi(this);
//     setupUi();

//     m_view->setDocument(m_doc);
//     // Start with "fit to width" for a nice default
//     m_view->setZoomMode(QPdfView::ZoomMode::FitToWidth);

//     setWindowTitle("PDFEditor");
//     resize(1100, 780);
// }

// MainWindow::~MainWindow() { delete ui; }

// void MainWindow::setupUi() {
//     // Put the PDF view into the central widget
//     auto *layout = new QVBoxLayout(ui->centralwidget);
//     layout->setContentsMargins(0,0,0,0);
//     layout->addWidget(m_view);

//     // Toolbar
//     auto *tb = addToolBar("Main");

//     auto openAct     = tb->addAction("Open",    this, &MainWindow::openPdf);
//     auto saveCopyAct = tb->addAction("Save Copy", this, &MainWindow::saveCopyAs);
//     tb->addSeparator();

//     auto prevAct     = tb->addAction("Prev",    this, &MainWindow::prevPage);
//     auto nextAct     = tb->addAction("Next",    this, &MainWindow::nextPage);

//     m_pageSpin = new QSpinBox(this);
//     m_pageSpin->setMinimum(1);
//     m_pageSpin->setMaximum(1);
//     m_pageSpin->setEnabled(false);
//     connect(m_pageSpin, &QSpinBox::valueChanged, this, &MainWindow::pageSpinChanged);
//     tb->addWidget(m_pageSpin);

//     m_pageLabel = new QLabel("/ 0", this);
//     tb->addWidget(m_pageLabel);

//     tb->addSeparator();
//     auto zoomInAct   = tb->addAction("Zoom +",  this, &MainWindow::zoomIn);
//     auto zoomOutAct  = tb->addAction("Zoom −",  this, &MainWindow::zoomOut);
//     auto fitWAct     = tb->addAction("Fit Width", this, &MainWindow::fitWidth);
//     auto fitPAct     = tb->addAction("Fit Page",  this, &MainWindow::fitPage);

//     tb->addSeparator();
//     auto rotLAct     = tb->addAction("Rotate ⟲", this, &MainWindow::rotateLeft);
//     auto rotRAct     = tb->addAction("Rotate ⟳", this, &MainWindow::rotateRight);

//     tb->addSeparator();
//     auto mergeAct    = tb->addAction("Merge…",   this, &MainWindow::mergePdfs);
//     auto rangeAct    = tb->addAction("Extract Range…", this, &MainWindow::extractRange);
//     auto splitAct    = tb->addAction("Split Pages…",   this, &MainWindow::splitIntoSinglePages);

//     Q_UNUSED(openAct); Q_UNUSED(saveCopyAct); Q_UNUSED(prevAct); Q_UNUSED(nextAct);
//     Q_UNUSED(zoomInAct); Q_UNUSED(zoomOutAct); Q_UNUSED(fitWAct); Q_UNUSED(fitPAct);
//     Q_UNUSED(rotLAct); Q_UNUSED(rotRAct); Q_UNUSED(mergeAct); Q_UNUSED(rangeAct); Q_UNUSED(splitAct);
// }

// void MainWindow::openPdf() {
//     const QString fn = QFileDialog::getOpenFileName(this, "Open PDF", {}, "PDF Files (*.pdf)");
//     if (fn.isEmpty()) return;

//     auto err = m_doc->load(fn);
//     if (err != QPdfDocument::NoError) {
//         QMessageBox::warning(this, "Open PDF", "Failed to open PDF.");
//         return;
//     }
//     m_currentFile = fn;

//     // Go to page 1
//     if (auto nav = m_view->pageNavigation())
//         nav->setCurrentPage(0);

//     updatePageUi();
//     setWindowTitle(QString("PDFEditor — %1").arg(QFileInfo(fn).fileName()));
// }

// void MainWindow::saveCopyAs() {
//     if (m_currentFile.isEmpty()) {
//         QMessageBox::information(this, "Save Copy", "Open a PDF first.");
//         return;
//     }
//     const QString out = QFileDialog::getSaveFileName(this, "Save Copy As", {}, "PDF Files (*.pdf)");
//     if (out.isEmpty()) return;
//     if (!QFile::copy(m_currentFile, out)) {
//         QMessageBox::warning(this, "Save Copy", "Failed to save copy.");
//     }
// }

// void MainWindow::updatePageUi() {
//     const int pages = m_doc->pageCount();
//     m_pageSpin->setEnabled(pages > 0);
//     m_pageSpin->setMaximum(qMax(1, pages));
//     m_pageSpin->setValue(1);
//     m_pageLabel->setText(QString("/ %1").arg(pages));
// }

// void MainWindow::nextPage() {
//     if (!m_doc->pageCount()) return;
//     auto nav = m_view->pageNavigation();
//     int p = nav->currentPage();
//     if (p + 1 < m_doc->pageCount()) {
//         nav->setCurrentPage(p + 1);
//         m_pageSpin->setValue(p + 2);
//     }
// }

// void MainWindow::prevPage() {
//     if (!m_doc->pageCount()) return;
//     auto nav = m_view->pageNavigation();
//     int p = nav->currentPage();
//     if (p > 0) {
//         nav->setCurrentPage(p - 1);
//         m_pageSpin->setValue(p);
//     }
// }

// void MainWindow::pageSpinChanged(int oneBased) {
//     if (!m_doc->pageCount()) return;
//     auto nav = m_view->pageNavigation();
//     nav->setCurrentPage(qBound(0, oneBased - 1, m_doc->pageCount() - 1));
// }

// void MainWindow::zoomIn() {
//     m_view->setZoomMode(QPdfView::ZoomMode::Custom);
//     m_view->setZoomFactor(m_view->zoomFactor() * 1.2);
// }

// void MainWindow::zoomOut() {
//     m_view->setZoomMode(QPdfView::ZoomMode::Custom);
//     m_view->setZoomFactor(m_view->zoomFactor() / 1.2);
// }

// void MainWindow::fitWidth() {
//     m_view->setZoomMode(QPdfView::ZoomMode::FitToWidth);
// }

// void MainWindow::fitPage() {
//     m_view->setZoomMode(QPdfView::ZoomMode::FitInView);
// }

// void MainWindow::rotateLeft() {
//     m_view->setRotation((m_view->rotation() + 270) % 360);
// }

// void MainWindow::rotateRight() {
//     m_view->setRotation((m_view->rotation() + 90) % 360);
// }

// // ---------- qpdf helpers ----------
// bool MainWindow::ensureQpdf(QString *exePathOut) const {
//     const QString exe = QStandardPaths::findExecutable("qpdf");
//     if (exePathOut) *exePathOut = exe;
//     if (exe.isEmpty()) {
//         QMessageBox::warning(nullptr, "qpdf not found",
//                              "qpdf executable is not on PATH.\n\n"
//                              "Install with vcpkg:\n"
//                              "  vcpkg install qpdf:x64-mingw-dynamic\n\n"
//                              "Then add the vcpkg 'installed/x64-mingw-dynamic/bin' folder to your PATH and restart Qt Creator.");
//         return false;
//     }
//     return true;
// }

// bool MainWindow::runQpdf(const QStringList &args, QString *stderrOut) const {
//     QString exe;
//     if (!ensureQpdf(&exe)) return false;

//     QProcess p;
//     p.start(exe, args);
//     if (!p.waitForFinished(-1)) {
//         QMessageBox::warning(nullptr, "qpdf", "qpdf did not finish.");
//         return false;
//     }
//     if (p.exitStatus() != QProcess::NormalExit || p.exitCode() != 0) {
//         if (stderrOut) *stderrOut = p.readAllStandardError();
//         QMessageBox::warning(nullptr, "qpdf error",
//                              QString("qpdf failed.\n\nArgs:\n%1\n\nError:\n%2")
//                                  .arg(args.join(' '))
//                                  .arg(QString::fromUtf8(p.readAllStandardError())));
//         return false;
//     }
//     return true;
// }

// // Merge: qpdf --empty --pages file1.pdf file2.pdf -- out.pdf
// void MainWindow::mergePdfs() {
//     const QStringList files = QFileDialog::getOpenFileNames(this, "Select PDFs to merge", {}, "PDF Files (*.pdf)");
//     if (files.size() < 2) return;
//     const QString out = QFileDialog::getSaveFileName(this, "Save merged PDF as", {}, "PDF Files (*.pdf)");
//     if (out.isEmpty()) return;

//     QStringList args;
//     args << "--empty" << "--pages";
//     args << files;
//     args << "--" << out;

//     runQpdf(args);
// }

// // Extract range: qpdf --empty --pages in.pdf 2-5 -- out.pdf
// void MainWindow::extractRange() {
//     const QString in = m_currentFile.isEmpty()
//     ? QFileDialog::getOpenFileName(this, "Source PDF", {}, "PDF Files (*.pdf)")
//     : m_currentFile;
//     if (in.isEmpty()) return;

//     bool ok = false;
//     const QString range = QInputDialog::getText(this, "Extract Page Range",
//                                                 "Range (e.g. 1-4,6):", QLineEdit::Normal,
//                                                 "1-3", &ok);
//     if (!ok || range.trimmed().isEmpty()) return;

//     const QString out = QFileDialog::getSaveFileName(this, "Save extracted range as", {}, "PDF Files (*.pdf)");
//     if (out.isEmpty()) return;

//     QStringList args;
//     args << "--empty" << "--pages" << in << range << "--" << out;
//     runQpdf(args);
// }




// // Split to single pages: qpdf --split-pages=1 in.pdf out-%d.pdf
// void MainWindow::splitIntoSinglePages() {
//     const QString in = m_currentFile.isEmpty()
//     ? QFileDialog::getOpenFileName(this, "PDF to split", {}, "PDF Files (*.pdf)")
//     : m_currentFile;
//     if (in.isEmpty()) return;

//     const QString dir = QFileDialog::getExistingDirectory(this, "Choose output folder");
//     if (dir.isEmpty()) return;

//     const QString pattern = QDir(dir).filePath("page-%d.pdf");
//     QStringList args;
//     args << "--split-pages=1" << in << pattern;
//     runQpdf(args);
// }
















// #include "mainwindow.h"
// #include "./ui_mainwindow.h"

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
// {
//     ui->setupUi(this);
// }

// MainWindow::~MainWindow()
// {
//     delete ui;
// }
