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
