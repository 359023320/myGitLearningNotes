<涛哥git学习日记>
【2018.4.16】
1.git repo gerrit三者关系
git一次只能管理一个仓库
repo可以管理多个仓库
gerrit只是一个网页管理工具
2.git本地仓库/远程仓库/分布式版本管理/集中式版本管理
git是分布式版本管理工具,具有本地仓库
但是也有一个服务器管理提交
3.文件状态
untracked
tracked(staged, modified, unmodified)
4.git分支
分支是指向提交对象的可变指针
(HEAD->分支->commit)
5.git版本号
哈希值,一般取前7位

【2018.4.17】
1.在所有仓建git分支
repo start 分支名 --all(若没有分支名,则新建,若存在分支名,则所有仓切换到该分支)
【参见git仓库,repo命令,分支之间的关系】
2.强制更新所有仓,丢弃本地修改
repo forall -c "git clean -dfx; git reset --hard origin/master"
在所有仓下执行""里面的内容
git clean -dfx:删除未被跟踪的文件(包括忽略的)(例如自己添加的cfg.mak文件,但是modified文件不会被清除)
为什么cfg.mak属于untracked文件但是git status看不到呢？
因为它被加在了隐藏文件.gitignore里面被忽略掉了
git reset --hard origin/master:将本地仓库代码还原为与远程仓库一致
(origin/master叫远程分支,该分支在本地仓库,反映了本地仓库上一次与远程仓库的通信状态)

【2018.4.18】
1.git stash和git pop(另可参考7.6的笔记)
git stash -a save "message":将当前未上库的内容保存,"message"随便写
git stash pop:将stash中的内容还原
【注意】:
当我修改两个文件:如inject和crypto, git status显示俩为modified状态
在执行完git stash -a save后本地修改的文件就被还原了, git status为clear状态.
只有在git stash pop后修改的文件又保持原来的状态
【注意】:
git stash save可以保存多次修改
git stash pop只能取最近保存的那一次

2.修改记录查看git log
【git log写法的常用组合】:
git log -n:显示最近n次的提交
git log --pretty=oneline:一个提交显示一行
git log --name-only:在git log的基础上显示修改哪些文件
git lg:以图示的方式显示提交顺序
(自己一般的组合是:git log --pretty=oneline -n)

【2018.4.19】
1.查看分支状态(repo branches与git branch -vv)
两者都可以看到本地所有的分支, 区别在于:
repo branches:查看所有分支, 但注重查看分支在哪些仓下有
git branch -vv:查看当前仓库所有分支, 但是注重查看分支最新的commitId与对应的远程分支
【补充】:
<1>.git branch:查看当前仓库的所有本地分支
比如我在Code仓下创建一个mytest分支, 在Code仓下git branch就可以看到mytest分支
但是在CI仓下git branch就看不到mytest分支
说明git branch只能看到本仓库下的本地分支
<2>.git branch -r:查看当前仓库的所有远程分支
r=remote
<3>.git branch -a:查看当前仓库的所有本地分支与远程分支
a=all

2.创建分支
【注意】:建议不要在某一个单独的仓库下面创建分支(例如用git checkout -b branchname或者git branch branchname指令)
repo start BRANCHNAME --all:在所有仓下创建名为BRANCHNAME的分支, 若BRANCHNAME分支存在, 则所有仓的内容切换到BRANCHNAME分支上
只有用该方式创建的分支才能用repo upload提交, 因为它除了创建分支以外, 还会和远程分支关联

3.将其它提交应用到当前分支
git cherry-pick commitId
【补充】:详细请见git cherry-pick深入理解

4.本地分支应用场景
代码备份与提交协助:开发的时候在开发分支上提交保存, 再到提交分支上上传提交
见<涛哥git大讲堂>1

5.追加提交(git commit --amend)
假如我提交了一版v1(如修改crypto.c文件), 这时我又想提交, 但是不想新增commit(或者已经repo upload但是没有merge)
这时我修改了文件(如修改inject.c文件), 正常git add然后git commit --amend
这时会在刚才修改crypto.c那个提交上面去修改信息(注意:这时的commitId一定不同)
crypto.c是属于已经提交的, inject.c是新追加的提交
但是修改后的提交显示crypto.c与inject.c都是要commit的文件
最后重新repo upload上库即可
【原理】:
git commit --amend只会提交暂存区里面的东西, 若刚刚commit没有修改任何文件的情况下又git commit --amend的话仅会修改提交信息
最终只有一个提交, amend后的提交会代替之前的提交结果

【2018.4.24】
1.git打patch:git format-patch -n
将本地某几次提交发送给其他人
n可以随意指定, 如-1, -2等等
【注意】:如果n!=1则会产生多个patch, 一个提交产生一个patch, 需要把这些patch都发送给使用方
所以最好把需要的改动弄到一个提交上去
patch的使用:git apply xxx.patch
【注意】:此命令只会更改工作区代码, 不会产生新的提交

2.撤销没有上库的提交
(1).首先在gerrit上撤销:abandon
(2).代码上执行git reset --soft HEAD^

3.撤销commit:用新的commit来冲掉需要撤销的commit
【注意】:commit不能删除, 只能用新的commit来还原以前文件的状态
git revert 要撤销的版本号
同时可以在gerrit上点击revert来完成撤销工作
【例子】:例如我修改了inject.c提交了一版commit0
git revert commit0:产生新的commit1, inject.c又回到了0之前的状态

