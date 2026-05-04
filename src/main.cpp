/*
    介绍：CompilerIDE 是由 CompilerIDE Team 开发的一款IDE，采用 GPL 3.0 协议开源，内置数据评测器、竞赛模式、洛谷题目爬取器、对拍器等众多功能
    源码：20000+ 行
    AI 创作声明：本 IDE 部分内容使用 DeepSeek、Claude、Gemini 等辅助生成
    创作不易，如果你喜欢，可以在 Github 上点一个 Star 哦
*/
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QVBoxLayout>
#include <QPdfView>
#include <QPdfDocument>
#include <QHBoxLayout>
#include <QWebEnginePermission>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QTimer>
#include <QWebEngineProfile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QClipboard>
#include <QGuiApplication>
#include <QRegularExpression>
#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QDockWidget>
#include <QPointer>
#include <QStackedWidget>
#include <QWindow>
#include <windows.h>
#include <dwmapi.h>
#include <tlhelp32.h>
#include <QThread>
#include <QElapsedTimer>
#include <QWidget>
#include <QProcess>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>
#include <QMutex>
#include <QDesktopServices>
#include <QUrl>
#include <QPlainTextEdit>
#include <QFont>
#include <QFontDatabase>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QStringListModel>
#include <QListView>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QTime>
#include <QTextCursor>
#include <QTextBlock>
#include <QScrollBar>
#include <QDir>
#include <QStandardPaths>
#include <QComboBox>
#include <QLabel>
#include <QTextStream>
#include <QPair>
#include <QList>
#include <QPainter>
#include <QPaintEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QTextDocument>
#include <QStyleFactory>
#include <QPalette>
#include <QTabWidget>
#include <QSplitter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QToolButton>
#include <QInputDialog>
#include <QCollator>
#include <QMimeData>
#include <QShortcut>
#include <QTextDocumentFragment>
#include <QTextBlockUserData>
#include <QScrollArea>
#include <QTemporaryFile>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include <QProxyStyle>
#include <QCommonStyle>
#include <shellapi.h>
#include <Psapi.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressDialog>
#include <QDirIterator>
#include <QListWidget>
#include <QProgressBar>
#include <QCryptographicHash>
#include <QRadioButton>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTextBrowser>
#include <QHeaderView>
#include <QChart>
#include <QChartView>
#include <QFontComboBox>
#include <QLCDNumber>

#pragma comment(lib, "windowsapp.lib")

const QString IDE_VERSION = "3.5.2";

const QString BUILD_DATE = "2026-05-04";
const QString BUILD_TIME = "16:36:04";

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#endif

const QString ACCOUNTS_FILE = "accounts.dat";
const QString SESSION_FILE = "session.dat";

class ModernStyle : public QProxyStyle
{
public:
    ModernStyle() : QProxyStyle("Fusion") {}

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const override
    {
        if (element == PE_FrameFocusRect)
        {
            return;
        }

        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override
    {
        switch (metric)
        {
        case PM_SplitterWidth:
            return 6;
        case PM_TabBarTabHSpace:
            return 20;
        case PM_TabBarTabVSpace:
            return 10;
        default:
            return QProxyStyle::pixelMetric(metric, option, widget);
        }
    }
};

// IDE 设置界面
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr,
                            const QString &currentCompilerPath = "",
                            const QString &currentDebuggerPath = "",
                            bool autoBrackets = true,
                            bool autoQuotes = true,
                            bool autoIndent = true,
                            int indentSize = 4,
                            bool lineNumbers = true,
                            bool darkTheme = true,
                            bool codeBeautify = true,
                            bool codeCompletion = true,
                            const QFont &editorFont = QFont("Consolas", 11));

    QString getCompilerPath() const;
    QString getDebuggerPath() const;
    bool getAutoBrackets() const;
    bool getAutoQuotes() const;
    bool getAutoIndent() const;
    int getIndentSize() const;
    bool getLineNumbers() const;
    bool getDarkTheme() const;
    bool getCodeCompletion() const;
    bool getCodeBeautify() const;
    QFont getEditorFont() const;
    int getEditorFontSize() const;
    bool getShowIndentGuides() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void browseCompilerPath();
    void browseDebuggerPath();
    void onCompilerPathChanged(const QString &path);
    void addCustomCompletion();
    void removeCustomCompletion();

private:
    QFontComboBox *fontCombo;
    QSpinBox *fontSizeSpin;
    QCheckBox *codeBeautifyCheck;
    QCheckBox *splashCheck;
    QCheckBox *welcomeCheck;
    QCheckBox *codeCompletionCheck;
    QLineEdit *compilerPathEdit;
    QLineEdit *debuggerPathEdit;
    QCheckBox *showIndentGuidesCheck;
    QCheckBox *autoBracketsCheck;
    QCheckBox *autoQuotesCheck;
    QCheckBox *autoIndentCheck;
    QSpinBox *indentSizeSpin;
    QCheckBox *lineNumbersCheck;
    QListWidget *customCompletionList;
    QLineEdit *newCompletionEdit;
    QCheckBox *darkThemeCheck;
    QComboBox *uiStyleCombo;

    QString findCompilerInPath(const QString &basePath);
    QString findDebuggerFromCompilerPath(const QString &compilerPath);
    QString autoDetectDebuggerPath();
    void loadCustomCompletions();
    void saveCustomCompletions();
};

SettingsDialog::SettingsDialog(QWidget *parent, const QString &currentCompilerPath,
                               const QString &currentDebuggerPath,
                               bool autoBrackets, bool autoQuotes, bool autoIndent,
                               int indentSize, bool lineNumbers,
                               bool darkTheme, bool codeBeautify, bool codeCompletion, const QFont &editorFont)
    : QDialog(parent)
{
    setWindowTitle(tr("设置 - Compiler IDE %1").arg(IDE_VERSION));
    setModal(true);
    setMinimumSize(750, 500);
    resize(800, 550);

    qDebug() << "[SettingsDialog] 初始字体:" << editorFont.family() << editorFont.pointSize();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QListWidget *navList = new QListWidget;
    navList->setFixedWidth(180);
    navList->setSpacing(4);
    navList->setFrameShape(QFrame::NoFrame);

    // 导航栏 UI 设置
    QString navStyle = darkTheme ?
                       "QListWidget { background: #252526; border: none; padding-top: 8px; }"
                       "QListWidget::item { padding: 14px 20px; color: #cccccc; border-radius: 0px; margin: 2px 0px; }"
                       "QListWidget::item:selected { background: #37373d; color: #ffffff; border-left: 3px solid #0d6efd; }"
                       "QListWidget::item:hover:!selected { background: #2a2d2e; }"
                       :
                       "QListWidget { background: #f3f3f3; border: none; padding-top: 8px; }"
                       "QListWidget::item { padding: 14px 20px; color: #333333; border-radius: 0px; margin: 2px 0px; }"
                       "QListWidget::item:selected { background: #ffffff; color: #000000; border-left: 3px solid #0d6efd; }"
                       "QListWidget::item:hover:!selected { background: #e8e8e8; }";
    navList->setStyleSheet(navStyle);

    // 导航页面选项
    QListWidgetItem *compilerItem = new QListWidgetItem(tr("  编译器设置"));
    QListWidgetItem *editorItem = new QListWidgetItem(tr("  编辑器设置"));
    QListWidgetItem *customCompItem = new QListWidgetItem(tr("  自定义代码补全"));

    navList->addItem(compilerItem);
    navList->addItem(editorItem);
    navList->addItem(customCompItem);
    navList->setCurrentRow(0);

    QWidget *rightPanel = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    QStackedWidget *contentStack = new QStackedWidget;

    QString infoTextColor = darkTheme ? "#AAAAAA" : "#666666";

    QString groupBoxStyle = QString(
                                "QGroupBox {"
                                "    border: 1px solid %1;"
                                "    border-radius: 8px;"
                                "    margin-top: 12px;"
                                "    padding-top: 8px;"
                                "    font-weight: bold;"
                                "}"
                                "QGroupBox::title {"
                                "    subcontrol-origin: margin;"
                                "    left: 16px;"
                                "    padding: 0 8px;"
                                "}"
                            ).arg(darkTheme ? "#4A5F7F" : "#D0D0D0");

    // 编译器设置页面
    QWidget *compilerPage = new QWidget;
    QVBoxLayout *compilerPageLayout = new QVBoxLayout(compilerPage);
    compilerPageLayout->setContentsMargins(24, 24, 24, 24);

    QScrollArea *compilerScrollArea = new QScrollArea;
    compilerScrollArea->setWidgetResizable(true);
    compilerScrollArea->setFrameShape(QFrame::NoFrame);
    compilerScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    compilerScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *compilerContent = new QWidget;
    QVBoxLayout *compilerContentLayout = new QVBoxLayout(compilerContent);
    compilerContentLayout->setSpacing(16);

    QGroupBox *compilerGroup = new QGroupBox(tr("编译器设置"));
    compilerGroup->setStyleSheet(groupBoxStyle);
    QFormLayout *compilerLayout = new QFormLayout;

    compilerPathEdit = new QLineEdit(currentCompilerPath);
    QPushButton *browseCompilerButton = new QPushButton(tr("浏览..."));
    connect(browseCompilerButton, &QPushButton::clicked, this, &SettingsDialog::browseCompilerPath);
    connect(compilerPathEdit, &QLineEdit::textChanged, this, &SettingsDialog::onCompilerPathChanged);

    QHBoxLayout *compilerPathLayout = new QHBoxLayout;
    compilerPathLayout->addWidget(compilerPathEdit);
    compilerPathLayout->addWidget(browseCompilerButton);

    compilerLayout->addRow(tr("编译器路径:"), compilerPathLayout);

    QLabel *compilerInfoLabel = new QLabel(tr("请指定已安装的C++编译器路径\n(例如: g++.exe, gcc.exe, cl.exe等)"));
    compilerInfoLabel->setWordWrap(true);
    compilerInfoLabel->setStyleSheet(QString("QLabel { color: %1; font-size: 9pt; }").arg(infoTextColor));
    compilerLayout->addRow(tr("说明:"), compilerInfoLabel);

    QLabel *pathSourceLabel = new QLabel();
    if (currentCompilerPath.contains("CompilerIDE"))
    {
        pathSourceLabel->setText(tr("当前使用安装时设置的编译器路径"));
        pathSourceLabel->setStyleSheet("QLabel { color: green; }");
    }
    else
    {
        pathSourceLabel->setText(tr("当前使用自动检测或用户选择的编译器路径"));
        pathSourceLabel->setStyleSheet("QLabel { color: orange; }");
    }
    compilerLayout->addRow(tr("路径来源:"), pathSourceLabel);

    compilerGroup->setLayout(compilerLayout);

    QGroupBox *debuggerGroup = new QGroupBox(tr("调试器设置"));
    debuggerGroup->setStyleSheet(groupBoxStyle);
    QFormLayout *debuggerLayout = new QFormLayout;

    debuggerPathEdit = new QLineEdit(currentDebuggerPath);
    QPushButton *browseDebuggerButton = new QPushButton(tr("浏览..."));
    connect(browseDebuggerButton, &QPushButton::clicked, this, &SettingsDialog::browseDebuggerPath);

    QHBoxLayout *debuggerPathLayout = new QHBoxLayout;
    debuggerPathLayout->addWidget(debuggerPathEdit);
    debuggerPathLayout->addWidget(browseDebuggerButton);

    debuggerLayout->addRow(tr("调试器路径:"), debuggerPathLayout);

    QLabel *debuggerInfoLabel = new QLabel(tr("请指定GDB调试器路径\n(例如: gdb.exe)\n如果留空，将自动从编译器目录检测"));
    debuggerInfoLabel->setWordWrap(true);
    debuggerInfoLabel->setStyleSheet(QString("QLabel { color: %1; font-size: 9pt; }").arg(infoTextColor));
    debuggerLayout->addRow(tr("说明:"), debuggerInfoLabel);

    QLabel *debuggerPathSourceLabel = new QLabel();
    if (!currentDebuggerPath.isEmpty() && QFile::exists(currentDebuggerPath))
    {
        debuggerPathSourceLabel->setText(tr("当前使用用户设置的调试器路径"));
        debuggerPathSourceLabel->setStyleSheet("QLabel { color: green; }");
    }
    else
    {
        debuggerPathSourceLabel->setText(tr("将自动从编译器目录检测"));
        debuggerPathSourceLabel->setStyleSheet("QLabel { color: orange; }");
    }
    debuggerLayout->addRow(tr("路径来源:"), debuggerPathSourceLabel);

    QPushButton *autoDetectDebuggerBtn = new QPushButton(tr("从编译器目录自动检测"));
    connect(autoDetectDebuggerBtn, &QPushButton::clicked, this, [this]()
    {
        QString detectedPath = findDebuggerFromCompilerPath(compilerPathEdit->text());
        if (!detectedPath.isEmpty())
        {
            debuggerPathEdit->setText(detectedPath);
            QMessageBox::information(this, tr("自动检测"), tr("已检测到调试器: %1").arg(detectedPath));
        }
        else
        {
            QMessageBox::warning(this, tr("自动检测"), tr("未能在编译器目录中找到gdb.exe"));
        }
    });
    debuggerLayout->addRow("", autoDetectDebuggerBtn);

    debuggerGroup->setLayout(debuggerLayout);

    compilerContentLayout->addWidget(compilerGroup);
    compilerContentLayout->addWidget(debuggerGroup);
    compilerContentLayout->addStretch();

    compilerScrollArea->setWidget(compilerContent);
    compilerPageLayout->addWidget(compilerScrollArea);

    // 编辑器设置页面
    QWidget *editorPage = new QWidget;
    QVBoxLayout *editorPageLayout = new QVBoxLayout(editorPage);
    editorPageLayout->setContentsMargins(24, 24, 24, 24);

    QScrollArea *editorScrollArea = new QScrollArea;
    editorScrollArea->setWidgetResizable(true);
    editorScrollArea->setFrameShape(QFrame::NoFrame);
    editorScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    editorScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *editorContent = new QWidget;
    QVBoxLayout *editorContentLayout = new QVBoxLayout(editorContent);
    editorContentLayout->setSpacing(16);

    showIndentGuidesCheck = new QCheckBox(tr("显示缩进引导线"));
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    showIndentGuidesCheck->setChecked(settings.value("showIndentGuides", true).toBool());

    QGroupBox *editorGroup = new QGroupBox(tr("编辑器设置"));
    editorGroup->setStyleSheet(groupBoxStyle);
    QFormLayout *editorLayout = new QFormLayout;
    editorLayout->setVerticalSpacing(12);

    autoBracketsCheck = new QCheckBox(tr("启用自动括号补全"));
    autoBracketsCheck->setChecked(autoBrackets);

    autoQuotesCheck = new QCheckBox(tr("启用自动引号补全"));
    autoQuotesCheck->setChecked(autoQuotes);

    codeBeautifyCheck = new QCheckBox(tr("启用代码美化"));
    codeBeautifyCheck->setChecked(codeBeautify);

    codeCompletionCheck = new QCheckBox(tr("启用代码补全"));
    codeCompletionCheck->setChecked(codeCompletion);

    fontCombo = new QFontComboBox;
    fontCombo->setCurrentFont(editorFont);

    fontSizeSpin = new QSpinBox;
    fontSizeSpin->setRange(6, 72);
    fontSizeSpin->setValue(editorFont.pointSize());

    autoIndentCheck = new QCheckBox(tr("启用自动缩进"));
    autoIndentCheck->setChecked(autoIndent);

    indentSizeSpin = new QSpinBox;
    indentSizeSpin->setRange(1, 8);
    indentSizeSpin->setValue(indentSize);
    indentSizeSpin->setSuffix(tr(" 个空格"));

    lineNumbersCheck = new QCheckBox(tr("显示行号"));
    lineNumbersCheck->setChecked(lineNumbers);

    darkThemeCheck = new QCheckBox(tr("使用暗色主题"));
    darkThemeCheck->setChecked(darkTheme);

    editorLayout->addRow(darkThemeCheck);
    splashCheck = new QCheckBox(tr("启用启动界面"));
    welcomeCheck = new QCheckBox(tr("启用欢迎界面"));
    splashCheck->setChecked(settings.value("enableSplash", false).toBool());
    welcomeCheck->setChecked(settings.value("enableWelcome", true).toBool());

    editorLayout->addRow(splashCheck);
    editorLayout->addRow(welcomeCheck);
    editorLayout->addRow(autoBracketsCheck);
    editorLayout->addRow(autoQuotesCheck);
    editorLayout->addRow(codeBeautifyCheck);
    editorLayout->addRow(codeCompletionCheck);
    editorLayout->addRow(lineNumbersCheck);
    editorLayout->addRow(autoIndentCheck);
    editorLayout->addRow(showIndentGuidesCheck);
    editorLayout->addRow(tr("缩进大小:"), indentSizeSpin);
    editorLayout->addRow(tr("字体:"), fontCombo);
    editorLayout->addRow(tr("字号:"), fontSizeSpin);

    editorGroup->setLayout(editorLayout);

    editorContentLayout->addWidget(editorGroup);
    editorContentLayout->addStretch();

    editorScrollArea->setWidget(editorContent);
    editorPageLayout->addWidget(editorScrollArea);

    contentStack->addWidget(compilerPage);
    contentStack->addWidget(editorPage);

    // 自定义代码不全页面
    QWidget *customCompPage = new QWidget;
    QVBoxLayout *customCompPageLayout = new QVBoxLayout(customCompPage);
    customCompPageLayout->setContentsMargins(24, 24, 24, 24);

    QGroupBox *customCompGroup = new QGroupBox(tr("自定义代码补全词条"));
    customCompGroup->setStyleSheet(
        QString("QGroupBox {"
                "    border: 1px solid %1;"
                "    border-radius: 8px;"
                "    margin-top: 12px;"
                "    padding-top: 8px;"
                "    font-weight: bold;"
                "}"
                "QGroupBox::title {"
                "    subcontrol-origin: margin;"
                "    left: 16px;"
                "    padding: 0 8px;"
                "}").arg(darkTheme ? "#4A5F7F" : "#D0D0D0")
    );
    QVBoxLayout *customCompLayout = new QVBoxLayout(customCompGroup);
    customCompLayout->setSpacing(12);

    QLabel *customCompInfo = new QLabel(tr("在此处添加您专属的代码补全词条。\n(默认的 C++ 关键字、基础类型和函数已在后台内置，不支持删除，也无需在此重复添加)"));
    customCompInfo->setWordWrap(true);
    customCompInfo->setStyleSheet(QString("QLabel { color: %1; font-size: 9pt; padding: 4px 0px; }").arg(infoTextColor));

    QHBoxLayout *customInputLayout = new QHBoxLayout;
    customInputLayout->setSpacing(10);
    newCompletionEdit = new QLineEdit;
    newCompletionEdit->setPlaceholderText(tr("输入新词条..."));
    newCompletionEdit->installEventFilter(this);
    QString editStyle = QString(
                            "QLineEdit {"
                            "    border: 1px solid %1;"
                            "    border-radius: 5px;"
                            "    padding: 6px 10px;"
                            "    background: %2;"
                            "    color: %3;"
                            "    font-size: 12px;"
                            "}"
                            "QLineEdit:focus {"
                            "    border: 1px solid %4;"
                            "}"
                        ).arg(darkTheme ? "#5A5F7A" : "#CCCCCC",
                              darkTheme ? "#2D2D2D" : "#FFFFFF",
                              darkTheme ? "#F0F0F0" : "#000000",
                              darkTheme ? "#2E86AB" : "#0d6efd");
    newCompletionEdit->setStyleSheet(editStyle);
    if (darkTheme)
    {
        newCompletionEdit->setStyleSheet(editStyle + "QLineEdit::placeholder { color: #888888; }");
    }

    QPushButton *addCustomCompBtn = new QPushButton(tr("添加"));
    QString addBtnStyle = QString(
                              "QPushButton {"
                              "    border: none;"
                              "    border-radius: 6px;"
                              "    padding: 6px 16px;"
                              "    font-weight: bold;"
                              "    min-width: 70px;"
                              "    background-color: %1;"
                              "    color: white;"
                              "}"
                              "QPushButton:hover { background-color: %2; }"
                              "QPushButton:pressed { background-color: %3; }"
                          ).arg(darkTheme ? "#0d6efd" : "#0d6efd",
                                darkTheme ? "#0b5ed7" : "#0b5ed7",
                                darkTheme ? "#0a58ca" : "#0a58ca");
    addCustomCompBtn->setStyleSheet(addBtnStyle);

    customInputLayout->addWidget(newCompletionEdit);
    customInputLayout->addWidget(addCustomCompBtn);

    customCompletionList = new QListWidget;
    QString listStyle = QString(
                            "QListWidget {"
                            "    background: %1;"
                            "    color: %2;"
                            "    border: 1px solid %3;"
                            "    border-radius: 6px;"
                            "    outline: none;"
                            "    font-size: 12px;"
                            "}"
                            "QListWidget::item {"
                            "    padding: 6px 10px;"
                            "    border-bottom: 1px solid %4;"
                            "}"
                            "QListWidget::item:selected {"
                            "    background: %5;"
                            "    color: white;"
                            "}"
                            "QListWidget::item:hover:!selected {"
                            "    background: %6;"
                            "}"
                        ).arg(darkTheme ? "#1e1e1e" : "#FFFFFF",
                              darkTheme ? "#F0F0F0" : "#000000",
                              darkTheme ? "#555555" : "#CCCCCC",
                              darkTheme ? "#2D2D2D" : "#E0E0E0",
                              darkTheme ? "#264F78" : "#0d6efd",
                              darkTheme ? "#2A2D2E" : "#F0F0F0");
    customCompletionList->setStyleSheet(listStyle);

    QPushButton *removeCustomCompBtn = new QPushButton(tr("删除选中词条"));
    QString removeBtnStyle = QString(
                                 "QPushButton {"
                                 "    border: none;"
                                 "    border-radius: 6px;"
                                 "    padding: 6px 16px;"
                                 "    font-weight: bold;"
                                 "    min-width: 70px;"
                                 "    background-color: %1;"
                                 "    color: white;"
                                 "}"
                                 "QPushButton:hover { background-color: %2; }"
                                 "QPushButton:pressed { background-color: %3; }"
                             ).arg(darkTheme ? "#dc3545" : "#dc3545",
                                   darkTheme ? "#c82333" : "#c82333",
                                   darkTheme ? "#bd2130" : "#bd2130");
    removeCustomCompBtn->setStyleSheet(removeBtnStyle);

    customCompLayout->addWidget(customCompInfo);
    customCompLayout->addLayout(customInputLayout);
    customCompLayout->addWidget(customCompletionList);
    customCompLayout->addWidget(removeCustomCompBtn);

    customCompPageLayout->addWidget(customCompGroup);
    contentStack->addWidget(customCompPage);

    connect(addCustomCompBtn, &QPushButton::clicked, this, &SettingsDialog::addCustomCompletion);
    connect(newCompletionEdit, &QLineEdit::returnPressed, this, &SettingsDialog::addCustomCompletion);
    connect(removeCustomCompBtn, &QPushButton::clicked, this, &SettingsDialog::removeCustomCompletion);

    loadCustomCompletions();

    QWidget *buttonBar = new QWidget;
    QHBoxLayout *buttonBarLayout = new QHBoxLayout(buttonBar);
    buttonBarLayout->setContentsMargins(16, 12, 16, 12);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    okButton->setText(tr("保存设置"));
    cancelButton->setText(tr("取消"));

    int buttonHeight = 28;
    int borderRadius = 8;
    QString baseStyle = QString(
                            "QPushButton {"
                            "    border: none;"
                            "    border-radius: %1px;"
                            "    padding: 4px 12px;"
                            "    font-weight: bold;"
                            "    min-width: 80px;"
                            "    height: %2px;"
                            "}"
                        ).arg(borderRadius).arg(buttonHeight);

    if (darkTheme)
    {
        okButton->setStyleSheet(baseStyle +
                                "QPushButton {"
                                "    background-color: #0d6efd;"
                                "    color: white;"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: #0b5ed7;"
                                "}"
                                "QPushButton:pressed {"
                                "    background-color: #0a58ca;"
                                "}"
                               );
        cancelButton->setStyleSheet(baseStyle +
                                    "QPushButton {"
                                    "    background-color: #6c757d;"
                                    "    color: white;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #5c636a;"
                                    "}"
                                    "QPushButton:pressed {"
                                    "    background-color: #565e64;"
                                    "}"
                                   );
    }
    else
    {
        okButton->setStyleSheet(baseStyle +
                                "QPushButton {"
                                "    background-color: #0d6efd;"
                                "    color: white;"
                                "}"
                                "QPushButton:hover {"
                                "    background-color: #0b5ed7;"
                                "}"
                                "QPushButton:pressed {"
                                "    background-color: #0a58ca;"
                                "}"
                               );
        cancelButton->setStyleSheet(baseStyle +
                                    "QPushButton {"
                                    "    background-color: #f8f9fa;"
                                    "    color: #212529;"
                                    "    border: 1px solid #ced4da;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #e2e6ea;"
                                    "}"
                                    "QPushButton:pressed {"
                                    "    background-color: #dae0e5;"
                                    "}"
                                   );
    }

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this]()
    {
        qDebug() << "[SettingsDialog] 即将保存：字体 ="
                 << fontCombo->currentFont().family() << "，字号 =" << fontSizeSpin->value();
        QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
        settings.setValue("enableSplash", splashCheck->isChecked());
        settings.setValue("enableWelcome", welcomeCheck->isChecked());
        settings.setValue("showIndentGuides", showIndentGuidesCheck->isChecked());
        saveCustomCompletions();
        accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    buttonBarLayout->addStretch();
    buttonBarLayout->addWidget(buttonBox);

    rightLayout->addWidget(contentStack, 1);
    rightLayout->addWidget(buttonBar);

    mainLayout->addWidget(navList);
    mainLayout->addWidget(rightPanel, 1);

    connect(navList, &QListWidget::currentRowChanged, contentStack, &QStackedWidget::setCurrentIndex);

    setLayout(mainLayout);
}

bool SettingsDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == newCompletionEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            addCustomCompletion();
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}

bool SettingsDialog::getCodeCompletion() const
{
    return codeCompletionCheck->isChecked();
}

bool SettingsDialog::getCodeBeautify() const
{
    return codeBeautifyCheck->isChecked();
}

bool SettingsDialog::getShowIndentGuides() const
{
    return showIndentGuidesCheck->isChecked();
}

void SettingsDialog::loadCustomCompletions()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    QStringList items = settings.value("CustomCompletions", QStringList()).toStringList();
    customCompletionList->clear();
    customCompletionList->addItems(items);
}

void SettingsDialog::saveCustomCompletions()
{
    QStringList items;
    for(int i = 0; i < customCompletionList->count(); ++i)
    {
        items.append(customCompletionList->item(i)->text());
    }
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    settings.setValue("CustomCompletions", items);
}

void SettingsDialog::addCustomCompletion()
{
    QString text = newCompletionEdit->text().trimmed();
    if (text.isEmpty())
    {
        return;
    }

    QList<QListWidgetItem*> found = customCompletionList->findItems(text, Qt::MatchExactly);
    if (found.isEmpty())
    {
        customCompletionList->addItem(text);
        newCompletionEdit->clear();
        customCompletionList->scrollToBottom();
    }
    else
    {
        QMessageBox::warning(this, tr("提示"), tr("该补全词条已存在！"));
    }
}

void SettingsDialog::removeCustomCompletion()
{
    QList<QListWidgetItem*> selected = customCompletionList->selectedItems();
    for (QListWidgetItem* item : selected)
    {
        delete customCompletionList->takeItem(customCompletionList->row(item));
    }
}

QFont SettingsDialog::getEditorFont() const
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeSpin->value());
    qDebug() << "[SettingsDialog] getEditorFont 返回:" << font.family() << font.pointSize();
    return font;
}

int SettingsDialog::getEditorFontSize() const
{
    return fontSizeSpin->value();
}

QString SettingsDialog::findCompilerInPath(const QString &basePath)
{
    QDir dir(basePath);
    if (!dir.exists())
    {
        return QString();
    }

    QDirIterator it(basePath, QStringList() << "g++.exe" << "g++",
                    QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString compilerPath = it.next();
        if (compilerPath.contains("mingw", Qt::CaseInsensitive) ||
                compilerPath.contains("gcc", Qt::CaseInsensitive))
        {
            return QDir::toNativeSeparators(compilerPath);
        }
    }
    return QString();
}

QString SettingsDialog::findDebuggerFromCompilerPath(const QString &compilerPath)
{
    if (compilerPath.isEmpty())
    {
        return QString();
    }

    QFileInfo compilerInfo(compilerPath);
    QString compilerDir = compilerInfo.absolutePath();

    QString gdbPath = compilerDir + "/gdb.exe";
    if (QFile::exists(gdbPath))
    {
        return QDir::toNativeSeparators(gdbPath);
    }

    gdbPath = compilerDir + "/gdb";
    if (QFile::exists(gdbPath))
    {
        return QDir::toNativeSeparators(gdbPath);
    }

    if (compilerDir.endsWith("/bin", Qt::CaseInsensitive) ||
            compilerDir.endsWith("\\bin", Qt::CaseInsensitive))
    {

    }
    else
    {
        gdbPath = compilerDir + "/bin/gdb.exe";
        if (QFile::exists(gdbPath))
        {
            return QDir::toNativeSeparators(gdbPath);
        }
        gdbPath = compilerDir + "/bin/gdb";
        if (QFile::exists(gdbPath))
        {
            return QDir::toNativeSeparators(gdbPath);
        }
    }

    QDir parentDir(compilerDir);
    if (parentDir.cdUp())
    {
        gdbPath = parentDir.absolutePath() + "/bin/gdb.exe";
        if (QFile::exists(gdbPath))
        {
            return QDir::toNativeSeparators(gdbPath);
        }
        gdbPath = parentDir.absolutePath() + "/bin/gdb";
        if (QFile::exists(gdbPath))
        {
            return QDir::toNativeSeparators(gdbPath);
        }
    }

    QDirIterator it(compilerDir, QStringList() << "gdb.exe" << "gdb",
                    QDir::Files, QDirIterator::Subdirectories);
    if (it.hasNext())
    {
        return QDir::toNativeSeparators(it.next());
    }

    int mingwIndex = compilerPath.indexOf("mingw", 0, Qt::CaseInsensitive);
    if (mingwIndex != -1)
    {
        int endIndex = compilerPath.indexOf('/', mingwIndex);
        if (endIndex == -1)
        {
            endIndex = compilerPath.indexOf('\\', mingwIndex);
        }
        if (endIndex != -1)
        {
            QString mingwRoot = compilerPath.left(endIndex);
            QDirIterator mingwIt(mingwRoot, QStringList() << "gdb.exe" << "gdb",
                                 QDir::Files, QDirIterator::Subdirectories);
            if (mingwIt.hasNext())
            {
                return QDir::toNativeSeparators(mingwIt.next());
            }
        }
    }

    return QString();
}

QString SettingsDialog::autoDetectDebuggerPath()
{
    QString detectedPath = findDebuggerFromCompilerPath(compilerPathEdit->text());
    if (!detectedPath.isEmpty())
    {
        return detectedPath;
    }

    QStringList commonPaths =
    {
        "C:/MinGW/bin/gdb.exe",
        "C:/mingw64/bin/gdb.exe",
        "C:/mingw32/bin/gdb.exe",
        "C:/msys64/mingw64/bin/gdb.exe",
        "C:/msys64/mingw32/bin/gdb.exe",
        "C:/msys64/usr/bin/gdb.exe",
        "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/gdb.exe",
        "C:/TDM-GCC-64/bin/gdb.exe",
        "C:/TDM-GCC-32/bin/gdb.exe",
        QCoreApplication::applicationDirPath() + "/mingw/bin/gdb.exe",
        QCoreApplication::applicationDirPath() + "/mingw64/bin/gdb.exe"
    };

    QString pathEnv = qEnvironmentVariable("PATH");
    QStringList pathDirs = pathEnv.split(';', Qt::SkipEmptyParts);
    for (const QString &dir : pathDirs)
    {
        QString gdbPath = dir + "/gdb.exe";
        if (!commonPaths.contains(gdbPath))
        {
            commonPaths.append(gdbPath);
        }
    }

    for (const QString &path : commonPaths)
    {
        if (QFile::exists(path))
        {
            return QDir::toNativeSeparators(path);
        }
    }

#ifdef Q_OS_WIN
    return "gdb.exe";
#else
    return "gdb";
#endif
}

void SettingsDialog::browseDebuggerPath()
{
    QString startDir = QDir::homePath();
    if (!compilerPathEdit->text().isEmpty())
    {
        QFileInfo compilerInfo(compilerPathEdit->text());
        if (compilerInfo.exists())
        {
            startDir = compilerInfo.absolutePath();
        }
    }

    QString path = QFileDialog::getOpenFileName(this, tr("选择调试器"),
                   startDir,
#ifdef Q_OS_WIN
                   tr("GDB调试器 (gdb.exe);;可执行文件 (*.exe);;所有文件 (*)"));
#else
                   tr("GDB调试器 (gdb);;所有文件 (*)"));
#endif
    if (!path.isEmpty())
    {
        debuggerPathEdit->setText(path);
    }
}

QString SettingsDialog::getCompilerPath() const
{
    return compilerPathEdit->text();
}

QString SettingsDialog::getDebuggerPath() const
{
    return debuggerPathEdit->text();
}

void SettingsDialog::onCompilerPathChanged(const QString &path)
{
    if (debuggerPathEdit->text().isEmpty() || !QFile::exists(debuggerPathEdit->text()))
    {
        QString detectedDebugger = findDebuggerFromCompilerPath(path);
        if (!detectedDebugger.isEmpty())
        {
            debuggerPathEdit->setText(detectedDebugger);
        }
    }
}

bool SettingsDialog::getAutoBrackets() const
{
    return autoBracketsCheck->isChecked();
}

bool SettingsDialog::getAutoQuotes() const
{
    return autoQuotesCheck->isChecked();
}

bool SettingsDialog::getAutoIndent() const
{
    return autoIndentCheck->isChecked();
}

int SettingsDialog::getIndentSize() const
{
    return indentSizeSpin->value();
}

bool SettingsDialog::getLineNumbers() const
{
    return lineNumbersCheck->isChecked();
}

bool SettingsDialog::getDarkTheme() const
{
    return darkThemeCheck->isChecked();
}

void SettingsDialog::browseCompilerPath()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择编译器"),
                   QDir::homePath(),
#ifdef Q_OS_WIN
                   tr("可执行文件 (*.exe);;所有文件 (*)"));
#else
                   tr("所有文件 (*)"));
#endif
    if (!path.isEmpty())
    {
        compilerPathEdit->setText(path);
    }
}

// 启动界面
class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    ~SplashScreen();

    void showWithAnimation();
    void hideWithAnimation();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onHideAnimationFinished();

signals:
    void hidden();

private:
    void setupUI();
    void setupAnimations();

    QLabel *titleLabel;
    QLabel *versionLabel;
    QLabel *loadingLabel;
    QProgressBar *progressBar;
    QVBoxLayout *mainLayout;

    QPropertyAnimation *showAnimation;
    QPropertyAnimation *hideAnimation;
    QGraphicsOpacityEffect *opacityEffect;

    QTimer *progressTimer;
    int progressValue;
};

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent), progressValue(0)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(600, 400);

    setupUI();
    setupAnimations();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move(screenGeometry.center() - rect().center());
}

SplashScreen::~SplashScreen()
{
    if (progressTimer)
    {
        progressTimer->stop();
        delete progressTimer;
    }
}

void SplashScreen::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    titleLabel = new QLabel("Compiler IDE", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 32, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2E86AB; background: transparent;");

    versionLabel = new QLabel("Version " + IDE_VERSION, this);
    versionLabel->setAlignment(Qt::AlignCenter);
    QFont versionFont("Arial", 14, QFont::Normal);
    versionLabel->setFont(versionFont);
    versionLabel->setStyleSheet("color: #A23B72; background: transparent;");

    loadingLabel = new QLabel("正在启动...", this);
    loadingLabel->setAlignment(Qt::AlignCenter);
    QFont loadingFont("Arial", 10, QFont::Normal);
    loadingLabel->setFont(loadingFont);
    loadingLabel->setStyleSheet("color: #F18F01; background: transparent;");

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(6);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: none;"
        "    background: rgba(255, 255, 255, 30);"
        "    border-radius: 3px;"
        "}"
        "QProgressBar::chunk {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 #2E86AB, stop:0.5 #A23B72, stop:1 #F18F01);"
        "    border-radius: 3px;"
        "}"
    );

    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(versionLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(loadingLabel);
    mainLayout->addWidget(progressBar);

    opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.0);
    setGraphicsEffect(opacityEffect);
}

void SplashScreen::setupAnimations()
{
    showAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    showAnimation->setDuration(800);
    showAnimation->setStartValue(0.0);
    showAnimation->setEndValue(1.0);
    showAnimation->setEasingCurve(QEasingCurve::OutCubic);

    hideAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    hideAnimation->setDuration(600);
    hideAnimation->setStartValue(1.0);
    hideAnimation->setEndValue(0.0);
    hideAnimation->setEasingCurve(QEasingCurve::InCubic);
    connect(hideAnimation, &QPropertyAnimation::finished, this, &SplashScreen::onHideAnimationFinished);

    progressTimer = new QTimer(this);
    connect(progressTimer, &QTimer::timeout, this, [this]()
    {
        progressValue += 2;
        progressBar->setValue(progressValue);

        if (progressValue < 25)
        {
            loadingLabel->setText("正在初始化核心模块...");
        }
        else if (progressValue < 50)
        {
            loadingLabel->setText("正在加载编辑器组件...");
        }
        else if (progressValue < 75)
        {
            loadingLabel->setText("正在配置编译器设置...");
        }
        else
        {
            loadingLabel->setText("正在准备用户界面...");
        }

        if (progressValue >= 100)
        {
            progressTimer->stop();
            hideWithAnimation();
        }
    });
}

void SplashScreen::showWithAnimation()
{
    show();
    showAnimation->start();
    progressTimer->start(30);
}

void SplashScreen::hideWithAnimation()
{
    hideAnimation->start();
}

void SplashScreen::onHideAnimationFinished()
{
    hide();
    emit hidden();
}

void SplashScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(30, 30, 40));
    gradient.setColorAt(0.5, QColor(40, 40, 55));
    gradient.setColorAt(1, QColor(25, 25, 35));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 20, 20);

    QPixmap logo(":/icons/CompilerIDE_logo.png");
    if (!logo.isNull())
    {
        logo = logo.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(width()/2 - logo.width()/2, 60, logo);
    }

    QPen borderPen(QColor(100, 100, 150, 100));
    borderPen.setWidth(2);
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 20, 20);

    painter.setPen(QPen(QColor(46, 134, 171, 50), 3));
    painter.drawLine(50, height() - 50, width() - 50, height() - 50);
}

// 欢迎对话框
class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

    QString getSelectedFile() const
    {
        return selectedFile;
    }

private slots:
    void onNewProjectClicked();
    void onOpenProjectClicked();
    void onOpenFileClicked();
    void onRecentFileClicked(QListWidgetItem *item);
    void onThemeToggled(bool dark);
    void clearRecentFiles();

private:
    void setupUI();
    void setupConnections();
    void loadRecentFiles();
    void saveRecentFiles();
    void addToRecentFiles(const QString &filePath);

    QListWidget *recentList;
    QPushButton *newFileBtn;
    QPushButton *openFileBtn;
    QPushButton *quickStartBtn;
    QPushButton *clearRecentBtn;
    QCheckBox *darkThemeCheck;

    QStringList recentFiles;
    QString selectedFile;
};

WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent), selectedFile("")
{
    setWindowTitle("欢迎使用 Compiler IDE " + IDE_VERSION);
    setFixedSize(800, 600);
    setModal(true);

    setupUI();
    setupConnections();
    loadRecentFiles();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move(screenGeometry.center() - rect().center());
}

WelcomeDialog::~WelcomeDialog()
{
    saveRecentFiles();
}

void WelcomeDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(120);
    headerWidget->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "stop:0 #2E86AB, stop:0.3 #A23B72, stop:0.6 #F18F01, stop:1 #C73E1D);"
        "border-top-left-radius: 10px;"
        "border-top-right-radius: 10px;"
    );

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(40, 0, 40, 0);

    QLabel *titleLabel = new QLabel("Compiler IDE " + IDE_VERSION, headerWidget);
    titleLabel->setStyleSheet("color: white; font-size: 28px; font-weight: bold; background: transparent;");

    QLabel *subtitleLabel = new QLabel("现代化 C++ 开发环境", headerWidget);
    subtitleLabel->setStyleSheet("color: rgba(255,255,255,0.8); font-size: 14px; background: transparent;");

    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(subtitleLabel);
    textLayout->setSpacing(5);

    headerLayout->addLayout(textLayout);
    headerLayout->addStretch();

    QWidget *contentWidget = new QWidget(this);
    contentWidget->setStyleSheet("background: #2D3047; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px;");

    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(40);

    QWidget *quickActionsWidget = new QWidget(contentWidget);
    quickActionsWidget->setFixedWidth(300);

    QVBoxLayout *actionsLayout = new QVBoxLayout(quickActionsWidget);
    actionsLayout->setSpacing(15);

    QLabel *actionsTitle = new QLabel("快速开始", quickActionsWidget);
    actionsTitle->setStyleSheet("color: #E0E0E0; font-size: 20px; font-weight: bold; margin-bottom: 10px;");

    newFileBtn = new QPushButton("新建文件", quickActionsWidget);
    newFileBtn->setFixedHeight(50);
    newFileBtn->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2E86AB, stop:1 #A23B72);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3B9BC8, stop:1 #B84F8A);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1E5A7A, stop:1 #7C2C5A);"
        "}"
    );

    openFileBtn = new QPushButton("打开文件", quickActionsWidget);
    openFileBtn->setFixedHeight(50);
    openFileBtn->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #F18F01, stop:1 #C73E1D);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FFA726, stop:1 #E64A19);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #B26A00, stop:1 #9C2700);"
        "}"
    );

    quickStartBtn = new QPushButton("快速开始", quickActionsWidget);
    quickStartBtn->setFixedHeight(50);
    quickStartBtn->setStyleSheet(
        "QPushButton {"
        "    background: #404764;"
        "    color: white;"
        "    border: 2px solid #5A5F7A;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background: #4A5070;"
        "    border-color: #6B7190;"
        "}"
        "QPushButton:pressed {"
        "    background: #363A52;"
        "}"
    );

    actionsLayout->addWidget(actionsTitle);
    actionsLayout->addWidget(newFileBtn);
    actionsLayout->addWidget(openFileBtn);
    actionsLayout->addWidget(quickStartBtn);
    actionsLayout->addStretch();

    QWidget *recentWidget = new QWidget(contentWidget);
    QVBoxLayout *recentLayout = new QVBoxLayout(recentWidget);
    recentLayout->setSpacing(15);

    QHBoxLayout *recentHeaderLayout = new QHBoxLayout();
    QLabel *recentTitle = new QLabel("最近文件", recentWidget);
    recentTitle->setStyleSheet("color: #E0E0E0; font-size: 20px; font-weight: bold;");

    clearRecentBtn = new QPushButton("清空列表", recentWidget);
    clearRecentBtn->setFixedSize(80, 25);
    clearRecentBtn->setStyleSheet(
        "QPushButton {"
        "    background: transparent;"
        "    color: #F18F01;"
        "    border: 1px solid #F18F01;"
        "    border-radius: 4px;"
        "    font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "    background: rgba(241, 143, 1, 0.2);"
        "}"
        "QPushButton:pressed {"
        "    background: rgba(241, 143, 1, 0.3);"
        "}"
    );

    recentHeaderLayout->addWidget(recentTitle);
    recentHeaderLayout->addStretch();
    recentHeaderLayout->addWidget(clearRecentBtn);

    recentList = new QListWidget(recentWidget);
    recentList->setStyleSheet(
        "QListWidget {"
        "    background: #404764;"
        "    border: 1px solid #5A5F7A;"
        "    border-radius: 8px;"
        "    color: #E0E0E0;"
        "    font-size: 12px;"
        "    outline: none;"
        "}"
        "QListWidget::item {"
        "    border-bottom: 1px solid #5A5F7A;"
        "    padding: 12px;"
        "}"
        "QListWidget::item:selected {"
        "    background: #2E86AB;"
        "    color: white;"
        "}"
        "QListWidget::item:hover {"
        "    background: #4A5070;"
        "}"
        "QListWidget::item:last {"
        "    border-bottom: none;"
        "}"
    );

    recentLayout->addLayout(recentHeaderLayout);
    recentLayout->addWidget(recentList);

    contentLayout->addWidget(quickActionsWidget);
    contentLayout->addWidget(recentWidget);

    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(contentWidget);
}

void WelcomeDialog::setupConnections()
{
    connect(newFileBtn, &QPushButton::clicked, this, &WelcomeDialog::onNewProjectClicked);
    connect(openFileBtn, &QPushButton::clicked, this, &WelcomeDialog::onOpenProjectClicked);
    connect(quickStartBtn, &QPushButton::clicked, this, &WelcomeDialog::onOpenFileClicked);
    connect(recentList, &QListWidget::itemClicked, this, &WelcomeDialog::onRecentFileClicked);
    connect(clearRecentBtn, &QPushButton::clicked, this, &WelcomeDialog::clearRecentFiles);
}

void WelcomeDialog::loadRecentFiles()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    recentFiles = settings.value("recentFiles").toStringList();

    recentList->clear();

    for (const QString &filePath : recentFiles)
    {
        if (QFile::exists(filePath))
        {
            QFileInfo fileInfo(filePath);
            QString displayText = QString("%1\n%2")
                                  .arg(fileInfo.fileName())
                                  .arg(fileInfo.absolutePath());

            QListWidgetItem *item = new QListWidgetItem(displayText);
            item->setData(Qt::UserRole, filePath);

            if (fileInfo.suffix().toLower() == "cpp" || fileInfo.suffix().toLower() == "cc")
            {
                item->setForeground(QColor("#2E86AB"));
            }
            else if (fileInfo.suffix().toLower() == "h" || fileInfo.suffix().toLower() == "hpp")
            {
                item->setForeground(QColor("#A23B72"));
            }
            else
            {
                item->setForeground(QColor("#F18F01"));
            }

            recentList->addItem(item);
        }
    }

    if (recentList->count() == 0)
    {
        QListWidgetItem *item = new QListWidgetItem("暂无最近打开的文件");
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        item->setForeground(QColor("#666666"));
        recentList->addItem(item);
    }
}

void WelcomeDialog::saveRecentFiles()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    settings.setValue("recentFiles", recentFiles);
}

void WelcomeDialog::addToRecentFiles(const QString &filePath)
{
    if (filePath.isEmpty() || !QFile::exists(filePath))
    {
        return;
    }

    recentFiles.removeAll(filePath);
    recentFiles.prepend(filePath);

    while (recentFiles.size() > 10)
    {
        recentFiles.removeLast();
    }

    saveRecentFiles();
}

void WelcomeDialog::onNewProjectClicked()
{
    selectedFile = "__NEW_FILE__";
    accept();
}

void WelcomeDialog::onOpenProjectClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "",
                       "C++文件 (*.cpp *.cc *.cxx *.c++ *.h *.hpp *.hh *.hxx *.h++);;PDF文件 (*.pdf);;所有文件 (*)");
    if (!fileName.isEmpty())
    {
        selectedFile = fileName;
        addToRecentFiles(selectedFile);
        accept();
    }
}

void WelcomeDialog::onOpenFileClicked()
{
    selectedFile = "";
    accept();
}

void WelcomeDialog::onRecentFileClicked(QListWidgetItem *item)
{
    if (item && (item->flags() & Qt::ItemIsEnabled))
    {
        QString filePath = item->data(Qt::UserRole).toString();
        if (QFile::exists(filePath))
        {
            selectedFile = filePath;
            recentFiles.removeAll(filePath);
            recentFiles.prepend(filePath);
            accept();
        }
        else
        {
            QMessageBox::warning(this, "文件不存在", "所选文件不存在，已从最近文件列表中移除。");
            recentFiles.removeAll(filePath);
            delete recentList->takeItem(recentList->row(item));
        }
    }
}

void WelcomeDialog::onThemeToggled(bool dark)
{

}

void WelcomeDialog::clearRecentFiles()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认清空", "确定要清空最近文件列表吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        recentFiles.clear();
        saveRecentFiles();
        loadRecentFiles();
    }
}

// 代码高亮
class CppHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit CppHighlighter(QTextDocument *parent = nullptr);

    void setDarkTheme(bool dark);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat controlFlowFormat;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat typeFormat;
    QTextCharFormat macroFormat;
    QTextCharFormat parenthesisFormat;
    QTextCharFormat functionNameFormat;

    bool isDarkTheme;

    void setupColors();
    void rebuildHighlightingRules();
};

CppHighlighter::CppHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), isDarkTheme(true)
{
    setupColors();

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void CppHighlighter::highlightBlock(const QString &text)
{
    QVector<QPair<int, int>> stringRanges;
    QRegularExpressionMatchIterator stringIt =
        QRegularExpression("\"(?:[^\"\\\\]|\\\\.)*\"").globalMatch(text);
    while (stringIt.hasNext())
    {
        QRegularExpressionMatch match = stringIt.next();
        stringRanges.append(qMakePair(match.capturedStart(), match.capturedLength()));
    }
    QRegularExpressionMatchIterator charIt =
        QRegularExpression("'(?:[^'\\\\]|\\\\.)*'").globalMatch(text);
    while (charIt.hasNext())
    {
        QRegularExpressionMatch match = charIt.next();
        stringRanges.append(qMakePair(match.capturedStart(), match.capturedLength()));
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = text.indexOf(commentStartExpression);
    }

    QVector<QPair<int, int>> multiLineCommentRanges;

    int tempStartIndex = startIndex;
    while (tempStartIndex >= 0)
    {
        bool inString = false;
        for (const auto &range : stringRanges)
        {
            if (tempStartIndex >= range.first && tempStartIndex < range.first + range.second)
            {
                inString = true;
                break;
            }
        }
        if (inString)
        {
            tempStartIndex = text.indexOf(commentStartExpression, tempStartIndex + 2);
            continue;
        }

        QRegularExpressionMatch match = commentEndExpression.match(text, tempStartIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - tempStartIndex;
        }
        else
        {
            commentLength = endIndex - tempStartIndex + match.capturedLength();
        }
        multiLineCommentRanges.append(qMakePair(tempStartIndex, commentLength));
        tempStartIndex = text.indexOf(commentStartExpression, tempStartIndex + commentLength);
    }

    if (previousBlockState() == 1 && startIndex == -1)
    {
        setFormat(0, text.length(), multiLineCommentFormat);
        return;
    }

    QVector<QPair<int, int>> singleLineCommentRanges;
    {
        int searchFrom = 0;
        while (searchFrom < text.length() - 1)
        {
            int idx = text.indexOf("//", searchFrom);
            if (idx == -1)
            {
                break;
            }

            bool inString = false;
            for (const auto &range : stringRanges)
            {
                if (idx >= range.first && idx < range.first + range.second)
                {
                    inString = true;
                    searchFrom = range.first + range.second;
                    break;
                }
            }
            if (inString)
            {
                continue;
            }

            bool inMultiLineComment = false;
            for (const auto &range : multiLineCommentRanges)
            {
                if (idx >= range.first && idx < range.first + range.second)
                {
                    inMultiLineComment = true;
                    searchFrom = range.first + range.second;
                    break;
                }
            }
            if (inMultiLineComment)
            {
                continue;
            }

            singleLineCommentRanges.append(qMakePair(idx, text.length() - idx));
            break;
        }
    }

    auto isInCommentOrString = [&](int pos, int length) -> bool
    {
        for (const auto &range : multiLineCommentRanges)
        {
            if (pos >= range.first && pos < range.first + range.second)
            {
                return true;
            }
        }
        for (const auto &range : singleLineCommentRanges)
        {
            if (pos >= range.first && pos < range.first + range.second)
            {
                return true;
            }
        }
        return false;
    };

    for (const HighlightingRule &rule : highlightingRules)
    {
        if (rule.format == singleLineCommentFormat)
        {
            continue;
        }

        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            int start = match.capturedStart();
            int length = match.capturedLength();

            if (isInCommentOrString(start, length))
            {
                continue;
            }

            if (rule.format == functionNameFormat && match.lastCapturedIndex() >= 2)
            {
                int funcStart = match.capturedStart(2);
                if (!isInCommentOrString(funcStart, match.capturedLength(2)))
                {
                    setFormat(funcStart, match.capturedLength(2), rule.format);
                }
            }
            else if (rule.pattern.pattern().contains("include") && match.lastCapturedIndex() >= 1)
            {
                QString fullMatch = match.captured(0);
                int bracketStart = fullMatch.indexOf(QRegularExpression("[<\"]"));
                if (bracketStart != -1)
                {
                    int actualStart = start + bracketStart;
                    int bracketLength = fullMatch.length() - bracketStart;
                    if (!isInCommentOrString(actualStart, bracketLength))
                    {
                        setFormat(actualStart, bracketLength, rule.format);
                    }
                }
            }
            else
            {
                setFormat(start, length, rule.format);
            }
        }
    }

    for (const auto &range : stringRanges)
    {
        bool inComment = false;
        for (const auto &commentRange : multiLineCommentRanges)
        {
            if (range.first >= commentRange.first &&
                    range.first < commentRange.first + commentRange.second)
            {
                inComment = true;
                break;
            }
        }
        for (const auto &commentRange : singleLineCommentRanges)
        {
            if (range.first >= commentRange.first &&
                    range.first < commentRange.first + commentRange.second)
            {
                inComment = true;
                break;
            }
        }
        if (!inComment)
        {
            setFormat(range.first, range.second, quotationFormat);
        }
    }

    for (const auto &range : singleLineCommentRanges)
    {
        bool inMultiLine = false;
        for (const auto &mlRange : multiLineCommentRanges)
        {
            if (range.first >= mlRange.first &&
                    range.first < mlRange.first + mlRange.second)
            {
                inMultiLine = true;
                break;
            }
        }
        if (!inMultiLine)
        {
            setFormat(range.first, range.second, singleLineCommentFormat);
        }
    }

    for (const auto &range : multiLineCommentRanges)
    {
        setFormat(range.first, range.second, multiLineCommentFormat);
    }
}

void CppHighlighter::rebuildHighlightingRules()
{
    highlightingRules.clear();

    // C++关键字
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"
                    << "\\busing\\b"
                    << "\\bnew\\b" << "\\bdelete\\b" << "\\bthis\\b"
                    << "\\band\\b" << "\\bnot\\b" << "\\bor\\b"
                    << "\\bxor\\b" << "\\band_eq\\b" << "\\bor_eq\\b"
                    << "\\bxor_eq\\b" << "\\bnot_eq\\b" << "\\bbitand\\b"
                    << "\\bbitor\\b" << "\\bcompl\\b" << "\\basm\\b"
                    << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b"
                    << "\\btrue\\b" << "\\bmutable\\b" << "\\btypename\\b"
                    << "\\btypeid\\b" << "\\bwchar_t\\b" << "\\bstatic_cast\\b"
                    << "\\bconst_cast\\b" << "\\bdynamic_cast\\b"
                    << "\\breinterpret_cast\\b" << "\\bnullptr\\b"
                    << "\\bdecltype\\b" << "\\bthread_local\\b"
                    << "\\balignas\\b" << "\\balignof\\b" << "\\bstatic_assert\\b"
                    << "\\bnoexcept\\b" << "\\boverride\\b" << "\\bfinal\\b"
                    << "\\bconcept\\b" << "\\brequires\\b" << "\\bconsteval\\b"
                    << "\\bconstinit\\b" << "\\bconstexpr\\b" << "\\bco_await\\b"
                    << "\\bco_yield\\b" << "\\bco_return\\b" << "\\bimport\\b"
                    << "\\bmodule\\b" << "\\breflexpr\\b" << "\\bauto\\b";

    for (const QString &pattern : keywordPatterns)
    {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // 类名
    HighlightingRule classRule;
    classRule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    classRule.format = classFormat;
    highlightingRules.append(classRule);

    // 函数
    HighlightingRule functionRule;
    functionRule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\s*\\()");
    functionRule.format = functionFormat;
    highlightingRules.append(functionRule);

    // 函数名
    HighlightingRule functionNameRule;
    functionNameRule.pattern = QRegularExpression("\\b(void|int|char|double|float|bool|short|long|unsigned|signed|auto)\\s+([A-Za-z_][A-Za-z0-9_]*)(?=\\s*\\()");
    functionNameRule.format = functionNameFormat;
    highlightingRules.append(functionNameRule);

    // 预处理器
    HighlightingRule preprocessorRule;
    preprocessorRule.pattern = QRegularExpression("^\\s*#\\s*[a-zA-Z_][a-zA-Z0-9_]*");
    preprocessorRule.format = preprocessorFormat;
    highlightingRules.append(preprocessorRule);

    HighlightingRule includeHeaderRule;
    includeHeaderRule.pattern = QRegularExpression("^\\s*#\\s*include\\s*[<\"]([^>\"]+)[>\"]");
    // 头文件
    QTextCharFormat includeHeaderFormat;
    if (isDarkTheme)
    {
        includeHeaderFormat.setForeground(QColor(255, 165, 0));
    }
    else
    {
        includeHeaderFormat.setForeground(QColor(255, 140, 0));
    }
    includeHeaderRule.format = includeHeaderFormat;
    highlightingRules.append(includeHeaderRule);

    // 字符串
    HighlightingRule quotationRule;
    quotationRule.pattern = QRegularExpression("\".*?\"");
    quotationRule.format = quotationFormat;
    highlightingRules.append(quotationRule);

    // 字符
    HighlightingRule charRule;
    charRule.pattern = QRegularExpression("'.*?'");
    charRule.format = quotationFormat;
    highlightingRules.append(charRule);

    // 单行注释
    HighlightingRule singleLineCommentRule;
    singleLineCommentRule.pattern = QRegularExpression("//[^\\n]*");
    singleLineCommentRule.format = singleLineCommentFormat;
    highlightingRules.append(singleLineCommentRule);

    HighlightingRule numberRule;
    numberRule.pattern = QRegularExpression("\\b[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?(([uU](ll|LL|l|L)?)|((ll|LL|l|L)[uU]?)|[fF])?");
    numberRule.format = numberFormat;
    highlightingRules.append(numberRule);

    HighlightingRule hexNumberRule;
    hexNumberRule.pattern = QRegularExpression("\\b0[xX][0-9a-fA-F]+([uUlL]{1,3})?\\b");
    hexNumberRule.format = numberFormat;
    highlightingRules.append(hexNumberRule);

    HighlightingRule octNumberRule;
    octNumberRule.pattern = QRegularExpression("\\b0[0-7]+([uUlL]{1,3})?\\b");
    octNumberRule.format = numberFormat;
    highlightingRules.append(octNumberRule);

    // 类型
    HighlightingRule typeRule;
    typeRule.pattern = QRegularExpression("\\b(int|float|double|char|void|bool|short|long|unsigned|signed)\\b");
    typeRule.format = typeFormat;
    highlightingRules.append(typeRule);

    // 括号
    HighlightingRule leftParenthesisRule;
    leftParenthesisRule.pattern = QRegularExpression("\\(");
    leftParenthesisRule.format = parenthesisFormat;
    highlightingRules.append(leftParenthesisRule);

    HighlightingRule rightParenthesisRule;
    rightParenthesisRule.pattern = QRegularExpression("\\)");
    rightParenthesisRule.format = parenthesisFormat;
    highlightingRules.append(rightParenthesisRule);

    QStringList controlFlowKeywords;
    controlFlowKeywords << "\\bif\\b" << "\\belse\\b" << "\\bfor\\b"
                        << "\\bwhile\\b" << "\\bdo\\b" << "\\bswitch\\b"
                        << "\\bcase\\b" << "\\bdefault\\b" << "\\bbreak\\b"
                        << "\\bcontinue\\b" << "\\breturn\\b" << "\\bgoto\\b"
                        << "\\btry\\b" << "\\bcatch\\b" << "\\bthrow\\b";

    for (const QString &pattern : controlFlowKeywords)
    {
        HighlightingRule rule;
        rule.pattern = QRegularExpression(pattern);
        rule.format = controlFlowFormat;
        highlightingRules.append(rule);
    }

    HighlightingRule elseIfRule;
    elseIfRule.pattern = QRegularExpression("\\belse\\s+if\\b");
    elseIfRule.format = controlFlowFormat;
    highlightingRules.append(elseIfRule);
}

void CppHighlighter::setDarkTheme(bool dark)
{
    isDarkTheme = dark;
    setupColors();
    rehighlight();
}

void CppHighlighter::setupColors()
{
    if (isDarkTheme)
    {
        keywordFormat.setForeground(QColor(86, 156, 214));
        keywordFormat.setFontWeight(QFont::Bold);
        classFormat.setForeground(QColor(184, 115, 212));
        classFormat.setFontWeight(QFont::Bold);
        singleLineCommentFormat.setForeground(QColor(106, 153, 85));
        multiLineCommentFormat.setForeground(QColor(106, 153, 85));
        quotationFormat.setForeground(QColor(206, 145, 120));
        functionFormat.setForeground(QColor(78, 201, 176));
        preprocessorFormat.setForeground(QColor(86, 156, 214));
        numberFormat.setForeground(QColor(181, 206, 168));
        typeFormat.setForeground(QColor(78, 201, 176));
        typeFormat.setFontWeight(QFont::Bold);
        macroFormat.setForeground(QColor(200, 100, 200));
        parenthesisFormat.setForeground(QColor(255, 215, 0));
        functionNameFormat.setForeground(QColor(255, 255, 160));
        functionNameFormat.setFontWeight(QFont::Normal);
        controlFlowFormat.setForeground(QColor(197, 134, 192));
        controlFlowFormat.setFontWeight(QFont::Normal);
    }
    else
    {
        keywordFormat.setForeground(QColor(0, 0, 180));
        keywordFormat.setFontWeight(QFont::Bold);
        classFormat.setForeground(QColor(128, 0, 128));
        classFormat.setFontWeight(QFont::Bold);
        singleLineCommentFormat.setForeground(QColor(0, 128, 0));
        multiLineCommentFormat.setForeground(QColor(0, 128, 0));
        quotationFormat.setForeground(QColor(163, 21, 21));
        functionFormat.setForeground(QColor(120, 94, 50));
        preprocessorFormat.setForeground(QColor(128, 64, 0));
        numberFormat.setForeground(QColor(9, 134, 88));
        typeFormat.setForeground(QColor(0, 128, 128));
        typeFormat.setFontWeight(QFont::Bold);
        macroFormat.setForeground(QColor(128, 0, 128));
        parenthesisFormat.setForeground(QColor(100, 80, 0));
        functionNameFormat.setForeground(QColor(116, 83, 37));
        functionNameFormat.setFontWeight(QFont::Normal);
        controlFlowFormat.setForeground(QColor(175, 0, 219));
        controlFlowFormat.setFontWeight(QFont::Normal);
    }
    rebuildHighlightingRules();
}

class CodeCompleter;

// 代码编辑器
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setAutoBracketsEnabled(bool enabled);
    void setAutoQuotesEnabled(bool enabled);
    void setAutoIndentEnabled(bool enabled);
    void setIndentSize(int size);
    void setCodeFoldingEnabled(bool enabled);
    void setLineNumbersEnabled(bool enabled);
    void setDarkThemeEnabled(bool enabled);

    void setCompleter(CodeCompleter *comp);
    void setCompletionEnabled(bool enabled)
    {
        completionEnabled = enabled;
    }
    bool isCompletionEnabled() const
    {
        return completionEnabled;
    }

    void setShowIndentGuides(bool show)
    {
        showIndentGuides = show;
        viewport()->update();
    }
    bool getShowIndentGuides() const
    {
        return showIndentGuides;
    }

    void setCompetitionModeSettings(bool disableCodeCompletion,
                                    bool disableAutoBrackets,
                                    bool disableAutoQuotes,
                                    bool disableAutoIndent)
    {
        competitionMode = disableCodeCompletion;
        comp_disableAutoBrackets = disableAutoBrackets;
        comp_disableAutoQuotes = disableAutoQuotes;
        comp_disableAutoIndent = disableAutoIndent;
    }

    void setCompetitionMode(bool enabled)
    {
        competitionMode = enabled;
    }
    bool isCompetitionMode() const
    {
        return competitionMode;
    }

    void disableAllCompletion()
    {
        completionEnabled = false;
        hideCompletion();
        competitionMode = true;
    }

    void enableAllCompletion()
    {
        completionEnabled = true;
        competitionMode = false;
    }

    void hideCompletion();
    void closeCompletionPopup()
    {
        hideCompletion();
    }
    void setCodeBeautifyEnabled(bool enabled);
    bool isCodeBeautifyEnabled() const
    {
        return codeBeautify;
    }
    void beautifyCurrentLine();
    QString beautifyCode(const QString &code, bool isControlStructure);

    void updateLineNumberAreaWidth(int newBlockCount);

    void setBreakpoints(const QSet<int> &lines);
    void addBreakpoint(int line);
    void removeBreakpoint(int line);
    void toggleBreakpoint(int line);
    bool hasBreakpoint(int line) const;
    QSet<int> getBreakpoints() const
    {
        return breakpointLines;
    }
    void setCurrentDebugLine(int line);
    void clearCurrentDebugLine();

    void setEditorFont(const QFont &font);
    void setErrorLine(int line);
    void setWarningLine(int line);
    void clearErrorWarningLines();

    void clearErrorLine()
    {
        errorLine = -1;
        viewport()->update();
    }

    void clearWarningLine()
    {
        warningLine = -1;
        viewport()->update();
    }

private:
    CodeCompleter *completer;
    QListView *completionList;
    QWidget *completionPopup;
    QString completionPrefix;
    bool completionEnabled;
    bool ignoreNextKeyPress;

    void showCompletion(const QStringList &completions);
    void insertCompletion(const QString &completion);
    QString getWordUnderCursor() const;
    bool eventFilter(QObject *obj, QEvent *event) override;

    bool competitionMode;
    bool showIndentGuides = true;
    QList<QTextEdit::ExtraSelection> matchSelections;
    QString lastSelectedText;

    bool codeBeautify;
    bool isInCommentOrStringForBeautify(const QString &text, int position) const;
    bool isControlStructureLine(const QString &line) const;

    int errorLine = -1;
    int warningLine = -1;
    QColor errorBgColor;
    QColor warningBgColor;

    bool isPositionInCommentOrString() const;
    QSet<int> breakpointLines;
    int currentDebugLine;

    void drawBreakpointMarker(QPainter &painter, const QRect &rect, bool isCurrentLine);
    void updateCompletionListStyle();
    class LineNumberArea : public QWidget
    {
    public:
        LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
        {
            setMouseTracking(true);
            setAttribute(Qt::WA_Hover, true);
            setCursor(Qt::PointingHandCursor);
        }

        QSize sizeHint() const override
        {
            return QSize(codeEditor->lineNumberAreaWidth(), 0);
        }

    protected:
        void paintEvent(QPaintEvent *event) override
        {
            codeEditor->lineNumberAreaPaintEvent(event);
        }

        void mousePressEvent(QMouseEvent *event) override
        {
            if (event->button() == Qt::LeftButton)
            {
                int breakpointAreaWidth = 20;

                if (event->position().x() < breakpointAreaWidth)
                {
                    int y = static_cast<int>(event->position().y());

                    QTextBlock block = codeEditor->firstVisibleBlock();
                    int top = qRound(codeEditor->blockBoundingGeometry(block).translated(codeEditor->contentOffset()).top());
                    int bottom = top + qRound(codeEditor->blockBoundingRect(block).height());

                    while (block.isValid())
                    {
                        if (block.isVisible() && y >= top && y < bottom)
                        {
                            int lineNumber = block.blockNumber() + 1;

                            if (codeEditor->breakpointLines.contains(lineNumber))
                            {
                                codeEditor->breakpointLines.remove(lineNumber);
                            }
                            else
                            {
                                codeEditor->breakpointLines.insert(lineNumber);
                            }

                            update();

                            qDebug() << "断点切换: 行" << lineNumber << "当前状态:" << codeEditor->breakpointLines.contains(lineNumber);

                            event->accept();
                            return;
                        }

                        block = block.next();
                        top = bottom;
                        bottom = top + qRound(codeEditor->blockBoundingRect(block).height());

                        if (top > height())
                        {
                            break;
                        }
                    }
                }
            }
            QWidget::mousePressEvent(event);
        }

        void mouseDoubleClickEvent(QMouseEvent *event) override
        {
            mousePressEvent(event);
        }

    private:
        CodeEditor *codeEditor;
    };

    QWidget *lineNumberArea;
    bool autoBrackets;
    bool autoQuotes;
    bool autoIndent;
    int indentSize;
    bool lineNumbers;
    bool darkTheme;

    bool comp_disableAutoBrackets;
    bool comp_disableAutoQuotes;
    bool comp_disableAutoIndent;

    void handleAutoBrackets(QKeyEvent *event);
    void handleAutoQuotes(QKeyEvent *event);
    void handleAutoIndent();
    void updateExtraSelections();

private slots:
    void updateMatchHighlights();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void focusInEvent(QFocusEvent *event) override
    {
        QPlainTextEdit::focusInEvent(event);
        setFocus();
    }

    void focusOutEvent(QFocusEvent *event) override
    {
        if (completionPopup && completionPopup->isVisible())
        {
            QWidget *focusWidget = QApplication::focusWidget();
            if (!completionPopup->isAncestorOf(focusWidget))
            {
                hideCompletion();
                QPlainTextEdit::focusOutEvent(event);
            }
            else
            {
                setFocus();
            }
        }
        else
        {
            QPlainTextEdit::focusOutEvent(event);
        }
    }

signals:
    void cursorPositionChanged();

    void fontSizeChanged(int newSize);
    void breakpointToggled(int line, bool added);
};

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent), completer(nullptr), completionEnabled(true),
      autoBrackets(true), autoQuotes(true), autoIndent(true), indentSize(4),
      lineNumbers(true), darkTheme(true), ignoreNextKeyPress(false),
      competitionMode(false), currentDebugLine(-1),codeBeautify(true)
{
    lineNumberArea = new LineNumberArea(this);

    setLineWrapMode(QPlainTextEdit::NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    connect(document(), &QTextDocument::contentsChanged, this, [this]()
    {
        int currentLine = textCursor().blockNumber() + 1;
        bool needUpdate = false;

        if (errorLine > 0 && currentLine == errorLine)
        {
            errorLine = -1;
            needUpdate = true;
        }
        if (warningLine > 0 && currentLine == warningLine)
        {
            warningLine = -1;
            needUpdate = true;
        }

        if (needUpdate)
        {
            updateExtraSelections();
        }
    });

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    QFont font;
    font.setFamily("Consolas");
    font.setStyleHint(QFont::TypeWriter);
    font.setFixedPitch(true);
    font.setPointSize(11);
    setFont(font);

    setTabStopDistance(indentSize * fontMetrics().horizontalAdvance(' '));

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(30, 30, 30));
    p.setColor(QPalette::Text, QColor(220, 220, 220));
    setPalette(p);

    lineNumberArea->setStyleSheet("background: #2D2D2D; color: #888888;");

    errorBgColor = QColor(255, 230, 230);
    warningBgColor = QColor(255, 245, 220);

    lineNumberArea->setVisible(lineNumbers);

    completionPopup = new QWidget(this, Qt::Tool | Qt::FramelessWindowHint);
    completionPopup->setAttribute(Qt::WA_ShowWithoutActivating, true);
    completionPopup->setFocusPolicy(Qt::NoFocus);
    completionPopup->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout(completionPopup);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);

    completionList = new QListView(completionPopup);
    completionList->setModel(new QStringListModel(completionPopup));
    completionList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    completionList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    completionList->setFocusPolicy(Qt::NoFocus);
    completionList->setSelectionMode(QAbstractItemView::SingleSelection);
    updateCompletionListStyle();

    layout->addWidget(completionList);
    completionPopup->setFixedSize(200, 150);

    completionList->installEventFilter(this);

    connect(completionList, &QListView::clicked, this, [this](const QModelIndex &index)
    {
        if (index.isValid())
        {
            insertCompletion(index.data().toString());
            setFocus();
        }
    });

    connect(completionList, &QListView::doubleClicked, this, [this](const QModelIndex &index)
    {
        if (index.isValid())
        {
            insertCompletion(index.data().toString());
            setFocus();
        }
    });

    connect(this, &QPlainTextEdit::selectionChanged, this, &CodeEditor::updateMatchHighlights);
    connect(document(), &QTextDocument::contentsChanged, this, &CodeEditor::updateMatchHighlights);
}

void CodeEditor::updateCompletionListStyle()
{
    if (darkTheme)
    {
        completionList->setStyleSheet(
            "QListView {"
            "    background-color: #2d2d2d;"
            "    color: #d4d4d4;"
            "    border: 1px solid #454545;"
            "    font-family: 'Consolas', 'Courier New', monospace;"
            "    font-size: 12px;"
            "    outline: none;"
            "}"
            "QListView::item {"
            "    padding: 4px 8px;"
            "}"
            "QListView::item:selected {"
            "    background-color: #094771;"
            "    color: white;"
            "}"
            "QListView::item:hover {"
            "    background-color: #3d3d3d;"
            "}"
        );
    }
    else
    {
        completionList->setStyleSheet(
            "QListView {"
            "    background-color: #ffffff;"
            "    color: #000000;"
            "    border: 1px solid #cccccc;"
            "    font-family: 'Consolas', 'Courier New', monospace;"
            "    font-size: 12px;"
            "    outline: none;"
            "}"
            "QListView::item {"
            "    padding: 4px 8px;"
            "}"
            "QListView::item:selected {"
            "    background-color: #0078d4;"
            "    color: white;"
            "}"
            "QListView::item:hover {"
            "    background-color: #f0f0f0;"
            "}"
        );
    }
}

void CodeEditor::updateMatchHighlights()
{
    QString selected = textCursor().selectedText();
    selected.replace(QChar::ParagraphSeparator, QChar::Space);
    selected = selected.trimmed();
    if (selected.isEmpty())
    {
        matchSelections.clear();
        updateExtraSelections();
        return;
    }

    QString pattern = QRegularExpression::escape(selected);
    QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);

    QTextCursor cursor(document());
    QList<QTextEdit::ExtraSelection> newSelections;
    while (!cursor.isNull() && !cursor.atEnd())
    {
        cursor = document()->find(regex, cursor);
        if (!cursor.isNull())
        {
            QTextEdit::ExtraSelection sel;
            sel.cursor = cursor;
            sel.format.setBackground(QColor(200, 200, 200, 100));
            newSelections.append(sel);
        }
    }

    matchSelections = newSelections;
    updateExtraSelections();
}

void CodeEditor::setAutoBracketsEnabled(bool enabled)
{
    autoBrackets = enabled;
}

void CodeEditor::setAutoQuotesEnabled(bool enabled)
{
    autoQuotes = enabled;
}

void CodeEditor::setAutoIndentEnabled(bool enabled)
{
    autoIndent = enabled;
}

void CodeEditor::setIndentSize(int size)
{
    indentSize = size;
    setTabStopDistance(indentSize * fontMetrics().horizontalAdvance(' '));
}

void CodeEditor::setErrorLine(int blockNumber)
{
    errorLine = blockNumber;
    warningLine = -1;
    updateExtraSelections();
}

void CodeEditor::setWarningLine(int blockNumber)
{
    warningLine = blockNumber;
    errorLine = -1;
    updateExtraSelections();
}

void CodeEditor::clearErrorWarningLines()
{
    errorLine = -1;
    warningLine = -1;
    updateExtraSelections();
}

void CodeEditor::setCodeBeautifyEnabled(bool enabled)
{
    codeBeautify = enabled;
}

bool CodeEditor::isInCommentOrStringForBeautify(const QString &text, int position) const
{
    if (position < 0 || position >= text.length())
    {
        return false;
    }

    bool inString = false;
    bool inChar = false;
    bool inBlockComment = false;
    bool escaped = false;

    for (int i = 0; i < position; ++i)
    {
        QChar c = text[i];

        if (escaped)
        {
            escaped = false;
            continue;
        }

        if (c == '\\')
        {
            escaped = true;
            continue;
        }

        if (!inBlockComment && !inString && !inChar && c == '/' && i + 1 < text.length() && text[i+1] == '/')
        {
            return true;
        }

        if (!inString && !inChar)
        {
            if (!inBlockComment && c == '/' && i + 1 < text.length() && text[i+1] == '*')
            {
                inBlockComment = true;
                i++;
                continue;
            }
            if (inBlockComment && c == '*' && i + 1 < text.length() && text[i+1] == '/')
            {
                inBlockComment = false;
                i++;
                continue;
            }
        }

        if (!inBlockComment)
        {
            if (c == '"' && !inChar)
            {
                inString = !inString;
            }
            else if (c == '\'' && !inString)
            {
                inChar = !inChar;
            }
        }
    }

    return inString || inChar || inBlockComment;
}

bool CodeEditor::isControlStructureLine(const QString &line) const
{
    QString trimmed = line.trimmed();

    QRegularExpression controlPattern(
        R"(^\s*(if|else\s+if|else|for|while|switch|do)\s*\()"
    );

    QRegularExpression funcPattern(
        R"(^\s*((?:void|int|char|double|float|bool|long|short|unsigned|signed|auto|inline|static|const|virtual)\s+)+\w+\s*\()"
    );

    QRegularExpression methodPattern(
        R"(^\s*\w+\s*::\s*\w+\s*\()"
    );

    return controlPattern.match(trimmed).hasMatch() ||
           funcPattern.match(trimmed).hasMatch() ||
           methodPattern.match(trimmed).hasMatch();
}

// 代码格式化（美化）
QString CodeEditor::beautifyCode(const QString &code, bool isControlStructure)
{
    QString result = code;

    QList<QString> protectedPointers;
    int pointerIndex = 0;
    int maxIterations = 100;

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        QRegularExpression ptrPattern(R"(\b([a-zA-Z_][a-zA-Z0-9_]*)\s+\*\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*[,;)])");
        QRegularExpressionMatch match = ptrPattern.match(result);
        if (!match.hasMatch())
        {
            break;
        }

        QString typeName = match.captured(1);
        QString varName = match.captured(2);
        QString placeholder = QString("__PTR_%1__").arg(pointerIndex++);
        QString replacement = typeName + " " + placeholder + varName + match.captured(0).right(1);
        result.replace(match.capturedStart(), match.capturedLength(), replacement);
        protectedPointers.append(placeholder);
    }

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        QRegularExpression ptrCommaPattern(R"(,\s*\*\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*[,;)])");
        QRegularExpressionMatch match = ptrCommaPattern.match(result);
        if (!match.hasMatch())
        {
            break;
        }

        QString varName = match.captured(1);
        QString placeholder = QString("__PTR_%1__").arg(pointerIndex++);
        QString replacement = ", " + placeholder + varName + match.captured(0).right(1);
        result.replace(match.capturedStart(), match.capturedLength(), replacement);
        protectedPointers.append(placeholder);
    }

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        QRegularExpression ptrParenPattern(R"(\(\s*([a-zA-Z_][a-zA-Z0-9_]*)\s+\*\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*\))");
        QRegularExpressionMatch match = ptrParenPattern.match(result);
        if (!match.hasMatch())
        {
            break;
        }

        QString typeName = match.captured(1);
        QString varName = match.captured(2);
        QString placeholder = QString("__PTR_%1__").arg(pointerIndex++);
        QString replacement = "(" + typeName + " " + placeholder + varName + ")";
        result.replace(match.capturedStart(), match.capturedLength(), replacement);
        protectedPointers.append(placeholder);
    }

    QList<QString> protectedStrings;
    QRegularExpression stringPattern(R"("(?:[^"\\]|\\.)*"|'(?:[^'\\]|\\.)*')");
    QRegularExpressionMatchIterator it = stringPattern.globalMatch(result);

    QList<QRegularExpressionMatch> matches;
    while (it.hasNext())
    {
        matches.prepend(it.next());
    }

    for (const auto &match : matches)
    {
        protectedStrings.append(match.captured());
        result.replace(match.capturedStart(), match.capturedLength(),
                       QString("__STR_%1__").arg(protectedStrings.size() - 1));
    }

    result.replace(QRegularExpression(">(\\s+)\\("), ">(");

    QList<QString> protectedTemplates;
    int templateIndex = 0;

    for (int iter = 0; iter < maxIterations; ++iter)
    {
        QRegularExpression templatePattern(R"(\b([a-zA-Z_][a-zA-Z0-9_]*)\s*<([^<>&|;]*)>)");
        QRegularExpressionMatch match = templatePattern.match(result);

        if (!match.hasMatch())
        {
            break;
        }

        QString templateName = match.captured(1);
        QString templateArgs = match.captured(2);

        QString processedArgs = templateArgs.trimmed();
        processedArgs.replace(QRegularExpression("\\s+"), " ");
        processedArgs.replace(QRegularExpression("\\s*,\\s*"), ", ");

        QString processedTemplate = templateName + "<" + processedArgs + ">";

        QString placeholder = QString("__TMPL_%1__").arg(templateIndex++);
        protectedTemplates.append(processedTemplate);
        result.replace(match.capturedStart(), match.capturedLength(), placeholder);
    }

    QList<QString> protectedComments;
    QRegularExpression commentPattern(R"(//.*$|/\*.*?\*/)");
    QRegularExpressionMatchIterator commentIt = commentPattern.globalMatch(result);

    QList<QRegularExpressionMatch> commentMatches;
    while (commentIt.hasNext())
    {
        commentMatches.prepend(commentIt.next());
    }

    for (const auto &match : commentMatches)
    {
        protectedComments.prepend(match.captured());
        result.replace(match.capturedStart(), match.capturedLength(),
                       QString("__CMT_%1__").arg(protectedComments.size() - 1));
    }

    result.replace(QRegularExpression(R"(\bif\s*\()"), "if (");
    result.replace(QRegularExpression(R"(\bfor\s*\()"), "for (");
    result.replace(QRegularExpression(R"(\bwhile\s*\()"), "while (");
    result.replace(QRegularExpression(R"(\bswitch\s*\()"), "switch (");
    result.replace(QRegularExpression(R"(\bcatch\s*\()"), "catch (");

    int searchPos = 0;
    while (true)
    {
        int forPos = result.indexOf(QRegularExpression("\\bfor\\b"), searchPos);
        if (forPos == -1)
        {
            break;
        }

        int leftParen = -1;
        for (int i = forPos + 3; i < result.length(); i++)
        {
            if (result[i] == '(')
            {
                leftParen = i;
                break;
            }
            if (!result[i].isSpace())
            {
                break;
            }
        }

        if (leftParen == -1)
        {
            searchPos = forPos + 3;
            continue;
        }

        int parenCount = 1;
        int rightParen = leftParen + 1;
        while (rightParen < result.length() && parenCount > 0)
        {
            if (result[rightParen] == '(')
            {
                parenCount++;
            }
            else if (result[rightParen] == ')')
            {
                parenCount--;
            }
            rightParen++;
        }
        rightParen--;

        if (parenCount != 0)
        {
            searchPos = forPos + 3;
            continue;
        }

        for (int i = leftParen + 1; i < rightParen; i++)
        {
            if (result[i] == ';')
            {
                if (i + 1 < rightParen && !result[i + 1].isSpace())
                {
                    result.insert(i + 1, ' ');
                    rightParen++;
                }
            }
        }
        searchPos = rightParen + 1;
    }

    result.replace("++", "__INCR__");
    result.replace("--", "__DECR__");
    result.replace("->", "__ARROW__");
    result.replace("<<", "__LSHIFT__");
    result.replace(">>", "__RSHIFT__");
    result.replace("::", "__SCOPE__");

    result.replace(QRegularExpression("(\\w)\\s*\\+\\+"), "\\1++");
    result.replace(QRegularExpression("(\\w)\\s*\\-\\-"), "\\1--");
    result.replace(QRegularExpression("\\+\\+\\s*(\\w)"), "++\\1");
    result.replace(QRegularExpression("\\-\\-\\s*(\\w)"), "--\\1");

    QList<QString> protectedCompoundOps;
    QStringList compoundOps = {"+=", "-=", "*=", "/=", "%=", "&=", "|=", "^="};
    int compoundIndex = 0;

    for (const QString &op : compoundOps)
    {
        QRegularExpression opPattern(QString("\\s*%1\\s*").arg(QRegularExpression::escape(op)));
        QRegularExpressionMatchIterator opIt = opPattern.globalMatch(result);

        QList<QRegularExpressionMatch> opMatches;
        while (opIt.hasNext())
        {
            opMatches.prepend(opIt.next());
        }

        for (const auto &match : opMatches)
        {
            QString formattedOp = QString(" %1 ").arg(op);
            QString placeholder = QString("__COMPOP_%1__").arg(compoundIndex++);
            protectedCompoundOps.append(formattedOp);
            result.replace(match.capturedStart(), match.capturedLength(), placeholder);
        }
    }

    QStringList compareOps = {"<=", ">=", "==", "!="};
    for (const QString &op : compareOps)
    {
        QRegularExpression opPattern(QString("\\s*%1\\s*").arg(QRegularExpression::escape(op)));
        result.replace(opPattern, QString(" %1 ").arg(op));
    }

    result.replace(QRegularExpression("\\s*&&\\s*"), " && ");
    result.replace(QRegularExpression("\\s*\\|\\|\\s*"), " || ");

    result.replace(QRegularExpression("\\s*\\?\\s*"), " ? ");
    result.replace(QRegularExpression("\\s*:\\s*"), " : ");

    QString prevResult;
    int loopCount = 0;
    do
    {
        prevResult = result;
        result.replace(QRegularExpression("([^\\s<>=_])\\s*<\\s*([^\\s<>=_])"), "\\1 < \\2");
        result.replace(QRegularExpression("([^\\s<>=_])\\s*>\\s*([^\\s<>=_])"), "\\1 > \\2");
    }
    while (result != prevResult && ++loopCount < 10);

    loopCount = 0;
    do
    {
        prevResult = result;
        result.replace(QRegularExpression("([a-zA-Z0-9_\$\$])&([a-zA-Z0-9_(])"), "\\1 & \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\$\$])\\|([a-zA-Z0-9_(])"), "\\1 | \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\$\$])\\^([a-zA-Z0-9_(])"), "\\1 ^ \\2");
    }
    while (result != prevResult && ++loopCount < 10);

    result.replace(QRegularExpression("\\s*\\+\\s*"), "+");
    result.replace(QRegularExpression("\\s*\\-\\s*"), "-");
    result.replace(QRegularExpression("\\s*\\*\\s*"), "*");
    result.replace(QRegularExpression("\\s*\\/\\s*"), "/");
    result.replace(QRegularExpression("\\s*%\\s*"), "%");

    loopCount = 0;
    do
    {
        prevResult = result;
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\]])\\+([a-zA-Z0-9_(])"), "\\1 + \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9)\\]])-([a-zA-Z0-9_(])"), "\\1 - \\2");
        result.replace(QRegularExpression("([><=!])-([a-zA-Z0-9_(])"), "\\1 -\\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\]])\\*([a-zA-Z0-9_(])"), "\\1 * \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\]])\\/([a-zA-Z0-9_(])"), "\\1 / \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\]])%([a-zA-Z0-9_(])"), "\\1 % \\2");
    }
    while (result != prevResult && ++loopCount < 10);

    loopCount = 0;
    do
    {
        prevResult = result;
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\])])\\+([a-zA-Z0-9_(])"), "\\1 + \\2");
        result.replace(QRegularExpression("([><=!])-([a-zA-Z0-9_(])"), "\\1 -\\2");
        result.replace(QRegularExpression("([a-zA-Z0-9)\\]])-([a-zA-Z0-9_(])"), "\\1 - \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\])])\\*([a-zA-Z0-9_(])"), "\\1 * \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\])])\\/([a-zA-Z0-9_(])"), "\\1 / \\2");
        result.replace(QRegularExpression("([a-zA-Z0-9_\\$\\$_\\])])%([a-zA-Z0-9_(])"), "\\1 % \\2");
    }
    while (result != prevResult && ++loopCount < 10);

    int assignLoopCount = 0;
    do
    {
        prevResult = result;
        QRegularExpression assignPattern("([^=!<>+\\-*/%&|^_])=([^=])");
        result.replace(assignPattern, "\\1 = \\2");
    }
    while (result != prevResult && ++assignLoopCount < 10);

    result.replace(QRegularExpression("(?<![=!<>+\\-*/%&|^\\s])=(?![=\\s])"), " = ");

    result.replace(QRegularExpression(",\\s*"), ", ");

    result.replace("__INCR__", "++");
    result.replace("__DECR__", "--");
    result.replace("__ARROW__", "->");
    result.replace("__LSHIFT__", " << ");
    result.replace("__RSHIFT__", " >> ");
    result.replace("__SCOPE__", "::");

    for (int i = 0; i < protectedCompoundOps.size(); ++i)
    {
        result.replace(QString("__COMPOP_%1__").arg(i), protectedCompoundOps[i]);
    }

    for (int i = protectedTemplates.size() - 1; i >= 0; --i)
    {
        result.replace(QString("__TMPL_%1__").arg(i), protectedTemplates[i]);
    }

    result.replace(QRegularExpression(",(?!\\s)"), ", ");
    result.replace(QRegularExpression(",\\s{2,}"), ", ");

    result.replace(QRegularExpression("  +"), " ");

    QStringList pointerKeywords =
    {
        "void", "int", "char", "double", "float", "bool", "long", "short",
        "signed", "unsigned", "auto", "wchar_t", "size_t",
        "class", "struct", "typename", "virtual", "const", "static"
    };
    QString typePattern = pointerKeywords.join("|");
    result.replace(QRegularExpression(QString("\\b(%1)\\s+\\*\\s+([a-zA-Z_][a-zA-Z0-9_]*)").arg(typePattern)), "\\1 *\\2");

    result.replace(QRegularExpression("(^|[;{}]\\s*)([a-zA-Z_][a-zA-Z0-9_]*)\\s+\\*\\s+([a-zA-Z_][a-zA-Z0-9_]*)"), "\\1\\2 *\\3");

    result.replace(QRegularExpression(",\\s*\\*\\s+([a-zA-Z_][a-zA-Z0-9_]*)"), ", *\\1");

    result.replace(QRegularExpression("\\(\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s+\\*\\s+([a-zA-Z_][a-zA-Z0-9_]*)"), "(\\1 *\\2");
    result.replace(QRegularExpression(",\\s+([a-zA-Z_][a-zA-Z0-9_]*)\\s+\\*\\s+([a-zA-Z_][a-zA-Z0-9_]*)"), ", \\1 *\\2");

    result.replace(QRegularExpression("([a-zA-Z0-9_\\]])\\s*\\.\\s*([a-zA-Z_])"), "\\1.\\2");

    result.replace(QRegularExpression("\\(\\s*\\)"), "()");
    result.replace(QRegularExpression("\\)\\s*([);,])"), ")\\1");

    result.replace(QRegularExpression("\\b([a-zA-Z0-9_]+)\\*([a-zA-Z0-9_]+)\\b"), "\\1 * \\2");

    result.replace(QRegularExpression("([a-zA-Z0-9_)\\]\\}])\\s*\\*\\s*([a-zA-Z0-9_(\\[])"), "\\1 * \\2");

    for (int i = 0; i < protectedStrings.size(); ++i)
    {
        result.replace(QString("__STR_%1__").arg(i), protectedStrings[i]);
    }

    for (int i = 0; i < protectedPointers.size(); ++i)
    {
        result.replace(protectedPointers[i], "*");
    }

    for (int i = 0; i < protectedComments.size(); ++i)
    {
        result.replace(QString("__CMT_%1__").arg(i), protectedComments[i]);
    }

    return result;
}

void CodeEditor::beautifyCurrentLine()
{
    if (!codeBeautify)
    {
        return;
    }

    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    QString lineText = block.text();

    QString indent;
    for (int i = 0; i < lineText.length(); ++i)
    {
        if (lineText[i].isSpace())
        {
            indent += lineText[i];
        }
        else
        {
            break;
        }
    }

    QString trimmedLine = lineText.trimmed();

    if (trimmedLine.startsWith("//") || trimmedLine.startsWith("/*"))
    {
        return;
    }

    bool isControlStructure = isControlStructureLine(lineText);

    QString beautified = beautifyCode(trimmedLine, isControlStructure);

    if (beautified == trimmedLine)
    {
        return;
    }

    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    cursor.insertText(indent + beautified);
}

bool CodeEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == completionList && completionPopup->isVisible())
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
            {
                QModelIndex index = completionList->currentIndex();
                if (index.isValid())
                {
                    insertCompletion(index.data().toString());
                }
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Escape)
            {
                hideCompletion();
                this->setFocus(Qt::OtherFocusReason);
                return true;
            }
            else if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down)
            {
                return false;
            }
            else if (keyEvent->key() == Qt::Key_Tab)
            {
                QModelIndex index = completionList->currentIndex();
                if (index.isValid())
                {
                    insertCompletion(index.data().toString());
                }
                return true;
            }
            else
            {
                hideCompletion();
                this->setFocus(Qt::OtherFocusReason);
                return false;
            }
        }
    }

    return QPlainTextEdit::eventFilter(obj, event);
}

void CodeEditor::setCodeFoldingEnabled(bool enabled)
{
    updateLineNumberAreaWidth(0);
    update();
}

void CodeEditor::setLineNumbersEnabled(bool enabled)
{
    lineNumbers = enabled;
    lineNumberArea->setVisible(true);
    updateLineNumberAreaWidth(0);
    update();
}

void CodeEditor::setDarkThemeEnabled(bool enabled)
{
    darkTheme = enabled;
    QPalette p = palette();
    if (enabled)
    {
        errorBgColor = QColor(80, 40, 40);
        warningBgColor = QColor(80, 70, 40);
    }
    else
    {
        errorBgColor = QColor(255, 230, 230);
        warningBgColor = QColor(255, 245, 220);
    }
    viewport()->update();
    if (enabled)
    {
        p.setColor(QPalette::Base, QColor(30, 30, 30));
        p.setColor(QPalette::Text, QColor(220, 220, 220));
        lineNumberArea->setStyleSheet("background: #2D2D2D; color: #888888;");
    }
    else
    {
        p.setColor(QPalette::Base, Qt::white);
        p.setColor(QPalette::Text, Qt::black);
        lineNumberArea->setStyleSheet("background: #F5F5F5; color: #666666;");
    }
    setPalette(p);

    updateCompletionListStyle();

    CppHighlighter *highlighter = qobject_cast<CppHighlighter*>(
                                      document()->findChild<QSyntaxHighlighter*>());
    if (highlighter)
    {
        highlighter->setDarkTheme(enabled);
    }

    highlightCurrentLine();
    lineNumberArea->update();
    update();
}

void CodeEditor::setBreakpoints(const QSet<int> &lines)
{
    breakpointLines = lines;
    lineNumberArea->update();
}

void CodeEditor::addBreakpoint(int line)
{
    breakpointLines.insert(line);
    lineNumberArea->update();
    emit breakpointToggled(line, true);
}

void CodeEditor::removeBreakpoint(int line)
{
    breakpointLines.remove(line);
    lineNumberArea->update();
    emit breakpointToggled(line, false);
}

void CodeEditor::toggleBreakpoint(int line)
{
    if (line < 1)
    {
        return;
    }

    if (breakpointLines.contains(line))
    {
        breakpointLines.remove(line);
    }
    else
    {
        breakpointLines.insert(line);
    }

    if (lineNumberArea)
    {
        lineNumberArea->update();
    }

    bool added = breakpointLines.contains(line);
    QTimer::singleShot(0, this, [this, line, added]()
    {
        emit breakpointToggled(line, added);
    });
}

bool CodeEditor::hasBreakpoint(int line) const
{
    return breakpointLines.contains(line);
}

void CodeEditor::setCurrentDebugLine(int line)
{
    currentDebugLine = line;

    if (line > 0)
    {
        QTextBlock block = document()->findBlockByLineNumber(line - 1);
        if (block.isValid())
        {
            QTextCursor cursor(block);
            setTextCursor(cursor);
            centerCursor();
        }
    }

    updateExtraSelections();
    lineNumberArea->update();
}

void CodeEditor::clearCurrentDebugLine()
{
    currentDebugLine = -1;
    updateExtraSelections();
    lineNumberArea->update();
}

void CodeEditor::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        QPoint angleDelta = event->angleDelta();

        if (!angleDelta.isNull())
        {
            QFont currentFont = font();
            int currentSize = currentFont.pointSize();

            if (angleDelta.y() > 0)
            {
                currentSize = qMin(currentSize + 1, 72);
            }
            else if (angleDelta.y() < 0)
            {
                currentSize = qMax(currentSize - 1, 6);
            }

            currentFont.setPointSize(currentSize);
            setFont(currentFont);

            setTabStopDistance(indentSize * fontMetrics().horizontalAdvance(' '));

            updateLineNumberAreaWidth(0);

            emit fontSizeChanged(currentSize);

            event->accept();
            return;
        }
    }

    QPlainTextEdit::wheelEvent(event);
}

void CodeEditor::drawBreakpointMarker(QPainter &painter, const QRect &rect, bool isCurrentLine)
{
    int size = qMin(rect.width(), rect.height()) - 4;
    int x = rect.left() + (rect.width() - size) / 2;
    int y = rect.top() + (rect.height() - size) / 2;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 60, 60));
    painter.drawEllipse(x, y, size, size);

    if (isCurrentLine)
    {
        painter.setBrush(QColor(255, 255, 0));
        QPolygon arrow;
        int arrowSize = size / 2;
        int cx = x + size / 2;
        int cy = y + size / 2;
        arrow << QPoint(cx - arrowSize/2, cy - arrowSize/2)
              << QPoint(cx + arrowSize/2, cy)
              << QPoint(cx - arrowSize/2, cy + arrowSize/2);
        painter.drawPolygon(arrow);
    }
}

int CodeEditor::lineNumberAreaWidth()
{
    int breakpointAreaWidth = 20;

    if (!lineNumbers)
    {
        return breakpointAreaWidth;
    }

    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = breakpointAreaWidth + 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* 留空 */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    int lineNumWidth = lineNumberAreaWidth();

    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumWidth, cr.height()));
}

bool CodeEditor::isPositionInCommentOrString() const
{
    QTextCursor cursor = textCursor();
    int position = cursor.position();
    QString fullText = toPlainText().left(position);

    bool inString = false;
    bool inChar = false;
    bool inBlockComment = false;
    bool escaped = false;

    for (int i = 0; i < fullText.length(); ++i)
    {
        QChar c = fullText[i];

        if (escaped)
        {
            escaped = false;
            continue;
        }

        if (c == '\\')
        {
            escaped = true;
            continue;
        }

        if (!inBlockComment)
        {
            if (c == '"' && !inChar)
            {
                inString = !inString;
            }
            else if (c == '\'' && !inString)
            {
                inChar = !inChar;
            }
        }

        if (!inString && !inChar)
        {
            if (!inBlockComment && c == '/' && i + 1 < fullText.length() && fullText[i+1] == '*')
            {
                inBlockComment = true;
                i++;
                continue;
            }
            if (inBlockComment && c == '*' && i + 1 < fullText.length() && fullText[i+1] == '/')
            {
                inBlockComment = false;
                i++;
                continue;
            }
        }
    }

    return inString || inChar || inBlockComment;
}

void CodeEditor::setEditorFont(const QFont &font)
{
    qDebug() << "[CodeEditor] 应用字体:" << font.family() << font.pointSize();
    setFont(font);
    setTabStopDistance(indentSize * fontMetrics().horizontalAdvance(' '));
    updateLineNumberAreaWidth(0);
}

void CodeEditor::highlightCurrentLine()
{
    updateExtraSelections();
}

void CodeEditor::updateExtraSelections()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection currentLineSelection;
        QColor lineColor;
        if (darkTheme)
        {
            lineColor = QColor(45, 45, 45);
        }
        else
        {
            lineColor = QColor(240, 240, 240);
        }
        currentLineSelection.format.setBackground(lineColor);
        currentLineSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
        currentLineSelection.cursor = textCursor();
        currentLineSelection.cursor.clearSelection();

        int currentLine = textCursor().blockNumber() + 1;
        if (currentLine != errorLine && currentLine != warningLine)
        {
            extraSelections.append(currentLineSelection);
        }
    }

    if (errorLine > 0)
    {
        QTextBlock block = document()->findBlockByNumber(errorLine - 1);
        if (block.isValid())
        {
            QTextEdit::ExtraSelection errorSelection;
            errorSelection.format.setBackground(errorBgColor);
            errorSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
            errorSelection.cursor = QTextCursor(block);
            errorSelection.cursor.clearSelection();
            extraSelections.append(errorSelection);
        }
    }

    if (warningLine > 0)
    {
        QTextBlock block = document()->findBlockByNumber(warningLine - 1);
        if (block.isValid())
        {
            QTextEdit::ExtraSelection warningSelection;
            warningSelection.format.setBackground(warningBgColor);
            warningSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
            warningSelection.cursor = QTextCursor(block);
            warningSelection.cursor.clearSelection();
            extraSelections.append(warningSelection);
        }
    }

    if (currentDebugLine > 0)
    {
        QTextBlock block = document()->findBlockByNumber(currentDebugLine - 1);
        if (block.isValid())
        {
            QTextEdit::ExtraSelection debugSelection;
            debugSelection.format.setBackground(QColor(255, 255, 150));
            debugSelection.format.setProperty(QTextFormat::FullWidthSelection, true);
            debugSelection.cursor = QTextCursor(block);
            debugSelection.cursor.clearSelection();
            extraSelections.append(debugSelection);
        }
    }

    extraSelections.append(matchSelections);

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

    painter.setFont(this->font());

    QColor backgroundColor, textColor, currentLineColor;
    if (darkTheme)
    {
        backgroundColor = QColor(45, 45, 45);
        textColor = QColor(150, 150, 150);
        currentLineColor = QColor(60, 60, 60);
    }
    else
    {
        backgroundColor = QColor(245, 245, 245);
        textColor = QColor(100, 100, 100);
        currentLineColor = QColor(230, 230, 230);
    }

    painter.fillRect(event->rect(), backgroundColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    int breakpointAreaWidth = 20;
    int lineNumberWidth = lineNumberArea->width() - breakpointAreaWidth;

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            int lineNumber = blockNumber + 1;
            bool isCurrentDebug = (lineNumber == currentDebugLine);
            bool hasBreakpointHere = breakpointLines.contains(lineNumber);

            if (isCurrentDebug)
            {
                painter.fillRect(QRect(0, top, lineNumberArea->width(), fontMetrics().height()),
                                 QColor(255, 255, 0, 50));
            }

            if (hasBreakpointHere || isCurrentDebug)
            {
                QRect markerRect(0, top, breakpointAreaWidth, fontMetrics().height());

                if (hasBreakpointHere)
                {
                    int size = qMin(breakpointAreaWidth, fontMetrics().height()) - 4;
                    int x = (breakpointAreaWidth - size) / 2;
                    int y = top + (fontMetrics().height() - size) / 2;

                    painter.setRenderHint(QPainter::Antialiasing);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(QColor(255, 60, 60));
                    painter.drawEllipse(x, y, size, size);
                }

                if (isCurrentDebug)
                {
                    int arrowSize = 8;
                    int cx = breakpointAreaWidth / 2;
                    int cy = top + fontMetrics().height() / 2;

                    painter.setBrush(QColor(255, 220, 0));
                    painter.setPen(QPen(QColor(200, 150, 0), 1));

                    QPolygon arrow;
                    arrow << QPoint(cx - 3, cy - arrowSize/2)
                          << QPoint(cx + arrowSize/2, cy)
                          << QPoint(cx - 3, cy + arrowSize/2);
                    painter.drawPolygon(arrow);
                }
            }

            if (lineNumbers)
            {
                QString number = QString::number(lineNumber);
                painter.setPen(isCurrentDebug ? QColor(0, 0, 0) : textColor);
                painter.drawText(breakpointAreaWidth, top, lineNumberWidth - 4,
                                 fontMetrics().height(),
                                 Qt::AlignRight, number);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::mousePressEvent(QMouseEvent *event)
{
    if (completionPopup && completionPopup->isVisible())
    {
        QRect popupRect = completionPopup->geometry();
        QPoint globalClickPos = mapToGlobal(event->pos());

        if (!popupRect.contains(globalClickPos))
        {
            hideCompletion();
        }
    }

    QPlainTextEdit::mousePressEvent(event);

    emit cursorPositionChanged();
}

void CodeEditor::paintEvent(QPaintEvent *event)
{
    QPlainTextEdit::paintEvent(event);
    if (!showIndentGuides)
    {
        return;
    }

    QPainter painter(viewport());
    QColor guideColor = darkTheme ? QColor(80, 80, 80, 255) : QColor(210, 210, 210, 255);
    painter.setPen(QPen(guideColor, 0, Qt::SolidLine));
    painter.setRenderHint(QPainter::Antialiasing, false);

    QFontMetricsF fm(font());
    qreal spaceWidth = fm.horizontalAdvance(' ');
    qreal docMargin = document()->documentMargin();

    QTextBlock block = firstVisibleBlock();
    QPointF offset = contentOffset();
    qreal yPos = blockBoundingGeometry(block).translated(offset).top();

    auto getLeadingSpaces = [&](const QTextBlock &b) -> int
    {
        QString t = b.text();
        int spaces = 0;
        for (int i = 0; i < t.length(); ++i)
        {
            if (t[i] == ' ')
            {
                spaces++;
            }
            else if (t[i] == '	')
            {
                spaces = ((spaces / indentSize) + 1) * indentSize;
            }
            else
            {
                break;
            }
        }
        return spaces;
    };

    while (block.isValid())
    {
        QRectF r = blockBoundingRect(block);
        if (yPos > viewport()->height())
        {
            break;
        }

        if (yPos + r.height() >= 0)
        {
            QString text = block.text();
            bool isBlank = text.trimmed().isEmpty();
            int drawDepth = 0;

            if (!isBlank)
            {
                drawDepth = getLeadingSpaces(block);
            }
            else
            {
                QTextBlock prev = block.previous();
                while (prev.isValid() && prev.text().trimmed().isEmpty())
                {
                    prev = prev.previous();
                }

                QTextBlock next = block.next();
                while (next.isValid() && next.text().trimmed().isEmpty())
                {
                    next = next.next();
                }

                int aboveIndent = 0;
                int belowIndent = 0;

                if (prev.isValid())
                {
                    aboveIndent = getLeadingSpaces(prev);
                    if (prev.text().trimmed().endsWith('{'))
                    {
                        aboveIndent += indentSize;
                    }
                }
                if (next.isValid())
                {
                    belowIndent = getLeadingSpaces(next);
                    if (next.text().trimmed().startsWith('}'))
                    {
                        belowIndent += indentSize;
                    }
                }

                drawDepth = qMin(aboveIndent, belowIndent);
            }

            for (int col = indentSize; col < drawDepth; col += indentSize)
            {
                int x = qFloor(offset.x() + docMargin + col * spaceWidth);
                if (x >= 0 && x <= viewport()->width())
                {
                    painter.drawLine(x, qRound(yPos), x, qRound(yPos + r.height()));
                }
            }
        }

        yPos += r.height();
        block = block.next();
    }
}

void CodeEditor::handleAutoBrackets(QKeyEvent *event)
{
    QChar key = event->text().isEmpty() ? QChar() : event->text().at(0);
    QTextCursor cursor = textCursor();

    if (key == '(')
    {
        hideCompletion();
        cursor.insertText("()");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        event->accept();
    }
    else if (key == '[')
    {
        hideCompletion();
        cursor.insertText("[]");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        event->accept();
    }
    else if (key == '{')
    {
        event->ignore();
    }
    else if (key == ')' || key == ']' || key == '}')
    {
        hideCompletion();

        if (key == '}')
        {
            QTextCursor testCursor = textCursor();
            QTextBlock block = testCursor.block();
            QString text = block.text();
            QString trimmed = text.trimmed();

            if (trimmed == "}" || trimmed.isEmpty())
            {
                int targetIndent = 0;
                QTextBlock prevBlock = block.previous();
                while (prevBlock.isValid())
                {
                    if (!prevBlock.text().trimmed().isEmpty())
                    {
                        QString prevText = prevBlock.text();
                        for (int i = 0; i < prevText.length(); ++i)
                        {
                            if (prevText[i] == ' ')
                            {
                                targetIndent++;
                            }
                            else if (prevText[i] == '\t')
                            {
                                targetIndent += indentSize;
                            }
                            else
                            {
                                break;
                            }
                        }
                        break;
                    }
                    prevBlock = prevBlock.previous();
                }

                testCursor.movePosition(QTextCursor::StartOfBlock);
                testCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

                QString newLine(targetIndent, ' ');
                newLine.append('}');
                testCursor.insertText(newLine);

                if (document()->characterAt(testCursor.position()) == '}')
                {
                    testCursor.deleteChar();
                }

                setTextCursor(testCursor);
                event->accept();
                return;
            }
        }

        QTextCursor skipCursor = textCursor();
        if (skipCursor.hasSelection())
        {
            event->ignore();
            return;
        }

        QChar nextChar = document()->characterAt(skipCursor.position());
        if (nextChar == key)
        {
            skipCursor.movePosition(QTextCursor::Right);
            setTextCursor(skipCursor);
            event->accept();
        }
        else
        {
            if (key == '}')
            {
                skipCursor.insertText("}");
                event->accept();
            }
            else
            {
                event->ignore();
            }
        }
    }
    else
    {
        event->ignore();
    }
}

void CodeEditor::handleAutoQuotes(QKeyEvent *event)
{
    QChar key = event->text().isEmpty() ? QChar() : event->text().at(0);
    QTextCursor cursor = textCursor();

    if (key == '\"')
    {
        hideCompletion();

        cursor.insertText("\"\"");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        event->accept();
    }
    else if (key == '\'')
    {
        hideCompletion();

        cursor.insertText("''");
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void CodeEditor::handleAutoIndent()
{
    QTextCursor cursor = textCursor();
    QTextBlock currentBlock = cursor.block();
    QTextBlock previousBlock = currentBlock.previous();

    if (!previousBlock.isValid())
    {
        return;
    }

    QString previousText = previousBlock.text();
    int indentLevel = 0;

    for (int i = 0; i < previousText.length(); ++i)
    {
        if (previousText[i] == ' ')
        {
            indentLevel++;
        }
        else if (previousText[i] == '	')
        {
            indentLevel += indentSize;
        }
        else
        {
            break;
        }
    }

    QString currentLineText = currentBlock.text();

    bool hasClosingBrace = currentLineText.trimmed() == "}";

    QString trimmedPrevious = previousText.trimmed();
    if (trimmedPrevious.endsWith('{') && !hasClosingBrace)
    {
        indentLevel += indentSize;
    }

    if (indentLevel > 0)
    {
        QString indent(indentLevel, ' ');
        cursor.insertText(indent);
    }

    if (hasClosingBrace && trimmedPrevious.endsWith('}'))
    {
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, indentSize);
        QString selectedText = cursor.selectedText();
        if (selectedText.trimmed().isEmpty() && selectedText.length() == indentSize)
        {
            cursor.removeSelectedText();
        }
    }
}

class CompilerIDE;
class NewFileDialog;
class WelcomeDialog;
class SplashScreen;
class CodeEditor;

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    enum FileType
    {
        NormalCpp,
        EasyXProject,
        Cancelled
    };

    explicit NewFileDialog(QWidget *parent = nullptr);
    ~NewFileDialog();

    FileType getSelectedFileType() const
    {
        return selectedType;
    }
    bool isEasyX() const
    {
        return selectedType == EasyXProject;
    }


private slots:
    void onCardClicked(FileType type);
    void onCreateClicked();
    void onCancelClicked();

private:
    void setupUI();
    QWidget* createProjectCard(const QString &title, const QString &description,
                               const QString &iconText, FileType type,
                               const QColor &accentColor, bool isDarkTheme);
    void updateCardSelection(QWidget *card);

    FileType selectedType;
    QWidget *normalCard;
    QWidget *easyxCard;
    QWidget *selectedCard;
    QPushButton *createBtn;
    QPushButton *cancelBtn;
    bool isDarkTheme;
};

NewFileDialog::NewFileDialog(QWidget *parent)
    : QDialog(parent)
    , selectedType(Cancelled)
    , normalCard(nullptr)
    , easyxCard(nullptr)
    , selectedCard(nullptr)
    , isDarkTheme(false)
{
    if (parent)
    {
        QVariant themeVariant = parent->property("darkTheme");
        if (themeVariant.isValid())
        {
            isDarkTheme = themeVariant.toBool();
        }
    }
    setupUI();
}

NewFileDialog::~NewFileDialog()
{
}

void NewFileDialog::setupUI()
{
    setWindowTitle(tr("新建文件"));
    setFixedSize(700, 550);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *headerWidget = new QWidget(this);
    headerWidget->setFixedHeight(80);
    headerWidget->setStyleSheet(
        "QWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "              stop:0 #667eea, stop:1 #764ba2);"
        "}"
    );

    QVBoxLayout *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(30, 15, 30, 15);

    QLabel *titleLabel = new QLabel(tr("新建文件"), headerWidget);
    titleLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 28px;"
        "  font-weight: bold;"
        "  color: white;"
        "}"
    );

    QLabel *subtitleLabel = new QLabel(tr("选择要创建的文件类型"), headerWidget);
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 14px;"
        "  color: rgba(255, 255, 255, 0.9);"
        "}"
    );

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(subtitleLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(headerWidget);

    QWidget *contentWidget = new QWidget(this);

    QString contentBg = isDarkTheme ? "#2c3e50" : "#f5f7fa";
    contentWidget->setStyleSheet(QString("QWidget { background: %1; }").arg(contentBg));

    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(20);

    QLabel *hintLabel = new QLabel(tr("请选择一个项目类型："), contentWidget);
    QString hintColor = isDarkTheme ? "#ecf0f1" : "#2c3e50";
    hintLabel->setStyleSheet(
        QString("QLabel {"
                "  font-size: 15px;"
                "  color: %1;"
                "  font-weight: 600;"
                "}").arg(hintColor)
    );
    contentLayout->addWidget(hintLabel);

    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(20);

    normalCard = createProjectCard(
                     tr("普通 C++ 文件"),
                     tr("创建标准的 C++ 源代码文件"),
                     "",
                     NormalCpp,
                     QColor("#3498db"),
                     isDarkTheme
                 );
    cardsLayout->addWidget(normalCard);

    easyxCard = createProjectCard(
                    tr("EasyX 图形项目"),
                    tr("创建 EasyX 图形程序\n包含图形库头文件和初始化代码"),
                    "",
                    EasyXProject,
                    QColor("#e74c3c"),
                    isDarkTheme
                );
    cardsLayout->addWidget(easyxCard);

    contentLayout->addLayout(cardsLayout);
    contentLayout->addStretch();

    QWidget *infoWidget = new QWidget(contentWidget);
    QString infoBg = isDarkTheme ? "#34495e" : "white";
    QString infoBorder = isDarkTheme ? "#4a5f7f" : "#e1e8ed";
    infoWidget->setStyleSheet(
        QString("QWidget {"
                "  background: %1;"
                "  border-radius: 8px;"
                "  border: 1px solid %2;"
                "}").arg(infoBg, infoBorder)
    );

    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setContentsMargins(20, 15, 20, 15);
    infoLayout->setSpacing(8);

    QLabel *infoTitle = new QLabel(tr("提示"), infoWidget);
    QString infoTitleColor = isDarkTheme ? "#ecf0f1" : "#34495e";
    infoTitle->setStyleSheet(
        QString("QLabel {"
                "  font-size: 14px;"
                "  font-weight: bold;"
                "  color: %1;"
                "}").arg(infoTitleColor)
    );

    QLabel *infoText = new QLabel(
        tr("普通 C++ 文件：适合编写标准 C++ 程序\n"
           "EasyX 项目：自动配置图形库环境，适合图形编程"),
        infoWidget
    );
    QString infoTextColor = isDarkTheme ? "#95a5a6" : "#7f8c8d";
    infoText->setStyleSheet(
        QString("QLabel {"
                "  font-size: 13px;"
                "  color: %1;"
                "  line-height: 1.6;"
                "}").arg(infoTextColor)
    );

    infoLayout->addWidget(infoTitle);
    infoLayout->addWidget(infoText);

    contentLayout->addWidget(infoWidget);

    mainLayout->addWidget(contentWidget);

    QWidget *footerWidget = new QWidget(this);
    footerWidget->setFixedHeight(70);
    QString footerBg = isDarkTheme ? "#34495e" : "white";
    QString footerBorder = isDarkTheme ? "#4a5f7f" : "#e1e8ed";
    footerWidget->setStyleSheet(
        QString("QWidget {"
                "  background: %1;"
                "  border-top: 1px solid %2;"
                "}").arg(footerBg, footerBorder)
    );

    QHBoxLayout *footerLayout = new QHBoxLayout(footerWidget);
    footerLayout->setContentsMargins(40, 15, 40, 15);
    footerLayout->addStretch();

    cancelBtn = new QPushButton(tr("取消"), footerWidget);
    cancelBtn->setFixedSize(120, 40);
    cancelBtn->setCursor(Qt::PointingHandCursor);

    QString cancelBg = isDarkTheme ? "#7f8c8d" : "#ecf0f1";
    QString cancelHover = isDarkTheme ? "#95a5a6" : "#d5dbdb";
    QString cancelPressed = isDarkTheme ? "#6c7a7b" : "#bdc3c7";
    QString cancelColor = isDarkTheme ? "white" : "#2c3e50";

    cancelBtn->setStyleSheet(
        QString("QPushButton {"
                "  background-color: %1;"
                "  color: %2;"
                "  border: none;"
                "  border-radius: 6px;"
                "  font-size: 14px;"
                "  font-weight: 600;"
                "}"
                "QPushButton:hover {"
                "  background-color: %3;"
                "}"
                "QPushButton:pressed {"
                "  background-color: %4;"
                "}").arg(cancelBg, cancelColor, cancelHover, cancelPressed)
    );

    createBtn = new QPushButton(tr("创建文件"), footerWidget);
    createBtn->setFixedSize(120, 40);
    createBtn->setCursor(Qt::PointingHandCursor);
    createBtn->setEnabled(false);
    createBtn->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "              stop:0 #667eea, stop:1 #764ba2);"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "              stop:0 #5568d3, stop:1 #6a3f8f);"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "              stop:0 #4a5bc4, stop:1 #5d3680);"
        "}"
        "QPushButton:disabled {"
        "  background: #bdc3c7;"
        "  color: #7f8c8d;"
        "}"
    );

    footerLayout->addWidget(cancelBtn);
    footerLayout->addWidget(createBtn);

    mainLayout->addWidget(footerWidget);

    connect(cancelBtn, &QPushButton::clicked, this, &NewFileDialog::onCancelClicked);
    connect(createBtn, &QPushButton::clicked, this, &NewFileDialog::onCreateClicked);
}

QWidget* NewFileDialog::createProjectCard(const QString &title, const QString &description,
        const QString &iconText, FileType type,
        const QColor &accentColor, bool isDarkTheme)
{
    QPushButton *card = new QPushButton(this);
    card->setFixedSize(280, 200);
    card->setCursor(Qt::PointingHandCursor);
    card->setProperty("fileType", QVariant::fromValue(type));
    card->setProperty("accentColor", accentColor);
    card->setProperty("isDarkTheme", isDarkTheme);
    card->setCheckable(true);
    card->setAutoExclusive(false);

    QString cardBg = isDarkTheme ? "#34495e" : "white";
    QString cardBorder = isDarkTheme ? "#4a5f7f" : "#e1e8ed";
    QString cardHoverBg = isDarkTheme ? "#3d566e" : "#fafbfc";

    card->setStyleSheet(
        QString("QPushButton {"
                "  background: %1;"
                "  border: 2px solid %2;"
                "  border-radius: 12px;"
                "  text-align: left;"
                "  padding: 0px;"
                "}"
                "QPushButton:hover {"
                "  border-color: %3;"
                "  background: %4;"
                "}"
                "QPushButton:checked {"
                "  background: %5;"
                "  border: 3px solid %3;"
                "}").arg(cardBg, cardBorder, accentColor.name(),
                         cardHoverBg, accentColor.lighter(isDarkTheme ? 150 : 195).name())
    );

    QWidget *contentWidget = new QWidget(card);
    contentWidget->setStyleSheet("background: transparent; border: none;");
    contentWidget->setAttribute(Qt::WA_TransparentForMouseEvents);  // 让鼠标事件穿透

    QVBoxLayout *cardLayout = new QVBoxLayout(contentWidget);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(12);

    QLabel *iconLabel = new QLabel(iconText, contentWidget);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 48px;"
        "  background: transparent;"
        "  border: none;"
        "}"
    );
    iconLabel->setFixedHeight(60);

    QLabel *titleLabel = new QLabel(title, contentWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setWordWrap(true);
    QString titleColor = isDarkTheme ? "#ecf0f1" : "#2c3e50";
    titleLabel->setStyleSheet(
        QString("QLabel {"
                "  font-size: 16px;"
                "  font-weight: bold;"
                "  color: %1;"
                "  background: transparent;"
                "  border: none;"
                "}").arg(titleColor)
    );

    QLabel *descLabel = new QLabel(description, contentWidget);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setWordWrap(true);
    QString descColor = isDarkTheme ? "#95a5a6" : "#7f8c8d";
    descLabel->setStyleSheet(
        QString("QLabel {"
                "  font-size: 12px;"
                "  color: %1;"
                "  line-height: 1.5;"
                "  background: transparent;"
                "  border: none;"
                "}").arg(descColor)
    );

    cardLayout->addWidget(iconLabel);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(descLabel);
    cardLayout->addStretch();

    QVBoxLayout *mainCardLayout = new QVBoxLayout(card);
    mainCardLayout->setContentsMargins(0, 0, 0, 0);
    mainCardLayout->addWidget(contentWidget);

    connect(card, &QPushButton::clicked, this, [this, type]()
    {
        onCardClicked(type);
    });

    return card;
}

void NewFileDialog::updateCardSelection(QWidget *card)
{
    if (normalCard)
    {
        QPushButton *btn = qobject_cast<QPushButton*>(normalCard);
        if (btn)
        {
            btn->setChecked(false);
        }
    }

    if (easyxCard)
    {
        QPushButton *btn = qobject_cast<QPushButton*>(easyxCard);
        if (btn)
        {
            btn->setChecked(false);
        }
    }

    if (card)
    {
        QPushButton *btn = qobject_cast<QPushButton*>(card);
        if (btn)
        {
            btn->setChecked(true);
        }
    }
}

void NewFileDialog::onCardClicked(FileType type)
{
    selectedType = type;

    if (type == NormalCpp)
    {
        selectedCard = normalCard;
    }
    else if (type == EasyXProject)
    {
        selectedCard = easyxCard;
    }

    updateCardSelection(selectedCard);
    createBtn->setEnabled(true);
}

void NewFileDialog::onCreateClicked()
{
    if (selectedType != Cancelled)
    {
        accept();
    }
}

void NewFileDialog::onCancelClicked()
{
    selectedType = Cancelled;
    reject();
}

Q_DECLARE_METATYPE(NewFileDialog::FileType)

// AI 辅助生成
class Debugger : public QObject
{
    Q_OBJECT

public:
    explicit Debugger(QObject *parent = nullptr);
    ~Debugger();

    bool startDebugging(const QString &executablePath, const QString &sourcePath);
    void stopDebugging();
    void setBreakpoint(const QString &file, int lineNumber);
    void removeBreakpoint(const QString &file, int lineNumber);
    void removeAllBreakpoints();
    void continueExecution();
    void stepOver();
    void stepInto();
    void stepOut();
    void runToCursor(const QString &file, int lineNumber);
    void evaluateExpression(const QString &expression);
    void getLocalVariables();
    void getCallStack();
    void setVariable(const QString &name, const QString &value);

    bool isRunning() const { return debuggerProcess != nullptr && debuggerProcess->state() == QProcess::Running; }
    bool isPaused() const { return paused; }

    void setDebuggerPath(const QString &path) { debuggerPath = path; }
    QString getDebuggerPath() const { return debuggerPath; }

    QMap<QString, QSet<int>> getAllBreakpoints() const { return breakpoints; }
    bool hasBreakpoint(const QString &file, int line) const;

private:
    bool doStartDebugging(const QString &executablePath, const QString &sourcePath);
    QTimer *initTimer = nullptr;  // 初始化定时器

signals:
    void breakpointHit(const QString &file, int line, const QString &function);
    void debuggingStarted();
    void debuggingStopped();
    void debuggingFinished();
    void outputReceived(const QString &output);
    void errorReceived(const QString &error);
    void executionPaused(const QString &file, int line, const QString &reason);
    void executionResumed();
    void variablesUpdated(const QList<QPair<QString, QString>> &variables);
    void callStackUpdated(const QStringList &frames);
    void expressionEvaluated(const QString &expression, const QString &value);
    void breakpointAdded(const QString &file, int line);
    void breakpointRemoved(const QString &file, int line);

private:
    void launchDebugConsole();

private slots:
    void readDebuggerOutput();
    void readDebuggerError();
    void onDebuggerFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    // 添加工作线程
    QThread *workerThread;

    // 添加互斥锁，防止竞争条件
    QMutex processMutex;

    // 添加线程安全的命令队列
    QStringList commandQueue;

    // 添加一个函数来处理线程安全的调试器操作
    void sendCommandAsync(const QString &command);
    void processQueuedCommands();

private:
    QString createDebugBatchFile(const QString &executablePath);
    QProcess *consoleProcess = nullptr;
    QString debugBatPath;

private:
    QString extractField(const QString &content, const QString &fieldName);

private:
    QProcess *debuggerProcess;
    QString currentExecutable;
    QString currentSourceFile;
    QMap<QString, QSet<int>> breakpoints; // file -> set of lines
    QString debuggerPath;
    bool paused;
    QString outputBuffer;
    int nextBreakpointId;

    void sendCommand(const QString &command);
    void parseGDBOutput(const QString &output);
    void parseStoppedEvent(const QString &output);
    void parseVariables(const QString &output);
    void parseCallStack(const QString &output);
    void parseBreakpointInfo(const QString &output);
    QString extractValue(const QString &output, const QString &key);
};

Debugger::Debugger(QObject *parent)
    : QObject(parent), debuggerProcess(nullptr), debuggerPath(""),
      paused(false), nextBreakpointId(1), workerThread(nullptr)
{
    qDebug() << "Debugger 已初始化";
}

Debugger::~Debugger()
{
    // 首先停止调试
    stopDebugging();

    // 给一些时间让进程结束
    QThread::msleep(100);

    // 【修改】移除工作线程相关代码
    // 不再需要处理 workerThread
}

bool Debugger::startDebugging(const QString &executablePath, const QString &sourcePath)
{
    qDebug() << "startDebugging called:" << executablePath;

    if (debuggerProcess && debuggerProcess->state() == QProcess::Running) {
        debuggerProcess->kill();
        debuggerProcess->waitForFinished(500);
        delete debuggerProcess;
        debuggerProcess = nullptr;
    }

    // 清理之前的控制台进程
    if (consoleProcess) {
        consoleProcess->kill();
        consoleProcess->waitForFinished(500);
        delete consoleProcess;
        consoleProcess = nullptr;
    }

    currentExecutable = executablePath;
    currentSourceFile = sourcePath;
    paused = false;
    outputBuffer.clear();

    // 创建调试批处理文件
    debugBatPath = createDebugBatchFile(executablePath);
    if (debugBatPath.isEmpty()) {
        emit errorReceived(tr("无法创建调试启动脚本"));
        return false;
    }

    debuggerProcess = new QProcess(this);
    debuggerProcess->setProcessChannelMode(QProcess::SeparateChannels);

    // ========== 关键修改：设置环境变量 ==========
    QString appDir = QCoreApplication::applicationDirPath();
    QString mingwBin = appDir + "/mingw/bin";

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString currentPath = env.value("PATH");
    env.insert("PATH", mingwBin + ";" + currentPath);
    debuggerProcess->setProcessEnvironment(env);
    // ========== 修改结束 ==========

    connect(debuggerProcess, &QProcess::readyReadStandardOutput,
            this, &Debugger::readDebuggerOutput);
    connect(debuggerProcess, &QProcess::readyReadStandardError,
            this, &Debugger::readDebuggerError);
    connect(debuggerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Debugger::onDebuggerFinished);

    QFileInfo fileInfo(executablePath);
    debuggerProcess->setWorkingDirectory(fileInfo.absolutePath());

    QStringList args;
    args << "--interpreter=mi2" << "--quiet" << executablePath;

    QString gdbPath = debuggerPath.isEmpty() ? "gdb" : debuggerPath;
    debuggerProcess->start(gdbPath, args);

    if (initTimer) {
        initTimer->stop();
        delete initTimer;
    }

    initTimer = new QTimer(this);
    initTimer->setSingleShot(true);
    connect(initTimer, &QTimer::timeout, this, [this, sourcePath, executablePath]() {
        if (debuggerProcess && debuggerProcess->state() == QProcess::Running) {

            sendCommand("-gdb-set pagination off");
            sendCommand("-gdb-set confirm off");
            sendCommand("-gdb-set print pretty on");

            // 不使用 new-console，因为我们会用批处理包装

            QTimer::singleShot(200, this, [this, sourcePath]() {
                if (!debuggerProcess || debuggerProcess->state() != QProcess::Running) return;

                for (auto it = breakpoints.begin(); it != breakpoints.end(); ++it) {
                    const QString &file = it.key();
                    const QSet<int> &lines = it.value();
                    for (int line : lines) {
                        QFileInfo fi(file);
                        QString cmd = QString("-break-insert %1:%2").arg(fi.fileName()).arg(line);
                        sendCommand(cmd);
                    }
                }

                QTimer::singleShot(500, this, [this]() {
                    if (debuggerProcess && debuggerProcess->state() == QProcess::Running) {
                        // 启动独立控制台窗口运行程序
                        launchDebugConsole();

                        sendCommand("-exec-run");
                        emit debuggingStarted();
                        emit outputReceived(tr("调试器已启动"));
                    }
                });
            });
        }
    });

    initTimer->start(500);
    return true;
}

QString Debugger::createDebugBatchFile(const QString &executablePath)
{
#ifdef Q_OS_WIN
    QFileInfo fi(executablePath);
    QString batPath = fi.absolutePath() + "/debug_wrapper.bat";

    QFile batFile(batPath);
    if (batFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&batFile);
        out.setEncoding(QStringConverter::System);

        QString fullExePath = QDir::toNativeSeparators(executablePath);

        // ========== 关键修改：获取MinGW路径并设置环境变量 ==========
        QString appDir = QCoreApplication::applicationDirPath();
        QString mingwBin = QDir::toNativeSeparators(appDir + "/mingw/bin");
        // ========== 修改结束 ==========

        out << "@echo off\r\n";
        out << "chcp 65001 > nul\r\n";

        // ========== 关键修改：设置PATH环境变量 ==========
        out << "set PATH=" << mingwBin << ";%PATH%\r\n";
        // ========== 修改结束 ==========

        out << "cls\r\n";
        out << "echo ========================================\r\n";
        out << "echo            Debug Session\r\n";
        out << "echo ========================================\r\n";
        out << "echo.\r\n";
        out << "echo Program Output:\r\n";
        out << "echo ----------------------------------------\r\n";
        out << "\"" << fullExePath << "\"\r\n";
        out << "set EXIT_CODE=%ERRORLEVEL%\r\n";
        out << "echo.\r\n";
        out << "echo ----------------------------------------\r\n";
        out << "echo.\r\n";
        out << "echo Program exited with code %EXIT_CODE%\r\n";
        out << "echo Press any key to continue . . .\r\n";
        out << "pause > nul\r\n";

        batFile.close();
        return batPath;
    }
#endif
    return QString();
}

void Debugger::launchDebugConsole()
{
#ifdef Q_OS_WIN
    if (!debugBatPath.isEmpty() && QFile::exists(debugBatPath)) {
        // 使用 ShellExecute 方式启动，更可靠
        QFileInfo fi(currentExecutable);
        QString workDir = QDir::toNativeSeparators(fi.absolutePath());
        QString batPathNative = QDir::toNativeSeparators(debugBatPath);

        // 方法1：使用 ShellExecuteW 直接启动批处理文件
        HINSTANCE result = ShellExecuteW(
            NULL,
            L"open",
            reinterpret_cast<LPCWSTR>(batPathNative.utf16()),
            NULL,
            reinterpret_cast<LPCWSTR>(workDir.utf16()),
            SW_SHOWNORMAL
        );

        if ((intptr_t)result > 32) {
            qDebug() << "调试控制台窗口已启动";
        } else {
            qDebug() << "ShellExecute 失败，尝试备用方法";

            // 备用方法：使用 QProcess
            consoleProcess = new QProcess(this);
            consoleProcess->setWorkingDirectory(fi.absolutePath());

            // 构建完整命令行字符串
            QString cmdLine = QString("cmd.exe /c start \"Debug\" /wait \"%1\"").arg(batPathNative);
            consoleProcess->startCommand(cmdLine);
        }
    } else {
        qDebug() << "批处理文件不存在:" << debugBatPath;
        emit errorReceived(tr("无法创建调试启动脚本"));
    }
#endif
}

// 【新增】实际启动调试的函数
bool Debugger::doStartDebugging(const QString &executablePath, const QString &sourcePath)
{
    currentExecutable = executablePath;
    currentSourceFile = sourcePath;
    paused = false;
    outputBuffer.clear();

    debuggerProcess = new QProcess();
    debuggerProcess->setProcessChannelMode(QProcess::SeparateChannels);

    connect(debuggerProcess, &QProcess::readyReadStandardOutput,
            this, &Debugger::readDebuggerOutput);
    connect(debuggerProcess, &QProcess::readyReadStandardError,
            this, &Debugger::readDebuggerError);
    connect(debuggerProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Debugger::onDebuggerFinished);

    QFileInfo fileInfo(executablePath);
    debuggerProcess->setWorkingDirectory(fileInfo.absolutePath());

    QStringList args;
    args << "--interpreter=mi2" << "--quiet" << executablePath;

    QString gdbPath = debuggerPath.isEmpty() ? "gdb" : debuggerPath;

    qDebug() << "启动调试器:" << gdbPath << args;

    // 连接启动信号
    connect(debuggerProcess, &QProcess::started, this, [this]() {
        // 发送初始化命令（异步）
        QTimer::singleShot(100, this, [this]() {
            sendCommand("-gdb-set pagination off");
            sendCommand("-gdb-set confirm off");
            sendCommand("-gdb-set print pretty on");

            // 设置断点
            QMutexLocker locker(&processMutex);
            for (auto it = breakpoints.begin(); it != breakpoints.end(); ++it) {
                const QString &file = it.key();
                const QSet<int> &lines = it.value();
                for (int line : lines) {
                    sendCommand(QString("-break-insert %1:%2").arg(file).arg(line));
                }
            }

            // 延迟运行程序
            QTimer::singleShot(200, this, [this]() {
                sendCommand("-exec-run");
            });
        });

        emit debuggingStarted();
        emit outputReceived(tr("调试器已启动"));
    });

    // 连接错误信号
    connect(debuggerProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        Q_UNUSED(error)
        emit errorReceived(tr("调试器启动失败"));
    });

    debuggerProcess->start(gdbPath, args);

    return true;  // 立即返回，不等待
}

void Debugger::stopDebugging()
{
    qDebug() << "stopDebugging called";

    if (initTimer) {
        initTimer->stop();
        delete initTimer;
        initTimer = nullptr;
    }

    // 清理控制台进程
    if (consoleProcess) {
        consoleProcess->kill();
        consoleProcess->waitForFinished(500);
        delete consoleProcess;
        consoleProcess = nullptr;
    }

    // 删除临时批处理文件
    if (!debugBatPath.isEmpty() && QFile::exists(debugBatPath)) {
        QFile::remove(debugBatPath);
        debugBatPath.clear();
    }

    if (debuggerProcess) {
        if (debuggerProcess->state() == QProcess::Running) {
            debuggerProcess->write("-gdb-exit\n");

            QTimer::singleShot(1000, this, [this]() {
                if (debuggerProcess && debuggerProcess->state() == QProcess::Running) {
                    qDebug() << "强制终止调试器";
                    debuggerProcess->kill();
                }
            });
        } else {
            debuggerProcess->deleteLater();
            debuggerProcess = nullptr;
            paused = false;
            emit debuggingStopped();
        }
    } else {
        emit debuggingStopped();
    }
}


bool Debugger::hasBreakpoint(const QString &file, int line) const
{
    if (breakpoints.contains(file)) {
        return breakpoints[file].contains(line);
    }
    return false;
}

void Debugger::setBreakpoint(const QString &file, int line)
{
    // 只是记录断点，不发送命令给GDB
    breakpoints[file].insert(line);

    qDebug() << "记录断点:" << file << ":" << line;

    // 如果调试器正在运行，异步发送命令
    if (isRunning()) {
        // 使用 QMetaObject::invokeMethod 确保在正确的线程中执行
        QMetaObject::invokeMethod(this, [this, file, line]() {
            if (debuggerProcess && debuggerProcess->state() == QProcess::Running) {
                debuggerProcess->write(QString("-break-insert \"%1\":%2\n").arg(file).arg(line).toUtf8());
            }
        }, Qt::QueuedConnection);
    }

    // 异步发送信号
    QTimer::singleShot(0, this, [this, file, line]() {
        emit breakpointAdded(file, line);
        emit outputReceived(tr("断点已设置: %1:%2").arg(file).arg(line));
    });
}

void Debugger::removeBreakpoint(const QString &file, int lineNumber)
{
    if (breakpoints.contains(file)) {
        breakpoints[file].remove(lineNumber);
        if (breakpoints[file].isEmpty()) {
            breakpoints.remove(file);
        }
    }

    if (isRunning()) {
        // 获取断点编号并删除
        sendCommand(QString("-break-delete %1:%2").arg(file).arg(lineNumber));
    }

    emit breakpointRemoved(file, lineNumber);
    emit outputReceived(tr("断点已移除: %1:%2").arg(file).arg(lineNumber));
}

void Debugger::removeAllBreakpoints()
{
    breakpoints.clear();
    if (isRunning()) {
        sendCommand("-break-delete");
    }
    emit outputReceived(tr("所有断点已移除"));
}


void Debugger::continueExecution()
{
    if (isRunning() && paused) {
        paused = false;
        sendCommand("-exec-continue");
        emit executionResumed();
    }
}

// 修改 sendCommandAsync 函数
void Debugger::sendCommandAsync(const QString &command)
{
    // 使用 QTimer::singleShot 确保在正确的线程中执行
    QTimer::singleShot(0, this, [this, command]() {
        QMutexLocker locker(&processMutex);

        if (debuggerProcess && debuggerProcess->state() == QProcess::Running) {
            debuggerProcess->write((command + "\n").toUtf8());
            qDebug() << "异步发送GDB命令:" << command;
        }
    });
}

void Debugger::processQueuedCommands()
{
    QMutexLocker locker(&processMutex);
    if (commandQueue.isEmpty() || !debuggerProcess) {
        return;
    }

    QString command = commandQueue.takeFirst();

    if (debuggerProcess->state() == QProcess::Running) {
        qint64 written = debuggerProcess->write((command + "\n").toUtf8());

        // 不等待写入完成，立即返回
        if (written > 0) {
            // 立即刷新缓冲区，但不阻塞
            debuggerProcess->waitForBytesWritten(0);
        }

        qDebug() << "发送GDB命令:" << command << "字节数:" << written;
    }

    // 处理下一个命令
    if (!commandQueue.isEmpty()) {
        QTimer::singleShot(1, this, &Debugger::processQueuedCommands);
    }
}

void Debugger::stepOver()
{
    if (isRunning() && paused) {
        sendCommand("-exec-next");
    }
}

void Debugger::stepInto()
{
    if (isRunning() && paused) {
        sendCommand("-exec-step");
    }
}

void Debugger::stepOut()
{
    if (isRunning() && paused) {
        sendCommand("-exec-finish");
    }
}

void Debugger::runToCursor(const QString &file, int lineNumber)
{
    if (isRunning()) {
        sendCommand(QString("-exec-until %1:%2").arg(file).arg(lineNumber));
        emit outputReceived(tr("运行到光标位置: %1:%2").arg(file).arg(lineNumber));
    }
}

void Debugger::evaluateExpression(const QString &expression)
{
    if (isRunning() && paused) {
        sendCommand(QString("-data-evaluate-expression \"%1\"").arg(expression));
    }
}

void Debugger::getLocalVariables()
{
    if (isRunning() && paused) {
        sendCommand("-stack-list-locals --all-values");
    }
}

void Debugger::getCallStack()
{
    if (isRunning() && paused) {
        sendCommand("-stack-list-frames");
    }
}

void Debugger::setVariable(const QString &name, const QString &value)
{
    if (isRunning() && paused) {
        sendCommand(QString("-gdb-set var %1=%2").arg(name).arg(value));
        emit outputReceived(tr("设置变量: %1 = %2").arg(name).arg(value));
        getLocalVariables(); // 刷新变量列表
    }
}

void Debugger::sendCommand(const QString &command)
{
    if (!debuggerProcess) {
        qDebug() << "调试器未初始化，无法发送命令:" << command;
        emit errorReceived(tr("错误: 调试器未初始化"));
        return;
    }

    if (debuggerProcess->state() != QProcess::Running) {
        qDebug() << "调试器未运行，无法发送命令:" << command;
        // 不发送错误信号，避免干扰用户
        return;
    }

    debuggerProcess->write((command + "\n").toUtf8());
    qDebug() << "发送GDB命令:" << command;

    // 让事件循环处理
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 10);
}

void Debugger::readDebuggerOutput()
{
    if (!debuggerProcess) return;

    QString output = QString::fromUtf8(debuggerProcess->readAllStandardOutput());
    outputBuffer += output;

    // 按行处理输出
    QStringList lines = outputBuffer.split('\n');
    outputBuffer = lines.takeLast(); // 保留不完整的行

    for (const QString &line : lines) {
        if (!line.trimmed().isEmpty()) {
            parseGDBOutput(line);
        }
    }
}

void Debugger::readDebuggerError()
{
    if (!debuggerProcess) return;

    QString error = QString::fromUtf8(debuggerProcess->readAllStandardError());
    if (!error.trimmed().isEmpty()) {
        emit errorReceived(error);
    }
}

void Debugger::onDebuggerFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)

    qDebug() << "调试器进程结束，退出码:" << exitCode;

    paused = false;

    // 安全地清理进程
    if (debuggerProcess) {
        debuggerProcess->deleteLater();
        debuggerProcess = nullptr;
    }

    // 清理控制台进程
    if (consoleProcess) {
        // 不立即关闭，让用户看到退出信息
        // consoleProcess 会在用户按键后自动关闭
        consoleProcess->deleteLater();
        consoleProcess = nullptr;
    }

    // 删除临时批处理文件（延迟删除，确保控制台已读取）
    if (!debugBatPath.isEmpty()) {
        QString pathToDelete = debugBatPath;
        debugBatPath.clear();
        QTimer::singleShot(5000, this, [pathToDelete]() {
            if (QFile::exists(pathToDelete)) {
                QFile::remove(pathToDelete);
            }
        });
    }

    // 清理定时器
    if (initTimer) {
        initTimer->stop();
        delete initTimer;
        initTimer = nullptr;
    }

    emit debuggingStopped();
    emit outputReceived(tr("调试会话已结束"));
}

void Debugger::parseGDBOutput(const QString &output)
{
    QString trimmed = output.trimmed();

    // 调试输出 - 帮助诊断问题
    if (!trimmed.isEmpty() && !trimmed.startsWith("(gdb)")) {
        qDebug() << "[GDB 原始输出]" << trimmed;
    }

    // 控制台输出
    if (trimmed.startsWith("~")) {
        QString text = trimmed.mid(2, trimmed.length() - 3);
        text.replace("\\n", "\n").replace("\\t", "\t").replace("\\\"", "\"");
        emit outputReceived(text);
    }
    // 程序停止
    else if (trimmed.startsWith("*stopped")) {
        parseStoppedEvent(trimmed);
    }
    // 程序运行中
    else if (trimmed.startsWith("*running")) {
        paused = false;
        emit executionResumed();
    }
    // 命令完成
    else if (trimmed.startsWith("^done")) {
        if (trimmed.contains("stack=")) {
            qDebug() << "[GDB] 检测到调用栈响应，调用 parseCallStack";
            parseCallStack(trimmed);
        }
        // 解析局部变量
        if (trimmed.contains("locals=")) {
            qDebug() << "收到局部变量响应";
            parseVariables(trimmed);
        }
        // 解析调用栈
        else if (trimmed.contains("stack=")) {
            qDebug() << "收到调用栈响应";
            parseCallStack(trimmed);
        }
        // 解析函数参数
        else if (trimmed.contains("stack-args=")) {
            qDebug() << "收到函数参数响应";
            parseVariables(trimmed);  // 参数也当作变量处理
        }
        // 表达式求值结果
        else if (trimmed.contains("value=")) {
            QString value = extractValue(trimmed, "value");
            emit expressionEvaluated("", value);
        }
    }
    // 错误
    else if (trimmed.startsWith("^error")) {
        QString msg = extractValue(trimmed, "msg");
        emit errorReceived(tr("GDB错误: %1").arg(msg));
    }
    // 断点信息
    else if (trimmed.startsWith("=breakpoint-created") || trimmed.startsWith("=breakpoint-modified")) {
        parseBreakpointInfo(trimmed);
    }
}

void Debugger::parseStoppedEvent(const QString &output)
{
    QString reason = extractValue(output, "reason");
    QString file = extractValue(output, "fullname");
    if (file.isEmpty()) {
        file = extractValue(output, "file");
    }
    QString lineStr = extractValue(output, "line");
    QString func = extractValue(output, "func");

    int line = lineStr.toInt();

    qDebug() << "程序停止 - 原因:" << reason << "文件:" << file << "行:" << line << "函数:" << func;

    // 程序正常退出
    if (reason == "exited-normally" || reason == "exited") {
        paused = false;
        QString exitCode = extractValue(output, "exit-code");
        if (reason == "exited-normally") {
            emit outputReceived(tr("程序正常退出"));
        } else {
            emit outputReceived(tr("程序退出，代码: %1").arg(exitCode));
        }
        emit debuggingFinished();
        return;
    }

    // 程序被信号终止
    if (reason == "exited-signalled") {
        paused = false;
        QString signalName = extractValue(output, "signal-name");
        emit errorReceived(tr("程序被信号终止: %1").arg(signalName));
        emit debuggingFinished();
        return;
    }

    // 其他情况，程序暂停
    paused = true;

    if (reason == "breakpoint-hit") {
        emit breakpointHit(file, line, func);
        emit outputReceived(tr("断点命中: %1 第 %2 行 (%3)").arg(file).arg(line).arg(func));
    }
    else if (reason == "end-stepping-range" || reason == "function-finished") {
        emit outputReceived(tr("停止于: %1 第 %2 行 (%3)").arg(file).arg(line).arg(func));
    }
    else if (reason == "signal-received") {
        QString signalName = extractValue(output, "signal-name");
        QString signalMeaning = extractValue(output, "signal-meaning");
        emit errorReceived(tr("收到信号: %1 (%2)").arg(signalName).arg(signalMeaning));
    }

    emit executionPaused(file, line, reason);

    // ★★★ 关键：延迟获取变量，确保程序完全停止 ★★★
    if (paused) {
        QTimer::singleShot(100, this, [this]() {
            if (paused) {
                qDebug() << "请求局部变量...";
                sendCommand("-stack-list-locals --all-values");

                QTimer::singleShot(50, this, [this]() {
                    if (paused) {
                        qDebug() << "请求调用栈...";
                        sendCommand("-stack-list-frames");
                    }
                });
            }
        });
    }
}

void Debugger::parseVariables(const QString &output)
{
    QList<QPair<QString, QString>> variables;

    qDebug() << "解析变量输出:" << output;

    // 使用自定义分隔符 REGEX 来避免 )" 被误解析
    // 匹配格式: {name="a",value="10"} 或 {name="a",type="int",value="10"}
    QRegularExpression varPattern(
        R"REGEX(\{[^}]*name="([^"]+)"[^}]*value="([^"]*)"[^}]*\})REGEX"
    );

    QRegularExpressionMatchIterator matches = varPattern.globalMatch(output);

    while (matches.hasNext()) {
        QRegularExpressionMatch match = matches.next();
        QString name = match.captured(1);
        QString value = match.captured(2);

        // 处理转义字符
        value.replace("\\\"", "\"");
        value.replace("\\n", "\n");
        value.replace("\\t", "\t");
        value.replace("\\\\", "\\");

        qDebug() << "解析到变量:" << name << "=" << value;
        variables.append(qMakePair(name, value));
    }

    // 如果上面的模式没匹配到，尝试另一种格式
    if (variables.isEmpty()) {
        // 使用普通字符串（需要双重转义）
        QRegularExpression altPattern("name=\"([^\"]+)\"");
        QRegularExpression valPattern("value=\"([^\"]*)\"");

        QStringList names, values;

        QRegularExpressionMatchIterator nameMatches = altPattern.globalMatch(output);
        while (nameMatches.hasNext()) {
            names.append(nameMatches.next().captured(1));
        }

        QRegularExpressionMatchIterator valMatches = valPattern.globalMatch(output);
        while (valMatches.hasNext()) {
            values.append(valMatches.next().captured(1));
        }

        int count = qMin(names.size(), values.size());
        for (int i = 0; i < count; i++) {
            QString value = values[i];
            value.replace("\\\"", "\"");
            variables.append(qMakePair(names[i], value));
            qDebug() << "备用模式解析到变量:" << names[i] << "=" << value;
        }
    }

    if (!variables.isEmpty()) {
        emit variablesUpdated(variables);
    } else {
        qDebug() << "未能解析到任何变量，原始输出:" << output;
    }
}

void Debugger::parseCallStack(const QString &output)
{
    QStringList stack;

    qDebug() << "=== 调用栈原始输出 ===" << output;

    // 方法1: 解析 GDB MI 格式
    // ^done,stack=[frame={level="0",addr="0x...",func="main",file="test.cpp",fullname="...",line="5",arch="..."}]

    // 提取 stack=[...] 部分
    int stackStart = output.indexOf("stack=[");
    if (stackStart != -1) {
        int stackEnd = output.lastIndexOf("]");
        QString stackContent = output.mid(stackStart, stackEnd - stackStart + 1);
        qDebug() << "调用栈内容:" << stackContent;

        // 分割每个 frame={...}
        int pos = 0;
        while ((pos = stackContent.indexOf("frame={", pos)) != -1) {
            int frameEnd = stackContent.indexOf("}", pos);
            if (frameEnd == -1) break;

            QString frameContent = stackContent.mid(pos, frameEnd - pos + 1);
            qDebug() << "帧内容:" << frameContent;

            // 提取各个字段
            QString level = extractField(frameContent, "level");
            QString func = extractField(frameContent, "func");
            QString file = extractField(frameContent, "file");
            QString line = extractField(frameContent, "line");

            QString frameInfo;
            if (!file.isEmpty() && !line.isEmpty()) {
                QString fileName = QFileInfo(file).fileName();
                frameInfo = QString("#%1  %2() at %3:%4").arg(level, func, fileName, line);
            } else {
                frameInfo = QString("#%1  %2()").arg(level, func);
            }

            if (!frameInfo.isEmpty()) {
                stack.append(frameInfo);
                qDebug() << "添加调用栈帧:" << frameInfo;
            }

            pos = frameEnd + 1;
        }
    }

    // 方法2: 如果方法1失败，尝试直接提取 func 字段
    if (stack.isEmpty() && output.contains("func=")) {
        QStringList funcs;
        QStringList levels;
        QStringList lines;

        // 提取所有 func
        int pos = 0;
        while ((pos = output.indexOf("func=\"", pos)) != -1) {
            int start = pos + 6;
            int end = output.indexOf("\"", start);
            if (end != -1) {
                funcs.append(output.mid(start, end - start));
            }
            pos = end + 1;
        }

        // 提取所有 level
        pos = 0;
        while ((pos = output.indexOf("level=\"", pos)) != -1) {
            int start = pos + 7;
            int end = output.indexOf("\"", start);
            if (end != -1) {
                levels.append(output.mid(start, end - start));
            }
            pos = end + 1;
        }

        // 提取所有 line
        pos = 0;
        while ((pos = output.indexOf("line=\"", pos)) != -1) {
            int start = pos + 6;
            int end = output.indexOf("\"", start);
            if (end != -1) {
                lines.append(output.mid(start, end - start));
            }
            pos = end + 1;
        }

        for (int i = 0; i < funcs.size(); i++) {
            QString level = (i < levels.size()) ? levels[i] : QString::number(i);
            QString line = (i < lines.size()) ? lines[i] : "";

            QString frameInfo;
            if (!line.isEmpty()) {
                frameInfo = QString("#%1  %2() 行 %3").arg(level, funcs[i], line);
            } else {
                frameInfo = QString("#%1  %2()").arg(level, funcs[i]);
            }
            stack.append(frameInfo);
        }
    }

    qDebug() << "解析到的调用栈:" << stack;

    if (!stack.isEmpty()) {
        emit callStackUpdated(stack);
    } else {
        // 即使解析失败，也发送一个默认值
        stack.append(tr("#0  (未知)"));
        emit callStackUpdated(stack);
    }
}

// 辅助函数：从字符串中提取字段值
QString Debugger::extractField(const QString &content, const QString &fieldName)
{
    QString pattern = fieldName + "=\"";
    int start = content.indexOf(pattern);
    if (start == -1) return QString();

    start += pattern.length();
    int end = content.indexOf("\"", start);
    if (end == -1) return QString();

    return content.mid(start, end - start);
}

void Debugger::parseBreakpointInfo(const QString &output)
{
    QString file = extractValue(output, "fullname");
    if (file.isEmpty()) {
        file = extractValue(output, "file");
    }
    QString lineStr = extractValue(output, "line");
    int line = lineStr.toInt();

    if (!file.isEmpty() && line > 0) {
        qDebug() << "断点信息:" << file << ":" << line;
    }
}

QString Debugger::extractValue(const QString &output, const QString &key)
{
    // 匹配格式: key="value"
    QString pattern = QString("%1=\"([^\"]*)\"").arg(QRegularExpression::escape(key));
    QRegularExpression regex(pattern);
    QRegularExpressionMatch match = regex.match(output);

    if (match.hasMatch()) {
        QString value = match.captured(1);
        // 处理转义
        value.replace("\\\"", "\"");
        value.replace("\\n", "\n");
        value.replace("\\\\", "\\");
        return value;
    }

    return QString();
}

// 代码补全
class CodeCompleter : public QObject
{
    Q_OBJECT

public:
    explicit CodeCompleter(QObject *parent = nullptr);
    void setEditor(CodeEditor *editor);
    QStringList getCompletions(const QString &prefix, int cursorPosition);
    void reloadCustomCompletions();

private:
    CodeEditor *codeEditor;
    QStringList cppKeywords;
    QStringList cppTypes;
    QStringList cppFunctions;
    QStringList customCompletions;

    void initializeKeywords();
    void extractLocalSymbols(const QString &text, int cursorPosition);
};

CodeCompleter::CodeCompleter(QObject *parent)
    : QObject(parent), codeEditor(nullptr)
{
    initializeKeywords();
    reloadCustomCompletions();
}

void CodeCompleter::reloadCustomCompletions()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    customCompletions = settings.value("CustomCompletions", QStringList()).toStringList();
}

void CodeCompleter::setEditor(CodeEditor *editor)
{
    codeEditor = editor;
}

void CodeCompleter::initializeKeywords()
{
    cppKeywords =
    {
        "auto", "bool", "break", "case", "catch", "char", "class", "const",
        "continue", "default", "delete", "do", "double", "else", "enum", "explicit",
        "extern", "false", "float", "for", "if", "int", "long", "namespace",
        "new", "nullptr", "operator", "private", "protected", "public", "return",
        "short", "signed", "sizeof", "static", "struct", "switch", "template",
        "this", "throw", "true", "try", "typedef", "typename", "union", "unsigned",
        "using", "virtual", "void", "volatile", "while", "include"
    };

    cppTypes =
    {
        "int", "float", "double", "char", "bool", "void", "short", "long",
        "unsigned", "size_t",

        "vector", "list", "deque", "array", "forward_list", "priority_queue",

        "map", "set", "multimap", "multiset",
        "unordered_map", "unordered_set", "unordered_multimap", "unordered_multiset",

        "stack", "queue", "priority_queue",

        "string", "wstring", "u16string", "u32string",

        "bitset", "valarray",

        "unique_ptr", "shared_ptr", "weak_ptr",

        "pair", "tuple", "function", "optional", "variant"

        "fstream", "ifstream", "ofstream", "FILE",

        "include"
    };

    cppFunctions =
    {
        "cout", "cin", "endl", "printf", "scanf",

        "strlen", "strcpy", "strcmp", "memset", "memcpy",
        "getline", "to_string", "stoi", "stod",

        "malloc", "free", "new", "delete",

        "abs", "sqrt", "pow", "sin", "cos", "tan", "min", "max", "setprecision",

        "freopen", "fopen", "fclose", "fread", "fwrite", "fprintf", "fscanf",
        "fgetc", "fputc", "fgets", "fputs", "fseek", "ftell", "rewind",
        "feof", "ferror", "clearerr",

        "sort", "find", "for_each", "transform", "copy", "swap", "next_permutation", "stable_sort",
        "lower_bound", "upper_bound",

        "begin", "end", "size", "empty", "push_back", "pop_back",
        "insert", "erase", "clear", "resize", "push",

        "make_shared", "make_unique",

        "static_cast", "dynamic_cast",

        "assert", "throw", "catch", "try"
    };

    cppFunctions << "NULL" << "true" << "false" << "nullptr" << "fixed"
                 << "stdin" << "stdout" << "stderr"
                 << "SEEK_SET" << "SEEK_CUR" << "SEEK_END"
                 << "EOF";
}

QStringList CodeCompleter::getCompletions(const QString &prefix, int cursorPosition)
{
    Q_UNUSED(cursorPosition)

    if (prefix.isEmpty())
    {
        return QStringList();
    }

    QStringList completions;
    QString lowerPrefix = prefix.toLower();

    for (const QString &keyword : cppKeywords)
    {
        if (keyword.toLower().startsWith(lowerPrefix))
        {
            completions.append(keyword);
        }
    }

    for (const QString &type : cppTypes)
    {
        if (type.toLower().startsWith(lowerPrefix))
        {
            completions.append(type);
        }
    }

    for (const QString &func : cppFunctions)
    {
        if (func.toLower().startsWith(lowerPrefix))
        {
            completions.append(func);
        }
    }

    QStringList qtClasses =
    {
        "QApplication", "QMainWindow", "QWidget", "QDialog", "QLabel",
        "QPushButton", "QLineEdit", "QTextEdit", "QPlainTextEdit",
        "QVBoxLayout", "QHBoxLayout", "QGridLayout", "QFormLayout",
        "QMenu", "QMenuBar", "QToolBar", "QStatusBar", "QAction",
        "QString", "QList", "QVector", "QMap", "QHash", "QSet",
        "QFile", "QDir", "QFileInfo", "QSettings", "QProcess",
        "QTimer", "QDateTime", "QDate", "QTime", "QUrl"
    };

    for (const QString &qtClass : qtClasses)
    {
        if (qtClass.toLower().startsWith(lowerPrefix))
        {
            completions.append(qtClass);
        }
    }

    for (const QString &custom : customCompletions)
    {
        if (custom.toLower().startsWith(lowerPrefix))
        {
            completions.append(custom);
        }
    }

    completions.sort(Qt::CaseInsensitive);
    completions.removeDuplicates();

    return completions;
}

// 因放在 CodeEditor 类中会报错（CodeCompleter 未声明），就把函数放在此处
void CodeEditor::setCompleter(CodeCompleter *comp)
{
    completer = comp;
    if (completer)
    {
        completer->setEditor(this);
    }
}

void CodeEditor::showCompletion(const QStringList &completions)
{
    if (completions.isEmpty() || !completionEnabled || !completionPopup || !completionList)
    {
        hideCompletion();
        return;
    }

    QStringListModel *model = qobject_cast<QStringListModel*>(completionList->model());
    if (!model)
    {
        model = new QStringListModel(completionPopup);
        completionList->setModel(model);
    }
    model->setStringList(completions);

    if (completionList->model()->rowCount() > 0)
    {
        completionList->setCurrentIndex(completionList->model()->index(0, 0));
    }

    QTextCursor cursor = textCursor();
    QRect cursorRect = this->cursorRect(cursor);
    QPoint globalPos = mapToGlobal(cursorRect.bottomLeft());

    int itemHeight = 24;
    int maxVisibleItems = 8;
    int visibleItems = qMin(completions.size(), maxVisibleItems);
    int popupHeight = visibleItems * itemHeight + 4;
    int popupWidth = 250;

    completionPopup->setFixedSize(popupWidth, popupHeight);

    QScreen *screen = QGuiApplication::screenAt(globalPos);
    if (screen)
    {
        QRect screenRect = screen->availableGeometry();
        if (globalPos.y() + popupHeight > screenRect.bottom())
        {
            globalPos.setY(mapToGlobal(cursorRect.topLeft()).y() - popupHeight);
        }
        if (globalPos.x() + popupWidth > screenRect.right())
        {
            globalPos.setX(screenRect.right() - popupWidth);
        }
    }

    completionPopup->move(globalPos);

    if (!completionPopup->isVisible())
    {
        completionPopup->show();
    }

    this->setFocus(Qt::OtherFocusReason);
}

void CodeEditor::insertCompletion(const QString &completion)
{
    if (completion.isEmpty())
    {
        return;
    }

    hideCompletion();

    QTextCursor cursor = textCursor();

    int prefixLen = completionPrefix.length();
    for (int i = 0; i < prefixLen; ++i)
    {
        cursor.deletePreviousChar();
    }

    cursor.insertText(completion);
    setTextCursor(cursor);

    this->setFocus(Qt::OtherFocusReason);
}

void CodeEditor::hideCompletion()
{
    if (completionPopup)
    {
        completionPopup->hide();
        completionPopup->setVisible(false);
    }
}

QString CodeEditor::getWordUnderCursor() const
{
    QTextCursor cursor = textCursor();

    int currentPos = cursor.position();
    QTextBlock block = cursor.block();
    QString blockText = block.text();
    int posInBlock = currentPos - block.position();

    int start = posInBlock;
    while (start > 0)
    {
        QChar c = blockText[start - 1];
        if (c.isLetterOrNumber() || c == '_')
        {
            start--;
        }
        else
        {
            break;
        }
    }

    QString word = blockText.mid(start, posInBlock - start);

    return word;
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_BraceRight ||
            (!event->text().isEmpty() && event->text()[0] == '}'))
    {

        bool shouldHandleBrace = autoIndent && !(competitionMode && comp_disableAutoIndent);

        if (shouldHandleBrace)
        {
            QTextCursor cur = textCursor();
            QString txt = cur.block().text();
            int pos = cur.positionInBlock();
            int lineLength = txt.length();

            if (txt.trimmed() == "}" && pos == lineLength)
            {
                cur.insertText("}");
                setTextCursor(cur);
                event->accept();
                return;
            }

            if (document()->characterAt(cur.position()) == '}')
            {
                cur.movePosition(QTextCursor::Right);
                setTextCursor(cur);
                event->accept();
                return;
            }

            if (txt.left(pos).trimmed().isEmpty())
            {
                int targetIndent = -1;
                int braceBalance = 0;
                int currentBlockNum = cur.blockNumber();

                for (int i = currentBlockNum; i >= 0; --i)
                {
                    QString blockText = document()->findBlockByNumber(i).text();
                    for (int j = blockText.length() - 1; j >= 0; --j)
                    {
                        if (i == currentBlockNum && j >= pos)
                        {
                            continue;
                        }
                        if (blockText[j] == '}')
                        {
                            braceBalance++;
                        }
                        else if (blockText[j] == '{')
                        {
                            braceBalance--;
                            if (braceBalance <= 0)
                            {
                                targetIndent = 0;
                                for (int k = 0; k < blockText.length(); ++k)
                                {
                                    if (blockText[k] == ' ')
                                    {
                                        targetIndent++;
                                    }
                                    else if (blockText[k] == '	')
                                    {
                                        targetIndent += indentSize;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if (targetIndent != -1)
                    {
                        break;
                    }
                }

                int newSpaces = targetIndent;
                if (newSpaces == -1)
                {
                    newSpaces = 0;
                    for (int i = 0; i < pos; ++i)
                    {
                        if (txt[i] == ' ')
                        {
                            newSpaces++;
                        }
                        else if (txt[i] == '	')
                        {
                            newSpaces += indentSize;
                        }
                    }
                }

                cur.beginEditBlock();
                QString textAfter = txt.mid(pos);
                cur.movePosition(QTextCursor::StartOfBlock);
                cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
                cur.insertText(QString(newSpaces, ' ') + "}" + textAfter);

                if (textAfter.startsWith('}'))
                {
                    cur.setPosition(cur.block().position() + newSpaces + 1);
                    cur.deleteChar();
                }
                else
                {
                    cur.setPosition(cur.block().position() + newSpaces + 1);
                }
                cur.endEditBlock();

                setTextCursor(cur);
                event->accept();
                return;
            }
        }
    }

    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
            autoIndent && !(competitionMode && comp_disableAutoIndent))
    {

        QTextCursor cursor = textCursor();
        int pos = cursor.positionInBlock();
        QString fullLineText = cursor.block().text();

        if (pos > 0 && pos < fullLineText.length() &&
                fullLineText[pos - 1] == '{' && fullLineText[pos] == '}')
        {

            int currentIndent = 0;
            for (int i = 0; i < fullLineText.length(); ++i)
            {
                if (fullLineText[i] == ' ')
                {
                    currentIndent++;
                }
                else if (fullLineText[i] == '	')
                {
                    currentIndent += indentSize;
                }
                else
                {
                    break;
                }
            }

            QString baseIndent = QString(currentIndent, ' ');
            QString innerIndent = QString(currentIndent + indentSize, ' ');

            cursor.beginEditBlock();
            cursor.insertText("\n" + innerIndent);
            int savedPos = cursor.position();
            cursor.insertText("\n" + baseIndent);
            cursor.setPosition(savedPos);
            cursor.endEditBlock();

            setTextCursor(cursor);
            emit cursorPositionChanged();
            event->accept();
            return;
        }
    }

    if (completionPopup && completionPopup->isVisible() && completionList)
    {
        switch (event->key())
        {
        case Qt::Key_Up:
        {
            int currentRow = completionList->currentIndex().row();
            int rowCount = completionList->model()->rowCount();
            if (currentRow > 0)
            {
                completionList->setCurrentIndex(completionList->model()->index(currentRow - 1, 0));
            }
            else if (rowCount > 0)
            {
                completionList->setCurrentIndex(completionList->model()->index(rowCount - 1, 0));
            }
            event->accept();
            return;
        }
        case Qt::Key_Down:
        {
            int currentRow = completionList->currentIndex().row();
            int rowCount = completionList->model()->rowCount();
            if (currentRow < rowCount - 1)
            {
                completionList->setCurrentIndex(completionList->model()->index(currentRow + 1, 0));
            }
            else if (rowCount > 0)
            {
                completionList->setCurrentIndex(completionList->model()->index(0, 0));
            }
            event->accept();
            return;
        }
        case Qt::Key_Enter:
        case Qt::Key_Return:
            hideCompletion();
            break;
        case Qt::Key_Tab:
        {
            QModelIndex index = completionList->currentIndex();
            if (index.isValid())
            {
                insertCompletion(index.data().toString());
            }
            event->accept();
            return;
        }
        case Qt::Key_Escape:
            hideCompletion();
            event->accept();
            return;
        case Qt::Key_PageUp:
        {
            int currentRow = completionList->currentIndex().row();
            int newRow = qMax(0, currentRow - 5);
            completionList->setCurrentIndex(completionList->model()->index(newRow, 0));
            event->accept();
            return;
        }
        case Qt::Key_PageDown:
        {
            int currentRow = completionList->currentIndex().row();
            int rowCount = completionList->model()->rowCount();
            int newRow = qMin(rowCount - 1, currentRow + 5);
            completionList->setCurrentIndex(completionList->model()->index(newRow, 0));
            event->accept();
            return;
        }
        default:
            break;
        }
    }

    if (event->key() == Qt::Key_Tab && !(completionPopup && completionPopup->isVisible()))
    {
        QTextCursor cursor = textCursor();
        if (cursor.hasSelection())
        {
            int startPos = cursor.selectionStart();
            int endPos   = cursor.selectionEnd();

            cursor.setPosition(startPos);
            int startBlock = cursor.blockNumber();
            cursor.setPosition(endPos);
            int endBlock = cursor.blockNumber();
            if (endBlock < startBlock)
            {
                std::swap(startBlock, endBlock);
            }

            int totalInserted = (endBlock - startBlock + 1) * indentSize;

            cursor.beginEditBlock();
            for (int blockNum = endBlock; blockNum >= startBlock; --blockNum)
            {
                QTextBlock block = document()->findBlockByNumber(blockNum);
                if (block.isValid())
                {
                    cursor.setPosition(block.position());
                    cursor.insertText(QString(indentSize, ' '));
                }
            }
            cursor.endEditBlock();

            int newStartPos = startPos + indentSize;
            int newEndPos = endPos + totalInserted;

            cursor.setPosition(newStartPos);
            cursor.setPosition(newEndPos, QTextCursor::KeepAnchor);
            setTextCursor(cursor);

            event->accept();
            return;
        }
    }

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        QTextCursor cursor = textCursor();
        int currentColumn = cursor.positionInBlock();

        QPlainTextEdit::keyPressEvent(event);

        cursor = textCursor();
        QTextBlock block = cursor.block();
        QString lineText = block.text();

        if (lineText.trimmed().isEmpty() && lineText.length() > 0)
        {
            int indentEnd = 0;
            for (int i = 0; i < lineText.length(); ++i)
            {
                if (lineText[i].isSpace())
                {
                    indentEnd = i + 1;
                }
                else
                {
                    break;
                }
            }

            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, indentEnd);
            setTextCursor(cursor);
        }

        emit cursorPositionChanged();
        return;
    }

    if (competitionMode)
    {
        hideCompletion();
    }

    QChar key = event->text().isEmpty() ? QChar() : event->text().at(0);

    bool shouldAutoBrackets = autoBrackets && !(competitionMode && comp_disableAutoBrackets);
    if (shouldAutoBrackets)
    {
        handleAutoBrackets(event);
        if (event->isAccepted())
        {
            return;
        }
    }

    bool shouldAutoQuotes = autoQuotes && !(competitionMode && comp_disableAutoQuotes);
    if (shouldAutoQuotes)
    {
        handleAutoQuotes(event);
        if (event->isAccepted())
        {
            return;
        }
    }

    if (codeBeautify && !competitionMode)
    {
        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        QString lineText = block.text().trimmed();

        bool inCommentOrString = isPositionInCommentOrString();

        if (!inCommentOrString)
        {
            if (key == ';')
            {
                hideCompletion();

                QTextCursor cursor = textCursor();
                QTextBlock block = cursor.block();
                QString lineText = block.text();
                int pos = cursor.positionInBlock();

                int balance = 0;
                for (int i = 0; i < pos; ++i)
                {
                    if (lineText[i] == '(')
                    {
                        balance++;
                    }
                    else if (lineText[i] == ')')
                    {
                        balance--;
                    }
                }

                bool skipBeautify = false;
                if (balance > 0)
                {
                    skipBeautify = true;
                }
                else
                {
                    if (isControlStructureLine(lineText))
                    {
                        int openParen = -1, closeParen = -1;
                        QRegularExpression controlPattern(R"(^\s*(if|else\s+if|else|for|while|switch|do)\b)");
                        QRegularExpressionMatch match = controlPattern.match(lineText);
                        if (match.hasMatch())
                        {
                            int keywordEnd = match.capturedEnd();
                            for (int i = keywordEnd; i < lineText.length(); ++i)
                            {
                                if (lineText[i] == '(')
                                {
                                    openParen = i;
                                    break;
                                }
                            }
                            if (openParen != -1)
                            {
                                int depth = 1;
                                for (int i = openParen + 1; i < lineText.length(); ++i)
                                {
                                    if (lineText[i] == '(')
                                    {
                                        depth++;
                                    }
                                    else if (lineText[i] == ')')
                                    {
                                        depth--;
                                        if (depth == 0)
                                        {
                                            closeParen = i;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if (openParen != -1)
                        {
                            if (closeParen == -1)
                            {
                                skipBeautify = true;
                            }
                            else
                            {
                                if (pos <= closeParen)
                                {
                                    skipBeautify = true;
                                }
                                else
                                {
                                    skipBeautify = false;
                                }
                            }
                        }
                        else
                        {
                            skipBeautify = false;
                        }
                    }
                }

                if (skipBeautify)
                {
                    QPlainTextEdit::keyPressEvent(event);
                    return;
                }

                QPlainTextEdit::keyPressEvent(event);
                beautifyCurrentLine();
                return;
            }

            if (key == '{')
            {
                hideCompletion();

                QString prevLineText = "";
                QTextBlock prevBlock = block.previous();
                if (prevBlock.isValid())
                {
                    prevLineText = prevBlock.text().trimmed();
                }

                if (!lineText.isEmpty() && isControlStructureLine(lineText))
                {
                    beautifyCurrentLine();
                }
                else if (lineText.isEmpty() && isControlStructureLine(prevLineText))
                {
                    QTextCursor prevCursor = textCursor();
                    prevCursor.movePosition(QTextCursor::PreviousBlock);
                    prevCursor.movePosition(QTextCursor::StartOfBlock);
                    prevCursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

                    QString prevIndent;
                    QString fullPrevLine = prevBlock.text();
                    for (int i = 0; i < fullPrevLine.length(); ++i)
                    {
                        if (fullPrevLine[i].isSpace())
                        {
                            prevIndent += fullPrevLine[i];
                        }
                        else
                        {
                            break;
                        }
                    }

                    QString beautified = beautifyCode(prevLineText, true);
                    if (beautified != prevLineText)
                    {
                        prevCursor.insertText(prevIndent + beautified);
                    }
                }
            }
        }
    }

    if (codeBeautify && !competitionMode &&
            (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
    {

        hideCompletion();

        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        QString lineText = block.text().trimmed();
        int pos = cursor.positionInBlock();

        bool inCommentOrString = isPositionInCommentOrString();

        QString fullLineText = block.text();
        if (pos > 0 && pos < fullLineText.length() &&
                fullLineText[pos - 1] == '{' && fullLineText[pos] == '}')
        {

            int currentIndent = 0;
            for (int i = 0; i < fullLineText.length(); ++i)
            {
                if (fullLineText[i] == ' ')
                {
                    currentIndent++;
                }
                else if (fullLineText[i] == '	')
                {
                    currentIndent += indentSize;
                }
                else
                {
                    break;
                }
            }

            QString baseIndent = QString(currentIndent, ' ');
            QString innerIndent = QString(currentIndent + indentSize, ' ');

            cursor.beginEditBlock();
            cursor.insertText("\n" + innerIndent);
            int savedPos = cursor.position();
            cursor.insertText("\n" + baseIndent);
            cursor.setPosition(savedPos);
            cursor.endEditBlock();

            setTextCursor(cursor);
            emit cursorPositionChanged();
            event->accept();
            return;
        }

        if (!inCommentOrString && !lineText.isEmpty())
        {
            if (lineText.endsWith(')') && isControlStructureLine(lineText))
            {
                beautifyCurrentLine();
            }
        }
    }

    if (key == '{' && shouldAutoBrackets)
    {
        hideCompletion();
        QTextCursor cursor = textCursor();

        QTextBlock currentBlock = cursor.block();
        QString currentText = currentBlock.text();

        int currentIndent = 0;
        for (int i = 0; i < currentText.length(); ++i)
        {
            if (currentText[i] == ' ')
            {
                currentIndent++;
            }
            else if (currentText[i] == '	')
            {
                currentIndent += indentSize;
            }
            else
            {
                break;
            }
        }

        cursor.beginEditBlock();
        cursor.insertText("{}");
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        cursor.endEditBlock();

        setTextCursor(cursor);
        event->accept();
        return;
    }

    if (event->key() == Qt::Key_Space && (event->modifiers() & Qt::ControlModifier))
    {
        if (completionEnabled && completer && !isPositionInCommentOrString())
        {
            QString word = getWordUnderCursor();
            completionPrefix = word;
            QStringList completions = completer->getCompletions(word, textCursor().position());
            showCompletion(completions);
        }
        event->accept();
        return;
    }

    QPlainTextEdit::keyPressEvent(event);
    emit cursorPositionChanged();

    bool shouldAutoIndent = autoIndent && !(competitionMode && comp_disableAutoIndent);
    if (shouldAutoIndent && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
    {
        QTextCursor cursor = textCursor();
        QTextBlock currentBlock = cursor.block();
        QString currentLineText = currentBlock.text();

        if (currentLineText.trimmed() == "}")
        {
            int targetIndent = -1;
            int braceBalance = 0;
            int currentBlockNum = cursor.blockNumber();

            for (int i = currentBlockNum; i >= 0; --i)
            {
                QString blockText = document()->findBlockByNumber(i).text();
                for (int j = blockText.length() - 1; j >= 0; --j)
                {
                    if (i == currentBlockNum && j >= cursor.positionInBlock())
                    {
                        continue;
                    }
                    if (blockText[j] == '}')
                    {
                        braceBalance++;
                    }
                    else if (blockText[j] == '{')
                    {
                        braceBalance--;
                        if (braceBalance <= 0)
                        {
                            targetIndent = 0;
                            for (int k = 0; k < blockText.length(); ++k)
                            {
                                if (blockText[k] == ' ')
                                {
                                    targetIndent++;
                                }
                                else if (blockText[k] == '	')
                                {
                                    targetIndent += indentSize;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                if (targetIndent != -1)
                {
                    break;
                }
            }

            if (targetIndent != -1)
            {
                cursor.movePosition(QTextCursor::StartOfLine);
                int currentSpaces = 0;
                while (currentSpaces < currentLineText.length() &&
                        (currentLineText[currentSpaces] == ' ' || currentLineText[currentSpaces] == '\t'))
                {
                    currentSpaces++;
                }
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, currentSpaces);
                cursor.insertText(QString(targetIndent, ' '));
            }
        }
        else
        {
            handleAutoIndent();
        }
    }

    if (!competitionMode && completionEnabled && completer && !event->text().isEmpty())
    {
        QChar lastChar = event->text().at(0);

        if (event->key() == Qt::Key_Backspace)
        {
            hideCompletion();
            return;
        }

        if (lastChar == ';' || lastChar == '(' || lastChar == ')' ||
                lastChar == '{' || lastChar == '}' || lastChar == '[' ||
                lastChar == ']' || lastChar == ',' || lastChar == ' ' ||
                lastChar == '\'' || lastChar == '"' || lastChar == ':' ||
                lastChar == '\n' || lastChar == '\r' || lastChar == '	')
        {
            hideCompletion();
        }
        else if (lastChar.isLetterOrNumber() || lastChar == '_' ||
                 lastChar == '.' || lastChar == ':' || lastChar == '>')
        {

            if (isPositionInCommentOrString())
            {
                hideCompletion();
                return;
            }

            QString word = getWordUnderCursor();

            if (word.length() >= 1)
            {
                completionPrefix = word;
                QStringList completions = completer->getCompletions(word, textCursor().position());

                if (!completions.isEmpty())
                {
                    showCompletion(completions);
                }
                else
                {
                    hideCompletion();
                }
            }
            else
            {
                hideCompletion();
            }
        }
        else
        {
            hideCompletion();
        }
    }
}

// 终端，目前 Bug 一堆，下个版本改
class IntegratedTerminal : public QDockWidget
{
    Q_OBJECT

public:
    explicit IntegratedTerminal(QWidget *parent = nullptr);
    void executeCommand(const QString &command);
    void setWorkingDirectory(const QString &dir);
    void setTheme(bool dark);

public slots:
    void clear();

private slots:
    void readOutput();
    void readError();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onReturnPressed();

private:
    QPlainTextEdit *terminalOutput;
    QLineEdit *terminalInput;
    QProcess *terminalProcess;
    QString currentWorkingDir;

    void setupTerminal();
    void setupChineseEnvironment();
    QByteArray encodeForWindowsTerminal(const QString &text);
    QString decodeFromWindowsTerminal(const QByteArray &data);
};

IntegratedTerminal::IntegratedTerminal(QWidget *parent)
    : QDockWidget(tr("终端"), parent), terminalProcess(nullptr)
{
    setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    setFeatures(DockWidgetMovable | DockWidgetClosable);

    setupTerminal();
}

void IntegratedTerminal::setTheme(bool dark)
{
    if (dark)
    {
        terminalOutput->setStyleSheet(
            "QPlainTextEdit {"
            "    background-color: #1E1E1E;"
            "    color: #00FF00;"
            "    font-family: 'Consolas', 'Monospace';"
            "    font-size: 10pt;"
            "    selection-background-color: #006600;"
            "}"
        );

        terminalInput->setStyleSheet(
            "QLineEdit {"
            "    background-color: #1E1E1E;"
            "    color: #00FF00;"
            "    border: 1px solid #00FF00;"
            "    font-family: 'Consolas', 'Monospace';"
            "    font-size: 10pt;"
            "    padding: 2px;"
            "}"
        );
    }
    else
    {
        terminalOutput->setStyleSheet(
            "QPlainTextEdit {"
            "    background-color: #F5F5F5;"
            "    color: #000000;"
            "    font-family: 'Consolas', 'Monospace';"
            "    font-size: 10pt;"
            "    selection-background-color: #B0C4DE;"
            "    border: 1px solid #CCCCCC;"
            "}"
        );

        terminalInput->setStyleSheet(
            "QLineEdit {"
            "    background-color: #FFFFFF;"
            "    color: #000000;"
            "    border: 1px solid #CCCCCC;"
            "    font-family: 'Consolas', 'Monospace';"
            "    font-size: 10pt;"
            "    padding: 2px;"
            "}"
        );
    }
}

void IntegratedTerminal::setupTerminal()
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(2);

    terminalOutput = new QPlainTextEdit(this);
    terminalOutput->setReadOnly(true);
    terminalOutput->setWordWrapMode(QTextOption::WrapAnywhere);

    terminalInput = new QLineEdit(this);

    layout->addWidget(terminalOutput);
    layout->addWidget(terminalInput);

    setWidget(mainWidget);

    setTheme(true);

    // 不设置 UTF-8 会乱码
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    env.insert("LANG", "zh_CN.UTF-8");
    env.insert("LC_ALL", "zh_CN.UTF-8");
    env.insert("PYTHONIOENCODING", "utf-8");

#ifdef Q_OS_WIN
    env.insert("CHCP", "936");
    env.insert("LANGUAGE", "zh_CN");
#endif

    terminalProcess = new QProcess(this);
    terminalProcess->setProcessEnvironment(env);
    terminalProcess->setProcessChannelMode(QProcess::MergedChannels);

#ifdef Q_OS_WIN
    terminalProcess->start("cmd.exe");

    if (terminalProcess->waitForStarted(1000))
    {
        QByteArray chcpCommand = QByteArray("chcp 936\r\n");
        terminalProcess->write(chcpCommand);

        QTimer::singleShot(500, [this]()
        {
            terminalOutput->appendPlainText(tr("终端已启动。输入命令并按Enter执行。"));
        });
    }
#else
    // Linux/macOS（其实没用，毕竟 IDE 只能在 Windows 环境下运行）
    terminalProcess->start("/bin/bash");
    terminalOutput->appendPlainText(tr("终端已启动。输入命令并按Enter执行。"));
#endif

    connect(terminalProcess, &QProcess::readyReadStandardOutput,
            this, &IntegratedTerminal::readOutput);
    connect(terminalProcess, &QProcess::readyReadStandardError,
            this, &IntegratedTerminal::readError);
    connect(terminalProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &IntegratedTerminal::onProcessFinished);
    connect(terminalInput, &QLineEdit::returnPressed,
            this, &IntegratedTerminal::onReturnPressed);

#ifdef Q_OS_WIN
    QTimer::singleShot(1000, this, &IntegratedTerminal::setupChineseEnvironment);
#endif
}

void IntegratedTerminal::setupChineseEnvironment()
{
#ifdef Q_OS_WIN
    if (terminalProcess && terminalProcess->state() == QProcess::Running)
    {
        QByteArray chcpCommand = QByteArray("chcp 936\r\n");
        terminalProcess->write(chcpCommand);

        QByteArray fontCommand = QByteArray("powershell -Command \"Set-ItemProperty -Path 'HKCU:\\Console' -Name 'FaceName' -Value 'SimSun-ExtB'\" 2>nul\r\n");
        terminalProcess->write(fontCommand);

        terminalProcess->waitForBytesWritten(1000);
    }
#endif
}

QByteArray IntegratedTerminal::encodeForWindowsTerminal(const QString &text)
{
#ifdef Q_OS_WIN
    return text.toLocal8Bit();
#else
    return text.toUtf8();
#endif
}

QString IntegratedTerminal::decodeFromWindowsTerminal(const QByteArray &data)
{
#ifdef Q_OS_WIN
    return QString::fromLocal8Bit(data);
#else
    return QString::fromUtf8(data);
#endif
}

void IntegratedTerminal::executeCommand(const QString &command)
{
    if (terminalProcess && terminalProcess->state() == QProcess::Running)
    {
        terminalOutput->appendPlainText("$ " + command);

        QByteArray commandData = encodeForWindowsTerminal(command + "\r\n");
        terminalProcess->write(commandData);

        terminalProcess->waitForBytesWritten(1000);
    }
}

void IntegratedTerminal::setWorkingDirectory(const QString &dir)
{
    currentWorkingDir = dir;
    if (!dir.isEmpty())
    {
        QString command = "cd /d \"" + dir + "\"";
        executeCommand(command);
    }
}

void IntegratedTerminal::clear()
{
    terminalOutput->clear();
}

void IntegratedTerminal::readOutput()
{
    if (terminalProcess)
    {
        QByteArray outputData = terminalProcess->readAllStandardOutput();

        QString output = decodeFromWindowsTerminal(outputData);

        output.remove(QRegularExpression("\\x1B\\[[0-9;]*[A-Za-z]"));

        output.remove(QRegularExpression("[\\x00-\\x09\\x0B-\\x1F\\x7F]"));

        if (!output.trimmed().isEmpty())
        {
            terminalOutput->appendPlainText(output);
            terminalOutput->verticalScrollBar()->setValue(
                terminalOutput->verticalScrollBar()->maximum());
        }
    }
}

void IntegratedTerminal::readError()
{
    if (terminalProcess)
    {
        QByteArray errorData = terminalProcess->readAllStandardError();

        QString error = decodeFromWindowsTerminal(errorData);

        error.remove(QRegularExpression("\\x1B\\[[0-9;]*[A-Za-z]"));

        error.remove(QRegularExpression("[\\x00-\\x09\\x0B-\\x1F\\x7F]"));

        if (!error.trimmed().isEmpty())
        {
            terminalOutput->appendPlainText(tr("错误: ") + error);
        }
    }
}

void IntegratedTerminal::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)
    terminalOutput->appendPlainText(tr("终端进程已结束"));
}

void IntegratedTerminal::onReturnPressed()
{
    QString command = terminalInput->text();
    if (!command.isEmpty())
    {
        executeCommand(command);
        terminalInput->clear();
    }
}

// 代码片段
class SnippetManager : public QDockWidget
{
    Q_OBJECT

public:
    explicit SnippetManager(QWidget *parent = nullptr);
    void addSnippet(const QString &name, const QString &code, const QString &category = "General");
    void removeSnippet(const QString &name);
    QString getSnippet(const QString &name) const;
    QStringList getCategories() const;
    QStringList getSnippetsInCategory(const QString &category) const;

public slots:
    void insertSnippet(const QString &name);

signals:
    void snippetInserted(const QString &code);

private slots:
    void onSnippetDoubleClicked(QListWidgetItem *item);
    void onAddSnippetClicked();
    void onEditSnippetClicked();
    void onDeleteSnippetClicked();
    void onCategoryChanged(const QString &category);

private:
    QMap<QString, QMap<QString, QString>> snippets;
    QListWidget *snippetList;
    QComboBox *categoryCombo;
    QLineEdit *searchEdit;

    void setupUI();
    void loadSnippets();
    void saveSnippets();
    void refreshSnippetList();
};

SnippetManager::SnippetManager(QWidget *parent)
    : QDockWidget(tr("代码片段"), parent)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setFeatures(DockWidgetMovable | DockWidgetClosable);

    setupUI();
    loadSnippets();
}

void SnippetManager::setupUI()
{
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText(tr("搜索代码片段..."));
    layout->addWidget(searchEdit);

    QHBoxLayout *categoryLayout = new QHBoxLayout;
    categoryLayout->addWidget(new QLabel(tr("类别:")));
    categoryCombo = new QComboBox(this);
    categoryLayout->addWidget(categoryCombo);
    layout->addLayout(categoryLayout);

    snippetList = new QListWidget(this);
    layout->addWidget(snippetList);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *addButton = new QPushButton(tr("添加"), this);
    QPushButton *editButton = new QPushButton(tr("编辑"), this);
    QPushButton *deleteButton = new QPushButton(tr("删除"), this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    layout->addLayout(buttonLayout);

    setWidget(mainWidget);

    connect(snippetList, &QListWidget::itemDoubleClicked,
            this, &SnippetManager::onSnippetDoubleClicked);
    connect(addButton, &QPushButton::clicked,
            this, &SnippetManager::onAddSnippetClicked);
    connect(editButton, &QPushButton::clicked,
            this, &SnippetManager::onEditSnippetClicked);
    connect(deleteButton, &QPushButton::clicked,
            this, &SnippetManager::onDeleteSnippetClicked);
    connect(categoryCombo, &QComboBox::currentTextChanged,
            this, &SnippetManager::onCategoryChanged);
    connect(searchEdit, &QLineEdit::textChanged,
            this, [this](const QString &text)
    {
        Q_UNUSED(text) refreshSnippetList();
    });

    categoryCombo->addItems({"General", "C++", "Qt", "Algorithms", "Data Structures"});
}

void SnippetManager::addSnippet(const QString &name, const QString &code, const QString &category)
{
    QString cat = category.isEmpty() ? "General" : category;
    snippets[cat][name] = code;
    saveSnippets();
    refreshSnippetList();
}

void SnippetManager::removeSnippet(const QString &name)
{
    for (auto it = snippets.begin(); it != snippets.end(); ++it)
    {
        if (it.value().contains(name))
        {
            it.value().remove(name);
            break;
        }
    }
    saveSnippets();
    refreshSnippetList();
}

QString SnippetManager::getSnippet(const QString &name) const
{
    for (auto it = snippets.constBegin(); it != snippets.constEnd(); ++it)
    {
        if (it.value().contains(name))
        {
            return it.value().value(name);
        }
    }
    return QString();
}

QStringList SnippetManager::getCategories() const
{
    return snippets.keys();
}

QStringList SnippetManager::getSnippetsInCategory(const QString &category) const
{
    if (snippets.contains(category))
    {
        return snippets[category].keys();
    }
    return QStringList();
}

void SnippetManager::insertSnippet(const QString &name)
{
    QString code = getSnippet(name);
    if (!code.isEmpty())
    {
        emit snippetInserted(code);
    }
}

void SnippetManager::onSnippetDoubleClicked(QListWidgetItem *item)
{
    if (item)
    {
        insertSnippet(item->text());
    }
}

void SnippetManager::onAddSnippetClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("添加代码片段"),
                                         tr("片段名称:"), QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty())
    {
        QString code = QInputDialog::getMultiLineText(this, tr("添加代码片段"),
                       tr("代码内容:"), "", &ok);
        if (ok && !code.isEmpty())
        {
            QString category = categoryCombo->currentText();
            addSnippet(name, code, category);
        }
    }
}

void SnippetManager::onEditSnippetClicked()
{
    QListWidgetItem *item = snippetList->currentItem();
    if (!item)
    {
        return;
    }

    QString oldName = item->text();
    QString oldCode = getSnippet(oldName);

    bool ok;
    QString newName = QInputDialog::getText(this, tr("编辑代码片段"),
                                            tr("片段名称:"), QLineEdit::Normal, oldName, &ok);
    if (ok && !newName.isEmpty())
    {
        QString newCode = QInputDialog::getMultiLineText(this, tr("编辑代码片段"),
                          tr("代码内容:"), oldCode, &ok);
        if (ok)
        {
            removeSnippet(oldName);
            QString category = categoryCombo->currentText();
            addSnippet(newName, newCode, category);
        }
    }
}

void SnippetManager::onDeleteSnippetClicked()
{
    QListWidgetItem *item = snippetList->currentItem();
    if (!item)
    {
        return;
    }

    QString name = item->text();
    QMessageBox::StandardButton reply = QMessageBox::question(
                                            this, tr("确认删除"), tr("确定要删除代码片段\"%1\"吗?").arg(name));

    if (reply == QMessageBox::Yes)
    {
        removeSnippet(name);
    }
}

void SnippetManager::onCategoryChanged(const QString &category)
{
    Q_UNUSED(category)
    refreshSnippetList();
}

void SnippetManager::loadSnippets()
{
    QSettings settings("CompilerIDE", "Compiler IDE " + IDE_VERSION);
    settings.beginGroup("Snippets");

    QStringList categories = settings.childGroups();
    for (const QString &category : categories)
    {
        settings.beginGroup(category);
        QStringList keys = settings.childKeys();
        for (const QString &key : keys)
        {
            snippets[category][key] = settings.value(key).toString();
        }
        settings.endGroup();
    }
    settings.endGroup();

    if (snippets.isEmpty())
    {
        addSnippet("Hello World",
                   "#include <iostream>\n\n"
                   "using namespace std;\n\n"
                   "int main() {\n"
                   "    cout << \"Hello, World!\" << endl;\n"
                   "    return 0;\n"
                   "}", "C++");

        addSnippet("For Loop",
                   "for (int i = 0; i < count; i++) {\n"
                   "    // 你的代码在这里\n"
                   "}", "C++");

        addSnippet("Class Template",
                   "class MyClass {\n"
                   "public:\n"
                   "    MyClass();\n"
                   "    ~MyClass();\n"
                   "    \n"
                   "private:\n"
                   "    // 私有成员\n"
                   "};", "C++");
    }

    refreshSnippetList();
}

void SnippetManager::saveSnippets()
{
    QSettings settings("CompilerIDE", "Compiler IDE " + IDE_VERSION);
    settings.beginGroup("Snippets");
    settings.remove("");

    for (auto catIt = snippets.begin(); catIt != snippets.end(); ++catIt)
    {
        settings.beginGroup(catIt.key());
        for (auto snippetIt = catIt.value().begin(); snippetIt != catIt.value().end(); ++snippetIt)
        {
            settings.setValue(snippetIt.key(), snippetIt.value());
        }
        settings.endGroup();
    }
    settings.endGroup();
}

void SnippetManager::refreshSnippetList()
{
    snippetList->clear();

    QString category = categoryCombo->currentText();
    QString searchText = searchEdit->text().toLower();

    if (snippets.contains(category))
    {
        const QMap<QString, QString> &categorySnippets = snippets[category];
        for (auto it = categorySnippets.begin(); it != categorySnippets.end(); ++it)
        {
            if (searchText.isEmpty() || it.key().toLower().contains(searchText))
            {
                snippetList->addItem(it.key());
            }
        }
    }
}

// 项目管理器
class ProjectManager : public QDockWidget
{
    Q_OBJECT

public:
    explicit ProjectManager(QWidget *parent = nullptr);
    void setRootPath(const QString &path);
    QString getCurrentFile() const;
    void refresh();

signals:
    void fileSelected(const QString &filePath);
    void fileOpened(const QString &filePath);

private slots:
    void onFileClicked(const QModelIndex &index);
    void onFileDoubleClicked(const QModelIndex &index);
    void showContextMenu(const QPoint &point);
    void newFile();
    void newFolder();
    void rename();
    void deleteFile();
    void openInExplorer();
    void copyPath();
    void copyRelativePath();

private:
    QFileSystemModel *model;
    QTreeView *treeView;
    QString currentPath;

    void createActions();
    QAction *newFileAction;
    QAction *newFolderAction;
    QAction *renameAction;
    QAction *deleteAction;
    QAction *refreshAction;
    QAction *openInExplorerAction;
    QAction *copyPathAction;
    QAction *copyRelativePathAction;
};

ProjectManager::ProjectManager(QWidget *parent)
    : QDockWidget(tr("项目管理器"), parent)
{
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setFeatures(DockWidgetMovable | DockWidgetClosable);

    QToolBar *toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(16, 16));
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    createActions();
    toolbar->addAction(newFolderAction);
    toolbar->addAction(deleteAction);
    toolbar->addAction(refreshAction);

    model = new QFileSystemModel(this);
    model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs);

    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIsDecorated(true);
    treeView->setAlternatingRowColors(true);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    treeView->setDragDropMode(QAbstractItemView::InternalMove);
    treeView->setDefaultDropAction(Qt::MoveAction);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);

    treeView->header()->setStretchLastSection(false);
    treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    QWidget *titleWidget = new QWidget(this);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
    titleLayout->setContentsMargins(5, 2, 5, 2);
    titleLayout->addStretch();
    titleLayout->addWidget(toolbar);
    setTitleBarWidget(titleWidget);

    setWidget(treeView);

    connect(treeView, &QTreeView::clicked, this, &ProjectManager::onFileClicked);
    connect(treeView, &QTreeView::doubleClicked, this, &ProjectManager::onFileDoubleClicked);
    connect(treeView, &QTreeView::customContextMenuRequested, this, &ProjectManager::showContextMenu);

    QShortcut *deleteShortcut = new QShortcut(QKeySequence::Delete, treeView);
    connect(deleteShortcut, &QShortcut::activated, this, &ProjectManager::deleteFile);

    QShortcut *renameShortcut = new QShortcut(QKeySequence(Qt::Key_F2), treeView);
    connect(renameShortcut, &QShortcut::activated, this, &ProjectManager::rename);

    QShortcut *refreshShortcut = new QShortcut(QKeySequence::Refresh, treeView);
    connect(refreshShortcut, &QShortcut::activated, this, &ProjectManager::refresh);

    setRootPath("");
}

void ProjectManager::createActions()
{
    // 图标目前其实没用，但后期会加入
    newFolderAction = new QAction(QIcon(":/icons/newfolder.png"), tr("新建文件夹"), this);
    newFolderAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_N));
    newFolderAction->setStatusTip(tr("创建新文件夹"));
    connect(newFolderAction, &QAction::triggered, this, &ProjectManager::newFolder);

    renameAction = new QAction(QIcon(":/icons/rename.png"), tr("重命名"), this);
    renameAction->setShortcut(QKeySequence(Qt::Key_F2));
    renameAction->setStatusTip(tr("重命名选中的文件或文件夹"));
    connect(renameAction, &QAction::triggered, this, &ProjectManager::rename);

    deleteAction = new QAction(QIcon(":/icons/delete.png"), tr("删除"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("删除选中的文件或文件夹"));
    connect(deleteAction, &QAction::triggered, this, &ProjectManager::deleteFile);

    refreshAction = new QAction(QIcon(":/icons/refresh.png"), tr("刷新"), this);
    refreshAction->setShortcut(QKeySequence::Refresh);
    refreshAction->setStatusTip(tr("刷新项目视图"));
    connect(refreshAction, &QAction::triggered, this, &ProjectManager::refresh);

    openInExplorerAction = new QAction(QIcon(":/icons/open.png"), tr("在资源管理器中打开"), this);
    openInExplorerAction->setStatusTip(tr("在系统文件管理器中打开"));
    connect(openInExplorerAction, &QAction::triggered, this, &ProjectManager::openInExplorer);

    copyPathAction = new QAction(tr("复制完整路径"), this);
    copyPathAction->setStatusTip(tr("复制文件或文件夹的完整路径"));
    connect(copyPathAction, &QAction::triggered, this, &ProjectManager::copyPath);

    copyRelativePathAction = new QAction(tr("复制相对路径"), this);
    copyRelativePathAction->setStatusTip(tr("复制文件或文件夹相对于项目根目录的路径"));
    connect(copyRelativePathAction, &QAction::triggered, this, &ProjectManager::copyRelativePath);
}

void ProjectManager::setRootPath(const QString &path)
{
    if (path.isEmpty())
    {
        model->setRootPath("");
        treeView->setRootIndex(QModelIndex());
        setWindowTitle(tr("项目管理器"));
        currentPath = "";
        return;
    }

    currentPath = path;
    QModelIndex index = model->setRootPath(path);
    treeView->setRootIndex(index);

    setWindowTitle(tr("项目管理器 - %1").arg(QDir(path).dirName()));
}

void ProjectManager::newFile()
{
    // 这个函数删了会莫名其妙报错，就没有删去，实则没有任何用处
}

void ProjectManager::refresh()
{
    treeView->setRootIndex(model->setRootPath(currentPath));
}

QString ProjectManager::getCurrentFile() const
{
    QModelIndex index = treeView->currentIndex();
    if (index.isValid())
    {
        return model->filePath(index);
    }
    return QString();
}

void ProjectManager::onFileDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    QString filePath = model->filePath(index);

    if (model->isDir(index))
    {
        if (treeView->isExpanded(index))
        {
            treeView->collapse(index);
        }
        else
        {
            treeView->expand(index);
        }
        return;
    }

    QFileInfo fileInfo(filePath);
    QString ext = fileInfo.suffix().toLower();

    QStringList blockedExts =
    {
        "exe", "dll", "mp3", "mp4", "avi", "mkv", "wav", "flac", "iso",
        "zip", "rar", "7z", "tar", "gz", "bin", "dat", "obj",
        "doc", "docx", "xls", "xlsx", "ppt", "pptx",
        "jpg", "jpeg", "png", "gif", "bmp", "ico", "svg", "wmv", "dat"
    };

    if (blockedExts.contains(ext))
    {
        QMessageBox::warning(this, tr("无法打开文件"),
                             tr("无法打开此类型的文件 (*.%1)\n\nCompiler IDE 仅支持编辑文本文件").arg(ext));
        return;
    }

    emit fileOpened(filePath);
}

void ProjectManager::onFileClicked(const QModelIndex &index)
{
    if (!index.isValid() || model->isDir(index))
    {
        return;
    }

    QString filePath = model->filePath(index);
    QFileInfo fileInfo(filePath);
    QString ext = fileInfo.suffix().toLower();

    QStringList blockedExts =
    {
        "exe", "dll", "mp3", "mp4", "avi", "mkv", "wav", "iso", "bin",
        "doc", "docx", "xls", "xlsx", "ppt", "pptx",
        "jpg", "jpeg", "png", "gif", "bmp", "zip", "rar", "wmv", "dat"
    };

    if (blockedExts.contains(ext))
    {
        return;
    }

    emit fileSelected(filePath);
}

void ProjectManager::showContextMenu(const QPoint &point)
{
    QModelIndex index = treeView->indexAt(point);
    QMenu menu;

    if (index.isValid())
    {
        if (model->isDir(index))
        {
            menu.addAction(openInExplorerAction);
            menu.addSeparator();
            menu.addAction(renameAction);
            menu.addAction(deleteAction);
            menu.addSeparator();
            menu.addAction(copyPathAction);
        }
        else
        {
            menu.addAction(openInExplorerAction);
            menu.addSeparator();
            menu.addAction(renameAction);
            menu.addAction(deleteAction);
            menu.addSeparator();
            menu.addAction(copyPathAction);
        }
    }
    else
    {
        menu.addAction(newFolderAction);
        menu.addSeparator();
        menu.addAction(refreshAction);
    }

    menu.exec(treeView->mapToGlobal(point));
}

void ProjectManager::newFolder()
{
    QModelIndex index = treeView->currentIndex();
    QString path;

    if (index.isValid() && model->isDir(index))
    {
        path = model->filePath(index);
    }
    else if (index.isValid())
    {
        path = model->filePath(index.parent());
    }
    else
    {
        path = currentPath;
    }

    bool ok;
    QString folderName = QInputDialog::getText(this, tr("新建文件夹"), tr("文件夹名:"),
                         QLineEdit::Normal, "NewFolder", &ok);
    if (ok && !folderName.isEmpty())
    {
        QDir dir(path);
        if (dir.mkdir(folderName))
        {
            refresh();

            QModelIndex newIndex = model->index(dir.filePath(folderName));
            if (newIndex.isValid())
            {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
                treeView->expand(newIndex);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("无法创建文件夹"));
        }
    }
}

void ProjectManager::rename()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    QString oldPath = model->filePath(index);
    QString name = model->fileName(index);

    bool ok;
    QString newName = QInputDialog::getText(this, tr("重命名"), tr("新名称:"),
                                            QLineEdit::Normal, name, &ok);
    if (ok && !newName.isEmpty())
    {
        QDir dir = QFileInfo(oldPath).absoluteDir();
        QString newPath = dir.absoluteFilePath(newName);

        if (QFile::exists(newPath))
        {
            QMessageBox::warning(this, tr("错误"), tr("名称已存在"));
            return;
        }

        if (QFile::rename(oldPath, newPath))
        {
            refresh();

            QModelIndex newIndex = model->index(newPath);
            if (newIndex.isValid())
            {
                treeView->setCurrentIndex(newIndex);
                treeView->scrollTo(newIndex);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("无法重命名"));
        }
    }
}

void ProjectManager::deleteFile()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    QString path = model->filePath(index);
    QString name = model->fileName(index);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("确认删除"), tr("确定要删除\"%1\"吗?").arg(name),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        bool success = false;
        if (model->isDir(index))
        {
            QDir dir(path);
            success = dir.removeRecursively();
        }
        else
        {
            success = QFile::remove(path);
        }

        if (success)
        {
            refresh();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("无法删除"));
        }
    }
}

void ProjectManager::openInExplorer()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    QString path = model->filePath(index);

#ifdef Q_OS_WIN
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(path);
    QProcess::startDetached("explorer", args);
#elif defined(Q_OS_MAC)
    // 没啥用
    QProcess::execute("open", QStringList() << "-R" << path);
#else
    QFileInfo fileInfo(path);
    QProcess::execute("xdg-open", QStringList() << fileInfo.absolutePath());
#endif
}

void ProjectManager::copyPath()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    QString path = model->filePath(index);
    QApplication::clipboard()->setText(path);
}

void ProjectManager::copyRelativePath()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    QString path = model->filePath(index);
    QDir projectDir(currentPath);
    QString relativePath = projectDir.relativeFilePath(path);
    QApplication::clipboard()->setText(relativePath);
}

class CompetitionModeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompetitionModeDialog(QWidget *parent = nullptr);
    ~CompetitionModeDialog();

    bool isCountdown() const
    {
        return countdownRadio->isChecked();
    }
    int getMinutes() const
    {
        return minutesSpin->value();
    }
    bool disableCodeCompletion() const
    {
        return codeCompletionCheck->isChecked();
    }
    bool disableAISuggestions() const
    {
        return aiSuggestionsCheck->isChecked();
    }
    bool disableAutoBrackets() const
    {
        return autoBracketsCheck->isChecked();
    }
    bool disableAutoQuotes() const
    {
        return autoQuotesCheck->isChecked();
    }
    bool disableAutoIndent() const
    {
        return autoIndentCheck->isChecked();
    }
    bool disableTerminal() const
    {
        return terminalCheck->isChecked();
    }
    bool disableSnippets() const
    {
        return snippetsCheck->isChecked();
    }
    bool disableDiagnostics() const
    {
        return diagnosticsCheck->isChecked();
    }
    bool disableProjectManager() const
    {
        return projectManagerCheck->isChecked();
    }

private slots:
    void updateTimerPreview();

private:
    void setupUI();

    QRadioButton *countdownRadio;
    QRadioButton *stopwatchRadio;
    QSpinBox *minutesSpin;
    QLabel *timerPreview;

    QCheckBox *codeCompletionCheck;
    QCheckBox *aiSuggestionsCheck;
    QCheckBox *autoBracketsCheck;
    QCheckBox *autoQuotesCheck;
    QCheckBox *autoIndentCheck;
    QCheckBox *terminalCheck;
    QCheckBox *snippetsCheck;
    QCheckBox *diagnosticsCheck;
    QCheckBox *projectManagerCheck;
};

CompetitionModeDialog::CompetitionModeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("竞赛模式设置"));
    setModal(true);
    setMinimumSize(550, 500);

    setupUI();
}

CompetitionModeDialog::~CompetitionModeDialog()
{
    // 析构函数，暂时留空
}

void CompetitionModeDialog::setupUI()
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *contentWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(contentWidget);

    QLabel *titleLabel = new QLabel(tr("竞赛模式设置"));
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(20);

    QGroupBox *timerGroup = new QGroupBox(tr("计时器设置"));
    QVBoxLayout *timerLayout = new QVBoxLayout(timerGroup);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    countdownRadio = new QRadioButton(tr("倒计时器"));
    stopwatchRadio = new QRadioButton(tr("正计时器"));
    countdownRadio->setChecked(true);

    typeLayout->addWidget(countdownRadio);
    typeLayout->addWidget(stopwatchRadio);
    typeLayout->addStretch();

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(new QLabel(tr("倒计时时间:")));
    minutesSpin = new QSpinBox;
    minutesSpin->setRange(1, 300);
    minutesSpin->setValue(60);
    minutesSpin->setSuffix(tr(" 分钟"));
    timeLayout->addWidget(minutesSpin);
    timeLayout->addStretch();

    timerPreview = new QLabel;
    timerPreview->setAlignment(Qt::AlignCenter);
    QFont previewFont = timerPreview->font();
    previewFont.setPointSize(14);
    previewFont.setBold(true);
    timerPreview->setFont(previewFont);
    timerPreview->setStyleSheet("QLabel { color: #2E86AB; padding: 10px; border: 1px solid #2E86AB; border-radius: 5px; }");

    timerLayout->addLayout(typeLayout);
    timerLayout->addLayout(timeLayout);
    timerLayout->addWidget(timerPreview);

    QGroupBox *editorFeaturesGroup = new QGroupBox(tr("代码编辑功能设置"));
    QVBoxLayout *editorFeaturesLayout = new QVBoxLayout(editorFeaturesGroup);

    codeCompletionCheck = new QCheckBox(tr("禁用代码补全（必须禁用）"));
    aiSuggestionsCheck = new QCheckBox(tr("禁用 DeepSeek AI 助手（必须禁用）"));
    autoBracketsCheck = new QCheckBox(tr("禁用自动括号补全"));
    autoQuotesCheck = new QCheckBox(tr("禁用自动引号补全"));
    autoIndentCheck = new QCheckBox(tr("禁用自动缩进"));

    codeCompletionCheck->setChecked(true);
    aiSuggestionsCheck->setChecked(true);
    codeCompletionCheck->setEnabled(false);
    aiSuggestionsCheck->setEnabled(false);

    editorFeaturesLayout->addWidget(codeCompletionCheck);
    editorFeaturesLayout->addWidget(aiSuggestionsCheck);
    editorFeaturesLayout->addWidget(autoBracketsCheck);
    editorFeaturesLayout->addWidget(autoQuotesCheck);
    editorFeaturesLayout->addWidget(autoIndentCheck);

    QGroupBox *ideFeaturesGroup = new QGroupBox(tr("IDE功能设置"));
    QVBoxLayout *ideFeaturesLayout = new QVBoxLayout(ideFeaturesGroup);

    terminalCheck = new QCheckBox(tr("禁用终端"));
    snippetsCheck = new QCheckBox(tr("禁用代码片段"));
    diagnosticsCheck = new QCheckBox(tr("禁用诊断工具"));
    projectManagerCheck = new QCheckBox(tr("禁用项目管理器"));

    terminalCheck->setChecked(false);
    snippetsCheck->setChecked(false);
    diagnosticsCheck->setChecked(false);
    projectManagerCheck->setChecked(false);

    ideFeaturesLayout->addWidget(terminalCheck);
    ideFeaturesLayout->addWidget(snippetsCheck);
    ideFeaturesLayout->addWidget(diagnosticsCheck);
    ideFeaturesLayout->addWidget(projectManagerCheck);

    QLabel *infoLabel = new QLabel(tr("注意：代码补全和AI建议在竞赛模式下必须禁用，其他功能可根据需要选择。"));
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { color: #F18F01; background: #FFF9E6; padding: 8px; border-radius: 4px; }");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    mainLayout->addWidget(timerGroup);
    mainLayout->addWidget(editorFeaturesGroup);
    mainLayout->addWidget(ideFeaturesGroup);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(buttonBox);

    scrollArea->setWidget(contentWidget);

    QVBoxLayout *dialogLayout = new QVBoxLayout(this);
    dialogLayout->addWidget(scrollArea);

    connect(countdownRadio, &QRadioButton::toggled, this, &CompetitionModeDialog::updateTimerPreview);
    connect(stopwatchRadio, &QRadioButton::toggled, this, &CompetitionModeDialog::updateTimerPreview);
    connect(minutesSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &CompetitionModeDialog::updateTimerPreview);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updateTimerPreview();
}

void CompetitionModeDialog::updateTimerPreview()
{
    if (countdownRadio->isChecked())
    {
        int totalSeconds = minutesSpin->value() * 60;
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;
        timerPreview->setText(tr("倒计时: %1:%2:%3")
                              .arg(hours, 2, 10, QLatin1Char('0'))
                              .arg(minutes, 2, 10, QLatin1Char('0'))
                              .arg(seconds, 2, 10, QLatin1Char('0')));
    }
    else
    {
        timerPreview->setText(tr("正计时: 00:00:00"));
    }
}

class UpdateManager : public QObject
{
    Q_OBJECT

public:
    explicit UpdateManager(QObject *parent = nullptr);
    ~UpdateManager();

    void checkForUpdates(bool silent = false);
    bool isUpdateAvailable() const
    {
        return updateAvailable;
    }
    QString getDownloadUrl() const
    {
        return downloadUrl;
    }
    QString getNewVersion() const
    {
        return newVersion;
    }

    void downloadAndUpdate();

signals:
    void updateCheckFinished(bool updateAvailable, const QString &newVersion);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(bool success);
    void updateReady();
    void noUpdateAvailable();

private slots:
    void onVersionCheckFinished();
    void onUrlCheckFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();

private:
    bool isAutoCheck;
    bool shouldShowDialog;
    bool isChecking;
    void showUpdateDialog();
    void startDownload();
    void setupAutoUpdate();
    void handleRedirects();
    void fetchUpdateInfo();
    void showUpdateDialogWithInfo();
    QString updateInfo;

    QNetworkAccessManager *networkManager;
    QNetworkReply *versionReply;
    QNetworkReply *urlReply;
    QNetworkReply *downloadReply;

    bool updateAvailable;
    bool silentCheck;
    QString newVersion;
    QString downloadUrl;
    QString downloadedFilePath;

    QProgressDialog *progressDialog;
};

UpdateManager::UpdateManager(QObject *parent)
    : QObject(parent), networkManager(nullptr), versionReply(nullptr),
      urlReply(nullptr), downloadReply(nullptr), updateAvailable(false),
      silentCheck(false), progressDialog(nullptr), isChecking(false)
{
    networkManager = new QNetworkAccessManager(this);
}

UpdateManager::~UpdateManager()
{
    if (versionReply)
    {
        versionReply->abort();
        versionReply->deleteLater();
    }
    if (urlReply)
    {
        urlReply->abort();
        urlReply->deleteLater();
    }
    if (downloadReply)
    {
        downloadReply->abort();
        downloadReply->deleteLater();
    }
    if (progressDialog)
    {
        progressDialog->deleteLater();
    }
}

void UpdateManager::checkForUpdates(bool silent)
{
    if (isChecking)
    {
        qDebug() << "更新检查正在进行中，跳过重复请求";
        return;
    }

    isChecking = true;
    silentCheck = silent;
    isAutoCheck = silent;

    updateAvailable = false;
    newVersion.clear();
    downloadUrl.clear();
    updateInfo.clear();

    if (versionReply)
    {
        versionReply->abort();
        versionReply->deleteLater();
        versionReply = nullptr;
    }

    QUrl versionUrl("https://compileride.github.io/version.txt");
    QNetworkRequest versionRequest(versionUrl);
    versionRequest.setRawHeader("User-Agent", QString("CompilerIDE/%1").arg(IDE_VERSION).toUtf8());
    versionRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                                QNetworkRequest::NoLessSafeRedirectPolicy);

    versionRequest.setTransferTimeout(10000);

    versionReply = networkManager->get(versionRequest);
    connect(versionReply, &QNetworkReply::finished, this, &UpdateManager::onVersionCheckFinished);
    connect(versionReply, &QNetworkReply::errorOccurred, this, [this, silent](QNetworkReply::NetworkError error)
    {
        Q_UNUSED(error)
        isChecking = false;

        if (!silent)
        {
            qDebug() << "版本检查失败:" << versionReply->errorString();
            QMessageBox::warning(nullptr, tr("检查更新失败"),
                                 tr("无法检查更新: %1").arg(versionReply->errorString()));
        }
        else
        {
            qDebug() << "自动检查更新失败:" << versionReply->errorString();
        }
        emit updateCheckFinished(false, "");
    });

    if (!silent)
    {
        qDebug() << "开始手动检查更新...";
    }
    else
    {
        qDebug() << "开始自动检查更新...";
    }
}

void UpdateManager::onVersionCheckFinished()
{
    QTimer::singleShot(0, this, [this]()
    {
        isChecking = false;
    });

    if (versionReply->error() != QNetworkReply::NoError)
    {
        if (!silentCheck)
        {
            qDebug() << "版本检查错误:" << versionReply->errorString();
            QMessageBox::warning(nullptr, tr("检查更新失败"),
                                 tr("无法检查更新: %1").arg(versionReply->errorString()));
        }
        else
        {
            qDebug() << "自动版本检查错误:" << versionReply->errorString();
        }
        emit updateCheckFinished(false, "");
        return;
    }

    QByteArray versionData = versionReply->readAll();
    QString remoteVersion = QString::fromUtf8(versionData).trimmed();

    qDebug() << "本地版本:" << IDE_VERSION << "远程版本:" << remoteVersion;

    bool hasUpdate = false;
    QStringList localParts = IDE_VERSION.split('.');
    QStringList remoteParts = remoteVersion.split('.');

    for (int i = 0; i < qMin(localParts.size(), remoteParts.size()); ++i)
    {
        int localNum = localParts[i].toInt();
        int remoteNum = remoteParts[i].toInt();

        if (remoteNum > localNum)
        {
            hasUpdate = true;
            break;
        }
        else if (remoteNum < localNum)
        {
            break;
        }
    }

    if (hasUpdate)
    {
        updateAvailable = true;
        newVersion = remoteVersion;

        qDebug() << "发现新版本:" << newVersion;

        QUrl urlUrl("https://compileride.github.io/download_URL.txt");
        QNetworkRequest urlRequest(urlUrl);
        urlRequest.setRawHeader("User-Agent", QString("CompilerIDE/%1").arg(IDE_VERSION).toUtf8());
        urlRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                                QNetworkRequest::NoLessSafeRedirectPolicy);
        urlRequest.setTransferTimeout(10000);

        urlReply = networkManager->get(urlRequest);
        connect(urlReply, &QNetworkReply::finished, this, &UpdateManager::onUrlCheckFinished);
        connect(urlReply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError error)
        {
            Q_UNUSED(error)
            qDebug() << "下载链接获取失败:" << urlReply->errorString();

            if (updateAvailable)
            {
                downloadUrl = "https://github.com/Eric-T800/Eric-T800.github.io/releases";
                showUpdateDialogWithInfo();
            }

            emit updateCheckFinished(false, "");
        });
    }
    else
    {
        if (!silentCheck)
        {
            qDebug() << "当前已是最新版本";
            emit noUpdateAvailable();
        }
        else
        {
            qDebug() << "自动检查: 当前已是最新版本";
        }
        emit updateCheckFinished(false, "");
    }
}

void UpdateManager::onUrlCheckFinished()
{
    QTimer::singleShot(0, this, [this]()
    {
        isChecking = false;
    });

    if (urlReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "下载链接获取错误:" << urlReply->errorString();

        if (updateAvailable)
        {
            downloadUrl = "https://github.com/CompilerIDE/CompilerIDE.github.io/releases";
            fetchUpdateInfo();
        }

        emit updateCheckFinished(false, "");
        return;
    }

    QByteArray urlData = urlReply->readAll();
    downloadUrl = QString::fromUtf8(urlData).trimmed();

    qDebug() << "发现新版本:" << newVersion << "下载链接:" << downloadUrl;

    emit updateCheckFinished(true, newVersion);

    if (updateAvailable)
    {
        fetchUpdateInfo();
    }
}

void UpdateManager::showUpdateDialog()
{
    if (silentCheck)
    {
        qDebug() << "静默检查发现新版本:" << newVersion << "但不显示对话框";
        return;
    }

    fetchUpdateInfo();
}

void UpdateManager::fetchUpdateInfo()
{
    QUrl infoUrl("https://compileride.github.io/Update_info.txt");
    QNetworkRequest infoRequest(infoUrl);
    infoRequest.setRawHeader("User-Agent", QString("CompilerIDE/%1").arg(IDE_VERSION).toUtf8());
    infoRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                             QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *infoReply = networkManager->get(infoRequest);

    connect(infoReply, &QNetworkReply::finished, this, [this, infoReply]()
    {
        if (infoReply->error() == QNetworkReply::NoError)
        {
            QByteArray infoData = infoReply->readAll();
            updateInfo = QString::fromUtf8(infoData).trimmed();
            qDebug() << "成功获取更新信息:" << updateInfo;
        }
        else
        {
            updateInfo = tr("无法获取更新详情信息");
            qDebug() << "获取更新信息失败:" << infoReply->errorString();
        }

        infoReply->deleteLater();
        showUpdateDialogWithInfo();
    });

    connect(infoReply, &QNetworkReply::errorOccurred, this, [this, infoReply](QNetworkReply::NetworkError error)
    {
        Q_UNUSED(error)
        updateInfo = tr("无法获取更新详情信息");
        infoReply->deleteLater();
        showUpdateDialogWithInfo();
    });
}

void UpdateManager::showUpdateDialogWithInfo()
{
    QTimer::singleShot(0, this, [this]()
    {
        QString message = tr("发现新版本 %1\n当前版本: %2\n\n").arg(newVersion).arg(IDE_VERSION);

        if (!updateInfo.isEmpty() && updateInfo != tr("无法获取更新详情信息"))
        {
            message += tr("更新详情：\n");
            message += updateInfo;
        }
        else
        {
            message += tr("更新详情：\n暂无详细更新信息");
        }
        message += tr("\n\nIDE将在更新下载完成后立即更新。");

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("发现新版本 - Compiler IDE"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(message);
        msgBox.setInformativeText(tr("是否立即下载更新？"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);

        if (updateInfo.length() > 100)
        {
            msgBox.setMinimumWidth(600);
            msgBox.setMinimumHeight(400);
        }

        if (msgBox.exec() == QMessageBox::Yes)
        {
            startDownload();
        }
        else
        {
            qDebug() << "用户选择了不下载更新";
        }
    });
}

void UpdateManager::startDownload()
{
    if (downloadUrl.isEmpty())
    {
        QMessageBox::warning(nullptr, tr("错误"), tr("下载链接无效"));
        return;
    }

    progressDialog = new QProgressDialog(tr("正在下载更新..."), tr("取消"), 0, 100, nullptr);
    progressDialog->setWindowTitle(tr("下载更新"));
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(0);
    progressDialog->setValue(0);
    progressDialog->setAutoClose(false);

    connect(progressDialog, &QProgressDialog::canceled, this, [this]()
    {
        if (downloadReply)
        {
            downloadReply->abort();
        }
        progressDialog->close();
    });

    QUrl downloadUrlObj(downloadUrl);
    QNetworkRequest downloadRequest(downloadUrlObj);
    downloadRequest.setRawHeader("User-Agent", QString("CompilerIDE/%1").arg(IDE_VERSION).toUtf8());
    downloadRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                                 QNetworkRequest::NoLessSafeRedirectPolicy);

    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString fileName = "CompilerIDE_Update_Setup.exe";
    downloadedFilePath = tempDir + "/" + fileName;

    downloadReply = networkManager->get(downloadRequest);
    connect(downloadReply, &QNetworkReply::downloadProgress, this, &UpdateManager::onDownloadProgress);
    connect(downloadReply, &QNetworkReply::finished, this, &UpdateManager::onDownloadFinished);

    qDebug() << "开始下载更新文件:" << downloadUrl << "到:" << downloadedFilePath;
}

void UpdateManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0)
    {
        int progress = static_cast<int>((bytesReceived * 100) / bytesTotal);
        if (progressDialog)
        {
            progressDialog->setValue(progress);

            QString sizeReceived = QString::number(bytesReceived / (1024.0 * 1024.0), 'f', 2);
            QString sizeTotal = QString::number(bytesTotal / (1024.0 * 1024.0), 'f', 2);
            progressDialog->setLabelText(tr("正在下载更新...\n%1 MB / %2 MB").arg(sizeReceived).arg(sizeTotal));
        }
        emit downloadProgress(bytesReceived, bytesTotal);
    }
}

void UpdateManager::onDownloadFinished()
{
    if (downloadReply->error() != QNetworkReply::NoError)
    {
        if (downloadReply->error() != QNetworkReply::OperationCanceledError)
        {
            QString errorMsg = tr("下载更新失败: %1").arg(downloadReply->errorString());
            QMessageBox::critical(nullptr, tr("下载失败"), errorMsg);
            qDebug() << "下载失败:" << downloadReply->errorString();
        }
        if (progressDialog)
        {
            progressDialog->close();
        }
        emit downloadFinished(false);
        return;
    }

    QFile file(downloadedFilePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QByteArray data = downloadReply->readAll();
        file.write(data);
        file.close();

        if (progressDialog)
        {
            progressDialog->setLabelText(tr("下载完成，准备更新..."));
            progressDialog->setValue(100);
            progressDialog->close();
        }

        qDebug() << "更新文件下载完成:" << downloadedFilePath << "文件大小:" << QFileInfo(downloadedFilePath).size() << "字节";

        QDialog *countdownDialog = new QDialog();
        countdownDialog->setWindowTitle(tr("安装更新"));
        countdownDialog->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        countdownDialog->setAttribute(Qt::WA_DeleteOnClose);

        QVBoxLayout *layout = new QVBoxLayout(countdownDialog);
        QLabel *messageLabel = new QLabel(tr("更新已下载完成，将在3秒后关闭IDE并安装更新..."));
        QLabel *countdownLabel = new QLabel("3");
        countdownLabel->setAlignment(Qt::AlignCenter);
        QFont font = countdownLabel->font();
        font.setPointSize(48);
        font.setBold(true);
        countdownLabel->setFont(font);

        layout->addWidget(messageLabel);
        layout->addWidget(countdownLabel);

        countdownDialog->resize(400, 200);
        countdownDialog->show();

        QTimer *countdownTimer = new QTimer(countdownDialog);

        connect(countdownTimer, &QTimer::timeout, [countdownLabel]()
        {
            int currentValue = countdownLabel->text().toInt();
            currentValue--;
            if (currentValue > 0)
            {
                countdownLabel->setText(QString::number(currentValue));
            }
        });
        countdownTimer->start(1000);

        QTimer::singleShot(3000, [this, countdownDialog]()
        {
            countdownDialog->close();

            setupAutoUpdate();
        });

        emit downloadFinished(true);
        emit updateReady();
    }
    else
    {
        QMessageBox::critical(nullptr, tr("保存失败"),
                              tr("无法保存下载的文件: %1").arg(file.errorString()));
        emit downloadFinished(false);
    }

    downloadReply->deleteLater();
    downloadReply = nullptr;
}

void UpdateManager::setupAutoUpdate()
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString scriptPath;

#ifdef Q_OS_WIN
    QString installerPath = downloadedFilePath;

    if (QFile::exists(installerPath))
    {
        qDebug() << "直接启动安装程序:" << installerPath;

        bool started = QProcess::startDetached(installerPath, QStringList());

        if (started)
        {
            qDebug() << "安装程序已启动，准备退出当前应用";

            QTimer::singleShot(1000, []()
            {
                QApplication::quit();
            });
        }
        else
        {
            qDebug() << "无法启动安装程序";
            QMessageBox::critical(nullptr, tr("更新错误"),
                                  tr("无法启动安装程序。请手动运行: %1").arg(installerPath));
        }
    }
    else
    {
        qDebug() << "安装程序文件不存在:" << installerPath;
        QMessageBox::critical(nullptr, tr("更新错误"),
                              tr("安装程序文件不存在: %1").arg(installerPath));
    }
#else
    // Linux/macOS（实则没啥用）
    scriptPath = tempDir + "/CompilerIDE_Update.sh";

    QFile scriptFile(scriptPath);
    if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&scriptFile);
        out << "#!/bin/bash\n";
        out << "echo \"Updating Compiler IDE...\"\n";
        out << "sleep 3\n";

        out << "echo \"Installing new version...\"\n";
        out << "chmod +x \"" << downloadedFilePath << "\"\n";
        out << "\"" << downloadedFilePath << "\"\n";

        out << "echo \"Update completed!\"\n";
        out << "rm -f \"" << scriptPath << "\"\n";
        out << "rm -f \"" << downloadedFilePath << "\"\n";

        scriptFile.close();

        QFile::setPermissions(scriptPath, QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
        QProcess::startDetached("/bin/bash", QStringList() << scriptPath);
    }
#endif
}

void UpdateManager::downloadAndUpdate()
{
    if (updateAvailable && !downloadUrl.isEmpty())
    {
        startDownload();
    }
}

class CompilerIDE;

class SideBarContainer : public QWidget
{
    Q_OBJECT
public:
    SideBarContainer(QWidget *parent = nullptr);
    void addPage(const QString &name, QWidget *widget);
    QString getCurrentFeatureName() const;
    void switchToFeature(const QString &name);
    void setDarkTheme(bool dark);
    void setFeatureEnabled(const QString &name, bool enabled);
    bool isFeatureEnabled(const QString &name) const;

signals:
    void featureDisabledClicked(const QString &name);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void setupUI();
    void updateButtonIcon(QPushButton *btn, const QString &pageName, bool dark);

    QMap<QString, bool> featureEnabledMap;
    int m_currentIndex;
    QStackedWidget *contentStack;
    QVBoxLayout *navLayout;
    QLabel *titleLabel;
    QList<QPushButton*> navButtons;
    QStringList pageNames;
    bool currentThemeIsDark;
};

SideBarContainer::SideBarContainer(QWidget *parent) : QWidget(parent), currentThemeIsDark(true), m_currentIndex(-1)
{
    setupUI();
    this->hide();
}

void SideBarContainer::addPage(const QString &name, QWidget *widget)
{
    contentStack->addWidget(widget);

    QPushButton *btn = new QPushButton();
    btn->setCheckable(true);
    btn->setAutoExclusive(true);
    btn->setObjectName("SideBarNavBtn");
    btn->setFixedWidth(45);
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    btn->setToolTip(name);
    btn->setProperty("pageName", name);
    btn->installEventFilter(this);

    featureEnabledMap[name] = true;

    int index = contentStack->count() - 1;

    int stretchIdx = -1;
    for (int i = 0; i < navLayout->count(); ++i)
    {
        if (navLayout->itemAt(i)->spacerItem())
        {
            stretchIdx = i;
            break;
        }
    }
    if (stretchIdx >= 0)
    {
        navLayout->insertWidget(stretchIdx, btn);
    }
    else
    {
        navLayout->addWidget(btn);
        navLayout->addStretch();
    }

    connect(btn, &QPushButton::clicked, [this, index, name]()
            {
                m_currentIndex = index;
                titleLabel->setText(name);
                contentStack->setCurrentIndex(index);
                if (this->isHidden())
                {
                    this->show();
                }
            });

    navButtons.append(btn);
    pageNames.append(name);
    updateButtonIcon(btn, name, currentThemeIsDark);
}

QString SideBarContainer::getCurrentFeatureName() const
{
    int index = contentStack->currentIndex();
    if (index >= 0 && index < pageNames.size())
    {
        return pageNames[index];
    }
    return QString();
}

void SideBarContainer::switchToFeature(const QString &name)
{
    int index = pageNames.indexOf(name);
    if (index >= 0 && index < navButtons.size())
    {
        for (int i = 0; i < navButtons.size(); ++i)
        {
            navButtons[i]->setChecked(i == index);
        }
        titleLabel->setText(name);
        contentStack->setCurrentIndex(index);
        m_currentIndex = index;
        if (this->isHidden())
        {
            this->show();
        }
    }
}

void SideBarContainer::setDarkTheme(bool dark)
{
    currentThemeIsDark = dark;

    QString bgColor = dark ? "#252526" : "#F3F3F3";
    QString borderColor = dark ? "#3C3C3C" : "#DDDDDD";
    QString textColor = dark ? "#CCCCCC" : "#333333";
    QString activeColor = dark ? "#37373D" : "#FFFFFF";

    this->setStyleSheet(QString(
                            "QWidget#SideBarRoot { background-color: %1; border-right: 1px solid %2; }"
                            "QLabel#SideBarTitle { color: %3; font-weight: bold; font-size: 11px; text-transform: uppercase; }"
                            "QPushButton#SideBarCloseBtn { border: none; color: %3; background: transparent; font-size: 14px; }"
                            "QPushButton#SideBarCloseBtn:hover { background-color: rgba(255,255,255,0.1); }"
                            "QWidget#ActivityBar { background-color: %1; border-right: 1px solid %2; }"
                            "QPushButton#SideBarNavBtn { border: none; background: transparent; padding: 10px 0; border-right: 2px solid transparent; }"
                            "QPushButton#SideBarNavBtn:checked { background-color: %4; border-left: 3px solid #007ACC; }"
                            "QPushButton#SideBarNavBtn:hover:!checked { background-color: rgba(255,255,255,0.05); }"
                            ).arg(bgColor, borderColor, textColor, activeColor));

    for (int i = 0; i < navButtons.size(); ++i)
    {
        updateButtonIcon(navButtons[i], pageNames[i], dark);
    }
}

bool SideBarContainer::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Enter)
    {
        for (QPushButton *btn : navButtons)
        {
            if (btn != watched)
            {
                QEvent leaveEvent(QEvent::Leave);
                QCoreApplication::sendEvent(btn, &leaveEvent);
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QPushButton *btn = qobject_cast<QPushButton*>(watched);
        if (btn && navButtons.contains(btn))
        {
            QString name = btn->property("pageName").toString();
            if (!isFeatureEnabled(name))
            {
                btn->setChecked(false);
                emit featureDisabledClicked(name);
                return true;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void SideBarContainer::setupUI()
{
    setObjectName("SideBarRoot");
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *activityBar = new QWidget;
    activityBar->setObjectName("ActivityBar");
    activityBar->setFixedWidth(45);
    navLayout = new QVBoxLayout(activityBar);
    navLayout->setContentsMargins(0, 0, 0, 0);
    navLayout->setSpacing(0);
    navLayout->setAlignment(Qt::AlignTop);

    // 左侧面板，但懒得改了
    QWidget *rightPart = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPart);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    QWidget *header = new QWidget;
    header->setFixedHeight(24);
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(15, 4, 5, 0);
    titleLabel = new QLabel("SIDEBAR");
    titleLabel->setObjectName("SideBarTitle");
    titleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    headerLayout->addWidget(titleLabel, 0, Qt::AlignTop);
    headerLayout->addStretch();
    contentStack = new QStackedWidget;
    rightLayout->addWidget(header);
    rightLayout->addWidget(contentStack);

    mainLayout->addWidget(activityBar);
    mainLayout->addWidget(rightPart, 1);
}

void SideBarContainer::updateButtonIcon(QPushButton *btn, const QString &pageName, bool dark)
{
    QString suffix = dark ? "dark" : "light";
    QString prefix;
    if (pageName == "项目管理器")
    {
        prefix = "project_manager";
    }
    else if (pageName == "文件浏览器")
    {
        prefix = "file_browser";
    }
    else if (pageName == "符号浏览器")
    {
        prefix = "symbol_browser";
    }
    else if (pageName == "大纲视图")
    {
        prefix = "outline";
    }
    else if (pageName == "诊断工具")
    {
        prefix = "diagnostic";
    }
    else if (pageName == "代码片段")
    {
        prefix = "code_snippet";
    }
    else if (pageName == "对拍器")
    {
        prefix = "comparator";
    }
    else if (pageName == "AI 助手")
    {
        prefix = "ai";
    }
    else if (pageName == "洛谷题目爬取器")
    {
        prefix = "luogu";
    }
    else if (pageName == "数据评测器")
    {
        prefix = "tester";
    }
    else
    {
        return;
    }

    QString iconPath = QString(":/icons/%1_%2.svg").arg(prefix, suffix);
    if (QFile::exists(iconPath))
    {
        QIcon icon(iconPath);
        btn->setIcon(icon);
        btn->setIconSize(QSize(20, 20));
        btn->setText("");
    }
    else
    {
        btn->setText(pageName);
        btn->setIcon(QIcon());
    }
}

void SideBarContainer::setFeatureEnabled(const QString &name, bool enabled)
{
    featureEnabledMap[name] = enabled;
    for (int i = 0; i < pageNames.size(); ++i)
    {
        if (pageNames[i] == name)
        {
            if (!enabled)
            {
                navButtons[i]->setStyleSheet("QPushButton { opacity: 0.5; }");
            }
            else
            {
                navButtons[i]->setStyleSheet("");
            }
            break;
        }
    }
}

bool SideBarContainer::isFeatureEnabled(const QString &name) const
{
    return featureEnabledMap.value(name, true);
}

// 人工 + AI 辅助
namespace {
    QString difficultyToString(int diff) {
        switch (diff) {
        case 0: return QObject::tr("暂无评定");
        case 1: return QObject::tr("入门");
        case 2: return QObject::tr("普及-");
        case 3: return QObject::tr("普及/提高-");
        case 4: return QObject::tr("普及+/提高");
        case 5: return QObject::tr("提高+/省选-");
        case 6: return QObject::tr("省选/NOI-");
        case 7: return QObject::tr("NOI/NOI+/CTSC");
        default: return QObject::tr("未知");
        }
    }

    QString timeLimitToString(int ms) {
        if (ms % 1000 == 0)
            return QString::number(ms / 1000) + "s";
        else
            return QString::number(ms) + "ms";
    }

    QString memoryLimitToString(int kb) {
        if (kb % 1024 == 0 && kb > 0)
            return QString::number(kb / 1024) + "MB";
        else
            return QString::number(kb) + "KB";
    }

    QString preprocessForCopy(const QString &text) {
        QString result = text;
        QRegularExpression antiAiRegex(R"(::anti-ai\[(.*?)\])");
        result.replace(antiAiRegex, "\\1");
        return result;
    }

    QString buildFullMarkdown(const QString &pid, const QString &title,
                              const QString &background,
                              const QString &description,
                              const QString &inputFormat,
                              const QString &outputFormat,
                              const QJsonArray &samples,
                              const QString &hint)
    {
        QString bg = preprocessForCopy(background);
        QString desc = preprocessForCopy(description);
        QString in = preprocessForCopy(inputFormat);
        QString out = preprocessForCopy(outputFormat);
        QString ht = preprocessForCopy(hint);

        QString md = QString("# %1 %2\n\n").arg(pid).arg(title);

        if (!bg.isEmpty()) md += "## 题目背景\n\n" + bg + "\n\n";
        if (!desc.isEmpty()) md += "## 题目描述\n\n" + desc + "\n\n";
        if (!in.isEmpty()) md += "## 输入格式\n\n" + in + "\n\n";
        if (!out.isEmpty()) md += "## 输出格式\n\n" + out + "\n\n";

        if (!samples.isEmpty()) {
            for (int i = 0; i < samples.size(); ++i) {
                QJsonArray sample = samples[i].toArray();
                if (sample.size() >= 2) {
                    QString input = sample[0].toString().trimmed();
                    QString output = sample[1].toString().trimmed();
                    md += QString("## 输入输出样例 #%1\n\n").arg(i+1);
                    md += "### 输入 #1\n\n```\n" + input + "\n```\n\n";
                    md += "### 输出 #1\n\n```\n" + output + "\n```\n\n";
                }
            }
        }

        if (!ht.isEmpty()) md += "## 说明/提示\n\n" + ht + "\n\n";
        return md;
    }

    QString escapeForJsString(const QString &str) {
        QString escaped = str;
        escaped.replace('\\', "\\\\");
        escaped.replace('\'', "\\'");
        escaped.replace('\"', "\\\"");
        escaped.replace('\n', "\\n");
        escaped.replace('\r', "\\r");
        return escaped;
    }

    QString convertMarkdownTableToHtml(const QString &markdownTable) {
        QStringList lines = markdownTable.split('\n', Qt::SkipEmptyParts);
        if (lines.size() < 3) return markdownTable;

        QString headerDivider = lines[1].trimmed();
        QStringList alignCells = headerDivider.split('|', Qt::SkipEmptyParts);
        int colCount = alignCells.size();
        QVector<QString> aligns(colCount, "left");
        for (int i = 0; i < colCount; ++i) {
            QString cell = alignCells[i].trimmed();
            if (cell.startsWith(':') && cell.endsWith(':')) aligns[i] = "center";
            else if (cell.endsWith(':')) aligns[i] = "right";
            else aligns[i] = "left";
        }

        int dataRowCount = lines.size() - 2;
        QVector<QVector<QString>> grid(dataRowCount, QVector<QString>(colCount, ""));
        for (int row = 0; row < dataRowCount; ++row) {
            QString line = lines[row + 2].trimmed();
            if (line.startsWith('|')) line = line.mid(1);
            if (line.endsWith('|')) line.chop(1);
            QStringList cells = line.split('|');
            for (int col = 0; col < cells.size() && col < colCount; ++col)
                grid[row][col] = cells[col].trimmed();
        }

        struct CellInfo {
            int rowspan = 1;
            int colspan = 1;
            bool skip = false;
            QString content;
        };
        QVector<QVector<CellInfo>> info(dataRowCount, QVector<CellInfo>(colCount));
        for (int row = 0; row < dataRowCount; ++row)
            for (int col = 0; col < colCount; ++col)
                info[row][col].content = grid[row][col];

        // ^ 向上合并
        for (int row = 1; row < dataRowCount; ++row) {
            for (int col = 0; col < colCount; ++col) {
                if (info[row][col].content == "^") {
                    int srcRow = row - 1;
                    while (srcRow >= 0 && info[srcRow][col].content == "^") srcRow--;
                    if (srcRow >= 0) {
                        info[srcRow][col].rowspan = row - srcRow + 1;
                        info[row][col].skip = true;
                    }
                }
            }
        }

        // < 向左合并
        for (int row = 0; row < dataRowCount; ++row) {
            for (int col = 1; col < colCount; ++col) {
                if (info[row][col].content == "<" && !info[row][col].skip) {
                    int srcCol = col - 1;
                    while (srcCol >= 0 && info[row][srcCol].content == "<") srcCol--;
                    if (srcCol >= 0) {
                        info[row][srcCol].colspan = col - srcCol + 1;
                        info[row][col].skip = true;
                    }
                }
            }
        }

        QString html = "<table border=\"1\" style=\"border-collapse:collapse;\">\n";

        QString headerLine = lines[0].trimmed();
        if (headerLine.startsWith('|')) headerLine = headerLine.mid(1);
        if (headerLine.endsWith('|')) headerLine.chop(1);
        QStringList headerCells = headerLine.split('|');
        html += "  <thead>\n    <tr>\n";
        for (int i = 0; i < headerCells.size() && i < colCount; ++i)
            html += QString("      <th style=\"text-align:%1;padding:4px 8px;\" class=\"luogu-th\">%2</th>\n")
                        .arg(aligns[i], headerCells[i].trimmed());
        html += "    </tr>\n  </thead>\n  <tbody>\n";

        for (int row = 0; row < dataRowCount; ++row) {
            html += "    <tr>\n";
            for (int col = 0; col < colCount; ++col) {
                if (info[row][col].skip) continue;
                QString tdAttr;
                if (info[row][col].rowspan > 1)
                    tdAttr += QString(" rowspan=\"%1\"").arg(info[row][col].rowspan);
                if (info[row][col].colspan > 1)
                    tdAttr += QString(" colspan=\"%1\"").arg(info[row][col].colspan);
                tdAttr += QString(" style=\"text-align:%1;padding:4px 8px;\"").arg(aligns[col]);
                html += QString("      <td%1 class=\"luogu-td\">%2</td>\n").arg(tdAttr, info[row][col].content);
            }
            html += "    </tr>\n";
        }
        html += "  </tbody>\n</table>\n";
        return html;
    }

    QString preprocessLuoguMarkdown(const QString &input) {
        QString result = input;
        QRegularExpression antiAiHideRegex(R"(::anti-ai\[(.*?)\])");
        result.replace(antiAiHideRegex, "");

        QRegularExpression tableBlockRegex(R"(::cute-table\{tuack\}\s*\n((?:\|[^\n]*\n?)+))");
        QRegularExpressionMatchIterator it = tableBlockRegex.globalMatch(result);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString tableMarkdown = match.captured(1);
            QString htmlTable = convertMarkdownTableToHtml(tableMarkdown);
            result.replace(match.capturedStart(), match.capturedLength(), htmlTable);
        }

        QStringList lines = result.split('\n');
        QStringList outLines;
        bool inAlign = false;
        QString currentAlign;
        QStringList alignContent;

        QRegularExpression startRegex(R"(^\s*:::align\{([^}]+)\}\s*$)");
        QRegularExpression endRegex(R"(^\s*:::\s*$)");

        for (const QString &line : lines) {
            if (!inAlign) {
                QRegularExpressionMatch startMatch = startRegex.match(line);
                if (startMatch.hasMatch()) {
                    inAlign = true;
                    currentAlign = startMatch.captured(1).trimmed();
                    alignContent.clear();
                } else {
                    outLines.append(line);
                }
            } else {
                QRegularExpressionMatch endMatch = endRegex.match(line);
                if (endMatch.hasMatch()) {
                    QString content = alignContent.join('\n');
                    QString replacement = QString("<div style=\"text-align:%1;\">\n%2\n</div>").arg(currentAlign, content);
                    outLines.append(replacement);
                    inAlign = false;
                } else {
                    alignContent.append(line);
                }
            }
        }

        if (inAlign) {
            QString content = alignContent.join('\n');
            QString replacement = QString("<div style=\"text-align:%1;\">\n%2\n</div>").arg(currentAlign, content);
            outLines.append(replacement);
        }

        result = outLines.join('\n');
        return result;
    }

    QString buildProblemHtml(const QString &fullTitle,
                             const QString &difficultyStr,
                             const QString &timeStr,
                             const QString &memoryStr,
                             const QString &background,
                             const QString &description,
                             const QString &inputFormat,
                             const QString &outputFormat,
                             const QJsonArray &samples,
                             const QString &hint)
    {
        auto hideAntiAi = [](QString text) -> QString {
            return text.replace(QRegularExpression(R"(::anti-ai\[.*?\])"), "");
        };

        auto processCuteTables = [](QString text) -> QString {
            QRegularExpression re(R"(::cute-table\{tuack\}\s*\n((?:\|[^\n]*\n?)+))");
            QString result = text;
            QList<QRegularExpressionMatch> matches;
            auto it = re.globalMatch(result);
            while (it.hasNext()) matches.append(it.next());
            for (int i = matches.size() - 1; i >= 0; --i) {
                auto &m = matches[i];
                result.replace(m.capturedStart(), m.capturedLength(),
                               convertMarkdownTableToHtml(m.captured(1)));
            }
            return result;
        };

        auto processPlainTables = [](QString text) -> QString {
            QStringList lines = text.split('\n');
            QStringList out;
            int i = 0;
            while (i < lines.size()) {
                if (i + 1 < lines.size()
                    && lines[i].trimmed().startsWith('|')
                    && lines[i+1].trimmed().startsWith('|')
                    && lines[i+1].contains(QRegularExpression(R"([-:]+)")))
                {
                    QStringList tableLines;
                    tableLines.append(lines[i]);
                    tableLines.append(lines[i+1]);
                    int j = i + 2;
                    while (j < lines.size() && lines[j].trimmed().startsWith('|')) {
                        tableLines.append(lines[j]);
                        ++j;
                    }
                    QString tableBlock = tableLines.join('\n');
                    QString html = convertMarkdownTableToHtml(tableBlock);
                    out.append(html);
                    i = j;
                } else {
                    out.append(lines[i]);
                    ++i;
                }
            }
            return out.join('\n');
        };

        QString bgProcessed    = processPlainTables(processCuteTables(hideAntiAi(background)));
        QString descProcessed  = processPlainTables(processCuteTables(hideAntiAi(description)));
        QString inProcessed    = processPlainTables(processCuteTables(hideAntiAi(inputFormat)));
        QString outProcessed   = processPlainTables(processCuteTables(hideAntiAi(outputFormat)));
        QString hintProcessed  = processPlainTables(processCuteTables(hideAntiAi(hint)));

        QString bgJs    = escapeForJsString(bgProcessed);
        QString descJs  = escapeForJsString(descProcessed);
        QString inJs    = escapeForJsString(inProcessed);
        QString outJs   = escapeForJsString(outProcessed);
        QString hintJs  = escapeForJsString(hintProcessed);

        QString samplesHtml;
        if (!samples.isEmpty()) {
            samplesHtml += "<h2>输入输出样例</h2>\n";
            for (int i = 0; i < samples.size(); ++i) {
                QJsonArray sample = samples[i].toArray();
                if (sample.size() >= 2) {
                    samplesHtml += QString(R"(
        <div class="sample">
            <h3>样例输入 #%1</h3>
            <pre><code class="sample-code">%2</code></pre>
            <h3>样例输出 #%1</h3>
            <pre><code class="sample-code">%3</code></pre>
        </div>
    )").arg(i+1)
                                       .arg(sample[0].toString().toHtmlEscaped())
                                       .arg(sample[1].toString().toHtmlEscaped());
                }
            }
        }

        auto getDifficultyColor = [](const QString &diff) -> QString
        {
            if (diff.contains("入门"))           return "#FE4C61";
            if (diff.contains("普及-"))          return "#F39C11";
            if (diff.contains("普及/提高-"))     return "#FFC116";
            if (diff.contains("普及+/提高"))     return "#52C41A";
            if (diff.contains("提高+/省选-"))    return "#3498DB";
            if (diff.contains("省选/NOI-"))      return "#9D3DCF";
            if (diff.contains("NOI/NOI+/CTSC")) return "#0E1D69";
            return "#BFBFBF";
        };
        QString difficultyHtml = QString(
                                     "<span style=\"background-color:%1;color:white;padding:2px 8px;"
                                     "border-radius:12px;font-weight:500;\">%2</span>")
                                     .arg(getDifficultyColor(difficultyStr), difficultyStr.toHtmlEscaped());
        QString infoHtml = QString(
                               "    <div class=\"info\">难度: %1 | 时间限制: %2 | 空间限制: %3</div>\n")
                               .arg(difficultyHtml, timeStr.toHtmlEscaped(), memoryStr.toHtmlEscaped());

        QString htmlTemplate = R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="referrer" content="no-referrer">
    <title>)" + fullTitle.toHtmlEscaped() + R"(</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/github-markdown-css@5/github-markdown.min.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/github.min.css">
    <style>
        body { padding: 20px; }
        .markdown-body { box-sizing: border-box; min-width: 200px; max-width: 980px; margin: 0 auto; }
        pre { background-color: #f6f8fa; border-radius: 3px; padding: 16px; overflow: auto; }
        code { font-family: 'SF Mono', Monaco, Consolas, monospace; }
        .sample pre code.sample-code, .sample pre code.sample-code * {
            color: #24292e !important; background-color: #f6f8fa !important;
            border: none !important; text-decoration: none !important;
            font-weight: normal !important; font-style: normal !important;
        }
        .sample { margin: 20px 0; } .sample pre { margin: 5px 0; }
        .info { margin-bottom: 20px; padding: 10px; background-color: #f0f0f0; border-radius: 5px; }
        details { margin: 1em 0; padding: 8px 12px; border-left: 4px solid #ddd; border-radius: 4px; }
        summary { font-weight: bold; cursor: pointer; }
        blockquote footer { margin-top: 0.5em; font-style: italic; color: #666; }
        table { border-collapse: collapse; margin: 1em 0; }
        td, th { border: 1px solid #ddd; padding: 4px 8px; }
    </style>
    <script>
        window.MathJax = {
            tex: {
                inlineMath: [['$', '$'], ['\\(', '\\)']],
                displayMath: [['$$', '$$'], ['\\[', '\\]']],
                processEscapes: true,
                tags: 'none',
                macros: {
                    bm: ["\\boldsymbol{#1}", 1]
                }
            },
            options: { skipHtmlTags: ['script','noscript','style','textarea','pre','code'] }
        };
    </script>
    <script src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml-full.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/marked/marked.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
    <script>
    (function() {
        function protectMath(src) {
            const store = [];
            function escapeHtml(text) {
                const div = document.createElement('div');
                div.textContent = text;
                return div.innerHTML;
            }

            src = src.replace(/(^|\n)(>?\s*)\$\$([\s\S]*?)\$\$/g, (match, lineStart, prefix, inner) => {
                let cleanedInner = inner.split('\n').map(line => {
                    return line.replace(/^\s*>\s?/, '');
                }).join('\n');

                let toStore = cleanedInner;
                if (toStore.includes('\\\\') && !/\\begin\{/.test(toStore)) {
                    if (toStore.includes('&')) {
                        toStore = '\\begin{aligned}' + toStore + '\\end{aligned}';
                    } else {
                        toStore = '\\begin{gathered}' + toStore + '\\end{gathered}';
                    }
                }
                store.push('$$' + escapeHtml(toStore) + '$$');
                return lineStart + prefix + '\x02MATH' + (store.length - 1) + '\x03';
            });

            src = src.replace(/\$([^\n$]+?)\$/g, (_, inner) => {
                store.push('$' + escapeHtml(inner) + '$');
                return '\x02MATH' + (store.length - 1) + '\x03';
            });

            return { src, store };
        }

        function restoreMath(html, store) {
            return html.replace(/\x02MATH(\d+)\x03/g, (_, i) => store[parseInt(i)]);
        }

        // Bilibili 视频
        const bilibiliVideo = {
            name: 'bilibiliVideo',
            level: 'inline',
            start(src) { return src.indexOf('![](bilibili:'); },
            tokenizer(src) {
                const m = /^!\[\]\(bilibili:([A-Za-z0-9]+)(\?[^)]*?)?\)/.exec(src);
                if (!m) return undefined;
                return { type: 'bilibiliVideo', raw: m[0], vid: m[1], query: m[2] || '' };
            },
            renderer(token) {
                let { vid, query } = token;
                let page = 1, t = 0;
                if (query) {
                    const p = new URLSearchParams(query.slice(1));
                    page = parseInt(p.get('page')) || 1;
                    t    = parseInt(p.get('t'))    || 0;
                }
                let base = 'https://player.bilibili.com/player.html?isOutside=true&high_quality=1&danmaku=0';
                if      (/^BV/i.test(vid)) base += `&bvid=${vid}`;
                else if (/^av/i.test(vid)) base += `&aid=${vid.slice(2)}`;
                else                       base += `&aid=${vid}`;
                base += `&page=${page}&t=${t}&autoplay=0`;
                return `<div style="position:relative;padding-bottom:56.25%;height:0;overflow:hidden;margin:1em 0;">` +
                       `<iframe src="${base}" style="position:absolute;top:0;left:0;width:100%;height:100%;border:0;" ` +
                       `allowfullscreen="true" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; fullscreen" ` +
                       `referrerpolicy="no-referrer-when-downgrade"></iframe></div>\n`;
            }
        };

        const luoguBlock = {
            name: 'luoguBlock',
            level: 'block',
            start(src) {
                const idx = src.search(/^:{2,}[a-z]/m);
                return idx === -1 ? undefined : idx;
            },
            tokenizer(src) {
                const hm = /^(:{2,})([a-z]+)(?:\{([^}]*)\})?(?:\[([^\]]*)\])?(\{open\})?\s*\n/.exec(src);
                if (!hm) return undefined;
                const colons = hm[1], blockType = hm[2];
                const braceParam = hm[3] || '', bracketParam = hm[4] || '', isOpen = !!hm[5];
                const after = src.slice(hm[0].length);
                const em = new RegExp('(?:^|\\n)' + colons + '(?!:)[ \\t]*(?:\\n|$)').exec(after);
                if (!em) return undefined;
                return {
                    type: 'luoguBlock',
                    raw: hm[0] + after.slice(0, em.index + em[0].length),
                    blockType, braceParam, bracketParam, isOpen,
                    text: after.slice(0, em.index)
                };
            },
            renderer(token) {
                const { blockType, braceParam, bracketParam, isOpen, text } = token;
                if (blockType === 'align')
                    return `<div style="text-align:${braceParam||'left'};">\n${marked.parse(text)}\n</div>\n`;
                if (blockType === 'epigraph') {
                    const attr = bracketParam || braceParam || '';
                    const footer = attr ? `<footer style="text-align:right;font-style:italic;color:#666;">${attr}</footer>` : '';
                    return `<blockquote><p>${marked.parseInline(text)}</p>${footer}</blockquote>\n`;
                }
                if (['info','success','warning','error'].includes(blockType)) {
                    const cm = {info:'#3498db',success:'#52c41a',warning:'#ffc116',error:'#e74c3c'};
                    const c = cm[blockType];
                    const title = bracketParam || blockType;
                    return `<details${isOpen?' open':''} style="border-left:4px solid ${c};padding:8px 12px;margin:8px 0;">` +
                           `<summary style="color:${c};font-weight:bold;">${title}</summary>\n${marked.parse(text)}\n</details>\n`;
                }
                return marked.parse(text);
            }
        };

        marked.use({ extensions: [bilibiliVideo, luoguBlock] });

        function renderMarkdown(containerId, rawText) {
            const el = document.getElementById(containerId);
            if (!el || !rawText) return;
            const { src, store } = protectMath(rawText);
            let html = marked.parse(src, { gfm: true });

            const tempDiv = document.createElement('div');
            tempDiv.innerHTML = html;
            tempDiv.querySelectorAll('.luogu-td, .luogu-th').forEach(cell => {
                cell.innerHTML = marked.parseInline(cell.innerHTML);
            });
            html = tempDiv.innerHTML;

            html = restoreMath(html, store);
            el.innerHTML = html;
        }
        window._renderMarkdown = renderMarkdown;
    })();
    </script>
</head>
<body>
<div class="markdown-body">
    <h1>)" + fullTitle.toHtmlEscaped() + R"(</h1>
    )" + infoHtml + "\n";

        if (!bgProcessed.isEmpty())
        {
            htmlTemplate += "    <h2>题目背景</h2><div id=\"background\"></div>\n";
        }
        if (!descProcessed.isEmpty())
        {
              htmlTemplate += "    <h2>题目描述</h2><div id=\"description\"></div>\n";
        }
        if (!inProcessed.isEmpty())
        {
            htmlTemplate += "    <h2>输入格式</h2><div id=\"input-format\"></div>\n";
        }
        if (!outProcessed.isEmpty())
        {
            htmlTemplate += "    <h2>输出格式</h2><div id=\"output-format\"></div>\n";
        }
        htmlTemplate += samplesHtml;
        if (!hintProcessed.isEmpty())
        {
              htmlTemplate += "    <h2>说明/提示</h2><div id=\"hint\"></div>\n";
        }

        htmlTemplate += R"(
</div>
<script>
(function() {
    _renderMarkdown('background',    ')" + bgJs   + R"(');
    _renderMarkdown('description',   ')" + descJs + R"(');
    _renderMarkdown('input-format',  ')" + inJs   + R"(');
    _renderMarkdown('output-format', ')" + outJs  + R"(');
    _renderMarkdown('hint',          ')" + hintJs + R"(');
    hljs.highlightAll();
    if (window.MathJax) MathJax.typesetPromise();
})();
</script>
</body>
</html>
)";
        return htmlTemplate;
    }
}

class LuoguFetcherWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LuoguFetcherWidget(CompilerIDE *ide, QWidget *parent = nullptr);

private slots:
    void onFetchClicked();
    void onCopyClicked();

private:
    void fetchProblem(const QUrl &url);
    QPointer<QWebEnginePage> m_currentPage;
    CompilerIDE *mainWindow;
    QWebEngineView *view;
    QLineEdit *idEdit;
    QPushButton *fetchButton;
    QPushButton *copyButton;
    QString currentMarkdown;
    bool m_isLoading;
};

class DeepSeekAssistantWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeepSeekAssistantWidget(QWidget *parent = nullptr);

private:
    QWebEngineView *view;
    QWebEngineProfile *profile;
    QString profileStoragePath;
};

DeepSeekAssistantWidget::DeepSeekAssistantWidget(QWidget *parent)
    : QWidget(parent)
{
    // 账户数据存储路径
    profileStoragePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
                         + "/DeepSeekProfile";
    QDir().mkpath(profileStoragePath);

    profile = new QWebEngineProfile(profileStoragePath, this);
    profile->setPersistentStoragePath(profileStoragePath);
    profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

    view = new QWebEngineView(profile, this);
    view->setUrl(QUrl("https://chat.deepseek.com/"));
    view->setZoomFactor(0.87);

    connect(view->page(), &QWebEnginePage::permissionRequested,
            this, [this](QWebEnginePermission permission)
    {
        if (permission.permissionType() == QWebEnginePermission::PermissionType::ClipboardReadWrite)
        {
            // 允许使用剪贴板
            permission.grant();
        }
        else
        {
            permission.deny();
        }
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(view);
}

// 对拍器
class CheckerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckerWidget(CompilerIDE *ide, QWidget *parent = nullptr);
    ~CheckerWidget();

private slots:
    void onBrowseStd();
    void onBrowseSol();
    void onBrowseData();
    void onStartClicked();
    void onStopClicked();
    void onResetClicked();
    void onCompileFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onDataFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onStdFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onSolFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onTimeout();

private:
    CompilerIDE *mainWindow;

    QLineEdit *stdPathEdit;
    QLineEdit *solPathEdit;
    QLineEdit *dataPathEdit;

    QLabel *stdStatusIcon;
    QLabel *solStatusIcon;
    QLabel *dataStatusIcon;

    QSpinBox *groupCountSpin;
    QSpinBox *timeLimitSpin;

    QPushButton *startBtn;
    QPushButton *stopBtn;
    QPushButton *resetBtn;

    QPlainTextEdit *inputDisplay;
    QPlainTextEdit *expectedDisplay;
    QPlainTextEdit *actualDisplay;

    QWidget *completionWidget;
    QTimer *completionTimer;

    QLabel *statusLabel;

    QWidget *progressWidget;
    QProgressBar *progressBar;
    QLabel *progressStatusLabel;
    QLabel *progressTimeLabel;

    QGroupBox *detailGroup;

    QProcess *compileProcess;
    QStringList pendingCompileFiles;
    QStringList compileErrors;
    QString currentCompileTarget;
    QLabel *currentStatusIcon;

    QProcess *dataProcess;
    QProcess *stdProcess;
    QProcess *solProcess;
    QTimer *timer;
    QElapsedTimer *stageTimer;
    int currentTimeMs;

    enum State { Idle, Compiling, RunningData, RunningStd, RunningSol, Finished, Stopped } state;

    int totalGroups;
    int currentGroup;
    int timeLimitMs;
    bool stopRequested;

    QString stdExePath;
    QString solExePath;
    QString dataExePath;
    QString tempDir;

    QString currentInput;
    QString currentStdOutput;
    QString currentSolOutput;

    void prewarmPrograms();
    void setState(State newState);
    void updateProgressDisplay();
    void startNextCompile();
    QStringList buildCompilerArgs(const QString &sourceFile, const QString &outputExe);
    void parseCompilerOutput(const QString &output, const QString &target,
                             const QString &fullPath,
                             QStringList &errors, QStringList &warnings);
    void setIconPending(QLabel *icon);
    void updateIcon(QLabel *icon, bool success, const QString &tooltip = QString());
    void runData();
    void runStd();
    void runSol();
    void compareOutputs();
    void showWAInfo();
    void cleanUpTempFiles();
};

// IDE 核心
class CompilerIDE : public QMainWindow
{
    Q_OBJECT

public:
    CompilerIDE();
    ~CompilerIDE();

    void loadFile(const QString &fileName);
    void newFile();
    void updateErrorTable(const QStringList &errors, const QStringList &warnings);
    bool isDarkTheme() const { return darkTheme; }
    bool maybeSaveAll();
    void restartIDEWithMessage();
    QString getCompilerPath() const { return compilerPath; }
    QString getCompilerType() const { return compilerType; }
    QString getCppStandard() const { return cppStandard; }
    QString getOptimizationLevel() const { return optimizationLevel; }
    void showFloatingMessage(const QString &message, int duration = 3000, bool isError = false);

public slots:
    void checkForUpdates();
    void autoCheckForUpdates();

signals:
    void compilationFinished(bool success);

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    QStringList recentFiles;
    int untitledCounter;
    bool isEasyXProject = false;
    QMap<QWidget*, bool> editorEasyXFlags;
    bool competitionMode;
    QTimer *competitionTimer = nullptr;
    QLabel *timerLabel = nullptr;
    int timerSeconds;
    bool isCountdown;
    QDockWidget *competitionDock = nullptr;
    QWidget *competitionWidget = nullptr;
    QAction *debugPanelAct = nullptr;
    bool comp_disableCodeCompletion;
    bool comp_disableDeepSeek;
    bool originalDeepSeekVisible;
    bool comp_disableAutoBrackets;
    bool comp_disableAutoQuotes;
    bool comp_disableAutoIndent;
    QAction *competitionModeAct = nullptr;
    QAction *startCountdownAct = nullptr;
    QAction *startStopwatchAct = nullptr;
    QAction *stopTimerAct = nullptr;
    QAction *resetTimerAct = nullptr;
    QLabel *floatingTimeWarning = nullptr;
    QTimer *warningFlashTimer = nullptr;
    bool warningVisible;
    HANDLE hProcess = NULL;
    QTabWidget *tabWidget = nullptr;
    QPlainTextEdit *outputEdit = nullptr;
    QTableWidget *errorTableWidget = nullptr;
    QDockWidget *outputDock = nullptr;
    QDockWidget *errorDock = nullptr;
    ProjectManager *projectManager = nullptr;
    QDockWidget *projectDock = nullptr;
    QDockWidget *fileBrowserDock = nullptr;
    QDockWidget *symbolBrowserDock = nullptr;
    QDockWidget *outlineDock = nullptr;
    QProcess *compileProcess = nullptr;
    QProcess *runProcess = nullptr;
    QString curFile;
    QString compilerPath;
    QString compilerType;
    QString cppStandard;
    bool autoBrackets;
    bool autoQuotes;
    bool autoIndent;
    int indentSize;
    bool codeFolding;
    bool lineNumbers;
    bool darkTheme;
    QMenu *fileMenu = nullptr;
    QMenu *editMenu = nullptr;
    QMenu *buildMenu = nullptr;
    QMenu *viewMenu = nullptr;
    QMenu *toolsMenu = nullptr;
    QMenu *helpMenu = nullptr;
    QToolBar *fileToolBar = nullptr;
    QToolBar *buildToolBar = nullptr;
    QToolBar *editToolBar = nullptr;
    QToolBar *viewToolBar = nullptr;
    QAction *newAct = nullptr;
    QAction *openAct = nullptr;
    QAction *openProjectAct = nullptr;
    QAction *saveAct = nullptr;
    QAction *saveAsAct = nullptr;
    QAction *closeAct = nullptr;
    QAction *closeAllAct = nullptr;
    QAction *exitAct = nullptr;
    QAction *cutAct = nullptr;
    QAction *copyAct = nullptr;
    QAction *pasteAct = nullptr;
    QAction *toggleCommentAct = nullptr;
    QAction *findAct = nullptr;
    QAction *replaceAct = nullptr;
    QAction *findNextAct = nullptr;
    QAction *findPreviousAct = nullptr;
    QAction *gotoLineAct = nullptr;
    QPointer<QAction> compileAct;
    QPointer<QAction> runAct;
    QPointer<QAction> compileRunAct;
    QAction *clearOutputAct = nullptr;
    QAction *settingsAct = nullptr;
    QAction *aboutAct = nullptr;
    QAction *projectManagerAct = nullptr;
    QAction *fileBrowserAct = nullptr;
    QAction *symbolBrowserAct = nullptr;
    QAction *outlineAct = nullptr;
    QAction *zoomInAct = nullptr;
    QAction *zoomOutAct = nullptr;
    QAction *resetZoomAct = nullptr;
    QAction *aiCompletionAct = nullptr;
    QComboBox *cppStandardCombo = nullptr;
    QDialog *findDialog = nullptr;
    QLineEdit *findEdit = nullptr;
    QCheckBox *caseSensitiveCheck = nullptr;
    QCheckBox *wholeWordCheck = nullptr;
    QCheckBox *regexCheck = nullptr;
    QPushButton *findNextButton = nullptr;
    QPushButton *findPrevButton = nullptr;
    QPushButton *replaceButton = nullptr;
    QPushButton *replaceAllButton = nullptr;
    QLineEdit *replaceEdit = nullptr;
    bool isHidingPanel;
    QDockWidget *diagnosticsDock = nullptr;
    QWidget *diagnosticsWidget = nullptr;
    QTimer *diagnosticsTimer = nullptr;
    QLabel *cpuUsageLabel = nullptr;
    QLabel *memoryUsageLabel = nullptr;
    QLabel *systemUptimeLabel = nullptr;
    QLabel *processMemoryLabel = nullptr;
    QLabel *processThreadsLabel = nullptr;
    QChart *cpuChart = nullptr;
    QChart *memoryChart = nullptr;
    QLineSeries *cpuSeries = nullptr;
    QLineSeries *memorySeries = nullptr;
    QChartView *cpuChartView = nullptr;
    QChartView *memoryChartView = nullptr;
    QVector<QPointF> cpuHistory;
    QVector<QPointF> memoryHistory;
    int maxHistoryPoints;
    qint64 initialUptime;
    QAction *diagnosticsAct = nullptr;
    UpdateManager *updateManager = nullptr;
    QAction *checkForUpdatesAct = nullptr;
    bool isCheckingForUpdates;
    QMetaObject::Connection updateCheckConnection;
    QMetaObject::Connection noUpdateConnection;
    bool comp_disableTerminal;
    bool comp_disableSnippets;
    bool comp_disableDiagnostics;
    bool comp_disableProjectManager;
    bool originalTerminalVisible;
    bool originalSnippetsVisible;
    bool originalDiagnosticsVisible;
    bool originalProjectManagerVisible;
    QPushButton *pauseBtn = nullptr;
    bool usingTempDir;
    QString tempSourceFile;
    QString actualSourceFile;
    QString actualOutputDir;
    QString originalOutputExe;
    QString debuggerPath;
    QDockWidget *debugDock = nullptr;
    QWidget *debugWidget = nullptr;
    QTableWidget *variablesTable = nullptr;
    QListWidget *callStackList = nullptr;
    QLineEdit *watchExpressionEdit = nullptr;
    QListWidget *watchList = nullptr;
    QPlainTextEdit *debugOutputEdit = nullptr;
    bool isDebugging;
    QString currentDebugFile;
    int currentDebugLine;
    QDockWidget *checkerDock = nullptr;
    CheckerWidget *checkerWidget;
    QAction *checkerAct = nullptr;
    QAction *debugRunToCursorAct = nullptr;
    QStringList accumulatedErrors;
    QStringList accumulatedWarnings;
    bool isNewCompilation;
    QTabWidget *debugTabs = nullptr;
    QTextBrowser *debugOutput = nullptr;
    QTableWidget *breakpointsTable = nullptr;
    QTreeWidget *variablesTree = nullptr;
    QLineEdit *expressionInput = nullptr;
    QPushButton *evalButton = nullptr;
    QString currentDebugExecutable;
    enum DebugState {
        DebugState_Idle,
        DebugState_Running,
        DebugState_Paused,
        DebugState_Finished
    };
    DebugState currentDebugState;
    QMap<QString, QString> debugFileMapping;
    QMap<QPair<QString, int>, QString> conditionalBreakpoints;
    QString optimizationLevel;
    QComboBox *optimizationCombo = nullptr;
    QMenu *errorTableContextMenu = nullptr;
    QAction *copyErrorAction = nullptr;
    bool pendingDebugAfterCompile = false;
    QStringList watchExpressions;
    QLabel *competitionStatusLabel = nullptr;
    QString lastTimerStyle;
    bool pendingRunAfterCompile = false;
    QAction *tempCompileAct = nullptr;
    QAction *tempCompileRunAct = nullptr;
    QString tempCompileSourceFile;
    QString tempCompileExeFile;
    bool codeCompletionEnabled;
    QProcess *runningProcess = nullptr;
    HANDLE hRunningProcess = NULL;
    QString currentRunningExe;
    QAction *feedbackAct = nullptr;
    bool codeBeautify;
    Debugger *debugger = nullptr;
    CodeCompleter *codeCompleter = nullptr;
    IntegratedTerminal *terminal = nullptr;
    SnippetManager *snippetManager = nullptr;
    QDockWidget *terminalDock = nullptr;
    QDockWidget *snippetDock = nullptr;
    QAction *debugStartAct = nullptr;
    QAction *debugStopAct = nullptr;
    QAction *debugContinueAct = nullptr;
    QAction *debugStepOverAct = nullptr;
    QAction *debugStepIntoAct = nullptr;
    QAction *debugStepOutAct = nullptr;
    QAction *debugToggleBreakpointAct = nullptr;
    QAction *terminalShowAct = nullptr;
    QAction *snippetShowAct = nullptr;
    bool isRestarting = false;
    QValueAxis *cpuAxisX = nullptr;
    QValueAxis *cpuAxisY = nullptr;
    QValueAxis *memoryAxisX = nullptr;
    QValueAxis *memoryAxisY = nullptr;
    QString defaultNormalTemplate;
    QString defaultEasyXTemplate;
    QAction *templateSettingsAct = nullptr;
    QTextCharFormat defaultFormat;
    QTextCharFormat successFormat;
    QTextCharFormat errorFormat;
    QMap<CodeEditor*, QString> editorEncodings;
    QDockWidget *sampleTesterDock = nullptr;
    QWidget *sampleTesterWidget = nullptr;
    QVBoxLayout *testCasesLayout = nullptr;
    QScrollArea *testCasesScrollArea = nullptr;
    QSpinBox *timeLimitSpinBox = nullptr;
    QPushButton *addTestBtn = nullptr;
    QPushButton *runAllTestsBtn = nullptr;
    QLabel *overallResultLabel = nullptr;
    QList<QWidget*> testCaseWidgets;
    QProcess *testProcess = nullptr;
    QTimer *testTimer = nullptr;
    int currentTestIndex;
    bool isRunningAllTests;
    bool isSilentCompiling;
    QString testExecutablePath;
    struct TestResult {
        QString status;
        int timeMs;
        QString actualOutput;
    };
    QList<TestResult> testResults;
    QAction *formatCodeAct = nullptr;
    QString lastCompiledFile;
    QDockWidget *luoguDock = nullptr;
    QAction *luoguAct = nullptr;
    QFont editorFont;
    SideBarContainer *rightSidebar;
    QDockWidget *aiDock = nullptr;
    QAction *aiAct = nullptr;
    QPointer<QProcess> m_tempCompileProcess;
    bool m_isTempCompiling = false;
    QStringList sidebarFeatureNames;
    QAction *openSourceNoticeAct = nullptr;

    void addToRecentFiles(const QString &filePath);
    void loadRecentFiles();
    void saveRecentFiles();
    void setTheme(bool dark);
    void warmupAndRunTest();
    void createCompetitionDock();
    void updateTimerDisplay();
    void applyCompetitionModeSettings();
    void restoreNormalModeSettings();
    bool isCompetitionMode() const { return competitionMode; }
    QString detectCompilerType(const QString &compilerPath);
    void parseCompilerOutput(const QString &output);
    void createFindDialog();
    bool performFind(bool forward, bool showMessage = true);
    void updateFindDialog();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    void runExecutable(const QString &programPath, bool isAfterCompile = false);
    CodeEditor* currentEditor() const;
    CodeEditor* editorAt(int index) const;
    int findEditor(const QString &fileName) const;
    void checkCompilerSupport();
    void loadPdfFile(const QString &fileName);
    void showFloatingTimeWarning(const QString &message, int duration);
    QPair<QString, QString> extractLineAndColumn(const QString &errorLine);
    void createDiagnosticsDock();
    void updateSystemInfo();
    double getCPUUsage();
    double getMemoryUsage();
    double getProcessMemoryUsage();
    int getProcessThreadCount();
    QString formatUptime(qint64 seconds);
    void updateCharts();
    void cleanupCharts();
    QString getSelectedText();
    QString getCurrentFunction();
    QStringList getLocalVariables(const QString &functionCode);
    bool isValidIdentifier(const QString &name);
    QString generateMethodName(const QString &code);
    QString cleanExtraNewlines(const QString &text);
    void testCompilerCapabilities();
    QStringList getSupportedStandards();
    bool testStandardSupport(const QString &standard);
    QPair<int, int> findFunctionBoundaries(int cursorPosition) {
        CodeEditor *editor = currentEditor();
        if (!editor) return qMakePair(0, 0);
        QTextCursor cursor = editor->textCursor();
        cursor.setPosition(cursorPosition);
        int functionStart = 0;
        int braceCount = 0;
        bool inFunction = false;
        while (!cursor.atStart()) {
            QString line = cursor.block().text();
            if ((line.contains("main") && (line.contains("int") || line.contains("void"))) ||
                (line.contains('{') && !line.trimmed().startsWith("//"))) {
                functionStart = cursor.block().position();
                inFunction = true;
                break;
            }
            cursor.movePosition(QTextCursor::Up);
        }
        if (!inFunction) {
            cursor.movePosition(QTextCursor::Start);
            functionStart = 0;
        }
        cursor.setPosition(functionStart);
        int functionEnd = editor->toPlainText().length();
        braceCount = 0;
        bool counting = false;
        while (!cursor.atEnd()) {
            QString line = cursor.block().text();
            for (int i = 0; i < line.length(); ++i) {
                if (line[i] == '{') {
                    braceCount++;
                    counting = true;
                } else if (line[i] == '}') {
                    braceCount--;
                    if (counting && braceCount == 0) {
                        functionEnd = cursor.block().position() + i + 1;
                        return qMakePair(functionStart, functionEnd);
                    }
                }
            }
            cursor.movePosition(QTextCursor::Down);
        }
        return qMakePair(functionStart, functionEnd);
    }
    QString findDebuggerFromCompilerPath(const QString &compilerPath);
    QString autoDetectDebuggerPath();
    void createDebugDock();
    void updateDebugUI();
    void clearDebugUI();
    void startDebugging(const QString &executable);
    bool isInCommentOrString(const QString &text, int position);
    bool containsCriticalStructure(const QString &code) {
        QStringList criticalPatterns = {
            "int main", "void main", "main(", "int main(",
            "class ", "struct ", "namespace ", "template<"
        };
        for (const QString &pattern : criticalPatterns) {
            if (code.contains(pattern)) {
                return true;
            }
        }
        return false;
    }
    void setupDebugActions();
    void updateDebugControls();
    void clearDebugInfo();
    void refreshBreakpointsTable();
    void updateVariablesTree(const QList<QPair<QString, QString>> &variables);
    void updateCallStack(const QStringList &frames);
    void showExpressionResult(const QString &expression, const QString &value);
    void onBreakpointToggleRequested(int line, bool added);
    void setCurrentDebugLine(int line);
    void clearCurrentDebugLine();
    void setDebugState(DebugState state);
    void killProcessByName(const QString &exeName);
    void loadTemplateSettings();
    void saveTemplateSettings();
    void appendColoredText(const QString &text, const QColor &color) {
        QTextCursor cursor = outputEdit->textCursor();
        QTextCharFormat format;
        format.setForeground(color);
        cursor.movePosition(QTextCursor::End);
        cursor.setCharFormat(format);
        cursor.insertText(text);
        resetOutputFormat();
    }
    QColor getSuccessColor() const {
        return darkTheme ? QColor(0, 255, 255) : QColor(0, 150, 0);
    }
    QColor getFailColor() const {
        return darkTheme ? QColor(255, 50, 50) : QColor(200, 0, 0);
    }
    void resetOutputFormat() {
        QTextCursor cursor = outputEdit->textCursor();
        QTextCharFormat format;
        if (darkTheme) {
            format.setForeground(QColor(220, 220, 220));
        } else {
            format.setForeground(Qt::black);
        }
        cursor.setCharFormat(format);
    }
    void createSampleTesterDock();
    QWidget* createTestCaseWidget(int index);
    void updateTestCaseResult(int index, const QString &status, int timeMs, const QString &actualOutput);
    void updateOverallResult();
    QString getStatusColor(const QString &status);
    void cleanupTestFiles();
    void toggleTestCaseCollapse();
    void setTestCaseCollapsed(int index, bool collapsed);
    void initTextFormats() {
        defaultFormat.setForeground(darkTheme ? QColor(220, 220, 220) : Qt::black);
        successFormat.setForeground(darkTheme ? QColor(0, 255, 255) : QColor(0, 150, 0));
        errorFormat.setForeground(darkTheme ? QColor(255, 50, 50) : QColor(200, 0, 0));
    }
    void appendColoredTextNoNewline(const QString &text, const QTextCharFormat &format) {
        QTextCursor cursor = outputEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.setCharFormat(format);
        cursor.insertText(text);
    }
    void appendColoredText(const QString &text, const QTextCharFormat &format) {
        QTextCursor cursor = outputEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.setCharFormat(format);
        cursor.insertText(text + "\n");
        cursor.setCharFormat(defaultFormat);
        outputEdit->setTextCursor(cursor);
    }
    void appendDefaultText(const QString &text) {
        QTextCursor cursor = outputEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.setCharFormat(defaultFormat);
        cursor.insertText(text + "\n");
    }
    QString detectFileEncoding(const QString &filePath);
    void updateComboBoxStyles();
    void applyGlobalMenuStyle();

private slots:
    void open();
    void openProject();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void compile();
    void run();
    void compileAndRun();
    void clearOutput();
    void compilerFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void readCompilerOutput();
    void readCompilerError();
    void showErrorInCode(int row, int column);
    void updateStatusBar();
    void changeCppStandard(int index);
    void showSettings();
    void applySettings(const QString &compilerPath, bool autoBrackets, bool autoQuotes, bool autoIndent, int indentSize, bool lineNumbers, bool darkTheme, bool codeCompletion, const QFont &editorFont);
    void toggleComment();
    void runProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void readRunOutput();
    void onFileSelected(const QString &filePath);
    void closeTab(int index);
    void currentTabChanged(int index);
    void find();
    void replace();
    void findNext();
    void findPrevious();
    void gotoLine();
    void showProjectManager();
    void showFileBrowser();
    void showSymbolBrowser();
    void showOutline();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void showCompetitionModeDialog();
    void startCompetitionMode(bool countdown, int minutes,
                             bool disableCodeCompletion, bool disableAISuggestions,
                             bool disableAutoBrackets, bool disableAutoQuotes,
                             bool disableAutoIndent,
                             bool disableTerminal, bool disableSnippets,
                             bool disableDiagnostics, bool disableProjectManager);
    void stopCompetitionMode();
    void updateTimer();
    void startCountdownTimer();
    void startStopwatchTimer();
    void stopCompetitionTimer();
    void resetCompetitionTimer();
    void onCompetitionModeToggled(bool enabled);
    void checkDisabledFeature(const QString &featureName);
    void onUpdateCheckFinished(bool updateAvailable, const QString &newVersion);
    void showUpdateInfo();
    void onRefactorActionTriggered();
    void toggleCompetitionTimer();
    void onDebugStarted();
    void onDebugStopped();
    void onDebugPaused(const QString &file, int line, const QString &reason);
    void onDebugResumed();
    void onVariablesUpdated(const QList<QPair<QString, QString>> &variables);
    void onCallStackUpdated(const QStringList &frames);
    void onBreakpointHit(const QString &file, int line, const QString &function);
    void onDebugOutput(const QString &output);
    void onDebugError(const QString &error);
    void addWatchExpression();
    void removeWatchExpression();
    void onCallStackItemClicked(QListWidgetItem *item);
    void onVariableDoubleClicked(int row, int column);
    void addConditionalBreakpoint();
    void editBreakpointCondition();
    void continueStartDebugging();
    void startDebugging();
    void stopDebugging();
    void continueDebugging();
    void stepOver();
    void stepInto();
    void stepOut();
    void toggleBreakpoint();
    void runToCursor();
    void evaluateExpression();
    void setVariableValue();
    void removeAllBreakpoints();
    void onBreakpointTableItemDoubleClicked(QTableWidgetItem *item);
    void onCallStackItemDoubleClicked(QListWidgetItem *item);
    void onVariablesTreeItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onDebuggingStarted();
    void onDebuggingStopped();
    void onDebuggingFinished();
    void onDebugOutputReceived(const QString &output);
    void onDebugErrorReceived(const QString &error);
    void onExecutionPaused(const QString &file, int line, const QString &reason);
    void onExecutionResumed();
    void onExpressionEvaluated(const QString &expression, const QString &value);
    void onBreakpointAdded(const QString &file, int line);
    void onBreakpointRemoved(const QString &file, int line);
    void tempCompile();
    void tempCompileAndRun();
    void showErrorTableContextMenu(const QPoint &pos);
    void copySelectedError();
    void launchDebugger(const QString &sourceFile);
    void showMemoryView();
    void updateMemoryView();
    void runTest(int index);
    void runNextTest();
    void onEditorFontSizeChanged(int newSize);
    void openFeedbackPage();
    void evaluateAllWatches();
    void showTemplateSettings();
    void showSampleTester();
    void addTestCase();
    void removeTestCase();
    void runSingleTest();
    void runAllTests();
    void onTestFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onTestTimeout();
    void silentCompileForTest();
    void onSilentCompileFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void formatAllCode();
    void changeOptimizationLevel(int index);
};

CompilerIDE::CompilerIDE()
    : compileProcess(nullptr), runProcess(nullptr),
    cppStandard("c++17"), autoBrackets(true), autoQuotes(true),
    autoIndent(true), indentSize(4), lineNumbers(true),
    darkTheme(true), debugger(nullptr), codeCompleter(nullptr),
    terminal(nullptr), snippetManager(nullptr), diagnosticsTimer(nullptr), maxHistoryPoints(60), initialUptime(0), isNewCompilation(false), errorTableWidget(nullptr), isCheckingForUpdates(false),      isDebugging(false),
    currentDebugState(DebugState_Idle),
    debugDock(nullptr),
    debugWidget(nullptr),
    debugTabs(nullptr),
    comp_disableDeepSeek(false),
    originalDeepSeekVisible(false),
    debugOutput(nullptr),
    breakpointsTable(nullptr),
    variablesTree(nullptr),
    codeBeautify(true),
    callStackList(nullptr),
    expressionInput(nullptr),
    evalButton(nullptr),
    debugStartAct(nullptr),
    debugStopAct(nullptr),
    viewMenu(nullptr),
    debugContinueAct(nullptr),
    debugStepOverAct(nullptr),
    debugStepIntoAct(nullptr),
    debugStepOutAct(nullptr),
    debugToggleBreakpointAct(nullptr),
    debugRunToCursorAct(nullptr),
    untitledCounter(1)
{
    QApplication::setStyle(new ModernStyle);

    setWindowIcon(QIcon(":/icons/CompilerIDE_logo.png"));

    loadRecentFiles();

    setWindowTitle(tr("Compiler IDE %1").arg(IDE_VERSION));
    setUnifiedTitleAndToolBarOnMac(true);

    tabWidget = new QTabWidget;
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    setCentralWidget(tabWidget);

    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &CompilerIDE::closeTab);
    connect(tabWidget, &QTabWidget::currentChanged, this, &CompilerIDE::currentTabChanged);

    createDiagnosticsDock();
    createDebugDock();

    debugger = new Debugger(this);
    isDebugging = false;
    codeCompleter = new CodeCompleter(this);
    terminal = new IntegratedTerminal(this);
    snippetManager = new SnippetManager(this);

    updateManager = new UpdateManager(this);
    connect(updateManager, &UpdateManager::updateCheckFinished,
            this, &CompilerIDE::onUpdateCheckFinished);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    createDockWindows();

    createFindDialog();

    readSettings();

    qDebug() << "[Constructor] 刚刚读取完 editorFont 的时候:" << editorFont.family() << editorFont.pointSize();

    initTextFormats();

    setTheme(darkTheme);

    compilerType = detectCompilerType(compilerPath);

    checkCompilerSupport();

    terminalDock = terminal;
    terminalDock->setVisible(false);
    addDockWidget(Qt::BottomDockWidgetArea, terminalDock);

    snippetDock = snippetManager;
    snippetDock->setVisible(false);
    snippetManager->setFeatures(QDockWidget::NoDockWidgetFeatures);
    snippetManager->setTitleBarWidget(new QWidget());
    addDockWidget(Qt::RightDockWidgetArea, snippetDock);

    if (debuggerPath.isEmpty())
    {
        debuggerPath = findDebuggerFromCompilerPath(compilerPath);
        if (debuggerPath.isEmpty())
        {
            debuggerPath = autoDetectDebuggerPath();
        }
    }

    if (debugger)
    {
        debugger->setDebuggerPath(debuggerPath);
    }

    connect(this, &CompilerIDE::compilationFinished, this, [this](bool success)
    {
        if (success)
        {
            debugStartAct->setEnabled(true);
        }
    });

    connect(debugger, &Debugger::breakpointHit, this, [this](const QString &file, int line, const QString &function)
    {
        outputEdit->appendPlainText(tr("断点命中: %1 第 %2 行").arg(file).arg(line));
    });

    connect(debugger, &Debugger::outputReceived, this, [this](const QString &output)
    {
        outputEdit->appendPlainText(tr("[调试] ") + output);
    });

    connect(debugger, &Debugger::variablesUpdated,
            this, &CompilerIDE::updateVariablesTree);

    connect(debugger, &Debugger::callStackUpdated,
            this, &CompilerIDE::updateCallStack);

    connect(debugger, &Debugger::executionPaused,
            this, &CompilerIDE::onExecutionPaused);

    connect(snippetManager, &SnippetManager::snippetInserted, this, [this](const QString &code)
    {
        CodeEditor *editor = currentEditor();
        if (editor)
        {
            editor->insertPlainText(code);
        }
    });

#ifdef Q_OS_WIN
    if (compilerPath.isEmpty())
    {
        compilerPath = "g++.exe";
    }
#else
    if (compilerPath.isEmpty())
    {
        compilerPath = "g++";
    }
#endif

    updateStatusBar();

    QString initialDir = QDir::currentPath();
    terminal->setWorkingDirectory(initialDir);

    connect(debugger, &Debugger::debuggingStarted, this, &CompilerIDE::onDebuggingStarted);
    connect(debugger, &Debugger::debuggingStopped, this, &CompilerIDE::onDebuggingStopped);
    connect(debugger, &Debugger::debuggingFinished, this, &CompilerIDE::onDebuggingFinished);
    connect(debugger, &Debugger::outputReceived, this, &CompilerIDE::onDebugOutputReceived);
    connect(debugger, &Debugger::errorReceived, this, &CompilerIDE::onDebugErrorReceived);
    connect(debugger, &Debugger::executionPaused, this, &CompilerIDE::onExecutionPaused);
    connect(debugger, &Debugger::executionResumed, this, &CompilerIDE::onExecutionResumed);
    connect(debugger, &Debugger::breakpointHit, this, &CompilerIDE::onBreakpointHit);
    connect(debugger, &Debugger::variablesUpdated, this, &CompilerIDE::onVariablesUpdated);
    connect(debugger, &Debugger::callStackUpdated, this, &CompilerIDE::onCallStackUpdated);
    connect(debugger, &Debugger::expressionEvaluated, this, &CompilerIDE::onExpressionEvaluated);
    connect(debugger, &Debugger::breakpointAdded, this, &CompilerIDE::onBreakpointAdded);
    connect(debugger, &Debugger::breakpointRemoved, this, &CompilerIDE::onBreakpointRemoved);

    createDebugDock();

    sampleTesterDock = nullptr;
    testProcess = nullptr;
    testTimer = nullptr;
    currentTestIndex = -1;
    isRunningAllTests = false;
    isSilentCompiling = false;
    createSampleTesterDock();

    competitionMode = false;
    competitionTimer = nullptr;
    timerLabel = nullptr;
    timerSeconds = 0;
    isCountdown = false;

    comp_disableCodeCompletion = true;
    comp_disableDeepSeek = true;
    comp_disableAutoBrackets = false;
    comp_disableAutoQuotes = false;
    comp_disableAutoIndent = false;

    comp_disableTerminal = false;
    comp_disableSnippets = false;
    comp_disableDiagnostics = false;
    comp_disableProjectManager = false;

    originalTerminalVisible = false;
    originalSnippetsVisible = false;
    originalDiagnosticsVisible = false;
    originalProjectManagerVisible = false;
    isHidingPanel = false;

    lastTimerStyle.clear();

    createCompetitionDock();

    setProperty("darkTheme", darkTheme);

    connect(terminalDock, &QDockWidget::visibilityChanged,
            terminalShowAct, &QAction::setChecked);

    connect(snippetDock, &QDockWidget::visibilityChanged,
            snippetShowAct, &QAction::setChecked);

    connect(diagnosticsDock, &QDockWidget::visibilityChanged,
            diagnosticsAct, &QAction::setChecked);

    connect(debugDock, &QDockWidget::visibilityChanged,
            debugPanelAct, &QAction::setChecked);

    connect(projectDock, &QDockWidget::visibilityChanged,
            projectManagerAct, &QAction::setChecked);

    connect(luoguDock, &QDockWidget::visibilityChanged, this, [this](bool visible)
    {
        QTimer::singleShot(0, this, [this, visible]()
        {
            luoguAct->setChecked(visible);
        });
    });

    if (terminalDock && terminalShowAct)
    {
        connect(terminalDock, &QDockWidget::visibilityChanged,
                terminalShowAct, &QAction::setChecked);
    }

    if (snippetDock && snippetShowAct)
    {
        connect(snippetDock, &QDockWidget::visibilityChanged,
                snippetShowAct, &QAction::setChecked);
    }

    if (diagnosticsDock && diagnosticsAct)
    {
        connect(diagnosticsDock, &QDockWidget::visibilityChanged,
                diagnosticsAct, &QAction::setChecked);
    }

    if (debugDock && debugPanelAct)
    {
        connect(debugDock, &QDockWidget::visibilityChanged,
                debugPanelAct, &QAction::setChecked);
    }

    if (projectDock && projectManagerAct)
    {
        connect(projectDock, &QDockWidget::visibilityChanged,
                projectManagerAct, &QAction::setChecked);
    }

    if (fileBrowserDock && fileBrowserAct)
    {
        connect(fileBrowserDock, &QDockWidget::visibilityChanged,
                fileBrowserAct, &QAction::setChecked);
    }

    if (symbolBrowserDock && symbolBrowserAct)
    {
        connect(symbolBrowserDock, &QDockWidget::visibilityChanged,
                symbolBrowserAct, &QAction::setChecked);
    }

    if (outlineDock && outlineAct)
    {
        connect(outlineDock, &QDockWidget::visibilityChanged,
                outlineAct, &QAction::setChecked);
    }

    checkerDock = new QDockWidget(tr("对拍器"), this);
    checkerDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    checkerDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    checkerWidget = new CheckerWidget(this, checkerDock);
    checkerDock->setWidget(checkerWidget);
    checkerDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, checkerDock);

    aiDock = new QDockWidget(tr("DeepSeek AI 助手"), this);
    aiDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    aiDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    DeepSeekAssistantWidget *aiWidget = new DeepSeekAssistantWidget(aiDock);
    aiDock->setWidget(aiWidget);
    aiDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, aiDock);

    resizeDocks({aiDock}, {500}, Qt::Horizontal);

    connect(aiDock, &QDockWidget::visibilityChanged, this, [this](bool visible)
    {
        QTimer::singleShot(0, this, [this, visible]()
        {
            aiAct->setChecked(visible);
        });
    });

    luoguDock = new QDockWidget(tr("洛谷题目爬取器"), this);
    luoguDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    luoguDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    LuoguFetcherWidget *fetcher = new LuoguFetcherWidget(this, luoguDock);
    luoguDock->setWidget(fetcher);
    luoguDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, luoguDock);

    floatingTimeWarning = new QLabel(this);
    floatingTimeWarning->setAlignment(Qt::AlignCenter);
    floatingTimeWarning->setStyleSheet(
        "QLabel {"
        "    background-color: rgba(255, 50, 50, 200);"
        "    color: white;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border-radius: 8px;"
        "}"
        );
    floatingTimeWarning->setVisible(false);
    floatingTimeWarning->raise();

    warningFlashTimer = new QTimer(this);
    warningVisible = false;
    connect(warningFlashTimer, &QTimer::timeout, this, [this]()
    {
        warningVisible = !warningVisible;
        floatingTimeWarning->setVisible(warningVisible);
    });

    errorTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(errorTableWidget, &QTableWidget::customContextMenuRequested,
            this, &CompilerIDE::showErrorTableContextMenu);

    errorTableContextMenu = new QMenu(this);
    copyErrorAction = new QAction(tr("复制错误/警告信息"), this);
    connect(copyErrorAction, &QAction::triggered, this, &CompilerIDE::copySelectedError);
    errorTableContextMenu->addAction(copyErrorAction);

    QList<QDockWidget*> rightDocks =
    {
        projectDock,
        fileBrowserDock,
        symbolBrowserDock,
        outlineDock,
        diagnosticsDock,
        snippetDock,
        checkerDock,
        aiDock,
        luoguDock,
        sampleTesterDock
    };
    rightDocks.removeAll(nullptr);
    if (!rightDocks.isEmpty())
    {
        QList<int> sizes(rightDocks.size(), 400);
        resizeDocks(rightDocks, sizes, Qt::Horizontal);
    }

    defaultNormalTemplate = "#include <bits/stdc++.h>\n"
                            "using namespace std;\n"
                            "int main()\n"
                            "{\n"
                            "    \n"
                            "    return 0;\n"
                            "}";

    defaultEasyXTemplate = "#include <bits/stdc++.h>\n"
                           "#include <easyx.h>\n"
                           "#include <graphics.h>\n"
                           "int main()\n"
                           "{\n"
                           "    \n"
                           "    return 0;\n"
                           "}";

    loadTemplateSettings();

    // 应为左侧，但懒得改了
    rightSidebar = new SideBarContainer(this);
    rightSidebar->setDarkTheme(darkTheme);
    rightSidebar->hide();

    QDockWidget *sidebarDock = new QDockWidget(this);
    sidebarDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    sidebarDock->setTitleBarWidget(new QWidget());
    sidebarDock->setWidget(rightSidebar);
    addDockWidget(Qt::LeftDockWidgetArea, sidebarDock);
    resizeDocks({sidebarDock}, {400}, Qt::Horizontal);

    struct SidebarItem
    {
        QString name;
        QWidget *widget;
    };
    QList<SidebarItem> items =
    {
        { tr("项目管理器"), projectManager },
        { tr("文件浏览器"), fileBrowserDock->widget() },
        { tr("符号浏览器"), symbolBrowserDock->widget() },
        { tr("大纲视图"), outlineDock->widget() },
        { tr("诊断工具"), diagnosticsDock->widget() },
        { tr("代码片段"), snippetManager },
        { tr("对拍器"), checkerWidget },
        { tr("AI 助手"), aiDock->widget() },
        { tr("洛谷题目爬取器"), luoguDock->widget() },
        { tr("数据评测器"), sampleTesterWidget }
    };

    sidebarFeatureNames.clear();
    for (const auto &item : items)
    {
        if (!item.widget)
        {
            continue;
        }
        item.widget->setParent(rightSidebar);
        rightSidebar->addPage(item.name, item.widget);
        sidebarFeatureNames.append(item.name);
    }

    if (!sidebarFeatureNames.isEmpty())
    {
        rightSidebar->switchToFeature(sidebarFeatureNames.first());
    }

    connect(rightSidebar, &SideBarContainer::featureDisabledClicked,
            this, [this](const QString &name)
            {
                showFloatingMessage(tr("竞赛模式下已禁用“%1”功能").arg(name), 2000, true);
            });

    qDebug() << "=== 编译器路径调试信息 ===";
    qDebug() << "最终选择的编译器路径:" << compilerPath;
    qDebug() << "路径是否存在:" << QFile::exists(compilerPath);
    qDebug() << "应用程序目录:" << QCoreApplication::applicationDirPath();
    qDebug() << "=== 调试信息结束 ===";
}

CompilerIDE::~CompilerIDE()
{
    if (compileProcess)
    {
        compileProcess->kill();
        compileProcess->waitForFinished();
        delete compileProcess;
    }
    if (runProcess)
    {
        runProcess->kill();
        runProcess->waitForFinished();
        delete runProcess;
    }
    if (hRunningProcess != NULL)
    {
        TerminateProcess(hRunningProcess, 0);
        WaitForSingleObject(hRunningProcess, 1000);
        CloseHandle(hRunningProcess);
        hRunningProcess = NULL;
    }

    QApplication::processEvents(QEventLoop::AllEvents, 100);
    QThread::msleep(50);

    if (debugger->isRunning())
    {
        debugger->stopDebugging();
    }
    if (errorTableContextMenu)
    {
        delete errorTableContextMenu;
    }
    if (diagnosticsTimer)
    {
        diagnosticsTimer->stop();
        delete diagnosticsTimer;
    }

    cleanupCharts();

    delete debugger;
    delete codeCompleter;
    delete terminal;
    delete snippetManager;
}

void CompilerIDE::setTheme(bool dark)
{
    initTextFormats();

    if (dark)
    {
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        QApplication::setPalette(darkPalette);

        QPalette outputPalette = outputEdit->palette();
        outputPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        outputPalette.setColor(QPalette::Text, QColor(220, 220, 220));
        outputEdit->setPalette(outputPalette);

        if (errorTableWidget)
        {
            QPalette tablePalette = errorTableWidget->palette();
            tablePalette.setColor(QPalette::Base, QColor(25, 25, 25));
            tablePalette.setColor(QPalette::Text, QColor(220, 220, 220));
            tablePalette.setColor(QPalette::AlternateBase, QColor(35, 35, 35));
            errorTableWidget->setPalette(tablePalette);

            errorTableWidget->horizontalHeader()->setStyleSheet(
                "QHeaderView::section {"
                "    background-color: #2D3047;"
                "    color: #E0E0E0;"
                "    padding: 4px;"
                "    border: 1px solid #5A5F7A;"
                "}"
            );
        }

        if (debugOutput)
        {
            debugOutput->setStyleSheet(
                "QTextBrowser {"
                "    background-color: #1E1E1E;"
                "    color: #D4D4D4;"
                "    border: 1px solid #3C3C3C;"
                "}"
            );
        }

        if (breakpointsTable)
        {
            breakpointsTable->setStyleSheet(
                "QTableWidget {"
                "    background-color: #1E1E1E;"
                "    color: #D4D4D4;"
                "    gridline-color: #3C3C3C;"
                "}"
                "QTableWidget::item:selected {"
                "    background-color: #264F78;"
                "}"
                "QHeaderView::section {"
                "    background-color: #2D2D2D;"
                "    color: #D4D4D4;"
                "    padding: 4px;"
                "    border: 1px solid #3C3C3C;"
                "}"
            );
        }

        if (variablesTree)
        {
            variablesTree->setStyleSheet(
                "QTreeWidget {"
                "    background-color: #1E1E1E;"
                "    color: #D4D4D4;"
                "}"
                "QTreeWidget::item:selected {"
                "    background-color: #264F78;"
                "}"
                "QTreeWidget::item:hover {"
                "    background-color: #2A2D2E;"
                "}"
                "QHeaderView::section {"
                "    background-color: #2D2D2D;"
                "    color: #D4D4D4;"
                "    padding: 4px;"
                "    border: 1px solid #3C3C3C;"
                "}"
            );
        }

        if (callStackList)
        {
            callStackList->setStyleSheet(
                "QListWidget {"
                "    background-color: #1E1E1E;"
                "    color: #D4D4D4;"
                "    border: 1px solid #3C3C3C;"
                "}"
                "QListWidget::item:selected {"
                "    background-color: #264F78;"
                "}"
                "QListWidget::item:hover {"
                "    background-color: #2A2D2E;"
                "}"
            );
        }

        if (expressionInput)
        {
            expressionInput->setStyleSheet(
                "QLineEdit {"
                "    background-color: #1E1E1E;"
                "    color: #D4D4D4;"
                "    border: 1px solid #3C3C3C;"
                "    padding: 4px;"
                "}"
            );
        }

        if (watchList)
        {
            watchList->setStyleSheet(
                "QListWidget {"
                "    background-color: #1E1E1E;"
                "    color: #D4D4D4;"
                "    border: 1px solid #3C3C3C;"
                "}"
                "QListWidget::item:selected {"
                "    background-color: #264F78;"
                "}"
            );
        }

        if (cpuChart)
        {
            cpuChart->setTheme(QChart::ChartThemeDark);
        }
        if (memoryChart)
        {
            memoryChart->setTheme(QChart::ChartThemeDark);
        }

        if (terminal)
        {
            terminal->setTheme(true);
        }

        if (timerLabel)
        {
            timerLabel->setStyleSheet(
                "QLabel {"
                "    color: #E0E0E0;"
                "    font-size: 14pt;"
                "    font-weight: bold;"
                "    padding: 5px;"
                "}"
            );
        }

    }
    else
    {
        QApplication::setPalette(style()->standardPalette());

        QPalette outputPalette = outputEdit->palette();
        outputPalette.setColor(QPalette::Base, Qt::white);
        outputPalette.setColor(QPalette::Text, Qt::black);
        outputEdit->setPalette(outputPalette);

        if (errorTableWidget)
        {
            QPalette tablePalette = errorTableWidget->palette();
            tablePalette.setColor(QPalette::Base, Qt::white);
            tablePalette.setColor(QPalette::Text, Qt::black);
            tablePalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
            errorTableWidget->setPalette(tablePalette);

            errorTableWidget->horizontalHeader()->setStyleSheet(
                "QHeaderView::section {"
                "    background-color: #E0E0E0;"
                "    color: #333333;"
                "    padding: 4px;"
                "    border: 1px solid #CCCCCC;"
                "}"
            );
        }

        if (debugOutput)
        {
            debugOutput->setStyleSheet(
                "QTextBrowser {"
                "    background-color: white;"
                "    color: black;"
                "    border: 1px solid #CCCCCC;"
                "}"
            );
        }

        if (breakpointsTable)
        {
            breakpointsTable->setStyleSheet(
                "QTableWidget {"
                "    background-color: white;"
                "    color: black;"
                "    gridline-color: #CCCCCC;"
                "}"
                "QTableWidget::item:selected {"
                "    background-color: #0078D7;"
                "    color: white;"
                "}"
                "QHeaderView::section {"
                "    background-color: #F0F0F0;"
                "    color: black;"
                "    padding: 4px;"
                "    border: 1px solid #CCCCCC;"
                "}"
            );
        }

        if (variablesTree)
        {
            variablesTree->setStyleSheet(
                "QTreeWidget {"
                "    background-color: white;"
                "    color: black;"
                "}"
                "QTreeWidget::item:selected {"
                "    background-color: #0078D7;"
                "    color: white;"
                "}"
                "QTreeWidget::item:hover {"
                "    background-color: #E5F3FF;"
                "}"
                "QHeaderView::section {"
                "    background-color: #F0F0F0;"
                "    color: black;"
                "    padding: 4px;"
                "    border: 1px solid #CCCCCC;"
                "}"
            );
        }

        if (callStackList)
        {
            callStackList->setStyleSheet(
                "QListWidget {"
                "    background-color: white;"
                "    color: black;"
                "    border: 1px solid #CCCCCC;"
                "}"
                "QListWidget::item:selected {"
                "    background-color: #0078D7;"
                "    color: white;"
                "}"
                "QListWidget::item:hover {"
                "    background-color: #E5F3FF;"
                "}"
            );
        }

        if (expressionInput)
        {
            expressionInput->setStyleSheet(
                "QLineEdit {"
                "    background-color: white;"
                "    color: black;"
                "    border: 1px solid #CCCCCC;"
                "    padding: 4px;"
                "}"
            );
        }

        if (watchList)
        {
            watchList->setStyleSheet(
                "QListWidget {"
                "    background-color: white;"
                "    color: black;"
                "    border: 1px solid #CCCCCC;"
                "}"
                "QListWidget::item:selected {"
                "    background-color: #0078D7;"
                "    color: white;"
                "}"
            );
        }

        if (cpuChart)
        {
            cpuChart->setTheme(QChart::ChartThemeLight);
        }
        if (memoryChart)
        {
            memoryChart->setTheme(QChart::ChartThemeLight);
        }

        if (terminal)
        {
            terminal->setTheme(false);
        }

        if (timerLabel)
        {
            timerLabel->setStyleSheet(
                "QLabel {"
                "    color: #000000;"
                "    font-size: 14pt;"
                "    font-weight: bold;"
                "    padding: 5px;"
                "}"
            );
        }
    }

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setDarkThemeEnabled(dark);
        }
    }

    setProperty("darkTheme", dark);
    updateComboBoxStyles();
    applyGlobalMenuStyle();
    update();
}

void CompilerIDE::updateComboBoxStyles()
{
    QColor borderColor = palette().color(QPalette::Mid).darker(130);
    QString borderStyle = QString("border: 1px solid %1;").arg(borderColor.name());

    QString style = QString(R"(
        QComboBox QAbstractItemView
        {
            %1
            border-radius: 6px;
            background: palette(base);
            selection-background-color: palette(highlight);
            selection-color: palette(highlighted-text);
            outline: 0;
        }
    )").arg(borderStyle);

    if (cppStandardCombo)
    {
        cppStandardCombo->setStyleSheet(style);
    }
    if (optimizationCombo)
    {
        optimizationCombo->setStyleSheet(style);
    }
}

void CompilerIDE::applyGlobalMenuStyle()
{
    QColor borderColor = palette().color(QPalette::Mid).darker(130);
    QString borderStyle = QString("border: 1px solid %1;").arg(borderColor.name());

    QString menuStyle = QString(R"(
        QMenu
        {
            border-radius: 6px;
            %1
        }
    )").arg(borderStyle);

    qApp->setStyleSheet(qApp->styleSheet() + menuStyle);
}

void CompilerIDE::addToRecentFiles(const QString &filePath)
{
    if (filePath.isEmpty() || !QFile::exists(filePath))
    {
        return;
    }

    recentFiles.removeAll(filePath);

    recentFiles.prepend(filePath);

    while (recentFiles.size() > 10)
    {
        recentFiles.removeLast();
    }

    saveRecentFiles();
}

void CompilerIDE::loadRecentFiles()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6"); // 如果更新版本号，上个版本的设置内容会被覆盖，所以就不改版本号
    recentFiles = settings.value("recentFiles").toStringList();
}

void CompilerIDE::saveRecentFiles()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    settings.setValue("recentFiles", recentFiles);
}

QString CompilerIDE::detectCompilerType(const QString &compilerPath)
{
    if (compilerPath.isEmpty())
    {
        return "g++";
    }

    QFileInfo fileInfo(compilerPath);
    QString fileName = fileInfo.fileName().toLower();

    if (fileName.contains("g++"))
    {
        return "g++";
    }
    else if (fileName.contains("gcc") && !fileName.contains("g++"))
    {
        return "gcc";
    }
    else if (fileName.contains("cl") && !fileName.contains("clang"))
    {
        return "msvc";
    }
    else if (fileName.contains("clang"))
    {
        return "clang";
    }

    QProcess process;
    process.start(compilerPath, QStringList() << "--version");
    if (process.waitForFinished(3000) && process.exitCode() == 0)
    {
        QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
        if (output.contains("g++", Qt::CaseInsensitive))
        {
            return "g++";
        }
        else if (output.contains("gcc", Qt::CaseInsensitive) && output.contains("c++", Qt::CaseInsensitive))
        {
            return "g++";
        }
        else if (output.contains("gcc", Qt::CaseInsensitive))
        {
            return "gcc";
        }
        else if (output.contains("clang", Qt::CaseInsensitive))
        {
            return "clang";
        }
        else if (output.contains("microsoft", Qt::CaseInsensitive))
        {
            return "msvc";
        }
    }

    if (fileName.endsWith(".exe"))
    {
        if (fileName.contains("cl"))
        {
            return "msvc";
        }
        else
        {
            return "g++";
        }
    }

    return "g++";
}

bool CompilerIDE::testStandardSupport(const QString &standard)
{
    if (compilerPath.isEmpty())
    {
        return false;
    }

    QProcess process;

    QString testProgram = R"(
        int main() { return 0; }
    )";

    QTemporaryFile tempFile;
    if (tempFile.open())
    {
        tempFile.write(testProgram.toUtf8());
        tempFile.close();

        QStringList args;
        if (compilerType == "msvc")
        {
            if (standard == "C++14")
            {
                args << "/std:c++14";
            }
            else if (standard == "C++17")
            {
                args << "/std:c++17";
            }
            else if (standard == "C++20")
            {
                args << "/std:c++20";
            }
            else if (standard == "C++23")
            {
                args << "/std:c++latest";
            }
        }
        else
        {
            if (standard == "C++14")
            {
                args << "-std=c++14";
            }
            else if (standard == "C++17")
            {
                args << "-std=c++17";
            }
            else if (standard == "C++20")
            {
                args << "-std=c++20";
            }
            else if (standard == "C++23")
            {
                args << "-std=c++2b";
            }
        }

        args << "-c" << tempFile.fileName() << "-o" << (tempFile.fileName() + ".o");

        process.start(compilerPath, args);

        if (process.waitForFinished(5000))
        {
            return (process.exitCode() == 0);
        }
    }

    return false;
}

QStringList CompilerIDE::getSupportedStandards()
{
    QStringList supportedStandards;

    supportedStandards << "C++14";

    if (testStandardSupport("C++17"))
    {
        supportedStandards << "C++17";
    }

    if (testStandardSupport("C++20"))
    {
        supportedStandards << "C++20";
    }

    if (testStandardSupport("C++23"))
    {
        supportedStandards << "C++23";
    }

    qDebug() << "支持的C++标准:" << supportedStandards;
    return supportedStandards;
}

void CompilerIDE::checkCompilerSupport()
{
    if (!compilerPath.isEmpty() && QFile::exists(compilerPath))
    {
        qDebug() << "使用已经设置的编译器路径，跳过自动检测:" << compilerPath;

        QStringList supportedStandards = getSupportedStandards();

        QString currentStandard = cppStandardCombo->currentText();

        cppStandardCombo->clear();
        cppStandardCombo->addItems(supportedStandards);

        int index = cppStandardCombo->findText(currentStandard);
        if (index >= 0)
        {
            cppStandardCombo->setCurrentIndex(index);
        }
        else if (!supportedStandards.isEmpty())
        {
            cppStandardCombo->setCurrentIndex(supportedStandards.size() - 1);
        }

        cppStandard = cppStandardCombo->currentText().toLower();
        return;
    }

    if (compilerPath.isEmpty())
    {
        cppStandardCombo->clear();
        cppStandardCombo->addItems({"C++14", "C++17", "C++20", "C++23"});
        cppStandardCombo->setCurrentText("C++17");
        return;
    }

    QStringList supportedStandards = getSupportedStandards();
    QString currentStandard = cppStandardCombo->currentText();

    cppStandardCombo->clear();
    cppStandardCombo->addItems(supportedStandards);

    int index = cppStandardCombo->findText(currentStandard);
    if (index >= 0)
    {
        cppStandardCombo->setCurrentIndex(index);
    }
    else if (!supportedStandards.isEmpty())
    {
        cppStandardCombo->setCurrentIndex(supportedStandards.size() - 1);
    }

    cppStandard = cppStandardCombo->currentText().toLower();

    qDebug() << "选择的C++标准:" << cppStandard;
}

void CompilerIDE::closeEvent(QCloseEvent *event)
{
    if (isRestarting)
    {
        event->accept();
        return;
    }

    if (!maybeSaveAll())
    {
        event->ignore();
        return;
    }

    writeSettings();
    event->accept();
}

void CompilerIDE::createDiagnosticsDock()
{
    diagnosticsDock = new QDockWidget(tr("诊断工具（可能有较小误差）"), this);
    diagnosticsDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    diagnosticsDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    diagnosticsWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(diagnosticsWidget);

    QGroupBox *systemGroup = new QGroupBox(tr("系统信息"));
    QFormLayout *systemLayout = new QFormLayout(systemGroup);

    cpuUsageLabel = new QLabel("0%");
    memoryUsageLabel = new QLabel("0%");
    systemUptimeLabel = new QLabel("00:00:00");
    processMemoryLabel = new QLabel("0 MB");
    processThreadsLabel = new QLabel("0");

    systemLayout->addRow(tr("CPU使用率:"), cpuUsageLabel);
    systemLayout->addRow(tr("内存使用率:"), memoryUsageLabel);
    systemLayout->addRow(tr("系统运行时间:"), systemUptimeLabel);
    systemLayout->addRow(tr("进程内存:"), processMemoryLabel);
    systemLayout->addRow(tr("进程线程数:"), processThreadsLabel);

    QGroupBox *chartsGroup = new QGroupBox(tr("性能图表"));
    QVBoxLayout *chartsLayout = new QVBoxLayout(chartsGroup);

    cpuChart = new QChart();
    cpuChart->setTitle(tr("CPU使用率"));
    cpuChart->setAnimationOptions(QChart::NoAnimation);

    cpuSeries = new QLineSeries();
    cpuSeries->setName(tr("CPU使用率"));

    cpuChart->addSeries(cpuSeries);

    cpuAxisY = new QValueAxis();
    cpuAxisY->setRange(0, 100);
    cpuAxisY->setTitleText(tr("使用率 (%)"));
    cpuChart->addAxis(cpuAxisY, Qt::AlignLeft);
    cpuSeries->attachAxis(cpuAxisY);

    cpuAxisX = new QValueAxis();
    cpuAxisX->setRange(0, maxHistoryPoints);
    cpuAxisX->setTitleText(tr("时间"));
    cpuAxisX->setTickCount(6);
    cpuAxisX->setLabelFormat("%d");
    cpuChart->addAxis(cpuAxisX, Qt::AlignBottom);
    cpuSeries->attachAxis(cpuAxisX);

    cpuChartView = new QChartView(cpuChart);
    cpuChartView->setRenderHint(QPainter::Antialiasing);
    cpuChartView->setMinimumHeight(200);

    memoryChart = new QChart();
    memoryChart->setTitle(tr("内存使用率"));
    memoryChart->setAnimationOptions(QChart::NoAnimation);

    memorySeries = new QLineSeries();
    memorySeries->setName(tr("内存使用率"));

    memoryChart->addSeries(memorySeries);

    memoryAxisY = new QValueAxis();
    memoryAxisY->setRange(0, 100);
    memoryAxisY->setTitleText(tr("使用率 (%)"));
    memoryChart->addAxis(memoryAxisY, Qt::AlignLeft);
    memorySeries->attachAxis(memoryAxisY);

    memoryAxisX = new QValueAxis();
    memoryAxisX->setRange(0, maxHistoryPoints);
    memoryAxisX->setTitleText(tr("时间"));
    memoryAxisX->setTickCount(6);
    memoryAxisX->setLabelFormat("%d");
    memoryChart->addAxis(memoryAxisX, Qt::AlignBottom);
    memorySeries->attachAxis(memoryAxisX);

    memoryChartView = new QChartView(memoryChart);
    memoryChartView->setRenderHint(QPainter::Antialiasing);
    memoryChartView->setMinimumHeight(200);

    chartsLayout->addWidget(cpuChartView);
    chartsLayout->addWidget(memoryChartView);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    QPushButton *refreshButton = new QPushButton(tr("立即刷新"));
    QPushButton *clearButton = new QPushButton(tr("清除图表"));

    controlLayout->addWidget(refreshButton);
    controlLayout->addWidget(clearButton);
    controlLayout->addStretch();

    mainLayout->addWidget(systemGroup);
    mainLayout->addWidget(chartsGroup);
    mainLayout->addLayout(controlLayout);

    scrollArea->setWidget(diagnosticsWidget);
    diagnosticsDock->setWidget(scrollArea);
    diagnosticsDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, diagnosticsDock);

    connect(refreshButton, &QPushButton::clicked, this, &CompilerIDE::updateSystemInfo);
    connect(clearButton, &QPushButton::clicked, this, [this]()
    {
        cpuHistory.clear();
        memoryHistory.clear();
        cpuSeries->clear();
        memorySeries->clear();
    });

    diagnosticsTimer = new QTimer(this);
    connect(diagnosticsTimer, &QTimer::timeout, this, &CompilerIDE::updateSystemInfo);
    diagnosticsTimer->start(2000);

    initialUptime = QDateTime::currentSecsSinceEpoch();
    updateSystemInfo();
}

void CompilerIDE::updateSystemInfo()
{
    double cpuUsage = getCPUUsage();
    double memoryUsage = getMemoryUsage();
    double processMemory = getProcessMemoryUsage();
    int threadCount = getProcessThreadCount();

    cpuUsageLabel->setText(QString("%1%").arg(cpuUsage, 0, 'f', 1));
    memoryUsageLabel->setText(QString("%1%").arg(memoryUsage, 0, 'f', 1));
    processMemoryLabel->setText(QString("%1 MB").arg(processMemory, 0, 'f', 1));
    processThreadsLabel->setText(QString::number(threadCount));

    qint64 uptime = QDateTime::currentSecsSinceEpoch() - initialUptime;
    systemUptimeLabel->setText(formatUptime(uptime));

    QDateTime currentTime = QDateTime::currentDateTime();
    qint64 timeKey = currentTime.toMSecsSinceEpoch();

    cpuHistory.append(QPointF(cpuHistory.size(), cpuUsage));
    memoryHistory.append(QPointF(memoryHistory.size(), memoryUsage));

    if (cpuHistory.size() > maxHistoryPoints)
    {
        cpuHistory.removeFirst();
        memoryHistory.removeFirst();

        for (int i = 0; i < cpuHistory.size(); ++i)
        {
            cpuHistory[i].setX(i);
            memoryHistory[i].setX(i);
        }
    }

    updateCharts();
}

void CompilerIDE::setDebugState(DebugState state)
{
    currentDebugState = state;
    updateDebugUI();
}

double CompilerIDE::getCPUUsage()
{
    static ULARGE_INTEGER prevIdleTime = {0};
    static ULARGE_INTEGER prevKernelTime = {0};
    static ULARGE_INTEGER prevUserTime = {0};
    static bool firstCall = true;

    FILETIME idleTime, kernelTime, userTime;

    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
    {
        return 0.0;
    }

    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;
    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;
    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    if (firstCall)
    {
        prevIdleTime = idle;
        prevKernelTime = kernel;
        prevUserTime = user;
        firstCall = false;

        static bool waiting = true;
        if (waiting)
        {
            waiting = false;
            return 0.0;
        }
    }

    ULONGLONG idleDiff = idle.QuadPart - prevIdleTime.QuadPart;
    ULONGLONG kernelDiff = kernel.QuadPart - prevKernelTime.QuadPart;
    ULONGLONG userDiff = user.QuadPart - prevUserTime.QuadPart;

    prevIdleTime = idle;
    prevKernelTime = kernel;
    prevUserTime = user;

    ULONGLONG totalTime = kernelDiff + userDiff;

    if (totalTime == 0)
    {
        return 0.0;
    }

    double usage = 100.0 * (totalTime - idleDiff) / totalTime;

    static int debugCounter = 0;
    if (debugCounter++ % 10 == 0)
    {
        qDebug() << "CPU Calculation - Idle:" << idleDiff
                 << "Kernel:" << kernelDiff
                 << "User:" << userDiff
                 << "Total:" << totalTime
                 << "Usage:" << usage << "%";
    }

    if (usage < 0.0)
    {
        usage = 0.0;
    }
    if (usage > 100.0)
    {
        usage = 100.0;
    }

    return usage;
}

double CompilerIDE::getMemoryUsage()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo))
    {
        DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
        DWORDLONG physMemUsed = totalPhysMem - memInfo.ullAvailPhys;

        return (physMemUsed * 100.0) / totalPhysMem;
    }

    return 0.0;
}

double CompilerIDE::getProcessMemoryUsage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(),
                             reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc),
                             sizeof(pmc)))
    {
        return pmc.WorkingSetSize / (1024.0 * 1024.0);
    }

    return 0.0;
}

int CompilerIDE::getProcessThreadCount()
{
    DWORD processId = GetCurrentProcessId();
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    THREADENTRY32 te32;
    te32.dwSize = sizeof(THREADENTRY32);

    int threadCount = 0;
    if (Thread32First(hSnapshot, &te32))
    {
        do
        {
            if (te32.th32OwnerProcessID == processId)
            {
                threadCount++;
            }
        }
        while (Thread32Next(hSnapshot, &te32));
    }

    CloseHandle(hSnapshot);
    return threadCount;
}

QString CompilerIDE::formatUptime(qint64 seconds)
{
    qint64 hours = seconds / 3600;
    qint64 minutes = (seconds % 3600) / 60;
    qint64 secs = seconds % 60;

    return QString("%1:%2:%3")
           .arg(hours, 2, 10, QLatin1Char('0'))
           .arg(minutes, 2, 10, QLatin1Char('0'))
           .arg(secs, 2, 10, QLatin1Char('0'));
}

void CompilerIDE::updateCharts()
{
    cpuSeries->clear();
    for (const QPointF &point : cpuHistory)
    {
        cpuSeries->append(point);
    }

    memorySeries->clear();
    for (const QPointF &point : memoryHistory)
    {
        memorySeries->append(point);
    }

    QValueAxis *cpuAxisX = qobject_cast<QValueAxis*>(cpuChart->axes(Qt::Horizontal).first());
    QValueAxis *memoryAxisX = qobject_cast<QValueAxis*>(memoryChart->axes(Qt::Horizontal).first());

    if (cpuAxisX && memoryAxisX)
    {
        int currentMax = qMax(1, cpuHistory.size());
        cpuAxisX->setRange(0, currentMax);
        memoryAxisX->setRange(0, currentMax);
    }
}

void CompilerIDE::cleanupCharts()
{
    if (cpuChart)
    {
        delete cpuChart;
        cpuChart = nullptr;
    }
    if (memoryChart)
    {
        delete memoryChart;
        memoryChart = nullptr;
    }
}

void CompilerIDE::createDebugDock()
{
    if (debugDock)
    {
        delete debugDock;
    }

    debugDock = new QDockWidget(tr("调试"), this);
    debugDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    debugDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

    debugWidget = new QWidget(debugDock);
    QVBoxLayout *mainLayout = new QVBoxLayout(debugWidget);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(2);

    QToolBar *debugToolbar = new QToolBar(debugWidget);
    debugToolbar->setIconSize(QSize(24, 24));
    debugToolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    debugStartAct = new QAction(tr("开始调试"), this);
    debugStopAct = new QAction(tr("停止调试"), this);
    debugContinueAct = new QAction(tr("继续执行"), this);
    debugStepOverAct = new QAction(tr("单步跳过"), this);
    debugStepIntoAct = new QAction(tr("单步进入"), this);
    debugStepOutAct = new QAction(tr("单步跳出"), this);
    debugToggleBreakpointAct = new QAction(tr("切换断点"), this);
    debugRunToCursorAct = new QAction(tr("运行到光标"), this);

    debugStartAct->setToolTip(tr("开始调试 (F5)"));
    debugStopAct->setToolTip(tr("停止调试 (Shift+F5)"));
    debugContinueAct->setToolTip(tr("继续执行 (F5)"));
    debugStepOverAct->setToolTip(tr("单步跳过 (F10)"));
    debugStepIntoAct->setToolTip(tr("单步进入 (F11)"));
    debugStepOutAct->setToolTip(tr("单步跳出 (Shift+F11)"));
    debugToggleBreakpointAct->setToolTip(tr("切换断点 (F9)"));
    debugRunToCursorAct->setToolTip(tr("运行到光标"));

    debugStartAct->setShortcut(QKeySequence(Qt::Key_F5));
    debugStopAct->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F5));
    debugStepOverAct->setShortcut(QKeySequence(Qt::Key_F10));
    debugStepIntoAct->setShortcut(QKeySequence(Qt::Key_F11));
    debugStepOutAct->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F11));
    debugToggleBreakpointAct->setShortcut(QKeySequence(Qt::Key_F9));

    connect(debugStartAct, &QAction::triggered, this, [this]()
    {
        startDebugging();
    });
    connect(debugStopAct, &QAction::triggered, this, &CompilerIDE::stopDebugging);
    connect(debugContinueAct, &QAction::triggered, this, &CompilerIDE::continueDebugging);
    connect(debugStepOverAct, &QAction::triggered, this, &CompilerIDE::stepOver);
    connect(debugStepIntoAct, &QAction::triggered, this, &CompilerIDE::stepInto);
    connect(debugStepOutAct, &QAction::triggered, this, &CompilerIDE::stepOut);
    connect(debugToggleBreakpointAct, &QAction::triggered, this, &CompilerIDE::toggleBreakpoint);
    connect(debugRunToCursorAct, &QAction::triggered, this, &CompilerIDE::runToCursor);

    debugToolbar->addAction(debugStartAct);
    debugToolbar->addAction(debugStopAct);
    debugToolbar->addSeparator();
    debugToolbar->addAction(debugContinueAct);
    debugToolbar->addAction(debugStepOverAct);
    debugToolbar->addAction(debugStepIntoAct);
    debugToolbar->addAction(debugStepOutAct);
    debugToolbar->addSeparator();
    debugToolbar->addAction(debugToggleBreakpointAct);
    debugToolbar->addAction(debugRunToCursorAct);

    mainLayout->addWidget(debugToolbar);

    debugTabs = new QTabWidget(debugWidget);

    QWidget *outputTab = new QWidget();
    QVBoxLayout *outputLayout = new QVBoxLayout(outputTab);
    outputLayout->setContentsMargins(2, 2, 2, 2);

    debugOutput = new QTextBrowser();
    debugOutput->setFont(QFont("Consolas", 10));
    debugOutput->setOpenExternalLinks(false);
    debugOutput->setStyleSheet(
        "QTextBrowser {"
        "    background-color: #1E1E1E;"
        "    color: #D4D4D4;"
        "    border: 1px solid #3C3C3C;"
        "}"
    );
    outputLayout->addWidget(debugOutput);

    QWidget *breakpointsTab = new QWidget();
    QVBoxLayout *breakpointsLayout = new QVBoxLayout(breakpointsTab);
    breakpointsLayout->setContentsMargins(2, 2, 2, 2);

    breakpointsTable = new QTableWidget();
    breakpointsTable->setColumnCount(3);
    breakpointsTable->setHorizontalHeaderLabels(QStringList() << tr("文件") << tr("行号") << tr("条件"));
    breakpointsTable->horizontalHeader()->setStretchLastSection(true);
    breakpointsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    breakpointsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    breakpointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    breakpointsTable->setAlternatingRowColors(true);
    breakpointsTable->verticalHeader()->setVisible(false);
    breakpointsTable->setStyleSheet(
        "QTableWidget {"
        "    background-color: #1E1E1E;"
        "    color: #D4D4D4;"
        "    gridline-color: #3C3C3C;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #264F78;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2D2D2D;"
        "    color: #D4D4D4;"
        "    padding: 4px;"
        "    border: 1px solid #3C3C3C;"
        "}"
    );

    QHBoxLayout *bpButtonLayout = new QHBoxLayout();
    QPushButton *addCondBpBtn = new QPushButton(tr("添加条件断点"));
    QPushButton *editBpBtn = new QPushButton(tr("编辑条件"));
    QPushButton *clearBpBtn = new QPushButton(tr("清除所有"));
    bpButtonLayout->addWidget(addCondBpBtn);
    bpButtonLayout->addWidget(editBpBtn);
    bpButtonLayout->addWidget(clearBpBtn);
    bpButtonLayout->addStretch();

    breakpointsLayout->addWidget(breakpointsTable);
    breakpointsLayout->addLayout(bpButtonLayout);

    connect(addCondBpBtn, &QPushButton::clicked, this, &CompilerIDE::addConditionalBreakpoint);
    connect(editBpBtn, &QPushButton::clicked, this, &CompilerIDE::editBreakpointCondition);
    connect(clearBpBtn, &QPushButton::clicked, this, &CompilerIDE::removeAllBreakpoints);
    connect(breakpointsTable, &QTableWidget::itemDoubleClicked,
            this, &CompilerIDE::onBreakpointTableItemDoubleClicked);

    QWidget *variablesTab = new QWidget();
    QVBoxLayout *variablesLayout = new QVBoxLayout(variablesTab);
    variablesLayout->setContentsMargins(2, 2, 2, 2);

    variablesTree = new QTreeWidget();
    variablesTree->setHeaderLabels(QStringList() << tr("名称") << tr("值") << tr("类型"));
    variablesTree->setColumnCount(3);
    variablesTree->setAlternatingRowColors(true);
    variablesTree->setRootIsDecorated(true);
    variablesTree->header()->setStretchLastSection(true);
    variablesTree->setStyleSheet(
        "QTreeWidget {"
        "    background-color: #1E1E1E;"
        "    color: #D4D4D4;"
        "}"
        "QTreeWidget::item:selected {"
        "    background-color: #264F78;"
        "}"
        "QTreeWidget::item:hover {"
        "    background-color: #2A2D2E;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2D2D2D;"
        "    color: #D4D4D4;"
        "    padding: 4px;"
        "    border: 1px solid #3C3C3C;"
        "}"
    );

    variablesTree->setColumnWidth(0, 150);
    variablesTree->setColumnWidth(1, 200);
    variablesTree->setColumnWidth(2, 100);

    variablesLayout->addWidget(variablesTree);

    connect(variablesTree, &QTreeWidget::itemDoubleClicked,
            this, &CompilerIDE::onVariablesTreeItemDoubleClicked);

    QWidget *callStackTab = new QWidget();
    QVBoxLayout *callStackLayout = new QVBoxLayout(callStackTab);
    callStackLayout->setContentsMargins(2, 2, 2, 2);

    callStackList = new QListWidget();
    callStackList->setAlternatingRowColors(true);
    callStackList->setStyleSheet(
        "QListWidget {"
        "    background-color: #1E1E1E;"
        "    color: #D4D4D4;"
        "    border: 1px solid #3C3C3C;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #264F78;"
        "}"
        "QListWidget::item:hover {"
        "    background-color: #2A2D2E;"
        "}"
    );

    callStackLayout->addWidget(callStackList);

    connect(callStackList, &QListWidget::itemDoubleClicked,
            this, &CompilerIDE::onCallStackItemDoubleClicked);

    QWidget *watchTab = new QWidget();
    QVBoxLayout *watchLayout = new QVBoxLayout(watchTab);
    watchLayout->setContentsMargins(2, 2, 2, 2);

    QHBoxLayout *exprLayout = new QHBoxLayout();
    expressionInput = new QLineEdit();
    expressionInput->setPlaceholderText(tr("输入要监视的表达式..."));
    expressionInput->setStyleSheet(
        "QLineEdit {"
        "    background-color: #1E1E1E;"
        "    color: #D4D4D4;"
        "    border: 1px solid #3C3C3C;"
        "    padding: 4px;"
        "}"
    );

    evalButton = new QPushButton(tr("求值"));
    QPushButton *addWatchBtn = new QPushButton(tr("添加监视"));

    exprLayout->addWidget(expressionInput);
    exprLayout->addWidget(evalButton);
    exprLayout->addWidget(addWatchBtn);

    watchList = new QListWidget();
    watchList->setAlternatingRowColors(true);
    watchList->setStyleSheet(
        "QListWidget {"
        "    background-color: #1E1E1E;"
        "    color: #D4D4D4;"
        "    border: 1px solid #3C3C3C;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #264F78;"
        "}"
    );

    QHBoxLayout *watchBtnLayout = new QHBoxLayout();
    QPushButton *removeWatchBtn = new QPushButton(tr("移除"));
    QPushButton *refreshWatchBtn = new QPushButton(tr("刷新全部"));
    watchBtnLayout->addWidget(removeWatchBtn);
    watchBtnLayout->addWidget(refreshWatchBtn);
    watchBtnLayout->addStretch();

    watchLayout->addLayout(exprLayout);
    watchLayout->addWidget(watchList);
    watchLayout->addLayout(watchBtnLayout);

    connect(evalButton, &QPushButton::clicked, this, &CompilerIDE::evaluateExpression);
    connect(expressionInput, &QLineEdit::returnPressed, this, &CompilerIDE::evaluateExpression);
    connect(addWatchBtn, &QPushButton::clicked, this, &CompilerIDE::addWatchExpression);
    connect(removeWatchBtn, &QPushButton::clicked, this, &CompilerIDE::removeWatchExpression);
    connect(refreshWatchBtn, &QPushButton::clicked, this, &CompilerIDE::evaluateAllWatches);

    debugTabs->addTab(outputTab, tr("输出"));
    debugTabs->addTab(breakpointsTab, tr("断点"));
    debugTabs->addTab(variablesTab, tr("变量"));
    debugTabs->addTab(callStackTab, tr("调用栈"));
    debugTabs->addTab(watchTab, tr("监视"));

    mainLayout->addWidget(debugTabs);

    debugWidget->setLayout(mainLayout);
    debugDock->setWidget(debugWidget);

    updateDebugControls();

    addDockWidget(Qt::BottomDockWidgetArea, debugDock);
    debugDock->setVisible(false);

    bool isDark = property("darkTheme").toBool();

    if (isDark)
    {
        if (debugOutput)
        {
            debugOutput->setStyleSheet(
                "QTextBrowser { background-color: #1E1E1E; color: #D4D4D4; border: 1px solid #3C3C3C; }"
            );
        }
        if (breakpointsTable)
        {
            breakpointsTable->setStyleSheet(
                "QTableWidget { background-color: #1E1E1E; color: #D4D4D4; gridline-color: #3C3C3C; }"
                "QTableWidget::item:selected { background-color: #264F78; }"
                "QHeaderView::section { background-color: #2D2D2D; color: #D4D4D4; padding: 4px; border: 1px solid #3C3C3C; }"
            );
        }
        if (variablesTree)
        {
            variablesTree->setStyleSheet(
                "QTreeWidget { background-color: #1E1E1E; color: #D4D4D4; }"
                "QTreeWidget::item:selected { background-color: #264F78; }"
                "QTreeWidget::item:hover { background-color: #2A2D2E; }"
                "QHeaderView::section { background-color: #2D2D2D; color: #D4D4D4; padding: 4px; border: 1px solid #3C3C3C; }"
            );
        }
        if (callStackList)
        {
            callStackList->setStyleSheet(
                "QListWidget { background-color: #1E1E1E; color: #D4D4D4; border: 1px solid #3C3C3C; }"
                "QListWidget::item:selected { background-color: #264F78; }"
                "QListWidget::item:hover { background-color: #2A2D2E; }"
            );
        }
        if (expressionInput)
        {
            expressionInput->setStyleSheet(
                "QLineEdit { background-color: #1E1E1E; color: #D4D4D4; border: 1px solid #3C3C3C; padding: 4px; }"
            );
        }
        if (watchList)
        {
            watchList->setStyleSheet(
                "QListWidget { background-color: #1E1E1E; color: #D4D4D4; border: 1px solid #3C3C3C; }"
                "QListWidget::item:selected { background-color: #264F78; }"
            );
        }
    }
    else
    {
        if (debugOutput)
        {
            debugOutput->setStyleSheet(
                "QTextBrowser { background-color: white; color: black; border: 1px solid #CCCCCC; }"
            );
        }
        if (breakpointsTable)
        {
            breakpointsTable->setStyleSheet(
                "QTableWidget { background-color: white; color: black; gridline-color: #CCCCCC; }"
                "QTableWidget::item:selected { background-color: #0078D7; color: white; }"
                "QHeaderView::section { background-color: #F0F0F0; color: black; padding: 4px; border: 1px solid #CCCCCC; }"
            );
        }
        if (variablesTree)
        {
            variablesTree->setStyleSheet(
                "QTreeWidget { background-color: white; color: black; }"
                "QTreeWidget::item:selected { background-color: #0078D7; color: white; }"
                "QTreeWidget::item:hover { background-color: #E5F3FF; }"
                "QHeaderView::section { background-color: #F0F0F0; color: black; padding: 4px; border: 1px solid #CCCCCC; }"
            );
        }
        if (callStackList)
        {
            callStackList->setStyleSheet(
                "QListWidget { background-color: white; color: black; border: 1px solid #CCCCCC; }"
                "QListWidget::item:selected { background-color: #0078D7; color: white; }"
                "QListWidget::item:hover { background-color: #E5F3FF; }"
            );
        }
        if (expressionInput)
        {
            expressionInput->setStyleSheet(
                "QLineEdit { background-color: white; color: black; border: 1px solid #CCCCCC; padding: 4px; }"
            );
        }
        if (watchList)
        {
            watchList->setStyleSheet(
                "QListWidget { background-color: white; color: black; border: 1px solid #CCCCCC; }"
                "QListWidget::item:selected { background-color: #0078D7; color: white; }"
            );
        }
    }
}

void CompilerIDE::tempCompile()
{
    if (m_isTempCompiling)
    {
        QMessageBox::warning(this, tr("提示"), tr("已有临时编译任务正在运行，请稍后..."));
        return;
    }

    if (compileProcess && compileProcess->state() == QProcess::Running)
    {
        QMessageBox::warning(this, tr("提示"), tr("已有编译任务正在运行，请稍后..."));
        return;
    }

    if (outputDock)
    {
        outputDock->raise();
    }

    if (m_tempCompileProcess)
    {
        m_tempCompileProcess->disconnect();
        if (m_tempCompileProcess->state() == QProcess::Running)
        {
            m_tempCompileProcess->kill();
            QTimer::singleShot(500, this, [this]()
            {
                if (m_tempCompileProcess && m_tempCompileProcess->state() != QProcess::NotRunning)
                {
                    m_tempCompileProcess->kill();
                }
            });
        }
        m_tempCompileProcess->deleteLater();
        m_tempCompileProcess = nullptr;
    }

    m_isTempCompiling = true;

    resetOutputFormat();
    outputEdit->clear();
    if (errorTableWidget)
    {
        errorTableWidget->setRowCount(0);
    }

    QTextCursor cursor = outputEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.setCharFormat(defaultFormat);
    outputEdit->setTextCursor(cursor);

    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        QMessageBox::warning(this, tr("临时编译"), tr("没有打开的编辑器"));
        m_isTempCompiling = false;
        return;
    }

    editor->clearErrorWarningLines();

    QString tempDir = QDir::temp().absolutePath() + "/CompilerIDE_TempBuild";
    QDir().mkpath(tempDir);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    tempCompileSourceFile = tempDir + "/temp_" + timestamp + ".cpp";
    tempCompileExeFile = tempDir + "/temp_" + timestamp + ".exe";

    QFile tempFile(tempCompileSourceFile);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("临时编译"),
                             tr("无法创建临时文件: %1").arg(tempFile.errorString()));
        m_isTempCompiling = false;
        return;
    }

    QTextStream out(&tempFile);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#endif
    out << editor->toPlainText();
    tempFile.close();

    isNewCompilation = true;
    accumulatedErrors.clear();
    accumulatedWarnings.clear();

    outputEdit->clear();
    outputEdit->appendPlainText(tr("=== 临时编译 ==="));
    outputEdit->appendPlainText("");
    outputEdit->appendPlainText(tr("临时源文件: %1").arg(tempCompileSourceFile));
    outputEdit->appendPlainText("");
    outputEdit->appendPlainText(tr("编译器: %1").arg(compilerPath));
    outputEdit->appendPlainText("");

    if (!QFile::exists(compilerPath))
    {
        QString errorMsg = tr("编译器路径不存在: %1").arg(compilerPath);
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        m_isTempCompiling = false;
        return;
    }

    QStringList args;
    bool isEasyX = editorEasyXFlags.value(editor, false);
    QString actualCompilerPath = compilerPath;

    if (compilerType == "msvc")
    {
        if (cppStandard == "c++14")
        {
            args << "/std:c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "/std:c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "/std:c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "/std:c++latest";
        }
        args << "/EHsc" << "/W4" << "/nologo" << "/utf-8";
        args << "/Fe" + tempCompileExeFile;
        args << tempCompileSourceFile;
    }
    else
    {
        if (cppStandard == "c++14")
        {
            args << "-std=c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "-std=c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "-std=c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "-std=c++2b";
        }
        args << "-g";
        args << optimizationLevel;;
        args << "-Wall" << "-Wextra";
        args << "-finput-charset=UTF-8" << "-fexec-charset=UTF-8";
        args << "-static";
        args << "-static-libgcc" << "-static-libstdc++";

        if (isEasyX)
        {
            QString appDir = QCoreApplication::applicationDirPath();
            QString bundledTDMGCC = appDir + "/TDM-GCC/bin/g++.exe";

            QString compilerName = "配置的编译器";

            if (QFile::exists(bundledTDMGCC))
            {
                actualCompilerPath = bundledTDMGCC;
                compilerName = "内置 TDM-GCC";
                outputEdit->appendPlainText(tr("  使用内置 TDM-GCC 编译器"));
                outputEdit->appendPlainText(tr("  路径: %1").arg(bundledTDMGCC));
            }
            else
            {
                outputEdit->appendPlainText(tr("  使用配置的编译器"));
                outputEdit->appendPlainText(tr("  路径: %1").arg(actualCompilerPath));
            }

            QFileInfo compilerInfo(actualCompilerPath);
            QString compilerDir = compilerInfo.absolutePath();
            QDir baseDir(compilerDir);
            baseDir.cdUp();

            QString mingwLib = baseDir.absolutePath() + "/x86_64-w64-mingw32/lib";
            QString mingwInclude = baseDir.absolutePath() + "/x86_64-w64-mingw32/include";
            QString easyxLib = mingwLib + "/libgraphics.a";

            outputEdit->appendPlainText(tr("  EasyX 图形模式"));
            outputEdit->appendPlainText(tr("  库路径: %1").arg(mingwLib));
            outputEdit->appendPlainText(tr("  头文件路径: %1").arg(mingwInclude));
            outputEdit->appendPlainText("");

            if (!QFile::exists(easyxLib))
            {
                easyxLib = mingwLib + "/libeasyxw.a";
                if (!QFile::exists(easyxLib))
                {
                    easyxLib = mingwLib + "/libeasyx.a";
                    if (!QFile::exists(easyxLib))
                    {
                        QString errorMsg = tr("错误: 找不到 EasyX 库文件\n"
                                              "请确保 %1 编译器包含 EasyX 库").arg(compilerName);
                        outputEdit->appendPlainText(errorMsg);
                        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
                        updateErrorTable(accumulatedErrors, accumulatedWarnings);
                        m_isTempCompiling = false;
                        return;
                    }
                }
            }

            outputEdit->appendPlainText(tr("找到 EasyX 库: %1").arg(QFileInfo(easyxLib).fileName()));
            outputEdit->appendPlainText("");

            args << "-L" + QDir::toNativeSeparators(mingwLib);
            args << "-I" + QDir::toNativeSeparators(mingwInclude);

            args << "-DUNICODE" << "-D_UNICODE";

            args << "-static-libgcc" << "-static-libstdc++";
        }

        args << "-o" << tempCompileExeFile;
        args << tempCompileSourceFile;

        if (isEasyX)
        {
            args << "-leasyxw"
                 << "-lgdi32"
                 << "-limm32"
                 << "-lmsimg32"
                 << "-lole32"
                 << "-loleaut32"
                 << "-lwinmm"
                 << "-luuid"
                 << "-lgdiplus";
        }
    }

    if (compileProcess)
    {
        compileProcess->kill();
        compileProcess->waitForFinished();
        delete compileProcess;
    }

    compileProcess = new QProcess(this);
    compileProcess->setProcessChannelMode(QProcess::MergedChannels);

    QString appDir = QCoreApplication::applicationDirPath();
    QString mingwBin = appDir + "/mingw/bin";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString currentPath = env.value("PATH");
    env.insert("PATH", mingwBin + ";" + currentPath);
#ifdef Q_OS_WIN
    env.insert("PYTHONIOENCODING", "utf-8");
    env.insert("UTF-8", "1");
#endif
    compileProcess->setProcessEnvironment(env);

    connect(compileProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus)
    {
        if (exitCode == 0)
        {
            outputEdit->appendPlainText(tr("=== 临时编译成功 ==="));
            statusBar()->showMessage(tr("临时编译成功"), 3000);
        }
        else
        {
            outputEdit->appendPlainText(tr("=== 临时编译失败 ==="));
            statusBar()->showMessage(tr("临时编译失败"), 3000);
            errorDock->setVisible(true);
            errorDock->raise();
        }

        m_isTempCompiling = false;

        if (compileProcess)
        {
            compileProcess->deleteLater();
            compileProcess = nullptr;
        }
    });

    connect(compileProcess, &QProcess::readyReadStandardOutput,
            this, &CompilerIDE::readCompilerOutput);
    connect(compileProcess, &QProcess::readyReadStandardError,
            this, &CompilerIDE::readCompilerError);

    QString commandLine = actualCompilerPath + " " + args.join(" ");
    outputEdit->appendPlainText(tr("执行命令: %1").arg(commandLine));
    outputEdit->appendPlainText("");

    compileProcess->start(actualCompilerPath, args);
    if (!compileProcess->waitForStarted(5000))
    {
        QString errorMsg = tr("无法启动编译器: %1").arg(actualCompilerPath);
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        m_isTempCompiling = false;
    }
}

void CompilerIDE::tempCompileAndRun()
{
    if (m_isTempCompiling)
    {
        QMessageBox::warning(this, tr("提示"), tr("已有临时编译任务正在运行，请稍后..."));
        return;
    }

    if (compileProcess && compileProcess->state() == QProcess::Running)
    {
        QMessageBox::warning(this, tr("提示"), tr("已有编译任务正在运行，请稍后..."));
        return;
    }

    if (outputDock)
    {
        outputDock->raise();
    }

    if (m_tempCompileProcess)
    {
        m_tempCompileProcess->disconnect();
        if (m_tempCompileProcess->state() == QProcess::Running)
        {
            m_tempCompileProcess->kill();
            QTimer::singleShot(500, this, [this]()
            {
                if (m_tempCompileProcess && m_tempCompileProcess->state() != QProcess::NotRunning)
                {
                    m_tempCompileProcess->kill();
                }
            });
        }
        m_tempCompileProcess->deleteLater();
        m_tempCompileProcess = nullptr;
    }

    m_isTempCompiling = true;

    resetOutputFormat();
    outputEdit->clear();
    if (errorTableWidget)
    {
        errorTableWidget->setRowCount(0);
    }

    QTextCursor cursor = outputEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.setCharFormat(defaultFormat);
    outputEdit->setTextCursor(cursor);

    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        QMessageBox::warning(this, tr("临时编译运行"), tr("没有打开的编辑器"));
        m_isTempCompiling = false;
        return;
    }

    editor->clearErrorWarningLines();

    QString tempDir = QDir::temp().absolutePath() + "/CompilerIDE_TempBuild";
    QDir().mkpath(tempDir);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    tempCompileSourceFile = tempDir + "/temp_" + timestamp + ".cpp";
    tempCompileExeFile = tempDir + "/temp_" + timestamp + ".exe";

    QFile tempFile(tempCompileSourceFile);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("临时编译运行"),
                             tr("无法创建临时文件: %1").arg(tempFile.errorString()));
        m_isTempCompiling = false;
        return;
    }

    QTextStream out(&tempFile);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    out.setEncoding(QStringConverter::Utf8);
#endif
    out << editor->toPlainText();
    tempFile.close();

    isNewCompilation = true;
    accumulatedErrors.clear();
    accumulatedWarnings.clear();

    outputEdit->clear();
    outputEdit->appendPlainText(tr("=== 临时编译运行 ==="));
    outputEdit->appendPlainText("");
    outputEdit->appendPlainText(tr("临时源文件: %1").arg(tempCompileSourceFile));
    outputEdit->appendPlainText("");
    outputEdit->appendPlainText(tr("编译器: %1").arg(compilerPath));
    outputEdit->appendPlainText("");

    if (!QFile::exists(compilerPath))
    {
        QString errorMsg = tr("编译器路径不存在: %1").arg(compilerPath);
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        m_isTempCompiling = false;
        return;
    }

    QStringList args;
    bool isEasyX = editorEasyXFlags.value(editor, false);
    QString actualCompilerPath = compilerPath;

    if (compilerType == "msvc")
    {
        if (cppStandard == "c++14")
        {
            args << "/std:c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "/std:c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "/std:c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "/std:c++latest";
        }
        args << "/EHsc" << "/W4" << "/nologo" << "/utf-8";
        args << "/Fe" + tempCompileExeFile;
        args << tempCompileSourceFile;
    }
    else
    {
        if (cppStandard == "c++14")
        {
            args << "-std=c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "-std=c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "-std=c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "-std=c++2b";
        }
        args << "-g";
        args << optimizationLevel;
        args << "-Wall" << "-Wextra";
        args << "-finput-charset=UTF-8" << "-fexec-charset=UTF-8";
        args << "-static";
        args << "-static-libgcc" << "-static-libstdc++";

        if (isEasyX)
        {
            QString appDir = QCoreApplication::applicationDirPath();
            QString bundledTDMGCC = appDir + "/TDM-GCC/bin/g++.exe";

            QString compilerName = "配置的编译器";

            if (QFile::exists(bundledTDMGCC))
            {
                actualCompilerPath = bundledTDMGCC;
                compilerName = "内置 TDM-GCC";
                outputEdit->appendPlainText(tr("  使用内置 TDM-GCC 编译器"));
                outputEdit->appendPlainText(tr("  路径: %1").arg(bundledTDMGCC));
            }
            else
            {
                outputEdit->appendPlainText(tr("  使用配置的编译器"));
                outputEdit->appendPlainText(tr("  路径: %1").arg(actualCompilerPath));
            }

            QFileInfo compilerInfo(actualCompilerPath);
            QString compilerDir = compilerInfo.absolutePath();
            QDir baseDir(compilerDir);
            baseDir.cdUp();

            QString mingwLib = baseDir.absolutePath() + "/x86_64-w64-mingw32/lib";
            QString mingwInclude = baseDir.absolutePath() + "/x86_64-w64-mingw32/include";
            QString easyxLib = mingwLib + "/libgraphics.a";

            outputEdit->appendPlainText(tr("  EasyX 图形模式"));
            outputEdit->appendPlainText(tr("  库路径: %1").arg(mingwLib));
            outputEdit->appendPlainText(tr("  头文件路径: %1").arg(mingwInclude));
            outputEdit->appendPlainText("");

            if (!QFile::exists(easyxLib))
            {
                easyxLib = mingwLib + "/libeasyxw.a";
                if (!QFile::exists(easyxLib))
                {
                    easyxLib = mingwLib + "/libeasyx.a";
                    if (!QFile::exists(easyxLib))
                    {
                        QString errorMsg = tr("错误: 找不到 EasyX 库文件\n"
                                              "请确保 %1 编译器包含 EasyX 库").arg(compilerName);
                        outputEdit->appendPlainText(errorMsg);
                        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
                        updateErrorTable(accumulatedErrors, accumulatedWarnings);
                        m_isTempCompiling = false;
                        return;
                    }
                }
            }

            outputEdit->appendPlainText(tr("找到 EasyX 库: %1").arg(QFileInfo(easyxLib).fileName()));
            outputEdit->appendPlainText("");

            args << "-L" + QDir::toNativeSeparators(mingwLib);
            args << "-I" + QDir::toNativeSeparators(mingwInclude);

            args << "-DUNICODE" << "-D_UNICODE";

            args << "-static-libgcc" << "-static-libstdc++";
        }

        args << "-o" << tempCompileExeFile;
        args << tempCompileSourceFile;

        if (isEasyX)
        {
            args << "-leasyxw"
                 << "-lgdi32"
                 << "-limm32"
                 << "-lmsimg32"
                 << "-lole32"
                 << "-loleaut32"
                 << "-lwinmm"
                 << "-luuid"
                 << "-lgdiplus";
        }
    }

    if (compileProcess)
    {
        compileProcess->kill();
        compileProcess->waitForFinished();
        delete compileProcess;
    }

    compileProcess = new QProcess(this);
    compileProcess->setProcessChannelMode(QProcess::MergedChannels);

    QString appDir = QCoreApplication::applicationDirPath();
    QString mingwBin = appDir + "/mingw/bin";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString currentPath = env.value("PATH");
    env.insert("PATH", mingwBin + ";" + currentPath);
#ifdef Q_OS_WIN
    env.insert("PYTHONIOENCODING", "utf-8");
    env.insert("UTF-8", "1");
#endif
    compileProcess->setProcessEnvironment(env);

    connect(compileProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus)
    {
        if (exitCode == 0)
        {
            outputEdit->appendPlainText(tr("临时编译成功，正在运行"));
            outputEdit->appendPlainText("");
            outputEdit->appendPlainText("");
            statusBar()->showMessage(tr("临时编译成功，正在运行"), 3000);

            if (QFile::exists(tempCompileExeFile))
            {
                runExecutable(tempCompileExeFile);
            }
            else
            {
                outputEdit->appendPlainText(tr("错误：找不到可执行文件"));
            }
        }
        else
        {
            outputEdit->appendPlainText("");
            outputEdit->appendPlainText(tr("=== 临时编译失败 ==="));
            statusBar()->showMessage(tr("临时编译失败"), 3000);
            errorDock->setVisible(true);
            errorDock->raise();
        }

        m_isTempCompiling = false;

        if (compileProcess)
        {
            compileProcess->deleteLater();
            compileProcess = nullptr;
        }
    });

    connect(compileProcess, &QProcess::readyReadStandardOutput,
            this, &CompilerIDE::readCompilerOutput);
    connect(compileProcess, &QProcess::readyReadStandardError,
            this, &CompilerIDE::readCompilerError);

    QString commandLine = actualCompilerPath + " " + args.join(" ");
    outputEdit->appendPlainText(tr("执行命令: %1").arg(commandLine));
    outputEdit->appendPlainText("");

    compileProcess->start(actualCompilerPath, args);
    if (!compileProcess->waitForStarted(5000))
    {
        QString errorMsg = tr("无法启动编译器: %1").arg(actualCompilerPath);
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        m_isTempCompiling = false;
    }
}

void CompilerIDE::updateDebugControls()
{
    if (!debugStartAct || !debugStopAct || !debugContinueAct ||
            !debugStepOverAct || !debugStepIntoAct || !debugStepOutAct ||
            !debugToggleBreakpointAct || !debugRunToCursorAct)
    {
        return;
    }

    bool isRunning = debugger && debugger->isRunning();
    bool isPaused = debugger && debugger->isPaused();

    debugStartAct->setEnabled(!isRunning);
    debugStopAct->setEnabled(isRunning);
    debugContinueAct->setEnabled(isRunning && isPaused);
    debugStepOverAct->setEnabled(isRunning && isPaused);
    debugStepIntoAct->setEnabled(isRunning && isPaused);
    debugStepOutAct->setEnabled(isRunning && isPaused);
    debugToggleBreakpointAct->setEnabled(true);
    debugRunToCursorAct->setEnabled(isRunning && isPaused);

    if (compileAct)
    {
        compileAct->setEnabled(!isRunning);
    }
    if (runAct)
    {
        runAct->setEnabled(!isRunning);
    }
    if (compileRunAct)
    {
        compileRunAct->setEnabled(!isRunning);
    }
}

void CompilerIDE::showMemoryView()
{
    QDialog *memoryDialog = new QDialog(this);
    memoryDialog->setWindowTitle(tr("内存查看器"));
    memoryDialog->resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(memoryDialog);

    QHBoxLayout *addressLayout = new QHBoxLayout();
    QLabel *addressLabel = new QLabel(tr("地址:"));
    QLineEdit *addressEdit = new QLineEdit("0x00000000");
    QPushButton *refreshButton = new QPushButton(tr("刷新"));
    addressLayout->addWidget(addressLabel);
    addressLayout->addWidget(addressEdit);
    addressLayout->addWidget(refreshButton);

    QTextEdit *memoryView = new QTextEdit();
    memoryView->setFont(QFont("Courier New", 10));
    memoryView->setReadOnly(true);

    layout->addLayout(addressLayout);
    layout->addWidget(memoryView);

    connect(refreshButton, &QPushButton::clicked, this, [=]()
    {
        QString address = addressEdit->text();
        if (debugger->isRunning() && debugger->isPaused())
        {
            debugger->evaluateExpression(QString("*(unsigned char*)%1").arg(address));
            memoryView->setPlainText(tr("正在查看内存地址 %1...").arg(address));
        }
    });

    memoryDialog->exec();
}

void CompilerIDE::startDebugging()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        QMessageBox::warning(this, tr("调试错误"), tr("没有打开的文件"));
        return;
    }

    if (editor->document()->isModified())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                            tr("保存文件"),
                                            tr("文件有未保存的修改。是否在调试前保存？"),
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
        {
            if (!save())
            {
                return;
            }
        }
        else if (reply == QMessageBox::Cancel)
        {
            return;
        }
    }

    if (compilerPath.isEmpty())
    {
        QMessageBox::warning(this, tr("调试错误"), tr("未设置编译器路径"));
        showSettings();
        return;
    }

    QString effectiveDebuggerPath = debuggerPath;
    if (effectiveDebuggerPath.isEmpty())
    {
        effectiveDebuggerPath = findDebuggerFromCompilerPath(compilerPath);
    }
    if (effectiveDebuggerPath.isEmpty())
    {
        effectiveDebuggerPath = autoDetectDebuggerPath();
    }
    if (effectiveDebuggerPath.isEmpty() || !QFile::exists(effectiveDebuggerPath))
    {
        QMessageBox::warning(this, tr("调试错误"), tr("未找到调试器(GDB)"));
        showSettings();
        return;
    }

    debuggerPath = effectiveDebuggerPath;
    debugger->setDebuggerPath(debuggerPath);

    debugStartAct->setEnabled(false);

    outputEdit->appendPlainText(tr("=== 开始调试准备 ==="));
    statusBar()->showMessage(tr("正在编译..."));

    pendingDebugAfterCompile = true;

    compile();
}

void CompilerIDE::launchDebugger(const QString &sourceFile)
{
    outputEdit->appendPlainText(tr("正在启动调试器..."));

    QFileInfo sourceInfo(sourceFile);
    QString executablePath = sourceInfo.absolutePath() + "/" +
                             sourceInfo.completeBaseName() + ".exe";

    if (!QFile::exists(executablePath))
    {
        QStringList possiblePaths =
        {
            sourceInfo.absolutePath() + "/" + sourceInfo.completeBaseName(),
            sourceInfo.absolutePath() + "/a.exe",
            sourceInfo.absolutePath() + "/a.out"
        };

        for (const QString &path : possiblePaths)
        {
            if (QFile::exists(path))
            {
                executablePath = path;
                break;
            }
        }
    }

    if (!QFile::exists(executablePath))
    {
        QMessageBox::warning(this, tr("调试错误"),
                             tr("未找到可执行文件: %1").arg(executablePath));
        debugStartAct->setEnabled(true);
        statusBar()->showMessage(tr("调试启动失败"), 3000);
        return;
    }

    outputEdit->appendPlainText(tr("可执行文件: %1").arg(executablePath));
    outputEdit->appendPlainText(tr("调试器: %1").arg(debuggerPath));

    CodeEditor *editor = currentEditor();
    if (editor)
    {
        QSet<int> editorBreakpoints = editor->getBreakpoints();
        for (int line : editorBreakpoints)
        {
            debugger->setBreakpoint(sourceFile, line);
            outputEdit->appendPlainText(tr("设置断点: 第 %1 行").arg(line));
        }
    }

    if (debugger->startDebugging(executablePath, sourceFile))
    {
        setDebugState(DebugState_Running);
        isDebugging = true;
        debugDock->show();
        debugTabs->setCurrentIndex(0);
        statusBar()->showMessage(tr("调试器已启动"));
    }
    else
    {
        QMessageBox::warning(this, tr("调试错误"), tr("无法启动调试器"));
        debugStartAct->setEnabled(true);
        statusBar()->showMessage(tr("调试启动失败"), 3000);
    }
}

void CompilerIDE::continueStartDebugging()
{
    statusBar()->showMessage(tr("正在启动调试器..."));

    QString sourcePath = curFile;
    QFileInfo sourceInfo(sourcePath);
    QString executablePath;

    if (compilerType == "g++" || compilerType == "gcc")
    {
        executablePath = sourceInfo.absolutePath() + "/" + sourceInfo.completeBaseName() + ".exe";
    }
    else
    {
        executablePath = sourceInfo.absolutePath() + "/" + sourceInfo.completeBaseName() + ".exe";
    }

    if (!QFile::exists(executablePath))
    {
        QStringList possiblePaths =
        {
            sourceInfo.absolutePath() + "/" + sourceInfo.completeBaseName(),
            sourceInfo.absolutePath() + "/a.exe",
            sourceInfo.absolutePath() + "/a.out"
        };

        for (const QString &path : possiblePaths)
        {
            if (QFile::exists(path))
            {
                executablePath = path;
                break;
            }
        }
    }

    if (!QFile::exists(executablePath))
    {
        QMessageBox::warning(this, tr("调试错误"), tr("未找到可执行文件: %1").arg(executablePath));
        debugStartAct->setEnabled(true);
        return;
    }

    outputEdit->appendPlainText(tr("可执行文件: %1").arg(executablePath));
    outputEdit->appendPlainText(tr("调试器: %1").arg(debuggerPath));

    CodeEditor *editor = currentEditor();
    if (editor)
    {
        QSet<int> editorBreakpoints = editor->getBreakpoints();
        for (int line : editorBreakpoints)
        {
            debugger->setBreakpoint(sourcePath, line);
            outputEdit->appendPlainText(tr("设置断点: 第 %1 行").arg(line));
        }
    }

    outputEdit->appendPlainText(tr("启动调试器..."));

    if (debugger->startDebugging(executablePath, sourcePath))
    {
        setDebugState(DebugState_Running);
        isDebugging = true;
        debugDock->show();
        debugTabs->setCurrentIndex(0);
    }
    else
    {
        debugStartAct->setEnabled(true);
    }
}

void CompilerIDE::stopDebugging()
{
    if (!debugger->isRunning())
    {
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("停止调试"),
                                        tr("确定要停止调试会话吗？"),
                                        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        debugger->stopDebugging();
        isDebugging = false;
        setDebugState(DebugState_Idle);
        clearCurrentDebugLine();

        debugOutput->append(tr("=== 调试会话已停止 ==="));

        CodeEditor *editor = currentEditor();
        if (editor)
        {
            editor->clearCurrentDebugLine();
        }
    }
}

void CompilerIDE::continueDebugging()
{
    debugger->continueExecution();
}

void CompilerIDE::stepOver()
{
    debugger->stepOver();
}

void CompilerIDE::stepInto()
{
    debugger->stepInto();
}

void CompilerIDE::stepOut()
{
    debugger->stepOut();
}

void CompilerIDE::showFloatingMessage(const QString &message, int duration, bool isError)
{
    QLabel *label = new QLabel(this);
    label->setText(message);
    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);
    label->setMaximumWidth(600);

    QString bgColor, textColor, borderColor;
    if (isError)
    {
        if (darkTheme)
        {
            bgColor = "#8B0000";
            textColor = "white";
            borderColor = "#FF6B6B";
        }
        else
        {
            bgColor = "#dc3545";
            textColor = "white";
            borderColor = "#ff6b6b";
        }
    }
    else
    {
        if (darkTheme)
        {
            bgColor = "#1e7e34";
            textColor = "white";
            borderColor = "#5cb85c";
        }
        else
        {
            bgColor = "#28a745";
            textColor = "white";
            borderColor = "#5cb85c";
        }
    }

    label->setStyleSheet(QString(
                             "QLabel {"
                             "    background-color: %1;"
                             "    color: %2;"
                             "    font-size: 14px;"
                             "    font-weight: bold;"
                             "    padding: 10px 20px;"
                             "    border-radius: 8px;"
                             "    border: 2px solid %3;"
                             "}"
                         ).arg(bgColor, textColor, borderColor));
    label->adjustSize();

    int x = width() - label->width() - 20;
    int y = 20;
    label->move(x, y);
    label->raise();
    label->show();

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(label);
    label->setGraphicsEffect(effect);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::OutCubic);
    fadeIn->start();

    QTimer::singleShot(duration - 300, label, [label, effect]()
    {
        QPropertyAnimation *fadeOut = new QPropertyAnimation(effect, "opacity");
        fadeOut->setDuration(300);
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);
        fadeOut->setEasingCurve(QEasingCurve::InCubic);
        connect(fadeOut, &QPropertyAnimation::finished, label, &QLabel::deleteLater);
        fadeOut->start();
    });
}

void CompilerIDE::toggleBreakpoint()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }

    QTextCursor cursor = editor->textCursor();
    int line = cursor.blockNumber() + 1;

    QString filePath = curFile;
    if (filePath.isEmpty())
    {
        return;
    }

    if (editor->hasBreakpoint(line))
    {
        editor->removeBreakpoint(line);
        debugger->removeBreakpoint(filePath, line);
    }
    else
    {
        editor->addBreakpoint(line);
        debugger->setBreakpoint(filePath, line);
    }
}

void CompilerIDE::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    QTimer::singleShot(200, this, [this]()
    {
        qDebug() << "\n=== 修复光标 ===";

        while (QApplication::overrideCursor())
        {
            QApplication::restoreOverrideCursor();
        }

        QList<QWidget*> widgets = this->findChildren<QWidget*>();

        qDebug() << "总共找到" << widgets.size() << "个子控件";

        int waitCursorCount = 0;
        for (QWidget *widget : widgets)
        {
            Qt::CursorShape shape = widget->cursor().shape();
            if (shape == Qt::WaitCursor || shape == Qt::BusyCursor)
            {
                qDebug() << "发现等待光标:"
                         << widget->metaObject()->className()
                         << "对象名:" << widget->objectName()
                         << "光标形状:" << shape;
                waitCursorCount++;
            }
        }

        qDebug() << "发现" << waitCursorCount << "个等待光标";

        for (QWidget *widget : widgets)
        {
            widget->unsetCursor();
        }

        this->unsetCursor();

        CodeEditor *editor = currentEditor();
        if (editor)
        {
            editor->setCursor(Qt::IBeamCursor);
            qDebug() << "编辑器光标已设置为 IBeam";
        }

        qDebug() << "=== 修复完成 ===\n";
    });
}

void CompilerIDE::runToCursor()
{
    CodeEditor *editor = currentEditor();
    if (!editor || !debugger->isRunning())
    {
        return;
    }

    QTextCursor cursor = editor->textCursor();
    int line = cursor.blockNumber() + 1;

    QString filePath = curFile;
    if (filePath.isEmpty())
    {
        return;
    }

    debugger->runToCursor(filePath, line);
}

void CompilerIDE::evaluateExpression()
{
    QString expression = expressionInput->text().trimmed();
    if (expression.isEmpty())
    {
        return;
    }

    debugger->evaluateExpression(expression);
}

void CompilerIDE::removeAllBreakpoints()
{
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->setBreakpoints(QSet<int>());
    }

    debugger->removeAllBreakpoints();
    refreshBreakpointsTable();
}

void CompilerIDE::onDebuggingStarted()
{
    debugOutput->append(tr("=== 调试会话开始 ==="));
    isDebugging = true;
    updateDebugControls();
}

void CompilerIDE::onDebuggingStopped()
{
    debugOutput->append(tr("=== 调试会话结束 ==="));
    isDebugging = false;
    clearCurrentDebugLine();
    updateDebugControls();
}

void CompilerIDE::onDebuggingFinished()
{
    debugOutput->append(tr("程序执行完成"));
    isDebugging = false;
    clearCurrentDebugLine();
    updateDebugControls();
}

void CompilerIDE::onDebugOutputReceived(const QString &output)
{
    QString formattedOutput = output;

    if (output.contains("error:", Qt::CaseInsensitive) ||
            output.contains("错误:", Qt::CaseInsensitive))
    {
        formattedOutput = QString("<font color='red'><b>%1</b></font>").arg(output);
    }
    else if (output.contains("warning:", Qt::CaseInsensitive) ||
             output.contains("警告:", Qt::CaseInsensitive))
    {
        formattedOutput = QString("<font color='orange'><b>%1</b></font>").arg(output);
    }
    else if (output.contains("成功", Qt::CaseInsensitive) ||
             output.contains("success", Qt::CaseInsensitive))
    {
        formattedOutput = QString("<font color='green'><b>%1</b></font>").arg(output);
    }

    debugOutput->append(formattedOutput);

    QTextCursor cursor = debugOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    debugOutput->setTextCursor(cursor);
}

void CompilerIDE::onDebugErrorReceived(const QString &error)
{
    debugOutput->append(tr("错误: ") + error);
}

void CompilerIDE::onExecutionPaused(const QString &file, int line, const QString &reason)
{
    qDebug() << "程序暂停: " << file << ":" << line << "原因:" << reason;

    currentDebugFile = file;
    currentDebugLine = line;

    if (!file.isEmpty())
    {
        QString currentFileName = QFileInfo(curFile).fileName();
        QString pausedFileName = QFileInfo(file).fileName();

        if (currentFileName == pausedFileName || file == curFile)
        {
            setCurrentDebugLine(line);
        }
    }

    if (debugOutput)
    {
        debugOutput->append(tr("<font color='yellow'>程序暂停: %1 第 %2 行</font>").arg(file).arg(line));
    }
    statusBar()->showMessage(tr("程序暂停于 %1:%2").arg(QFileInfo(file).fileName()).arg(line));

    updateDebugControls();

    if (debugDock)
    {
        debugDock->show();
        debugDock->raise();
    }
}

void CompilerIDE::onExecutionResumed()
{
    debugOutput->append(tr("程序继续执行"));
    clearCurrentDebugLine();
    updateDebugControls();
}

void CompilerIDE::onBreakpointHit(const QString &file, int line, const QString &function)
{
    debugOutput->append(tr("断点命中: %1 第 %2 行 (%3)").arg(file).arg(line).arg(function));

    if (QFileInfo(file).fileName() == QFileInfo(curFile).fileName())
    {
        setCurrentDebugLine(line);
    }

    debugDock->show();
    debugTabs->setCurrentIndex(3);
}

void CompilerIDE::onVariablesUpdated(const QList<QPair<QString, QString>> &variables)
{
    updateVariablesTree(variables);
}

void CompilerIDE::onCallStackItemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
    {
        return;
    }

    QString frame = item->text();

    QStringList patterns =
    {
        "#(\\d+)\\s+(\\S+)\\s+at\\s+(\\S+):(\\d+)",
        "#(\\d+)\\s+0x[0-9a-f]+ in (\\S+)\\s+\\((.+):(\\d+)\\)",
        "#(\\d+)\\s+(\\S+)\\s+\\((.+):(\\d+)\\)",
        "#(\\d+)\\s+(\\S+)\\s+from\\s+(\\S+):(\\d+)"
    };

    QString function, file, lineStr;
    int line = -1;

    for (const QString &pattern : patterns)
    {
        QRegularExpression regex(pattern);
        QRegularExpressionMatch match = regex.match(frame);

        if (match.hasMatch())
        {
            function = match.captured(2);
            file = match.captured(3);
            lineStr = match.captured(4);
            line = lineStr.toInt();
            break;
        }
    }

    if (line > 0)
    {
        QString fullPath = file;

        if (!QFile::exists(fullPath))
        {
            for (auto it = debugFileMapping.begin(); it != debugFileMapping.end(); ++it)
            {
                if (it.key().contains(QFileInfo(file).fileName()))
                {
                    fullPath = it.value();
                    break;
                }
            }

            if (!QFile::exists(fullPath))
            {
                QFileInfo fileInfo(file);
                QStringList searchPaths =
                {
                    QDir::currentPath(),
                    QFileInfo(curFile).absolutePath(),
                    QCoreApplication::applicationDirPath()
                };

                for (const QString &path : searchPaths)
                {
                    QString candidate = path + "/" + fileInfo.fileName();
                    if (QFile::exists(candidate))
                    {
                        fullPath = candidate;
                        break;
                    }
                }
            }
        }

        if (QFile::exists(fullPath))
        {
            loadFile(fullPath);

            CodeEditor *editor = currentEditor();
            if (editor)
            {
                QTextBlock block = editor->document()->findBlockByLineNumber(line - 1);
                if (block.isValid())
                {
                    QTextCursor cursor(block);
                    editor->setTextCursor(cursor);
                    editor->centerCursor();

                    QList<QTextEdit::ExtraSelection> extraSelections;
                    QTextEdit::ExtraSelection selection;
                    selection.format.setBackground(QColor(255, 255, 200));
                    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
                    selection.cursor = cursor;
                    extraSelections.append(selection);
                    editor->setExtraSelections(extraSelections);

                    QTimer::singleShot(5000, editor, [editor]()
                    {
                        editor->setExtraSelections(QList<QTextEdit::ExtraSelection>());
                    });
                }
            }
        }
        else
        {
            QMessageBox::information(this, tr("文件未找到"),
                                     tr("无法找到源文件: %1\n行号: %2").arg(file).arg(line));
        }
    }
    else
    {
        QMessageBox::information(this, tr("无法解析调用栈"),
                                 tr("无法解析调用栈信息: %1").arg(frame));
    }
}

void CompilerIDE::onCallStackUpdated(const QStringList &frames)
{
    updateCallStack(frames);
}

void CompilerIDE::onExpressionEvaluated(const QString &expression, const QString &value)
{
    if (watchExpressions.contains(expression))
    {
        QTreeWidgetItem *watchItem = nullptr;

        for (int i = 0; i < variablesTree->topLevelItemCount(); ++i)
        {
            if (variablesTree->topLevelItem(i)->text(0) == tr("监视"))
            {
                watchItem = variablesTree->topLevelItem(i);
                break;
            }
        }

        if (!watchItem)
        {
            watchItem = new QTreeWidgetItem(variablesTree);
            watchItem->setText(0, tr("监视"));
            watchItem->setForeground(0, QBrush(QColor(0, 0, 200))); // 蓝色标题
        }

        bool found = false;
        for (int i = 0; i < watchItem->childCount(); ++i)
        {
            if (watchItem->child(i)->text(0) == expression)
            {
                watchItem->child(i)->setText(1, value);
                watchItem->child(i)->setText(2, tr("表达式"));
                found = true;
                break;
            }
        }

        if (!found)
        {
            QTreeWidgetItem *child = new QTreeWidgetItem(watchItem);
            child->setText(0, expression);
            child->setText(1, value);
            child->setText(2, tr("表达式"));
            child->setForeground(0, QBrush(QColor(100, 0, 100)));
        }

        variablesTree->expandItem(watchItem);
    }

    debugOutput->append(tr("表达式求值: %1 = %2").arg(expression).arg(value));
}

void CompilerIDE::onBreakpointAdded(const QString &file, int line)
{
    refreshBreakpointsTable();
}

void CompilerIDE::onBreakpointRemoved(const QString &file, int line)
{
    refreshBreakpointsTable();
}

void CompilerIDE::clearDebugInfo()
{
    breakpointsTable->setRowCount(0);
    variablesTree->clear();
    callStackList->clear();
    debugOutput->clear();
}

void CompilerIDE::refreshBreakpointsTable()
{
    if (!breakpointsTable)
    {
        return;
    }

    breakpointsTable->setRowCount(0);

    QMap<QString, QSet<int>> allBreakpoints = debugger->getAllBreakpoints();
    int row = 0;

    for (auto it = allBreakpoints.begin(); it != allBreakpoints.end(); ++it)
    {
        const QString &file = it.key();
        const QSet<int> &lines = it.value();

        for (int line : lines)
        {
            breakpointsTable->insertRow(row);

            QFileInfo fileInfo(file);
            QString displayName = fileInfo.fileName();

            QTableWidgetItem *fileItem = new QTableWidgetItem(displayName);
            fileItem->setToolTip(file);
            fileItem->setData(Qt::UserRole, file);

            QTableWidgetItem *lineItem = new QTableWidgetItem(QString::number(line));
            lineItem->setTextAlignment(Qt::AlignCenter);
            lineItem->setData(Qt::UserRole, line);

            QPair<QString, int> key(file, line);
            QString condition = conditionalBreakpoints.value(key, "");
            QTableWidgetItem *conditionItem = new QTableWidgetItem(
                condition.isEmpty() ? tr("无") : condition);

            if (!condition.isEmpty())
            {
                conditionItem->setForeground(QBrush(QColor(100, 200, 100)));
            }

            breakpointsTable->setItem(row, 0, fileItem);
            breakpointsTable->setItem(row, 1, lineItem);
            breakpointsTable->setItem(row, 2, conditionItem);

            row++;
        }
    }

    breakpointsTable->resizeColumnsToContents();
}

void CompilerIDE::showErrorTableContextMenu(const QPoint &pos)
{
    QTableWidgetItem *item = errorTableWidget->itemAt(pos);
    if (!item)
    {
        return;
    }

    int row = item->row();
    if (row < 0 || row >= errorTableWidget->rowCount())
    {
        return;
    }

    QString type = errorTableWidget->item(row, 0)->text();

    if (type == tr("错误"))
    {
        copyErrorAction->setText(tr("复制错误信息"));
    }
    else if (type == tr("警告"))
    {
        copyErrorAction->setText(tr("复制警告信息"));
    }
    else
    {
        copyErrorAction->setText(tr("复制信息"));
    }

    errorTableContextMenu->exec(errorTableWidget->viewport()->mapToGlobal(pos));
}

void CompilerIDE::copySelectedError()
{
    QList<QTableWidgetItem*> selectedItems = errorTableWidget->selectedItems();
    if (selectedItems.isEmpty())
    {
        return;
    }

    int row = selectedItems.first()->row();
    if (row < 0 || row >= errorTableWidget->rowCount())
    {
        return;
    }

    qDebug() << "=== 调试信息 ===";
    qDebug() << "总列数：" << errorTableWidget->columnCount();
    for (int col = 0; col < errorTableWidget->columnCount(); col++)
    {
        QTableWidgetItem *item = errorTableWidget->item(row, col);
        QString text = item ? item->text() : "NULL";
        qDebug() << "列" << col << ":" << text;
    }

    QString col0 = errorTableWidget->item(row, 0) ?
                   errorTableWidget->item(row, 0)->text() : "";
    QString col1 = errorTableWidget->item(row, 1) ?
                   errorTableWidget->item(row, 1)->text() : "";
    QString col2 = errorTableWidget->item(row, 2) ?
                   errorTableWidget->item(row, 2)->text() : "";
    QString col3 = errorTableWidget->item(row, 3) ?
                   errorTableWidget->item(row, 3)->text() : "";

    QString fullMessage;

    if (col3.contains(".cpp:") || col3.contains(".c:"))
    {
        fullMessage = col3;
    }
    else if (col2.contains(".cpp:") || col2.contains(".c:"))
    {
        fullMessage = col2;
    }
    else if (col1.contains(".cpp:") || col1.contains(".c:"))
    {
        fullMessage = col1;
    }
    else
    {
        fullMessage = QString("%1 %2 %3 %4").arg(col0).arg(col1).arg(col2).arg(col3);
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(fullMessage);

    statusBar()->showMessage(tr("已复制错误信息"), 2000);
}

void CompilerIDE::evaluateAllWatches()
{
    if (!debugger->isRunning() || !debugger->isPaused())
    {
        return;
    }

    QTreeWidget *watchTree = new QTreeWidget();
    watchTree->setHeaderLabels(QStringList() << tr("表达式") << tr("值") << tr("类型"));

    for (const QString &expr : watchExpressions)
    {
        debugger->evaluateExpression(expr);
    }
}

void CompilerIDE::setVariableValue()
{
    // 无用函数，留空，删了会报错
}

void CompilerIDE::updateMemoryView()
{
    // 同上
}

void CompilerIDE::updateVariablesTree(const QList<QPair<QString, QString>> &variables)
{
    if (!variablesTree)
    {
        qDebug() << "variablesTree 未初始化!";
        return;
    }

    qDebug() << "更新变量树，变量数量:" << variables.size();

    variablesTree->clear();

    if (variables.isEmpty())
    {
        QTreeWidgetItem *emptyItem = new QTreeWidgetItem(variablesTree);
        emptyItem->setText(0, tr("(无局部变量)"));
        emptyItem->setForeground(0, QBrush(QColor(128, 128, 128)));
        return;
    }

    QTreeWidgetItem *localsItem = new QTreeWidgetItem(variablesTree);
    localsItem->setText(0, tr("局部变量"));
    localsItem->setForeground(0, QBrush(QColor(86, 156, 214)));
    localsItem->setExpanded(true);

    for (const auto &var : variables)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(localsItem);
        item->setText(0, var.first);
        item->setText(1, var.second);

        qDebug() << "添加变量到树:" << var.first << "=" << var.second;

        QString value = var.second;
        QString type = tr("auto");

        if (value.startsWith('"') && value.endsWith('"'))
        {
            type = "string";
            item->setForeground(1, QBrush(QColor(206, 145, 120)));
        }
        else if (value == "true" || value == "false")
        {
            type = "bool";
            item->setForeground(1, QBrush(QColor(86, 156, 214)));
        }
        else if (value.contains('.') && !value.contains('{'))
        {
            type = "double";
            item->setForeground(1, QBrush(QColor(181, 206, 168)));
        }
        else
        {
            bool ok;
            value.toInt(&ok);
            if (ok)
            {
                type = "int";
                item->setForeground(1, QBrush(QColor(181, 206, 168)));
            }
        }

        item->setText(2, type);
        item->setForeground(2, QBrush(QColor(78, 201, 176)));
    }

    variablesTree->expandAll();

    if (debugTabs)
    {
        debugTabs->setCurrentIndex(2);
    }
}

void CompilerIDE::updateCallStack(const QStringList &stack)
{
    if (!callStackList)
    {
        qDebug() << "callStackList 未初始化!";
        return;
    }

    callStackList->clear();

    qDebug() << "更新调用栈UI，帧数:" << stack.size();

    for (const QString &frame : stack)
    {
        QListWidgetItem *item = new QListWidgetItem(frame);

        if (frame.startsWith("#0"))
        {
            item->setForeground(QBrush(QColor(78, 201, 176)));
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
        }
        else
        {
            item->setForeground(QBrush(QColor(180, 180, 180)));
        }

        callStackList->addItem(item);
    }

    if (callStackList->count() > 0)
    {
        callStackList->setCurrentRow(0);
    }
}

void CompilerIDE::showExpressionResult(const QString &expression, const QString &value)
{
    debugOutput->append(tr("表达式 %1 = %2").arg(expression).arg(value));
}

void CompilerIDE::setCurrentDebugLine(int line)
{
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->setCurrentDebugLine(line);
    }
}

void CompilerIDE::clearCurrentDebugLine()
{
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->clearCurrentDebugLine();
    }
}

void CompilerIDE::onBreakpointTableItemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString file = breakpointsTable->item(row, 0)->data(Qt::UserRole).toString();
    int line = breakpointsTable->item(row, 1)->data(Qt::UserRole).toInt();

    if (QFile::exists(file))
    {
        loadFile(file);
        CodeEditor *editor = currentEditor();
        if (editor)
        {
            QTextCursor cursor(editor->document()->findBlockByLineNumber(line - 1));
            editor->setTextCursor(cursor);
            editor->centerCursor();
        }
    }
}

void CompilerIDE::onVariablesTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (column == 0)
    {
        QString variableName = item->text(0);
        expressionInput->setText(variableName);
        debugTabs->setCurrentIndex(4);
        expressionInput->setFocus();
    }
    else if (column == 1)
    {
        QString variableName = item->text(0);
        QString currentValue = item->text(1);

        bool ok;
        QString newValue = QInputDialog::getText(this, tr("修改变量值"),
                           tr("为变量 %1 输入新值:").arg(variableName),
                           QLineEdit::Normal, currentValue, &ok);
        if (ok && !newValue.isEmpty())
        {
            debugger->setVariable(variableName, newValue);
        }
    }
}

void CompilerIDE::onDebugStarted()
{
    isDebugging = true;
    debugDock->setVisible(true);
    debugDock->raise();

    debugStartAct->setEnabled(false);
    debugStopAct->setEnabled(true);
    debugContinueAct->setEnabled(true);
    debugStepOverAct->setEnabled(true);
    debugStepIntoAct->setEnabled(true);
    debugStepOutAct->setEnabled(true);

    compileAct->setEnabled(false);
    runAct->setEnabled(false);
    compileRunAct->setEnabled(false);

    statusBar()->showMessage(tr("调试会话已启动"), 3000);
}

void CompilerIDE::editBreakpointCondition()
{
    int row = breakpointsTable->currentRow();
    if (row < 0)
    {
        return;
    }

    QString file = breakpointsTable->item(row, 0)->data(Qt::UserRole).toString();
    int line = breakpointsTable->item(row, 1)->data(Qt::UserRole).toInt();

    QPair<QString, int> key(file, line);
    QString currentCondition = conditionalBreakpoints.value(key, "");

    bool ok;
    QString condition = QInputDialog::getText(this, tr("编辑断点条件"),
                        tr("修改第 %1 行的断点条件:").arg(line),
                        QLineEdit::Normal, currentCondition, &ok);

    if (ok)
    {
        if (condition.trimmed().isEmpty())
        {
            conditionalBreakpoints.remove(key);
        }
        else
        {
            conditionalBreakpoints[key] = condition.trimmed();
        }
        refreshBreakpointsTable();
    }
}

void CompilerIDE::addConditionalBreakpoint()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }

    QTextCursor cursor = editor->textCursor();
    int line = cursor.blockNumber() + 1;
    QString filePath = curFile;

    if (filePath.isEmpty())
    {
        return;
    }

    bool ok;
    QString condition = QInputDialog::getText(this, tr("条件断点"),
                        tr("为第 %1 行设置条件 (例如: i == 5):").arg(line),
                        QLineEdit::Normal, "", &ok);

    if (ok)
    {
        if (!editor->hasBreakpoint(line))
        {
            editor->addBreakpoint(line);
        }

        QPair<QString, int> key(filePath, line);
        if (condition.trimmed().isEmpty())
        {
            conditionalBreakpoints.remove(key);
        }
        else
        {
            conditionalBreakpoints[key] = condition.trimmed();
        }

        if (debugger->isRunning())
        {
            debugger->removeBreakpoint(filePath, line);

            if (condition.trimmed().isEmpty())
            {
                debugger->setBreakpoint(filePath, line);
                debugOutput->append(tr("设置无条件断点: %1:%2").arg(QFileInfo(filePath).fileName()).arg(line));
            }
            else
            {
                // 目前未实现
                debugger->setBreakpoint(filePath, line);
                debugOutput->append(tr("设置条件断点: %1:%2 (条件: %3)")
                                    .arg(QFileInfo(filePath).fileName()).arg(line).arg(condition));
            }
        }

        refreshBreakpointsTable();
    }
}

void CompilerIDE::onDebugStopped()
{
    isDebugging = false;

    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->clearCurrentDebugLine();
    }

    debugStartAct->setEnabled(true);
    debugStopAct->setEnabled(false);
    debugContinueAct->setEnabled(false);
    debugStepOverAct->setEnabled(false);
    debugStepIntoAct->setEnabled(false);
    debugStepOutAct->setEnabled(false);

    compileAct->setEnabled(true);
    runAct->setEnabled(true);
    compileRunAct->setEnabled(true);

    clearDebugUI();

    statusBar()->showMessage(tr("调试会话已结束"), 3000);
}

void CompilerIDE::onDebugPaused(const QString &file, int line, const QString &reason)
{
    Q_UNUSED(reason)

    currentDebugFile = file;
    currentDebugLine = line;

    if (!file.isEmpty() && QFile::exists(file))
    {
        int tabIndex = findEditor(file);
        if (tabIndex < 0)
        {
            loadFile(file);
        }
        else
        {
            tabWidget->setCurrentIndex(tabIndex);
        }

        CodeEditor *editor = currentEditor();
        if (editor && line > 0)
        {
            editor->setCurrentDebugLine(line);
        }
    }

    statusBar()->showMessage(tr("程序暂停于 %1:%2").arg(QFileInfo(file).fileName()).arg(line));
}

void CompilerIDE::onDebugResumed()
{
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->clearCurrentDebugLine();
    }

    statusBar()->showMessage(tr("程序继续执行..."));
}

void CompilerIDE::onDebugOutput(const QString &output)
{
    if (debugOutputEdit)
    {
        debugOutputEdit->appendPlainText(output);

        QScrollBar *scrollBar = debugOutputEdit->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }

    outputEdit->appendPlainText(tr("[调试] ") + output);
}

void CompilerIDE::onDebugError(const QString &error)
{
    if (debugOutputEdit)
    {
        debugOutputEdit->appendPlainText(tr("错误: ") + error);

        QScrollBar *scrollBar = debugOutputEdit->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }

    outputEdit->appendPlainText(tr("[调试错误] ") + error);
}

void CompilerIDE::addWatchExpression()
{
    bool ok;
    QString expression = QInputDialog::getText(this, tr("添加监视"),
                         tr("输入要监视的表达式:"), QLineEdit::Normal, "", &ok);

    if (ok && !expression.trimmed().isEmpty())
    {
        watchExpressions.append(expression.trimmed());
        evaluateAllWatches();
    }
}

void CompilerIDE::removeWatchExpression()
{
    if (!watchList)
    {
        return;
    }

    QListWidgetItem *item = watchList->currentItem();
    if (item)
    {
        delete watchList->takeItem(watchList->row(item));
    }
}

void CompilerIDE::onCallStackItemClicked(QListWidgetItem *item)
{
    if (!item)
    {
        return;
    }

    QString frameText = item->text();

    QRegularExpression framePattern("#(\\d+)\\s+(\\w+)(?:\\s+at\\s+([^:]+):(\\d+))?");
    QRegularExpressionMatch match = framePattern.match(frameText);

    if (match.hasMatch())
    {
        QString file = match.captured(3);
        QString lineStr = match.captured(4);

        if (!file.isEmpty() && !lineStr.isEmpty())
        {
            int line = lineStr.toInt();

            if (QFile::exists(file))
            {
                loadFile(file);
                CodeEditor *editor = currentEditor();
                if (editor && line > 0)
                {
                    QTextBlock block = editor->document()->findBlockByLineNumber(line - 1);
                    if (block.isValid())
                    {
                        QTextCursor cursor(block);
                        editor->setTextCursor(cursor);
                        editor->centerCursor();
                    }
                }
            }
        }
    }
}

void CompilerIDE::startDebugging(const QString &executable)
{
    if (!debugger)
    {
        return;
    }

    debugger->setDebuggerPath(debuggerPath);
    debugger->removeAllBreakpoints();

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            QString fileName = tabWidget->tabText(i);
            if (fileName.endsWith("*"))
            {
                fileName.chop(1);
            }

            if (tabWidget->currentIndex() == i && !curFile.isEmpty())
            {
                fileName = curFile;
            }

            QSet<int> breakpoints = editor->getBreakpoints();
            for (int line : breakpoints)
            {
                debugger->setBreakpoint(fileName, line);
            }
        }
    }

    if (!debugger->startDebugging(executable, curFile))
    {
        QMessageBox::critical(this, tr("调试错误"),
                              tr("无法启动调试器。请检查设置中的调试器路径。\n\n"
                                 "当前调试器路径: %1\n\n"
                                 "建议:\n"
                                 "1. 确保GDB已安装\n"
                                 "2. 检查调试器路径是否正确\n"
                                 "3. 确保程序使用 -g 选项编译").arg(debuggerPath));
    }
}

void CompilerIDE::onVariableDoubleClicked(int row, int column)
{
    Q_UNUSED(column)

    if (!variablesTable || !isDebugging || !debugger->isPaused())
    {
        return;
    }

    QTableWidgetItem *nameItem = variablesTable->item(row, 0);
    QTableWidgetItem *valueItem = variablesTable->item(row, 1);

    if (!nameItem || !valueItem)
    {
        return;
    }

    QString varName = nameItem->text();
    QString currentValue = valueItem->text();

    bool ok;
    QString newValue = QInputDialog::getText(this, tr("修改变量"),
                       tr("变量 %1 的新值:").arg(varName),
                       QLineEdit::Normal, currentValue, &ok);

    if (ok && !newValue.isEmpty() && newValue != currentValue)
    {
        debugger->setVariable(varName, newValue);
    }
}

bool CompilerIDE::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == competitionStatusLabel && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if (competitionMode && competitionDock)
            {
                competitionDock->setVisible(true);
                competitionDock->raise();
            }
            return true;
        }
    }

    if (QPdfView *pdfView = qobject_cast<QPdfView*>(watched))
    {
        if (event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            if (wheelEvent->modifiers() & Qt::ControlModifier)
            {
                QPdfView::ZoomMode mode = pdfView->zoomMode();
                qreal factor = pdfView->zoomFactor();

                if (mode == QPdfView::ZoomMode::FitInView)
                {
                    factor = pdfView->zoomFactor();
                    pdfView->setZoomMode(QPdfView::ZoomMode::Custom);
                }

                qreal delta = (wheelEvent->angleDelta().y() > 0) ? 1.1 : (1.0 / 1.1);
                qreal newFactor = factor * delta;
                newFactor = qBound(0.1, newFactor, 10.0);

                pdfView->setZoomFactor(newFactor);
                wheelEvent->accept();
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void CompilerIDE::updateDebugUI()
{
    bool isRunning = debugger->isRunning();
    bool isPaused = debugger->isPaused();

    debugStartAct->setEnabled(!isRunning && currentDebugState == DebugState_Idle);
    debugStopAct->setEnabled(isRunning && (currentDebugState == DebugState_Running || currentDebugState == DebugState_Paused));
    debugContinueAct->setEnabled(isRunning && currentDebugState == DebugState_Paused);
    debugStepOverAct->setEnabled(isRunning && currentDebugState == DebugState_Paused);
    debugStepIntoAct->setEnabled(isRunning && currentDebugState == DebugState_Paused);
    debugStepOutAct->setEnabled(isRunning && currentDebugState == DebugState_Paused);
    debugToggleBreakpointAct->setEnabled(true);
    debugRunToCursorAct->setEnabled(isRunning && currentDebugState == DebugState_Paused);

    QString stateText;
    switch (currentDebugState)
    {
    case DebugState_Idle:
        stateText = tr("就绪");
        break;
    case DebugState_Running:
        stateText = tr("运行中");
        break;
    case DebugState_Paused:
        stateText = tr("已暂停");
        break;
    case DebugState_Finished:
        stateText = tr("调试完成");
        break;
    }

    QStatusBar *statusBar = this->statusBar();
    statusBar->showMessage(tr("调试状态: %1").arg(stateText), 5000);
}

void CompilerIDE::clearDebugUI()
{
    if (variablesTable)
    {
        variablesTable->setRowCount(0);
    }

    if (callStackList)
    {
        callStackList->clear();
    }

    currentDebugLine = -1;
    currentDebugFile.clear();
}

void CompilerIDE::createActions()
{
    newAct = new QAction(tr("新建(&N)"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("创建新文件"));
    connect(newAct, &QAction::triggered, this, &CompilerIDE::newFile);

    openAct = new QAction(tr("打开(&O)..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("打开现有文件"));
    connect(openAct, &QAction::triggered, this, &CompilerIDE::open);

    openProjectAct = new QAction(tr("打开项目(&P)..."), this);
    openProjectAct->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    openProjectAct->setStatusTip(tr("打开项目文件夹"));
    connect(openProjectAct, &QAction::triggered, this, &CompilerIDE::openProject);

    saveAct = new QAction(tr("保存(&S)"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("保存文档到磁盘"));
    connect(saveAct, &QAction::triggered, this, &CompilerIDE::save);

    saveAsAct = new QAction(tr("另存为(&A)..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("以新名称保存文档"));
    connect(saveAsAct, &QAction::triggered, this, &CompilerIDE::saveAs);

    closeAct = new QAction(tr("关闭(&C)"), this);
    closeAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_W));
    closeAct->setStatusTip(tr("关闭当前文件"));
    connect(closeAct, &QAction::triggered, [this]() { closeTab(tabWidget->currentIndex()); });

    closeAllAct = new QAction(tr("关闭所有(&L)"), this);
    closeAllAct->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_W));
    closeAllAct->setStatusTip(tr("关闭所有文件"));
    connect(closeAllAct, &QAction::triggered, [this]() {
        for (int i = tabWidget->count() - 1; i >= 0; --i) {
            closeTab(i);
        }
    });

    exitAct = new QAction(tr("退出(&X)"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("退出应用程序"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    cutAct = new QAction(tr("剪切(&T)"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("将当前选择的内容剪切到剪贴板"));
    connect(cutAct, &QAction::triggered, [this]() { if (currentEditor()) currentEditor()->cut(); });

    copyAct = new QAction(tr("复制(&C)"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("将当前选择的内容复制到剪贴板"));
    connect(copyAct, &QAction::triggered, [this]() { if (currentEditor()) currentEditor()->copy(); });

    pasteAct = new QAction(tr("粘贴(&P)"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("将剪贴板的内容粘贴到当前选择"));
    connect(pasteAct, &QAction::triggered, [this]() { if (currentEditor()) currentEditor()->paste(); });

    toggleCommentAct = new QAction(tr("切换注释(&/)"), this);
    toggleCommentAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Slash));
    toggleCommentAct->setStatusTip(tr("切换选中行的注释"));
    connect(toggleCommentAct, &QAction::triggered, this, &CompilerIDE::toggleComment);

    findAct = new QAction(tr("查找(&F)..."), this);
    findAct->setShortcuts(QKeySequence::Find);
    findAct->setStatusTip(tr("查找文本"));
    connect(findAct, &QAction::triggered, this, &CompilerIDE::find);

    replaceAct = new QAction(tr("替换(&R)..."), this);
    replaceAct->setShortcuts(QKeySequence::Replace);
    replaceAct->setStatusTip(tr("查找并替换文本"));
    connect(replaceAct, &QAction::triggered, this, &CompilerIDE::replace);

    findNextAct = new QAction(tr("查找下一个"), this);
    findNextAct->setShortcut(QKeySequence(Qt::Key_F3));
    findNextAct->setStatusTip(tr("查找下一个匹配项"));
    connect(findNextAct, &QAction::triggered, this, &CompilerIDE::findNext);

    findPreviousAct = new QAction(tr("查找上一个"), this);
    findPreviousAct->setShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F3));
    findPreviousAct->setStatusTip(tr("查找上一个匹配项"));
    connect(findPreviousAct, &QAction::triggered, this, &CompilerIDE::findPrevious);

    gotoLineAct = new QAction(tr("转到行(&G)..."), this);
    gotoLineAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
    gotoLineAct->setStatusTip(tr("转到指定行号"));
    connect(gotoLineAct, &QAction::triggered, this, &CompilerIDE::gotoLine);

    terminalShowAct = new QAction(tr("终端"), this);
    terminalShowAct->setCheckable(true);
    terminalShowAct->setChecked(false);
    terminalShowAct->setStatusTip(tr("显示或隐藏集成终端"));
    connect(terminalShowAct, &QAction::triggered, this, [this](bool checked) {
        terminalDock->setVisible(checked);
    });

    debugPanelAct = new QAction(tr("调试面板"), this);
    debugPanelAct->setCheckable(true);
    debugPanelAct->setChecked(false);
    connect(debugPanelAct, &QAction::triggered, this, [this](bool checked) {
        if (debugDock) {
            debugDock->setVisible(checked);
        }
    });

    feedbackAct = new QAction(tr("反馈问题"), this);
    feedbackAct->setStatusTip(tr("在GitHub上反馈问题"));
    connect(feedbackAct, &QAction::triggered, this, &CompilerIDE::openFeedbackPage);

    formatCodeAct = new QAction(tr("格式化代码(&F)"), this);
    formatCodeAct->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F));
    formatCodeAct->setStatusTip(tr("格式化当前文件的所有代码"));
    connect(formatCodeAct, &QAction::triggered, this, &CompilerIDE::formatAllCode);

    templateSettingsAct = new QAction(tr("代码模板设置(&T)..."), this);
    templateSettingsAct->setStatusTip(tr("设置新建文件的缺省源代码"));
    connect(templateSettingsAct, &QAction::triggered, this, &CompilerIDE::showTemplateSettings);

    compileAct = new QAction(tr("编译(&B)"), this);
    compileAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
    compileAct->setStatusTip(tr("编译当前文件"));
    connect(compileAct, &QAction::triggered, this, &CompilerIDE::compile);

    runAct = new QAction(tr("运行(&R)"), this);
    runAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    runAct->setStatusTip(tr("运行上次编译的程序"));
    connect(runAct, &QAction::triggered, this, &CompilerIDE::run);

    compileRunAct = new QAction(tr("编译并运行(&F5)"), this);
    compileRunAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F5));
    compileRunAct->setStatusTip(tr("编译并运行当前文件"));
    connect(compileRunAct, &QAction::triggered, this, &CompilerIDE::compileAndRun);

    clearOutputAct = new QAction(tr("清除输出"), this);
    clearOutputAct->setStatusTip(tr("清除输出面板"));
    connect(clearOutputAct, &QAction::triggered, this, &CompilerIDE::clearOutput);

    settingsAct = new QAction(tr("设置(&S)..."), this);
    settingsAct->setStatusTip(tr("配置应用程序设置"));
    connect(settingsAct, &QAction::triggered, this, &CompilerIDE::showSettings);

    tempCompileAct = new QAction(tr("临时编译"), this);
    tempCompileAct->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_B));
    tempCompileAct->setStatusTip(tr("无需保存文件，直接编译当前编辑器内容"));
    connect(tempCompileAct, &QAction::triggered, this, &CompilerIDE::tempCompile);

    tempCompileRunAct = new QAction(tr("临时编译运行"), this);
    tempCompileRunAct->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_F5));
    tempCompileRunAct->setStatusTip(tr("无需保存文件，直接编译并运行当前编辑器内容"));
    connect(tempCompileRunAct, &QAction::triggered, this, &CompilerIDE::tempCompileAndRun);

    zoomInAct = new QAction(tr("放大"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("放大文本"));
    connect(zoomInAct, &QAction::triggered, this, &CompilerIDE::zoomIn);

    zoomOutAct = new QAction(tr("缩小"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("缩小文本"));
    connect(zoomOutAct, &QAction::triggered, this, &CompilerIDE::zoomOut);

    resetZoomAct = new QAction(tr("重置缩放"), this);
    resetZoomAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    resetZoomAct->setStatusTip(tr("重置文本缩放"));
    connect(resetZoomAct, &QAction::triggered, this, &CompilerIDE::resetZoom);

    competitionModeAct = new QAction(tr("竞赛模式"), this);
    competitionModeAct->setCheckable(true);
    competitionModeAct->setChecked(false);
    competitionModeAct->setStatusTip(tr("开启竞赛模式，禁用部分功能以提高专注度"));
    connect(competitionModeAct, &QAction::triggered, this, &CompilerIDE::showCompetitionModeDialog);

    aboutAct = new QAction(tr("关于(&A)"), this);
    aboutAct->setStatusTip(tr("显示应用程序的关于框"));
    connect(aboutAct, &QAction::triggered, this, &CompilerIDE::about);

    checkForUpdatesAct = new QAction(tr("检查更新"), this);
    checkForUpdatesAct->setStatusTip(tr("检查 Compiler IDE 更新"));
    connect(checkForUpdatesAct, &QAction::triggered, this, &CompilerIDE::checkForUpdates);

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
}

void CompilerIDE::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openProjectAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(closeAllAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(toggleCommentAct);
    editMenu->addSeparator();
    editMenu->addAction(findAct);
    editMenu->addAction(replaceAct);
    editMenu->addAction(findNextAct);
    editMenu->addAction(findPreviousAct);
    editMenu->addAction(gotoLineAct);

    buildMenu = menuBar()->addMenu(tr("构建(&B)"));
    buildMenu->addAction(compileAct);
    buildMenu->addAction(runAct);
    buildMenu->addAction(compileRunAct);
    buildMenu->addSeparator();
    buildMenu->addAction(tempCompileAct);
    buildMenu->addAction(tempCompileRunAct);
    buildMenu->addSeparator();
    buildMenu->addAction(clearOutputAct);

    viewMenu = menuBar()->addMenu(tr("视图(&V)"));
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(resetZoomAct);
    viewMenu->addSeparator();
    viewMenu->addAction(terminalShowAct);
    viewMenu->addAction(debugPanelAct);

    toolsMenu = menuBar()->addMenu(tr("工具(&T)"));
    toolsMenu->addAction(settingsAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(templateSettingsAct);
    toolsMenu->addSeparator();
    toolsMenu->addAction(formatCodeAct);
    toolsMenu->addAction(competitionModeAct);
    QAction *showCompetitionPanelAct = new QAction(tr("显示竞赛面板"), this);
    showCompetitionPanelAct->setStatusTip(tr("显示竞赛模式面板"));
    connect(showCompetitionPanelAct, &QAction::triggered, this, [this]()
    {
        if (competitionMode && competitionDock)
        {
            competitionDock->setVisible(true);
            competitionDock->raise();
        }
        else
        {
            QMessageBox::information(this, tr("提示"), tr("请先开启竞赛模式"));
        }
    });
    toolsMenu->addAction(showCompetitionPanelAct);

    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(feedbackAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);

    QAction *manualCheckUpdateAct = new QAction(tr("检查更新"), this);
    manualCheckUpdateAct->setStatusTip(tr("手动检查 Compiler IDE 更新"));
    connect(manualCheckUpdateAct, &QAction::triggered, this, [this]()
    {
        static QTime lastClickTime = QTime::currentTime().addSecs(-10);
        QTime currentTime = QTime::currentTime();
        if (lastClickTime.msecsTo(currentTime) < 1000)
        {
            qDebug() << "检测到重复点击，忽略";
            return;
        }
        lastClickTime = currentTime;
        this->checkForUpdates();
    });
    helpMenu->addAction(manualCheckUpdateAct);
}

void CompilerIDE::createToolBars()
{
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->setObjectName("FileToolBar");
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("编辑"));
    editToolBar->setObjectName("EditToolBar");
    editToolBar->addAction(toggleCommentAct);
    editToolBar->addSeparator();
    editToolBar->addAction(findAct);
    editToolBar->addAction(replaceAct);

    buildToolBar = addToolBar(tr("构建"));
    buildToolBar->setObjectName("BuildToolBar");
    buildToolBar->addAction(compileAct);
    buildToolBar->addAction(runAct);
    buildToolBar->addAction(compileRunAct);
    buildToolBar->addAction(clearOutputAct);
    buildToolBar->addSeparator();
    buildToolBar->addAction(tempCompileAct);
    buildToolBar->addAction(tempCompileRunAct);
    buildToolBar->addSeparator();
    buildToolBar->addAction(clearOutputAct);

    viewToolBar = addToolBar(tr("视图"));
    viewToolBar->setObjectName("ViewToolBar");
    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);
    viewToolBar->addAction(resetZoomAct);

    buildToolBar->addSeparator();
    QLabel *standardLabel = new QLabel(tr("C++标准:"));
    buildToolBar->addWidget(standardLabel);

    cppStandardCombo = new QComboBox;
    cppStandardCombo->addItem("C++14");
    cppStandardCombo->addItem("C++17");
    cppStandardCombo->addItem("C++20");
    cppStandardCombo->addItem("C++23");
    cppStandardCombo->setCurrentText("C++17");
    connect(cppStandardCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CompilerIDE::changeCppStandard);
    buildToolBar->addWidget(cppStandardCombo);

    optimizationCombo = new QComboBox(this);
    optimizationCombo->addItems({"O0", "O1", "O2", "O3", "Os"});
    optimizationCombo->setCurrentText("O0");
    optimizationLevel = "-O0";
    connect(optimizationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CompilerIDE::changeOptimizationLevel);
    buildToolBar->addWidget(new QLabel(tr("    优化: ")));
    buildToolBar->addWidget(optimizationCombo);

    updateComboBoxStyles();
    applyGlobalMenuStyle();
}

void CompilerIDE::loadPdfFile(const QString &fileName)
{
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        if (tabWidget->tabToolTip(i) == fileName)
        {
            tabWidget->setCurrentIndex(i);
            return;
        }
    }

    QPdfView *pdfView = new QPdfView;
    pdfView->setDocument(new QPdfDocument(pdfView));

    QPdfDocument *doc = pdfView->document();
    doc->load(fileName);

    if (doc->status() != QPdfDocument::Status::Ready)
    {
        QMessageBox::warning(this, tr("打开 PDF 失败"),
                             tr("无法加载 PDF 文件：\n%1").arg(fileName));
        delete pdfView;
        return;
    }

    pdfView->setPageMode(QPdfView::PageMode::MultiPage);
    pdfView->setZoomMode(QPdfView::ZoomMode::FitInView);
    pdfView->installEventFilter(this);

    int index = tabWidget->addTab(pdfView, QFileInfo(fileName).fileName());
    tabWidget->setCurrentIndex(index);
    tabWidget->setTabToolTip(index, fileName);

    addToRecentFiles(fileName);
    statusBar()->showMessage(tr("已打开 PDF: %1").arg(fileName), 2000);
}

void CompilerIDE::autoCheckForUpdates()
{
    if (isCheckingForUpdates)
    {
        qDebug() << "自动更新检查正在进行中，跳过重复请求";
        return;
    }

    isCheckingForUpdates = true;

    statusBar()->showMessage(tr("正在检查更新..."));

    if (updateCheckConnection)
    {
        disconnect(updateCheckConnection);
    }

    updateManager->checkForUpdates(true);

    updateCheckConnection = connect(updateManager, &UpdateManager::updateCheckFinished,
                                    this, [this](bool updateAvailable, const QString &newVersion)
    {
        isCheckingForUpdates = false;

        if (updateAvailable)
        {
            statusBar()->showMessage(tr("发现新版本 %1").arg(newVersion), 3000);
            qDebug() << "自动检查发现新版本:" << newVersion;
        }
        else
        {
            statusBar()->showMessage(tr("当前已是最新版本"), 2000);
            qDebug() << "自动检查: 当前已是最新版本";
        }

        disconnect(updateCheckConnection);
    });

    connect(updateManager, &UpdateManager::noUpdateAvailable, this, [this]()
    {
        isCheckingForUpdates = false;
        statusBar()->showMessage(tr("当前已是最新版本"), 2000);
    });
}

QString CompilerIDE::cleanExtraNewlines(const QString &text)
{
    QString result = text;

    while (result.startsWith('\n'))
    {
        result = result.mid(1);
    }
    while (result.endsWith('\n'))
    {
        result = result.left(result.length() - 1);
    }

    QRegularExpression multipleNewlines(R"(\n{2,})");
    result.replace(multipleNewlines, "\n");

    return result;
}

void CompilerIDE::onUpdateCheckFinished(bool updateAvailable, const QString &newVersion)
{
    if (updateAvailable)
    {
        qDebug() << "新版本可用:" << newVersion;
    }
    else
    {
        if (QObject::sender() == checkForUpdatesAct)
        {
            QMessageBox::information(this, tr("检查更新"),
                                     tr("当前已是最新版本 (%1)").arg(IDE_VERSION));
        }
    }
}

void CompilerIDE::checkForUpdates()
{
    if (isCheckingForUpdates)
    {
        qDebug() << "更新检查正在进行中，跳过重复请求";
        return;
    }

    isCheckingForUpdates = true;

    statusBar()->showMessage(tr("正在检查更新..."));

    if (updateCheckConnection)
    {
        disconnect(updateCheckConnection);
    }
    if (noUpdateConnection)
    {
        disconnect(noUpdateConnection);
    }

    updateManager->checkForUpdates(false);

    updateCheckConnection = connect(updateManager, &UpdateManager::updateCheckFinished,
                                    this, [this](bool updateAvailable, const QString &newVersion)
    {
        isCheckingForUpdates = false;

        if (updateAvailable)
        {
            statusBar()->showMessage(tr("发现新版本 %1").arg(newVersion), 3000);
        }
        else
        {
            statusBar()->showMessage(tr("检查完成"), 2000);
        }

        disconnect(updateCheckConnection);
    });

    noUpdateConnection = connect(updateManager, &UpdateManager::noUpdateAvailable,
                                 this, [this]()
    {
        isCheckingForUpdates = false;

        QMessageBox::information(this, tr("检查更新"), tr("当前已是最新版本 (%1)").arg(IDE_VERSION));
        statusBar()->showMessage(tr("当前已是最新版本"), 3000);

        disconnect(noUpdateConnection);
    });
}

void CompilerIDE::showUpdateInfo()
{
    QString updateInfo = tr(
                             "<h3>Compiler IDE 自动更新</h3>"
                             "<p>当前版本: <b>%1</b></p>"
                             "<p>自动更新功能会在启动时检查新版本，并在安装包下载完成的 3 秒后自动开始更新。</p>"
                             "<p>您也可以手动检查更新。</p>"
                         ).arg(IDE_VERSION);

    QMessageBox::information(this, tr("更新信息"), updateInfo);
}

void CompilerIDE::createCompetitionDock()
{
    competitionDock = new QDockWidget(tr("竞赛模式"), this);
    competitionDock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    competitionDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    competitionDock->setVisible(false);

    competitionWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(competitionWidget);

    timerLabel = new QLabel("00:00:00");
    timerLabel->setAlignment(Qt::AlignCenter);
    QFont timerFont = timerLabel->font();
    timerFont.setPointSize(20);
    timerFont.setBold(true);
    timerLabel->setFont(timerFont);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *countdownBtn = new QPushButton(tr("倒计时"));
    QPushButton *stopwatchBtn = new QPushButton(tr("正计时"));

    pauseBtn = new QPushButton(tr("暂停"));

    QPushButton *resetBtn = new QPushButton(tr("重置"));
    QPushButton *endCompBtn = new QPushButton(tr("结束竞赛"));
    endCompBtn->setStyleSheet("QPushButton { color: red; font-weight: bold; }");

    QPushButton *hideBtn = new QPushButton(tr("隐藏面板"));

    buttonLayout->addWidget(countdownBtn);
    buttonLayout->addWidget(stopwatchBtn);
    buttonLayout->addWidget(pauseBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addWidget(endCompBtn);
    buttonLayout->addWidget(hideBtn);

    layout->addWidget(timerLabel);
    layout->addLayout(buttonLayout);

    competitionWidget->setLayout(layout);
    competitionDock->setWidget(competitionWidget);
    addDockWidget(Qt::TopDockWidgetArea, competitionDock);

    connect(countdownBtn, &QPushButton::clicked, this, &CompilerIDE::startCountdownTimer);
    connect(stopwatchBtn, &QPushButton::clicked, this, &CompilerIDE::startStopwatchTimer);

    connect(pauseBtn, &QPushButton::clicked, this, &CompilerIDE::toggleCompetitionTimer);

    connect(resetBtn, &QPushButton::clicked, this, &CompilerIDE::resetCompetitionTimer);

    connect(hideBtn, &QPushButton::clicked, this, [this]()
    {
        isHidingPanel = true;
        competitionDock->setVisible(false);
    });

    connect(competitionDock, &QDockWidget::visibilityChanged, this, [this](bool visible)
    {
        if (!visible && competitionMode)
        {
            if (isHidingPanel)
            {
                isHidingPanel = false;
                return;
            }

            competitionDock->setVisible(true);

            QMessageBox::StandardButton reply = QMessageBox::question(
                                                    this, tr("退出竞赛模式"),
                                                    tr("您正处于竞赛模式中，确定要退出竞赛模式吗？"),
                                                    QMessageBox::Yes | QMessageBox::No
                                                );

            if (reply == QMessageBox::Yes)
            {
                stopCompetitionMode();
            }
        }
    });

    connect(endCompBtn, &QPushButton::clicked, this, [this]()
    {
        if (competitionMode)
        {
            QMessageBox::StandardButton reply = QMessageBox::question(
                                                    this, tr("退出竞赛模式"),
                                                    tr("您正处于竞赛模式中，确定要退出竞赛模式吗？"),
                                                    QMessageBox::Yes | QMessageBox::No
                                                );
            if (reply == QMessageBox::Yes)
            {
                stopCompetitionMode();
            }
        }
    });
}

void CompilerIDE::toggleCompetitionTimer()
{
    if (!competitionMode)
    {
        return;
    }

    if (competitionTimer->isActive())
    {
        competitionTimer->stop();
        pauseBtn->setText(tr("继续"));
        outputEdit->appendPlainText(tr("计时器已暂停"));
    }
    else
    {
        competitionTimer->start(1000);
        pauseBtn->setText(tr("暂停"));
        outputEdit->appendPlainText(tr("计时器继续运行"));
    }
}


void CompilerIDE::showCompetitionModeDialog()
{
    if (competitionMode)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(
                                                this, tr("退出竞赛模式"),
                                                tr("您正处于竞赛模式中。确定要退出竞赛模式吗？"),
                                                QMessageBox::Yes | QMessageBox::No
                                            );

        if (reply == QMessageBox::Yes)
        {
            stopCompetitionMode();
            competitionModeAct->setChecked(false);
        }
        else
        {
            competitionModeAct->setChecked(true);
        }
        return;
    }

    CompetitionModeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        startCompetitionMode(
            dialog.isCountdown(),
            dialog.getMinutes(),
            dialog.disableCodeCompletion(),
            dialog.disableAISuggestions(),
            dialog.disableAutoBrackets(),
            dialog.disableAutoQuotes(),
            dialog.disableAutoIndent(),
            dialog.disableTerminal(),
            dialog.disableSnippets(),
            dialog.disableDiagnostics(),
            dialog.disableProjectManager()
        );
        competitionModeAct->setChecked(true);
    }
    else
    {
        competitionModeAct->setChecked(false);
    }
}

void CompilerIDE::startCompetitionMode(bool countdown, int minutes,
                                      bool disableCodeCompletion, bool disableAISuggestions,
                                      bool disableAutoBrackets, bool disableAutoQuotes,
                                      bool disableAutoIndent,
                                      bool disableTerminal, bool disableSnippets,
                                      bool disableDiagnostics, bool disableProjectManager)
{
    competitionMode = true;
    isCountdown = countdown;
    comp_disableCodeCompletion = disableCodeCompletion;
    comp_disableDeepSeek = disableAISuggestions;
    comp_disableAutoBrackets = disableAutoBrackets;
    comp_disableAutoQuotes = disableAutoQuotes;
    comp_disableAutoIndent = disableAutoIndent;
    comp_disableTerminal = disableTerminal;
    comp_disableSnippets = disableSnippets;
    comp_disableDiagnostics = disableDiagnostics;
    comp_disableProjectManager = disableProjectManager;
    originalTerminalVisible = terminalDock->isVisible();
    originalSnippetsVisible = snippetDock->isVisible();
    originalDiagnosticsVisible = diagnosticsDock->isVisible();
    originalProjectManagerVisible = projectDock->isVisible();

    if (isCountdown)
    {
        timerSeconds = minutes * 60;
    }
    else
    {
        timerSeconds = 0;
    }

    lastTimerStyle.clear();

    if (competitionStatusLabel)
    {
        competitionStatusLabel->setVisible(true);
    }

    updateTimerDisplay();

    applyCompetitionModeSettings();

    competitionDock->setVisible(true);
    competitionDock->raise();

    if(pauseBtn) pauseBtn->setText(tr("暂停"));

    if (!competitionTimer) {
        competitionTimer = new QTimer(this);
        connect(competitionTimer, &QTimer::timeout, this, &CompilerIDE::updateTimer);
    }

    competitionTimer->start(1000);

    outputEdit->appendPlainText(tr("=== 竞赛模式已开启 ==="));
    outputEdit->appendPlainText(tr("计时器: %1").arg(isCountdown ? tr("倒计时") : tr("正计时")));
    if (isCountdown)
    {
        outputEdit->appendPlainText(tr("初始时间: %1 分钟").arg(minutes));
    }
    outputEdit->appendPlainText(tr("已禁用功能:"));
    if (comp_disableCodeCompletion)
    {
        outputEdit->appendPlainText(tr("  - 代码补全"));
    }
    if (comp_disableDeepSeek)
    {
        outputEdit->appendPlainText(tr("  - DeepSeek AI 助手"));
    }
    if (comp_disableAutoBrackets)
    {
        outputEdit->appendPlainText(tr("  - 自动括号补全"));
    }
    if (comp_disableAutoQuotes)
    {
        outputEdit->appendPlainText(tr("  - 自动引号补全"));
    }
    if (comp_disableAutoIndent)
    {
        outputEdit->appendPlainText(tr("  - 自动缩进"));
    }
    if (comp_disableTerminal)
    {
        outputEdit->appendPlainText(tr("  - 终端"));
    }
    if (comp_disableSnippets)
    {
        outputEdit->appendPlainText(tr("  - 代码片段"));
    }
    if (comp_disableDiagnostics)
    {
        outputEdit->appendPlainText(tr("  - 诊断工具"));
    }
    if (comp_disableProjectManager)
    {
        outputEdit->appendPlainText(tr("  - 项目管理器"));
    }
    outputEdit->appendPlainText("");

    statusBar()->showMessage(tr("竞赛模式已开启"), 3000);
}

void CompilerIDE::stopCompetitionMode()
{
    if (!competitionMode)
    {
        return;
    }

    competitionMode = false;

    if (competitionTimer && competitionTimer->isActive())
    {
        competitionTimer->stop();
    }

    if (competitionStatusLabel)
    {
        competitionStatusLabel->setVisible(false);
    }

    lastTimerStyle.clear();

    restoreNormalModeSettings();

    competitionDock->setVisible(false);
    competitionModeAct->setChecked(false);

    outputEdit->appendPlainText(tr("=== 竞赛模式已关闭 ==="));
    outputEdit->appendPlainText("");

    statusBar()->showMessage(tr("竞赛模式已关闭"), 3000);
}

void CompilerIDE::showFloatingTimeWarning(const QString &message, int duration)
{
    if (warningFlashTimer->isActive())
    {
        warningFlashTimer->stop();
    }

    floatingTimeWarning->setText(message);
    floatingTimeWarning->adjustSize();

    int x = (width() - floatingTimeWarning->width()) / 2;
    int y = 50;
    floatingTimeWarning->move(x, y);
    floatingTimeWarning->raise();

    QGraphicsOpacityEffect *effect = qobject_cast<QGraphicsOpacityEffect*>(
        floatingTimeWarning->graphicsEffect());

    if (!effect)
    {
        effect = new QGraphicsOpacityEffect(floatingTimeWarning);
        floatingTimeWarning->setGraphicsEffect(effect);
    }

    QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity", this);
    fadeIn->setDuration(300);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::OutCubic);
    floatingTimeWarning->setVisible(true);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    if (duration > 0)
    {
        QTimer::singleShot(duration - 300, this, [this, effect]()
        {
            QPropertyAnimation *fadeOut = new QPropertyAnimation(effect, "opacity", this);
            fadeOut->setDuration(300);
            fadeOut->setStartValue(1.0);
            fadeOut->setEndValue(0.0);
            fadeOut->setEasingCurve(QEasingCurve::InCubic);

            connect(fadeOut, &QPropertyAnimation::finished, this, [this]()
            {
                floatingTimeWarning->setVisible(false);
            });

            fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }
}

void CompilerIDE::updateTimer()
{
    if (isCountdown)
    {
        if (timerSeconds > 0)
        {
            timerSeconds--;
            updateTimerDisplay();

            if (timerSeconds == 30 * 60)
            {
                if (competitionDock->isVisible())
                {
                    outputEdit->appendPlainText(tr("剩余时间：30分钟！"));
                }
                else
                {
                    showFloatingTimeWarning(tr("剩余时间：30分钟！"), 5000);
                }
            }
            else if (timerSeconds == 5 * 60)
            {
                if (competitionDock->isVisible())
                {
                    outputEdit->appendPlainText(tr("剩余时间：5分钟！"));
                }
                else
                {
                    showFloatingTimeWarning(tr("剩余时间：5分钟！"), 5000);
                }
            }
            else if (timerSeconds == 60)
            {
                if (competitionDock->isVisible())
                {
                    outputEdit->appendPlainText(tr("剩余时间：1分钟！"));
                }
                else
                {
                    showFloatingTimeWarning(tr("剩余时间：1分钟！"), 5000);
                }
            }
            else if (timerSeconds == 30)
            {
                if (competitionDock->isVisible())
                {
                    outputEdit->appendPlainText(tr("剩余时间：30秒！"));
                }
                else
                {
                    showFloatingTimeWarning(tr("剩余时间：30秒！"), 3000);
                }
            }
        }
        else
        {
            if (!competitionDock->isVisible())
            {
                showFloatingTimeWarning(tr("时间到！"), 0);
            }
            outputEdit->appendPlainText(tr("时间到！竞赛模式自动关闭。"));
            QMessageBox::information(this, tr("时间到"), tr("竞赛时间已结束！"));
            stopCompetitionMode();
            competitionModeAct->setChecked(false);
        }
    }
    else
    {
        timerSeconds++;
        updateTimerDisplay();
    }
}

void CompilerIDE::updateTimerDisplay()
{
    if (!timerLabel)
    {
        return;
    }

    int hours = timerSeconds / 3600;
    int minutes = (timerSeconds % 3600) / 60;
    int seconds = timerSeconds % 60;

    QString timeText = QString("%1:%2:%3")
                          .arg(hours, 2, 10, QLatin1Char('0'))
                          .arg(minutes, 2, 10, QLatin1Char('0'))
                          .arg(seconds, 2, 10, QLatin1Char('0'));

    timerLabel->setText(timeText);

    QString newStyle;
    QString statusColor;

    QString bgColor = darkTheme ? "#2D3047" : "#F0F0F0";
    QString textColorNormal = darkTheme ? "#2E86AB" : "#1E5A8E";
    QString textColorWarning = darkTheme ? "#F18F01" : "#D97706";
    QString textColorDanger = darkTheme ? "#FF6B6B" : "#DC2626";

    if (isCountdown)
    {
        if (timerSeconds <= 5 * 60)
        {
            newStyle = QString("QLabel { color: %1; background: %2; padding: 10px; border-radius: 5px; border: 2px solid %1; }")
                          .arg(textColorDanger).arg(bgColor);
            statusColor = textColorDanger;
        }
        else if (timerSeconds <= 15 * 60)
        {
            newStyle = QString("QLabel { color: %1; background: %2; padding: 10px; border-radius: 5px; border: 2px solid %1; }")
                          .arg(textColorWarning).arg(bgColor);
            statusColor = textColorWarning;
        }
        else
        {
            newStyle = QString("QLabel { color: %1; background: %2; padding: 10px; border-radius: 5px; border: 2px solid %1; }")
                          .arg(textColorNormal).arg(bgColor);
            statusColor = textColorNormal;
        }
    }
    else
    {
        newStyle = QString("QLabel { color: %1; background: %2; padding: 10px; border-radius: 5px; border: 2px solid %1; }")
                      .arg(textColorNormal).arg(bgColor);
        statusColor = textColorNormal;
    }

    if (lastTimerStyle != newStyle)
    {
        lastTimerStyle = newStyle;
        timerLabel->setStyleSheet(newStyle);
    }

    if (competitionStatusLabel)
    {
        QString modeText = isCountdown ? tr("倒计时") : tr("正计时");
        QString statusText = QString("%1: %2").arg(modeText).arg(timeText);
        competitionStatusLabel->setText(statusText);

        QString statusStyle = QString(
            "QLabel {"
            "    color: %1;"
            "    font-weight: bold;"
            "    padding: 2px 8px;"
            "    margin-right: 10px;"
            "}"
            "QLabel:hover {"
            "    background-color: rgba(46, 134, 171, 0.2);"
            "    border-radius: 3px;"
            "}"
        ).arg(statusColor);

        competitionStatusLabel->setStyleSheet(statusStyle);
    }
}

void CompilerIDE::applyCompetitionModeSettings()
{
    QString currentFeature = rightSidebar->getCurrentFeatureName();
    bool currentFeatureWillBeDisabled = false;
    QString firstEnabledFeature;

    for (const QString &name : sidebarFeatureNames)
    {
        bool disableThisFeature = false;

        if (comp_disableDeepSeek && (name.contains("AI") || name.contains("DeepSeek") || name.contains(tr("助手"))))
        {
            disableThisFeature = true;
        }
        else if (comp_disableSnippets && (name.contains(tr("片段")) || name.contains("Snippet")))
        {
            disableThisFeature = true;
        }
        else if (comp_disableDiagnostics && (name.contains(tr("诊断")) || name.contains("Diagnostic")))
        {
            disableThisFeature = true;
        }
        else if (comp_disableProjectManager && (name.contains(tr("项目")) || name.contains("Project")))
        {
            disableThisFeature = true;
        }

        rightSidebar->setFeatureEnabled(name, !disableThisFeature);

        if (name == currentFeature && disableThisFeature)
        {
            currentFeatureWillBeDisabled = true;
        }
        if (firstEnabledFeature.isEmpty() && !disableThisFeature)
        {
            firstEnabledFeature = name;
        }
    }

    if (currentFeatureWillBeDisabled && !firstEnabledFeature.isEmpty())
    {
        rightSidebar->switchToFeature(firstEnabledFeature);
        showFloatingMessage(tr("竞赛模式下已禁用“%1”功能，已自动跳转至第一个未被禁用的功能。").arg(currentFeature), 3000, true);
    }

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setCompetitionModeSettings(
                comp_disableCodeCompletion,
                comp_disableAutoBrackets,
                comp_disableAutoQuotes,
                comp_disableAutoIndent
            );
        }
    }

    if (comp_disableCodeCompletion && codeCompleter)
    {
        for (int i = 0; i < tabWidget->count(); i++)
        {
            CodeEditor *editor = editorAt(i);
            if (editor)
            {
                editor->disableAllCompletion();
            }
        }
    }

    if (comp_disableAutoBrackets)
    {
        for (int i = 0; i < tabWidget->count(); i++)
        {
            CodeEditor *editor = editorAt(i);
            if (editor)
            {
                editor->setAutoBracketsEnabled(false);
            }
        }
    }

    if (comp_disableAutoQuotes)
    {
        for (int i = 0; i < tabWidget->count(); i++)
        {
            CodeEditor *editor = editorAt(i);
            if (editor)
            {
                editor->setAutoQuotesEnabled(false);
            }
        }
    }

    if (comp_disableAutoIndent)
    {
        for (int i = 0; i < tabWidget->count(); i++)
        {
            CodeEditor *editor = editorAt(i);
            if (editor)
            {
                editor->setAutoIndentEnabled(false);
            }
        }
    }

    if (comp_disableTerminal)
    {
        terminalDock->setVisible(false);
        terminalShowAct->setEnabled(false);
    }
}

void CompilerIDE::restoreNormalModeSettings()
{
    for (const QString &name : sidebarFeatureNames)
    {
        rightSidebar->setFeatureEnabled(name, true);
    }

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setCompetitionModeSettings(false, false, false, false);
            editor->setAutoBracketsEnabled(autoBrackets);
            editor->setAutoQuotesEnabled(autoQuotes);
            editor->setAutoIndentEnabled(autoIndent);
        }
    }

    if (codeCompleter)
    {
        for (int i = 0; i < tabWidget->count(); i++)
        {
            CodeEditor *editor = editorAt(i);
            if (editor)
            {
                editor->enableAllCompletion();
            }
        }
    }

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setAutoBracketsEnabled(autoBrackets);
        }
    }

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setAutoQuotesEnabled(autoQuotes);
        }
    }

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setAutoIndentEnabled(autoIndent);
        }
    }

    terminalDock->setVisible(originalTerminalVisible);
    terminalShowAct->setEnabled(true);
}

void CompilerIDE::startCountdownTimer()
{
    if (!competitionMode)
    {
        QMessageBox::information(this, tr("竞赛模式未开启"),
                                tr("请先开启竞赛模式才能使用计时器功能。"));
        return;
    }

    bool ok;
    int minutes = QInputDialog::getInt(this, tr("设置倒计时"),
                                      tr("倒计时时间（分钟）:"), 60, 1, 300, 1, &ok);
    if (ok)
    {
        isCountdown = true;
        timerSeconds = minutes * 60;
        updateTimerDisplay();

        if (competitionTimer && !competitionTimer->isActive())
        {
            competitionTimer->start(1000);
        }

        outputEdit->appendPlainText(tr("倒计时开始: %1 分钟").arg(minutes));
    }
}

void CompilerIDE::startStopwatchTimer()
{
    if (!competitionMode)
    {
        QMessageBox::information(this, tr("竞赛模式未开启"),
                                tr("请先开启竞赛模式才能使用计时器功能。"));
        return;
    }

    isCountdown = false;
    timerSeconds = 0;
    updateTimerDisplay();

    if (competitionTimer)
    {
        competitionTimer->start(1000);
        pauseBtn->setText(tr("暂停"));
    }

    outputEdit->appendPlainText(tr("正计时开始"));
}

void CompilerIDE::stopCompetitionTimer()
{
    if (competitionTimer && competitionTimer->isActive())
    {
        competitionTimer->stop();
        outputEdit->appendPlainText(tr("计时器已停止"));
    }
}

void CompilerIDE::resetCompetitionTimer()
{
    if (competitionTimer && competitionTimer->isActive())
    {
        competitionTimer->stop();
    }

    if (isCountdown)
    {
        timerSeconds = 60 * 60;
    }
    else
    {
        timerSeconds = 0;
    }

    pauseBtn->setText(tr("开始"));

    updateTimerDisplay();
    outputEdit->appendPlainText(tr("计时器已重置"));
}


void CompilerIDE::onCompetitionModeToggled(bool enabled)
{
    if (enabled && !competitionMode)
    {
        showCompetitionModeDialog();
    }
    else if (!enabled && competitionMode)
    {
        stopCompetitionMode();
    }
}

void CompilerIDE::checkDisabledFeature(const QString &featureName)
{
    if (!competitionMode)
    {
        return;
    }

    bool showWarning = false;

    if (featureName == "codeCompletion" && comp_disableCodeCompletion)
    {
        showWarning = true;
    }
    else if (featureName == "deepSeek" && comp_disableDeepSeek)
    {
        showWarning = true;
    }
    else if (featureName == "autoBrackets" && comp_disableAutoBrackets)
    {
        showWarning = true;
    }
    else if (featureName == "autoQuotes" && comp_disableAutoQuotes)
    {
        showWarning = true;
    }
    else if (featureName == "autoIndent" && comp_disableAutoIndent)
    {
        showWarning = true;
    }

    if (showWarning)
    {
        QMessageBox::warning(this, tr("功能已禁用"),
                            tr("该功能在竞赛模式下已被禁用，无法使用。\n\n如需使用此功能，请先退出竞赛模式。"));
    }
}

QString CompilerIDE::findDebuggerFromCompilerPath(const QString &compilerPath)
{
    if (compilerPath.isEmpty())
    {
        return QString();
    }

    QFileInfo compilerInfo(compilerPath);
    QString compilerDir = compilerInfo.absolutePath();

    QString gdbPath = compilerDir + "/gdb.exe";
    if (QFile::exists(gdbPath))
    {
        qDebug() << "在编译器目录找到gdb:" << gdbPath;
        return QDir::toNativeSeparators(gdbPath);
    }

    gdbPath = compilerDir + "/gdb";
    if (QFile::exists(gdbPath))
    {
        qDebug() << "在编译器目录找到gdb:" << gdbPath;
        return QDir::toNativeSeparators(gdbPath);
    }

    if (compilerDir.endsWith("/bin", Qt::CaseInsensitive) ||
            compilerDir.endsWith("\\bin", Qt::CaseInsensitive))
    {

    }
    else
    {
        gdbPath = compilerDir + "/bin/gdb.exe";
        if (QFile::exists(gdbPath))
        {
            qDebug() << "在bin子目录找到gdb:" << gdbPath;
            return QDir::toNativeSeparators(gdbPath);
        }
        gdbPath = compilerDir + "/bin/gdb";
        if (QFile::exists(gdbPath))
        {
            qDebug() << "在bin子目录找到gdb:" << gdbPath;
            return QDir::toNativeSeparators(gdbPath);
        }
    }

    QDir parentDir(compilerDir);
    if (parentDir.cdUp())
    {
        gdbPath = parentDir.absolutePath() + "/bin/gdb.exe";
        if (QFile::exists(gdbPath))
        {
            qDebug() << "在父目录bin中找到gdb:" << gdbPath;
            return QDir::toNativeSeparators(gdbPath);
        }
        gdbPath = parentDir.absolutePath() + "/bin/gdb";
        if (QFile::exists(gdbPath))
        {
            qDebug() << "在父目录bin中找到gdb:" << gdbPath;
            return QDir::toNativeSeparators(gdbPath);
        }
    }

    QDirIterator it(compilerDir, QStringList() << "gdb.exe" << "gdb",
                    QDir::Files, QDirIterator::Subdirectories);
    if (it.hasNext())
    {
        QString foundPath = it.next();
        qDebug() << "递归搜索找到gdb:" << foundPath;
        return QDir::toNativeSeparators(foundPath);
    }

    int mingwIndex = compilerPath.indexOf("mingw", 0, Qt::CaseInsensitive);
    if (mingwIndex != -1)
    {
        int endIndex = compilerPath.indexOf('/', mingwIndex);
        if (endIndex == -1)
        {
            endIndex = compilerPath.indexOf('\\', mingwIndex);
        }
        if (endIndex != -1)
        {
            QString mingwRoot = compilerPath.left(endIndex);
            QDirIterator mingwIt(mingwRoot, QStringList() << "gdb.exe" << "gdb",
                                 QDir::Files, QDirIterator::Subdirectories);
            if (mingwIt.hasNext())
            {
                QString foundPath = mingwIt.next();
                qDebug() << "在mingw根目录找到gdb:" << foundPath;
                return QDir::toNativeSeparators(foundPath);
            }
        }
    }

    qDebug() << "未能从编译器路径检测到gdb";
    return QString();
}

QString CompilerIDE::autoDetectDebuggerPath()
{
    QString detectedPath = findDebuggerFromCompilerPath(compilerPath);
    if (!detectedPath.isEmpty())
    {
        return detectedPath;
    }

    QStringList commonPaths =
    {
        "C:/MinGW/bin/gdb.exe",
        "C:/mingw64/bin/gdb.exe",
        "C:/mingw32/bin/gdb.exe",
        "C:/msys64/mingw64/bin/gdb.exe",
        "C:/msys64/mingw32/bin/gdb.exe",
        "C:/msys64/usr/bin/gdb.exe",
        "C:/Program Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin/gdb.exe",
        "C:/TDM-GCC-64/bin/gdb.exe",
        "C:/TDM-GCC-32/bin/gdb.exe",
        QCoreApplication::applicationDirPath() + "/mingw/bin/gdb.exe",
        QCoreApplication::applicationDirPath() + "/mingw64/bin/gdb.exe"
    };

    QString pathEnv = qEnvironmentVariable("PATH");
    QStringList pathDirs = pathEnv.split(';', Qt::SkipEmptyParts);
    for (const QString &dir : pathDirs)
    {
        QString gdbPath = dir + "/gdb.exe";
        if (!commonPaths.contains(gdbPath))
        {
            commonPaths.append(gdbPath);
        }
    }

    for (const QString &path : commonPaths)
    {
        if (QFile::exists(path))
        {
            qDebug() << "自动检测到gdb:" << path;
            return QDir::toNativeSeparators(path);
        }
    }

    qDebug() << "使用系统默认gdb";
#ifdef Q_OS_WIN
    return "gdb.exe";
#else
    return "gdb";
#endif
}

bool CompilerIDE::isInCommentOrString(const QString &text, int position)
{
    if (position < 0 || position >= text.length())
    {
        return false;
    }

    int lineStart = text.lastIndexOf('\n', position);
    if (lineStart == -1)
    {
        lineStart = 0;
    }
    else
    {
        lineStart++;
    }

    QString line = text.mid(lineStart, position - lineStart);
    if (line.contains("//"))
    {
        int commentPos = line.indexOf("//");
        if (commentPos < position - lineStart)
        {
            return true;
        }
    }

    int commentStart = text.lastIndexOf("/*", position);
    if (commentStart != -1)
    {
        int commentEnd = text.indexOf("*/", commentStart);
        if (commentEnd == -1 || commentEnd > position)
        {
            return true;
        }
    }

    int quoteCount = 0;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    bool escaped = false;

    for (int i = 0; i <= position && i < text.length(); ++i)
    {
        QChar c = text[i];

        if (escaped)
        {
            escaped = false;
            continue;
        }

        if (c == '\\')
        {
            escaped = true;
            continue;
        }

        if (c == '\'' && !inDoubleQuote)
        {
            inSingleQuote = !inSingleQuote;
        }
        else if (c == '"' && !inSingleQuote)
        {
            inDoubleQuote = !inDoubleQuote;
        }
    }

    return inSingleQuote || inDoubleQuote;
}

void CompilerIDE::openFeedbackPage()
{
    QUrl url("https://github.com/CompilerIDE/CompilerIDE.github.io/issues");
    if (!QDesktopServices::openUrl(url))
    {
        QMessageBox::warning(this, tr("打开失败"),
                             tr("无法打开浏览器，请手动访问：\n%1").arg(url.toString()));
    }
}

void CompilerIDE::createStatusBar()
{
    statusBar()->showMessage(tr("就绪"));

    competitionStatusLabel = new QLabel();
    competitionStatusLabel->setVisible(false);
    competitionStatusLabel->setCursor(Qt::PointingHandCursor);
    competitionStatusLabel->setToolTip(tr("点击显示竞赛面板"));
    competitionStatusLabel->setStyleSheet(
        "QLabel {"
        "    color: #2E86AB;"
        "    font-weight: bold;"
        "    padding: 2px 8px;"
        "    margin-right: 10px;"
        "}"
        "QLabel:hover {"
        "    background-color: rgba(46, 134, 171, 0.2);"
        "    border-radius: 3px;"
        "}"
    );

    competitionStatusLabel->installEventFilter(this);

    statusBar()->addPermanentWidget(competitionStatusLabel);
}

void CompilerIDE::createDockWindows()
{
    outputDock = new QDockWidget(tr("输出"), this);
    outputDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    outputEdit = new QPlainTextEdit;

    QFont outputFont("Consolas", 11, QFont::Bold);
    outputEdit->setFont(outputFont);

    outputEdit->setReadOnly(true);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    outputDock->setWidget(outputEdit);

    addDockWidget(Qt::BottomDockWidgetArea, outputDock);

    errorDock = new QDockWidget(tr("错误"), this);
    errorDock->setAllowedAreas(Qt::BottomDockWidgetArea);

    errorTableWidget = new QTableWidget;
    errorTableWidget->setColumnCount(4);
    errorTableWidget->setHorizontalHeaderLabels(QStringList()
        << tr("单元") << tr("行号") << tr("列号") << tr("错误信息"));

    errorTableWidget->setColumnWidth(0, 300);
    errorTableWidget->setColumnWidth(1, 60);
    errorTableWidget->setColumnWidth(2, 60);
    errorTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    errorTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    errorTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    errorTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    errorTableWidget->setAlternatingRowColors(true);
    errorTableWidget->horizontalHeader()->setStretchLastSection(true);

    if (darkTheme)
    {
        errorTableWidget->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "    background-color: #2D3047;"
            "    color: #E0E0E0;"
            "    padding: 4px;"
            "    border: 1px solid #5A5F7A;"
            "}"
        );
    }
    else
    {
        errorTableWidget->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "    background-color: #E0E0E0;"
            "    color: #333333;"
            "    padding: 4px;"
            "    border: 1px solid #CCCCCC;"
            "}"
        );
    }

    connect(errorTableWidget, &QTableWidget::cellClicked, [this](int row, int column)
    {
        this->showErrorInCode(row, column);
    });

    errorDock->setWidget(errorTableWidget);
    addDockWidget(Qt::BottomDockWidgetArea, errorDock);

    tabifyDockWidget(outputDock, errorDock);
    errorDock->raise();

    projectManager = new ProjectManager(this);
    projectDock = new QDockWidget(tr("项目管理器"), this);
    projectDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    projectDock->setWidget(projectManager);
    addDockWidget(Qt::RightDockWidgetArea, projectDock);
    projectDock->setVisible(false);

    connect(projectManager, &ProjectManager::fileSelected, this, &CompilerIDE::onFileSelected);

    fileBrowserDock = new QDockWidget(tr("文件浏览器"), this);
    fileBrowserDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    fileBrowserDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, fileBrowserDock);

    symbolBrowserDock = new QDockWidget(tr("符号浏览器"), this);
    symbolBrowserDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    symbolBrowserDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, symbolBrowserDock);

    outlineDock = new QDockWidget(tr("大纲视图"), this);
    outlineDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outlineDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, outlineDock);

    if (viewMenu)
    {
        viewMenu->addAction(outputDock->toggleViewAction());
        viewMenu->addAction(errorDock->toggleViewAction());
    }
    else
    {
        qWarning() << "viewMenu is not initialized!";
    }
}

void CompilerIDE::createFindDialog()
{
    findDialog = new QDialog(this);
    findDialog->setWindowTitle(tr("查找和替换"));
    findDialog->setModal(false);

    QVBoxLayout *layout = new QVBoxLayout;

    QFormLayout *formLayout = new QFormLayout;

    findEdit = new QLineEdit;
    replaceEdit = new QLineEdit;

    formLayout->addRow(tr("查找:"), findEdit);
    formLayout->addRow(tr("替换为:"), replaceEdit);

    QHBoxLayout *optionsLayout = new QHBoxLayout;
    caseSensitiveCheck = new QCheckBox(tr("区分大小写"));
    wholeWordCheck = new QCheckBox(tr("全字匹配"));
    regexCheck = new QCheckBox(tr("正则表达式"));

    optionsLayout->addWidget(caseSensitiveCheck);
    optionsLayout->addWidget(wholeWordCheck);
    optionsLayout->addWidget(regexCheck);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    findNextButton = new QPushButton(tr("查找下一个"));
    findPrevButton = new QPushButton(tr("查找上一个"));
    replaceButton = new QPushButton(tr("替换"));
    replaceAllButton = new QPushButton(tr("全部替换"));
    QPushButton *closeButton = new QPushButton(tr("关闭"));

    buttonLayout->addWidget(findNextButton);
    buttonLayout->addWidget(findPrevButton);
    buttonLayout->addWidget(replaceButton);
    buttonLayout->addWidget(replaceAllButton);
    buttonLayout->addWidget(closeButton);

    layout->addLayout(formLayout);
    layout->addLayout(optionsLayout);
    layout->addLayout(buttonLayout);

    findDialog->setLayout(layout);

    connect(findNextButton, &QPushButton::clicked, this, &CompilerIDE::findNext);
    connect(findPrevButton, &QPushButton::clicked, this, &CompilerIDE::findPrevious);
    connect(replaceButton, &QPushButton::clicked, [this]()
    {
        if (currentEditor())
        {
            QTextCursor cursor = currentEditor()->textCursor();
            if (cursor.hasSelection())
            {
                cursor.insertText(replaceEdit->text());
            }
            findNext();
        }
    });
    connect(replaceAllButton, &QPushButton::clicked, [this]()
    {
        if (!currentEditor())
        {
            return;
        }

        QTextCursor cursor = currentEditor()->textCursor();
        cursor.movePosition(QTextCursor::Start);
        currentEditor()->setTextCursor(cursor);

        int count = 0;
        while (performFind(true, false))
        {
            cursor = currentEditor()->textCursor();
            cursor.insertText(replaceEdit->text());
            count++;
        }

        QMessageBox::information(this, tr("替换完成"), tr("已替换 %1 个匹配项").arg(count));
    });
    connect(closeButton, &QPushButton::clicked, findDialog, &QDialog::hide);
}

void CompilerIDE::formatAllCode()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        QMessageBox::warning(this, tr("警告"), tr("没有打开的文件"));
        return;
    }

    if (competitionMode)
    {
        QMessageBox::warning(this, tr("警告"),
                             tr("竞赛模式下不支持代码格式化功能"));
        return;
    }

    bool originalCodeBeautify = editor->isCodeBeautifyEnabled();

    if (!originalCodeBeautify)
    {
        editor->setCodeBeautifyEnabled(true);
    }

    QTextCursor originalCursor = editor->textCursor();
    int originalPosition = originalCursor.position();

    QTextCursor cursor(editor->document());
    cursor.beginEditBlock();

    cursor.movePosition(QTextCursor::Start);
    int totalLines = editor->document()->blockCount();
    int formattedLines = 0;

    for (int i = 0; i < totalLines; ++i)
    {
        QTextBlock block = editor->document()->findBlockByNumber(i);
        if (!block.isValid())
        {
            continue;
        }

        QString lineText = block.text();
        QString trimmedText = lineText.trimmed();

        bool isPreprocessor = false;
        if (trimmedText.startsWith("#"))
        {
            if (trimmedText.length() > 1)
            {
                QChar nextChar = trimmedText[1];
                if (nextChar.isLetter())
                {
                    isPreprocessor = true;
                }
            }
        }

        if (isPreprocessor)
        {
            continue;
        }

        if (trimmedText.isEmpty() ||
                trimmedText.startsWith("//") ||
                trimmedText.startsWith("/*") ||
                trimmedText.startsWith("*"))
        {
            continue;
        }

        QString indent;
        for (int j = 0; j < lineText.length(); ++j)
        {
            if (lineText[j].isSpace())
            {
                indent += lineText[j];
            }
            else
            {
                break;
            }
        }

        QString beautified = editor->beautifyCode(trimmedText, true);

        if (beautified != trimmedText)
        {
            cursor.setPosition(block.position());
            cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            cursor.insertText(indent + beautified);
            formattedLines++;

            block = editor->document()->findBlockByNumber(i);
        }
    }

    cursor.endEditBlock();

    if (!originalCodeBeautify)
    {
        editor->setCodeBeautifyEnabled(false);
    }

    originalCursor.setPosition(qMin(originalPosition, editor->document()->characterCount() - 1));
    editor->setTextCursor(originalCursor);

    outputEdit->appendPlainText(tr("√ 代码格式化完成"));
    statusBar()->showMessage(tr("代码格式化完成"), 3000);
}

void CompilerIDE::writeSettings()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");

    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("compilerPath", compilerPath);
    settings.setValue("debuggerPath", debuggerPath);
    settings.setValue("autoBrackets", autoBrackets);
    settings.setValue("autoQuotes", autoQuotes);
    settings.setValue("codeBeautify", codeBeautify);
    settings.setValue("editorFontFamily", editorFont.family());
    settings.setValue("editorFontSize", editorFont.pointSize());
    settings.setValue("codeCompletion", codeCompletionEnabled);
    settings.setValue("autoIndent", autoIndent);
    settings.setValue("indentSize", indentSize);
    settings.setValue("codeFolding", codeFolding);
    settings.setValue("lineNumbers", lineNumbers);
    settings.setValue("darkTheme", darkTheme);
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("cppStandard", cppStandard);
    settings.setValue("optimizationLevel", optimizationLevel);

    qDebug() << "设置已保存 - 编译器路径:" << compilerPath;
    qDebug() << "调试器路径:" << debuggerPath;
    qDebug() << "[WriteSettings] 保存字体:" << editorFont.family() << editorFont.pointSize();
}

void CompilerIDE::readSettings()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");

    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(1000, 700)).toSize();
    resize(size);
    move(pos);

    QString savedCompilerPath = settings.value("compilerPath", "").toString();

    if (!savedCompilerPath.isEmpty() && QFile::exists(savedCompilerPath))
    {
        compilerPath = savedCompilerPath;
        qDebug() << "使用用户保存的编译器路径:" << compilerPath;
    }
    else
    {
        QString installedCompilerPath = "";
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/compiler_path.conf";
        QFile configFile(configPath);
        if (configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&configFile);
            installedCompilerPath = in.readLine().trimmed();
            configFile.close();
        }
        if (installedCompilerPath.isEmpty() || !QFile::exists(installedCompilerPath))
        {
            QString defaultPath = QCoreApplication::applicationDirPath() + "/mingw/bin/g++.exe";
            if (QFile::exists(defaultPath))
            {
                installedCompilerPath = defaultPath;
            }
        }
        if (installedCompilerPath.isEmpty() || !QFile::exists(installedCompilerPath))
        {
#ifdef Q_OS_WIN
            installedCompilerPath = "g++.exe";
#else
            installedCompilerPath = "g++";
#endif
        }

        compilerPath = installedCompilerPath;
        qDebug() << "使用自动检测的编译器路径:" << compilerPath;
    }

    QString savedDebuggerPath = settings.value("debuggerPath", "").toString();
    if (!savedDebuggerPath.isEmpty() && QFile::exists(savedDebuggerPath))
    {
        debuggerPath = savedDebuggerPath;
        qDebug() << "使用用户保存的调试器路径:" << debuggerPath;
    }
    else
    {
        debuggerPath = findDebuggerFromCompilerPath(compilerPath);
        if (debuggerPath.isEmpty())
        {
            debuggerPath = autoDetectDebuggerPath();
        }

        qDebug() << "使用自动检测的调试器路径:" << debuggerPath;
    }

    autoBrackets = settings.value("autoBrackets", true).toBool();
    autoQuotes = settings.value("autoQuotes", true).toBool();
    codeBeautify = settings.value("codeBeautify", true).toBool();
    codeCompletionEnabled = settings.value("codeCompletion", true).toBool();
    QString fontFamily = settings.value("editorFontFamily", "Consolas").toString();
    int fontSize = settings.value("editorFontSize", 11).toInt();
    qDebug() << "[readSettings] 读取字体:" << fontFamily << fontSize;
    editorFont = QFont(fontFamily, fontSize);
    autoIndent = settings.value("autoIndent", true).toBool();
    indentSize = settings.value("indentSize", 4).toInt();
    codeFolding = settings.value("codeFolding", true).toBool();
    lineNumbers = settings.value("lineNumbers", true).toBool();
    darkTheme = settings.value("darkTheme", true).toBool();
    recentFiles = settings.value("recentFiles", QStringList()).toStringList();
    cppStandard = settings.value("cppStandard", "c++17").toString();
    int standardIndex = cppStandardCombo->findText(cppStandard.toUpper());
    if (standardIndex >= 0)
    {
        cppStandardCombo->setCurrentIndex(standardIndex);
    }

    optimizationLevel = settings.value("optimizationLevel", "-O0").toString();
    int optIndex = optimizationCombo->findText(optimizationLevel.mid(1));
    if (optIndex >= 0)
    {
        optimizationCombo->setCurrentIndex(optIndex);
    }
    setTheme(darkTheme);
    compilerType = detectCompilerType(compilerPath);
    checkCompilerSupport();
    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setAutoBracketsEnabled(autoBrackets);
            editor->setAutoQuotesEnabled(autoQuotes);
            editor->setAutoIndentEnabled(autoIndent);
            editor->setIndentSize(indentSize);
            editor->setCodeFoldingEnabled(codeFolding);
            editor->setLineNumbersEnabled(lineNumbers);
            editor->setDarkThemeEnabled(darkTheme);
            editor->setFont(editorFont);
            if (codeCompletionEnabled)
            {
                editor->setCompletionEnabled(true);
                editor->setCompetitionMode(false);
            }
            else
            {
                editor->setCompletionEnabled(false);
                editor->setCompetitionMode(true);
            }

            if (competitionMode && comp_disableCodeCompletion)
            {
                editor->disableAllCompletion();
            }
            else
            {
                editor->enableAllCompletion();
            }
        }
    }

    updateStatusBar();

    bool needReopen = settings.value("needReopen", false).toBool();
    if (needReopen)
    {
        QStringList openFiles = settings.value("openFilesOnRestart").toStringList();
        int savedIndex = settings.value("currentIndexOnRestart", 0).toInt();

        qDebug() << "检测到重启标记，恢复文件:" << openFiles;

        QTimer::singleShot(200, this, [this, openFiles, savedIndex]()
        {
            for (const QString &filePath : openFiles)
            {
                if (QFile::exists(filePath))
                {
                    loadFile(filePath);
                }
            }

            if (savedIndex >= 0 && savedIndex < tabWidget->count())
            {
                tabWidget->setCurrentIndex(savedIndex);
            }

            statusBar()->showMessage(tr("已恢复之前打开的文件"), 3000);
        });

        settings.setValue("needReopen", false);
        settings.remove("openFilesOnRestart");
        settings.remove("currentIndexOnRestart");
        settings.sync();
    }

    qDebug() << "设置加载完成 - 编译器路径:" << compilerPath;
    qDebug() << "调试器路径:" << debuggerPath;
    qDebug() << "主题模式:" << (darkTheme ? "深色" : "浅色");
    qDebug() << "最近文件数:" << recentFiles.count();
    qDebug() << "[readSettings] 设置后 editorFont:" << editorFont.family() << editorFont.pointSize();
}

void CompilerIDE::changeOptimizationLevel(int index)
{
    QString level = optimizationCombo->itemText(index);
    optimizationLevel = "-" + level;
    qDebug() << "优化级别已更改为:" << optimizationLevel;
}

bool CompilerIDE::maybeSave()
{
    CodeEditor *editor = currentEditor();
    if (!editor || !editor->document()->isModified())
    {
        return true;
    }

    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Compiler IDE"),
                               tr("文档已被修改。\n"
                                  "是否要保存更改？"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

bool CompilerIDE::maybeSaveAll()
{
    QList<QPair<int, CodeEditor*>> modifiedEditors;

    for (int i = 0; i < tabWidget->count(); i++)
    {
        CodeEditor *editor = editorAt(i);
        if (editor && editor->document()->isModified())
        {
            modifiedEditors.append(qMakePair(i, editor));
        }
    }

    if (modifiedEditors.isEmpty())
    {
        return true;
    }

    for (const auto &pair : modifiedEditors)
    {
        int index = pair.first;
        CodeEditor *editor = pair.second;

        tabWidget->setCurrentIndex(index);
        editor->setFocus();
        editor->closeCompletionPopup();

        QString fileName = tabWidget->tabText(index);
        if (fileName.endsWith("*"))
        {
            fileName.chop(1);
        }
        if (fileName.isEmpty())
        {
            fileName = QString("untitled %1").arg(index + 1);
        }

        QString filePath = tabWidget->tabToolTip(index);
        bool isNewFile = filePath.isEmpty();

        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr("提示"));
        msgBox.setText(tr("文件 %1 未保存，是否保存？").arg(fileName));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setButtonText(QMessageBox::Save, tr("保存"));
        msgBox.setButtonText(QMessageBox::Discard, tr("不保存"));
        msgBox.setButtonText(QMessageBox::Cancel, tr("取消"));

        int ret = msgBox.exec();

        if (ret == QMessageBox::Save)
        {
            bool saved;

            if (isNewFile)
            {
                saved = saveAs();
            }
            else
            {
                saved = saveFile(filePath);
                if (saved)
                {
                    editor->document()->setModified(false);
                    QString cleanTitle = tabWidget->tabText(index);
                    if (cleanTitle.endsWith("*"))
                    {
                        cleanTitle.chop(1);
                        tabWidget->setTabText(index, cleanTitle);
                    }
                }
            }

            if (!saved)
            {
                return false;
            }
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

void CompilerIDE::createSampleTesterDock()
{
    sampleTesterDock = new QDockWidget(tr("数据评测器"), this);
    sampleTesterDock->setObjectName("SampleTesterDock");
    sampleTesterDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    sampleTesterWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(sampleTesterWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *timeLimitLabel = new QLabel(tr("时间限制(ms):"));
    timeLimitSpinBox = new QSpinBox();
    timeLimitSpinBox->setRange(100, 10000);
    timeLimitSpinBox->setValue(1000);
    timeLimitSpinBox->setSingleStep(100);
    timeLimitSpinBox->setMinimumWidth(100);

    addTestBtn = new QPushButton(tr("添加测试点"));
    runAllTestsBtn = new QPushButton(tr("评测所有测试点"));

    controlLayout->addWidget(timeLimitLabel);
    controlLayout->addWidget(timeLimitSpinBox);
    controlLayout->addStretch();
    controlLayout->addWidget(addTestBtn);
    controlLayout->addWidget(runAllTestsBtn);

    overallResultLabel = new QLabel();
    overallResultLabel->setAlignment(Qt::AlignCenter);
    overallResultLabel->setMinimumHeight(40);
    overallResultLabel->setStyleSheet(
        "QLabel { "
        "border-radius: 5px; "
        "padding: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "}"
    );
    overallResultLabel->hide();

    testCasesScrollArea = new QScrollArea();
    testCasesScrollArea->setWidgetResizable(true);
    testCasesScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget *testCasesContainer = new QWidget();
    testCasesLayout = new QVBoxLayout(testCasesContainer);
    testCasesLayout->setSpacing(10);
    testCasesLayout->addStretch();

    testCasesScrollArea->setWidget(testCasesContainer);

    mainLayout->addWidget(controlWidget);
    mainLayout->addWidget(overallResultLabel);
    mainLayout->addWidget(testCasesScrollArea);

    sampleTesterDock->setWidget(sampleTesterWidget);
    addDockWidget(Qt::RightDockWidgetArea, sampleTesterDock);
    sampleTesterDock->hide();

    connect(addTestBtn, &QPushButton::clicked, this, &CompilerIDE::addTestCase);
    connect(runAllTestsBtn, &QPushButton::clicked, this, &CompilerIDE::runAllTests);
}

void CompilerIDE::showSampleTester()
{
    if (sampleTesterDock)
    {
        sampleTesterDock->show();
        sampleTesterDock->raise();
    }
}

QWidget* CompilerIDE::createTestCaseWidget(int index)
{
    QWidget *testWidget = new QWidget();
    testWidget->setStyleSheet(
        "QWidget { "
        "background-color: palette(base); "
        "border: 1px solid palette(mid); "
        "border-radius: 8px; "
        "padding: 10px; "
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(testWidget);
    layout->setSpacing(8);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel(tr("测试点 #%1").arg(index + 1));
    titleLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: palette(text);");

    QLabel *statusLabel = new QLabel(tr("未测试"));
    statusLabel->setObjectName(QString("statusLabel_%1").arg(index));
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setMinimumWidth(80);
    statusLabel->setStyleSheet(
        "QLabel { "
        "background-color: #808080; "
        "color: white; "
        "border-radius: 3px; "
        "padding: 4px 8px; "
        "font-size: 11px; "
        "}"
    );

    QPushButton *runBtn = new QPushButton(tr("评测"));
    runBtn->setMaximumWidth(60);
    runBtn->setProperty("testIndex", index);
    connect(runBtn, &QPushButton::clicked, this, &CompilerIDE::runSingleTest);

    QPushButton *removeBtn = new QPushButton(tr("删除"));
    removeBtn->setMaximumWidth(60);
    removeBtn->setProperty("testIndex", index);
    connect(removeBtn, &QPushButton::clicked, this, &CompilerIDE::removeTestCase);

    QPushButton *collapseBtn = new QPushButton("▼");
    collapseBtn->setObjectName("collapseBtn");
    collapseBtn->setMaximumWidth(24);
    collapseBtn->setMaximumHeight(24);
    collapseBtn->setFlat(true);
    collapseBtn->setToolTip(tr("折叠/展开"));
    collapseBtn->setStyleSheet(
        "QPushButton { "
        "font-size: 10px; "
        "border: none; "
        "padding: 2px; "
        "}"
        "QPushButton:hover { "
        "background-color: palette(mid); "
        "border-radius: 3px; "
        "}"
    );
    collapseBtn->setProperty("testIndex", index);
    connect(collapseBtn, &QPushButton::clicked, this, &CompilerIDE::toggleTestCaseCollapse);

    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(statusLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(runBtn);
    headerLayout->addWidget(removeBtn);
    headerLayout->addWidget(collapseBtn);

    QWidget *contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(8);

    QLabel *inputLabel = new QLabel(tr("输入:"));
    inputLabel->setStyleSheet("font-weight: bold; color: palette(text);");
    QTextEdit *inputEdit = new QTextEdit();
    inputEdit->setObjectName(QString("inputEdit_%1").arg(index));
    inputEdit->setMaximumHeight(80);
    inputEdit->setPlaceholderText(tr("输入测试数据..."));
    inputEdit->setAcceptRichText(false);
    inputEdit->setStyleSheet(
        "QTextEdit { "
        "background-color: palette(base); "
        "color: palette(text); "
        "border: 1px solid palette(mid); "
        "}"
    );

    QLabel *expectedLabel = new QLabel(tr("期望输出:"));
    expectedLabel->setStyleSheet("font-weight: bold; color: palette(text);");
    QTextEdit *expectedEdit = new QTextEdit();
    expectedEdit->setObjectName(QString("expectedEdit_%1").arg(index));
    expectedEdit->setMaximumHeight(80);
    expectedEdit->setPlaceholderText(tr("输入期望输出..."));
    expectedEdit->setAcceptRichText(false);
    expectedEdit->setStyleSheet(
        "QTextEdit { "
        "background-color: palette(base); "
        "color: palette(text); "
        "border: 1px solid palette(mid); "
        "}"
    );

    QLabel *actualLabel = new QLabel(tr("实际输出:"));
    actualLabel->setStyleSheet("font-weight: bold; color: palette(text);");
    QTextEdit *actualEdit = new QTextEdit();
    actualEdit->setObjectName(QString("actualEdit_%1").arg(index));
    actualEdit->setMaximumHeight(80);
    actualEdit->setReadOnly(true);
    actualEdit->setPlaceholderText(tr("运行后显示实际输出..."));
    actualEdit->setStyleSheet(
        "QTextEdit { "
        "background-color: palette(window); "
        "color: palette(text); "
        "border: 1px solid palette(mid); "
        "}"
    );

    contentLayout->addWidget(inputLabel);
    contentLayout->addWidget(inputEdit);
    contentLayout->addWidget(expectedLabel);
    contentLayout->addWidget(expectedEdit);
    contentLayout->addWidget(actualLabel);
    contentLayout->addWidget(actualEdit);

    layout->addLayout(headerLayout);
    layout->addWidget(contentWidget);

    return testWidget;
}

void CompilerIDE::toggleTestCaseCollapse()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
    {
        return;
    }

    int index = btn->property("testIndex").toInt();
    if (index < 0 || index >= testCaseWidgets.size())
    {
        return;
    }

    QWidget *testWidget = testCaseWidgets[index];
    QWidget *contentWidget = testWidget->findChild<QWidget*>("contentWidget");

    if (contentWidget)
    {
        bool isCollapsed = !contentWidget->isVisible();
        contentWidget->setVisible(isCollapsed);
        btn->setText(isCollapsed ? "▼" : "▶");
    }
}

void CompilerIDE::setTestCaseCollapsed(int index, bool collapsed)
{
    if (index < 0 || index >= testCaseWidgets.size())
    {
        return;
    }

    QWidget *testWidget = testCaseWidgets[index];
    QWidget *contentWidget = testWidget->findChild<QWidget*>("contentWidget");
    QPushButton *collapseBtn = testWidget->findChild<QPushButton*>("collapseBtn");

    if (contentWidget && collapseBtn)
    {
        contentWidget->setVisible(!collapsed);
        collapseBtn->setText(collapsed ? "▶" : "▼");
    }
}

void CompilerIDE::addTestCase()
{
    int index = testCaseWidgets.size();
    QWidget *testWidget = createTestCaseWidget(index);

    testCasesLayout->insertWidget(testCasesLayout->count() - 1, testWidget);
    testCaseWidgets.append(testWidget);

    TestResult result;
    result.status = "PENDING";
    result.timeMs = 0;
    result.actualOutput = "";
    testResults.append(result);

    QTimer::singleShot(50, this, [this, testWidget]()
    {
        if (testCasesScrollArea && testCasesScrollArea->widget())
        {
            QPoint widgetPos = testWidget->mapTo(testCasesScrollArea->widget(), QPoint(0, 0));
            testCasesScrollArea->verticalScrollBar()->setValue(widgetPos.y());
        }
    });
}

void CompilerIDE::removeTestCase()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
    {
        return;
    }

    int index = btn->property("testIndex").toInt();
    if (index < 0 || index >= testCaseWidgets.size())
    {
        return;
    }

    QWidget *widget = testCaseWidgets[index];
    testCasesLayout->removeWidget(widget);
    widget->deleteLater();
    testCaseWidgets.removeAt(index);
    testResults.removeAt(index);

    if (testCaseWidgets.isEmpty())
    {
        overallResultLabel->hide();
    }

    for (int i = index; i < testCaseWidgets.size(); i++)
    {
        QWidget *w = testCaseWidgets[i];
        QLabel *titleLabel = w->findChild<QLabel*>();
        if (titleLabel)
        {
            titleLabel->setText(tr("测试点 #%1").arg(i + 1));
        }

        QList<QPushButton*> buttons = w->findChildren<QPushButton*>();
        for (QPushButton *button : buttons)
        {
            if (button->property("testIndex").isValid())
            {
                button->setProperty("testIndex", i);
            }
        }

        QTextEdit *inputEdit = w->findChild<QTextEdit*>(QString("inputEdit_%1").arg(i + 1));
        if (inputEdit)
        {
            inputEdit->setObjectName(QString("inputEdit_%1").arg(i));
        }

        QTextEdit *expectedEdit = w->findChild<QTextEdit*>(QString("expectedEdit_%1").arg(i + 1));
        if (expectedEdit)
        {
            expectedEdit->setObjectName(QString("expectedEdit_%1").arg(i));
        }

        QTextEdit *actualEdit = w->findChild<QTextEdit*>(QString("actualEdit_%1").arg(i + 1));
        if (actualEdit)
        {
            actualEdit->setObjectName(QString("actualEdit_%1").arg(i));
        }

        QLabel *statusLabel = w->findChild<QLabel*>(QString("statusLabel_%1").arg(i + 1));
        if (statusLabel)
        {
            statusLabel->setObjectName(QString("statusLabel_%1").arg(i));
        }
    }
}

void CompilerIDE::runSingleTest()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn)
    {
        return;
    }

    currentTestIndex = btn->property("testIndex").toInt();
    if (currentTestIndex < 0 || currentTestIndex >= testCaseWidgets.size())
    {
        return;
    }

    isRunningAllTests = false;

    updateTestCaseResult(currentTestIndex, "JUDGING", 0, "");
    overallResultLabel->hide();

    silentCompileForTest();
}

void CompilerIDE::runAllTests()
{
    if (testCaseWidgets.isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请先添加测试点"));
        return;
    }

    currentTestIndex = 0;
    isRunningAllTests = true;

    for (int i = 0; i < testResults.size(); i++)
    {
        testResults[i].status = "PENDING";
        testResults[i].timeMs = 0;
        testResults[i].actualOutput = "";
        updateTestCaseResult(i, "PENDING", 0, "");
    }

    overallResultLabel->setText(tr("Judging..."));
    overallResultLabel->setStyleSheet(
        "QLabel { "
        "background-color: #FFA500; "
        "color: white; "
        "border-radius: 5px; "
        "padding: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "}"
    );
    overallResultLabel->show();

    silentCompileForTest();
}

void CompilerIDE::silentCompileForTest()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        QMessageBox::warning(this, tr("错误"), tr("没有打开的文件"));
        return;
    }

    isSilentCompiling = true;

    accumulatedErrors.clear();
    accumulatedWarnings.clear();
    errorTableWidget->setRowCount(0);

    if (!QFile::exists(compilerPath))
    {
        QString errorMsg = tr("编译器路径不存在: %1").arg(compilerPath);
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        updateErrorTable(accumulatedErrors, accumulatedWarnings);

        for (int i = 0; i < testCaseWidgets.size(); i++)
        {
            testResults[i].status = "CE";
            updateTestCaseResult(i, "CE", 0, "");
        }
        updateOverallResult();
        isSilentCompiling = false;
        return;
    }

    QString tempDir = QDir::tempPath() + "/CompilerIDE_Test_" +
                      QString::number(QDateTime::currentMSecsSinceEpoch());
    QDir().mkpath(tempDir);

    QString tempSourcePath = tempDir + "/temp_source.cpp";
    QFile tempSource(tempSourcePath);
    if (!tempSource.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("错误"), tr("无法创建临时文件"));
        isSilentCompiling = false;
        return;
    }

    QTextStream out(&tempSource);
    out.setEncoding(QStringConverter::Utf8);
    out << editor->toPlainText();
    tempSource.close();

    QStringList args;
    QString outputExe = tempDir + "/test_program.exe";
    testExecutablePath = outputExe;

    if (compilerType == "msvc")
    {
        args << "/std:c++17" << "/O2" << "/EHsc" << "/nologo";
        args << "/Fe:" + outputExe;
        args << tempSourcePath;
    }
    else
    {
        args << "-std=c++17" << "-O2" << "-static" << "-o" << outputExe << tempSourcePath;
        args << "-lstdc++";
    }

    QProcess *silentCompileProcess = new QProcess(this);
    silentCompileProcess->setWorkingDirectory(tempDir);

    connect(silentCompileProcess, &QProcess::errorOccurred, this,
        [this, silentCompileProcess](QProcess::ProcessError error)
    {
            Q_UNUSED(error);
            isSilentCompiling = false;

            if (isRunningAllTests)
            {
                for (int i = 0; i < testCaseWidgets.size(); i++)
                {
                    testResults[i].status = "CE";
                    updateTestCaseResult(i, "CE", 0, "");
                }
                updateOverallResult();
            }
            else
            {
                testResults[currentTestIndex].status = "CE";
                updateTestCaseResult(currentTestIndex, "CE", 0, "");
            }

            outputEdit->appendPlainText(tr("编译器启动失败，请检查编译器路径"));
            statusBar()->showMessage(tr("编译器启动失败"), 3000);

            silentCompileProcess->deleteLater();
        });

    connect(silentCompileProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CompilerIDE::onSilentCompileFinished);

    silentCompileProcess->start(compilerPath, args);
}

void CompilerIDE::onSilentCompileFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QProcess *process = qobject_cast<QProcess*>(sender());
    if (!process)
    {
        return;
    }

    isSilentCompiling = false;

    QString errorOutput = QString::fromLocal8Bit(process->readAllStandardError());
    QString standardOutput = QString::fromLocal8Bit(process->readAllStandardOutput());

    process->deleteLater();

    if (exitCode != 0 || exitStatus != QProcess::NormalExit)
    {
        parseCompilerOutput(errorOutput + standardOutput);
        updateErrorTable(accumulatedErrors, accumulatedWarnings);

        if (isRunningAllTests)
        {
            for (int i = 0; i < testCaseWidgets.size(); ++i)
            {
                testResults[i].status = "CE";
                updateTestCaseResult(i, "CE", 0, "");
            }
            updateOverallResult();
        }
        else
        {
            testResults[currentTestIndex].status = "CE";
            updateTestCaseResult(currentTestIndex, "CE", 0, "");
        }

        errorDock->show();
        errorDock->raise();
        return;
    }

    warmupAndRunTest();
}

// 预热程序，使耗时更准
void CompilerIDE::warmupAndRunTest()
{
    QProcess *warmupProcess = new QProcess(this);
    warmupProcess->setWorkingDirectory(QFileInfo(testExecutablePath).absolutePath());

#ifdef Q_OS_WIN
    warmupProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_PROCESS_GROUP;
        args->flags |= CREATE_BREAKAWAY_FROM_JOB;
    });
#endif

    connect(warmupProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, warmupProcess](int, QProcess::ExitStatus)
    {
        warmupProcess->deleteLater();

        if (isRunningAllTests)
        {
            currentTestIndex = 0;
            runTest(currentTestIndex);
        }
        else
        {
            runTest(currentTestIndex);
        }
    });

    warmupProcess->start(testExecutablePath);

    if (warmupProcess->waitForStarted(1000))
    {
        warmupProcess->write("0\n");
        warmupProcess->closeWriteChannel();

        if (!warmupProcess->waitForFinished(500))
        {
            warmupProcess->kill();
            warmupProcess->waitForFinished(100);
        }
    }
    else
    {
        warmupProcess->deleteLater();

        if (isRunningAllTests)
        {
            currentTestIndex = 0;
            runTest(currentTestIndex);
        }
        else
        {
            runTest(currentTestIndex);
        }
    }
}

void CompilerIDE::runTest(int index)
{
    if (index < 0 || index >= testCaseWidgets.size())
    {
        return;
    }

    QWidget *testWidget = testCaseWidgets[index];
    QTextEdit *inputEdit = testWidget->findChild<QTextEdit*>(QString("inputEdit_%1").arg(index));

    if (!inputEdit)
    {
        return;
    }

    QString input = inputEdit->toPlainText();

    QString inputFilePath = QDir::temp().filePath(QString("test_input_%1.txt").arg(index));
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        testResults[index].status = "RE";
        testResults[index].timeMs = 0;
        updateTestCaseResult(index, "RE", 0, tr("无法创建输入文件"));

        if (isRunningAllTests)
        {
            runNextTest();
        }
        return;
    }

    QTextStream out(&inputFile);
    out << input;
    inputFile.close();

    testProcess = new QProcess(this);
    testProcess->setWorkingDirectory(QFileInfo(testExecutablePath).absolutePath());
    testProcess->setStandardInputFile(inputFilePath);

#ifdef Q_OS_WIN
    testProcess->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_PROCESS_GROUP;
        args->flags |= CREATE_BREAKAWAY_FROM_JOB;
    });
#endif

    testProcess->setProperty("testIndex", index);
    testProcess->setProperty("inputFilePath", inputFilePath);

    connect(testProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CompilerIDE::onTestFinished);

    testTimer = new QTimer(this);
    testTimer->setSingleShot(true);
    testTimer->setProperty("testIndex", index);
    connect(testTimer, &QTimer::timeout, this, &CompilerIDE::onTestTimeout);

    QElapsedTimer *elapsedTimer = new QElapsedTimer();
    elapsedTimer->start();
    testProcess->setProperty("elapsedTimer", QVariant::fromValue((void*)elapsedTimer));

    testProcess->start(testExecutablePath);

    if (testProcess->waitForStarted(1000))
    {
        int timeLimit = timeLimitSpinBox->value();
        testTimer->start(timeLimit + 200);

        updateTestCaseResult(index, "JUDGING", 0, "");
    }
    else
    {
        delete elapsedTimer;
        QFile::remove(inputFilePath);

        testResults[index].status = "RE";
        testResults[index].timeMs = 0;
        updateTestCaseResult(index, "RE", 0, tr("程序启动失败"));

        testProcess->deleteLater();
        testProcess = nullptr;
        testTimer->deleteLater();
        testTimer = nullptr;

        if (isRunningAllTests)
        {
            runNextTest();
        }
    }
}

void CompilerIDE::onTestFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (!testProcess)
    {
        return;
    }

    if (testTimer)
    {
        testTimer->stop();
        testTimer->deleteLater();
        testTimer = nullptr;
    }

    int index = testProcess->property("testIndex").toInt();

    QElapsedTimer *elapsedTimer = (QElapsedTimer*)testProcess->property("elapsedTimer").value<void*>();
    int elapsedMs = 0;
    if (elapsedTimer)
    {
        elapsedMs = elapsedTimer->elapsed();
        delete elapsedTimer;
    }

    testResults[index].timeMs = elapsedMs;

    QString actualOutput = QString::fromLocal8Bit(testProcess->readAllStandardOutput());
    testResults[index].actualOutput = actualOutput;

    int timeLimit = timeLimitSpinBox->value();
    QString status;

    if (elapsedMs > timeLimit + 200)
    {
        status = "TLE";
    }
    else if (exitStatus != QProcess::NormalExit || exitCode != 0)
    {
        status = "RE";
    }
    else
    {
        QWidget *testWidget = testCaseWidgets[index];
        QTextEdit *expectedEdit = testWidget->findChild<QTextEdit*>(QString("expectedEdit_%1").arg(index));

        if (expectedEdit)
        {
            QString expectedOutput = expectedEdit->toPlainText().trimmed();
            QString actualTrimmed = actualOutput.trimmed();

            expectedOutput.replace("\r\n", "\n");
            actualTrimmed.replace("\r\n", "\n");

            if (expectedOutput == actualTrimmed)
            {
                status = "AC";
            }
            else
            {
                status = "WA";
            }
        }
        else
        {
            status = "WA";
        }
    }

    testResults[index].status = status;
    updateTestCaseResult(index, status, elapsedMs, actualOutput);

    testProcess->deleteLater();
    testProcess = nullptr;

    if (isRunningAllTests)
    {
        runNextTest();
    }
    else
    {
        cleanupTestFiles();
    }
}

void CompilerIDE::onTestTimeout()
{
    if (!testProcess)
    {
        return;
    }

    int index = testTimer->property("testIndex").toInt();

    QElapsedTimer *elapsedTimer = (QElapsedTimer*)testProcess->property("elapsedTimer").value<void*>();
    int elapsedMs = 0;
    if (elapsedTimer)
    {
        elapsedMs = elapsedTimer->elapsed();
        delete elapsedTimer;
        testProcess->setProperty("elapsedTimer", QVariant());
    }

    testProcess->disconnect();
    testResults[index].timeMs = elapsedMs;
    testResults[index].status = "TLE";
    testResults[index].actualOutput = tr("超时");
    updateTestCaseResult(index, "TLE", elapsedMs, tr("超时"));

    if (testTimer)
    {
        testTimer->stop();
        testTimer->deleteLater();
        testTimer = nullptr;
    }

    QProcess *processToKill = testProcess;
    testProcess = nullptr;

#ifdef Q_OS_WIN
    qint64 pid = processToKill->processId();
    if (pid > 0)
    {
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD)pid);
        if (hProcess)
        {
            TerminateProcess(hProcess, 1);
            CloseHandle(hProcess);
        }
    }
    QTimer::singleShot(0, this, [this, processToKill]()
    {
        if (processToKill->state() != QProcess::NotRunning)
        {
            processToKill->kill();
        }

        QTimer *cleanupTimer = new QTimer(this);
        cleanupTimer->setProperty("process", QVariant::fromValue((void*)processToKill));
        cleanupTimer->setProperty("attempts", 0);
        connect(cleanupTimer, &QTimer::timeout, this, [this, cleanupTimer, processToKill]()
        {
            int attempts = cleanupTimer->property("attempts").toInt();

            if (processToKill->state() == QProcess::NotRunning || attempts >= 10)
            {
                cleanupTimer->stop();
                cleanupTimer->deleteLater();
                processToKill->deleteLater();

                if (isRunningAllTests)
                {
                    runNextTest();
                }
                else
                {
                    cleanupTestFiles();
                }
            }
            else
            {
                cleanupTimer->setProperty("attempts", attempts + 1);
                if (attempts == 5)
                {
                    processToKill->kill();
                }
            }
        });

        cleanupTimer->start(100);
    });
#else
    // Linux/macOS（没啥用处）
    processToKill->kill();

    QTimer::singleShot(100, this, [this, processToKill]()
    {
        processToKill->deleteLater();

        if (isRunningAllTests)
        {
            runNextTest();
        }
        else
        {
            cleanupTestFiles();
        }
    });
#endif
}

void CompilerIDE::runNextTest()
{
    currentTestIndex++;

    if (currentTestIndex < testCaseWidgets.size())
    {
        runTest(currentTestIndex);
    }
    else
    {
        updateOverallResult();
        cleanupTestFiles();
        isRunningAllTests = false;
    }
}

void CompilerIDE::updateTestCaseResult(int index, const QString &status, int timeMs, const QString &actualOutput)
{
    if (index < 0 || index >= testCaseWidgets.size())
    {
        return;
    }

    QWidget *testWidget = testCaseWidgets[index];

    QLabel *statusLabel = testWidget->findChild<QLabel*>(QString("statusLabel_%1").arg(index));
    if (statusLabel)
    {
        QString displayText;
        if (status == "AC")
        {
            displayText = tr("AC %1ms").arg(timeMs);
        }
        else if (status == "WA")
        {
            displayText = tr("WA %1ms").arg(timeMs);
        }
        else if (status == "TLE")
        {
            displayText = tr("TLE %1ms").arg(timeMs);
        }
        else if (status == "CE")
        {
            displayText = tr("CE");
        }
        else if (status == "RE")
        {
            displayText = tr("RE %1ms").arg(timeMs);
        }
        else if (status == "JUDGING")
        {
            displayText = tr("Judging...");
        }
        else if (status == "PENDING")
        {
            displayText = tr("Judging...");
        }
        else
        {
            displayText = tr("未测试");
        }

        statusLabel->setText(displayText);
        statusLabel->setStyleSheet(QString(
                                       "QLabel { "
                                       "background-color: %1; "
                                       "color: white; "
                                       "border-radius: 3px; "
                                       "padding: 4px 8px; "
                                       "font-size: 11px; "
                                       "}"
                                   ).arg(getStatusColor(status)));
    }

    QTextEdit *actualEdit = testWidget->findChild<QTextEdit*>(QString("actualEdit_%1").arg(index));
    if (actualEdit)
    {
        actualEdit->setPlainText(actualOutput);
    }

    if (status == "AC" || status == "ACCEPTED")
    {
        setTestCaseCollapsed(index, true);
    }
}

void CompilerIDE::updateOverallResult()
{
    if (!isRunningAllTests)
    {
        return;
    }

    if (testResults.isEmpty())
    {
        overallResultLabel->hide();
        return;
    }

    bool hasCompileError = false;
    bool allAccepted = true;

    for (const TestResult &result : testResults)
    {
        if (result.status == "CE")
        {
            hasCompileError = true;
            break;
        }
        if (result.status != "AC")
        {
            allAccepted = false;
        }
    }

    QString resultText;
    QString bgColor;

    if (hasCompileError)
    {
        resultText = tr("Compile Error");
        bgColor = "#DAA520";
    }
    else if (allAccepted)
    {
        resultText = tr("Accepted");
        bgColor = "#28a745";
    }
    else
    {
        resultText = tr("Unaccepted");
        bgColor = "#dc3545";
    }

    overallResultLabel->setText(resultText);
    overallResultLabel->setStyleSheet(QString(
                                          "QLabel { "
                                          "background-color: %1; "
                                          "color: white; "
                                          "border-radius: 5px; "
                                          "padding: 8px; "
                                          "font-size: 14px; "
                                          "font-weight: bold; "
                                          "}"
                                      ).arg(bgColor));
    overallResultLabel->show();
}

QString CompilerIDE::getStatusColor(const QString &status)
{
    if (status == "AC")
    {
        return "#28a745";
    }
    if (status == "WA")
    {
        return "#dc3545";
    }
    if (status == "TLE")
    {
        return "#000000";
    }
    if (status == "CE")
    {
        return "#DAA520";
    }
    if (status == "RE")
    {
        return "#6f42c1";
    }
    if (status == "JUDGING")
    {
        return "#FFA500";
    }
    if (status == "PENDING")
    {
        return "#FFA500";
    }
    return "#808080";
}

void CompilerIDE::cleanupTestFiles()
{
    if (!testExecutablePath.isEmpty())
    {
        QFileInfo fileInfo(testExecutablePath);
        QString tempDir = fileInfo.absolutePath();

        if (QFile::exists(testExecutablePath))
        {
            QFile::remove(testExecutablePath);
        }

        QDir dir(tempDir);
        if (dir.exists())
        {
            dir.removeRecursively();
        }

        testExecutablePath.clear();
    }
}

void CompilerIDE::restartIDEWithMessage()
{
    qDebug() << "[restartIDEWithMessage] 当前字体设置:" << editorFont.family() << editorFont.pointSize();

    isRestarting = true;

    QLabel *restartLabel = new QLabel(this);
    restartLabel->setStyleSheet(
        "QLabel {"
        "  background-color: rgba(255, 140, 0, 230);"
        "  color: white;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  padding: 25px 50px;"
        "  border-radius: 12px;"
        "  border: 3px solid #FF8C00;"
        "}"
    );
    restartLabel->setText(tr("主题已更改，正在重启编译器..."));
    restartLabel->setAlignment(Qt::AlignCenter);
    restartLabel->adjustSize();

    int x = (width() - restartLabel->width()) / 2;
    int y = (height() - restartLabel->height()) / 2;
    restartLabel->move(x, y);
    restartLabel->raise();
    restartLabel->show();

    if (compileProcess && compileProcess->state() == QProcess::Running)
    {
        compileProcess->kill();
        compileProcess->waitForFinished(1000);
        delete compileProcess;
        compileProcess = nullptr;
    }
    if (runProcess && runProcess->state() == QProcess::Running)
    {
        runProcess->kill();
        runProcess->waitForFinished(1000);
        delete runProcess;
        runProcess = nullptr;
    }
    if (debugger && debugger->isRunning())
    {
        debugger->stopDebugging();
        QThread::msleep(500);
    }
    if (hRunningProcess != NULL)
    {
        TerminateProcess(hRunningProcess, 0);
        WaitForSingleObject(hRunningProcess, 1000);
        CloseHandle(hRunningProcess);
        hRunningProcess = NULL;
    }
    killProcessByName("g++.exe");
    killProcessByName("gdb.exe");

    QApplication::processEvents(QEventLoop::AllEvents, 200);
    QThread::msleep(100);

    QStringList openFiles;
    int currentIndex = tabWidget->currentIndex();

    for (int i = 0; i < tabWidget->count(); ++i)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->document()->setModified(false);

            if (i == currentIndex && !curFile.isEmpty())
            {
                openFiles << curFile;
            }
            else
            {
                QString filePath = tabWidget->tabToolTip(i);
                if (!filePath.isEmpty() && QFile::exists(filePath))
                {
                    openFiles << filePath;
                }
            }
        }
    }

    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    settings.setValue("openFilesOnRestart", openFiles);
    settings.setValue("currentIndexOnRestart", currentIndex);
    settings.setValue("needReopen", true);
    settings.sync();

    qDebug() << "准备重启，保存的文件列表:" << openFiles;

    QString program = qApp->applicationFilePath();
    QStringList arguments = qApp->arguments();
    arguments.removeFirst();

    bool started = QProcess::startDetached(program, arguments);
    if (!started)
    {
        QMessageBox::critical(this, tr("重启失败"),
                              tr("无法启动新进程，请手动重新启动 Compiler IDE。"));
        isRestarting = false;
        return;
    }

    QTimer::singleShot(500, qApp, &QApplication::quit);
}

void CompilerIDE::loadFile(const QString &fileName)
{
    if (QFileInfo(fileName).suffix().toLower() == "pdf")
    {
        loadPdfFile(fileName);
        return;
    }

    int index = findEditor(fileName);
    if (index >= 0)
    {
        tabWidget->setCurrentIndex(index);
        return;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, tr("Compiler IDE"),
                             tr("无法读取文件 %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QString content;
    QString detectedEncoding = "UTF-8";

    auto isValidUtf8 = [](const QByteArray &data) -> bool
    {
        const unsigned char *bytes = reinterpret_cast<const unsigned char*>(data.constData());
        int len = data.size();
        int i = 0;

        if (len >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
        {
            i = 3;
        }

        bool hasHighBytes = false;

        while (i < len)
        {
            unsigned char c = bytes[i];

            if (c <= 0x7F)
            {
                i++;
            }
            else if (c >= 0xC2 && c <= 0xDF)
            {
                hasHighBytes = true;
                if (i + 1 >= len)
                {
                    return false;
                }
                if ((bytes[i + 1] & 0xC0) != 0x80)
                {
                    return false;
                }
                i += 2;
            }
            else if (c >= 0xE0 && c <= 0xEF)
            {
                hasHighBytes = true;
                if (i + 2 >= len)
                {
                    return false;
                }
                if ((bytes[i + 1] & 0xC0) != 0x80)
                {
                    return false;
                }
                if ((bytes[i + 2] & 0xC0) != 0x80)
                {
                    return false;
                }
                if (c == 0xE0 && bytes[i + 1] < 0xA0)
                {
                    return false;
                }
                i += 3;
            }
            else if (c >= 0xF0 && c <= 0xF4)
            {
                hasHighBytes = true;
                if (i + 3 >= len)
                {
                    return false;
                }
                if ((bytes[i + 1] & 0xC0) != 0x80)
                {
                    return false;
                }
                if ((bytes[i + 2] & 0xC0) != 0x80)
                {
                    return false;
                }
                if ((bytes[i + 3] & 0xC0) != 0x80)
                {
                    return false;
                }
                if (c == 0xF0 && bytes[i + 1] < 0x90)
                {
                    return false;
                }
                if (c == 0xF4 && bytes[i + 1] > 0x8F)
                {
                    return false;
                }
                i += 4;
            }
            else
            {
                return false;
            }
        }

        return true;
    };

    QFileInfo fileInfo(fileName);
    QString ext = fileInfo.suffix().toLower();
    QStringList gbkPriorityExts = {"nsi", "nsh", "bat", "cmd", "ini", "rc"};
    bool preferGBK = gbkPriorityExts.contains(ext);

    if (data.startsWith("\xEF\xBB\xBF"))
    {
        content = QString::fromUtf8(data.mid(3));
        detectedEncoding = "UTF-8-BOM";
    }
    else if (data.startsWith("\xFF\xFE"))
    {
        content = QString::fromUtf16(reinterpret_cast<const char16_t*>(data.constData() + 2), (data.size() - 2) / 2);
        detectedEncoding = "UTF-16LE";
    }
    else if (data.startsWith("\xFE\xFF"))
    {
        QByteArray swapped;
        for (int i = 2; i + 1 < data.size(); i += 2)
        {
            swapped.append(data[i + 1]);
            swapped.append(data[i]);
        }
        content = QString::fromUtf16(reinterpret_cast<const char16_t*>(swapped.constData()), swapped.size() / 2);
        detectedEncoding = "UTF-16BE";
    }
    else if (preferGBK)
    {
        QStringDecoder gbkDecoder("GBK");
        if (gbkDecoder.isValid())
        {
            content = gbkDecoder.decode(data);
            detectedEncoding = "GBK";
        }
        else
        {
            content = QString::fromLocal8Bit(data);
            detectedEncoding = "Local8Bit";
        }
    }
    else
    {
        if (isValidUtf8(data))
        {
            content = QString::fromUtf8(data);
            detectedEncoding = "UTF-8";
        }
        else
        {
            QStringDecoder gbkDecoder("GBK");
            if (gbkDecoder.isValid())
            {
                content = gbkDecoder.decode(data);
                detectedEncoding = "GBK";
            }
            else
            {
                content = QString::fromLocal8Bit(data);
                detectedEncoding = "Local8Bit";
            }
        }
    }

    CodeEditor *editor = new CodeEditor;
    editor->setPlainText(content);

    editorEncodings[editor] = detectedEncoding;
    qDebug() << "文件编码检测:" << fileName << "编码=" << detectedEncoding;

    editor->setAutoBracketsEnabled(autoBrackets);
    editor->setAutoQuotesEnabled(autoQuotes);
    editor->setCodeBeautifyEnabled(codeBeautify);
    editor->setCompletionEnabled(codeCompletionEnabled);
    qDebug() << "[newFile/loadFile] 即将设置 editorFont:" << editorFont.family() << editorFont.pointSize();
    editor->setEditorFont(editorFont);
    editor->setAutoIndentEnabled(autoIndent);
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    editor->setShowIndentGuides(settings.value("showIndentGuides", true).toBool());
    editor->setIndentSize(indentSize);
    editor->setCodeFoldingEnabled(codeFolding);
    editor->setLineNumbersEnabled(lineNumbers);
    new CppHighlighter(editor->document());
    editor->setDarkThemeEnabled(darkTheme);

    if (codeCompletionEnabled)
    {
        editor->setCompletionEnabled(true);
        editor->setCompetitionMode(false);
    }
    else
    {
        editor->setCompletionEnabled(false);
        editor->setCompetitionMode(true);
    }

    if (competitionMode && comp_disableCodeCompletion)
    {
        editor->disableAllCompletion();
    }

    QString fileKey = "file_" + QFileInfo(fileName).absoluteFilePath() + "_isEasyX";
    bool isEasyX = settings.value(fileKey, false).toBool();

    qDebug() << "加载文件类型标记:" << fileName << "isEasyX=" << isEasyX;

    editorEasyXFlags[editor] = isEasyX;

    connect(editor, &CodeEditor::breakpointToggled, this, [this, fileName](int line, bool added)
    {
        QTimer::singleShot(0, this, [this, fileName, line, added]()
        {
            if (!debugger)
            {
                qWarning() << "Debugger not initialized";
                return;
            }
            if (added)
            {
                debugger->setBreakpoint(fileName, line);
                outputEdit->appendPlainText(tr("断点已添加: %1:%2").arg(fileName).arg(line));
            }
            else
            {
                debugger->removeBreakpoint(fileName, line);
                outputEdit->appendPlainText(tr("断点已移除: %1:%2").arg(fileName).arg(line));
            }
        });
    });

    connect(editor, &CodeEditor::fontSizeChanged,
            this, &CompilerIDE::onEditorFontSizeChanged);

    connect(editor->document(), &QTextDocument::contentsChanged,
            this, &CompilerIDE::documentWasModified);

    connect(editor, &CodeEditor::cursorPositionChanged,
            this, &CompilerIDE::updateStatusBar);

    QString tabTitle = strippedName(fileName);
    if (isEasyX)
    {
        tabTitle += " [EasyX]";
    }
    int index2 = tabWidget->addTab(editor, tabTitle);
    tabWidget->setCurrentIndex(index2);

    tabWidget->setTabToolTip(index2, fileName);

    setCurrentFile(fileName);
    addToRecentFiles(fileName);
    statusBar()->showMessage(tr("文件已加载"), 2000);
}

void CompilerIDE::onRefactorActionTriggered()
{
    // 删了会报错，因此留空
}

bool CompilerIDE::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Compiler IDE"),
                             tr("无法写入文件 %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        file.close();
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString content = editor->toPlainText();

    QString encoding = editorEncodings.value(editor, "UTF-8");
    QByteArray encodedData;

    if (encoding == "UTF-8-BOM")
    {
        encodedData = QByteArray("\xEF\xBB\xBF") + content.toUtf8();
    }
    else if (encoding == "UTF-16LE")
    {
        encodedData = QByteArray("\xFF\xFE");
        QByteArray utf16data(reinterpret_cast<const char*>(content.utf16()), content.size() * 2);
        encodedData.append(utf16data);
    }
    else if (encoding == "UTF-16BE")
    {
        encodedData = QByteArray("\xFE\xFF");
        QString swapped = content;
        QByteArray utf16data(reinterpret_cast<const char*>(swapped.utf16()), swapped.size() * 2);
        for (int i = 0; i + 1 < utf16data.size(); i += 2)
        {
            encodedData.append(utf16data[i + 1]);
            encodedData.append(utf16data[i]);
        }
    }
    else if (encoding == "GBK" || encoding == "Local8Bit")
    {
        QStringEncoder gbkEncoder("GBK");
        if (gbkEncoder.isValid())
        {
            encodedData = gbkEncoder.encode(content);
        }
        else
        {
            encodedData = content.toLocal8Bit();
        }
    }
    else
    {
        encodedData = content.toUtf8();
    }

    file.write(encodedData);
    file.flush();
    QApplication::restoreOverrideCursor();
    file.close();
    setCurrentFile(fileName);

    int currentIndex = tabWidget->currentIndex();
    if (currentIndex >= 0)
    {
        tabWidget->setTabToolTip(currentIndex, fileName);
    }

    bool isEasyX = editorEasyXFlags.value(editor, false);
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    QString fileKey = "file_" + QFileInfo(fileName).absoluteFilePath() + "_isEasyX";
    settings.setValue(fileKey, isEasyX);
    settings.sync();

    qDebug() << "保存文件:" << fileName;
    qDebug() << "文件大小:" << content.length() << "字符";
    qDebug() << "文件编码:" << encoding;
    qDebug() << "EasyX标记:" << isEasyX;
    qDebug() << "配置键:" << fileKey;

    statusBar()->showMessage(tr("文件已保存"), 2000);
    return true;
}

void CompilerIDE::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->document()->setModified(false);
    }
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
    {
        shownName = "untitled.cpp";
    }
    setWindowFilePath(shownName);

    int index = tabWidget->currentIndex();
    if (index >= 0)
    {
        tabWidget->setTabText(index, strippedName(shownName));
        if (!fileName.isEmpty())
        {
            tabWidget->setTabToolTip(index, fileName);
        }
    }
}

QString CompilerIDE::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void CompilerIDE::newFile()
{
    if (CodeEditor *editor = currentEditor())
    {
        editor->closeCompletionPopup();
    }

    NewFileDialog dialog(this);

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    bool isEasyX = dialog.isEasyX();

    CodeEditor *editor = new CodeEditor;

    editor->setAutoBracketsEnabled(autoBrackets);
    editor->setAutoQuotesEnabled(autoQuotes);
    editor->setCodeBeautifyEnabled(codeBeautify);
    editor->setCompletionEnabled(codeCompletionEnabled);
    qDebug() << "[newFile/loadFile] 即将设置 editorFont:" << editorFont.family() << editorFont.pointSize();
    editor->setEditorFont(editorFont);
    editor->setAutoIndentEnabled(autoIndent);
    editor->setIndentSize(indentSize);
    editor->setCodeFoldingEnabled(codeFolding);
    editor->setLineNumbersEnabled(lineNumbers);
    new CppHighlighter(editor->document());
    editor->setDarkThemeEnabled(darkTheme);
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    editor->setShowIndentGuides(settings.value("showIndentGuides", true).toBool());

    if (codeCompletionEnabled)
    {
        editor->setCompletionEnabled(true);
        editor->setCompetitionMode(false);
    }
    else
    {
        editor->setCompletionEnabled(false);
        editor->setCompetitionMode(true);
    }

    if (competitionMode && comp_disableCodeCompletion)
    {
        editor->disableAllCompletion();
    }

    editorEasyXFlags[editor] = isEasyX;

    editorEncodings[editor] = "UTF-8";

    if (isEasyX)
    {
        editor->setPlainText(defaultEasyXTemplate);
    }
    else
    {
        editor->setPlainText(defaultNormalTemplate);
    }

    connect(editor, &CodeEditor::breakpointToggled, this, [this, editor](int line, bool added)
    {
        QString fileName = curFile;
        if (fileName.isEmpty())
        {
            int index = tabWidget->indexOf(editor);
            if (index >= 0)
            {
                fileName = tabWidget->tabText(index);
                if (fileName.endsWith("*"))
                {
                    fileName.chop(1);
                }
            }
        }

        QString fileNameCopy = fileName;
        QTimer::singleShot(0, this, [this, fileNameCopy, line, added]()
        {
            if (!debugger)
            {
                qWarning() << "Debugger not initialized yet";
                return;
            }

            if (added)
            {
                debugger->setBreakpoint(fileNameCopy, line);
                outputEdit->appendPlainText(tr("断点已添加: %1:%2").arg(fileNameCopy).arg(line));
            }
            else
            {
                debugger->removeBreakpoint(fileNameCopy, line);
                outputEdit->appendPlainText(tr("断点已移除: %1:%2").arg(fileNameCopy).arg(line));
            }
        });
    });

    connect(editor, &CodeEditor::fontSizeChanged,
            this, &CompilerIDE::onEditorFontSizeChanged);
    connect(editor->document(), &QTextDocument::contentsChanged,
            this, &CompilerIDE::documentWasModified);
    connect(editor, &CodeEditor::cursorPositionChanged,
            this, &CompilerIDE::updateStatusBar);

    QString tabTitle = QString("untitled%1").arg(untitledCounter++);
    int index = tabWidget->addTab(editor, tabTitle);
    tabWidget->setCurrentIndex(index);

    curFile.clear();
    setWindowTitle(tr("Compiler IDE %1").arg(IDE_VERSION));
}

void CompilerIDE::loadTemplateSettings()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    QString savedNormal = settings.value("CodeTemplate/Normal", "").toString();
    QString savedEasyX = settings.value("CodeTemplate/EasyX", "").toString();

    if (!savedNormal.isEmpty())
    {
        defaultNormalTemplate = savedNormal;
    }
    if (!savedEasyX.isEmpty())
    {
        defaultEasyXTemplate = savedEasyX;
    }
}

void CompilerIDE::saveTemplateSettings()
{
    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    settings.setValue("CodeTemplate/Normal", defaultNormalTemplate);
    settings.setValue("CodeTemplate/EasyX", defaultEasyXTemplate);
    settings.sync();
}

void CompilerIDE::showTemplateSettings()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("代码模板设置"));
    dialog->resize(750, 650);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    QLabel *infoLabel = new QLabel(tr("设置新建文件时的缺省源代码模板"));
    QFont infoFont = infoLabel->font();
    infoFont.setItalic(true);
    infoLabel->setFont(infoFont);
    infoLabel->setStyleSheet("color: #666; margin-bottom: 10px;");
    mainLayout->addWidget(infoLabel);

    QTabWidget *tabWidget = new QTabWidget;

    QWidget *normalTab = new QWidget;
    QVBoxLayout *normalLayout = new QVBoxLayout(normalTab);

    QLabel *normalLabel = new QLabel(tr("普通项目缺省源代码:"));
    normalLabel->setStyleSheet("font-weight: bold; margin-bottom: 5px;");

    QTextEdit *normalEdit = new QTextEdit;
    normalEdit->setPlainText(defaultNormalTemplate);
    normalEdit->setFont(QFont("Consolas", 10));
    normalEdit->setLineWrapMode(QTextEdit::NoWrap);
    normalEdit->setTabStopDistance(40);

    normalLayout->addWidget(normalLabel);
    normalLayout->addWidget(normalEdit);

    QWidget *easyxTab = new QWidget;
    QVBoxLayout *easyxLayout = new QVBoxLayout(easyxTab);

    QLabel *easyxLabel = new QLabel(tr("EasyX项目缺省源代码:"));
    easyxLabel->setStyleSheet("font-weight: bold; margin-bottom: 5px;");

    QTextEdit *easyxEdit = new QTextEdit;
    easyxEdit->setPlainText(defaultEasyXTemplate);
    easyxEdit->setFont(QFont("Consolas", 10));
    easyxEdit->setLineWrapMode(QTextEdit::NoWrap);
    easyxEdit->setTabStopDistance(40);

    easyxLayout->addWidget(easyxLabel);
    easyxLayout->addWidget(easyxEdit);

    tabWidget->addTab(normalTab, tr("普通项目"));
    tabWidget->addTab(easyxTab, tr("EasyX项目"));

    mainLayout->addWidget(tabWidget);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QPushButton *resetBtn = new QPushButton(tr("恢复默认模板"));
    connect(resetBtn, &QPushButton::clicked, [=]()
    {
        int ret = QMessageBox::question(
                      dialog,
                      tr("确认恢复"),
                      tr("确定要恢复到默认模板吗？当前的自定义模板将被覆盖。"),
                      QMessageBox::Yes | QMessageBox::No,
                      QMessageBox::No
                  );

        if (ret == QMessageBox::Yes)
        {
            normalEdit->setPlainText(
                "#include <bits/stdc++.h>\n"
                "using namespace std;\n"
                "int main()\n"
                "{\n"
                "    \n"
                "    return 0;\n"
                "}"
            );

            easyxEdit->setPlainText(
                "#include <bits/stdc++.h>\n"
                "#include <easyx.h>\n"
                "#include <graphics.h>\n"
                "int main()\n"
                "{\n"
                "    \n"
                "    return 0;\n"
                "}"
            );

            QMessageBox::information(dialog, tr("提示"), tr("已恢复为默认模板"));
        }
    });

    buttonLayout->addWidget(resetBtn);
    buttonLayout->addStretch();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel
    );
    buttonLayout->addWidget(buttonBox);

    mainLayout->addLayout(buttonLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, [=]()
    {
        defaultNormalTemplate = normalEdit->toPlainText();
        defaultEasyXTemplate = easyxEdit->toPlainText();
        saveTemplateSettings();

        outputEdit->appendPlainText(tr("√ 代码模板设置已保存"));
        statusBar()->showMessage(tr("代码模板设置已保存"), 3000);

        dialog->accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    dialog->exec();
}

void CompilerIDE::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
                           tr("C++文件 (*.cpp *.cc *.cxx *.c++ *.h *.hpp *.hh *.hxx *.h++);;PDF文件 (*.pdf);;所有文件 (*)"));
        if (!fileName.isEmpty())
        {
            loadFile(fileName);
        }
    }
}

void CompilerIDE::openProject()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("打开项目"), "",
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        projectManager->setRootPath(dir);
        projectDock->setWindowTitle(tr("项目管理器 - %1").arg(QDir(dir).dirName()));
    }
}

bool CompilerIDE::save()
{
    if (curFile.isEmpty() || curFile == "")
    {
        return saveAs();
    }
    return saveFile(curFile);
}

bool CompilerIDE::saveAs()
{
    QString defaultName = tabWidget->tabText(tabWidget->currentIndex());
    if (defaultName.endsWith("*"))
    {
        defaultName.chop(1);
    }

    if (defaultName.startsWith("untitled "))
    {
        defaultName += ".cpp";
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                       tr("另存为"),
                       QDir::homePath() + "/" + defaultName,
                       tr("C++源文件 (*.cpp *.cc *.cxx *.c++);;C++头文件 (*.h *.hpp *.hh);;所有文件 (*)"));

    if (fileName.isEmpty())
    {
        return false;
    }

    if (saveFile(fileName))
    {
        setCurrentFile(fileName);
        return true;
    }
    return false;
}

void CompilerIDE::about()
{
    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle(tr("关于 Compiler IDE"));
    aboutDialog.setMinimumWidth(620);
    aboutDialog.setMaximumHeight(560);
    aboutDialog.setMinimumHeight(560);
    aboutDialog.setModal(true);

    const bool isDark = darkTheme;

    const QString bgColor        = isDark ? "#202334" : "#FFFFFF";
    const QString cardColor      = isDark ? "#2B2F45" : "#F7F9FC";
    const QString textColor      = isDark ? "#F2F4F8" : "#1F2937";
    const QString subTextColor   = isDark ? "#AAB2C0" : "#667085";
    const QString borderColor    = isDark ? "#454B66" : "#E5E7EB";
    const QString highlightColor = "#2E86AB";
    const QString linkColor      = isDark ? "#5BC4F5" : "#1A6FA8";

    aboutDialog.setStyleSheet(QString(
        "QDialog {"
        "    background-color: %1;"
        "}"
        "QLabel {"
        "    color: %2;"
        "    font-family: 'Microsoft YaHei', 'Segoe UI', sans-serif;"
        "}"
        "QPushButton {"
        "    background-color: %5;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 8px 28px;"
        "    font-size: 13px;"
        "    font-weight: 600;"
        "    min-width: 90px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #3B9BC8;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1E5A7A;"
        "}"
        "QGroupBox {"
        "    background-color: %3;"
        "    border: 1px solid %4;"
        "    border-radius: 12px;"
        "    margin-top: 8px;"
        "    padding: 0px;"
        "    font-weight: 600;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 16px;"
        "    padding: 0 8px;"
        "    color: %5;"
        "}"
    ).arg(bgColor, textColor, cardColor, borderColor, highlightColor));

    QVBoxLayout *mainLayout = new QVBoxLayout(&aboutDialog);
    mainLayout->setContentsMargins(30, 28, 30, 26);
    mainLayout->setSpacing(14);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(18);

    QLabel *iconLabel = new QLabel();
    iconLabel->setFixedSize(84, 84);
    iconLabel->setAlignment(Qt::AlignCenter);

    QPixmap logo(":/icons/CompilerIDE_logo.png");
    if (!logo.isNull())
    {
        iconLabel->setPixmap(logo.scaled(84, 84, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        iconLabel->setStyleSheet(
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "stop:0 #2E86AB, stop:0.55 #A23B72, stop:1 #F18F01);"
            "border-radius: 20px;"
            "color: white; font-size: 20px; font-weight: bold;"
        );
        iconLabel->setText("C++");
    }

    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(5);

    QLabel *nameLabel = new QLabel("Compiler IDE");
    nameLabel->setStyleSheet(QString(
        "color: %1; font-size: 28px; font-weight: 700; letter-spacing: 0.2px;"
    ).arg(textColor));

    QLabel *versionLabel = new QLabel(tr("版本 %1").arg(IDE_VERSION));
    versionLabel->setStyleSheet(QString(
        "color: %1; font-size: 14px; font-weight: 600;"
    ).arg(highlightColor));

    QLabel *buildLabel = new QLabel(tr("构建日期：%1 %2")
                                    .arg(BUILD_DATE).arg(BUILD_TIME));
    buildLabel->setStyleSheet(QString(
        "color: %1; font-size: 12px;"
    ).arg(subTextColor));

    titleLayout->addWidget(nameLabel);
    titleLayout->addWidget(versionLabel);
    titleLayout->addWidget(buildLabel);
    titleLayout->addStretch();

    headerLayout->addWidget(iconLabel);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    QLabel *descLabel = new QLabel(
        tr("Compiler IDE 是一款面向 C++ 学习、算法竞赛与日常开发的现代化集成开发环境。")
    );
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setStyleSheet(QString(
        "background-color: %1;"
        "color: %2;"
        "border: 1px solid %3;"
        "border-radius: 12px;"
        "padding: 12px 18px;"
        "font-size: 13px;"
    ).arg(cardColor, subTextColor, borderColor));

    mainLayout->addWidget(descLabel);

    QGroupBox *featuresGroup = new QGroupBox(tr("主要特性"));
    QVBoxLayout *featuresLayout = new QVBoxLayout(featuresGroup);
    featuresLayout->setSpacing(6);
    featuresLayout->setContentsMargins(18, 10, 18, 10);

    QStringList features =
    {
        tr("多文件标签页编辑与语法高亮"),
        tr("智能代码补全与代码片段管理"),
        tr("集成终端、项目管理器与调试功能"),
        tr("竞赛模式、计时器与专注辅助"),
        tr("代码美化、样例评测与常用开发工具集成")
    };

    for (const QString &feature : features)
    {
        QLabel *featureLabel = new QLabel(QString("• %1").arg(feature));
        featureLabel->setStyleSheet(QString(
            "color: %1; font-size: 12px; padding: 1px 0;"
        ).arg(textColor));
        featuresLayout->addWidget(featureLabel);
    }

    mainLayout->addWidget(featuresGroup);

    QGroupBox *thirdPartyGroup = new QGroupBox(tr("第三方组件"));
    QVBoxLayout *thirdPartyLayout = new QVBoxLayout(thirdPartyGroup);
    thirdPartyLayout->setSpacing(6);
    thirdPartyLayout->setContentsMargins(18, 10, 18, 10);

    QHBoxLayout *qtRow = new QHBoxLayout();
    QLabel *qtLabel = new QLabel(tr("Qt %1").arg(QT_VERSION_STR));
    qtLabel->setStyleSheet(QString("color: %1; font-size: 12px;").arg(textColor));
    QLabel *qtLicenseLink = new QLabel(
        QString("<a href='https://doc.qt.io/qt-6/licensing.html' style='color:%1;'>LGPL v3</a>").arg(linkColor)
    );
    qtLicenseLink->setOpenExternalLinks(true);
    QLabel *qtProjectLink = new QLabel(
        QString("<a href='https://www.qt.io/' style='color:%1;'>项目主页</a>").arg(linkColor)
    );
    qtProjectLink->setOpenExternalLinks(true);
    qtRow->addWidget(qtLabel);
    qtRow->addStretch();
    qtRow->addWidget(qtLicenseLink);
    qtRow->addSpacing(12);
    qtRow->addWidget(qtProjectLink);
    thirdPartyLayout->addLayout(qtRow);

    QHBoxLayout *phosphorRow = new QHBoxLayout();
    QLabel *phosphorLabel = new QLabel(tr("Phosphor Icons（部分图标来源）"));
    phosphorLabel->setStyleSheet(QString("color: %1; font-size: 12px;").arg(textColor));
    QLabel *phosphorLicenseLink = new QLabel(
        QString("<a href='https://github.com/phosphor-icons/homepage/blob/master/LICENSE' style='color:%1;'>MIT License</a>").arg(linkColor)
    );
    phosphorLicenseLink->setOpenExternalLinks(true);
    QLabel *phosphorProjectLink = new QLabel(
        QString("<a href='https://phosphoricons.com/' style='color:%1;'>项目主页</a>").arg(linkColor)
    );
    phosphorProjectLink->setOpenExternalLinks(true);
    phosphorRow->addWidget(phosphorLabel);
    phosphorRow->addStretch();
    phosphorRow->addWidget(phosphorLicenseLink);
    phosphorRow->addSpacing(12);
    phosphorRow->addWidget(phosphorProjectLink);
    thirdPartyLayout->addLayout(phosphorRow);

    mainLayout->addWidget(thirdPartyGroup);

    QLabel *copyrightLabel = new QLabel(
        QString(
            "<div style='text-align:center; line-height:1.7;'>"
            "<span style='color:%1; font-size:12px;'>"
            "Copyright © 2026 CompilerIDE Team. All Rights Reserved.<br>"
            "本软件以 "
            "<a href='https://www.gnu.org/licenses/gpl-3.0.html' style='color:%2;'>GNU GPL v3.0</a>"
            " 协议开源，源代码托管于 "
            "<a href='https://github.com/CompilerIDE/CompilerIDE' style='color:%2;'>GitHub</a>"
            "。"
            "</span>"
            "</div>"
        ).arg(subTextColor, linkColor)
    );
    copyrightLabel->setTextFormat(Qt::RichText);
    copyrightLabel->setOpenExternalLinks(true);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->setWordWrap(true);

    mainLayout->addWidget(copyrightLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    QPushButton *closeButton = new QPushButton(tr("关闭"));
    closeButton->setDefault(true);
    connect(closeButton, &QPushButton::clicked, &aboutDialog, &QDialog::accept);
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    aboutDialog.exec();
}

void CompilerIDE::documentWasModified()
{
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        setWindowModified(editor->document()->isModified());

        int index = tabWidget->currentIndex();
        if (index >= 0)
        {
            QString title = tabWidget->tabText(index);
            if (title.endsWith("*"))
            {
                if (!editor->document()->isModified())
                {
                    title.chop(1);
                    tabWidget->setTabText(index, title);
                }
            }
            else
            {
                if (editor->document()->isModified())
                {
                    tabWidget->setTabText(index, title + "*");
                }
            }
        }
    }
    updateStatusBar();
}

QString CompilerIDE::detectFileEncoding(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return "UTF-8";
    }

    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty())
    {
        return "UTF-8";
    }

    if (data.size() >= 3)
    {
        if ((unsigned char)data[0] == 0xEF &&
                (unsigned char)data[1] == 0xBB &&
                (unsigned char)data[2] == 0xBF)
        {
            return "UTF-8";
        }
    }

    auto isValidUtf8 = [](const QByteArray &data) -> bool
    {
        const unsigned char *bytes = reinterpret_cast<const unsigned char*>(data.constData());
        int len = data.size();
        int i = 0;

        while (i < len)
        {
            unsigned char c = bytes[i];

            if (c <= 0x7F)
            {
                i++;
            }
            else if (c >= 0xC2 && c <= 0xDF)
            {
                if (i + 1 >= len)
                {
                    return false;
                }
                if ((bytes[i + 1] & 0xC0) != 0x80)
                {
                    return false;
                }
                i += 2;
            }
            else if (c >= 0xE0 && c <= 0xEF)
            {
                if (i + 2 >= len)
                {
                    return false;
                }
                if ((bytes[i + 1] & 0xC0) != 0x80)
                {
                    return false;
                }
                if ((bytes[i + 2] & 0xC0) != 0x80)
                {
                    return false;
                }
                i += 3;
            }
            else if (c >= 0xF0 && c <= 0xF4)
            {
                if (i + 3 >= len)
                {
                    return false;
                }
                if ((bytes[i + 1] & 0xC0) != 0x80)
                {
                    return false;
                }
                if ((bytes[i + 2] & 0xC0) != 0x80)
                {
                    return false;
                }
                if ((bytes[i + 3] & 0xC0) != 0x80)
                {
                    return false;
                }
                i += 4;
            }
            else
            {
                return false;
            }
        }
        return true;
    };

    int gbkChars = 0;
    for (int i = 0; i < data.size() - 1; i++)
    {
        unsigned char c1 = data[i];
        unsigned char c2 = data[i + 1];

        if (c1 >= 0x81 && c1 <= 0xFE &&
                ((c2 >= 0x40 && c2 <= 0x7E) || (c2 >= 0x80 && c2 <= 0xFE)))
        {
            gbkChars++;
        }
    }

    if (isValidUtf8(data))
    {
        if (gbkChars > 5)
        {
            QStringDecoder gbkDecoder("GBK");
            QString gbkDecoded = gbkDecoder.decode(data);

            if (!gbkDecoder.hasError() && !gbkDecoded.isEmpty())
            {
                return "GBK";
            }
        }
        return "UTF-8";
    }
    else
    {
        return "GBK";
    }
}

void CompilerIDE::compile()
{
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->clearErrorLine();
            editor->clearWarningLine();
        }
    }

    if (compileProcess)
    {
        if (compileProcess->state() == QProcess::Running)
        {
            resetOutputFormat();
            QMessageBox::warning(this, tr("提示"), tr("已有临时编译任务正在运行，请稍后..."));
            return;
        }
        else
        {
            compileProcess->disconnect();
            compileProcess->deleteLater();
            compileProcess = nullptr;
        }
    }

    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }

    if (!QFile::exists(compilerPath))
    {
        QString errorMsg = tr("编译器路径不存在: %1").arg(compilerPath);
        resetOutputFormat();
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        return;
    }

    QProcess testProcess;
    testProcess.start(compilerPath, QStringList() << "--version");
    if (!testProcess.waitForFinished(3000) || testProcess.exitCode() != 0)
    {
        QString errorMsg = tr("编译器不可用或路径错误: %1").arg(compilerPath);
        resetOutputFormat();
        outputEdit->appendPlainText(errorMsg);
        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));

        outputEdit->appendPlainText(tr("建议:"));
        outputEdit->appendPlainText(tr("1. 检查编译器路径是否正确"));
        outputEdit->appendPlainText(tr("2. 确保编译器已正确安装"));
        outputEdit->appendPlainText(tr("3. 对于MinGW，确保bin目录包含g++.exe、gcc.exe和必要的链接器"));

        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        return;
    }

    if (curFile.isEmpty() && !save())
    {
        return;
    }

    if (editor->document()->isModified())
    {
        if (!save())
        {
            return;
        }
    }

    usingTempDir = false;
    tempSourceFile.clear();
    originalOutputExe.clear();

    QString actualSourceFile = curFile;
    QString actualOutputDir = QFileInfo(curFile).absolutePath();

    lastCompiledFile = curFile;

    auto containsNonAscii = [](const QString &str) -> bool
    {
        for (const QChar &c : str)
        {
            if (c.unicode() > 127)
            {
                return true;
            }
        }
        return false;
    };

    if (containsNonAscii(curFile))
    {
        resetOutputFormat();
        outputEdit->appendPlainText(tr("检测到路径包含中文字符，使用临时目录编译..."));

        originalOutputExe = QFileInfo(curFile).absoluteDir().filePath(
                                QFileInfo(curFile).baseName() + ".exe");

        QString tempDir = QDir::temp().absolutePath() + "/CompilerIDE_Build";
        QDir().mkpath(tempDir);

        QString sourceFileName = QFileInfo(curFile).fileName();

        tempSourceFile = tempDir + "/" + sourceFileName;

        if (QFile::exists(tempSourceFile))
        {
            QFile::remove(tempSourceFile);
        }

        if (!QFile::copy(curFile, tempSourceFile))
        {
            resetOutputFormat();
            outputEdit->appendPlainText(tr("无法复制源文件到临时目录"));
            return;
        }


        actualSourceFile = tempSourceFile;
        actualOutputDir = tempDir;
        usingTempDir = true;

        outputEdit->appendPlainText(tr("临时源文件: %1").arg(tempSourceFile));
    }

    QString outputExe = QDir(actualOutputDir).filePath(QFileInfo(actualSourceFile).baseName());
#ifdef Q_OS_WIN
    outputExe += ".exe";
#endif

    isNewCompilation = true;

    if (QFile::exists(outputExe))
    {
        QString exeName = QFileInfo(outputExe).fileName();
        killProcessByName(exeName);
        QThread::msleep(300);

        QFile file(outputExe);
        if (file.open(QIODevice::ReadWrite))
        {
            file.close();
        }
        else
        {
            resetOutputFormat();
            outputEdit->appendPlainText(tr("警告：无法写入输出文件，可能正在被占用"));
            outputEdit->appendPlainText(tr("尝试继续编译..."));
        }
    }

    QString fileEncoding = detectFileEncoding(curFile);

    accumulatedErrors.clear();
    accumulatedWarnings.clear();

    QApplication::processEvents();

    outputEdit->clear();

    if (outputDock)
    {
        outputDock->raise();
    }

    QTextCursor cursor = outputEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);

    appendDefaultText(tr("编译文件: %1").arg(lastCompiledFile));
    outputEdit->appendPlainText("");

    QStringList args;
    bool isEasyX = editorEasyXFlags.value(editor, false);

    if (compilerType == "msvc")
    {
        if (cppStandard == "c++14")
        {
            args << "/std:c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "/std:c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "/std:c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "/std:c++latest";
        }

        args << "/EHsc" << "/W4" << "/nologo";
        args << "/I.";
        args << "/utf-8";
        args << "/Zi";

        QString outputFile = QFileInfo(curFile).absolutePath() + "/" + QFileInfo(curFile).baseName() + ".exe";
        args << "/Fe" + outputFile;
        args << curFile;

    }
    else
    {
        if (cppStandard == "c++14")
        {
            args << "-std=c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "-std=c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "-std=c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "-std=c++2b";
        }

        args << "-g";
        args << optimizationLevel;
        args << "-Wall" << "-Wextra" << "-pedantic";
        if (fileEncoding == "GBK")
        {
            args << "-finput-charset=GBK" << "-fexec-charset=GBK";
        }
        else
        {
            args << "-finput-charset=UTF-8" << "-fexec-charset=UTF-8";
        }
        args << "-static";
        args << "-static-libgcc" << "-static-libstdc++";

        QString outputFile = QFileInfo(curFile).absolutePath() + "/" + QFileInfo(curFile).baseName();
#ifdef Q_OS_WIN
        outputFile += ".exe";
#endif
        args << "-o" << outputExe;
        args << actualSourceFile;
        if (isEasyX)
        {
            QString appDir = QCoreApplication::applicationDirPath();
            QString bundledTDMGCC = appDir + "/TDM-GCC/bin/g++.exe";

            QString actualCompilerPath = compilerPath;
            QString compilerName = "配置的编译器";

            if (QFile::exists(bundledTDMGCC))
            {
                actualCompilerPath = bundledTDMGCC;
                compilerName = "内置 TDM-GCC";
                outputEdit->appendPlainText(tr("  使用内置 TDM-GCC 编译器"));
                outputEdit->appendPlainText(tr("  路径: %1").arg(bundledTDMGCC));
            }
            else
            {
                outputEdit->appendPlainText(tr("  使用配置的编译器"));
                outputEdit->appendPlainText(tr("  路径: %1").arg(compilerPath));
            }

            QFileInfo compilerInfo(actualCompilerPath);
            QString compilerDir = compilerInfo.absolutePath();
            QDir baseDir(compilerDir);
            baseDir.cdUp();

            QString mingwLib = baseDir.absolutePath() + "/x86_64-w64-mingw32/lib";
            QString mingwInclude = baseDir.absolutePath() + "/x86_64-w64-mingw32/include";
            QString easyxLib = mingwLib + "/libgraphics.a";

            outputEdit->appendPlainText(tr("  EasyX 图形模式"));
            outputEdit->appendPlainText(tr("  库路径: %1").arg(mingwLib));
            outputEdit->appendPlainText(tr("  头文件路径: %1").arg(mingwInclude));
            outputEdit->appendPlainText("");

            if (!QFile::exists(easyxLib))
            {
                easyxLib = mingwLib + "/libeasyxw.a";
                if (!QFile::exists(easyxLib))
                {
                    easyxLib = mingwLib + "/libeasyx.a";
                    if (!QFile::exists(easyxLib))
                    {
                        QString errorMsg = tr("错误: 找不到 EasyX 库文件\n"
                                              "请确保 %1 编译器包含 EasyX 库").arg(compilerName);
                        resetOutputFormat();
                        outputEdit->appendPlainText(errorMsg);
                        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
                        updateErrorTable(accumulatedErrors, accumulatedWarnings);
                        return;
                    }
                }
            }

            outputEdit->appendPlainText(tr("找到 EasyX 库: %1").arg(QFileInfo(easyxLib).fileName()));
            outputEdit->appendPlainText("");

            args << "-L" + QDir::toNativeSeparators(mingwLib);
            args << "-I" + QDir::toNativeSeparators(mingwInclude);
            args << "-DUNICODE" << "-D_UNICODE";
            args << "-static-libgcc" << "-static-libstdc++";
            args << "-leasyxw"
                 << "-lgdi32"
                 << "-limm32"
                 << "-lmsimg32"
                 << "-lole32"
                 << "-loleaut32"
                 << "-lwinmm"
                 << "-luuid"
                 << "-lgdiplus";

            compilerPath = actualCompilerPath;
        }
    }

    if (compileProcess)
    {
        compileProcess->kill();
        compileProcess->waitForFinished();
        delete compileProcess;
    }

    compileProcess = new QProcess(this);
    compileProcess->setProcessChannelMode(QProcess::MergedChannels);

    QString appDir = QCoreApplication::applicationDirPath();
    QString mingwBin = appDir + "/mingw/bin";

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString currentPath = env.value("PATH");
    env.insert("PATH", mingwBin + ";" + currentPath);

#ifdef Q_OS_WIN
    env.insert("PYTHONIOENCODING", "utf-8");
    env.insert("UTF-8", "1");
#endif
    compileProcess->setProcessEnvironment(env);

    connect(compileProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CompilerIDE::compilerFinished);
    connect(compileProcess, &QProcess::readyReadStandardOutput,
            this, &CompilerIDE::readCompilerOutput);
    connect(compileProcess, &QProcess::readyReadStandardError,
            this, &CompilerIDE::readCompilerError);

    QString commandLine = compilerPath + " " + args.join(" ");

    appendDefaultText(tr("编译命令: %1").arg(commandLine));
    outputEdit->appendPlainText("");

    appendDefaultText(tr("正在编译..."));

    qDebug() << "编译命令:" << commandLine;

    compileProcess->start(compilerPath, args);
    if (!compileProcess->waitForStarted(5000))
    {
        QString errorMsg = tr("无法启动编译器: %1").arg(compilerPath);
        appendColoredText(errorMsg, errorFormat);

        accumulatedErrors.append(QString("0|0|%1").arg(errorMsg));
        if (compilerType == "msvc")
        {
            QString noteMsg = tr("注意: MSVC编译器可能需要从开发者命令提示符运行");
            appendDefaultText(noteMsg);
            accumulatedErrors.append(QString("0|0|%1").arg(noteMsg));
        }

        updateErrorTable(accumulatedErrors, accumulatedWarnings);
    }
}

void CompilerIDE::killProcessByName(const QString &exeName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32))
    {
        do
        {
            QString processName = QString::fromWCharArray(pe32.szExeFile);
            if (processName.compare(exeName, Qt::CaseInsensitive) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pe32.th32ProcessID);
                if (hProcess != NULL)
                {
                    TerminateProcess(hProcess, 0);
                    WaitForSingleObject(hProcess, 2000);
                    CloseHandle(hProcess);
                }
            }
        }
        while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
}

void CompilerIDE::runExecutable(const QString &programPath, bool isAfterCompile)
{
    if (hRunningProcess != NULL)
    {
        TerminateProcess(hRunningProcess, 0);
        WaitForSingleObject(hRunningProcess, 3000);
        CloseHandle(hRunningProcess);
        hRunningProcess = NULL;

        QThread::msleep(500);
    }

    if (!QFile::exists(programPath))
    {
        appendColoredText(tr("错误：找不到可执行文件 %1").arg(programPath), errorFormat);
        appendDefaultText(tr("请先编译程序"));
        return;
    }

    if (isAfterCompile)
    {
        outputEdit->appendPlainText("");
    }

    appendDefaultText(tr("正在启动程序: %1").arg(programPath));

    QString batPath = QDir::temp().absolutePath() + "/run_program.bat";

    QString sourceEncoding = detectFileEncoding(curFile);
    bool isGBK = (sourceEncoding == "GBK");

    QFile batFile(batPath);
    if (batFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString nativeProgramPath = QDir::toNativeSeparators(programPath);
        QString programDir = QDir::toNativeSeparators(QFileInfo(programPath).absolutePath());

        if (isGBK)
        {
            QString batContent;
            batContent += "@echo off\r\n";
            batContent += "chcp 936 >nul 2>&1\r\n";
            batContent += "set PATH=" + QCoreApplication::applicationDirPath() + "/mingw/bin;%PATH%\r\n";
            batContent += "cd /d \"" + programDir + "\"\r\n";
            batContent += "\"" + nativeProgramPath + "\"\r\n";
            batContent += "set EXIT_CODE=%errorlevel%\r\n";
            batContent += "echo.\r\n";
            batContent += "echo 程序已退出，返回代码: %EXIT_CODE%\r\n";
            batContent += "pause\r\n";

            QStringEncoder encoder("GBK");
            if (!encoder.isValid())
            {
                batFile.write(batContent.toLocal8Bit());
            }
            else
            {
                QByteArray gbkData = encoder.encode(batContent);
                batFile.write(gbkData);
            }
        }
        else
        {
            QTextStream out(&batFile);
            out.setEncoding(QStringConverter::Utf8);
            out << "@echo off\n";
            out << "chcp 65001 >nul 2>&1\r\n";
            out << "set PATH=" << QCoreApplication::applicationDirPath() + "/mingw/bin" << ";%PATH%\r\n";
            out << "cd /d \"" << programDir << "\"\r\n";
            out << "\"" << nativeProgramPath << "\"\r\n";
            out << "set EXIT_CODE=%errorlevel%\r\n";
            out << "echo.\r\n";
            out << "echo 程序已退出，返回代码: %EXIT_CODE%\r\n";
            out << "pause\r\n";
        }
        batFile.close();
    }
    else
    {
        appendColoredText(tr("无法创建启动脚本"), errorFormat);
        return;
    }

    QString command = QString("cmd.exe /c \"%1\"").arg(QDir::toNativeSeparators(batPath));

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wchar_t commandW[4096];
    command.toWCharArray(commandW);
    commandW[command.length()] = '\0';

    if (CreateProcess(NULL, commandW, NULL, NULL, FALSE,
                      CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        hRunningProcess = pi.hProcess;
        currentRunningExe = programPath;
        CloseHandle(pi.hThread);

        outputEdit->appendPlainText("");
        appendColoredText(tr("程序已在控制台窗口中启动"), successFormat);
    }
    else
    {
        DWORD err = GetLastError();
        appendColoredText(tr("无法运行程序: %1 (错误代码: %2)").arg(programPath).arg(err), errorFormat);

        QProcess *process = new QProcess(this);
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString currentPath = env.value("PATH");
        env.insert("PATH", QCoreApplication::applicationDirPath() + "/mingw/bin;" + currentPath);
        process->setProcessEnvironment(env);

        if (process->startDetached(programPath))
        {
            appendDefaultText(tr("已通过备用方法启动程序"));
        }
        else
        {
            appendColoredText(tr("备用启动方法也失败"), errorFormat);
        }
        delete process;
    }
}

void CompilerIDE::run()
{
    resetOutputFormat();

    outputEdit->clear();

    if (outputDock)
    {
        outputDock->raise();
    }

    if (errorTableWidget)
    {
        errorTableWidget->setRowCount(0);
    }

    if (curFile.isEmpty())
    {
        qDebug() << "错误: curFile 为空";
        if (outputEdit)
        {
            outputEdit->appendPlainText(tr("错误：没有打开的文件"));
        }
        return;
    }

    if (!outputEdit)
    {
        qDebug() << "错误: outputEdit 为空指针";
        return;
    }

    QString programPath;
    QString tempDir = QDir::temp().absolutePath() + "/CompilerIDE_Build";
    QString program = QFileInfo(curFile).baseName();

    if (program.isEmpty())
    {
        qDebug() << "错误: 无法获取文件基本名";
        outputEdit->appendPlainText(tr("错误：无效的文件名"));
        return;
    }

#ifdef Q_OS_WIN
    program += ".exe";
#endif

    QString tempOriginalExe = tempDir + "/" + program;
    QString tempSourceExe = tempDir + "/temp_source.exe";
    QFileInfo curFileInfo(curFile);
    QString originalPath = curFileInfo.canonicalPath();

    if (originalPath.isEmpty())
    {
        originalPath = curFileInfo.absolutePath();
    }

    if (!originalPath.isEmpty())
    {
        originalPath = QDir(originalPath).filePath(program);
    }
    else
    {
        originalPath = QDir(curFileInfo.absolutePath()).filePath(program);
    }

    if (QFile::exists(tempOriginalExe))
    {
        programPath = tempOriginalExe;
        qDebug() << "使用临时目录原始名文件:" << tempOriginalExe;
    }
    else if (QFile::exists(originalPath))
    {
        programPath = originalPath;
        qDebug() << "使用原始路径文件:" << originalPath;
    }
    else if (QFile::exists(tempSourceExe))
    {
        programPath = tempSourceExe;
        qDebug() << "使用临时编译文件: temp_source.exe";
    }
    else
    {
        appendColoredText(tr("错误：找不到可执行文件"), errorFormat);
        appendDefaultText(tr("已检查路径:"));
        appendDefaultText("  1. " + tempOriginalExe); // 调整显示顺序
        appendDefaultText("  2. " + originalPath);
        appendDefaultText("  3. " + tempSourceExe);

        QDir dir(tempDir);
        if (dir.exists())
        {
            QStringList files = dir.entryList(QDir::Files);
            outputEdit->appendPlainText("");
            appendDefaultText(tr("临时目录实际文件列表:"));
            if (files.isEmpty())
            {
                appendDefaultText(tr("  (目录为空)"));
            }
            else
            {
                for (const QString &file : files)
                {
                    appendDefaultText("  - " + file);
                }
            }
        }
        else
        {
            appendDefaultText(tr("临时目录不存在: %1").arg(tempDir));
        }
        return;
    }

    qDebug() << "最终执行路径:" << programPath;

    if (programPath.isEmpty())
    {
        qDebug() << "错误: programPath 为空";
        appendColoredText(tr("错误：无法确定可执行文件路径"), errorFormat);
        return;
    }

    runExecutable(programPath, false);
}

void CompilerIDE::compileAndRun()
{
    if (hRunningProcess != NULL)
    {
        TerminateProcess(hRunningProcess, 0);
        WaitForSingleObject(hRunningProcess, 600);
        CloseHandle(hRunningProcess);
        hRunningProcess = NULL;
        QThread::msleep(50);
    }

    if (!currentRunningExe.isEmpty())
    {
        QString exeName = QFileInfo(currentRunningExe).fileName();
        killProcessByName(exeName);
        QThread::msleep(50);
    }

    outputEdit->clear();

    if (outputDock)
    {
        outputDock->raise();
    }

    accumulatedErrors.clear();
    accumulatedWarnings.clear();
    isNewCompilation = true;

    QTextCursor cursor = outputEdit->textCursor();
    cursor.setCharFormat(defaultFormat);

    if (errorTableWidget)
    {
        errorTableWidget->setRowCount(0);
    }

    disconnect(this, &CompilerIDE::compilationFinished, nullptr, nullptr);

    pendingRunAfterCompile = true;

    compile();
}

void CompilerIDE::clearOutput()
{
    outputEdit->clear();
    errorTableWidget->setRowCount(0);
}

void CompilerIDE::compilerFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)

    if (compileProcess)
    {
        compileProcess->disconnect();
    }

    outputDock->show();
    outputDock->raise();
    outputEdit->verticalScrollBar()->setValue(outputEdit->verticalScrollBar()->maximum());

    QTextCursor cursor = outputEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.setCharFormat(defaultFormat);

    if (exitCode == 0)
    {
        QString fullProgramPath;

        if (usingTempDir)
        {
            QString realBaseName = QFileInfo(lastCompiledFile).baseName();
            fullProgramPath = QDir::temp().absolutePath() + "/CompilerIDE_Build/" + realBaseName + ".exe";
            qDebug() << "使用临时目录，路径:" << fullProgramPath;

            if (QFile::exists(fullProgramPath))
            {
                qDebug() << "文件存在";
                QFileInfo fileInfo(fullProgramPath);
                qDebug() << "文件大小:" << fileInfo.size() << "字节";
                qDebug() << "文件权限:" << fileInfo.permissions();
            }
            else
            {
                qDebug() << "文件不存在！";
                resetOutputFormat();
                outputEdit->appendPlainText(tr("错误：编译成功但找不到可执行文件"));
                outputEdit->appendPlainText(tr("预期路径: %1").arg(fullProgramPath));

                QString tempDir = QDir::temp().absolutePath() + "/CompilerIDE_Build";
                QDir dir(tempDir);
                QStringList files = dir.entryList(QDir::Files);
                outputEdit->appendPlainText(tr("临时目录文件列表:"));
                if (files.isEmpty())
                {
                    outputEdit->appendPlainText(tr("  (目录为空)"));
                }
                else
                {
                    for (const QString &file : files)
                    {
                        QFileInfo info(dir.absoluteFilePath(file));
                        outputEdit->appendPlainText(QString("  - %1 (%2 字节)")
                                                    .arg(file).arg(info.size()));
                    }
                }

                goto cleanup;
            }
        }
        else
        {
            fullProgramPath = QFileInfo(lastCompiledFile).absolutePath() + "/" +
                              QFileInfo(lastCompiledFile).baseName() + ".exe";

            qDebug() << "使用原始目录，路径:" << fullProgramPath;
        }

        qDebug() << "编译成功，程序路径:" << fullProgramPath;

        outputEdit->appendPlainText("");
        appendColoredText(tr("编译成功！"), successFormat);

        cursor = outputEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.setCharFormat(defaultFormat);
        outputEdit->setTextCursor(cursor);

        if (pendingRunAfterCompile)
        {
            runExecutable(fullProgramPath, true);
        }

        if (usingTempDir && !originalOutputExe.isEmpty())
        {
            QString realBaseName = QFileInfo(lastCompiledFile).baseName();
            QString tempExe = QDir::temp().absolutePath() + "/CompilerIDE_Build/" + realBaseName + ".exe";

            if (QFile::exists(tempExe))
            {
                if (QFile::exists(originalOutputExe))
                {
                    QFile::remove(originalOutputExe);
                }

                if (QFile::copy(tempExe, originalOutputExe))
                {
                    appendDefaultText(tr("可执行文件已复制到: %1").arg(originalOutputExe));
                }
                else
                {
                    appendDefaultText(tr("警告: 无法将可执行文件复制到原目录"));
                }
            }
        }

        if (pendingDebugAfterCompile)
        {
            pendingDebugAfterCompile = false;

            resetOutputFormat();
            outputEdit->appendPlainText(tr("准备启动调试器..."));
            statusBar()->showMessage(tr("启动调试器..."));

            QString sourceFile = lastCompiledFile;

            QTimer::singleShot(300, this, [this, sourceFile]()
            {
                if (sourceFile.isEmpty())
                {
                    QMessageBox::warning(this, tr("调试错误"), tr("没有有效的源文件"));
                    debugStartAct->setEnabled(true);
                    return;
                }

                CodeEditor *editor = currentEditor();
                if (editor && debugger)
                {
                    QSet<int> editorBreakpoints = editor->getBreakpoints();
                    qDebug() << "同步断点数量:" << editorBreakpoints.size();

                    for (int line : editorBreakpoints)
                    {
                        debugger->setBreakpoint(sourceFile, line);
                        outputEdit->appendPlainText(tr("设置断点: 第 %1 行").arg(line));
                    }
                }

                launchDebugger(sourceFile);
            });
        }

        if (usingTempDir)
        {
            QTimer::singleShot(2000, this, [this]()
            {
                QString realBaseName = QFileInfo(lastCompiledFile).baseName();
                QString tempExe = QDir::temp().absolutePath() + "/CompilerIDE_Build/" + realBaseName + ".exe";
                if (QFile::exists(tempExe))
                {
                    QFile::remove(tempExe);
                }
                if (!tempSourceFile.isEmpty() && QFile::exists(tempSourceFile))
                {
                    QFile::remove(tempSourceFile);
                }

                usingTempDir = false;
                tempSourceFile.clear();
                originalOutputExe.clear();
            });
        }

        QTimer::singleShot(0, this, [this]()
        {
            emit compilationFinished(true);
        });
    }
    else
    {
        outputEdit->appendPlainText("");
        appendColoredText(tr("编译失败！"), errorFormat);

        cursor = outputEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.setCharFormat(defaultFormat);
        outputEdit->setTextCursor(cursor);

        errorDock->setVisible(true);
        QTimer::singleShot(50, this, [this]()
        {
            if (errorDock && errorTableWidget->rowCount() > 0)
            {
                errorDock->raise();
            }
        });

        QTimer::singleShot(0, this, [this]()
        {
            emit compilationFinished(false);
        });
    }

cleanup:
    pendingRunAfterCompile = false;
    pendingDebugAfterCompile = false;

    if (exitCode != 0)
    {
        if (usingTempDir)
        {
            usingTempDir = false;
            tempSourceFile.clear();
            originalOutputExe.clear();
        }

        if (debugStartAct)
        {
            debugStartAct->setEnabled(true);
        }
        statusBar()->showMessage(tr("编译失败"), 3000);
    }

    if (compileProcess)
    {
        QProcess *process = compileProcess;
        compileProcess = nullptr;

        QTimer::singleShot(100, this, [process]()
        {
            if (process)
            {
                process->deleteLater();
            }
        });
    }

    QTimer::singleShot(150, [this]()
    {
        if (errorTableWidget)
        {
            errorTableWidget->update();
        }
        if (errorDock)
        {
            errorDock->update();
        }
    });
}

void CompilerIDE::readCompilerOutput()
{
    QString output = QString::fromLocal8Bit(compileProcess->readAllStandardOutput());
    if (!output.trimmed().isEmpty())
    {
        parseCompilerOutput(output);
    }
}

void CompilerIDE::readCompilerError()
{
    if (!compileProcess)
    {
        return;
    }

    QString errorOutput = QString::fromLocal8Bit(
                              compileProcess->readAllStandardError());
    QString standardOutput = QString::fromLocal8Bit(
                                 compileProcess->readAllStandardOutput());

    QString allOutput;
    if (!errorOutput.trimmed().isEmpty())
    {
        allOutput += errorOutput;
    }
    if (!standardOutput.trimmed().isEmpty())
    {
        if (!allOutput.isEmpty())
        {
            allOutput += "\n";
        }
        allOutput += standardOutput;
    }

    if (!allOutput.trimmed().isEmpty())
    {
        qDebug() << "[编译器输出]" << allOutput;
        parseCompilerOutput(allOutput);
    }
}

void CompilerIDE::updateErrorTable(const QStringList &errors, const QStringList &warnings)
{
    if (!errorTableWidget)
    {
        return;
    }

    errorTableWidget->setRowCount(0);

    int row = 0;

    QColor errorColor, warningColor, backgroundColor;
    if (darkTheme)
    {
        errorColor = Qt::red;
        warningColor = QColor(255, 165, 0);
        backgroundColor = QColor(25, 25, 25);
    }
    else
    {
        errorColor = Qt::darkRed;
        warningColor = QColor(200, 100, 0);
        backgroundColor = Qt::white;
    }

    for (const QString &error : errors)
    {
        QStringList parts = error.split('|');

        if (parts.size() >= 4)
        {
            QString filePath = parts[0];
            QString lineNum = parts[1];
            QString colNum = parts[2];
            QString message = parts[3];

            errorTableWidget->insertRow(row);

            QTableWidgetItem *unitItem = new QTableWidgetItem(filePath);
            unitItem->setForeground(errorColor);
            unitItem->setBackground(backgroundColor);
            unitItem->setToolTip(filePath);

            QTableWidgetItem *lineItem = new QTableWidgetItem(
                lineNum == "0" ? tr("未知") : lineNum
            );
            lineItem->setForeground(errorColor);
            lineItem->setTextAlignment(Qt::AlignCenter);
            lineItem->setBackground(backgroundColor);

            QTableWidgetItem *columnItem = new QTableWidgetItem(
                colNum == "0" ? tr("未知") : colNum
            );
            columnItem->setForeground(errorColor);
            columnItem->setTextAlignment(Qt::AlignCenter);
            columnItem->setBackground(backgroundColor);

            QString displayMsg = QString("[error] %1").arg(message);
            QTableWidgetItem *messageItem = new QTableWidgetItem(displayMsg);
            messageItem->setForeground(errorColor);
            messageItem->setBackground(backgroundColor);
            messageItem->setToolTip(error);

            errorTableWidget->setItem(row, 0, unitItem);
            errorTableWidget->setItem(row, 1, lineItem);
            errorTableWidget->setItem(row, 2, columnItem);
            errorTableWidget->setItem(row, 3, messageItem);

            row++;
        }
        else if (parts.size() >= 3)
        {
            QString lineNum = parts[0];
            QString colNum = parts[1];
            QString message = parts[2];

            errorTableWidget->insertRow(row);

            QTableWidgetItem *unitItem = new QTableWidgetItem(curFile);
            unitItem->setForeground(errorColor);
            unitItem->setBackground(backgroundColor);

            QTableWidgetItem *lineItem = new QTableWidgetItem(
                lineNum == "0" ? tr("未知") : lineNum
            );
            lineItem->setForeground(errorColor);
            lineItem->setTextAlignment(Qt::AlignCenter);
            lineItem->setBackground(backgroundColor);

            QTableWidgetItem *columnItem = new QTableWidgetItem(
                colNum == "0" ? tr("未知") : colNum
            );
            columnItem->setForeground(errorColor);
            columnItem->setTextAlignment(Qt::AlignCenter);
            columnItem->setBackground(backgroundColor);

            QString displayMsg = QString("[error] %1").arg(message);
            QTableWidgetItem *messageItem = new QTableWidgetItem(displayMsg);
            messageItem->setForeground(errorColor);
            messageItem->setBackground(backgroundColor);

            errorTableWidget->setItem(row, 0, unitItem);
            errorTableWidget->setItem(row, 1, lineItem);
            errorTableWidget->setItem(row, 2, columnItem);
            errorTableWidget->setItem(row, 3, messageItem);

            row++;
        }
    }

    for (const QString &warning : warnings)
    {
        QStringList parts = warning.split('|');

        if (parts.size() >= 4)
        {
            QString filePath = parts[0];
            QString lineNum = parts[1];
            QString colNum = parts[2];
            QString message = parts[3];

            errorTableWidget->insertRow(row);

            QTableWidgetItem *unitItem = new QTableWidgetItem(filePath);
            unitItem->setForeground(warningColor);
            unitItem->setBackground(backgroundColor);
            unitItem->setToolTip(filePath);

            QTableWidgetItem *lineItem = new QTableWidgetItem(
                lineNum == "0" ? tr("未知") : lineNum
            );
            lineItem->setForeground(warningColor);
            lineItem->setTextAlignment(Qt::AlignCenter);
            lineItem->setBackground(backgroundColor);

            QTableWidgetItem *columnItem = new QTableWidgetItem(
                colNum == "0" ? tr("未知") : colNum
            );
            columnItem->setForeground(warningColor);
            columnItem->setTextAlignment(Qt::AlignCenter);
            columnItem->setBackground(backgroundColor);

            QString displayMsg = QString("[warning] %1").arg(message);
            QTableWidgetItem *messageItem = new QTableWidgetItem(displayMsg);
            messageItem->setForeground(warningColor);
            messageItem->setBackground(backgroundColor);
            messageItem->setToolTip(warning);

            errorTableWidget->setItem(row, 0, unitItem);
            errorTableWidget->setItem(row, 1, lineItem);
            errorTableWidget->setItem(row, 2, columnItem);
            errorTableWidget->setItem(row, 3, messageItem);

            row++;
        }
        else if (parts.size() >= 3)
        {
            QString lineNum = parts[0];
            QString colNum = parts[1];
            QString message = parts[2];

            errorTableWidget->insertRow(row);

            QTableWidgetItem *unitItem = new QTableWidgetItem(curFile);
            unitItem->setForeground(warningColor);
            unitItem->setBackground(backgroundColor);

            QTableWidgetItem *lineItem = new QTableWidgetItem(
                lineNum == "0" ? tr("未知") : lineNum
            );
            lineItem->setForeground(warningColor);
            lineItem->setTextAlignment(Qt::AlignCenter);
            lineItem->setBackground(backgroundColor);

            QTableWidgetItem *columnItem = new QTableWidgetItem(
                colNum == "0" ? tr("未知") : colNum
            );
            columnItem->setForeground(warningColor);
            columnItem->setTextAlignment(Qt::AlignCenter);
            columnItem->setBackground(backgroundColor);

            QString displayMsg = QString("[warning] %1").arg(message);
            QTableWidgetItem *messageItem = new QTableWidgetItem(displayMsg);
            messageItem->setForeground(warningColor);
            messageItem->setBackground(backgroundColor);

            errorTableWidget->setItem(row, 0, unitItem);
            errorTableWidget->setItem(row, 1, lineItem);
            errorTableWidget->setItem(row, 2, columnItem);
            errorTableWidget->setItem(row, 3, messageItem);

            row++;
        }
    }

    errorTableWidget->resizeColumnToContents(0);
    if (errorTableWidget->columnWidth(0) < 300)
    {
        errorTableWidget->setColumnWidth(0, 300);
    }
    errorTableWidget->setColumnWidth(1, 60);
    errorTableWidget->setColumnWidth(2, 60);

    if (row > 0)
    {
        errorDock->setVisible(true);
        qDebug() << "错误表格已更新，行数:" << row;
    }
}

QPair<QString, QString> CompilerIDE::extractLineAndColumn(const QString &errorLine)
{
    QString lineNum = "0";
    QString colNum = "0";

    QVector<QRegularExpression> patterns =
    {
        QRegularExpression(R"(^[^:]+:(\d+):(\d+):)"),
        QRegularExpression(R"(^[^\(]+\((\d+),(\d+)\):)"),
        QRegularExpression(R"(^[^:]+:(\d+):)"),
        QRegularExpression(R"((\d+):(\d+))"),
        QRegularExpression(R"(\((\d+),(\d+)\))")
    };

    for (const QRegularExpression &pattern : patterns)
    {
        QRegularExpressionMatch match = pattern.match(errorLine);
        if (match.hasMatch())
        {
            lineNum = match.captured(1);
            if (match.capturedLength() > 1)
            {
                colNum = match.captured(2);
            }
            else
            {
                colNum = "0";
            }
            qDebug() << "使用模式提取 - 行:" << lineNum << "列:" << colNum;
            break;
        }
    }

    return qMakePair(lineNum, colNum);
}

void CompilerIDE::parseCompilerOutput(const QString &output)
{
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    if (isNewCompilation)
    {
        accumulatedErrors.clear();
        accumulatedWarnings.clear();
        isNewCompilation = false;

        if (errorTableWidget)
        {
            errorTableWidget->setRowCount(0);
        }
        QApplication::processEvents();
    }

    QSet<QString> processedLines;

    qDebug() << "原始编译器输出:" << output;

    QRegularExpression gccErrorPattern(
        R"(^(.+):(\d+):(\d+):\s*(error|错误):\s*(.+)$)"
    );

    QRegularExpression gccWarningPattern(
        R"(^(.+):(\d+):(\d+):\s*(warning|警告):\s*(.+)$)"
    );

    QRegularExpression msvcErrorPattern(
        R"(^([^\(]+)\((\d+),(\d+)\):\s+(error|错误)\s+([^:]+):\s*(.+)$)"
    );

    QRegularExpression msvcWarningPattern(
        R"(^([^\(]+)\((\d+),(\d+)\):\s+(warning|警告)\s+([^:]+):\s*(.+)$)"
    );

    QRegularExpression simpleErrorPattern(
        R"(^([^:]+):(\d+):\s+(.+)$)"
    );

    bool hasNewErrors = false;
    bool hasNewWarnings = false;

    for (const QString &line : lines)
    {
        QString trimmedLine = line.trimmed();
        if (trimmedLine.isEmpty())
        {
            continue;
        }

        qDebug() << "处理行:" << trimmedLine;

        bool matched = false;

        QRegularExpressionMatch gccErrorMatch = gccErrorPattern.match(line);
        if (gccErrorMatch.hasMatch())
        {
            QString file = gccErrorMatch.captured(1);
            QString lineNum = gccErrorMatch.captured(2);
            QString colNum = gccErrorMatch.captured(3);
            QString message = gccErrorMatch.captured(5);

            QString fullError = QString("%1|%2|%3|%4").arg(file, lineNum, colNum, message);
            if (!accumulatedErrors.contains(fullError))
            {
                accumulatedErrors.append(fullError);
                hasNewErrors = true;
                qDebug() << "发现GCC错误 - 文件:" << file << "行:" << lineNum << "列:" << colNum << "消息:" << message;
            }
            matched = true;
            continue;
        }

        QRegularExpressionMatch gccWarningMatch = gccWarningPattern.match(line);
        if (gccWarningMatch.hasMatch())
        {
            QString file = gccWarningMatch.captured(1);
            QString lineNum = gccWarningMatch.captured(2);
            QString colNum = gccWarningMatch.captured(3);
            QString message = gccWarningMatch.captured(5);

            QString fullWarning = QString("%1|%2|%3|%4").arg(file, lineNum, colNum, message);
            if (!accumulatedWarnings.contains(fullWarning))
            {
                accumulatedWarnings.append(fullWarning);
                hasNewWarnings = true;
            }
            matched = true;
            continue;
        }

        QRegularExpressionMatch msvcErrorMatch = msvcErrorPattern.match(line);
        if (msvcErrorMatch.hasMatch())
        {
            QString file = msvcErrorMatch.captured(1);
            QString lineNum = msvcErrorMatch.captured(2);
            QString colNum = msvcErrorMatch.captured(3);
            QString message = msvcErrorMatch.captured(6);
            QString fullError = QString("%1|%2|%3|%4").arg(file, lineNum, colNum, message);
            if (!accumulatedErrors.contains(fullError))
            {
                accumulatedErrors.append(fullError);
                hasNewErrors = true;
            }
            matched = true;
            continue;
        }

        QRegularExpressionMatch msvcWarningMatch = msvcWarningPattern.match(line);
        if (msvcWarningMatch.hasMatch())
        {
            QString file = msvcWarningMatch.captured(1);
            QString lineNum = msvcWarningMatch.captured(2);
            QString colNum = msvcWarningMatch.captured(3);
            QString message = msvcWarningMatch.captured(6);
            QString fullWarning = QString("%1|%2|%3|%4").arg(file, lineNum, colNum, message);
            if (!accumulatedWarnings.contains(fullWarning))
            {
                accumulatedWarnings.append(fullWarning);
                hasNewWarnings = true;
            }
            matched = true;
            continue;
        }

        QRegularExpressionMatch simpleErrorMatch = simpleErrorPattern.match(line);
        if (simpleErrorMatch.hasMatch())
        {
            QString file = simpleErrorMatch.captured(1);
            QString lineNum = simpleErrorMatch.captured(2);
            QString message = simpleErrorMatch.captured(3);

            if (message.contains("error", Qt::CaseInsensitive) ||
                    message.contains("错误", Qt::CaseInsensitive))
            {
                QString fullError = QString("%1|%2|0|%3").arg(file, lineNum, message);
                if (!accumulatedErrors.contains(fullError))
                {
                    accumulatedErrors.append(fullError);
                    hasNewErrors = true;
                }
            }
            else if (message.contains("warning", Qt::CaseInsensitive) ||
                     message.contains("警告", Qt::CaseInsensitive))
            {
                QString fullWarning = QString("%1|%2|0|%3").arg(file, lineNum, message);
                if (!accumulatedWarnings.contains(fullWarning))
                {
                    accumulatedWarnings.append(fullWarning);
                    hasNewWarnings = true;
                }
            }
            matched = true;
            continue;
        }

        if (!matched && (line.contains("error:", Qt::CaseInsensitive) ||
                         line.contains("错误:", Qt::CaseInsensitive)))
        {
            auto lineCol = extractLineAndColumn(line);
            QString lineNum = lineCol.first;
            QString colNum = lineCol.second;

            QString pureMessage = trimmedLine;
            int errorIdx = line.indexOf("error:", Qt::CaseInsensitive);
            if (errorIdx != -1)
            {
                pureMessage = line.mid(errorIdx + 6).trimmed();
            }
            else
            {
                int errorIdxCN = line.indexOf("错误:");
                if (errorIdxCN != -1)
                {
                    pureMessage = line.mid(errorIdxCN + 3).trimmed();
                }
            }

            QString filePath = curFile;
            QRegularExpression filePattern(R"(^(.+):(\d+):(\d+):)");
            QRegularExpressionMatch fileMatch = filePattern.match(line);
            if (fileMatch.hasMatch())
            {
                filePath = fileMatch.captured(1);
            }

            QString fullError = QString("%1|%2|%3|%4").arg(filePath, lineNum, colNum, pureMessage);
            if (!accumulatedErrors.contains(fullError))
            {
                accumulatedErrors.append(fullError);
                hasNewErrors = true;
                qDebug() << "发现关键词错误:" << fullError;
            }
        }
        else if (!matched && (line.contains("warning:", Qt::CaseInsensitive) ||
                              line.contains("警告:", Qt::CaseInsensitive)))
        {
            auto lineCol = extractLineAndColumn(line);
            QString lineNum = lineCol.first;
            QString colNum = lineCol.second;
            QString pureMessage = trimmedLine;
            int warningIdx = line.indexOf("warning:", Qt::CaseInsensitive);
            if (warningIdx != -1)
            {
                pureMessage = line.mid(warningIdx + 8).trimmed();
            }
            else
            {
                int warningIdxCN = line.indexOf("警告:");
                if (warningIdxCN != -1)
                {
                    pureMessage = line.mid(warningIdxCN + 3).trimmed();
                }
            }

            QString filePath = curFile;
            QRegularExpression filePattern(R"(^(.+):(\d+):(\d+):)");
            QRegularExpressionMatch fileMatch = filePattern.match(line);
            if (fileMatch.hasMatch())
            {
                filePath = fileMatch.captured(1);
            }

            QString fullWarning = QString("%1|%2|%3|%4").arg(filePath, lineNum, colNum, pureMessage);
            if (!accumulatedWarnings.contains(fullWarning))
            {
                accumulatedWarnings.append(fullWarning);
                hasNewWarnings = true;
                qDebug() << "发现关键词警告:" << fullWarning;
            }
        }
    }
    if (hasNewErrors || hasNewWarnings)
    {
        updateErrorTable(accumulatedErrors, accumulatedWarnings);
        QApplication::processEvents();

        qDebug() << QString("发现新错误/警告 - 错误: %1, 警告: %2")
                 .arg(accumulatedErrors.count())
                 .arg(accumulatedWarnings.count());
    }
}

void CompilerIDE::runProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus)
    outputEdit->appendPlainText("\n");
    outputEdit->appendPlainText(tr("'%1' (进程 %2) 已退出，代码为 %3 .")
                                .arg(runProcess->program())
                                .arg(runProcess->processId())
                                .arg(exitCode));
    outputEdit->appendPlainText(tr("按任意键关闭此窗口. . ."));
}

void CompilerIDE::readRunOutput()
{
    QString output = QString::fromLocal8Bit(runProcess->readAllStandardOutput());
    QString error = QString::fromLocal8Bit(runProcess->readAllStandardError());
    if (!output.isEmpty())
    {
        outputEdit->appendPlainText(output);
    }
    if (!error.isEmpty())
    {
        outputEdit->appendPlainText(error);
    }
}

void CompilerIDE::showErrorInCode(int row, int column)
{
    Q_UNUSED(column)
    if (!errorTableWidget || row < 0 || row >= errorTableWidget->rowCount())
    {
        return;
    }
    QTableWidgetItem *lineItem = errorTableWidget->item(row, 1);
    QTableWidgetItem *columnItem = errorTableWidget->item(row, 2);
    QTableWidgetItem *typeItem = errorTableWidget->item(row, 3);
    if (!lineItem)
    {
        return;
    }
    QString lineText = lineItem->text();
    QString columnText = columnItem ? columnItem->text() : "0";
    QString typeText = typeItem ? typeItem->text() : "";
    if (lineText == tr("未知"))
    {
        return;
    }
    bool ok;
    int lineNumber = lineText.toInt(&ok) - 1;
    if (!ok || lineNumber < 0)
    {
        return;
    }
    int columnNumber = 0;
    if (columnText != tr("未知"))
    {
        columnNumber = columnText.toInt(&ok) - 1;
        if (!ok)
        {
            columnNumber = 0;
        }
    }
    CodeEditor *editor = currentEditor();
    if (editor)
    {
        editor->clearErrorWarningLines();
        QTextBlock block = editor->document()->findBlockByNumber(lineNumber);
        if (block.isValid())
        {
            QTextCursor cursor(block);
            if (columnNumber > 0 && columnNumber < block.length())
            {
                cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, columnNumber);
            }
            editor->setTextCursor(cursor);
            editor->centerCursor();
            bool isError = typeText.contains("错误") ||
                           typeText.contains("error") ||
                           typeText.toLower().contains("error");
            if (isError)
            {
                editor->setErrorLine(lineNumber + 1);
            }
            else
            {
                editor->setWarningLine(lineNumber + 1);
            }
            qDebug() << "定位到" << (isError ? "错误" : "警告")
                     << ": 行" << (lineNumber + 1) << "列" << (columnNumber + 1);
        }
        else
        {
            qDebug() << "无效的行号:" << lineNumber;
        }
    }
    else
    {
        qDebug() << "没有活动的编辑器或无效行号";
    }
}

void CompilerIDE::updateStatusBar()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        statusBar()->showMessage(tr("就绪"));
        return;
    }
    if (curFile.isEmpty())
    {
        statusBar()->showMessage(tr("无标题 - 第 %1 行, 第 %2 列 | 已修改 | %3")
                                 .arg(editor->textCursor().blockNumber() + 1)
                                 .arg(editor->textCursor().columnNumber() + 1)
                                 .arg(cppStandard));
    }
    else
    {
        statusBar()->showMessage(tr("%1 - 第 %2 行, 第 %3 列 | %4 | %5")
                                 .arg(strippedName(curFile))
                                 .arg(editor->textCursor().blockNumber() + 1)
                                 .arg(editor->textCursor().columnNumber() + 1)
                                 .arg(cppStandard)
                                 .arg(editor->document()->isModified() ? tr("已修改") : tr("已保存")));
    }
}

void CompilerIDE::changeCppStandard(int index)
{
    QString standard = cppStandardCombo->itemText(index).toLower();
    cppStandard = standard;
    updateStatusBar();
}

void CompilerIDE::showSettings()
{
    SettingsDialog dialog(this, compilerPath, debuggerPath, autoBrackets, autoQuotes, autoIndent,
                          indentSize, lineNumbers, darkTheme,
                          codeBeautify, codeCompletionEnabled, editorFont);
    if (dialog.exec() == QDialog::Accepted)
    {
        debuggerPath = dialog.getDebuggerPath();
        codeBeautify = dialog.getCodeBeautify();
        codeCompletionEnabled = dialog.getCodeCompletion();

        if (codeCompleter)
        {
            codeCompleter->reloadCustomCompletions();
        }

        applySettings(dialog.getCompilerPath(), dialog.getAutoBrackets(),
                      dialog.getAutoQuotes(), dialog.getAutoIndent(), dialog.getIndentSize(),
                      dialog.getLineNumbers(), dialog.getDarkTheme(),
                      codeCompletionEnabled, dialog.getEditorFont());
    }
}

void CompilerIDE::applySettings(const QString &compilerPath, bool autoBrackets,
                                bool autoQuotes, bool autoIndent, int indentSize,
                                bool lineNumbers, bool darkTheme,
                                bool codeCompletion, const QFont &editorFont)
{
    QString oldCompilerPath = this->compilerPath;
    bool oldDarkTheme = darkTheme;
    bool themeChanged = (this->darkTheme != darkTheme);
    qDebug() << "[applySettings] 新字体:" << editorFont.family() << editorFont.pointSize()
             << "旧字体:" << this->editorFont.family() << this->editorFont.pointSize();
    if (themeChanged)
    {
        if (!maybeSaveAll())
        {
            return;
        }
        this->darkTheme = darkTheme;
        this->compilerPath = compilerPath;
        this->autoBrackets = autoBrackets;
        this->autoQuotes = autoQuotes;
        this->autoIndent = autoIndent;
        this->codeCompletionEnabled = codeCompletion;
        this->editorFont = editorFont;
        this->indentSize = indentSize;
        this->lineNumbers = lineNumbers;
        this->compilerType = detectCompilerType(compilerPath);
        writeSettings();
        restartIDEWithMessage();
        return;
    }
    this->compilerPath = compilerPath;
    this->autoBrackets = autoBrackets;
    this->autoQuotes = autoQuotes;
    this->editorFont = editorFont;
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setEditorFont(editorFont);
            QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
            editor->setShowIndentGuides(settings.value("showIndentGuides", true).toBool());
        }
    }
    this->autoIndent = autoIndent;
    this->indentSize = indentSize;
    this->codeCompletionEnabled = codeCompletion;
    this->lineNumbers = lineNumbers;
    if (this->darkTheme != darkTheme)
    {
        this->darkTheme = darkTheme;
        setTheme(darkTheme);
    }
    if (oldDarkTheme != darkTheme)
    {
        setTheme(darkTheme);
    }
    this->compilerType = detectCompilerType(compilerPath);
    if (oldCompilerPath != compilerPath)
    {
        qDebug() << "编译器路径已更改，重新检测支持的标准";
        checkCompilerSupport();
    }
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        CodeEditor *editor = editorAt(i);
        if (editor)
        {
            editor->setAutoBracketsEnabled(autoBrackets);
            editor->setAutoQuotesEnabled(autoQuotes);
            editor->setAutoIndentEnabled(autoIndent);
            editor->setCompletionEnabled(codeCompletion);
            editor->setIndentSize(indentSize);
            editor->setLineNumbersEnabled(lineNumbers);
            editor->setCodeBeautifyEnabled(codeBeautify);

            if (codeCompletion)
            {
                editor->setCompletionEnabled(true);
                editor->setCompetitionMode(false);
            }
            else
            {
                editor->setCompletionEnabled(false);
                editor->setCompetitionMode(true);
            }
        }
    }
    writeSettings();
    updateStatusBar();
}

void CompilerIDE::toggleComment()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }
    QTextCursor cursor = editor->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::LineUnderCursor);
    }
    QString selectedText = cursor.selectedText();
    QStringList lines = selectedText.split(QChar::ParagraphSeparator);
    bool allCommented = true;
    for (const QString &line : lines)
    {
        if (!line.trimmed().startsWith("//") && !line.trimmed().isEmpty())
        {
            allCommented = false;
            break;
        }
    }
    for (int i = 0; i < lines.size(); ++i)
    {
        if (allCommented)
        {
            if (lines[i].trimmed().startsWith("//"))
            {
                int commentPos = lines[i].indexOf("//");
                lines[i].remove(commentPos, 2);
            }
        }
        else
        {
            if (!lines[i].trimmed().isEmpty() && !lines[i].trimmed().startsWith("//"))
            {
                int firstNonSpace = 0;
                while (firstNonSpace < lines[i].length() && lines[i][firstNonSpace].isSpace())
                {
                    firstNonSpace++;
                }
                lines[i].insert(firstNonSpace, "//");
            }
        }
    }
    cursor.insertText(lines.join(QChar::ParagraphSeparator));
}

void CompilerIDE::onFileSelected(const QString &filePath)
{
    loadFile(filePath);
}

void CompilerIDE::closeTab(int index)
{
    QWidget *widget = tabWidget->widget(index);
    if (!widget)
    {
        return;
    }

    CodeEditor *editor = qobject_cast<CodeEditor*>(widget);

    if (editor)
    {
        QString originalCurFile = curFile;
        QString tabFilePath = tabWidget->tabToolTip(index);
        curFile = tabFilePath;

        if (editor->document()->isModified())
        {
            tabWidget->setCurrentIndex(index);
            if (!maybeSave())
            {
                curFile = originalCurFile;
                return;
            }
        }

        curFile = originalCurFile;
        editorEasyXFlags.remove(editor);
    }

    tabWidget->removeTab(index);

    if (!editor)
    {
        widget->deleteLater();
    }
    else
    {
        delete widget;
    }

    if (tabWidget->count() == 0)
    {
        setCurrentFile("");
    }
    else
    {
        currentTabChanged(tabWidget->currentIndex());
    }
}

void CompilerIDE::currentTabChanged(int index)
{
    if (index < 0)
    {
        setCurrentFile("");
        return;
    }
    CodeEditor *editor = editorAt(index);
    if (editor)
    {
        editor->setCompleter(codeCompleter);
        editor->clearErrorWarningLines();
        if (competitionMode && comp_disableCodeCompletion)
        {
            editor->setCompletionEnabled(false);
        }
        else
        {
            editor->setCompletionEnabled(true);
        }

        QString fullFilePath = tabWidget->tabToolTip(index);

        if (fullFilePath.isEmpty())
        {
            curFile = "";
        }
        else
        {
            curFile = fullFilePath;
        }

        connect(editor, &CodeEditor::cursorPositionChanged,
                this, &CompilerIDE::updateStatusBar, Qt::UniqueConnection);
        updateStatusBar();
    }
}

CodeEditor* CompilerIDE::currentEditor() const
{
    return qobject_cast<CodeEditor*>(tabWidget->currentWidget());
}

CodeEditor* CompilerIDE::editorAt(int index) const
{
    return qobject_cast<CodeEditor*>(tabWidget->widget(index));
}

int CompilerIDE::findEditor(const QString &fileName) const
{
    for (int i = 0; i < tabWidget->count(); ++i)
    {
        QString tabText = tabWidget->tabText(i);
        if (tabText.endsWith("*"))
        {
            tabText.chop(1);
        }
        if (tabText == QFileInfo(fileName).fileName())
        {
            return i;
        }
    }
    return -1;
}

void CompilerIDE::find()
{
    updateFindDialog();
    findDialog->show();
    findDialog->raise();
    findEdit->setFocus();
    findEdit->selectAll();
}

void CompilerIDE::replace()
{
    updateFindDialog();
    findDialog->show();
    findDialog->raise();
    findEdit->setFocus();
    findEdit->selectAll();
}

void CompilerIDE::findNext()
{
    performFind(true);
}

void CompilerIDE::findPrevious()
{
    performFind(false);
}

void CompilerIDE::gotoLine()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }
    bool ok;
    int line = QInputDialog::getInt(this, tr("转到行"), tr("行号:"),
                                    1, 1, editor->document()->blockCount(), 1, &ok);
    if (ok)
    {
        QTextBlock block = editor->document()->findBlockByLineNumber(line - 1);
        if (block.isValid())
        {
            QTextCursor cursor(block);
            editor->setTextCursor(cursor);
            editor->centerCursor();
        }
    }
}

void CompilerIDE::showProjectManager()
{
    projectDock->setVisible(projectManagerAct->isChecked());
}

void CompilerIDE::showFileBrowser()
{
    fileBrowserDock->setVisible(fileBrowserAct->isChecked());
}

void CompilerIDE::showSymbolBrowser()
{
    symbolBrowserDock->setVisible(symbolBrowserAct->isChecked());
}

void CompilerIDE::showOutline()
{
    outlineDock->setVisible(outlineAct->isChecked());
}

void CompilerIDE::zoomIn()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }
    editor->hideCompletion();

    QFont font = editor->font();
    int newSize = font.pointSize() + 1;
    font.setPointSize(newSize);
    editor->setFont(font);
    editor->updateLineNumberAreaWidth(0);

    editorFont.setPointSize(newSize);
    writeSettings();
}

void CompilerIDE::zoomOut()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }
    editor->hideCompletion();

    QFont font = editor->font();
    int newSize = font.pointSize() - 1;
    font.setPointSize(newSize);
    editor->setFont(font);
    editor->updateLineNumberAreaWidth(0);

    editorFont.setPointSize(newSize);
    writeSettings();
}

void CompilerIDE::resetZoom()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }
    editor->hideCompletion();

    int newSize = 11;
    QFont font = editor->font();
    font.setPointSize(newSize);
    editor->setFont(font);
    editor->updateLineNumberAreaWidth(0);

    editorFont.setPointSize(newSize);
    writeSettings();
}

void CompilerIDE::onEditorFontSizeChanged(int newSize)
{
    editorFont.setPointSize(newSize);
    writeSettings();
}

void CompilerIDE::updateFindDialog()
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return;
    }
    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection())
    {
        findEdit->setText(cursor.selectedText());
    }
}

bool CompilerIDE::performFind(bool forward, bool showMessage)
{
    CodeEditor *editor = currentEditor();
    if (!editor)
    {
        return false;
    }
    QString searchText = findEdit->text();
    if (searchText.isEmpty())
    {
        return false;
    }
    QTextDocument::FindFlags flags;
    if (!forward)
    {
        flags |= QTextDocument::FindBackward;
    }
    if (caseSensitiveCheck->isChecked())
    {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (wholeWordCheck->isChecked())
    {
        flags |= QTextDocument::FindWholeWords;
    }
    bool found = false;
    if (regexCheck->isChecked())
    {
        QRegularExpression regex(searchText);
        if (caseSensitiveCheck->isChecked())
        {
            regex.setPatternOptions(QRegularExpression::NoPatternOption);
        }
        else
        {
            regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
        found = editor->find(regex, flags);
    }
    else
    {
        found = editor->find(searchText, flags);
    }
    if (!found && showMessage)
    {
        QMessageBox::information(this, tr("查找"), tr("找不到\"%1\"").arg(searchText));
    }
    return found;
}

LuoguFetcherWidget::LuoguFetcherWidget(CompilerIDE *ide, QWidget *parent)
    : QWidget(parent), m_isLoading(false), m_currentPage(nullptr), mainWindow(ide)
{
    setStyleSheet(
        "QWidget { background-color: white; color: black; }"
        "QLineEdit { border: 1px solid #ccc; padding: 4px; border-radius: 3px; }"
        "QPushButton { background-color: #3498db; color: white; border: none; padding: 6px 12px; border-radius: 3px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #1c6a9c; }"
        "QPushButton:disabled { background-color: #bdc3c7; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(5, 5, 5, 5);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText(tr("输入题目ID,例如 P1001"));
    fetchButton = new QPushButton(tr("爬取"), this);
    copyButton = new QPushButton(tr("复制 Markdown"), this);
    copyButton->setEnabled(false);
    inputLayout->addWidget(idEdit);
    inputLayout->addWidget(fetchButton);
    inputLayout->addWidget(copyButton);
    mainLayout->addLayout(inputLayout);

    view = new QWebEngineView(this);
    mainLayout->addWidget(view);

    view->setHtml(
        "<!DOCTYPE html>"
        "<html>"
        "<head><meta charset='utf-8'><style>"
        "body{background:#f5f5f5;display:flex;align-items:center;justify-content:center;height:100vh;margin:0;font-family:sans-serif;color:#666;}"
        "</style></head>"
        "<body><h2>请输入题目ID并点击“爬取”</h2></body>"
        "</html>"
    );

    connect(idEdit, &QLineEdit::returnPressed, fetchButton, &QPushButton::click);
    connect(fetchButton, &QPushButton::clicked, this, &LuoguFetcherWidget::onFetchClicked);
    connect(copyButton, &QPushButton::clicked, this, &LuoguFetcherWidget::onCopyClicked);
}

void LuoguFetcherWidget::onFetchClicked()
{
    QString pid = idEdit->text().trimmed();
    if (pid.isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("请输入题目ID"));
        return;
    }
    QUrl url("https://www.luogu.com.cn/problem/" + pid);
    fetchProblem(url);
}

void LuoguFetcherWidget::onCopyClicked()
{
    if (!currentMarkdown.isEmpty())
    {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(currentMarkdown);
        if (mainWindow)
        {
            mainWindow->showFloatingMessage(tr("Markdown 已复制到剪贴板"), 2000, false);
        }
    }
}

void LuoguFetcherWidget::fetchProblem(const QUrl &url)
{
    if (m_isLoading)
    {
        if (m_currentPage)
        {
            m_currentPage->deleteLater();
            m_currentPage = nullptr;
        }
        m_isLoading = false;
    }

    QString loadingHtml = R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
            background-color: #f5f5f5;
        }
        .loading-container {
            text-align: center;
        }
        .loading-text {
            font-size: 24px;
            color: #333;
            margin-bottom: 20px;
        }
        .spinner {
            border: 4px solid #f3f3f3;
            border-top: 4px solid #3498db;
            border-radius: 50%;
            width: 40px;
            height: 40px;
            animation: spin 1s linear infinite;
            margin: 0 auto;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    </style>
</head>
<body>
    <div class="loading-container">
        <div class="loading-text">正在爬取题目...</div>
        <div class="spinner"></div>
    </div>
</body>
</html>
)";
    view->setHtml(loadingHtml);
    copyButton->setEnabled(false);
    m_isLoading = true;

    QWebEnginePage *page = new QWebEnginePage(QWebEngineProfile::defaultProfile(), this);
    m_currentPage = page;

    connect(page, &QWebEnginePage::loadFinished, this, [this, page](bool ok)
    {
        if (page != m_currentPage)
        {
            page->deleteLater();
            return;
        }

        if (!ok)
        {
            if (mainWindow)
            {
                mainWindow->showFloatingMessage(tr("页面加载失败，请检查网络或题目ID"), 3000, true);
            }
            view->setHtml(
                "<html><body style='text-align:center;padding:50px;font-family:sans-serif;'>"
                "<h2 style='color:red;'>加载失败</h2>"
                "<p>请检查网络连接或题目ID是否正确。</p>"
                "</body></html>"
            );
            page->deleteLater();
            if (m_currentPage == page)
            {
                m_currentPage = nullptr;
            }
            m_isLoading = false;
            return;
        }

        QTimer::singleShot(0, this, [this, page]()
        {
            if (page != m_currentPage)
            {
                page->deleteLater();
                return;
            }

            page->runJavaScript(R"(
        (function() {
            var script = document.getElementById('lentille-context');
            return script ? script.textContent : null;
        })();
    )", [this, page](const QVariant &result)
            {
                if (page != m_currentPage)
                {
                    page->deleteLater();
                    return;
                }

                QString jsonStr = result.toString();
                if (jsonStr.isEmpty())
                {
                    if (mainWindow)
                    {
                        mainWindow->showFloatingMessage(tr("未找到题目数据，可能页面结构已变化"), 3000, true);
                    }
                    view->setHtml(
                        "<html><body style='text-align:center;padding:50px;font-family:sans-serif;'>"
                        "<h2 style='color:red;'>未找到数据</h2>"
                        "<p>题目可能不存在或页面结构已更新。</p>"
                        "</body></html>"
                    );
                    page->deleteLater();
                    if (m_currentPage == page)
                    {
                        m_currentPage = nullptr;
                    }
                    m_isLoading = false;
                    return;
                }

                QJsonParseError parseError;
                QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
                if (parseError.error != QJsonParseError::NoError)
                {
                    if (mainWindow)
                    {
                        mainWindow->showFloatingMessage(tr("JSON解析失败: %1").arg(parseError.errorString()), 3000, true);
                    }
                    view->setHtml(
                        "<html><body style='text-align:center;padding:50px;font-family:sans-serif;'>"
                        "<h2 style='color:red;'>数据解析错误</h2>"
                        "<p>返回的数据格式异常。</p>"
                        "</body></html>"
                    );
                    page->deleteLater();
                    if (m_currentPage == page)
                    {
                        m_currentPage = nullptr;
                    }
                    m_isLoading = false;
                    return;
                }

                QJsonObject root = doc.object();
                QJsonObject data = root.value("data").toObject();
                QJsonObject problem = data.value("problem").toObject();

                QJsonObject content;
                if (problem.contains("contenu") && problem.value("contenu").isObject())
                {
                    content = problem.value("contenu").toObject();
                }
                else if (problem.contains("content") && problem.value("content").isObject())
                {
                    content = problem.value("content").toObject();
                }

                QString pid = problem.value("pid").toString();
                QString title = problem.value("title").toString();
                int difficulty = problem.value("difficulty").toInt();
                QString difficultyStr = difficultyToString(difficulty);

                QJsonArray timeArray = problem.value("limits").toObject().value("time").toArray();
                QJsonArray memoryArray = problem.value("limits").toObject().value("memory").toArray();

                QString timeStr;
                QString memoryStr;
                if (timeArray.isEmpty() || timeArray[0].toInt() == 0)
                {
                    timeStr = tr("无");
                }
                else
                {
                    timeStr = timeLimitToString(timeArray[0].toInt());
                }
                if (memoryArray.isEmpty() || memoryArray[0].toInt() == 0)
                {
                    memoryStr = tr("无");
                }
                else
                {
                    memoryStr = memoryLimitToString(memoryArray[0].toInt());
                }

                QString background = content.value("background").toString();
                QString description = content.value("description").toString();
                QString inputFormat = content.value("formatI").toString();
                QString outputFormat = content.value("formatO").toString();
                QString hint = content.value("hint").toString();

                QJsonArray samples = problem.value("samples").toArray();

                currentMarkdown = buildFullMarkdown(pid, title, background, description,
                                                    inputFormat, outputFormat, samples, hint);

                QString fullTitle = pid + " " + title;
                QString html = buildProblemHtml(fullTitle, difficultyStr, timeStr, memoryStr,
                                                background, description, inputFormat, outputFormat,
                                                samples, hint);

                view->setHtml(html, QUrl("https://www.luogu.com.cn/"));
                copyButton->setEnabled(true);

                page->deleteLater();
                if (m_currentPage == page)
                {
                    m_currentPage = nullptr;
                }
                m_isLoading = false;
            });
        });
    });

    page->load(url);
}

CheckerWidget::CheckerWidget(CompilerIDE *ide, QWidget *parent)
    : QWidget(parent), mainWindow(ide),
      compileProcess(nullptr), dataProcess(nullptr), stdProcess(nullptr), solProcess(nullptr),
      timer(nullptr), state(Idle), stopRequested(false)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget *contentWidget = new QWidget;
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(12);
    contentLayout->setContentsMargins(12, 12, 12, 12);

    QGroupBox *fileGroup = new QGroupBox(tr("选择文件"));
    QGridLayout *fileLayout = new QGridLayout(fileGroup);

    fileLayout->addWidget(new QLabel(tr("标准/暴力代码:")), 0, 0);
    stdPathEdit = new QLineEdit;
    stdPathEdit->setReadOnly(true);
    QPushButton *browseStdBtn = new QPushButton(tr("浏览..."));
    browseStdBtn->setFixedWidth(80);
    stdStatusIcon = new QLabel;
    stdStatusIcon->setFixedSize(20, 20);
    fileLayout->addWidget(stdPathEdit, 0, 1);
    fileLayout->addWidget(browseStdBtn, 0, 2);
    fileLayout->addWidget(stdStatusIcon, 0, 3);

    fileLayout->addWidget(new QLabel(tr("对拍代码:")), 1, 0);
    solPathEdit = new QLineEdit;
    solPathEdit->setReadOnly(true);
    QPushButton *browseSolBtn = new QPushButton(tr("浏览..."));
    browseSolBtn->setFixedWidth(80);
    solStatusIcon = new QLabel;
    solStatusIcon->setFixedSize(20, 20);
    fileLayout->addWidget(solPathEdit, 1, 1);
    fileLayout->addWidget(browseSolBtn, 1, 2);
    fileLayout->addWidget(solStatusIcon, 1, 3);

    fileLayout->addWidget(new QLabel(tr("数据生成器:")), 2, 0);
    dataPathEdit = new QLineEdit;
    dataPathEdit->setReadOnly(true);
    QPushButton *browseDataBtn = new QPushButton(tr("浏览..."));
    browseDataBtn->setFixedWidth(80);
    dataStatusIcon = new QLabel;
    dataStatusIcon->setFixedSize(20, 20);
    fileLayout->addWidget(dataPathEdit, 2, 1);
    fileLayout->addWidget(browseDataBtn, 2, 2);
    fileLayout->addWidget(dataStatusIcon, 2, 3);

    contentLayout->addWidget(fileGroup);

    QHBoxLayout *paramLayout = new QHBoxLayout;
    paramLayout->addWidget(new QLabel(tr("对拍组数:")));
    groupCountSpin = new QSpinBox;
    groupCountSpin->setRange(1, 10000);
    groupCountSpin->setValue(100);
    paramLayout->addWidget(groupCountSpin);

    paramLayout->addSpacing(20);
    paramLayout->addWidget(new QLabel(tr("时间限制(ms):")));
    timeLimitSpin = new QSpinBox;
    timeLimitSpin->setRange(100, 10000);
    timeLimitSpin->setValue(1000);
    paramLayout->addWidget(timeLimitSpin);
    paramLayout->addStretch();

    contentLayout->addLayout(paramLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    startBtn = new QPushButton(tr("开始对拍"));
    stopBtn = new QPushButton(tr("结束对拍"));
    resetBtn = new QPushButton(tr("重置"));
    stopBtn->setEnabled(false);

    btnLayout->addWidget(startBtn);
    btnLayout->addWidget(stopBtn);
    btnLayout->addWidget(resetBtn);
    btnLayout->addStretch();

    contentLayout->addLayout(btnLayout);

    progressWidget = new QWidget;
    QVBoxLayout *progressLayout = new QVBoxLayout(progressWidget);

    completionWidget = new QWidget;
    QHBoxLayout *completionLayout = new QHBoxLayout(completionWidget);
    completionLayout->setContentsMargins(20, 20, 20, 20);

    QPixmap checkPixmap(32, 32);
    checkPixmap.fill(Qt::transparent);
    QPainter p(&checkPixmap);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(mainWindow->isDarkTheme() ? QColor(0,255,255) : QColor(0,150,0), 3));
    p.drawLine(4, 16, 14, 26);
    p.drawLine(14, 26, 28, 6);
    p.end();

    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(checkPixmap);

    QLabel *textLabel = new QLabel(tr("对拍完成，未发现差异"));
    QFont font = textLabel->font();
    font.setPointSize(14);
    font.setBold(true);
    textLabel->setFont(font);

    completionLayout->addStretch();
    completionLayout->addWidget(iconLabel);
    completionLayout->addWidget(textLabel);
    completionLayout->addStretch();

    completionWidget->setVisible(false);
    contentLayout->addWidget(completionWidget);

    completionTimer = new QTimer(this);
    completionTimer->setSingleShot(true);
    connect(completionTimer, &QTimer::timeout, this, [this]()
    {
        progressWidget->setVisible(false);
        completionWidget->setVisible(true);
        statusLabel->setText(tr("对拍完成"));
    });

    progressLayout->setSpacing(8);

    QLabel *progressTitle = new QLabel(tr("对拍进度"));
    QFont titleFont = progressTitle->font();
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    progressTitle->setFont(titleFont);
    progressTitle->setAlignment(Qt::AlignCenter);
    progressLayout->addWidget(progressTitle);

    progressStatusLabel = new QLabel(tr("就绪"));
    progressStatusLabel->setAlignment(Qt::AlignCenter);
    progressStatusLabel->setStyleSheet("QLabel { color: #3498db; font-size: 14px; }");
    progressLayout->addWidget(progressStatusLabel);

    progressBar = new QProgressBar;
    progressBar->setRange(0, groupCountSpin->value());
    progressBar->setValue(0);
    progressBar->setTextVisible(true);
    progressBar->setFormat(tr("%p% (%v/%m)"));
    progressLayout->addWidget(progressBar);

    progressTimeLabel = new QLabel(tr("已用时间: 0s"));
    progressTimeLabel->setAlignment(Qt::AlignCenter);
    progressTimeLabel->setStyleSheet("QLabel { color: #666; }");
    progressLayout->addWidget(progressTimeLabel);

    contentLayout->addWidget(progressWidget);

    detailGroup = new QGroupBox(tr("WA详情"));
    QVBoxLayout *detailLayout = new QVBoxLayout(detailGroup);

    detailLayout->addWidget(new QLabel(tr("输入数据:")));
    inputDisplay = new QPlainTextEdit;
    inputDisplay->setReadOnly(true);
    inputDisplay->setFont(QFont("Consolas", 10));
    inputDisplay->setMaximumHeight(100);
    detailLayout->addWidget(inputDisplay);

    detailLayout->addWidget(new QLabel(tr("期望输出 (std):")));
    expectedDisplay = new QPlainTextEdit;
    expectedDisplay->setReadOnly(true);
    expectedDisplay->setFont(QFont("Consolas", 10));
    expectedDisplay->setMaximumHeight(100);
    detailLayout->addWidget(expectedDisplay);

    detailLayout->addWidget(new QLabel(tr("实际输出 (sol):")));
    actualDisplay = new QPlainTextEdit;
    actualDisplay->setReadOnly(true);
    actualDisplay->setFont(QFont("Consolas", 10));
    actualDisplay->setMaximumHeight(100);
    detailLayout->addWidget(actualDisplay);

    contentLayout->addWidget(detailGroup);

    detailGroup->setVisible(false);
    progressWidget->setVisible(true);

    statusLabel = new QLabel(tr("就绪"));
    statusLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(statusLabel);

    QLabel *importantLabel = new QLabel(tr("注意：所有程序的输入输出均要从标准输入输出（stdin/stdout）进行，请不要使用文件读写。"));
    importantLabel->setWordWrap(true);
    importantLabel->setAlignment(Qt::AlignCenter);

    if (mainWindow && mainWindow->isDarkTheme())
    {
        importantLabel->setStyleSheet(
            "QLabel {"
            "   background-color: #5A3E2B;"
            "   color: #FFD700;"
            "   border: 1px solid #B8860B;"
            "   border-radius: 5px;"
            "   padding: 8px;"
            "   margin-top: 10px;"
            "   font-size: 11px;"
            "}"
        );
    }
    else
    {
        importantLabel->setStyleSheet(
            "QLabel {"
            "   background-color: #FFF3CD;"
            "   color: #856404;"
            "   border: 1px solid #FFEEBA;"
            "   border-radius: 5px;"
            "   padding: 8px;"
            "   margin-top: 10px;"
            "   font-size: 11px;"
            "}"
        );
    }
    contentLayout->addWidget(importantLabel);

    setIconPending(stdStatusIcon);
    setIconPending(solStatusIcon);
    setIconPending(dataStatusIcon);

    connect(browseStdBtn, &QPushButton::clicked, this, &CheckerWidget::onBrowseStd);
    connect(browseSolBtn, &QPushButton::clicked, this, &CheckerWidget::onBrowseSol);
    connect(browseDataBtn, &QPushButton::clicked, this, &CheckerWidget::onBrowseData);
    connect(startBtn, &QPushButton::clicked, this, &CheckerWidget::onStartClicked);
    connect(stopBtn, &QPushButton::clicked, this, &CheckerWidget::onStopClicked);
    connect(resetBtn, &QPushButton::clicked, this, &CheckerWidget::onResetClicked);

    scrollArea->setWidget(contentWidget);
    rootLayout->addWidget(scrollArea);

    stageTimer = new QElapsedTimer;
}

CheckerWidget::~CheckerWidget()
{
    cleanUpTempFiles();
    if (compileProcess)
    {
        compileProcess->deleteLater();
    }
    if (dataProcess)
    {
        dataProcess->deleteLater();
    }
    if (stdProcess)
    {
        stdProcess->deleteLater();
    }
    if (solProcess)
    {
        solProcess->deleteLater();
    }
    if (timer)
    {
        timer->deleteLater();
    }
    if (stageTimer)
    {
        delete stageTimer;
    }
}

void CheckerWidget::setIconPending(QLabel *icon)
{
    icon->setPixmap(QPixmap());
    icon->setText("⚪");
    icon->setStyleSheet("QLabel { color: gray; font-size: 16px; }");
    icon->setToolTip(tr("等待编译"));
}

void CheckerWidget::updateIcon(QLabel *icon, bool success, const QString &tooltip)
{
    icon->setText("");
    if (success)
    {
        icon->setPixmap(style()->standardPixmap(QStyle::SP_DialogApplyButton));
        icon->setToolTip(tooltip.isEmpty() ? tr("编译成功") : tooltip);
    }
    else
    {
        icon->setPixmap(style()->standardPixmap(QStyle::SP_DialogCancelButton));
        icon->setToolTip(tooltip.isEmpty() ? tr("编译失败") : tooltip);
    }
}

void CheckerWidget::onBrowseStd()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择标准/暴力代码"),
                       QDir::homePath(), tr("C++文件 (*.cpp *.cc *.cxx)"));
    if (!fileName.isEmpty())
    {
        stdPathEdit->setText(fileName);
        setIconPending(stdStatusIcon);
    }
}

void CheckerWidget::onBrowseSol()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择优化代码"),
                       QDir::homePath(), tr("C++文件 (*.cpp *.cc *.cxx)"));
    if (!fileName.isEmpty())
    {
        solPathEdit->setText(fileName);
        setIconPending(solStatusIcon);
    }
}

void CheckerWidget::onBrowseData()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择数据生成器"),
                       QDir::homePath(), tr("C++文件 (*.cpp *.cc *.cxx)"));
    if (!fileName.isEmpty())
    {
        dataPathEdit->setText(fileName);
        setIconPending(dataStatusIcon);
    }
}

void CheckerWidget::onStartClicked()
{
    QString stdPath = stdPathEdit->text().trimmed();
    QString solPath = solPathEdit->text().trimmed();
    QString dataPath = dataPathEdit->text().trimmed();

    if (stdPath.isEmpty() || solPath.isEmpty() || dataPath.isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("请先选择三个文件"));
        return;
    }
    if (stdPath == solPath || stdPath == dataPath || solPath == dataPath)
    {
        QMessageBox::warning(this, tr("错误"), tr("三个文件不能有重复"));
        return;
    }
    if (!QFile::exists(stdPath) || !QFile::exists(solPath) || !QFile::exists(dataPath))
    {
        QMessageBox::warning(this, tr("错误"), tr("所选文件不存在"));
        return;
    }

    stopRequested = false;
    totalGroups = groupCountSpin->value();
    timeLimitMs = timeLimitSpin->value();
    currentGroup = 0;
    compileErrors.clear();
    mainWindow->updateErrorTable(QStringList(), QStringList());

    inputDisplay->clear();
    expectedDisplay->clear();
    actualDisplay->clear();
    statusLabel->setText(tr("正在编译..."));

    detailGroup->setVisible(false);
    progressWidget->setVisible(true);
    completionWidget->setVisible(false);

    progressBar->setRange(0, totalGroups);
    progressBar->setValue(0);
    progressStatusLabel->setText(tr("正在编译..."));
    progressTimeLabel->setText(tr("已用时间: 0s"));

    stageTimer->restart();

    setIconPending(stdStatusIcon);
    setIconPending(solStatusIcon);
    setIconPending(dataStatusIcon);

    tempDir = QDir::temp().absolutePath() + "/CompilerIDE_Comparer_" +
              QString::number(QDateTime::currentMSecsSinceEpoch());
    QDir().mkpath(tempDir);

    pendingCompileFiles.clear();
    pendingCompileFiles << stdPath << solPath << dataPath;
    currentCompileTarget = "std";
    currentStatusIcon = stdStatusIcon;

    setState(Compiling);
    startBtn->setEnabled(false);
    stopBtn->setEnabled(true);
    resetBtn->setEnabled(false);

    startNextCompile();
}

void CheckerWidget::prewarmPrograms()
{
    if (!dataExePath.isEmpty() && QFile::exists(dataExePath))
    {
        QProcess dataProc;
        dataProc.start(dataExePath);
        if (dataProc.waitForStarted(2000))
        {
            dataProc.write("0\n");
            dataProc.closeWriteChannel();
            dataProc.waitForFinished(500);
        }
    }

    if (!stdExePath.isEmpty() && QFile::exists(stdExePath))
    {
        QProcess stdProc;
        stdProc.start(stdExePath);
        if (stdProc.waitForStarted(2000))
        {
            stdProc.write("0\n");
            stdProc.closeWriteChannel();
            stdProc.waitForFinished(500);
        }
    }

    if (!solExePath.isEmpty() && QFile::exists(solExePath))
    {
        QProcess solProc;
        solProc.start(solExePath);
        if (solProc.waitForStarted(2000))
        {
            solProc.write("0\n");
            solProc.closeWriteChannel();
            solProc.waitForFinished(500);
        }
    }
}

void CheckerWidget::startNextCompile()
{
    if (stopRequested)
    {
        setState(Stopped);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);
        return;
    }

    if (pendingCompileFiles.isEmpty())
    {
        if (!compileErrors.isEmpty())
        {
            statusLabel->setText(tr("编译失败，请查看错误栏"));
            setState(Idle);
            startBtn->setEnabled(true);
            stopBtn->setEnabled(false);
            resetBtn->setEnabled(true);
            return;
        }

        prewarmPrograms();

        statusLabel->setText(tr("开始对拍..."));
        currentGroup = 0;
        runData();
        return;
    }

    QString sourceFile = pendingCompileFiles.takeFirst();
    QFileInfo fi(sourceFile);
    QString baseName = fi.completeBaseName();
    QString exePath = tempDir + "/" + baseName + ".exe";

    if (sourceFile == stdPathEdit->text())
    {
        currentCompileTarget = "std";
        currentStatusIcon = stdStatusIcon;
        stdExePath = exePath;
    }
    else if (sourceFile == solPathEdit->text())
    {
        currentCompileTarget = "sol";
        currentStatusIcon = solStatusIcon;
        solExePath = exePath;
    }
    else if (sourceFile == dataPathEdit->text())
    {
        currentCompileTarget = "data";
        currentStatusIcon = dataStatusIcon;
        dataExePath = exePath;
    }

    QStringList args = buildCompilerArgs(sourceFile, exePath);

    if (compileProcess)
    {
        compileProcess->deleteLater();
        compileProcess = nullptr;
    }
    compileProcess = new QProcess(this);
    compileProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(compileProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CheckerWidget::onCompileFinished);
    connect(compileProcess, &QProcess::errorOccurred, this, &CheckerWidget::onProcessError);

    compileProcess->start(mainWindow->getCompilerPath(), args);
    if (!compileProcess->waitForStarted(3000))
    {
        onProcessError(QProcess::FailedToStart);
    }
}

QStringList CheckerWidget::buildCompilerArgs(const QString &sourceFile, const QString &outputExe)
{
    QStringList args;
    QString compilerType = mainWindow->getCompilerType();
    QString cppStandard = mainWindow->getCppStandard();
    if (compilerType == "msvc")
    {
        if (cppStandard == "c++14")
        {
            args << "/std:c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "/std:c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "/std:c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "/std:c++latest";
        }
        args << "/EHsc" << "/nologo";
        args << "/Fe:" + outputExe;
        args << sourceFile;
    }
    else
    {
        if (cppStandard == "c++14")
        {
            args << "-std=c++14";
        }
        else if (cppStandard == "c++17")
        {
            args << "-std=c++17";
        }
        else if (cppStandard == "c++20")
        {
            args << "-std=c++20";
        }
        else if (cppStandard == "c++23")
        {
            args << "-std=c++2b";
        }
        args << "-O2" << "-static";
        args << "-lstdc++";
        args << "-o" << outputExe;
        args << sourceFile;
    }
    return args;
}

void CheckerWidget::onCompileFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (stopRequested)
    {
        if (compileProcess)
        {
            compileProcess->deleteLater();
            compileProcess = nullptr;
        }
        return;
    }

    QByteArray output = compileProcess->readAllStandardOutput();
    QString outputStr = QString::fromLocal8Bit(output);

    QStringList errors, warnings;

    QString fullPath;
    if (currentCompileTarget == "std")
    {
        fullPath = stdPathEdit->text();
    }
    else if (currentCompileTarget == "sol")
    {
        fullPath = solPathEdit->text();
    }
    else if (currentCompileTarget == "data")
    {
        fullPath = dataPathEdit->text();
    }

    parseCompilerOutput(outputStr, currentCompileTarget, fullPath, errors, warnings);

    if (!errors.isEmpty())
    {
        compileErrors.append(errors);
        mainWindow->updateErrorTable(compileErrors, warnings);
        updateIcon(currentStatusIcon, false, errors.join("\n"));

        stopRequested = true;
        if (compileProcess)
        {
            compileProcess->disconnect();
            compileProcess->kill();
            compileProcess->deleteLater();
            compileProcess = nullptr;
        }

        statusLabel->setText(tr("编译失败，请查看错误栏"));
        progressStatusLabel->setText(tr("编译失败"));
        setState(Idle);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);

        completionTimer->stop();
        progressWidget->setVisible(true);
        completionWidget->setVisible(false);
        detailGroup->setVisible(false);

        return;
    }
    else
    {
        updateIcon(currentStatusIcon, true, tr("编译成功"));
    }

    startNextCompile();
}

void CheckerWidget::parseCompilerOutput(const QString &output, const QString &target,
                                        const QString &fullPath,
                                        QStringList &errors, QStringList &warnings)
{
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    QRegularExpression gccError(R"(^(.+?):(\d+):(\d+):\s*(error|错误):\s*(.+)$)");
    QRegularExpression gccWarning(R"(^(.+?):(\d+):(\d+):\s*(warning|警告):\s*(.+)$)");

    QString unit = target + " (" + fullPath + ")";

    for (const QString &line : lines)
    {
        QRegularExpressionMatch match;
        if ((match = gccError.match(line)).hasMatch())
        {
            QString lineNum = match.captured(2);
            QString colNum = match.captured(3);
            QString msg = match.captured(5);
            errors << QString("%1|%2|%3|%4").arg(unit, lineNum, colNum, msg);
        }
        else if ((match = gccWarning.match(line)).hasMatch())
        {
            QString lineNum = match.captured(2);
            QString colNum = match.captured(3);
            QString msg = match.captured(5);
            warnings << QString("%1|%2|%3|%4").arg(unit, lineNum, colNum, msg);
        }
        else if (line.contains("error:", Qt::CaseInsensitive) || line.contains("错误:", Qt::CaseInsensitive))
        {
            errors << QString("%1|0|0|%2").arg(unit, line);
        }
        else if (line.contains("warning:", Qt::CaseInsensitive) || line.contains("警告:", Qt::CaseInsensitive))
        {
            warnings << QString("%1|0|0|%2").arg(unit, line);
        }
    }
}

void CheckerWidget::onProcessError(QProcess::ProcessError error)
{
    QString errMsg;
    switch (error)
    {
    case QProcess::FailedToStart:
        errMsg = tr("编译器启动失败");
        break;
    case QProcess::Crashed:
        errMsg = tr("编译器崩溃");
        break;
    default:
        errMsg = tr("编译过程出错");
    }
    compileErrors << QString("%1|0|0|%2").arg(currentCompileTarget, errMsg);
    updateIcon(currentStatusIcon, false, errMsg);
    mainWindow->updateErrorTable(compileErrors, QStringList());
    startNextCompile();
}

void CheckerWidget::runData()
{
    if (stopRequested)
    {
        setState(Stopped);
        return;
    }
    setState(RunningData);
    statusLabel->setText(tr("正在生成数据... (第 %1 组)").arg(currentGroup + 1));
    progressStatusLabel->setText(tr("正在生成数据... (第 %1/%2 组)").arg(currentGroup + 1).arg(totalGroups));

    if (currentGroup == 0)
    {
        stageTimer->restart();
    }

    if (dataProcess)
    {
        dataProcess->deleteLater();
    }
    dataProcess = new QProcess(this);
    dataProcess->setProcessChannelMode(QProcess::SeparateChannels);
    connect(dataProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CheckerWidget::onDataFinished);
    connect(dataProcess, &QProcess::errorOccurred, this, &CheckerWidget::onProcessError);

    dataProcess->start(dataExePath);
    if (!dataProcess->waitForStarted(2000))
    {
        onProcessError(QProcess::FailedToStart);
        return;
    }
}

void CheckerWidget::onDataFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (stopRequested)
    {
        setState(Stopped);
        return;
    }

    if (exitStatus != QProcess::NormalExit || exitCode != 0)
    {
        QString errorMsg = QString::fromLocal8Bit(dataProcess->readAllStandardError());
        if (errorMsg.isEmpty())
        {
            errorMsg = tr("数据生成器运行失败");
        }
        else
        {
            errorMsg = tr("数据生成器运行失败:\n") + errorMsg;
        }

        currentInput = "";
        currentStdOutput = "";
        currentSolOutput = tr("[data 运行时错误] ") + errorMsg;

        showWAInfo();
        statusLabel->setText(tr("数据生成器运行失败"));
        progressStatusLabel->setText(tr("数据生成器运行失败"));
        setState(Finished);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);
        return;
    }

    currentInput = QString::fromLocal8Bit(dataProcess->readAllStandardOutput());
    runStd();
}

void CheckerWidget::runStd()
{
    if (stopRequested)
    {
        setState(Stopped);
        return;
    }
    setState(RunningStd);
    statusLabel->setText(tr("正在运行标准程序..."));
    progressStatusLabel->setText(tr("正在运行标准程序..."));

    if (stdProcess)
    {
        stdProcess->deleteLater();
    }
    stdProcess = new QProcess(this);
    stdProcess->setProcessChannelMode(QProcess::SeparateChannels);
    connect(stdProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CheckerWidget::onStdFinished);
    connect(stdProcess, &QProcess::errorOccurred, this, &CheckerWidget::onProcessError);

    stdProcess->start(stdExePath);
    if (!stdProcess->waitForStarted(2000))
    {
        onProcessError(QProcess::FailedToStart);
        return;
    }
    stdProcess->write(currentInput.toLocal8Bit());
    stdProcess->closeWriteChannel();

    currentTimeMs = 0;
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(timeLimitMs);
    connect(timer, &QTimer::timeout, this, &CheckerWidget::onTimeout);
    timer->start();
}

void CheckerWidget::onStdFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (stopRequested)
    {
        return;
    }

    if (timer)
    {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }

    if (exitStatus != QProcess::NormalExit || exitCode != 0)
    {
        QString errorMsg = QString::fromLocal8Bit(stdProcess->readAllStandardError());
        if (errorMsg.isEmpty())
        {
            errorMsg = tr("运行时错误 (RE)");
        }
        else
        {
            errorMsg = tr("运行时错误 (RE):\n") + errorMsg;
        }

        currentStdOutput = "";
        currentSolOutput = tr("[std 运行时错误] ") + errorMsg;

        showWAInfo();
        statusLabel->setText(tr("标准程序运行时错误"));
        progressStatusLabel->setText(tr("标准程序运行时错误"));
        setState(Finished);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);
        return;
    }

    currentStdOutput = QString::fromLocal8Bit(stdProcess->readAllStandardOutput());
    runSol();
}

void CheckerWidget::runSol()
{
    if (stopRequested)
    {
        setState(Stopped);
        return;
    }
    setState(RunningSol);
    statusLabel->setText(tr("正在运行对拍程序..."));
    progressStatusLabel->setText(tr("正在运行对拍程序..."));

    if (solProcess)
    {
        solProcess->deleteLater();
    }
    solProcess = new QProcess(this);
    solProcess->setProcessChannelMode(QProcess::SeparateChannels);
    connect(solProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &CheckerWidget::onSolFinished);
    connect(solProcess, &QProcess::errorOccurred, this, &CheckerWidget::onProcessError);

    solProcess->start(solExePath);
    if (!solProcess->waitForStarted(2000))
    {
        onProcessError(QProcess::FailedToStart);
        return;
    }
    solProcess->write(currentInput.toLocal8Bit());
    solProcess->closeWriteChannel();

    currentTimeMs = 0;
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(timeLimitMs);
    connect(timer, &QTimer::timeout, this, &CheckerWidget::onTimeout);
    timer->start();
}

void CheckerWidget::onSolFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (stopRequested)
    {
        return;
    }

    if (timer)
    {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }

    if (exitStatus != QProcess::NormalExit || exitCode != 0)
    {
        QString errorMsg = QString::fromLocal8Bit(solProcess->readAllStandardError());
        if (errorMsg.isEmpty())
        {
            errorMsg = tr("运行时错误 (RE)");
        }
        else
        {
            errorMsg = tr("运行时错误 (RE):\n") + errorMsg;
        }

        currentSolOutput = tr("[sol 运行时错误] ") + errorMsg;

        showWAInfo();
        statusLabel->setText(tr("对拍程序运行时错误"));
        progressStatusLabel->setText(tr("对拍程序运行时错误"));
        setState(Finished);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);
        return;
    }

    currentSolOutput = QString::fromLocal8Bit(solProcess->readAllStandardOutput());
    compareOutputs();
}

void CheckerWidget::compareOutputs()
{
    QString stdOut = currentStdOutput.trimmed();
    QString solOut = currentSolOutput.trimmed();

    if (stdOut != solOut)
    {
        showWAInfo();
        statusLabel->setText(tr("发现答案错误 (WA) - 第 %1 组").arg(currentGroup + 1));
        progressStatusLabel->setText(tr("发现答案错误 (WA)"));
        setState(Finished);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);
        return;
    }

    currentGroup++;

    progressBar->setValue(currentGroup);
    int elapsedSecs = stageTimer->elapsed() / 1000;
    progressTimeLabel->setText(tr("已用时间: %1s").arg(elapsedSecs));

    statusLabel->setText(tr("已完成 %1/%2 组").arg(currentGroup).arg(totalGroups));

    if (currentGroup >= totalGroups || stopRequested)
    {
        if (!stopRequested && currentGroup >= totalGroups)
        {
            completionTimer->start(500);
        }
        else
        {
            completionTimer->stop();
            progressWidget->setVisible(true);
            completionWidget->setVisible(false);
        }
        statusLabel->setText(tr("对拍完成，%1 组全部通过").arg(totalGroups));
        progressStatusLabel->setText(tr("对拍完成"));
        setState(Finished);
        startBtn->setEnabled(true);
        stopBtn->setEnabled(false);
        resetBtn->setEnabled(true);
    }
    else
    {
        runData();
    }
}

void CheckerWidget::showWAInfo()
{
    progressWidget->setVisible(false);
    detailGroup->setVisible(true);

    inputDisplay->setPlainText(currentInput);
    expectedDisplay->setPlainText(currentStdOutput);
    actualDisplay->setPlainText(currentSolOutput);
}

void CheckerWidget::onTimeout()
{
    if (stopRequested)
    {
        return;
    }

    if (state == RunningStd && stdProcess)
    {
        stdProcess->blockSignals(true);
        disconnect(stdProcess, nullptr, this, nullptr);
        stdProcess->kill();
        stdProcess->waitForFinished(1000);

        currentStdOutput = tr("[std 超时] 程序运行超时 (TLE)，时间限制: %1ms").arg(timeLimitMs);
        currentSolOutput = "";

        showWAInfo();
        statusLabel->setText(tr("标准程序超时"));
        progressStatusLabel->setText(tr("标准程序超时"));

        stdProcess->deleteLater();
        stdProcess = nullptr;

    }
    else if (state == RunningSol && solProcess)
    {
        solProcess->blockSignals(true);
        disconnect(solProcess, nullptr, this, nullptr);
        solProcess->kill();
        solProcess->waitForFinished(1000);

        currentSolOutput = tr("[sol 超时] 程序运行超时 (TLE)，时间限制: %1ms").arg(timeLimitMs);

        showWAInfo();
        statusLabel->setText(tr("对拍程序超时"));
        progressStatusLabel->setText(tr("对拍程序超时"));

        solProcess->deleteLater();
        solProcess = nullptr;
    }

    setState(Finished);
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    resetBtn->setEnabled(true);

    if (timer)
    {
        timer->stop();
        timer->deleteLater();
        timer = nullptr;
    }
}

void CheckerWidget::onStopClicked()
{
    stopRequested = true;

    if (dataProcess && dataProcess->state() == QProcess::Running)
    {
        dataProcess->kill();
    }
    if (stdProcess && stdProcess->state() == QProcess::Running)
    {
        stdProcess->kill();
    }
    if (solProcess && solProcess->state() == QProcess::Running)
    {
        solProcess->kill();
    }
    if (compileProcess && compileProcess->state() == QProcess::Running)
    {
        compileProcess->kill();
    }
    if (timer && timer->isActive())
    {
        timer->stop();
    }

    if (dataProcess)
    {
        dataProcess->disconnect();
        dataProcess->deleteLater();
        dataProcess = nullptr;
    }
    if (stdProcess)
    {
        stdProcess->disconnect();
        stdProcess->deleteLater();
        stdProcess = nullptr;
    }
    if (solProcess)
    {
        solProcess->disconnect();
        solProcess->deleteLater();
        solProcess = nullptr;
    }
    if (compileProcess)
    {
        compileProcess->disconnect();
        compileProcess->deleteLater();
        compileProcess = nullptr;
    }
    if (timer)
    {
        timer->deleteLater();
        timer = nullptr;
    }

    setState(Stopped);
    statusLabel->setText(tr("已停止"));
    progressStatusLabel->setText(tr("已停止"));
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    resetBtn->setEnabled(true);
    completionWidget->setVisible(false);
}

void CheckerWidget::onResetClicked()
{
    onStopClicked();
    stdPathEdit->clear();
    solPathEdit->clear();
    dataPathEdit->clear();
    setIconPending(stdStatusIcon);
    setIconPending(solStatusIcon);
    setIconPending(dataStatusIcon);
    inputDisplay->clear();
    expectedDisplay->clear();
    actualDisplay->clear();
    mainWindow->updateErrorTable(QStringList(), QStringList());
    statusLabel->setText(tr("就绪"));
    startBtn->setEnabled(true);
    stopBtn->setEnabled(false);
    resetBtn->setEnabled(true);

    progressWidget->setVisible(true);
    completionWidget->setVisible(false);
    detailGroup->setVisible(false);
    progressBar->setRange(0, groupCountSpin->value());
    progressBar->setValue(0);
    progressStatusLabel->setText(tr("就绪"));
    progressTimeLabel->setText(tr("已用时间: 0s"));

    cleanUpTempFiles();
    setState(Idle);
}

void CheckerWidget::cleanUpTempFiles()
{
    if (!tempDir.isEmpty() && QDir(tempDir).exists())
    {
        QDir(tempDir).removeRecursively();
        tempDir.clear();
    }
}

void CheckerWidget::setState(State newState)
{
    state = newState;
}

void CheckerWidget::updateProgressDisplay()
{
    // 删了会报错？忘了。也有可能是我懒得删了
}

int main(int argc, char *argv[])
{
    // 这里建议实际运行时删除，我在配置路径时把电脑弄炸了，只有加这个才能让程序正常运行
    qunsetenv("QT_QPA_PLATFORM_PLUGIN_PATH");
    qunsetenv("QT_PLUGIN_PATH");
    QString mingwPath = "D:/Qt/6.11.0/msvc2022_64/plugins";
    QDir mingwDir(mingwPath);
    if (mingwDir.exists() && mingwDir.exists("platforms/qwindows.dll"))
    {
        qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", mingwPath.toUtf8());
        qDebug() << "Development: Using MinGW plugins";
    }
    qputenv("QT_DEBUG_PLUGINS", "1");
    QApplication app(argc, argv);
    qDebug() << "Library paths:" << app.libraryPaths();
    qDebug() << "Plugin path env:" << qgetenv("QT_QPA_PLATFORM_PLUGIN_PATH");
    // 上方内容建议删除

    QApplication::setOrganizationName("CompilerIDE");
    QApplication::setApplicationName("Compiler IDE " + IDE_VERSION);
    QApplication::setApplicationVersion(IDE_VERSION);
    app.setWindowIcon(QIcon(":/icons/CompilerIDE_logo.png"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

    QSettings settings("CompilerIDE", "Compiler IDE 2.8.6");
    bool enableSplash = settings.value("enableSplash", false).toBool();
    bool enableWelcome = settings.value("enableWelcome", true).toBool();

    CompilerIDE *ide = new CompilerIDE();

    if (enableSplash)
    {
        SplashScreen *splash = new SplashScreen();
        splash->showWithAnimation();
        QObject::connect(splash, &SplashScreen::hidden, [splash, ide, argc, argv, enableWelcome]()
        {
            splash->deleteLater();
            QApplication::restoreOverrideCursor();
            QApplication::setOverrideCursor(Qt::ArrowCursor);
            QApplication::restoreOverrideCursor();

            if (enableWelcome)
            {
                WelcomeDialog welcomeDialog;
                if (welcomeDialog.exec() == QDialog::Accepted)
                {
                    QString selectedFile = welcomeDialog.getSelectedFile();
                    ide->showMaximized();
                    ide->show();
                    ide->raise();
                    ide->activateWindow();
                    ide->setCursor(Qt::ArrowCursor);
                    if (selectedFile == "__NEW_FILE__")
                    {
                        ide->newFile();
                    }
                    else if (!selectedFile.isEmpty())
                    {
                        ide->loadFile(selectedFile);
                    }
                    else if (argc > 1)
                    {
                        ide->loadFile(argv[1]);
                    }
                    QTimer::singleShot(500, ide, [ide]()
                    {
                        qDebug() << "开始自动检查更新...";
                        ide->autoCheckForUpdates();
                    });
                }
                else
                {
                    QApplication::quit();
                }
            }
            else
            {
                ide->showMaximized();
                ide->show();
                ide->raise();
                ide->activateWindow();
                if (argc > 1)
                {
                    ide->loadFile(argv[1]);
                }
                QTimer::singleShot(500, ide, [ide]()
                {
                    qDebug() << "开始自动检查更新...";
                    ide->autoCheckForUpdates();
                });
            }
        });
    }
    else
    {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        QApplication::restoreOverrideCursor();

        if (enableWelcome)
        {
            WelcomeDialog welcomeDialog;
            if (welcomeDialog.exec() == QDialog::Accepted)
            {
                QString selectedFile = welcomeDialog.getSelectedFile();
                ide->showMaximized();
                ide->show();
                ide->raise();
                ide->activateWindow();
                ide->setCursor(Qt::ArrowCursor);
                if (selectedFile == "__NEW_FILE__")
                {
                    ide->newFile();
                }
                else if (!selectedFile.isEmpty())
                {
                    ide->loadFile(selectedFile);
                }
                else if (argc > 1)
                {
                    ide->loadFile(argv[1]);
                }
                QTimer::singleShot(500, ide, [ide]()
                {
                    qDebug() << "开始自动检查更新...";
                    ide->autoCheckForUpdates();
                });
            }
            else
            {
                delete ide;
                return 0;
            }
        }
        else
        {
            ide->showMaximized();
            ide->show();
            ide->raise();
            ide->activateWindow();
            if (argc > 1)
            {
                ide->loadFile(argv[1]);
            }
            QTimer::singleShot(500, ide, [ide]()
            {
                qDebug() << "开始自动检查更新...";
                ide->autoCheckForUpdates();
            });
        }
    }

    int result = app.exec();
    delete ide;
    return result;
}

#include "main.moc"

// 到此结束！如果你喜欢，一定要记得点个 Star 哦~