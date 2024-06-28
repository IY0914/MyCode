using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;

public class ScoreCountUp : MonoBehaviour
{
    //スコア
    int score = 0;
    //テキストメッシュ
    TextMeshProUGUI textMeshProUGUI;
    //移動
    float time = 0.0f;

    //上限値と下限値の設定
    Vector2 min = new Vector2(0, 0);
    Vector2 max = new Vector2(100, 0);

    // Start is called before the first frame update
    void Start()
    {
       

        textMeshProUGUI = GetComponent<TextMeshProUGUI>();
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;

        score = (int) Vector2.Lerp(min, max, time).x;

        textMeshProUGUI.text = score.ToString();
    }
}
