using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class GimmicPaper : MonoBehaviour
{
    //プレイヤーのコントローラー
    GameObject player;
    //テキストメッシュ
    Text text;

    void Start()
    {
        //プレイヤーの情報取得
        player = GameObject.FindGameObjectWithTag("Player");
        //テキストの取得
        text = GetComponent<Text>();
    }

    // Update is called once per frame
    void Update()
    {
        //テキストの変換
        text.text = "×" + player.GetComponent<PlayerController>().paperNum;
    }
}
