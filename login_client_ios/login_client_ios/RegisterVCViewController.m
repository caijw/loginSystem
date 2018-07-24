//
//  RegisterVCViewController.m
//  XLBasePage
//
//  Created by jingwei cai on 7/24/18.
//  Copyright © 2018 ZXL. All rights reserved.
//

#import "RegisterVCViewController.h"

@interface RegisterVCViewController ()
@property (strong, nonatomic) UILabel *nickname_label;
@property (strong, nonatomic) UITextView *nickname_text;
@property (strong, nonatomic) UILabel *psw_label;
@property (strong, nonatomic) UITextView *psw_text;
@property (strong, nonatomic) UILabel *phone_num_label;
@property (strong, nonatomic) UITextView *phone_num_text;
@property (strong, nonatomic) UIButton *confirm_btn;
@end

@implementation RegisterVCViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.nickname_label = [[UILabel alloc] init];
    self.nickname_label.text = @"nickname:";
    self.nickname_label.frame = CGRectMake(20, 100, 100, 30);
    
    self.nickname_text = [[UITextView alloc] init];
    self.nickname_text.text = @"";
    self.nickname_text.frame = CGRectMake(20, 150, 200, 30);
    self.nickname_text.backgroundColor = [UIColor lightGrayColor];
    
    self.psw_label = [[UILabel alloc] init];
    self.psw_label.text = @"password:";
    self.psw_label.frame = CGRectMake(20, 200, 100, 30);
    
    self.psw_text = [[UITextView alloc] init];
    self.psw_text.text = @"";
    self.psw_text.frame = CGRectMake(20, 250, 200, 30);
    self.psw_text.backgroundColor = [UIColor lightGrayColor];
    
    self.phone_num_label = [[UILabel alloc] init];
    self.phone_num_label.text = @"phone number:";
    self.phone_num_label.frame = CGRectMake(20, 300, 200, 30);
    
    self.phone_num_text = [[UITextView alloc] init];
    self.phone_num_text.text = @"";
    self.phone_num_text.frame = CGRectMake(20, 350, 200, 30);
    self.phone_num_text.backgroundColor = [UIColor lightGrayColor];
    
    self.confirm_btn = [[UIButton alloc] init];
    [self.confirm_btn setTitle:@"go register" forState:UIControlStateNormal];
    [self.confirm_btn setTitle:@"go register" forState:UIControlStateHighlighted];
    [self.confirm_btn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [self.confirm_btn setTitleColor:[UIColor blackColor] forState:UIControlStateHighlighted];
    self.confirm_btn.backgroundColor = [UIColor lightGrayColor];
    self.confirm_btn.frame = CGRectMake(80, 400, 100, 30);
    [self.confirm_btn addTarget:self action:@selector(goRegister:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.nickname_label];
    [self.view addSubview:self.nickname_text];
    [self.view addSubview:self.psw_label];
    [self.view addSubview:self.psw_text];
    [self.view addSubview:self.phone_num_label];
    [self.view addSubview:self.phone_num_text];
    [self.view addSubview:self.confirm_btn];
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)goRegister:(UIButton*)sender{
    NSLog(@"click do goRegister");
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
