#pragma once
#include <QMainWindow>
#include <QVector>
#include <QRectF>
#include <QColor>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class MarkupKind { Highlight, Underline, StrikeOut };
class QWidget;
class QEvent;
class QPdfDocument;
class QPdfView;
class QSpinBox;
class QLabel;
class QPdfSearchModel;
class QLineEdit;
class QToolButton;
class QShortcut;

struct Markup {
    int page = 0;
    QVector<QRectF> quadsPts;   // rectangles in PAGE POINTS
    MarkupKind kind = MarkupKind::Highlight;
    QColor color = QColor(255, 235, 0, 96); // translucent yellow by default
};
class AnnotationOverlay;

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

    //search slots
    void findTextChanged(const QString& s);
    void findNext();
    void findPrev();
    void showFindBar();
    void hideFindBar();

    //Anotation slots
    void startHighlight();
    void startUnderline();
    void startStrike();
    void cancelAnnotate();
    void exportAnnotations();


protected:
    bool eventFilter(QObject* obj, QEvent* ev) override; // for Enter / Shift+Enter in the find box

private:
    void setupUi();
    void updatePageUi();

    Ui::MainWindow *ui;
    QPdfDocument   *m_doc;
    QPdfView       *m_view;
    QString         m_currentFile;

    QSpinBox *m_pageSpin;
    QLabel   *m_pageLabel;

    //Search Box ka Implementation
    QPdfSearchModel* m_search = nullptr;
    QWidget*         m_findBar = nullptr;
    QLineEdit*       m_findEdit = nullptr;
    QLabel*          m_findCount = nullptr;
    QToolButton*     m_btnPrev = nullptr;
    QToolButton*     m_btnNext = nullptr;
    QToolButton*     m_btnClose = nullptr;
    int              m_searchIndex = -1;


    // --- annotations (overlay + model) ---
    QVector<Markup>     m_marks;
    AnnotationOverlay*  m_overlay = nullptr;

    // annotate-by-drag state
    bool        m_annotateMode = false;
    MarkupKind  m_pendingKind  = MarkupKind::Highlight;
    int         m_selPage      = -1;
    QPointF     m_selStartPts, m_selEndPts;  // page-space (points)



    // Defining Shortcuts
    QShortcut* m_scFind = nullptr;
    QShortcut* m_scNext = nullptr;
    QShortcut* m_scPrev = nullptr;
    QShortcut* m_scEsc  = nullptr;

    //For annotations w
    bool mapViewportToPage(const QPoint& vpPos, int* outPage, QPointF* outPagePt) const;
    void addMarkupFromSelection(MarkupKind kind, int page, const QList<QRectF>& quadsPts);

    // sidecar I/O
    void saveAnnotationsJson(const QString& jsonPath) const;
    bool loadAnnotationsJson(const QString& jsonPath);

};
