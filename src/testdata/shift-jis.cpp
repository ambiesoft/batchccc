Skip to content
ブーログ
Just another WordPress site
TOP
  
ST31000524ASのファームウェアのアップデート
 2020年1月6日   admin
注意：これをやると壊れます。

Dellのinspiron-620sというマシンをWindows10で使っていたのだが、それに積んでいるHDDがLED点灯状態になる問題が発生した（普通は点滅する）。この現象に伴ってファイルが読めなくなる（読み込むと固まる）などの問題が発生したため、HDDのファームウェアのアップデートをしてみることにした。

HDDはST31000524ASという型番、検索するとDellでファームウェアのアップデートが提供されていたので実行してみることにした。


圧縮ファイルに含まれる、B7000100.exeというファイルを実行すると、おそらくファームウェアを適用したあとに再起動したのだが、HDDからブートできなくなってしまった。幸いこのマシンにはSSDも積んでいてそこからもブートできるようになっていたので、起動することはでき、HDDの中身も無事みたいだが、HDDが１つしかない場合は、Windowsを再インストールすることになると思う。

SSDからHDDで起動したあとにEasyBCDを使って再びHDDにBCDを書き込んで、HDDからブートできるように戻すことができた。



このソフトはブート時に使ったBCDの情報を表示するようだ。ここでE:\になっているのはシステムで予約済みの領域で、bootやEFIなどのフォルダとbootmgr,BOOTNXTなどのファイルがある。ここにBCDを書き込むことでもとに戻すことができた。

LED点灯問題は今のところ起きていないが、しばらく様子を見ないとわかない。

更新前のファームのバージョンは確認しなかったが、更新後の情報はHWiNFO64でみると以下のようになっていた。


追記
上記のファームウェアアップデートをしても同じ問題が再現したのでファームウェアの問題ではないようだった。その後色々しらべるとSEAGATEからディスク診断修復ツールであるSeaToolsというものがあるとわかったのでこれを試してみることにした。



ベーシックテストでもエラーはでなかったのでHDDにエラーはないものと思われる。

その後いろいろ調べてみると、同じような問題でDVDドライブがおかしくなるとLED点灯が起こるとあったので調べてみると問題が発生しているときはDVDドライブが見えなくなっていることがわかった。起動時から見えないときもあるし、起動してしばらくしてから見えなくなることもある。これが原因かもしれないので更に調査してみようと思う。

Posted in 未分類
Post navigation
コマンドの時間を計測する
Leave a Reply
Your email address will not be published. Required fields are marked *

Comment 

Name * 

Email * 

Website 

 Save my name, email, and website in this browser for the next time I comment.

日本語が含まれない投稿は無視されますのでご注意ください。（スパム対策）

Search for: 
Search …
 
Follow Us
    
Recent Posts
ST31000524ASのファームウェアのアップデート
コマンドの時間を計測する
USBハードディスクを使っていないとき電源を切る設定
stringのvectorをスペースで区切って１つのstringにする
Win32APIが呼ばれたときブレークポイントを設定する
Recent Comments
vmware playerで最大化したときの最上部のバーを消す  ブーログ on Fedoraのvmware ServerでゲストOS上でアンダーバーが入力できない
テストコメンター on MySQLのデータベースが派手に壊れた場合の修復覚書
Archives
January 2020
December 2019
September 2019
August 2019
July 2019
June 2019
May 2019
April 2019
March 2019
February 2019
October 2018
September 2018
July 2018
June 2018
May 2018
April 2018
March 2018
February 2018
January 2018
December 2017
November 2017
October 2017
September 2017
August 2017
July 2017
April 2017
March 2017
February 2017
January 2017
December 2016
November 2016
October 2016
June 2016
May 2016
April 2016
March 2016
December 2015
November 2015
October 2015
September 2015
August 2015
July 2015
June 2015
March 2015
February 2015
January 2015
December 2014
November 2014
October 2014
July 2014
June 2014
May 2014
April 2014
March 2014
December 2013
November 2013
September 2013
August 2013
February 2013
January 2013
December 2012
November 2012
October 2012
April 2012
March 2012
January 2012
September 2011
August 2011
July 2011
June 2011
May 2011
April 2011
March 2011
February 2011
December 2010
November 2010
May 2010
March 2010
January 2010
October 2009
September 2009
July 2009
June 2009
May 2009
April 2009
February 2009
January 2009
December 2008
November 2008
October 2008
September 2008
August 2008
July 2008
June 2008
May 2008
April 2008
March 2008
February 2008
January 2008
November 2007
August 2007
May 2007
April 2007
January 2007
Categories
axios
C#
C++
chrome
Command
eclipse
git
http
Linux
movie
MySQL
node
PHP
PowerShell
python
Qt
Visual Studio
Visual Studio Code
VMWare
Vue
Webアプリ
Windows
WSL
アプリ
未分類
Meta
Log in
Entries feed
Comments feed
WordPress.org
Proudly powered by WordPress	| Theme: Ogee | By: Phoenix Web Solutions