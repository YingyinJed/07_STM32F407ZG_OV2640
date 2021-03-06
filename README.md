# STM32F407 OV2640

#### 介绍
基于正点原子F4标准库OV2640实验移植到HAL库

#### 软件架构
软件架构说明


#### 需要注意的C文件

1.  ILI93xx.c(触摸屏代码)
2.  ov2640.c(ov2640和SCCB通信代码)
3.  dcmi.c(在USER CODE BEGIN Prototypes区域有DCMI传输相关函数)
4.  main.c

#### 使用说明

1.  根据CUBEMX的引脚说明进行期间连接

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request


#### 特技

1.  使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2.  Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3.  你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4.  [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5.  Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6.  Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
