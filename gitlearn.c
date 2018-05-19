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

【2018.5.14】
1.git pull的参数
我们知道:git fetch有三种形式:git fetch;git fetch origin foo;git fetch origin foo~:bar
git pull origin foo <==> git fetch origin foo + git merge o/foo
git pull origin bar~1:bugFix <==> git fetch origin bar~1:bugFix + git merge bugFix

2.git push
(1)git push负责将你的变更上传到指定的远程仓库,并在远程仓库上合并你的新提交记录
git push实际上会更新远程仓库的分支,然后也会更新存在于本地的远程分支
(2)远程跟踪
【思考】:为什么在git push的时候默认会把master分支上的提交更新到远程仓库的master分支并更新本地的远程分支呢?
@这种关系由分支的"remote tracking"属性决定的
@该属性是由git clone时默认产生的,git clone时本地两个分支:本地的master与远程分支o/master
@该属性的手动设置方法两种:
<1>git checkout -b totallyNotMaster o/master
创建并切换到totallyNotMaster分支上,并让它跟踪o/master
这样就可以把totallyNotMaster分支上的更新推送到远程仓库的master分支并更新本地远程分支o/master了
这种方法适用于本地新建分支的情况
<2>git branch -u o/master foo
foo分支跟踪o/master了
这样就可以把foo分支上的更新推送到远程仓库的master分支并更新本地远程分支o/master了
这种方法适用于本地存在foo分支的情况
但是git branch -u o/master foo并不会将当前分支切换到foo分支,需要checkout命令手动切换
若当前分支就在foo分支上,则命令可简化为git branch -u o/master
【注意】:虽然git branch -u o/master foo指定了foo分支去跟踪o/master,但是若没有checkout到foo分支上,git push仍会将默认masterpush到远程库
checkout到foo分支上后再git push则只pushfoo分支上的更新了
【理解】:git是根据当前检出分支的属性来决定push的仓库及目的地的
最初git clone时将master设置为跟踪o/master
可以手动根据git branch -u o/master foo将foo分支设置为跟踪o/master
但是此时并没有更改本地master分支的跟踪属性
所以*master时git push会push master上的更新到远程仓库
所以*foo时git push会push foo上的更新到远程仓库


【2018.5.15】
1.git push的参数
(1)
git push <remote> <place>
【注意】:如上带参数的git push会忽略remote tracking的影响
place参数告诉了git提交记录来自于本地仓库分支<place>,push目的地为远程仓库分支<place>
place参数同时指明了提交的来源与去向
【例子】:
git push origin master
将本地仓库master分支上的更新push到远程仓库origin上的master分支,并更新本地的远程分支o/master
此时就算是HEAD为test分支,且test分支的remote tracking属性为o/master也不会push test上的更新
但是整个过程中HEAD的位置仍是test分支
(2)
git push origin <source>:<destination>
【注意】:如上带参数的git push会忽略remote tracking的影响
source参数告诉git提交的来源:本地仓库某分支
destination参数告诉git提交的目的地:远程仓库某分支
【例子】:
git push origin foo^:master
git push origin master:newbranch
若此时远程仓库中没有newbranch,则会在远程仓库中新建newbranch分支,更新newbranch分支,同时在本地仓库中新建o/newbranch分支并更新

【2018.5.19】
1.偏移的提交历史
什么是偏移的提交历史
本地仓库:C0->C1(o/master)->C3(*master)
远程仓库:C0->C1->C2(master)
在本地仓库更新的时候可能只有C1(远程仓库只有C1存在),这时候本地仓库在C1的基础上进行开发出C3
在这期间远程仓库中可能有别人的提交,C2
这时如果git push的话由于本地仓库与远程仓库的基础不一致,会导致push不成功
解决思路是先更新本地仓库,再进行push
先git fetch更新本地仓库中的远程分支o/master
再git rebase o/master
这时就变为:C0->C1->C2(o/master)->C3(*master)
最后进行git push即可完成
【注意】:
上述操作:git fetch; git rebase o/master可用一条命令取代
<==>git pull --rebase(git fetch; git rebase <just-fetch-branch>)

2.git fetch与git push中<source>参数的诡异用法
如何删除远程仓库中的分支与本地仓库中的远程分支?
在本地仓库中新建分支的方法
没有source的source:
【例子】
git fetch origin :bugFix
只在本地仓库中创建一个新分支bugFix(HEAD并没有指向bugFix)
相当于git branch bugFix
git push  origin :side
删除远程仓库origin中的side分支,同时删除本地仓库中的远程分支o/side