【2018.4.26】
1.git reset HEAD^, git reset --soft HEAD^, git reset --hard HEAD^的区别
(1).git reset HEAD^ == git reset --mixed HEAD^:commit回到HEAD^, HEAD与HEAD^之间的差异文件变为modified状态(红色)
(2).git reset --soft HEAD^:commit回到HEAD^, HEAD与HEAD^之间的差异文件为staged状态(绿色)
(3).git reset --hard HEAD^:commit回到HEAD^, 工作区所有文件都回到HEAD^状态, 在HEAD上modified的文件没有了
也可以用git reset --hard HEAD来去除本地的修改, 将本地环境还原为干净的commit

【2018.4.27】
1.git reset与git checkout的区别
(1).HEAD, 分支与commit之间的关系
正常情况下:HEAD->分支->commit(可参考.git文件夹中的HEAD文件)
如:cat HEAD
ref:refs/heads/test_20180427
非正常情况下(如执行了git chckout commitId命令后):HEAD->临时分支->commit
(2)【例子git checkout】
分别有三个提交:commit1, commit2, commit3(假设1,2,3是提交的hash值)
假设现在的分支是test_20180427
那么现在的关系是:HEAD->test_20180427->commit3
当执行git checkout 1时, HEAD指针指向临时分支1, 临时分支1指向commit1, 这时代码变成commit1的代码
那么现在的关系是:HEAD->temp1->commit1, test_20180427->commit3
【注意】:临时分支1只是临时创建的, 不能用于提交代码(repo upload)
(3)【例子git reset】
分别有三个提交:commit1, commit2, commit3(假设1,2,3都是提交的hash值)
假设现在的分支是test_20180427
那么现在的关系是:HEAD->test_20180427->commit3
当执行git reset HEAD^^后现在的关系变为:HEAD->test_20180427->commit1
所以git reset是HEAD连同分支一起移动, 便于提交代码(repo upload), 而分离的HEAD虽然得到了想要的代码, 却不便于repo upload提交代码

2.如何删除git分支
删除某个仓下面的分支:git branch -D branchname
删除所有仓下面的分支:repo forall -c "git branch -D branchname"
【注意】:不能在当前分支上删除当前分支！

3.如何利用git找东西
git grep "xxx"
repo forall -c "git grep "xxx""

【2018.5.3】
1.gerrit搜索功能使用
【注意】:可以随意组合, 多个条件用空格隔开
指定状态:status:open
指定作者:owner:chentao
指定仓库:project:LIS/LIS_STB_GIT/LiSTBLinux/Code
【注意】:该仓库路径是远程仓库路径而不是本地仓库路径, 以.repo/manifests/default.xml中的为准
指定分支:branch:master
指定时间:after:2017-1-1 before:2017-3-1
指定版本:commit:HASH值
搜索log:message:inject
指定文件:file:文件路径

【2018.5.4】
1.关于git revert冲突的问题(也可以见2018.11.13的分析笔记)
有四个提交:1->2->3->4(HEAD)
执行git revert 3的时候:
提交顺序:1->2->3->4->(!3)(HEAD)
代码顺序:1->2->4(就是将3之于2的修改去掉同时保留4的修改)
【注意】:有时候1, 2, 3, 4离得太近会有冲突, 1, 2, 3, 4离得较远就不会冲突

2.git冲突解决工具:git mergetool
(1)merge时冲突是怎么产生的
conflict:两个已经提交的分支的相同文件相同位置的不同操作进行了合并
【conflict例子】:
base为1, 分支为master, 修改line1, 提交为commit2(1->2(master))
base为1, 分支为dev, 修改为line2, 提交为commit3(1->3(dev))
line1与line2为同一个文件的同一处, 这时不管git merge master还是git merge dev, 都会显示conflict
【非conflict例子】:
非conflict的例子较多, 不一一列举
(2)git mergetool工具的使用
目前看来只是用在git merge产生的conflict能用, 方法如同普通的冲突处理方法
(3)git diff, git diff HEAD, git diff --cached复习
git diff:比较工作区与暂存区的差异
git diff HEAD:比较工作区与HEAD的差异
git diff --cached:比较暂存区与HEAD的差异

3.git rebase后commitID变化问题
有两个分支, master与dev:
1->3->4(master)
1->2(*dev)
git rebase master后:
1->3->4->2'(*dev)
此时这个2'与2的commitID不同, 但是提交的时间却是一样的

【2018.5.23】
1.初次运行git前的配置
git自带git config工具去配置git的外观与行为, 一共有三个文件去控制
(1)/etc/gitconfig文件:影响所有用户下面的所有git仓库
git config --system命令会涉及该文件

(2)~/.gitconfig文件:只影响当前用户下所有的git仓库
git config --global命令会涉及该文件

(3)某个git仓库下的.git/config文件:仅影响该git仓库
git config命令会涉及该文件

2.配置用户信息, 编辑器, 检查git配置
(1)git config --global user.name "Andy Blackwood"
   git config --gloabl user.email xxxx@gmail.com
   git config --global core.editor vim
   git config --list
   列出git能找到的所有配置(可能会显示出重复的变量名, 因为git会从不同的config文件里读取配置信息, 但是git应用的配置是重复变量名的最后一个)
  
(2)git config <key>:用来检查某一项的配置
如:git config user.name
如:git config --global core.editor

