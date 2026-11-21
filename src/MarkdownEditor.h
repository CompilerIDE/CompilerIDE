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

#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QWebEngineView>
#include <QWebChannel>

class MarkdownPreviewBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)

public:
    explicit MarkdownPreviewBridge(QObject *parent = nullptr);

    QString content() const;
    void setContent(const QString &content);

signals:
    void contentChanged();

private:
    QString m_content;
};

class MarkdownEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownEditor(QWidget *parent = nullptr);
    explicit MarkdownEditor(QWidget *parent, int themeMode);

    void setThemeMode(int mode);
    int themeMode() const;

    QPlainTextEdit *editor() const;
    QWebEngineView *preview() const;
    QString toPlainText() const;
    void setPlainText(const QString &text);
    void setDocumentModified(bool modified);
    bool isModified() const;

signals:
    void textChanged();

private slots:
    void onTextChanged();
    void onEditorScrolled(int value);

private:
    void initUi();
    void loadPreviewHtml();
    void collectThemeColors(QString &background,
                            QString &foreground,
                            QString &borderColor,
                            QString &codeBackground,
                            QString &codeForeground,
                            QString &quoteColor,
                            QString &scrollTrack,
                            QString &scrollThumb,
                            QString &scrollThumbHover) const;

    QPlainTextEdit *m_editor = nullptr;
    QWebEngineView *m_preview = nullptr;
    QWebChannel *m_channel = nullptr;
    MarkdownPreviewBridge *m_bridge = nullptr;
    int m_themeMode = 2;
};

#endif