//
//  ViewController.m
//  login_client_ios
//
//  Created by jingwei cai on 7/24/18.
//  Copyright © 2018 jingwei cai. All rights reserved.
//
#import "ViewController.h"
#import "LoginVCViewController.h"
#import "RegisterVCViewController.h"
#import "HWHelloWorld.h"

@interface ViewController () <XLBasePageControllerDelegate,XLBasePageControllerDataSource>

@property (nonatomic,strong) NSArray *titleArray;
@property (nonatomic,strong) UIView *headerView;

@end

@implementation ViewController{
    HWHelloWorld *_cppApi;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor colorWithWhite:0.2 alpha:0.2];
    self.view.backgroundColor = [UIColor whiteColor];
    
    _titleArray = @[@"login",@"register"];
    
    
    self.delegate = self;
    self.dataSource = self;
    
    //self.lineWidth = 2.0;//选中下划线宽度
    self.titleFont = [UIFont systemFontOfSize:16.0];
    self.defaultColor = [UIColor blackColor];//默认字体颜色
    self.chooseColor = [UIColor blueColor];//选中字体颜色
    self.selectIndex = 0;//默认选中第几页
    
    [self reloadScrollPage];
    
    _cppApi = [HWHelloWorld create];
    NSString *str = [_cppApi getHelloWorld];
    
    NSLog(@"cpp out put: %@", str);
    
    
}

-(NSInteger)numberViewControllersInViewPager:(XLBasePageController *)viewPager
{
    return _titleArray.count;
}

-(UIViewController *)viewPager:(XLBasePageController *)viewPager indexViewControllers:(NSInteger)index
{
    if (index == 0) {
        //login
        LoginVCViewController *loginVCViewController = [[LoginVCViewController alloc] init];
        loginVCViewController.titleStr = _titleArray[index];
        loginVCViewController.index = index;
        return loginVCViewController;
    }else{
        //register
        RegisterVCViewController *registerVCViewController = [[RegisterVCViewController alloc] init];
        registerVCViewController.titleStr = _titleArray[index];
        registerVCViewController.index = index;
        return registerVCViewController;
    }
}

-(CGFloat)heightForTitleViewPager:(XLBasePageController *)viewPager
{
    return 50;
}

-(NSString *)viewPager:(XLBasePageController *)viewPager titleWithIndexViewControllers:(NSInteger)index
{
    return self.titleArray[index];
}

-(void)viewPagerViewController:(XLBasePageController *)viewPager didFinishScrollWithCurrentViewController:(UIViewController *)viewController
{
    self.title = viewController.title;
}

#pragma mark 预留--可不实现

//-(UIView *)headerViewForInViewPager:(XLBasePageController *)viewPager
//{
//    return self.headerView;
//}

-(CGFloat)heightForHeaderViewPager:(XLBasePageController *)viewPager
{
    return 100;
}

-(UIView *)headerView
{
    if (_headerView == nil) {
        _headerView = [[UIView alloc] init];
        _headerView.backgroundColor = [UIColor colorWithRed:120/255.0f green:210/255.0f blue:249/255.0f alpha:1];
        UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(0, 10, self.view.bounds.size.width, 40)];
        label.textColor = [UIColor grayColor];
        label.font = [UIFont systemFontOfSize:12.0];
        label.text = @"固定的头View,不可跟随滑动,可不显示";
        label.textAlignment = NSTextAlignmentCenter;
        [_headerView addSubview:label];
    }
    return _headerView;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end