3.git 获取帮助git <verb> --help
如:git config --help

【2018.5.24】
1.HEAD->branch->commit怎么看
cd .git
cat HEAD
如:我按照上述步骤操作下来得到:ref:refs/heads/MYMASTER_CODEX
cd .git/refs/heads
cat MYMASTER_CODEX
如:我按照上述步骤操作下来得到:113b4522056eb36e52fee90198d72b1abe13e238

【2018.5.25】
1.获取新建git仓库的两种方法:git init与git clone
(1)git init
项目文件夹为myprogram, 里面有很多文件
cd myprogram
git init
git add *.c
git commit -s
(2)git clone
从远程仓库克隆git仓库到本地
格式为:git clone [url] <ownreponame>
【例子】
git clone https://github.com/libgit2/libgit2
会在当前路径下创建libgit2的文件夹, 里面就是远程库所有的内容了, 本地仓库的名字也是libgit2
git clone https://github.com/libgit2/libgit2 mygit
在当前路径下创建mygit文件夹, 里面就是远程库所有的内容了, 本地仓库的名字是mygit

2.gitignore的简单写法
.gitignore是什么
在一个git仓库中, commit后的文件被git仓库管理起来, 但是没有git add->git commit的文件就是untracked文件
在git status的时候就会列出这些未被跟踪的文件, 显示它们的状态为untracked
可以把这些文件加入.gitignore文件中, git status就不显示它们的状态了
【注意】:.gitignore文件只对untracked文件有效, 在.gitignore文件中#与空行都属于无效的
【例子】
tmp文件夹下git init一个git仓库
有文件:1.c 2.c 3.c 1.o 2.o 3.o
有文件夹:dir1/1-1.txt
dir1/1-2.txt
dir1/1-3.txt
dir2/2-1.txt
dir2/2-2.txt
dir2/2-3.txt
dir3/3-1.txt
dir3/3-2.txt
dir3/3-3.txt
其中*.c已经被git管理起来了, 我们要忽略所有的.o文件, 忽略dir1, dir2下面的文件, 忽略dir3下面的文件除了dir3/3-3.txt
touch .gitignore
vim .gitignore
-------------------------
#ignore *.o
*.o
#ignore dir1 dir2
/dir1
/dir2
#ignore dir3 but dir3/3-3.txt
/dir3/ *.txt
!/dir3/3-3.txt
#ignore .gitignore itself
.gitignore
-------------------------
保存退出后git status连.gitignore的untracked状态也看不到了
【注意】:bin/与/bin的区别
/bin:仅忽略当前路径下bin目录
bin/:忽略所有名字为bin的目录

【2018.5.28】
1.git commit, git commit -s, git commit -m的区别
(1)git commit -s
-------------------------------------------------------------------
[Module] Abstract 纯英文

[问题描述]详细描述修改的问题以及修改思路
[问题单号]Service单号/JIRA单号/无

Signed-off-by:andyliao<xxxxx@yyy.com>
#Please enter the commit message for your changes, lines starting
#with '#' will be ignored, and an empty message aborts the commit.
#On branch master
#Changes to be committed:
#modified:1.c
-------------------------------------------------------------------
Line1~5是要自己填写的基本信息
Line6是sign off信息, 提交人的工号, 邮箱
Line8~12是注释信息, 提示哪些文件会被提交

【注意】
git commit时会将注释信息丢弃, 只用填写的基本信息和sign off信息生成commit信息

(2)git commit
-------------------------------------------------------------------
[Module] Abstract 纯英文

[问题描述]详细描述修改的问题以及修改思路
[问题单号]Service单号/JIRA单号/无

#Please enter the commit message for your changes, lines starting
#with '#' will be ignored, and an empty message aborts the commit.
#On branch master
#Changes to be committed:
#modified:1.c
-------------------------------------------------------------------

【注意】
可以看出git commit -s与git commit的区别在于:
git commit少了sign off信息

(3)git commit -m "xxx"
不会打开文本编辑器, 会用"xxx"中的信息去代替需要填充的信息, 也没有sign off信息

2.git mv命令重命名文件
【格式】:git mv fileoldname filenewname
【例子】:git mv 1.c liao.c
这个时候git status会发现liao.c已经在暂存区等待提交了
git mv 1.c liao.c <=> mv 1.c liao.c <=> git rm 1.c <=> git add liao.c

【2018.5.29】
1.git log的扩展使用
(1).git log -p
显示每个commit提交的差异, 适用于查看某个commit修改的内容差异

(2).git log --stat
显示每个commit文件修改的汇总信息, 修改了哪些文件, 增加/减少多少行(比--name-only信息全)

(3).git log --pretty=format:"%h %cn %s" --graph
--graph表示以图表的方式展示提交关系, format表示以什么样的格式进行展示
%h:提交哈希值的简串
%cn:commiter
%s:提交说明

(4).git log -S keyname
查看引入/删除 keyname的提交
比如我想查看O5_DRV_INJ_InjectAudioEsThread是谁引入的
git log -S O5_DRV_INJ_InjectAudioEsThread

(5).如何用git log查看某一时间段的提交
git log --since="year-month-day" --until="year-month-day"
【例子】
2018年5月31日早晨帮助李鑫查找commit, 他只记得在3月份
git log --since="2018-03-01" --until="2018-04-01"
【注意】
这样看到的提交有些时间不在3-1~4-1的范围之内, 是由于这些提交是cherry-pick过来或者rebase过来的
由于cherry-pick或者rebase的commitID会变但是提交时间显示的原始时间, 所以看到的时间不在范围内
只是由于在这段时间内cherry-pick或者rebase过来的, 就有记录

