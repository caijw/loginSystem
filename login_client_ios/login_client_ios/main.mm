//
//  main.mm
//  login_client_ios
//
//  Created by jingwei cai on 7/24/18.
//  Copyright © 2018 jingwei cai. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import <GRPCClient/GRPCCall+ChannelArg.h>
#import <GRPCClient/GRPCCall+Tests.h>
#import <login_client_ios/LoginSystem.pbrpc.h>

static NSString * const kHostAddress = @"localhost:50051";
LSSLoginSystem *loginClient;
NSString *ST;
int main(int argc, char * argv[]) {
    @autoreleasepool {
        
        [GRPCCall useInsecureConnectionsForHost:kHostAddress];
        [GRPCCall setUserAgentPrefix:@"LoginSystem/1.0" forHost:kHostAddress];
        
        loginClient = [[LSSLoginSystem alloc] initWithHost:kHostAddress];
        
        
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
