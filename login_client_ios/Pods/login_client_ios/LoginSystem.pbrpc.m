#if !defined(GPB_GRPC_PROTOCOL_ONLY) || !GPB_GRPC_PROTOCOL_ONLY
#import "LoginSystem.pbrpc.h"
#import "LoginSystem.pbobjc.h"
#import <ProtoRPC/ProtoRPC.h>
#import <RxLibrary/GRXWriter+Immediate.h>


@implementation LSSLoginSystem

// Designated initializer
- (instancetype)initWithHost:(NSString *)host {
  self = [super initWithHost:host
                 packageName:@"login_system"
                 serviceName:@"LoginSystem"];
  return self;
}

// Override superclass initializer to disallow different package and service names.
- (instancetype)initWithHost:(NSString *)host
                 packageName:(NSString *)packageName
                 serviceName:(NSString *)serviceName {
  return [self initWithHost:host];
}

#pragma mark - Class Methods

+ (instancetype)serviceWithHost:(NSString *)host {
  return [[self alloc] initWithHost:host];
}

#pragma mark - Method Implementations

#pragma mark hello(helloRequest) returns (helloResponse)

/**
 * test hello
 */
- (void)helloWithRequest:(LSShelloRequest *)request handler:(void(^)(LSShelloResponse *_Nullable response, NSError *_Nullable error))handler{
  [[self RPCTohelloWithRequest:request handler:handler] start];
}
// Returns a not-yet-started RPC object.
/**
 * test hello
 */
- (GRPCProtoCall *)RPCTohelloWithRequest:(LSShelloRequest *)request handler:(void(^)(LSShelloResponse *_Nullable response, NSError *_Nullable error))handler{
  return [self RPCToMethod:@"hello"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[LSShelloResponse class]
        responsesWriteable:[GRXWriteable writeableWithSingleHandler:handler]];
}
#pragma mark registerAccount(registerRequest) returns (registerResponse)

/**
 * 注册接口
 */
- (void)registerAccountWithRequest:(LSSregisterRequest *)request handler:(void(^)(LSSregisterResponse *_Nullable response, NSError *_Nullable error))handler{
  [[self RPCToregisterAccountWithRequest:request handler:handler] start];
}
// Returns a not-yet-started RPC object.
/**
 * 注册接口
 */
- (GRPCProtoCall *)RPCToregisterAccountWithRequest:(LSSregisterRequest *)request handler:(void(^)(LSSregisterResponse *_Nullable response, NSError *_Nullable error))handler{
  return [self RPCToMethod:@"registerAccount"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[LSSregisterResponse class]
        responsesWriteable:[GRXWriteable writeableWithSingleHandler:handler]];
}
#pragma mark loginAccount(loginRequest) returns (loginResponse)

/**
 * 登录接口
 */
- (void)loginAccountWithRequest:(LSSloginRequest *)request handler:(void(^)(LSSloginResponse *_Nullable response, NSError *_Nullable error))handler{
  [[self RPCTologinAccountWithRequest:request handler:handler] start];
}
// Returns a not-yet-started RPC object.
/**
 * 登录接口
 */
- (GRPCProtoCall *)RPCTologinAccountWithRequest:(LSSloginRequest *)request handler:(void(^)(LSSloginResponse *_Nullable response, NSError *_Nullable error))handler{
  return [self RPCToMethod:@"loginAccount"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[LSSloginResponse class]
        responsesWriteable:[GRXWriteable writeableWithSingleHandler:handler]];
}
#pragma mark verifyST(verifySTRequest) returns (verifySTResponse)

/**
 * session ticket 校验接口
 */
- (void)verifySTWithRequest:(LSSverifySTRequest *)request handler:(void(^)(LSSverifySTResponse *_Nullable response, NSError *_Nullable error))handler{
  [[self RPCToverifySTWithRequest:request handler:handler] start];
}
// Returns a not-yet-started RPC object.
/**
 * session ticket 校验接口
 */
- (GRPCProtoCall *)RPCToverifySTWithRequest:(LSSverifySTRequest *)request handler:(void(^)(LSSverifySTResponse *_Nullable response, NSError *_Nullable error))handler{
  return [self RPCToMethod:@"verifyST"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[LSSverifySTResponse class]
        responsesWriteable:[GRXWriteable writeableWithSingleHandler:handler]];
}
@end
#endif
