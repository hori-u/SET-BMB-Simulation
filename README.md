# SET-BMB-Simulation

放送通信融合環境における映像品質を考慮した配信スケジュールのシミュレーション

# Summary

近年,ストリーミング配信の普及に伴い,音声や映像といった連続メディアデータを受信しながら再生する放送通信融合環境に対する注目が高まっている. 放送通信融合環境では,サーバは放送方式と通信方式の両方を用いて,動画データを複数のセグメントに分割してユーザに配信する.
また, スケジューリング手法を用いることで,ユーザはデータの再生時に発生する再生中断時間を短縮する. 一方で,多くの動画配信プラットフォームでは課金型配信サービスが導入されている.
課金型配信サービスでは,一定額の利用料を定期的に支払った非課金ユーザは課金ユーザとなり,視聴中の再生時間の短縮や動画の映像品質の向上といったユーザの視聴意欲を向上させる機能を利用できる. 
既存のスケジューリング手法を課金型配信に適応する場合,課金の有無に応じた再生中の映像品質の変更を考慮おらず,再生中断時間を十分に短縮できない. 
本研究では,放送通信融合環境においてユーザの課金状況を考慮した複数画質動画配信のスケジューリング手法である Shortest Extra Time on Billing–based Multiple quality Broadcasting (SET-BMB) 法を提案する. 
提案手法では,従来の放送方式,通信方式,すべてのセグメントを持つスーパーノードによる端末伝送型方式でセグメントを配信する.
また,課金ユーザのみに対して,映像品質か異なる複数の動画間で差分となるセグメントを持つスーパーノードが端末間でセグメントを配信するようにスケジューリングすることで,再生中断時間を抑えたまま高画質動画の再生時に発生する再生中断時間を短縮する.
提案手法を用いた評価では,既存手法と比較して,ユーザによる再生中断時間をそれぞれ短縮することを確認した.

# Installation

* git clone :
  ```
  git clone https://github.com/hori-u/FPSGameProject.git
  ```

# Usage

* BMB-1min-2.cをコンパイルして実行

# Paper

* 放送通信融合環境における映像品質を考慮した配信スケジュールの作成手法の提案 : 
https://ipsj.ixsq.nii.ac.jp/ej/?action=repository_uri&item_id=234000&file_id=1&file_no=1

# Publications

* 第199回DPS研究発表会 :
https://www.ipsj.or.jp/kenkyukai/event/dps199mbl111its97.html

# Author

* 作成者 : 堀 悠人（Hori Yuto）
* 所属 : 岡山大学大学院環境生命自然科学研究科（Graduate School of Environment, Life, Natural Science and Technology, Okayama University）
* E-mail : pnjl7b2l@s.okayama-u.ac.jp
