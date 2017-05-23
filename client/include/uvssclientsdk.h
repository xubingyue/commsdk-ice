#ifndef UVSSCLIENTSDK_H
#define UVSSCLIENTSDK_H

#ifdef _WIN32
#   ifdef UVSS_COMM_EXPORTS
#       define UVSS_COMM_API _declspec(dllexport)
#   else
#       define UVSS_COMM_API _declspec(dllimport)
#   endif
#else
#   define UVSS_COMM_API
#endif

// UVSSMessageCallback: 调试信息回调函数定义
// handle: 连接标识 (由Connect返回, 用来对于服务器建立的连接进行标识)
// 注: 在没有对应连接时 (如SDK初始化失败或连接UVSS服务器失败时), 值为-1
// code: 信息编号
// 1: 连接成功 (UVSSConnect)
// -1: 初始化错误 (UVSSInitialize)
// -2: 连接失败 (UVSSConnect)
// -3: 连接断开
// message: 信息描述
typedef void(*UVSSMessageCallback)(int handle, int code,
                                   const char *message);

// UVSSCheckInfoCallback: 车辆检查信息回调函数定义
// handle: 连接标识 (由Connect返回, 用来对于服务器建立的连接进行标识)
// UVSSImagePath: 车底图像路径
// PlateImagePath: 车牌图像路径
// Channel: 车检通道信息
// PlateNumber: 车牌号码
// Direction: 车辆方向
// CheckDateTime: 检查日期时间 (格式: 2016/1/1 13:01:02)
// Extension: 扩展信息, 一般为空 ("")
typedef void(*UVSSCheckInfoCallback)(int handle,
                                     const char *UVSSImagePath, const char *PlateImagePath,
                                     const char *Channel, const char *PlateNumber, const char *Direction,
                                     const char *CheckDateTime, const char *Extension);

#ifdef __cplusplus
extern "C" {
#endif

// SetUVSSMessageCallback: 设置调试信息回调函数
UVSS_COMM_API void SetUVSSMessageCallback(UVSSMessageCallback);

// SetUVSSCheckInfoCallback: 设置车辆检测信息回调函数
UVSS_COMM_API void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback);

// UVSSInitialize: 初始化SDK (仅需调用一次)
// 返回值: 1: 初始化成功 -1: 出错
UVSS_COMM_API int UVSSInitialize();

// UVSSUninitialize: 销毁SDK, 释放占用的资源 (仅在程序退出时调用一次)
UVSS_COMM_API void UVSSUninitialize();

// UVSSConnect: 连接UVSS服务器
// ServerIPAddress: UVSS服务器IP地址, 默认值: 127.0.0.1
// ServerPort: UVSS服务器端口, 默认值: 20145
// 返回值:
// 连接成功: 大于0的连接标识 (handle)
// 出错: -1
// 已连接: -2 (不进行连接, 对已建立连接无影响)
// 与服务器版本号不一致: -3 (不进行连接)
UVSS_COMM_API int UVSSConnect(const char*, int);

// UVSSDisconnect: 与服务器断开连接
// handle: 连接标识 (由Connect返回, 用来对于服务器建立的连接进行标识)
// 返回值: 1: 成功, -1: 无指定handle对应的连接
UVSS_COMM_API int UVSSDisconnect(int);

// GetUVSSVersion: 获取版本号
// 返回值：版本号
UVSS_COMM_API const char* GetUVSSVersion();

#ifdef __cplusplus
}
#endif

#endif
