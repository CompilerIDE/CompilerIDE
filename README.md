<h1 align="center">Compiler IDE</h1>
<p align="center">
  <a href="https://github.com/CompilerIDE/CompilerIDE/blob/main/LICENSE"><img src="https://img.shields.io/github/license/CompilerIDE/CompilerIDE.svg?style=for-the-badge&new=1" alt="License"></a>
  <br>
</p>
<p align="center">
  <img src="https://img.shields.io/badge/C++-17+-47848F?style=flat&logo=c%2B%2B&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/MSVC-19.44+-blue?style=flat" alt="MSVC 19.44+">
  <img src="https://img.shields.io/badge/Qt-6.11.0+-blue?style=flat&logo=qt&logoColor=gree" alt="Qt 6.11.0+">
</p>

## 简介
自 $2024$ 年 $5$ 月起，历经 20 多个月的开发，我们终于能将 Compiler IDE，呈现给大家！

这不止是一个 IDE 的诞生，更是属于每一位 OIer 的新故事，即将从这里开始书写。未来已来，共同启程！

本 IDE 由 CompilerIDE Team 团队共同开发，支持的编程语言只有 C++。目前最新版本为 3.6.0。操作系统应为 Windows + 64 位（不支持 Win7 及更低版本）。

## IDE 下载

你可以在 [CompilerIDE 官网](https://compileride.cn)下载最新版本

## 开源协议

本 IDE 基于 [`GPL-3.0`](LICENSE) 开源

## 鸣谢
1. 开发者名单：[Eric](https://www.luogu.com.cn/user/995571)、[Hrb](https://www.luogu.com.cn/user/1286421)、[FlyHappily](https://www.luogu.com.cn/user/1364658)、[Lxy](https://www.luogu.com.cn/user/1616973)
2. 感谢 [zhangchenyi_awa](https://www.luogu.com.cn/user/1271781)、[chenyajun1324](https://www.luogu.com.cn/user/1809877)、[_firestar_](https://www.luogu.com.cn/user/1972540)、[AngryFlower](https://www.luogu.com.cn/user/1631802) 等人对 CompilerIDE 的 Bug 修复做出的卓越贡献

## 本地部署步骤

<h4 align="center">系统要求：Windows 10 / 11 (x64)</h4>

1. 下载 Qt：https://www.qt.io/development/download-qt-installer-oss
2. 在 cmd 中依次输入：```cd \d C:\Users\用户名\Downloads```与```"下载的文件名（例如：qt-online-installer-windows-x64-4.11.0.exe）" --mirror https://mirror.nju.edu.cn/qt```
3. 勾选对应组件并安装：<img width="873" height="811" alt="image" src="https://github.com/user-attachments/assets/1704a642-7ce5-42a4-ad7c-099f827a98da" />
                <img width="851" height="797" alt="image" src="https://github.com/user-attachments/assets/245c105c-f2fa-4066-8db2-ee72461f052a" />
                <img width="878" height="797" alt="image" src="https://github.com/user-attachments/assets/6836d8eb-b61a-4533-9cc0-d5a0929bdc9a" />
                <img width="858" height="805" alt="image" src="https://github.com/user-attachments/assets/6983c091-5abc-490b-92f8-a43d6bc372cb" />
                <img width="888" height="813" alt="image" src="https://github.com/user-attachments/assets/caada093-8526-4d0e-9813-5388dc9f637a" />
                <img width="800" height="330" alt="image" src="https://github.com/user-attachments/assets/2a28e5dd-2091-4ac5-9d29-7c74a70e7c52" />
4. 安装 Visual Studio 2022 生成工具、Visual Studio 2026 编译器与 SDK。
5. 打开 Qt Creater，点击 文件 -> New Project
6. 项目选择 Application(Qt) -> Qt Widgets Application，点击选择
7. 名称输入 CompilerIDE，项目地址自行填写，随后点击下一步
8. Build System 点击 CMake，点击下一步
9. Details 页面保持原样，点击下一步
10. Translation 页面保持原样，点击下一步
11. 构建套件选择 Desktop Qt 6.11.1 MSVC2022 64bit，选择子菜单中的 Debug 与 Release，点击下一步
12. 点击完成。
13. 在 PowerShell/cmd 中运行 ```git clone https://github.com/CompilerIDE/CompilerIDE```
14. 打开 CompilerIDE Git clone 完成后的文件夹
15. 将 Git clone 完成后的文件夹的所有文件代码粘贴至 Qt 项目中对应的文件
