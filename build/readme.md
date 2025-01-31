# Lighting Animation

### **作業環境** 

    Windows10 / Visual Studio 2015 / OPENGL

----

### **方法說明**

以下主要對render做說明

#### **model的控制**

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

並由i來判斷每個object的行為模式<br>
連接各物體的方式為 以父點座標當為基準 assign給子點 再對子點進行操作 <br>
最後儲存子點座標<br>

#### **shader說明**

先用 uniform <br>
把以下讀入 fragment<br> 
* ObjColor        : 物體本身的顏色，這裡設定成灰色
* ambientColor    : 環境光顏色，這裡設定成白光
* lightPos        : 光源位置
* viewPos         : camera位置
* lightColor      : 光的顏色(針對diffuse,specular使用)

先將讀進來的向量做normalize<br>
利用公式算出halfwayDir,reflectVec,diffuse,specularAmount,specular<br>
最後搭配texture輸出<br>
註: 此部分objColor,與texture圖片檔皆為灰色，可能看不太出來，但實測發現加了texture會更暗一些<br>

----

### **操作說明**:

主要以鍵盤、滑鼠操作 

####  **控制部位**

**A** : 控制左臂 <br>
**S** : 控制右臂 <br>
**D** : 控制左大腿 <br>
**F** : 控制右大腿 <br>

**Z** : 控制左手 <br>
**X** : 控制右手 <br>
**C** : 控制左腳 <br>
**V** : 控制右腳 <br>

**H** : 控制頭部 <br>

===== 以上皆設有活動範圍 ===== <br>

**space**   : 奔跑模式(開/關) <br>
**O**       : model左移 <br>
**P**       : model右移 <br>
**G**       : 光源移動(繞著model) <br>

#### **視角轉換**

**N** : 自動旋轉(開/關) <br>
**J** : 視角右移 <br>
**L** : 視角左移 <br>
**I** : 放大 <br>
**K** : 縮小 <br>
**滾輪** : 放大/縮小 <br>

#### **方向鍵控制旋轉**

**left** : 向左旋轉 <br>
**right** : 向右旋轉 <br>
**up** : 向上旋轉 <br>
**down** : 向下旋轉 <br>

#### **phong / blinn-phong 轉換**

**T**   : phong <br>
**Y**   : blinn-phong <br>

#### **燈光顏色轉換**

**Q**   : 隨時間換色 <br>
**W**   : 固定色 <br>

----

demo link : https://youtu.be/MtNk9dqrdn0