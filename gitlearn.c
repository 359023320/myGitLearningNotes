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