【2018.5.30】
1.Git别名
【例子】
git config --gloabl alias.br branch
git br <=> git branch

git config --global alias.co checkout
git co <=> git checkout
同样也可以在~./下的.gitconfig文件中的[alias]中去修改, 相当于
[alias]
	br = branch
	co = checkout
	
【2018.5.31】
1.见5.29的(5)
2.git远程仓库的基本使用
git remote
查看当前本地仓库对应的远程仓库(一个本地仓库可以对应多个远程仓库)
git remote -v
查看当前本地仓库对应的远程仓库名与URL
git remote add <shortname> <url>
为当前本地git仓库添加一个远程仓库, 远程仓库地址为url, 名字为shortname指定
例如:git remote add paul https://github.com/paulboone/ticgit
git remote show [remote-name]
查看远程仓库信息
例如:git remote show origin
git remote rename [oldname] [newname]
为当前本地仓库的远程仓库改名
例如:git remote rename pb paul
git remote rm [remote-name]
删除当前本地仓库的远程仓库
例如:git remote rm paul

【2018.6.2】
1.git打标签
(1).列出标签
git tag:列出当前git仓库所有的tag
git tag -l "v1.8.5*":列出v1.8.5*的所有tag

(2).创建标签
git标签分为两种:轻量标签(lightweight)与附注标签(annotated)
轻量标签只是一个特定提交的引用, 而附注标签除此之外则包含了如作者信息, 邮箱等很多信息
(2.1)附注标签的创建
git tag -a v1.4 -m "xxx"
其中-a就是-annotated的意思, -m就是输入的message, 如果不加-m选项, 则会打开编辑器手动输入
若创建好后想查看标签信息:
git show v1.4 则会看到标签信息与标签对应的commit信息
(2.2)轻量标签的创建
git tag v1.4
这时若想看到标签的信息, 运行git show v1.4则只会看到对应的commit信息, 没有标签信息
(2.3)给某一个cimmitID打标签
【注意】:
上面创建标签的方法, 无论是git tag v1.4还是git tag -a v1.4都是给当前分支的HEAD所指向的commit打标签
【注意】:
若要在某个指定的commit上打标签需要在命令的末尾加上commitID(前七位哈希值即可)
【例子】:
git tag -a v1.4 f61d5f88915
git tag v1.4 f61d5f88915
(2.4)共享标签
涉及远程仓库无法实验:教程见progit2 Page62
(2.5)检出标签
【思考】:如何回退到某个标签所在的版本?
方法一:git show [tagname]得到标签所在的commitID
方法二:git checkout -b [newbranchname] [tagname]在标签所在的commit上创建一个新分支并切换上去

【2018.6.5】
1.git add, git commit原理(block, 树对象, 提交对象)
git的每一个提交(commit)不是保存的文件差异, 而是提交时的文件快照
git add的时候会为每一个要暂存的文件计算哈希值, 并将这些文件快照保存到block中
git commit时会根据每一个文件的哈希值生成树对象, 再生成一个提交对象commit
提交对象(commit)中会包含指向树对象的指针和如作者, 邮箱信息等的所有提交信息
通过提交对象, 可以找到树对象, 而树对象中又有文件快照的哈希值, 所以能访问到文件
【注意】:
提交对象commit中还包含指向上一个提交(父提交)的指针, 第一个commit没有, 后面的依次指向前面的, 以此类推

2.git分支的概念
git分支仅仅是指向提交对象(commit)的可变指针, 再具体一点, git分支是一个文件, 该文件中包含了41个字节
(commitID的40字节的哈希字符串和一个换行符)
每个仓库都有一个master指针, 默认由git init创建
【实验】
cd Code/.git/refs/heads
ls
可以看到:
AUDIO_STUDY
这就是本地分支, ls -la可以看到它的属性为文件, 大小为41Byte
-rw-r--r-- 1 41 5月31 09:39 AUDIO_STUDY
cat AUDIO_STUDY
f61d5f88915a97e88e91e935092a515498ea9660

【2018.6.6】
1.git log --pretty=oneline --decorate
查看当前仓库各个本地分支和tag指向的提交对象
2.复习git log知识点

【2018.6.7】
1.利用git处理问题的流程
(1)本身在master分支, 因为新增需求, 拉出iss53分支
(2)在iss53分支上进行开发
(3)master分支出了问题, 需要修改。切换回master分支, 在此基础上拉出hotfix分支进行修复
(4)修复验证完成将hotfix分支合入master分支
(5)最后切换回master分支继续工作

2.什么是git merge过程中的"fast-forward"
commit1->commit2->(*HEAD,master)commit3->(hotfix)commit4
git merge hotfix即是说将hotfix merge到master上, 而hotfix与master是一条直线的关系
commit1->commit2->commit3->(*HEAD, master, hotfix)commit4
合并完毕只会将master向右移动, 这种就叫快进:fast-forward

【2018.6.11】
1.与git branch有关的命令
git branch
列出当前仓的所有本地分支

git branch -v
查看当前仓库每一个本地分支的最后一次提交

