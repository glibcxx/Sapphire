## Sapphire

**Sapphire**是一个个人项目，用于为 Minecraft Bedrock Edition （以下简称 MCBE）提供客户端插件，插件主要面向红石玩家。

目前提供的功能有：

-   变速功能，支持音频变速：
    alt+keypad_add 加速，alt+keypad_sub 减速，alt+keypad_delete 恢复原速
-   修复 MovingBlock 拖影。
-   活塞更新顺序可视化。

你可以通过 alt+p 查看设置。

## Usage

关闭游戏，双击 UWPinjector.exe，游戏会被自动启动，且同目录下的 dll 会被注入进游戏中。

使用须知：

-   只支持 64 位 UWP 版本，且游戏必须以 DX12 模式运行，不支持以 DX11 模式运行的游戏。
-   只兼容 1.21.2、1.21.50、1.21.60 这 3 个版本，其它版本兼容性靠运气。

## Issue

什么？你竟然发现 bug 了？那很正常，垃圾代码出 bug 是天经地义的事，报个 issue 就行。

## Contribute

啊？你还想塞代码？那真是太感谢力！！

## Feature Request

催更是吧，催，都可以催

## Dependencies

-   [ImGui](https://github.com/ocornut/imgui) Gui 库，好用
-   [MinHook](https://github.com/TsudaKageyu/minhook) Hook 库，好用
-   [kiero](https://github.com/Rebzzel/kiero) 一个 DX hook 库，好用
-   [json](https://github.com/nlohmann/json) 天下第一 Json 库
-   [fmod](https://www.fmod.com/download) 游戏用了，所以我用了

## Credits

-   [LeviLamina](https://github.com/LiteLDev/LeviLamina) 拜托，你见过这么优雅的 hook 框架吗，我~~抄~~超喜欢。
-   [Latite](https://github.com/LatiteClient/Latite) 部分 signature 取自此项目

## License

src 文件夹下的所有代码按照 GPLv3 协议开源。
external/fmod 下的代码归 Firelight Technologies Pty Ltd 所有
其余代码保留原始协议
