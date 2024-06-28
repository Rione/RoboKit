# Windowsの諸注意

WindowsにGitが入っている必要があります．Gitのインストールをしてください．(自分で調べてください)

### Gitが入っているかの確認

ターミナルで次のコマンドを打ってみてください．

```
git help
```

と打って反応があればGitが入っている証拠です．

- ターミナルで期待される反応はこんな感じです．
    
    ```
    git help
    usage: git [--version] [--help] [-C <path>] [-c <name>=<value>]
               [--exec-path[=<path>]] [--html-path] [--man-path] [--info-path]
               [-p | --paginate | -P | --no-pager] [--no-replace-objects] [--bare]
               [--git-dir=<path>] [--work-tree=<path>] [--namespace=<name>]
               [--super-prefix=<path>] [--config-env=<name>=<envvar>]
               <command> [<args>]
    
    These are common Git commands used in various situations:
    
    start a working area (see also: git help tutorial)
       clone     Clone a repository into a new directory
       init      Create an empty Git repository or reinitialize an existing one
    
    work on the current change (see also: git he
    ```
    

### RoboKitのファイルの初期化時にエラーが出る(longpathエラー)

- WindowsのGitはMacと違い，longpath = falseになっているので，trueにしましょう
- WindowsのGitの入っているPCで以下のコマンドを実行してください．

```
git config --system core.longpaths true
```
