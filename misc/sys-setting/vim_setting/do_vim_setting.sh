#!/bin/sh

rm -rf ~/.vim
rm -f ~/.vimrc
cp -rf vim ~/.vim
cp -f vimrc ~/.vimrc

rm -f ~/.gvimrc
echo "colors torte" >> ~/.gvimrc

echo \
'------------------------------------------------------------
Installed plugins for vim:
	+ taglist.vim
	+ cscope_macros.vim
	+ NERD_tree.vim
	+ snipMate.vim
	Any issure, pls see ~/.vim/doc/ for detials
------------------------------------------------------------'