//
//  LoginVCViewController.m
//  XLBasePage
//
//  Created by jingwei cai on 7/24/18.
//  Copyright © 2018 ZXL. All rights reserved.
//

#import "LoginVCViewController.h"
#import <login_client_ios/LoginSystem.pbrpc.h>
#import "LSSCrypto.h"
extern LSSLoginSystem *loginClient;
extern NSString *ST;
@interface LoginVCViewController ()
@property (strong, nonatomic) UILabel *uid_label;
@property (strong, nonatomic) UITextView *uid_text;
@property (strong, nonatomic) UILabel *psw_label;
@property (strong, nonatomic) UITextView *psw_text;
@property (strong, nonatomic) UIButton *confirm_login_btn;
@property (strong, nonatomic) UIButton *confirm_logout_btn;
@end

@implementation LoginVCViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.uid_label = [[UILabel alloc] init];
    self.uid_label.text = @"user id:";
    self.uid_label.frame = CGRectMake(20, 100, 200, 30);
    
    self.uid_text = [[UITextView alloc] init];
    self.uid_text.text = @"";
    self.uid_text.frame = CGRectMake(20, 150, 200, 30);
    self.uid_text.backgroundColor = [UIColor lightGrayColor];
    
    self.psw_label = [[UILabel alloc] init];
    self.psw_label.text = @"password:";
    self.psw_label.frame = CGRectMake(20, 200, 200, 30);
    
    self.psw_text = [[UITextView alloc] init];
    self.psw_text.text = @"";
    self.psw_text.frame = CGRectMake(20, 250, 200, 30);
    self.psw_text.backgroundColor = [UIColor lightGrayColor];
    
    self.confirm_login_btn = [[UIButton alloc] init];
    [self.confirm_login_btn setTitle:@"go login" forState:UIControlStateNormal];
    [self.confirm_login_btn setTitle:@"go login" forState:UIControlStateHighlighted];
    [self.confirm_login_btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.confirm_login_btn setTitleColor:[UIColor blackColor] forState:UIControlStateHighlighted];
    self.confirm_login_btn.backgroundColor = [UIColor lightGrayColor];
    self.confirm_login_btn.frame = CGRectMake(80, 300, 100, 30);
    
    self.confirm_logout_btn = [[UIButton alloc] init];
    [self.confirm_logout_btn setTitle:@"go logout" forState:UIControlStateNormal];
    [self.confirm_logout_btn setTitle:@"go logout" forState:UIControlStateHighlighted];
    [self.confirm_logout_btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.confirm_logout_btn setTitleColor:[UIColor blackColor] forState:UIControlStateHighlighted];
    self.confirm_logout_btn.backgroundColor = [UIColor lightGrayColor];
    self.confirm_logout_btn.frame = CGRectMake(80, 350, 100, 30);
    
    [self.confirm_login_btn addTarget:self action:@selector(goLogin:) forControlEvents:UIControlEventTouchUpInside];

//    [self.confirm_logout_btn addTarget:self action:@selector(goLogin:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.uid_label];
    [self.view addSubview:self.uid_text];
    [self.view addSubview:self.psw_label];
    [self.view addSubview:self.psw_text];
    [self.view addSubview:self.confirm_login_btn];
//    [self.view addSubview:self.confirm_logout_btn];

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}




