## Sapphire

**Sapphire**是一个个人项目，用于为 Minecraft Bedrock Edition （以下简称 MCBE）提供客户端插件开发框架。

## Usage

关闭游戏，双击 sapphire_launcher.exe，游戏会被自动启动，且路径 ./sapphire/mods 下的所有 dll 会被注入进游戏中。

使用须知：

-   只支持 64 位 UWP 版本，且游戏必须以 DX12 模式运行，不支持以 DX11 模式运行的游戏。
-   只兼容 1.21.2、1.21.50、1.21.60 这 3 个版本，其它版本兼容性靠运气。

## plugin

Sapphire 包含一个用作示例的内建插件，位于 [plugins 文件夹](src/plugins)， 提供以下几个功能：

-   变速功能，支持音频变速：
    alt+keypad_add 加速，alt+keypad_sub 减速，alt+keypad_delete 恢复原速
-   活塞与 MovingBlock 渲染优化。
-   活塞更新顺序可视化。
-   显示区块边界。
-   自由相机。

你可以通过 alt+p 查看设置。

你也可以自行开发插件。但由于本项目是新项目，所以缺乏教程和文档。

## Issue

如果你发现任何 bug，请直接报 Issue。

## Feature Request

如果你有任何建设性想法，欢迎在 Issues 或 Discussions 页面提出。

## Contribute

本项目没有太多要求，欢迎任何人来添砖加瓦。

## Dependencies

-   [ImGui](https://github.com/ocornut/imgui)
-   [MinHook](https://github.com/TsudaKageyu/minhook)
-   [kiero](https://github.com/Rebzzel/kiero)
-   [nlohmann](https://github.com/nlohmann/json)
-   [fmod](https://www.fmod.com/download)
-   [EnTT](https://github.com/skypjack/entt)
-   [GSL](https://github.com/microsoft/GSL)
-   [moodycamel](https://github.com/cameron314/concurrentqueue)

## Credits

-   [LeviLamina](https://github.com/LiteLDev/LeviLamina)
-   [Latite](https://github.com/LatiteClient/Latite)

## License

src 文件夹下的所有代码按照 GPLv3 协议开源。
external/fmod 下的代码归 Firelight Technologies Pty Ltd 所有
其余代码保留原始协议
