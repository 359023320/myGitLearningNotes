【2018.5.5】
1.gitbub是什么
github相当于一个远程仓库,一般先在github配置一个仓库,通过git clone命令到本地
【注意】
在git clone完毕之后进入该仓库的.git隐藏文件夹
vim config
在[core]下面添加editor = vim
目的是在git commit的时候默认用vim工具打开
【注意】
cd ~
vim .gitconfig
里面记录了name与email

2.本地分支/远程分支
(1)远程分支在git clone之后产生
(2)远程分支在本地仓库而不是远程仓库
(3)远程分支反映了远程仓库的状态(并不是最新的状态,而是上一次和它通信的状态,远程仓库最新的状态可能已经更改)
(4)不能git checkout到远程分支
如有一个远程分支名为o/master,指向提交commit1
如果git checkout o/master,会使HEAD进入分离的状态,等同于git checkout commit1
但是可以git reset --hard 到远程分支上
(5)远程分支命名规范
<remote name>/<branch name>
remote name指远程仓库的名字,branch name才是此分支的名字
【注意】
git默认的远程仓库名是origin,在git clone时已经定好了
vim .git/config可以看到:
[branch "master"]
	remote = origin
	merge = refs/heads/master
	
【2018.5.7】
1.与git config有关的文件
git config --system:影响的是/etc/gitconfig文件,系统中对所有用户都有效的配置
git config --global:影响的是~/.gitconfig文件,对当前用户下所有仓库都有效
git config         :影响的是./git/config文件,仅仅对当前仓库有效
所以在配置文本编辑器的时候可以用:
git config --global core.editor vim

2.git fetch
(1)远程分支反映了远程仓库的状态(并不是最新的状态,而是上一次和它通信的状态,远程仓库最新的状态可能已经更改)
(2)git fetch下载本地仓库与远程仓库中的差异提交,并更新远程分支
(3)git fetch仅仅是更新远程分支,并不会影响本地分支

【2018.5.10】
1.git fetch的参数
(1)
【例子】
git fetch origin foo
origin:远程仓库的名字
foo:远程仓库中分支名
到远程仓库origin中获取foo分支上本地不存在的提交,下载到本地并更新远程分支o/foo
即仅仅更新o/foo远程分支
【注意】
git fetch的大原则就是只更新远程分支
(2)
【例子-git fetch的小众用法】
git fetch origin foo~1：bar
origin:远程仓库的名字
foo~1:source:下载源:远程仓库中foo分支HEAD的父提交
bar:destination:下载目标地址:本地仓库中的bar分支
到远程仓库origin中获取foo分支HEAD的父提交,将本地不存在的提交下载到本地仓库bar分支,并更新bar分支
【注意】
这里git fetch更新了本地分支
若bar分支不存在,则会本地新建bar分支
bar不能是当前所在的分支(当前状态不能为*bar)
bar分支更新后也不是*bar状态
(3)
【例子】
git fetch
到远程仓库中找到每个分支,获取每个分支上本地不存在的提交下载到本地并更新本地的所有远程分支
【注意】
git fetch不带参数更新本地所有的远程分支

2.git pull
(1)可以对远程分支做的操作:
git cherry-pick o/master
git rebase o/master
git merge  o/master
(2)git pull:先更新远程分支,再将其合入本地分支
git pull == git fetch + git merge <just-fetch-branch>
即:先更新远程分支,再将其merge到本地分支上去
(3)git pull的参数