git branch --merged
查看当前仓库哪些本地分支合并到了当前分支上
(如果已经合并过的分支可以用git branch -d删除, 否则提示-d出错, 需要用git branch -D删除)
git branch --no-merged
git branch -d
git branch -D

【2018.6.12】
1.分支开发工作流
(1)长期分支
(2)特性分支

2.远程分支
*远程分支存在于本地, 反应上一次通信状态时远程仓库分支中的状态
*远程分支的命名方式为(remote)/(branch)
其中remote是远程仓库的名字, 在git clone时默认远程仓库名为origin, 其中branch为远程仓库分支名
*所以当git clone发生时两个仓库(本地/远端)中共有三个分支
本地:master与origin/master
远端:master
【注意】:
如果不与远程仓库通信, 本地的远程分支是不会移动的

【2018.6.13】
1.git rebase原理
【注意】:git merge xxx是将xxx merge到HEAD
【注意】:git rebase xxx 是将HEAD rebase到xxx上
【例子】:
C0->C1->C2->C4(*experiment)
          ->C3(master)
git rebase master将HEAD(experiment)rebase到master上
C0->C1->C2->C3(master)->C4(*experiment)

【原理】:
rebase涉及两个分支:master(目标分支)和experiment(HEAD分支)
(1).找到目标分支与源分支的最近共同祖先C2
(2).源分支:找到基于C2的所有提交C4, 存储为临时文件
(3).源分支:将源分支指向目标分支(HEAD->experiment->C3)
(4).源分支:依据现有的指向, 将存储的临时文件依序应用, 最后更新源分支

【2018.6.14】
1.git rebase --onto选项
【注意】:git rebase --onto选项用于多个分支之间的变基
【例子】:
C1->C2->C5->C6(master)
      ->C3->C4->C10(server)
	      ->C8->C9 (client)
git rebase --onto master server client意思是:
(1).找出client分支
(2).找出client分支上与server分支共同祖先之后的提交(C8,C9)
(3).将找出的提交rebase到master上, 效果为:
C1->C2->C5->C6(master)->C8->C9(client)

2.git rebase [targetbranch] [sourcebranch]
利用6月13号的例子:
C0->C1->C2->C4(*experiment)
          ->C3(master)
git rebase master与git rebase master experiment有同样的效果, 但是experiment必须为HEAD,
做之前必须检查或者手动checkout

【实验】:
git rebase [targetbranch] [sourcebranch]
即使targetbranch与sourcebranch都不是当前分支, 执行完后sourcebranch会成为当前分支

3.git rebase的使用原则
只对没有推送的本地修改用rebase清理提交历史, 已推送至别处的修改不要用rebase

【2018.6.15】
1.git利用SHA-1哈希值查找提交commit
git每个commit的完整哈希值是40字节, 如:git log --pretty=oneline -1
423bcdb3107de665ef478a4e5882e1d29884b2c0 [o5_TA] dynamic load on master make ok
git可以为SHA-1值生成简短且唯一的值, 需要加上--abbrev-commit选项
利用该选项输出的哈希值默认7字节, 有时会避免重复, 会增加字节数, 如:git log --pretty=oneline -1 --abbrev-commit
423bcdb [o5_TA] dynamic load on master make ok

2.如何查看分支指向哪个提交
git branch -v可以看到当前仓库每一个本地分支的最后一次提交, 包含了其他信息(哈希值, 提交描述等)

【2018.6.20】
1.精炼版:git reset和git checkout的区别
(1).HEAD在.git中, 是一个普通文件
cat HEAD
ref:refs/heads/MYDEV

(2).分支在.git/refs/heads中, 也是普通文件, 有41字节
cat MYDEV得到提交的哈希值

(3).git checkout commit(如git checkout 5708f6cc)
会生成临时分支5708f6cc, HEAD->临时分支5708f6cc->commit 5708f6cc
而git reset commit是HEAD和分支一起回退

【2018.6.21】
1.引用日志reflog
【注意】:
reflog引用日志只存在于本地仓库, 记录了你自己在本地仓库的操作
如果刚克隆的仓库, 是没有引用日志存在的

【2018.6.25】
1.git fetch
见自己总结的博客
https://blog.csdn.net/liaojunwu/article/details/90381846
https://blog.csdn.net/liaojunwu/article/details/90416892
https://blog.csdn.net/liaojunwu/article/details/90648587

【2018.6.27】
1.git push(依据为git push --help手册)
git push --help
NAME
	git-push - Update remote refs along with associated objects
SYNOPSIS
    git push [<options>] [<repository> [<refspec>...]]
OPTIONS
    <repository>
    The "remote" repository that is destination of a push operation. This parameter can be either a URL (see the section GIT URLS below) or the name of a
    remote (see the section REMOTES below).
	
	-f, --force
    Usually, the command refuses to update a remote ref that is not an ancestor of the local ref used to overwrite it. This flag disables the check. This
    can cause the remote repository to lose commits; use it with care.
	
	<refspec>...
    The format of a <refspec> parameter is an optional plus +, followed by the source ref <src>, followed by a colon :, followed by the destination ref
    <dst>. It is used to specify with what <src> object the <dst> ref in the remote repository is to be updated.
	
	The <src> is often the name of the branch you would want to push, but it can be any arbitrary "SHA-1 expression", such as master~4 or HEAD (see
    gitrevisions(7)).

    The <dst> tells which ref on the remote side is updated with this push. Arbitrary expressions cannot be used here, an actual ref must be named. If
    :<dst> is omitted, the same ref as <src> will be updated.

    The object referenced by <src> is used to update the <dst> reference on the remote side, but by default this is only allowed if the update can
    fast-forward <dst>. By having the optional leading +, you can tell git to update the <dst> ref even when the update is not a fast-forward. This does not
    attempt to merge <src> into <dst>. 
	
	Pushing an empty <src> allows you to delete the <dst> ref from the remote repository.
