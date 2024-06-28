using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class BreakCheck : MonoBehaviour
{
    //破壊可能オブジェクトを入れる配列
    private GameObject[] BreakObjects;

    //テキストメッシュプロ
    public TextMeshProUGUI BreakPersent;

    //パーセンテージを引くよう
    public static float Persent = 0.0f;
    float S_Object = 0;
    bool check = false;
    public static float PersentSum = 0.0f;

    //スライダー用
    Slider BreakSd;
    public int sliderMax = 100;
    public int sliderMin = 0;

    // Start is called before the first frame update
    void Start()
    {
        BreakSd = GetComponent<Slider>();
        BreakSd.maxValue = sliderMax;
        BreakSd.minValue = sliderMin;
    }

    // Update is called once per frame
    void Update()
    {
        //オブジェクトのタグの設定
        TagCheck("Object");

        if (!check)
        {
            //初期のオブジェクト数を保持
            S_Object = (float)BreakObjects.Length;
            //一個当たりの破壊ポイントの計算
            Persent = 100/ S_Object;
            check = true;
        }
    }

    //タグが一致していたら
    public void TagCheck(string TagName)
    {
        //配列にタグが付いているものを入れる
        BreakObjects = GameObject.FindGameObjectsWithTag(TagName);

        //パーセンテージの計算
        PersentSum = (Persent * (float)BreakObjects.Length);

        PersentSum = 100.0f - PersentSum;

        //タグのついたオブジェクトの個数が０個なら
        if (BreakObjects.Length == 0)
        {
            Debug.Log("全部破壊");
            SceneManager.LoadScene("ResultScene");
        }
        else
        {
            if(PersentSum >= 99.1f)
            {
                PersentSum = 100;
                //パーセンテージ表示
                BreakPersent.text = (int)PersentSum + "%";
            }
            else
            {
                BreakPersent.text = (int)PersentSum + "%";
            }
        }

        BreakSd.value = PersentSum;
       // Debug.Log(PersentSum);
    }

    public float Score()
    {
        return PersentSum;
    }
}
