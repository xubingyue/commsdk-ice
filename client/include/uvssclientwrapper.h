#ifndef UVSSCLIENTWRAPPER_H
#define UVSSCLIENTWRAPPER_H

#ifdef _WIN32
#   ifdef UVSS_EXPORTS
#      define UVSS_API _declspec(dllexport)
#   else
#      define UVSS_API _declspec(dllimport)
#   endif
#else
#   define UVSS_API
#endif

// UVSSMessageCallback: 调试信息回调函数定义
// connectionId: 连接标识 (由UVSSConnect返回, 用来标识与UVSS服务器建立的连接)
// 注: 正常情况下值为正整数; 在没有对应连接时 (SDK初始化失败或连接UVSS服务器失败), 值为-1
// code: 信息编号
// 1: 连接成功 (UVSSConnect)
// -1: 初始化错误 (UVSSInitialize)
// -2: 连接失败 (UVSSConnect)
// -3: 连接断开
// message: 信息描述
typedef void(*UVSSMessageCallback)(int connectionId, int code,
                                   const char* message);

// UVSSCheckInfoCallback: 车辆检查信息回调函数定义
// connectionId: 连接标识 (由UVSSConnect返回, 用来标识与UVSS服务器建立的连接)
// uvssImagePath: 车底图像路径
// plateImagePath: 车牌图像路径
// channel: 车检通道信息
// plateNumber: 车牌号码
// direction: 车辆方向
// dateTime: 检查日期时间 (格式: yyyy/m/d h:mm:ss, 如2016/1/1 13:01:02)
// extension: 扩展信息, 一般为空 ("")
typedef void(*UVSSCheckInfoCallback)(int connectionId,
                                     const char* uvssImagePath,
                                     const char* plateImagePath,
                                     const char* channel,
                                     const char* plateNumber,
                                     const char* direction,
                                     const char* dateTime,
                                     const char* extension);

// UVSSCheckInfoExCallback: 车辆检查信息Ex回调函数定义
// connectionId: 连接标识 (由UVSSConnect返回, 用来标识与UVSS服务器建立的连接)
// concatedString: 任意字符串
// concatedFilePath: 任意数量的文件路径连接成的字符串, 各路径间用 "|" 分隔
typedef void(*UVSSCheckInfoExCallback)(int connectionId,
                                       const char* concatedString,
                                       const char* concatedFilePath);

#ifdef __cplusplus
extern "C" {
#endif

// GetUVSSVersion: 获取客户端版本号
// 返回值：客户端版本号
UVSS_API const char* GetUVSSVersion();

// SetUVSSMessageCallback: 设置调试信息回调函数
UVSS_API void SetUVSSMessageCallback(UVSSMessageCallback uvssMessageCallback);

// SetUVSSCheckInfoCallback: 设置车辆检查信息回调函数
UVSS_API void SetUVSSCheckInfoCallback(UVSSCheckInfoCallback
                                       uvssCheckInfoCallback);

// SetUVSSCheckInfoExCallback: 设置车辆检查信息Ex回调函数
UVSS_API void SetUVSSCheckInfoExCallback(UVSSCheckInfoExCallback
                                         uvssCheckInfoExCallback);

// UVSSInitialize: 初始化SDK (仅需调用一次)
// 返回值: 1: 初始化成功, -1: 出错
UVSS_API int UVSSInitialize();

// UVSSConnect: 连接UVSS服务器
// ipAddress: UVSS服务器IP地址, 默认值: 127.0.0.1
// port: UVSS服务器端口, 默认值: 20145
// 返回值:
// 大于0的连接标识 (connectionId): 连接成功
// -1: 出错
// -2: 已连接(不进行连接, 对已建立连接无影响)
// -3: 与UVSS服务器版本号不一致(不进行连接)
UVSS_API int UVSSConnect(const char* ipAddress, int port);

// UVSSDisconnect: 与UVSS服务器断开连接
// connectionId: 连接标识 (由UVSSConnect返回, 用来标识与UVSS服务器建立的连接)
// 返回值: 1: 成功, -1: 无connectionId对应的连接
UVSS_API int UVSSDisconnect(int connectionId);

// SendUVSSCheckInfo: 发送车辆检查信息
// uvssImagePath: 车底图像路径
// plateImagePath: 车牌图像路径
// channel: 车检通道信息
// plateNumber: 车牌号码
// direction: 车辆方向
// dateTime: 检查日期时间 (格式: yyyy/m/d h:mm:ss, 如2016/1/1 13:01:02)
// extension: 扩展信息, 一般为空 ("")
UVSS_API void SendUVSSCheckInfo(const char* uvssImagePath,
                                const char* plateImagePath,
                                const char* channel,
                                const char* plateNumber,
                                const char* direction,
                                const char* dateTime,
                                const char* extension);

// SendUVSSCheckInfoByEndpoint: 按UVSS服务器端点发送车辆检查信息
// endpoint: UVSS服务器端点 (格式: ipAddress:port, 如127.0.0.1:20145)
// uvssImagePath: 车底图像路径
// plateImagePath: 车牌图像路径
// channel: 车检通道信息
// plateNumber: 车牌号码
// direction: 车辆方向
// dateTime: 检查日期时间 (格式: yyyy/m/d h:mm:ss, 如2016/1/1 13:01:02)
// extension: 扩展信息, 一般为空 ("")
UVSS_API void SendUVSSCheckInfoByEndpoint(const char* endpoint,
                                          const char* uvssImagePath,
                                          const char* plateImagePath,
                                          const char* channel,
                                          const char* plateNumber,
                                          const char* direction,
                                          const char* dateTime,
                                          const char* extension);

// SendUVSSCheckInfoEx: 发送车辆检查信息Ex
// concatedString: 任意字符串
// concatedFilePath: 任意数量的文件路径连接成的字符串, 各路径间用 "|" 分隔
UVSS_API void SendUVSSCheckInfoEx(const char* concatedString,
                                  const char* concatedFilePath);

// SendUVSSCheckInfoExByEndpoint: 按UVSS服务器端点发送车辆检查信息Ex
// endpoint: UVSS服务器端点 (格式: ipAddress:port, 如127.0.0.1:20145)
// concatedString: 任意字符串
// concatedFilePath: 任意数量的文件路径连接成的字符串, 各路径间用 "|" 分隔
UVSS_API void SendUVSSCheckInfoExByEndpoint(const char* endpoint,
                                            const char* concatedString,
                                            const char* concatedFilePath);

// UVSSUninitialize: 销毁SDK, 释放占用的资源 (仅在程序退出时调用一次)
UVSS_API void UVSSUninitialize();

#ifdef __cplusplus
}
#endif

#endif