EXAMPLES
	git push origin +dev:master
    Update the origin repository’s master branch with the dev branch, allowing non-fast-forward updates. This can leave unreferenced commits dangling in
    the origin repository. Consider the following situation, where a fast-forward is not possible:

                           o---o---o---A---B  origin/master
                                    \
                                     X---Y---Z  dev

    The above command would change the origin repository to

                                     A---B  (unnamed branch)
                                    /
                           o---o---o---X---Y---Z  master

    Commits A and B would no longer belong to a branch with a symbolic name, and so would be unreachable. As such, these commits would be removed by a git
    gc command on the origin repository.

【2018.7.6】
1.git stash save/pop/list/drop
【功能】
Use git stash when you want to record the current state of the working directory and the index, 
but want to go back to a clean working directory. 
The command saves your local modifications away and reverts the working directory to match the HEAD commit.
git stash可以保存工作区与暂存区的内容
git stash让工作区的内容保持和HEAD一致

【命令】
git stash list可以列出git stash所保存的内容
stash@{0}表示最新的stash,依次类推,stash@{1}...
【实验】
git stash list列出本地仓库在所有本地分支上保存的内容,最新保存的为stash@{0},依次类推
git stash list
stash@{0}:On STASH_PRACTICE2:1st stash practice
stash@{1}:On STASH_PRACTICE:0st stash test

【命令】
保存:
git stash save -a [<message>]
-a就是--all的意思
[<message>]可选,可以自行填入相关信息
【实验】
git stash save -a "message" 保存工作区与暂存区的内容,并让工作区,暂存区,HEAD保持一致
git stash save -a "0st stash test"
Saved working directory and index state On STASH_PRACTICE: 0st stash test
HEAD is now at xxxx

【命令】
弹出:
git stash pop [--index] [<stash>]
git stash apply [--index] [<stash>]
git stash pop将保存的信息弹出到工作区,[<stash>]默认stash@{0},如需要可以指定
注意:git stash pop会相应消除掉git stash list中的信息
注意:git stash pop可能会冲突
注意:--index选项不仅会将保存信息弹出到工作区,也会将相应信息弹出到暂存区,也可能会冲突
注意:可以将branch1分支上储藏的内容应用到branch2上, branch2的工作区也不一定必须clean, 但要注意冲突
【实验】
git stash pop stash@{n}会将指定储藏内容应用到当前分支的工作区(全部应用到工作区), 并消除stash list中相应的记录
而带上--index选项才是将当初所储藏的完全还原(工作区还工作区, 暂存区还暂存区)

【命令】
git stash apply [--index] [<stash>](推荐用, 因为stash的记录还在)
git stash drop [<stash>]

【小结】
git stash list <==> git stash drop [<stash>]
git stash save -a "message" <==> git stash pop [--index] [<stash>]/git stash apply [--index] [<stash>]

【补充】
git stash save --keep-index "message"
存储所有工作区与暂存区的文件,在 stash list中增加一条记录
但是与git stash save -a "message"不同的是：
-k选项只会让工作区的文件还原与HEAD一致,保留暂存区文件状态
但是书上说的没有保留暂存区文件时错误的,实际上是stash了的,可以在其他分支上去应用

git stash clear

【精炼版】
(1) git stash save -a "message"保存当前分支工作区与暂存区的所有内容, 并将工作区与暂存区还原为与HEAD一致
(2) git stash pop/apply stash@{n}只还原stash@{n}中保存的工作区的内容, 保存的暂存区内容不进行还原
(3) git stash pop/apply --index stash@{n}将还原stash{n}中保存的工作区与暂存区的内容
如果只是stash了工作区内容, 那么apply/pop时不需要带上--index选项
如果stash的内容有暂存区的东西, 那么apply/pop时必须带上--index选项

【2018.11.12】
【精炼版】
git cherry-pick产生冲突的处理方式
git cherry-pick commitID的基本原理是"合入", 将commitID与上一个commit间有变动的文件合入当前分支
所以cherry-pick会产生冲突
cherry-pick会将合并过程中没有冲突的文件放到暂存区, 将有冲突的文件放到工作区并标记(both modified)
解决办法是在工作区解决有冲突的文件, 冲突解决后git add到暂存区
可以运行git cherry-pick --continue或者直接git commit
两者都会生成新的提交

【例子】:
(1).为什么说cherry-pick是将commitID与上一个commit相比有变动的文件合入HEAD
例子见:/lianxi/lianxi_oj/cherrypick_conflict
两个分支master与dev, 共同祖先是origin文件, master在其上添加了master add, dev在其上添加了dev add
dev的第三个提交是新增了文件dev.txt

dev分支:
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# git log --pretty=oneline --abbrev-commit
f386c66 dev add a file
bd184d9 dev add
c8aa518 origin add

master分支:
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# git log master --pretty=oneline --abbrev-commit
5adac1b master add
c8aa518 origin add

