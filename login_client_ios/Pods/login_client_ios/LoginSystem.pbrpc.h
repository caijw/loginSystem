#if !defined(GPB_GRPC_FORWARD_DECLARE_MESSAGE_PROTO) || !GPB_GRPC_FORWARD_DECLARE_MESSAGE_PROTO
#import "LoginSystem.pbobjc.h"
#endif

#if !defined(GPB_GRPC_PROTOCOL_ONLY) || !GPB_GRPC_PROTOCOL_ONLY
#import <ProtoRPC/ProtoService.h>
#import <ProtoRPC/ProtoRPC.h>
#import <RxLibrary/GRXWriteable.h>
#import <RxLibrary/GRXWriter.h>
#endif

@class LSShelloRequest;
@class LSShelloResponse;
@class LSSloginRequest;
@class LSSloginResponse;
@class LSSregisterRequest;
@class LSSregisterResponse;
@class LSSverifySTRequest;
@class LSSverifySTResponse;

#if !defined(GPB_GRPC_FORWARD_DECLARE_MESSAGE_PROTO) || !GPB_GRPC_FORWARD_DECLARE_MESSAGE_PROTO
#endif

@class GRPCProtoCall;


NS_ASSUME_NONNULL_BEGIN

@protocol LSSLoginSystem <NSObject>

#pragma mark hello(helloRequest) returns (helloResponse)

/**
 * test hello
 */
- (void)helloWithRequest:(LSShelloRequest *)request handler:(void(^)(LSShelloResponse *_Nullable response, NSError *_Nullable error))handler;

/**
 * test hello
 */
- (GRPCProtoCall *)RPCTohelloWithRequest:(LSShelloRequest *)request handler:(void(^)(LSShelloResponse *_Nullable response, NSError *_Nullable error))handler;


#pragma mark registerAccount(registerRequest) returns (registerResponse)

/**
 * 注册接口
 */
- (void)registerAccountWithRequest:(LSSregisterRequest *)request handler:(void(^)(LSSregisterResponse *_Nullable response, NSError *_Nullable error))handler;

/**
 * 注册接口
 */
- (GRPCProtoCall *)RPCToregisterAccountWithRequest:(LSSregisterRequest *)request handler:(void(^)(LSSregisterResponse *_Nullable response, NSError *_Nullable error))handler;


#pragma mark loginAccount(loginRequest) returns (stream loginResponse)

/**
 * 登录接口
 */
- (void)loginAccountWithRequest:(LSSloginRequest *)request eventHandler:(void(^)(BOOL done, LSSloginResponse *_Nullable response, NSError *_Nullable error))eventHandler;

/**
 * 登录接口
 */
- (GRPCProtoCall *)RPCTologinAccountWithRequest:(LSSloginRequest *)request eventHandler:(void(^)(BOOL done, LSSloginResponse *_Nullable response, NSError *_Nullable error))eventHandler;


#pragma mark verifyST(verifySTRequest) returns (verifySTResponse)

/**
 * session ticket 校验接口
 */
- (void)verifySTWithRequest:(LSSverifySTRequest *)request handler:(void(^)(LSSverifySTResponse *_Nullable response, NSError *_Nullable error))handler;

/**
 * session ticket 校验接口
 */
- (GRPCProtoCall *)RPCToverifySTWithRequest:(LSSverifySTRequest *)request handler:(void(^)(LSSverifySTResponse *_Nullable response, NSError *_Nullable error))handler;


@end


#if !defined(GPB_GRPC_PROTOCOL_ONLY) || !GPB_GRPC_PROTOCOL_ONLY
/**
 * Basic service implementation, over gRPC, that only does
 * marshalling and parsing.
 */
@interface LSSLoginSystem : GRPCProtoService<LSSLoginSystem>
- (instancetype)initWithHost:(NSString *)host NS_DESIGNATED_INITIALIZER;
+ (instancetype)serviceWithHost:(NSString *)host;
@end
#endif

NS_ASSUME_NONNULL_END

