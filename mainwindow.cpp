#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtPdf/QPdfDocument>
#include <QtPdfWidgets/QPdfView>
#include <QtPdf/QPdfPageNavigator>
#include <QtPdf/QPdfSearchModel>
#include <QtPdf/QPdfLink>
#include <QLineEdit>


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


//Header s for search bar - making ir smaller and implementin g shortcuts
// #include <QLineEdit>
#include <QToolButton>
#include <QShortcut>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_doc(new QPdfDocument(this)), m_view(new QPdfView(this)),
    m_pageSpin(nullptr), m_pageLabel(nullptr)
{
    ui->setupUi(this);
    setupUi();

    m_view->setDocument(m_doc);
    #if (QT_VERSION >= QT_VERSION_CHECK(6, 6, 0))
        m_search = new QPdfSearchModel(this);
        m_search->setDocument(m_doc);
        m_view->setSearchModel(m_search);
    #endif

    m_view->setZoomMode(QPdfView::ZoomMode::FitToWidth);
    m_view->setPageMode(QPdfView::PageMode::SinglePage);

    setWindowTitle("PDFEditor");
    resize(1100, 780);
}

MainWindow::~MainWindow() { delete ui; }


void MainWindow::setupUi() {
    auto *layout = new QVBoxLayout(ui->centralwidget);
    layout->setContentsMargins(0,0,0,0);

    // 1) Compact find bar (hidden by default)
    m_findBar = new QWidget(this);
    m_findBar->setVisible(false);
    m_findBar->setMaximumHeight(32);

    auto *hb = new QHBoxLayout(m_findBar);
    hb->setContentsMargins(6, 3, 6, 3);
    hb->setSpacing(6);

    m_findEdit = new QLineEdit(m_findBar);
    m_findEdit->setPlaceholderText("Find");
    m_findEdit->setClearButtonEnabled(true);
    m_findEdit->installEventFilter(this);    // Enter/Shift+Enter handling
    hb->addWidget(m_findEdit);

    m_btnPrev = new QToolButton(m_findBar);
    m_btnPrev->setText("⟸");
    hb->addWidget(m_btnPrev);

    m_btnNext = new QToolButton(m_findBar);
    m_btnNext->setText("⟹");
    hb->addWidget(m_btnNext);

    m_findCount = new QLabel("0/0", m_findBar);
    hb->addWidget(m_findCount);

    m_btnClose = new QToolButton(m_findBar);
    m_btnClose->setText("✕");
    hb->addWidget(m_btnClose);

    // 2) Main toolbar (unchanged)
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

    // Add the find bar and the view to the page layout
    layout->addWidget(m_findBar);
    layout->addWidget(m_view);

    // Wire find bar actions
    connect(m_btnPrev,  &QToolButton::clicked, this, &MainWindow::findPrev);
    connect(m_btnNext,  &QToolButton::clicked, this, &MainWindow::findNext);
    connect(m_btnClose, &QToolButton::clicked, this, &MainWindow::hideFindBar);
    connect(m_findEdit, &QLineEdit::textChanged, this, &MainWindow::findTextChanged);

    // Global shortcuts
    m_scFind = new QShortcut(QKeySequence::Find, this);        // Ctrl+F
    m_scNext = new QShortcut(QKeySequence::FindNext, this);    // F3
    m_scPrev = new QShortcut(QKeySequence::FindPrevious, this);// Shift+F3
    m_scEsc  = new QShortcut(QKeySequence(Qt::Key_Escape), this);

    connect(m_scFind, &QShortcut::activated, this, &MainWindow::showFindBar);
    connect(m_scNext, &QShortcut::activated, this, &MainWindow::findNext);
    connect(m_scPrev, &QShortcut::activated, this, &MainWindow::findPrev);
    connect(m_scEsc,  &QShortcut::activated, this, &MainWindow::hideFindBar);
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

void MainWindow::findTextChanged(const QString& s) {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 6, 0))
    if (!m_search) return;
    m_search->setSearchString(s);

#   if (QT_VERSION >= QT_VERSION_CHECK(6, 8, 0))
    int n = m_search->count();
#   else
    int n = m_search->rowCount({});