那么在master分支上面执行命令git cherry-pick f386c66是会将f386c66的所有文件与HEAD合并呢还是仅仅合并f386c66与bd184d9有差异的文件?
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# git cherry-pick f386c66b93b2e749c2c90d6bdeb200d6810b50a5
[master 1f2f363] dev add a file
 0 files changed
 create mode 100644 dev.txt
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# ls -la
总用量 16
drwxr-xr-x 3 root root 4096 Mar 18 20:29 .
drwxr-xr-x 8 root root 4096 Mar 18 20:10 ..
-rw-r--r-- 1 root root    0 Mar 18 20:29 dev.txt
drwxr-xr-x 8 root root 4096 Mar 18 20:29 .git
-rw-r--r-- 1 root root   22 Mar 18 20:28 origin.txt
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# 

可以看到上述结论成立

(2).cherry-pick有冲突的行为
我们将dev的最近两次提交合并成一个提交, 修改了origin.txt也增加了dev.txt
我们将master回退一个提交
这时在master上cherry-pick dev的最新提交会怎么样呢?
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# git cherry-pick fb04f8cf990c4b4f05186eba5ac98f92b90add87
error: could not apply fb04f8c... test conflict
hint: after resolving the conflicts, mark the corrected paths
hint: with 'git add <paths>' or 'git rm <paths>'
hint: and commit the result with 'git commit'
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# 
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict# git status
# On branch master
# Changes to be committed:
#
#	new file:   dev.txt
#
# Unmerged paths:
#   (use "git add/rm <file>..." as appropriate to mark resolution)
#
#	both modified:      origin.txt
#
root@ubuntu:/lianxi/lianxi_oj/cherrypick_conflict#

可以看到, dev.txt没有冲突, 放到暂存区, 而origin.txt有冲突被标记
按照提示解决冲突后放入暂存区再commit即可

【2018.11.13】
1.git revert commitID原理以及冲突解决
由于commit已经上库, 不能删除某一个commit, 只有形成新的commit, 不包含想要去除的commit
git revert commitID, commitID就是想要删除的版本
git会形成一次新提交, 不包含commitID的修改

2.git revert commitID冲突
git revert commitID会将commitID^与HEAD进行合并, 所以会产生冲突
git会把没有冲突的文件放到暂存区, 有冲突的文件标记在工作区
解决冲突后git add到暂存区, 运行git commit或者git revert --continue会生成新提交
【注意】:
git revert有个-n选项(--no-commit), 不会产生新提交, 而是把合并出的内容填到工作区与暂存区

【实验】:/lianxi/lianxi_oj/revert_conflict
提交顺序和文件内容如下, 可以看到revert发生了冲突, 也可以看到hint的解决提示:git add->git commit/git revert --continue

root@ubuntu:/lianxi/lianxi_oj/revert_conflict# git log --pretty=oneline --abbrev-commit
ab0dd7d 4th add
08be042 3rd add
fb7587e 2nd add
7e63828 1st add
f5e9cab origin add
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# cat test.txt 
origin add
1st add
2nd add
3rd add
4th add
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# 
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# 
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# 
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# 
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# git revert fb7587e
error: could not revert fb7587e... 2nd add
hint: after resolving the conflicts, mark the corrected paths
hint: with 'git add <paths>' or 'git rm <paths>'
hint: and commit the result with 'git commit'
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# git status
# On branch master
# Unmerged paths:
#   (use "git reset HEAD <file>..." to unstage)
#   (use "git add/rm <file>..." as appropriate to mark resolution)
#
#	both modified:      test.txt
#
no changes added to commit (use "git add" and/or "git commit -a")
root@ubuntu:/lianxi/lianxi_oj/revert_conflict# 

1 origin add
2 1st add
3 <<<<<<< HEAD
4 2nd add
5 3rd add
6 4th add
7 =======
8 >>>>>>> parent of fb7587e... 2nd add
通过冲突文件可以看出, 是把fb7587e之前的提交与HEAD相合并的

【2019.5.9】
1.repo manifests简单解析
在实际项目工程上,git有很多仓库, 在创建仓库或者下载代码时需要指定一些仓库的路径, 属性等等
就需要用到repo manifests中的东西
下面引用一段实际用到的manifests, 加以简单的说明
参考博客如下:https://blog.csdn.net/shift_wwx/article/details/19557031
<manifest>

/*remote是指远程仓库的名字,一般的远程仓库都是origin, 但是这里是middleware/reponame
 *这个remote就指一个远程仓库的属性, 如果存在多个remote, 需要指定一个default的属性
 *下面的每个project都是一个需要clone的仓库, 如果指定了remote, 那么用相应remote的属性
 *没有指定就用default的属性
 *fetch是指使用这个远程仓库的每个project的GIT URL前缀
 *完整git URL:${remote_fetch}/${project_name}.git
 *review是指gerrit的网址*/
<remote name="middleware/reponame"
        fetch="http://netaddress"
		review="http://revie_netaddress"/>

