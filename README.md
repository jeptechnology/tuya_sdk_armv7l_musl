# Tuya Gateway SDK

## SDK directory structure description

The SDK product package catalog description is shown in the following table:

| Directory    | Sub-category | Description                                | Remarks                                                   |
| ------------ | ------------ | ------------------------------------------ | --------------------------------------------------------- |
| demos        | /            | Store the demo of the SDK                  | Refer to the SDK demo instructions                        |
|              | demo1        | A sample demo                              | Generally, one or more demos are provided                 |
| sdk          | /            | SDK header files and library files         | Refer to the subsequent functional interface description  |
|              | include      | API interface provided by SDK              |                                                           |
|              | lib          | libtuya_iot.a file compiled by SDK         |                                                           |
| platforms    | /            | SDK development and compilation            | An SDK product package will include the specified         |
|              |              |  environment and tool chain                | development and compilation environment                   | 
|              | platform1    | A compilation environment and tool chain   | A standardized compilation environment and tool chain are completed in accordance with the Tuya platform import standards, which have a strong relationship with library files. |
| build_app.sh | /            | Compile entry for demo                     |                                                           |
| CHANGELOG.md | /            | Version modification record                |                                                           |
| README.md    | /            | Introduction document                      |                                                           |

## Instructions for use

The SDK includes library files, compilation tool chain, and compilation scripts. It does not require any other configuration by the user, and can directly perform development and compilation operations in the current SDK product directory environment. A demo example is also provided to demonstrate how to quickly use the SDK for embedded application development. For specific details, please refer to the routine. The following takes **demo_gw_dev_wired_wifi ** as an example to illustrate the compilation and execution process.

### Compile

According to the hardware platform, adapt the tuya_iot_wifi_net.c and tuya_iot_wired_net.c interface functions under demos.


Compile the demo.

```
Compile command: build_app.sh APP_PATH APP_NAME APP_VERSION

./build_app.sh demos/demo_gw_dev_wired_wifi demo_gw_dev_wired_wifi 1.0.0
```

### carried out

```
./output/demo_gw_dev_wired_wifi_1.0.0/demo_gw_dev_wired_wifi
```



For document description, please refer to Tuya Development Platform SDK document.

