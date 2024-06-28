using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ChangeImage : MonoBehaviour
{
    public Sprite _on;
    public Sprite _off;
    private bool flg = true;

    public void changeImage()
    {
        //イメージを取得
        var img = GetComponent<Image>();
        //画像の情報をフラグで管理
        img.sprite = (flg) ? _on : _off;
        flg = !flg;

        //画像が選択されたら
        if(_on)
        {
            //効果音
            GetComponent<AudioSource>().Play();
            //赤色に染める
            img.color = Color.red;
            //Cursor.visible = false;
        }
    }
}