/*project代表每一个需要clone的本地仓库
 *name用于生成clone用的完整URL, 格式为:${remote_fetch}/${project_name}.git
 *path为这个本地仓的名字, code仓, code/src仓等等
 *remote指定了clone这个本地仓库需要用到的远程仓库, 如果没有指定, 那么用default属性里的
 *revision指远程仓库的提交, 可以是分支名, HASH值,表示从远程仓库middleware/reponame的master分支clone
 *sync就是同步属性*/
 /*【2019/5/10更正】
  *原以为remote是远程仓库, 而本地仓库只是从remote远程仓库中的revision分支clone下来的
  *原以为是很多本地仓库对一个远程仓库的不同分支的关系
  *询问了涛哥以后, 知道remote是指一个服务器, 而project里面的name才是服务器上的仓, revision是服务器仓的分支或提交
  *确定了服务器, 仓库, 提交, 才能确定clone的最终路径
  *实际上也是很多本地仓库对远程服务器很多仓库的关系*/
<project name="LIS/LIS_LTB/GIT/middleware/reponame"
         path="code"
		 remote="middleware/reponame"
		 revision="master"
		 sync-c="false"/>
		 
<project name="LIS/LIS_LTB/GIT/middleware/reponame/src"
         path="code/src"
		 remote="middleware/reponame"
		 revision="dev"
		 sync-c="false"/>

/*include用于导入另一个xml文件, clone其它仓用*/ 
<include name="LISLTBLinuxxxx.xml"/>

</manifest>

A description of the elements and their attributes follows.

【Element manifest】

The root element of the file.

【Element remote】

One or more remote elements may be specified. Each remote element specifies a Git URL shared by one or more projects and (optionally) the Gerrit review server those projects upload changes through.

Attribute name: A short name unique to this manifest file. The name specified here is used as the remote name in each project's .git/config, and is therefore automatically available to commands likegit fetch,git remote,git pull and git push.

Attribute alias: The alias, if specified, is used to override name to be set as the remote name in each project's .git/config. Its value can be duplicated while attributename has to be unique in the manifest file. This helps each project to be able to have same remote name which actually points to different remote url.

Attribute fetch: The Git URL prefix for all projects which use this remote. Each project's name is appended to this prefix to form the actual URL used to clone the project.

Attribute review: Hostname of the Gerrit server where reviews are uploaded to byrepo upload. This attribute is optional; if not specified thenrepo upload will not function.

【Element default】

At most one default element may be specified. Its remote and revision attributes are used when a project element does not specify its own remote or revision attribute.

Attribute remote: Name of a previously defined remote element. Project elements lacking a remote attribute of their own will use this remote.

Attribute revision: Name of a Git branch (e.g. master orrefs/heads/master). Project elements lacking their own revision attribute will use this revision.

Element manifest-server

At most one manifest-server may be specified. The url attribute is used to specify the URL of a manifest server, which is an XML RPC service that will return a manifest in which each project is pegged to a known good revision for the current branch and target.

The manifest server should implement:

GetApprovedManifest(branch, target)

The target to use is defined by environment variables TARGETPRODUCT and TARGETBUILDVARIANT. These variables are used to create a string of the form $TARGETPRODUCT-$TARGETBUILDVARIANT, e.g. passion-userdebug. If one of those variables or both are not present, the program will call GetApprovedManifest without the target paramater and the manifest server should choose a reasonable default target.

【Element project】

One or more project elements may be specified. Each element describes a single Git repository to be cloned into the repo client workspace.

Attribute name: A unique name for this project. The project's name is appended onto its remote's fetch URL to generate the actual URL to configure the Git remote with. The URL gets formed as:

${remotefetch}/${projectname}.git

where ${remotefetch} is the remote's fetch attribute and ${projectname} is the project's name attribute. The suffix ".git" is always appended as repo assumes the upstream is a forrest of bare Git repositories.

The project name must match the name Gerrit knows, if Gerrit is being used for code reviews.

Attribute path: An optional path relative to the top directory of the repo client where the Git working directory for this project should be placed. If not supplied the project name is used.

Attribute remote: Name of a previously defined remote element. If not supplied the remote given by the default element is used.

Attribute revision: Name of the Git branch the manifest wants to track for this project. Names can be relative to refs/heads (e.g. just "master") or absolute (e.g. "refs/heads/master"). Tags and/or explicit SHA-1s should work in theory, but have not been extensively tested. If not supplied the revision given by the default element is used.

Attribute groups: List of groups to which this project belongs, whitespace or comma separated. All projects belong to the group "default", and each project automatically belongs to a group of it's name:name and path:path. E.g. for , that project definition is implicitly in the following manifest groups: default, name:monkeys, and path:barrel-of.

【Element annotation】

Zero or more annotation elements may be specified as children of a project element. Each element describes a name-value pair that will be exported into each project's environment during a 'forall' command, prefixed with REPO__. In addition, there is an optional attribute "keep" which accepts the case insensitive values "true" (default) or "false". This attribute determines whether or not the annotation will be kept when exported with the manifest subcommand.

【Element remove-project】

Deletes the named project from the internal manifest table, possibly allowing a subsequent project element in the same manifest file to replace the project with a different source.

This element is mostly useful in the local_manifest.xml, where the user can remove a project, and possibly replace it with their own definition.

【Element include】

This element provides the capability of including another manifest file into the originating manifest. Normal rules apply for the target manifest to include- it must be a usable manifest on it's own.

Attribute name; the manifest to include, specified relative to the manifest repositories root.

Local Manifest

Additional remotes and projects may be added through a local manifest, stored in$TOP_DIR/.repo/local_manifest.xml.

For example:

$ cat .repo/local_manifest.xml
Users may add projects to the local manifest prior to a repo sync invocation, instructing repo to automatically download and manage these extra projects.
		 

		
		