#   endif

    // Empty query or no hits → clear highlights and show 0/0
    if (s.trimmed().isEmpty() || n == 0) {
        m_searchIndex = -1;
        m_view->setCurrentSearchResultIndex(-1);
        if (m_findCount) m_findCount->setText("0/0");
        return;
    }

    // Go to first hit
    m_searchIndex = 0;
    m_view->setCurrentSearchResultIndex(m_searchIndex);
    if (m_findCount) m_findCount->setText(QString("%1/%2").arg(m_searchIndex + 1).arg(n));

    const QPdfLink link = m_search->resultAtIndex(m_searchIndex);
    if (auto nav = m_view->pageNavigator())
        nav->jump(link.page(), link.location(), 0);
#else
    Q_UNUSED(s);
#endif
}

void MainWindow::findNext() {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 6, 0))
    if (!m_search) return;
#   if (QT_VERSION >= QT_VERSION_CHECK(6, 8, 0))
    const int n = m_search->count();
#   else
    const int n = m_search->rowCount({});
#   endif
    if (n <= 0) { if (m_findCount) m_findCount->setText("0/0"); return; }

    if (m_searchIndex < 0) m_searchIndex = 0;
    else m_searchIndex = (m_searchIndex + 1) % n;

    m_view->setCurrentSearchResultIndex(m_searchIndex);
    if (m_findCount) m_findCount->setText(QString("%1/%2").arg(m_searchIndex + 1).arg(n));

    const QPdfLink link = m_search->resultAtIndex(m_searchIndex);
    if (auto nav = m_view->pageNavigator())
        nav->jump(link.page(), link.location(), 0);
#endif
}

void MainWindow::findPrev() {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 6, 0))
    if (!m_search) return;
#   if (QT_VERSION >= QT_VERSION_CHECK(6, 8, 0))
    const int n = m_search->count();
#   else
    const int n = m_search->rowCount({});
#   endif
    if (n <= 0) { if (m_findCount) m_findCount->setText("0/0"); return; }

    if (m_searchIndex < 0) m_searchIndex = 0;
    else m_searchIndex = (m_searchIndex - 1 + n) % n;

    m_view->setCurrentSearchResultIndex(m_searchIndex);
    if (m_findCount) m_findCount->setText(QString("%1/%2").arg(m_searchIndex + 1).arg(n));

    const QPdfLink link = m_search->resultAtIndex(m_searchIndex);
    if (auto nav = m_view->pageNavigator())
        nav->jump(link.page(), link.location(), 0);
#endif
}


//Changes makde so that current / total is visible while searching
void MainWindow::showFindBar() {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 6, 0))
    if (!m_findBar) return;
    m_findBar->setVisible(true);
    if (m_findEdit) {
        m_findEdit->setFocus(Qt::ShortcutFocusReason);
        m_findEdit->selectAll();
    }
#   if (QT_VERSION >= QT_VERSION_CHECK(6, 8, 0))
    int n = m_search ? m_search->count() : 0;
#   else
    int n = m_search ? m_search->rowCount({}) : 0;
#   endif
    if (m_findCount) {
        if (m_searchIndex >= 0 && n > 0)
            m_findCount->setText(QString("%1/%2").arg(m_searchIndex + 1).arg(n));
        else
            m_findCount->setText("0/0");
    }
#else
    QMessageBox::information(this, "Find",
                             "Upgrade Qt to ≥ 6.6 to enable in-view highlights and navigation.");
#endif
}





//Error fixes - (Note to self : Mignt need to remove this)
// Hide the compact find bar (Esc or ✕)
void MainWindow::hideFindBar() {
    if (m_findBar && m_findBar->isVisible())
        m_findBar->setVisible(false);
}

// Make Enter = Next, Shift+Enter = Prev, Esc = close
bool MainWindow::eventFilter(QObject* obj, QEvent* ev) {
    if (obj == m_findEdit && ev->type() == QEvent::KeyPress) {
        auto *ke = static_cast<QKeyEvent*>(ev);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
            if (ke->modifiers() & Qt::ShiftModifier) findPrev();
            else findNext();
            return true; // handled
        }
        if (ke->key() == Qt::Key_Escape) {
            hideFindBar();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, ev);
}
