# 涂鸦网关 SDK

## SDK 目录结构说明

SDK的产物包目录说明如下表所示：

| 目录         | 子目录    | 描述                             | 备注                                                         |
| ------------ | --------- | -------------------------------- | ------------------------------------------------------------ |
| demos        | /         | 存放 SDK 的 demo                 | 参照 SDK demo 使用说明                                       |
|              | demo1     | 一个示例 demo                    | 一般会提供一个或者多个 demo                                  |
| sdk          | /         | SDK 的头文件和库文件             | 参照后续的功能接口说明                                       |
|              | include   | SDK 提供的 API 接口              |                                                              |
|              | lib       | SDK 编译得到的 libtuya_iot.a 文件 |                                                              |
| platforms    | /         | SDK 的开发编译环境和工具链       | 一个 SDK 产物包会包含指定的开发编译环境                      |
|              | platform1 | 一个编译环境和工具链             | 按照涂鸦平台导入标准完成标准化的编译环境和工具链，和库文件有强关联关系。 |
| build_app.sh | /         | demo 的编译入口                  |                                                              |
| CHANGELOG.md | /         | 版本修改的记录                   |                                                              |
| README.md    | /         | 介绍文档                         |                                                              |

## 使用说明

SDK 包括了库文件、编译工具链以及编译脚本，不需要用户进行任何其他的配置，直接就可以在当前的 SDK 产物目录环境下，进行开发、编译操作。同时也提供了 demo 示例，演示如何快速使用该 SDK 进行嵌入式应用开发。具体细节，请参照例程 。下面以 **demo_gw_dev_wired_wifi ** 为例说明编译以及执行过程。

### 编译

根据硬件平台，适配 demos 下的  tuya_iot_wifi_net.c  以及 tuya_iot_wired_net.c 接口函数。


编译 demo 。

```
编译命令：build_app.sh APP_PATH APP_NAME APP_VERSION

./build_app.sh demos/demo_gw_dev_wired_wifi demo_gw_dev_wired_wifi 1.0.0
```

### 执行

```
./output/demo_gw_dev_wired_wifi_1.0.0/demo_gw_dev_wired_wifi
```



文档说明参考涂鸦开发平台 SDK 文档。


