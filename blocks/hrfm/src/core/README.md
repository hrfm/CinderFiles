# app

cinder の App を継承したファイル群  
ライブラリというよりも若干フレームワーク寄りのクラスが多い。

# cv

OpenCV 周りの処理を毎回書くのが面倒なのでラップしたクラス群。

# display

Flash/AS3 の DisplayObjectTree を参考に作った描画ツリー用クラス群。

# events

ライブラリ全体でよく使用するイベントをまとめたクラス  
イベント周りの実装は cpplib の Event で定義している。

# geom

Vector や Point の geometric な処理をまとめ``ようとしてる``ディレクトリ

# gl

Shader / FBO / VBO まわりを扱う時に自分がよくやることを  
まとめて簡単に書けるようにしようと試みているものが入っているディレクトリ。

# io

主に Capture / Audio / OSC のやりとりの簡略化を行うためのクラス群。

# text

文字列というか Font の読み込みとかを簡単にするためのクラス群。

# utils

ファイルの読み込み、時間管理、テクスチャのグローバル管理などの便利クラス群。
