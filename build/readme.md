# Animation

### 作業環境: 

    Windows10 / Visual Studio 2015 / OPENGL

### 方法說明:

render中 主要利用

- glm::scale() : 
    - 控制矩陣的大小縮放
    - glm::scale(mPosition, glm::vec3(0.8f, 0.8f, 0.8f));
    - 也就是對mPosition做 x,y,z 縮放0.8倍
- glm::translate() : 
    - 控制矩陣的平移
    - glm::translate(mPosition, glm::vec3(0, k, 0));
    - 也就是對mPosition的y軸做平移k單位
- glm::rotate() : 
    - 控制矩陣的旋轉
    - glm::rotate(mPosition, (float)n, glm::vec3(0.0f, 1.0f, 0.0f));
    - 也就是對mPosition 以y軸為中心做旋轉n單位

並由i來判斷每個object的行為模式
連接各物體的方式為 以父點座標當為基準 assign給子點 再對子點進行操作 
最後儲存子點座標 若子點為leaf 則不必儲存

### 操作說明:

主要以鍵盤操作

=====  控制部位 =====

**A** : 控制左前大腿
**S** : 控制右前大腿
**D** : 控制左後大腿
**F** : 控制右後大腿

**Z** : 控制左前小腿
**X** : 控制右前小腿
**C** : 控制左後小腿
**V** : 控制右後小腿

**H** : 控制頭部
**T** : 控制尾巴
**B** : 控制身體

**Q** : 控制右翼
**W** : 控制左翼

===== 以上皆設有活動範圍 ===== 

**space** : 奔跑模式(開/關) 

===== 視角轉換 =====

**N** : 自動旋轉(開/關)
**J** : 視角右移
**L** : 視角左移
**I** : 放大
**K** : 縮小
**滾輪** : 放大/縮小

方向鍵

**left** : 向左旋轉
**right** : 向右旋轉
**up** : 向上旋轉
**down** : 向下旋轉