-(void)goLogin:(UIButton*)sender{
    NSLog(@"click do goLogin");
    NSString *uid = self.uid_text.text;
    NSString *psw = self.psw_text.text;
    LSSCrypto *_cpp_crypto_api = [LSSCrypto create];
    int32_t numTimestamp = [[NSDate date] timeIntervalSince1970];
    NSString *timestamp = [NSString stringWithFormat:@"%d", numTimestamp];
    int32_t tCost = 100;
    int32_t mCost = 100;
    int32_t parallelism = 100;
    NSString *salt = @"";  /*should use a random num as the salt and pass it to the regist server*/
    NSString *hash = [_cpp_crypto_api argon2:psw tCost:tCost mCost:mCost parallelism:parallelism salt:salt];
    NSString *padding = @"x";
    NSString *tmp_uid = [_cpp_crypto_api stringWithFixedLength:uid length:10 padding:padding];
    NSString *tmp_timestamp = [_cpp_crypto_api stringWithFixedLength:timestamp length:10 padding:padding];
    NSLog(@"hash: %@, tmp_uid: %@, tmp_timestamp: %@", hash, tmp_uid, tmp_timestamp);
    NSString *data = [_cpp_crypto_api AESEnc:[NSString stringWithFormat:@"%@%@", tmp_uid, tmp_timestamp]  key:hash];
    NSLog(@"hash: %@, data: %@", hash, data);
    LSSloginRequest *request = [LSSloginRequest message];
    request.data_p =  data;
    request.userId = uid;
    NSLog(@"userId: %@; data: %@", uid, data);
    
    [loginClient loginAccountWithRequest:request eventHandler:^(BOOL done,  LSSloginResponse *response, NSError *error) {
        if(response){
            int32_t ret = response.ret;
            NSString *msg = response.msg;
            BOOL logout = response.logout;
            NSString *tmpST = response.st;
            if(logout){
                /*force to logout*/
                ST = @"";
                UIAlertController *alertMessage;
                alertMessage = [UIAlertController alertControllerWithTitle: @"退出登录"   message:@"有其他设备登录了该账户" preferredStyle:UIAlertControllerStyleAlert];
                [alertMessage addAction:[UIAlertAction actionWithTitle:@"comfirm" style:UIAlertActionStyleDefault handler:nil]];
                
                [self presentViewController:alertMessage animated:YES completion:nil];
                
            }else if(ret == 0 && tmpST){
                ST = tmpST;
                UIAlertController *alertMessage;
                alertMessage = [UIAlertController alertControllerWithTitle: @"登录成功"   message:[NSString stringWithFormat:@"%@%@", @"session ticket:", ST] preferredStyle:UIAlertControllerStyleAlert];
                [alertMessage addAction:[UIAlertAction actionWithTitle:@"comfirm" style:UIAlertActionStyleDefault handler:nil]];
                
                [self presentViewController:alertMessage animated:YES completion:nil];
            }else{
                UIAlertController *alertMessage;
                alertMessage = [UIAlertController alertControllerWithTitle: @"登录失败"   message:[NSString stringWithFormat:@"%@%d%@%@", @"ret:", ret, @";msg:", msg] preferredStyle:UIAlertControllerStyleAlert];
                [alertMessage addAction:[UIAlertAction actionWithTitle:@"comfirm" style:UIAlertActionStyleDefault handler:nil]];
                
                [self presentViewController:alertMessage animated:YES completion:nil];
            }
        }else if(error){
            ST = @"";
            NSString *str =[NSString stringWithFormat:@"RPC error: %@", error];
            NSLog(@"RPC error: %@", error);
            UIAlertController *alertMessage;
            alertMessage = [UIAlertController alertControllerWithTitle: @"登录失败"   message:str preferredStyle:UIAlertControllerStyleAlert];
            [alertMessage addAction:[UIAlertAction actionWithTitle:@"comfirm" style:UIAlertActionStyleDefault handler:nil]];
            
            [self presentViewController:alertMessage animated:YES completion:nil];
        }else if(done){
            ST = @"";
            NSString *str = @"登录结束";
            NSLog(@"login finish: %@", str);
            UIAlertController *alertMessage;
            alertMessage = [UIAlertController alertControllerWithTitle: @"登录"   message:str preferredStyle:UIAlertControllerStyleAlert];
            [alertMessage addAction:[UIAlertAction actionWithTitle:@"comfirm" style:UIAlertActionStyleDefault handler:nil]];
            
            [self presentViewController:alertMessage animated:YES completion:nil];
        }

    }];
    
}
/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
