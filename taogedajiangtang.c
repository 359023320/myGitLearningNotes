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