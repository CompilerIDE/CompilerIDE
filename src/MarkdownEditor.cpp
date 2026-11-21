/*
 * Copyright (C) 2026 Compiler IDE Team (CompilerIDE_Official@outlook.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "MarkdownEditor.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollBar>
#include <QFont>
#include <QColor>
#include <QVariant>

MarkdownPreviewBridge::MarkdownPreviewBridge(QObject *parent)
    : QObject(parent)
{
}

QString MarkdownPreviewBridge::content() const
{
    return m_content;
}

void MarkdownPreviewBridge::setContent(const QString &content)
{
    if (m_content != content)
    {
        m_content = content;
        emit contentChanged();
    }
}

MarkdownEditor::MarkdownEditor(QWidget *parent)
    : MarkdownEditor(parent, -1)
{
}

MarkdownEditor::MarkdownEditor(QWidget *parent, int themeMode)
    : QWidget(parent)
{
    if (themeMode < 0)
    {
        QWidget *rootWindow = parent ? parent->window() : nullptr;
        if (rootWindow)
        {
            QVariant mode = rootWindow->property("themeMode");
            if (mode.isValid())
            {
                m_themeMode = mode.toInt();
            }
        }
    }
    else
    {
        m_themeMode = themeMode;
    }

    if (m_themeMode < 0 || m_themeMode > 2)
    {
        m_themeMode = 2;
    }

    initUi();
}

void MarkdownEditor::initUi()
{
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    m_editor = new QPlainTextEdit(this);
    m_editor->setFont(QFont("Consolas", 11));
    m_editor->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_editor->setTabStopDistance(40);

    m_preview = new QWebEngineView(this);
    m_preview->setContextMenuPolicy(Qt::NoContextMenu);

    splitter->addWidget(m_editor);
    splitter->addWidget(m_preview);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(splitter);

    m_bridge = new MarkdownPreviewBridge(this);
    m_channel = new QWebChannel(this);
    m_channel->registerObject(QStringLiteral("bridge"), m_bridge);

    m_preview->page()->setWebChannel(m_channel);

    connect(m_editor, &QPlainTextEdit::textChanged, this, &MarkdownEditor::onTextChanged);
    connect(m_editor->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &MarkdownEditor::onEditorScrolled);

    loadPreviewHtml();
}

int MarkdownEditor::themeMode() const
{
    return m_themeMode;
}

QPlainTextEdit *MarkdownEditor::editor() const
{
    return m_editor;
}

QWebEngineView *MarkdownEditor::preview() const
{
    return m_preview;
}

QString MarkdownEditor::toPlainText() const
{
    return m_editor->toPlainText();
}

void MarkdownEditor::setPlainText(const QString &text)
{
    m_editor->setPlainText(text);
}

void MarkdownEditor::setDocumentModified(bool modified)
{
    m_editor->document()->setModified(modified);
}

bool MarkdownEditor::isModified() const
{
    return m_editor->document()->isModified();
}

void MarkdownEditor::onTextChanged()
{
    m_bridge->setContent(m_editor->toPlainText());
    emit textChanged();
}

void MarkdownEditor::onEditorScrolled(int value)
{
    QScrollBar *scrollBar = m_editor->verticalScrollBar();
    if (!scrollBar)
    {
        return;
    }

    int maximum = scrollBar->maximum();
    if (maximum <= 0)
    {
        return;
    }

    double ratio = static_cast<double>(value) / static_cast<double>(maximum);
    QString script = QStringLiteral(
        "if (window.__mdScrollToRatio) { window.__mdScrollToRatio(%1); }")
        .arg(ratio, 0, 'f', 6);
    m_preview->page()->runJavaScript(script);
}

void MarkdownEditor::collectThemeColors(QString &background,
                                        QString &foreground,
                                        QString &borderColor,
                                        QString &codeBackground,
                                        QString &codeForeground,
                                        QString &quoteColor,
                                        QString &scrollTrack,
                                        QString &scrollThumb,
                                        QString &scrollThumbHover) const
{
    if (m_themeMode == 2)
    {
        background = QStringLiteral("#1e1e1e");
        foreground = QStringLiteral("#d4d4d4");
        borderColor = QStringLiteral("#3c3c3c");
        codeBackground = QStringLiteral("#252526");
        codeForeground = QStringLiteral("#d4d4d4");
        quoteColor = QStringLiteral("#8b949e");
        scrollTrack = QStringLiteral("#1e1e1e");
        scrollThumb = QStringLiteral("#424242");
        scrollThumbHover = QStringLiteral("#4f4f4f");
    }
    else if (m_themeMode == 1)
    {
        background = QStringLiteral("#FDF6E3");
        foreground = QStringLiteral("#3B3A32");
        borderColor = QStringLiteral("#C7B98E");
        codeBackground = QStringLiteral("#F3E8C8");
        codeForeground = QStringLiteral("#3B3A32");
        quoteColor = QStringLiteral("#6B6658");
        scrollTrack = QStringLiteral("#F3E8C8");
        scrollThumb = QStringLiteral("#B9AA7D");
        scrollThumbHover = QStringLiteral("#9F9069");
    }
    else
    {
        background = QStringLiteral("#ffffff");
        foreground = QStringLiteral("#24292e");
        borderColor = QStringLiteral("#dfe2e5");
        codeBackground = QStringLiteral("#f6f8fa");
        codeForeground = QStringLiteral("#24292e");
        quoteColor = QStringLiteral("#6a737d");
        scrollTrack = QStringLiteral("#f1f1f1");
        scrollThumb = QStringLiteral("#c1c1c1");
        scrollThumbHover = QStringLiteral("#a8a8a8");
    }
}

void MarkdownEditor::setThemeMode(int mode)
{
    if (mode < 0 || mode > 2)
    {
        mode = 2;
    }

    if (m_themeMode == mode)
    {
        return;
    }

    m_themeMode = mode;

    QString background;
    QString foreground;
    QString borderColor;
    QString codeBackground;
    QString codeForeground;
    QString quoteColor;
    QString scrollTrack;
    QString scrollThumb;
    QString scrollThumbHover;
    collectThemeColors(background, foreground, borderColor, codeBackground,
                       codeForeground, quoteColor, scrollTrack, scrollThumb, scrollThumbHover);

    if (m_preview && m_preview->page())
    {
        m_preview->page()->setBackgroundColor(QColor(background));
    }

    const bool isDark = (m_themeMode == 2);

    QString script = QStringLiteral(R"JS(
(function() {
    var root = document.documentElement;
    if (!root) return;

    root.style.setProperty('--md-bg', '%1');
    root.style.setProperty('--md-fg', '%2');
    root.style.setProperty('--md-border', '%3');
    root.style.setProperty('--md-code-bg', '%4');
    root.style.setProperty('--md-code-fg', '%5');
    root.style.setProperty('--md-quote', '%6');
    root.style.setProperty('--md-scroll-track', '%7');
    root.style.setProperty('--md-scroll-thumb', '%8');
    root.style.setProperty('--md-scroll-thumb-hover', '%9');

    var light = document.getElementById('hljs-light');
    var dark = document.getElementById('hljs-dark');
    if (light) light.disabled = %10;
    if (dark) dark.disabled = %11;
})();
)JS")
        .arg(background)
        .arg(foreground)
        .arg(borderColor)
        .arg(codeBackground)
        .arg(codeForeground)
        .arg(quoteColor)
        .arg(scrollTrack)
        .arg(scrollThumb)
        .arg(scrollThumbHover)
        .arg(isDark ? QStringLiteral("true") : QStringLiteral("false"))
        .arg(isDark ? QStringLiteral("false") : QStringLiteral("true"));

    m_preview->page()->runJavaScript(script);
}

void MarkdownEditor::loadPreviewHtml()
{
    QString background;
    QString foreground;
    QString borderColor;
    QString codeBackground;
    QString codeForeground;
    QString quoteColor;
    QString scrollTrack;
    QString scrollThumb;
    QString scrollThumbHover;
    collectThemeColors(background, foreground, borderColor, codeBackground,
                       codeForeground, quoteColor, scrollTrack, scrollThumb, scrollThumbHover);

    if (m_preview && m_preview->page())
    {
        m_preview->page()->setBackgroundColor(QColor(background));
    }

    const bool isDark = (m_themeMode == 2);

    QString html = QStringLiteral(R"HTML(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="color-scheme" content="only light">
<style id="theme-vars">
:root {
    --md-bg: __BG__;
    --md-fg: __FG__;
    --md-border: __BORDER__;
    --md-code-bg: __CODEBG__;
    --md-code-fg: __CODEFG__;
    --md-quote: __QUOTE__;
    --md-scroll-track: __TRACK__;
    --md-scroll-thumb: __THUMB__;
    --md-scroll-thumb-hover: __THUMBHOVER__;
}
</style>
<link rel="stylesheet" href="qrc:/markdown/katex.min.css">
<link id="hljs-light" rel="stylesheet" href="qrc:/markdown/github.min.css">
<link id="hljs-dark" rel="stylesheet" href="qrc:/markdown/github-dark.min.css">
<script src="qrc:/markdown/marked.umd.js"></script>
<script src="qrc:/markdown/katex.min.js"></script>
<script src="qrc:/markdown/highlight.min.js"></script>
<script src="qrc:/markdown/purify.min.js"></script>
<script src="qrc:/qtwebchannel/qwebchannel.js"></script>
<style>
html, body {
    margin: 0;
    padding: 0;
    background: var(--md-bg) !important;
    color: var(--md-fg) !important;
}
body {
    padding: 16px;
    font-family: -apple-system, "Segoe UI", "Microsoft YaHei", sans-serif;
    font-size: 15px;
    line-height: 1.7;
}
pre {
    background: var(--md-code-bg) !important;
    color: var(--md-code-fg) !important;
    border-radius: 6px;
    padding: 12px;
    overflow-x: auto;
}
pre code,
pre code.hljs,
code.hljs {
    background: transparent !important;
    color: var(--md-code-fg) !important;
    padding: 0 !important;
    font-family: "Cascadia Code", Consolas, monospace;
    font-size: 0.92em;
}
code {
    font-family: "Cascadia Code", Consolas, monospace;
    font-size: 0.92em;
}
:not(pre) > code {
    background: var(--md-code-bg) !important;
    color: var(--md-code-fg) !important;
    padding: 2px 5px;
    border-radius: 4px;
}
blockquote {
    border-left: 4px solid var(--md-quote) !important;
    padding-left: 12px;
    color: var(--md-quote) !important;
    margin-left: 0;
}
table {
    border-collapse: collapse;
    width: 100%;
}
th, td {
    border: 1px solid var(--md-border) !important;
    padding: 6px 12px;
}
th {
    background: var(--md-code-bg) !important;
}
img {
    max-width: 100%;
}
.katex-display {
    overflow-x: auto;
    overflow-y: hidden;
}
.katex {
    color: inherit !important;
}

/* ===== 滚动条主题化 ===== */
::-webkit-scrollbar {
    width: 10px;
    height: 10px;
    background: var(--md-scroll-track) !important;
}
::-webkit-scrollbar-track {
    background: var(--md-scroll-track) !important;
}
::-webkit-scrollbar-thumb {
    background: var(--md-scroll-thumb) !important;
    border-radius: 5px;
    border: 2px solid var(--md-scroll-track) !important;
}
::-webkit-scrollbar-thumb:hover {
    background: var(--md-scroll-thumb-hover) !important;
}
::-webkit-scrollbar-corner {
    background: var(--md-scroll-track) !important;
}
</style>
</head>
<body>
<div id="preview"></div>
<script>
(function() {
    'use strict';

    var initialTheme = __THEME__;

    var light = document.getElementById('hljs-light');
    var dark = document.getElementById('hljs-dark');
    if (light) light.disabled = (initialTheme === 2);
    if (dark) dark.disabled = (initialTheme !== 2);

    var bridge = null;
    var previewEl = document.getElementById('preview');
    var lineMap = [];

    function protectMath(text) {
        var store = [];
        text = text.replace(/\$\$([\s\S]+?)\$\$/g, function(m, inner) {
            store.push({ display: true, content: inner });
            return '\u0002MDMATH' + (store.length - 1) + '\u0003';
        });
        text = text.replace(/(^|[^\$\\])\$([^\$\n]+?)\$(?!\$)/g, function(m, before, inner) {
            store.push({ display: false, content: inner });
            return before + '\u0002MDMATH' + (store.length - 1) + '\u0003';
        });
        return { text: text, store: store };
    }

    function restoreMath(html, store) {
        return html.replace(/\u0002MDMATH(\d+)\u0003/g, function(m, idx) {
            var item = store[parseInt(idx, 10)];
            if (!item) return m;
            try {
                if (typeof katex !== 'undefined' && katex.renderToString) {
                    return katex.renderToString(item.content, {
                        displayMode: item.display,
                        throwOnError: false,
                        output: 'html'
                    });
                }
            } catch (e) {
                console.warn('KaTeX render error:', e);
            }
            return m;
        });
    }

    function renderMarkdown(text) {
        if (!text) {
            previewEl.innerHTML = '';
            return;
        }

        var protectedResult = protectMath(text);

        var rawHtml = marked.parse(protectedResult.text, {
            gfm: true,
            breaks: false
        });

        var cleanHtml = DOMPurify.sanitize(rawHtml, {
            ADD_TAGS: ['math', 'semantics', 'annotation', 'mrow', 'mi', 'mo', 'mn', 'msup', 'msub', 'mfrac', 'msqrt', 'mtext', 'mspace', 'mover', 'munder', 'munderover', 'mtable', 'mtr', 'mtd', 'mstyle', 'mpadded', 'mphantom', 'menclose', 'svg', 'path', 'line', 'rect', 'g'],
            ADD_ATTR: ['xmlns', 'mathvariant', 'displaystyle', 'scriptlevel', 'stretchy', 'fence', 'separator', 'largeop', 'movablelimits', 'accent', 'accentunder', 'form', 'lspace', 'rspace', 'notation', 'columnalign', 'rowalign', 'columnlines', 'rowlines', 'frame', 'framespacing', 'width', 'height', 'depth', 'voffset', 'linethickness', 'numalign', 'denomalign', 'bevelled', 'stackalign', 'charalign', 'location', 'crossout', 'length', 'position', 'shift', 'left', 'right', 'close', 'open', 'separators', 'minlabelspacing', 'side', 'charspacing', 'equalrows', 'equalcolumns', 'align', 'rowspacing', 'columnspacing', 'groupalign', 'edge', 'fixedsize', 'aria-hidden', 'viewBox', 'd', 'fill', 'stroke', 'x1', 'y1', 'x2', 'y2', 'x', 'y', 'width', 'height'],
            FORBID_TAGS: ['style', 'script'],
            FORBID_ATTR: ['onerror', 'onload', 'onclick', 'onmouseover']
        });

        var finalHtml = restoreMath(cleanHtml, protectedResult.store);

        previewEl.innerHTML = finalHtml;

        previewEl.querySelectorAll('pre code').forEach(function(block) {
            if (typeof hljs !== 'undefined') {
                block.classList.add('hljs');
                hljs.highlightElement(block);
            }
        });

        buildLineMap(text);
    }

    function buildLineMap(text) {
        lineMap = [];
        var lines = text.split('\n');
        var elements = previewEl.children;
        var elementIndex = 0;

        for (var i = 0; i < lines.length; i++) {
            var trimmed = lines[i].trim();
            if (trimmed.length === 0) {
                continue;
            }

            var tag = '';
            if (trimmed.startsWith('#')) {
                tag = 'h' + (trimmed.match(/^#+/)[0].length);
            } else if (trimmed.startsWith('```')) {
                tag = 'pre';
            } else if (trimmed.startsWith('- ') || trimmed.startsWith('* ') || trimmed.startsWith('+ ')) {
                tag = 'li';
            } else if (/^\d+\./.test(trimmed)) {
                tag = 'li';
            } else if (trimmed.startsWith('>')) {
                tag = 'blockquote';
            } else {
                tag = 'p';
            }

            for (var j = elementIndex; j < elements.length; j++) {
                var el = elements[j];
                var elTag = el.tagName.toLowerCase();
                if (elTag === tag || (tag === 'li' && (elTag === 'ul' || elTag === 'ol'))) {
                    lineMap.push({ line: i, element: el });
                    elementIndex = j + 1;
                    break;
                }
            }
        }
    }

    window.__mdScrollToRatio = function(ratio) {
        var maxScroll = document.documentElement.scrollHeight - window.innerHeight;
        if (maxScroll <= 0) return;
        window.scrollTo(0, maxScroll * ratio);
    };

    window.__mdScrollToLine = function(line) {
        if (!lineMap || lineMap.length === 0) return;
        var target = null;
        for (var i = 0; i < lineMap.length; i++) {
            if (lineMap[i].line >= line) {
                target = lineMap[i].element;
                break;
            }
        }
        if (!target && lineMap.length > 0) {
            target = lineMap[lineMap.length - 1].element;
        }
        if (target) {
            target.scrollIntoView({ behavior: 'smooth', block: 'start' });
        }
    };

    document.addEventListener('DOMContentLoaded', function() {
        new QWebChannel(qt.webChannelTransport, function(channel) {
            bridge = channel.objects.bridge;
            if (bridge) {
                if (bridge.content) {
                    renderMarkdown(bridge.content);
                }
                bridge.contentChanged.connect(function() {
                    renderMarkdown(bridge.content);
                });
            }
        });
    });
})();
</script>
</body>
</html>)HTML");

    html.replace(QStringLiteral("__BG__"), background);
    html.replace(QStringLiteral("__FG__"), foreground);
    html.replace(QStringLiteral("__BORDER__"), borderColor);
    html.replace(QStringLiteral("__CODEBG__"), codeBackground);
    html.replace(QStringLiteral("__CODEFG__"), codeForeground);
    html.replace(QStringLiteral("__QUOTE__"), quoteColor);
    html.replace(QStringLiteral("__TRACK__"), scrollTrack);
    html.replace(QStringLiteral("__THUMB__"), scrollThumb);
    html.replace(QStringLiteral("__THUMBHOVER__"), scrollThumbHover);
    html.replace(QStringLiteral("__THEME__"), isDark ? QStringLiteral("2") : QStringLiteral("0"));

    m_preview->setHtml(html, QUrl(QStringLiteral("qrc:/")));
}