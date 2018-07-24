//
//  LoginVCViewController.m
//  XLBasePage
//
//  Created by jingwei cai on 7/24/18.
//  Copyright © 2018 ZXL. All rights reserved.
//

#import "LoginVCViewController.h"

@interface LoginVCViewController ()
@property (strong, nonatomic) UILabel *uid_label;
@property (strong, nonatomic) UITextView *uid_text;
@property (strong, nonatomic) UILabel *psw_label;
@property (strong, nonatomic) UITextView *psw_text;
@property (strong, nonatomic) UIButton *confirm_btn;
@end

@implementation LoginVCViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.uid_label = [[UILabel alloc] init];
    self.uid_label.text = @"user id:";
    self.uid_label.frame = CGRectMake(20, 100, 100, 30);
    
    self.uid_text = [[UITextView alloc] init];
    self.uid_text.text = @"";
    self.uid_text.frame = CGRectMake(20, 150, 100, 30);
    self.uid_text.backgroundColor = [UIColor lightGrayColor];
    
    self.psw_label = [[UILabel alloc] init];
    self.psw_label.text = @"password:";
    self.psw_label.frame = CGRectMake(20, 200, 100, 30);
    
    self.psw_text = [[UITextView alloc] init];
    self.psw_text.text = @"";
    self.psw_text.frame = CGRectMake(20, 250, 200, 30);
    self.psw_text.backgroundColor = [UIColor lightGrayColor];
    
    self.confirm_btn = [[UIButton alloc] init];
    [self.confirm_btn setTitle:@"go login" forState:UIControlStateNormal];
    [self.confirm_btn setTitle:@"go login" forState:UIControlStateHighlighted];
    [self.confirm_btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.confirm_btn setTitleColor:[UIColor blackColor] forState:UIControlStateHighlighted];
    self.confirm_btn.backgroundColor = [UIColor lightGrayColor];
    self.confirm_btn.frame = CGRectMake(80, 300, 100, 30);
    
    [self.confirm_btn addTarget:self action:@selector(goLogin:) forControlEvents:UIControlEventTouchUpInside];

    
    [self.view addSubview:self.uid_label];
    [self.view addSubview:self.uid_text];
    [self.view addSubview:self.psw_label];
    [self.view addSubview:self.psw_text];
    [self.view addSubview:self.confirm_btn];
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)goLogin:(UIButton*)sender{
    NSLog(@"click do goLogin");
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
