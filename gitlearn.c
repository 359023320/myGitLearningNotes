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
