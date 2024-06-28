using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using TMPro;
using UnityEngine;

public class TimeLimitScript : MonoBehaviour
{
    //テキストメッシュプロ用のpublic変数
    public TextMeshProUGUI TimeText;

    //int型変換用変数
    public float TimeLimit = 120.0f;

    private void Update()
    {
        //時間をカウントする
        TimeLimit -= Time.deltaTime;

        //時間を表示する
        //TimeText.text = TimeLimit.ToString("f1") + "秒";

        if(TimeLimit <= 0.0f)
        {
            SceneManager.LoadScene("ResultScene");
        }
    }
}
